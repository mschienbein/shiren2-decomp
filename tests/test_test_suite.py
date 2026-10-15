"""Exercise real subprocess workers, retention helpers, and runner protocol failures."""
from __future__ import annotations

import argparse
import contextlib
import io
import json
import os
import queue
import re
import signal
import subprocess
import sys
import tempfile
import textwrap
import threading
import time
import unittest
from collections import Counter
from pathlib import Path
from unittest.mock import patch

_TOOLS = Path(__file__).resolve().parents[1] / "tools"
sys.path.insert(0, str(_TOOLS))
import test_suite


_SUCCESS = '''
import json
import os
import tempfile
import unittest
from pathlib import Path

ROOT = Path(os.environ["SUITE_FIXTURE_ROOT"])

def record(name):
    tmp = Path(tempfile.gettempdir()).resolve()
    assert all(Path(os.environ[key]).resolve() == tmp for key in ("TMPDIR", "TMP", "TEMP"))
    with tempfile.NamedTemporaryFile() as handle:
        assert Path(handle.name).parent.resolve() == tmp
    (ROOT / (name + ".json")).write_text(json.dumps({"pid": os.getpid(), "tmp": str(tmp)}))

class OtherTests(unittest.TestCase):
    def test_a(self):
        self.assertFalse((ROOT / "live-a.json").exists())
        record("serial-a")
    def test_b(self):
        record("serial-b")
        (ROOT / "serial-complete").touch()

class LiveVerificationTests(unittest.TestCase):
    def test_a(self):
        self.assertTrue((ROOT / "serial-complete").exists())
        record("live-a")
        print("Ran 900 tests in 0.000s")
        print("refusal detail remains visible")
    def test_b(self):
        self.assertTrue((ROOT / "serial-complete").exists())
        record("live-b")
        for n in range(3):
            with self.subTest(n=n):
                self.assertGreaterEqual(n, 0)
    @unittest.expectedFailure
    def test_expected_failure(self):
        self.fail("expected refusal")
    @unittest.skip("fixture skip")
    def test_skip(self):
        self.fail("must remain skipped")
'''

_RETENTION = '''
import json
import os
import tempfile
import unittest
from pathlib import Path
from certification import retained_trial, record_observed_output

ROOT = Path(os.environ["SUITE_FIXTURE_ROOT"])

def exercise(case, name):
    selected = os.environ.get("SHIREN2_REPLAY_EVIDENCE_ROOT")
    tmp = Path(tempfile.gettempdir()).resolve()
    with tempfile.TemporaryDirectory() as temporary:
        private = Path(temporary).resolve()
        case.assertEqual(private.parent, tmp)
        with retained_trial(private, "successful") as successful:
            (Path(successful) / "stdout.txt").write_text("successful output")
        with case.assertRaisesRegex(RuntimeError, "deliberate refusal"):
            with retained_trial(private, "failed") as failed:
                (Path(failed) / "raw.bin").write_bytes(b"failed output")
                raise RuntimeError("deliberate refusal")
        record_observed_output(["fixture-tool"], b"observed output", 7)
        base = Path(selected) if selected else private / "replay-evidence"
        observed = list(base.glob("observed-tool-*"))
        before = {
            "successful_stdout": (Path(successful) / "stdout.txt").read_text(),
            "failed_raw": (Path(failed) / "raw.bin").read_text(),
            "failed_status": json.loads((Path(failed) / "lifecycle.json").read_text())["status"],
            "observed": len(observed),
        }
        if observed:
            case.assertEqual((observed[0] / "stdout.txt").read_bytes(), b"observed output")
    after = {"private": private.exists(), "successful": Path(successful).exists(),
             "failed": Path(failed).exists(), "observed": sum(path.exists() for path in observed)}
    (ROOT / (name + ".json")).write_text(json.dumps({
        "selected": selected, "tmp": str(tmp), "base": str(base),
        "before": before, "after": after, "pid": os.getpid()}))

class OtherTests(unittest.TestCase):
    def test_retention(self):
        exercise(self, "serial-retention")

class LiveVerificationTests(unittest.TestCase):
    def test_retention(self):
        exercise(self, "live-retention")
    def test_retention_second(self):
        exercise(self, "live-retention-second")
'''

_INTERRUPT = '''
import json
import os
import signal
import subprocess
import sys
import time
import unittest
from pathlib import Path

ROOT = Path(os.environ["SUITE_FIXTURE_ROOT"])
CHILD = """
import signal, sys, time
from pathlib import Path
root = Path(sys.argv[1])
signal.signal(signal.SIGTERM, lambda *_: root.with_suffix(".terminated").touch())
if sys.argv[2] == "graceful":
    signal.signal(signal.SIGTERM, signal.SIG_DFL)
signal.signal(signal.SIGINT, signal.SIG_IGN)
root.touch()
deadline = time.monotonic() + 60
while not (root.parent / "release").exists() and time.monotonic() < deadline:
    time.sleep(0.01)
"""

def hold(name):
    signal.signal(signal.SIGTERM, lambda *_: (ROOT / (name + ".terminated")).touch())
    graceful = name == "a" and os.environ.get("SUITE_GRACEFUL_WORKER") == "1"
    if graceful:
        signal.signal(signal.SIGTERM, signal.SIG_DFL)
    signal.signal(signal.SIGINT, signal.SIG_IGN)
    child_ready = ROOT / (name + "-child.ready")
    child = subprocess.Popen([sys.executable, "-c", CHILD, str(child_ready),
                              "graceful" if graceful else "resistant"])
    pending = ROOT / (name + ".pending")
    pending.write_text(json.dumps({"pid": os.getpid(), "pgid": os.getpgrp(),
                                  "child": child.pid, "child_pgid": os.getpgid(child.pid)}))
    pending.replace(ROOT / (name + ".json"))
    early = name == "b" and os.environ.get("SUITE_EARLY_WORKER") == "1"
    try:
        deadline = time.monotonic() + 15
        while not child_ready.exists():
            if time.monotonic() > deadline:
                raise RuntimeError("dummy descendant did not start")
            time.sleep(0.01)
        (ROOT / (name + ".ready")).touch()
        if early:
            return
        deadline = time.monotonic() + 60
        while not (ROOT / "release").exists():
            if name == "a" and (ROOT / "collection-error").exists():
                (Path(os.environ["TMPDIR"]).parent / "collection.json").mkdir()
                return
            if time.monotonic() > deadline:
                raise RuntimeError("runner did not stop the dummy worker")
            time.sleep(0.01)
    finally:
        if not early:
            child.kill()
            child.wait()

class LiveVerificationTests(unittest.TestCase):
    def test_a(self):
        hold("a")
    def test_b(self):
        hold("b")
'''


class SuiteRunnerTests(unittest.TestCase):
    def setUp(self) -> None:
        self.temporary = tempfile.TemporaryDirectory()
        self.addCleanup(self.temporary.cleanup)
        self.root = Path(self.temporary.name).resolve()
        self.tests = self.root / "tests"
        self.tests.mkdir()
        self.invocations = 0

    def fixture(self, source: str, jobs: int = 2, evidence: str | None = None):
        (self.tests / "test_verification.py").write_text(textwrap.dedent(source))
        self.invocations += 1
        work = self.root / f"run-{self.invocations}"
        command = [sys.executable, "-u", str(_TOOLS / "test_suite.py"),
                   "--start-directory", str(self.tests), "--work-dir", str(work), "--jobs", str(jobs)]
        env = {**os.environ, "SUITE_FIXTURE_ROOT": str(self.root), "PYTHONDONTWRITEBYTECODE": "1"}
        env["PYTHONPATH"] = os.pathsep.join(filter(None, (str(_TOOLS), env.get("PYTHONPATH"))))
        env.pop("SHIREN2_REPLAY_EVIDENCE_ROOT", None)
        if evidence is not None:
            env["SHIREN2_REPLAY_EVIDENCE_ROOT"] = evidence
        return command, env, work

    def run_suite(self, source: str, jobs: int = 2, evidence: str | None = None):
        command, env, work = self.fixture(source, jobs, evidence)
        result = subprocess.run(command, cwd=self.root, env=env, stdout=subprocess.PIPE,
                                stderr=subprocess.STDOUT, text=True, timeout=30)
        return result, work

    def assert_summary(self, result, count: int, success: bool) -> None:
        self.assertEqual(re.findall(r"^Ran (\d+) tests? in", result.stdout, re.MULTILINE), [str(count)],
                         result.stdout)
        self.assertEqual(result.returncode == 0, success, result.stdout)
        self.assertEqual(result.stdout.splitlines()[-1].startswith("OK"), success, result.stdout)

    def test_warning_defaults_and_explicit_filters_reach_suite_log(self) -> None:
        for option in (None, "ignore"):
            with self.subTest(warnoption=option):
                command, env, work = self.fixture('''
                    import unittest
                    import warnings
                    class WarningTests(unittest.TestCase):
                        def test_warnings(self):
                            for category in (DeprecationWarning, ImportWarning, ResourceWarning):
                                warnings.warn("suite warning " + category.__name__, category)
                ''')
                env.pop("PYTHONWARNINGS", None)
                if option is not None:
                    env["PYTHONWARNINGS"] = option
                suite_log = self.root / "suite.log"
                with suite_log.open("w") as log:
                    result = subprocess.run(command, cwd=self.root, env=env, stdout=log,
                                            stderr=subprocess.STDOUT, timeout=30)
                output = suite_log.read_text()
                self.assertEqual(result.returncode, 0, output)
                for name in ("DeprecationWarning", "ImportWarning", "ResourceWarning"):
                    diagnostic = f"{name}: suite warning {name}"
                    for text in (output, (work / "serial/output.log").read_text()):
                        self.assertEqual(diagnostic in text, option is None, text)

    @staticmethod
    def process_status(pid: int, *, group: bool = False) -> str:
        # Query only recorded identities; never kill a PID saved by a fixture.
        result = subprocess.run(["/bin/ps", "-g" if group else "-p", str(pid), "-o", "stat="],
                                capture_output=True, text=True, check=False)
        if result.returncode not in (0, 1) or result.stderr:
            raise RuntimeError(f"Process inspection failed: {result.returncode}: {result.stderr}")
        return result.stdout.strip()

    def interrupted_runner(self, signum: int | None, *, graceful: bool = False,
                           group: bool = False, early: bool = False,
                           second_signal: int | None = None) -> None:
        command, env, work = self.fixture(_INTERRUPT, jobs=2)
        env["SUITE_GRACEFUL_WORKER"] = "1" if graceful else "0"
        env["SUITE_EARLY_WORKER"] = "1" if early else "0"
        hard_kill = signal.SIGKILL in (signum, second_signal)
        suite_log = self.root / "suite.log"
        records = []
        with suite_log.open("w") as log:
            process = subprocess.Popen(command, cwd=self.root, env=env, stdout=log,
                                       stderr=subprocess.STDOUT, start_new_session=True)
            try:
                deadline = time.monotonic() + 15
                while not all((self.root / f"{name}.ready").exists() for name in ("a", "b")):
                    self.assertIsNone(process.poll(), suite_log.read_text())
                    self.assertLess(time.monotonic(), deadline, suite_log.read_text())
                    time.sleep(0.01)
                records = [json.loads((self.root / f"{name}.json").read_text()) for name in ("a", "b")]
                self.assertEqual(len({record["pid"] for record in records}), 2)
                for record in records:
                    self.assertEqual(record["pgid"], record["pid"])
                    self.assertEqual(record["child_pgid"], record["pid"])
                    self.assertNotEqual(record["pgid"], os.getpgid(process.pid))
                    self.assertTrue(self.process_status(record["pid"]))
                    self.assertTrue(self.process_status(record["child"]))
                if early:
                    while not self.process_status(records[1]["pid"]).startswith("Z"):
                        self.assertLess(time.monotonic(), deadline, suite_log.read_text())
                        time.sleep(0.01)
                    self.assertFalse((work / "live-2/collection.json").exists())
                if signum is None:
                    (self.root / "collection-error").touch()
                else:
                    (os.killpg if group else os.kill)(process.pid, signum)
                if second_signal is not None:
                    while not (self.root / "a.terminated").exists():
                        self.assertLess(time.monotonic(), deadline, suite_log.read_text())
                        time.sleep(0.01)
                    os.kill(process.pid, second_signal)
                code = process.wait(timeout=8)
                self.assertEqual(code, -signal.SIGKILL if hard_kill else 1, suite_log.read_text())
                if not hard_kill:
                    for record in records:
                        self.assertFalse(self.process_status(record["pid"]), record)
                deadline = time.monotonic() + 3
                while any(self.process_status(record["pgid"], group=True) for record in records):
                    self.assertLess(time.monotonic(), deadline, records)
                    time.sleep(0.01)
                for record in records:
                    self.assertFalse(self.process_status(record["pid"]), record)
                    self.assertFalse(self.process_status(record["child"]), record)
                if hard_kill:
                    self.assertFalse((work / "result.json").exists())
                else:
                    if not early:
                        for name in (("a", "b") if signum is not None and not graceful else ("b",)):
                            self.assertTrue((self.root / f"{name}.terminated").exists())
                            self.assertTrue((self.root / f"{name}-child.terminated").exists())
                    aggregate = json.loads((work / "result.json").read_text())
                    self.assertFalse(aggregate["success"])
                    self.assertTrue(aggregate["runner_errors"])
            finally:
                # Cooperative release also works when the regression itself fails.
                # Only the still-owned direct child may receive a cleanup signal.
                (self.root / "release").touch()
                if process.poll() is None:
                    process.terminate()
                try:
                    process.wait(timeout=5)
                except subprocess.TimeoutExpired:
                    process.kill()
                    process.wait(timeout=5)

    def test_sigterm_reaps_two_live_worker_groups(self) -> None:
        self.interrupted_runner(signal.SIGTERM, second_signal=signal.SIGINT)

    def test_sigterm_reaps_resistant_group_after_other_worker_exits(self) -> None:
        self.interrupted_runner(signal.SIGTERM, graceful=True)

    def test_sigint_reaps_two_live_worker_groups(self) -> None:
        self.interrupted_runner(signal.SIGINT, second_signal=signal.SIGTERM)

    def test_collection_exception_reaps_other_live_worker_group(self) -> None:
        self.interrupted_runner(None)

    def test_sigkill_parent_cleans_new_session_workers_and_descendants(self) -> None:
        self.interrupted_runner(signal.SIGKILL)

    def test_sigkill_parent_group_cleans_new_session_workers_and_descendants(self) -> None:
        self.interrupted_runner(signal.SIGKILL, group=True)

    def test_parent_death_guard_survives_term_cleanup(self) -> None:
        self.interrupted_runner(signal.SIGTERM, second_signal=signal.SIGKILL)

    def test_parent_death_guard_survives_early_leader_exit(self) -> None:
        self.interrupted_runner(signal.SIGKILL, early=True)

    def test_sigterm_retires_already_exited_later_shard(self) -> None:
        self.interrupted_runner(signal.SIGTERM, early=True)

    def test_later_shard_reserves_identity_until_retirement(self) -> None:
        _, env, work = self.fixture(_INTERRUPT)
        env["SUITE_EARLY_WORKER"] = "1"
        work.mkdir()
        launched = {}
        signals = []
        real_launch, real_collect = test_suite._launch, test_suite._collect
        real_killpg = os.killpg
        reserved_wall = []

        def launch(*args, **kwargs):
            worker, directory = real_launch(*args, **kwargs)
            launched[directory.name] = worker
            return worker, directory

        def collect(worker, directory, selected):
            if directory.name == "live-1":
                later = launched["live-2"]
                self.assertTrue(later.exited.wait(8))
                self.assertIsNone(later.process.returncode)
                self.assertTrue(self.process_status(later.process.pid).startswith("Z"))
                self.assertFalse((work / "live-2/collection.json").exists())
                reserved_wall.append(later.wall_seconds)
                time.sleep(0.3)
                (self.root / "release").touch()
            return real_collect(worker, directory, selected)

        def killpg(pid, signum):
            worker = next(worker for worker in launched.values() if worker.process.pid == pid)
            signals.append((pid, signum, worker.process.returncode))
            # Even a broken implementation must not signal a freed identity.
            if worker.process.returncode is None:
                return real_killpg(pid, signum)

        with patch.dict(os.environ, env, clear=True), patch.object(test_suite, "_launch", launch), \
                patch.object(test_suite, "_collect", collect), patch("os.killpg", killpg), \
                contextlib.redirect_stdout(io.StringIO()):
            try:
                self.assertEqual(test_suite._run(self.tests, work, 2), 0)
                signal_count = len(signals)
                test_suite._stop_workers(list(launched.values()))
                self.assertEqual(len(signals), signal_count)
            finally:
                (self.root / "release").touch()
                test_suite._stop_workers(list(launched.values()))
        self.assertTrue(signals)
        self.assertTrue(all(code is None for _, _, code in signals), signals)
        self.assertIn(launched["live-2"].process.pid, [pid for pid, _, _ in signals])
        result = json.loads((work / "result.json").read_text())
        self.assertEqual(result["workers"]["live-2"]["worker_wall_seconds"], reserved_wall[0])
        self.assertGreater(result["workers"]["live-1"]["worker_wall_seconds"], reserved_wall[0] + 0.2)
        for worker in launched.values():
            self.assertIsNotNone(worker.process.returncode)
            self.assertFalse(self.process_status(worker.process.pid))

    def test_inherited_stdout_holder_cannot_stall_collection(self) -> None:
        command, env, work = self.fixture(_INTERRUPT)
        env["SUITE_EARLY_WORKER"] = "1"
        # Only the quick test runs, leaving a TERM-resistant stdout holder.
        (self.tests / "test_verification.py").write_text(
            textwrap.dedent(_INTERRUPT) + "\nLiveVerificationTests.test_a = None\n")
        try:
            result = subprocess.run(command, env=env, cwd=self.root, capture_output=True,
                                    text=True, timeout=8)
            self.assert_summary(result, 1, True)
            record = json.loads((self.root / "b.json").read_text())
            deadline = time.monotonic() + 3
            while self.process_status(record["pgid"], group=True):
                self.assertLess(time.monotonic(), deadline, record)
                time.sleep(0.01)
            self.assertFalse(self.process_status(record["child"]))
            self.assertIn("OK", (work / "live-1/output.log").read_text())
        finally:
            (self.root / "release").touch()

    def test_registration_error_retires_reserved_child(self) -> None:
        _, env, work = self.fixture(_SUCCESS)
        work.mkdir()
        children = []
        popen = subprocess.Popen

        def spawn(*args, **kwargs):
            child = popen(*args, **kwargs)
            children.append(child)
            return child

        with patch.dict(os.environ, env, clear=True), patch("test_suite.subprocess.Popen", spawn), \
                patch("test_suite.select.kevent", side_effect=OSError("registration refused")), \
                contextlib.redirect_stdout(io.StringIO()):
            self.assertEqual(test_suite._run(self.tests, work, 2), 1)
        self.assertEqual(len(children), 1)
        self.assertIsNotNone(children[0].returncode)
        self.assertFalse(self.process_status(children[0].pid))
        result = json.loads((work / "result.json").read_text())
        self.assertFalse(result["success"])
        self.assertIn("registration refused", result["runner_errors"])

    def test_real_workers_preserve_counts_partition_logs_and_isolation(self) -> None:
        result, work = self.run_suite(_SUCCESS)
        self.assert_summary(result, 6, True)
        plan = json.loads((work / "plan.json").read_text())
        flattened = [name for group in plan["groups"] for name in group]
        self.assertEqual(Counter(flattened), Counter(plan["ids"]))
        self.assertEqual(len(flattened), len(set(flattened)))
        events = [json.loads(line) for path in work.glob("*/events.jsonl")
                  for line in path.read_text().splitlines()]
        self.assertEqual(Counter(events), Counter(plan["ids"]))
        records = {name: json.loads((self.root / f"{name}.json").read_text())
                   for name in ("serial-a", "serial-b", "live-a", "live-b")}
        self.assertEqual(records["serial-a"], records["serial-b"])
        self.assertEqual(len({records[name]["pid"] for name in ("serial-a", "live-a", "live-b")}), 3)
        self.assertEqual(len({record["tmp"] for record in records.values()}), 3)
        for record in records.values():
            self.assertTrue(Path(record["tmp"]).is_relative_to(work))
        self.assertIn("refusal detail remains visible", result.stdout)
        self.assertIn("Ran 900 tests in", (work / "live-1/output.log").read_text())
        aggregate = json.loads((work / "result.json").read_text())
        self.assertEqual(aggregate["tests"], 6)
        self.assertEqual(aggregate["skipped"], 1)
        self.assertEqual(aggregate["expectedFailures"], 1)
        self.assertEqual(aggregate["runner_errors"], [])
        measured = []
        for name, worker in aggregate["workers"].items():
            self.assertGreater(worker["worker_wall_seconds"], 0)
            if name == "discovery":
                continue
            child = json.loads((work / name / "result.json").read_text())
            self.assertGreater(child["wall_seconds"], 0)
            self.assertGreaterEqual(worker["worker_wall_seconds"], child["wall_seconds"])
            self.assertEqual(worker["test_durations"], child["test_durations"])
            measured.extend(item["test"] for item in child["test_durations"])
            for item in child["test_durations"]:
                self.assertGreaterEqual(item["wall_seconds"], 0)
        self.assertEqual(Counter(measured), Counter(plan["ids"]))

    def test_unset_replay_retention_matches_direct_unsharded_unittest(self) -> None:
        _, env, _ = self.fixture(_RETENTION)
        direct = subprocess.run([sys.executable, "-m", "unittest", "discover", "-s", str(self.tests), "-v"],
                                cwd=self.root, env=env, stdout=subprocess.PIPE,
                                stderr=subprocess.STDOUT, text=True, timeout=30)
        self.assertEqual(direct.returncode, 0, direct.stdout)
        names = ("serial-retention", "live-retention", "live-retention-second")
        baseline = {name: json.loads((self.root / f"{name}.json").read_text()) for name in names}
        result, work = self.run_suite(_RETENTION)
        self.assert_summary(result, 3, True)
        records = {name: json.loads((self.root / f"{name}.json").read_text()) for name in names}
        self.assertEqual(len({record["pid"] for record in records.values()}), 3)
        self.assertEqual(len({record["tmp"] for record in records.values()}), 3)
        for name in names:
            actual = records[name]
            self.assertIsNone(actual["selected"])
            self.assertIsNone(baseline[name]["selected"])
            self.assertEqual(actual["before"], baseline[name]["before"])
            self.assertEqual(actual["before"]["observed"], 0)
            self.assertEqual(actual["after"], baseline[name]["after"])
            self.assertEqual(actual["after"], {"private": False, "successful": False,
                                               "failed": False, "observed": 0})
            self.assertTrue(Path(actual["tmp"]).is_relative_to(work))
            self.assertNotEqual(actual["tmp"], baseline[name]["tmp"])
            self.assertFalse(Path(actual["base"]).exists())
        self.assertEqual(list(work.rglob("replay-evidence")), [])
        self.assertEqual(list(work.rglob("observed-tool-*")), [])

    def test_valid_opt_in_root_is_suffixed_and_real_evidence_is_retained(self) -> None:
        evidence = self.root / "opt-in"
        result, work = self.run_suite(_RETENTION, evidence=str(evidence))
        self.assert_summary(result, 3, True)
        for name, worker in (("serial-retention", "serial"), ("live-retention", "live-1"),
                             ("live-retention-second", "live-2")):
            actual = json.loads((self.root / f"{name}.json").read_text())
            self.assertEqual(Path(actual["selected"]), evidence / worker)
            self.assertEqual(Path(actual["tmp"]), work / worker / "tmp")
            self.assertEqual(actual["before"]["observed"], 1)
            self.assertEqual(actual["after"], {"private": False, "successful": True,
                                               "failed": True, "observed": 1})
            self.assertEqual(len(list((evidence / worker).glob("failed-*/raw.bin"))), 1)
        self.assertFalse((evidence / "discovery").exists())

    def test_invalid_nonempty_replay_roots_reach_real_helper_refusals(self) -> None:
        target = self.root / "target"
        target.mkdir()
        symlink = self.root / "linked-root"
        symlink.symlink_to(target, target_is_directory=True)
        for value in ("relative-evidence", str(symlink)):
            with self.subTest(value=value):
                result, _ = self.run_suite('''
                    import os
                    import tempfile
                    import unittest
                    from pathlib import Path
                    from certification import retained_trial, record_observed_output
                    class LiveVerificationTests(unittest.TestCase):
                        def test_invalid_root(self):
                            self.assertEqual(os.environ["SHIREN2_REPLAY_EVIDENCE_ROOT"], EXPECTED)
                            with tempfile.TemporaryDirectory() as private:
                                with self.assertRaises(ValueError):
                                    with retained_trial(Path(private), "invalid"):
                                        self.fail("invalid root accepted")
                                with self.assertRaises(ValueError):
                                    record_observed_output(["fixture-tool"], "output", 0)
                '''.replace("EXPECTED", repr(value)), evidence=value)
                self.assert_summary(result, 1, True)
        self.assertEqual(list(target.iterdir()), [])
        self.assertFalse((self.root / "relative-evidence").exists())

    def test_subtest_failures_do_not_inflate_tests_and_retain_full_logs(self) -> None:
        result, work = self.run_suite('''
            import unittest
            class LiveVerificationTests(unittest.TestCase):
                def test_bad(self):
                    for n in range(2):
                        with self.subTest(n=n):
                            self.fail("original refusal message " + str(n))
                def test_good(self):
                    pass
        ''')
        self.assert_summary(result, 2, False)
        aggregate = json.loads((work / "result.json").read_text())
        self.assertEqual(aggregate["failures"], 2)
        self.assertEqual(aggregate["runner_errors"], [])
        for n in range(2):
            self.assertIn(f"original refusal message {n}", result.stdout)
            self.assertIn(f"original refusal message {n}", (work / "live-1/output.log").read_text())
        self.assertTrue((work / "live-2/result.json").exists())

    def test_crash_and_zero_exit_without_result_both_fail_closed(self) -> None:
        for code in (17, 0):
            with self.subTest(code=code):
                result, work = self.run_suite(f'''
                    import os
                    import unittest
                    class LiveVerificationTests(unittest.TestCase):
                        def test_crash(self):
                            print("crash detail", flush=True)
                            os.write(1, b"unterminated final output")
                            os._exit({code})
                        def test_good(self):
                            pass
                ''')
                self.assert_summary(result, 2, False)
                self.assertIn("crash detail", result.stdout)
                self.assertIn("[live-1] unterminated final output\n", result.stdout)
                self.assertTrue((work / "live-1/output.log").read_bytes().endswith(b"unterminated final output"))
                self.assertFalse((work / "live-1/result.json").exists())
                self.assertTrue((work / "live-2/result.json").exists())
                aggregate = json.loads((work / "result.json").read_text())
                self.assertTrue(aggregate["runner_errors"])
                self.assertEqual(aggregate["errors"], 0)

    def test_streams_live_before_concurrent_workers_complete(self) -> None:
        command, env, work = self.fixture('''
            import os
            import time
            import unittest
            from pathlib import Path
            ROOT = Path(os.environ["SUITE_FIXTURE_ROOT"])
            def wait_for_release(name):
                print("\\nREADY-" + name)
                until = time.monotonic() + 15
                while not (ROOT / "release").exists():
                    if time.monotonic() > until:
                        raise RuntimeError("parent did not observe live output")
                    time.sleep(0.01)
                for index in range(10):
                    print(name + "-line-" + str(index))
            class LiveVerificationTests(unittest.TestCase):
                def test_a(self):
                    wait_for_release("a")
                def test_b(self):
                    wait_for_release("b")
        ''')
        process = subprocess.Popen(command, cwd=self.root, env=env, stdout=subprocess.PIPE,
                                   stderr=subprocess.STDOUT, text=True)
        stream = process.stdout
        assert stream is not None
        lines = queue.Queue()
        output = []

        def reader():
            for line in stream:
                output.append(line)
                lines.put(line)

        thread = threading.Thread(target=reader)
        thread.start()
        try:
            seen = set()
            deadline = time.monotonic() + 10
            while len(seen) < 2:
                line = lines.get(timeout=max(0.01, deadline - time.monotonic()))
                for name, worker in (("a", "live-1"), ("b", "live-2")):
                    if f"READY-{name}" in line:
                        self.assertEqual(line, f"[{worker}] READY-{name}\n")
                        self.assertIn(f"READY-{name}\n", (work / worker / "output.log").read_text())
                        self.assertFalse((work / worker / "result.json").exists())
                        seen.add(name)
                self.assertIsNone(process.poll())
            (self.root / "release").touch()
            self.assertEqual(process.wait(timeout=15), 0)
        finally:
            (self.root / "release").touch()
            if process.poll() is None:
                process.kill()
            process.wait()
            thread.join(timeout=5)
            stream.close()
        for name, worker in (("a", "live-1"), ("b", "live-2")):
            for index in range(10):
                self.assertIn(f"[{worker}] {name}-line-{index}\n", output)
        self.assertEqual(len(re.findall(r"^Ran ", "".join(output), re.MULTILINE)), 1)

    def test_import_errors_remain_failing_tests_with_tracebacks(self) -> None:
        result, work = self.run_suite('raise ImportError("fixture import refusal")')
        self.assert_summary(result, 1, False)
        self.assertIn("ImportError: fixture import refusal", result.stdout)
        self.assertEqual(json.loads((work / "result.json").read_text())["errors"], 1)

    def test_real_worker_counts_survive_class_setup_id_mismatch(self) -> None:
        result, work = self.run_suite('''
            import unittest
            class ASetupFailure(unittest.TestCase):
                @classmethod
                def setUpClass(cls):
                    raise RuntimeError("fixture setup refusal A")
                def test_unreached(self):
                    pass
            class BSetupFailure(unittest.TestCase):
                @classmethod
                def setUpClass(cls):
                    raise RuntimeError("fixture setup refusal B")
                def test_unreached(self):
                    pass
            class OtherTests(unittest.TestCase):
                def test_a_failure(self):
                    for n in range(2):
                        with self.subTest(n=n):
                            self.fail("real subtest failure")
                def test_b_error(self):
                    raise RuntimeError("real test error")
                @unittest.skip("fixture skip")
                def test_c_skip(self):
                    pass
                @unittest.expectedFailure
                def test_d_expected(self):
                    self.fail("expected")
                @unittest.expectedFailure
                def test_e_unexpected(self):
                    pass
                def test_f_good(self):
                    pass
        ''')
        self.assert_summary(result, 6, False)
        expected = {"tests": 6, "failures": 2, "errors": 3, "skipped": 1,
                    "expectedFailures": 1, "unexpectedSuccesses": 1}
        raw = json.loads((work / "serial/result.json").read_text())
        aggregate = json.loads((work / "result.json").read_text())
        collected = json.loads((work / "serial/collection.json").read_text())
        for key, value in expected.items():
            self.assertEqual(raw[key], value)
            self.assertEqual(collected[key], value)
            self.assertEqual(aggregate[key], value)
        self.assertEqual(len(collected["assigned_ids"]), 8)
        self.assertEqual(len(collected["started_ids"]), 6)
        self.assertTrue(collected["runner_errors"])
        self.assertTrue(aggregate["runner_errors"])
        self.assertIn("errors=3", result.stdout.splitlines()[-1])

    def test_duplicate_discovery_ids_are_rejected(self) -> None:
        result, work = self.run_suite('''
            import unittest
            class Example(unittest.TestCase):
                def test_once(self):
                    pass
            def load_tests(loader, suite, pattern):
                return unittest.TestSuite([Example("test_once"), Example("test_once")])
        ''')
        self.assert_summary(result, 0, False)
        self.assertFalse((work / "plan.json").exists())
        self.assertIn("Duplicate discovered test IDs", result.stdout)

    def test_changed_discovery_between_processes_is_rejected(self) -> None:
        result, work = self.run_suite('''
            import os
            import unittest
            from pathlib import Path
            class OtherTests(unittest.TestCase):
                def test_once(self):
                    pass
            if Path(os.environ["TMPDIR"]).parent.name == "serial":
                OtherTests.test_added = lambda self: None
        ''')
        self.assert_summary(result, 0, False)
        self.assertFalse((work / "serial/events.jsonl").exists())
        self.assertTrue(json.loads((work / "result.json").read_text())["runner_errors"])
        self.assertIn("Discovered tests changed between planning and execution", result.stdout)

    def test_non_live_class_with_same_name_stays_serial(self) -> None:
        (self.tests / "test_other.py").write_text(textwrap.dedent('''
            import os
            import unittest
            from pathlib import Path
            class LiveVerificationTests(unittest.TestCase):
                def test_serial(self):
                    self.assertEqual(Path(os.environ["TMPDIR"]).parent.name, "serial")
        '''))
        result, work = self.run_suite("", jobs=2)
        self.assert_summary(result, 1, True)
        self.assertEqual(len(json.loads((work / "plan.json").read_text())["groups"]), 1)

    def test_lpt_balances_recorded_work_and_preserves_membership_and_order(self) -> None:
        timing = {"test_seconds": dict(zip("abcdef", (9, 8, 7, 6, 5, 4))),
                  "unknown_test_seconds": 20}
        live = sorted(timing["test_seconds"])
        ids = ["serial-first", *live, "serial-last"]
        groups = test_suite._partition(ids, live, 2, timing)
        self.assertEqual(groups[0], ["serial-first", "serial-last"])
        self.assertEqual(Counter(name for group in groups for name in group), Counter(ids))
        for group in groups:
            self.assertEqual(group, [name for name in ids if name in set(group)])
        def load(group):
            return sum(timing["test_seconds"][name] for name in group)

        lpt_loads = [load(group) for group in groups[1:]]
        round_robin_loads = [load(live[index::2]) for index in range(2)]
        self.assertLess(max(lpt_loads), max(round_robin_loads))
        self.assertEqual(sorted(lpt_loads), [19, 20])
        self.assertEqual(groups, test_suite._partition(ids, list(reversed(live)), 2, timing))
        with_unknown = test_suite._partition([*ids, "unknown"], [*live, "unknown"], 2, timing)
        self.assertIn("unknown", with_unknown[1])
        self.assertEqual(sum(group.count("unknown") for group in with_unknown), 1)
        tied = {"test_seconds": {"a": 3, "b": 3, "c": 2, "d": 2}, "unknown_test_seconds": 4}
        self.assertEqual(test_suite._partition(["d", "c", "b", "a"], ["d", "b", "a", "c"], 2, tied),
                         [[], ["c", "a"], ["d", "b"]])

    def test_jobs_bounds_and_invalid_partition_membership(self) -> None:
        for jobs in (0, -1, 33):
            with self.subTest(jobs=jobs), self.assertRaises(ValueError):
                test_suite._partition(["a"], ["a"], jobs)
        for ids, live in ((["a", "a"], ["a"]), (["a"], ["b"]), (["a"], ["a", "a"])):
            with self.subTest(ids=ids, live=live), self.assertRaises(ValueError):
                test_suite._partition(ids, live, 2)
        self.assertEqual(test_suite._partition(["a"], ["a"], 32), [[], ["a"]])
        for jobs in ("0", "33"):
            with self.subTest(jobs=jobs), self.assertRaises(argparse.ArgumentTypeError):
                test_suite._jobs(jobs)

    def test_malformed_results_fail_closed_without_erasing_usable_counts(self) -> None:
        class Exited:
            stream_errors = []
            wall_seconds = 0.1

            def wait(self):
                return 0

        directory = self.root / "worker"
        directory.mkdir()
        (directory / "events.jsonl").write_text('"a"\n')
        valid = {"tests": 1, "success": True, "failures": 0, "errors": 0, "skipped": 0,
                 "expectedFailures": 0, "unexpectedSuccesses": 0}
        for payload in ([], {}, {**valid, "tests": 2, "errors": 3}, {**valid, "tests": True}):
            with self.subTest(payload=payload), contextlib.redirect_stdout(io.StringIO()):
                (directory / "result.json").write_text(json.dumps(payload))
                count, success, collected = test_suite._collect(Exited(), directory, ["a"])
                self.assertEqual(count, 2 if isinstance(payload, dict) and payload.get("tests") == 2 else 1)
                self.assertFalse(success)
                self.assertTrue(collected["runner_errors"])
                if isinstance(payload, dict) and payload.get("errors") == 3:
                    self.assertEqual(collected["errors"], 3)
        (directory / "result.json").write_text(json.dumps(valid))
        with contextlib.redirect_stdout(io.StringIO()):
            count, success, collected = test_suite._collect(Exited(), directory, ["a", "missing"])
        self.assertEqual(count, 1)
        self.assertFalse(success)
        self.assertEqual(collected["errors"], 0)
        self.assertTrue(collected["runner_errors"])


if __name__ == "__main__":
    unittest.main()
