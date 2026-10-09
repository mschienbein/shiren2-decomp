"""Replay ``commands.json``: rewritten incrementally, byte-identical to a full re-encoding after every write.

No toolchain runs: the replay's subprocess calls are substituted. The termination tests
start only the project interpreter as a child and kill it with a real SIGTERM.
"""
from __future__ import annotations

import json
import os
import random
import signal
import subprocess
import sys
import tempfile
import textwrap
import unittest
from pathlib import Path
from unittest.mock import patch

TOOLS = Path(__file__).resolve().parents[1] / "tools"
sys.path.insert(0, str(TOOLS))
import certification
from certification import CommandLog


def encoded(records: list) -> str:
    """What the previous writer left after every change: the full list, indent 2, newline."""
    return json.dumps(records, indent=2) + "\n"


AWKWARD = [
    {},
    {"argv": [], "environment": {}, "nested": {"empty": {}, "list": [[], {}, [{}]]}},
    {"argv": ["caf\u00e9", "\u2603", "tab\there", "line\nbreak", 'quote"\\'], "returncode": -11},
    {"numbers": [0, -1, 1.5, 1e300, None, True, False], "text": "\u0000\u001f\u007f\ud7ff"},
    [],
    [1, [2, [3, {}]]],
    "plain string record",
    0,
]


class CommandLogTests(unittest.TestCase):
    def setUp(self) -> None:
        temporary = tempfile.TemporaryDirectory()
        self.addCleanup(temporary.cleanup)
        self.path = Path(temporary.name) / "commands.json"

    def test_the_chunk_form_is_the_full_encoding(self) -> None:
        self.assertEqual(json.dumps([], indent=2), "[]")
        for count in range(len(AWKWARD) + 1):
            records = AWKWARD[:count]
            if records:
                body = b",\n".join(CommandLog.chunk(record) for record in records)
                self.assertEqual(b"[\n" + body + b"\n]", json.dumps(records, indent=2).encode())

    def test_nothing_is_written_while_there_are_no_records(self) -> None:
        log = CommandLog(self.path)
        log.rewrite_from(0)
        self.assertFalse(self.path.exists())

    def test_every_write_leaves_exactly_the_full_encoding(self) -> None:
        rng = random.Random(16)
        for trial in range(40):
            with self.subTest(trial=trial):
                path = self.path.with_name(f"commands-{trial}.json")
                log = CommandLog(path)
                for step in range(rng.randint(1, 60)):
                    if not log.records or rng.random() < 0.55:
                        log.records.append(dict(rng.choice([r for r in AWKWARD if isinstance(r, dict)]), step=step))
                        changed = len(log.records) - 1
                    else:
                        changed = rng.randrange(len(log.records))
                        log.records[changed] = {"argv": [f"x{step}"] * rng.randint(0, 3), "returncode": rng.choice([0, 2, None])}
                    if rng.random() < 0.2 and changed > 0:  # a later-than-needed start is also exact
                        changed = rng.randrange(changed + 1)
                    log.rewrite_from(changed)
                    self.assertEqual(path.read_bytes(), encoded(log.records).encode())

    def test_serial_updates_touch_only_the_last_record(self) -> None:
        log = CommandLog(self.path)
        writes = []
        real_open = Path.open

        def spy(path, mode="r", *args, **kwargs):
            stream = real_open(path, mode, *args, **kwargs)
            if "+" in mode:
                original = stream.write
                stream.write = lambda data: writes.append(len(data)) or original(data)
            return stream

        with patch.object(Path, "open", spy):
            for number in range(200):
                record = {"argv": ["cc", f"object-{number}.o", "x" * 50]}
                log.records.append(record)
                log.rewrite_from(len(log.records) - 1)
                record["returncode"] = 0
                log.rewrite_from(len(log.records) - 1)
        self.assertEqual(self.path.read_bytes(), encoded(log.records).encode())
        self.assertLess(max(writes), 2 * len(CommandLog.chunk(log.records[-1])) + 10)  # O(record), never O(log)


class ReplayCommandEvidenceTests(unittest.TestCase):
    """reproduce_c_objects_and_link leaves the same commands.json the per-command rewrite did."""

    def setUp(self) -> None:
        temporary = tempfile.TemporaryDirectory()
        self.addCleanup(temporary.cleanup)
        self.directory = Path(temporary.name).resolve() / "run"
        (self.directory / "source/config").mkdir(parents=True)
        (self.directory / "input-manifest.json").write_text("{}")
        (self.directory / "shiren2.ld").write_text("frozen linker script\n")
        self.profile = {"tu_profiles": {}, "compiler": {"gcc": {"path": "/pinned/gcc"}}, "compiler_environment": {}}
        self.reference = self.directory / "baserom.z64"
        for target, value in (("workspace.verify_manifest", None), ("certification.python_command", "/venv/bin/python")):
            handle = patch(target, return_value=value)
            handle.start()
            self.addCleanup(handle.stop)
        handle = patch.dict(os.environ, {"SHIREN2_REPLAY_EVIDENCE_ROOT": ""})
        handle.start()
        self.addCleanup(handle.stop)

    def replay(self, completed: subprocess.CompletedProcess, split_output: str) -> Path:
        def run(command, cwd, env, capture_output):
            (Path(cwd) / "shiren2.ld").write_text(split_output)
            return completed

        with patch.object(certification.subprocess, "run", side_effect=run):
            with self.assertRaises((subprocess.CalledProcessError, ValueError)) as raised:
                certification.reproduce_c_objects_and_link(self.directory, {}, self.profile, self.reference)
        self.error = raised.exception
        (trial,) = (self.directory / "replay-evidence").iterdir()
        return trial

    def expected_record(self, trial: Path, returncode: int) -> dict:
        return {"argv": ["/venv/bin/python", "-m", "splat", "split", "source/config/shiren2.jp.yaml", "splat.override.yaml", "--disassemble-all"],
                "cwd": str(trial), "environment": certification.compiler_environment(trial, self.profile),
                "driver_children": "Internal compiler/Splat children untraced; argv/recipes unchanged", "returncode": returncode}

    def test_failed_command_leaves_its_returncode_in_the_retained_trial(self) -> None:
        trial = self.replay(subprocess.CompletedProcess([], 2, b"out", b"err"), "frozen linker script\n")
        self.assertIsInstance(self.error, subprocess.CalledProcessError)
        evidence = trial / "command-evidence"
        self.assertEqual((evidence / "commands.json").read_text(), encoded([self.expected_record(trial, 2)]))
        self.assertEqual(((evidence / "001.stdout").read_bytes(), (evidence / "001.stderr").read_bytes()), (b"out", b"err"))
        self.assertEqual(json.loads((trial / "lifecycle.json").read_text())["status"], "failed")

    def test_check_failure_after_a_successful_command_writes_the_final_state(self) -> None:
        trial = self.replay(subprocess.CompletedProcess([], 0, b"", b""), "different linker script\n")
        self.assertRegex(str(self.error), "Frozen YAML does not reproduce the linker script")
        self.assertEqual((trial / "command-evidence/commands.json").read_text(), encoded([self.expected_record(trial, 0)]))


# A child replays with a mocked toolchain and receives a real, default SIGTERM during or just
# after command 2: the process dies without unwinding, so only what was already on disk survives.
CHILD = textwrap.dedent("""
    import os, signal, subprocess, sys
    from pathlib import Path
    from unittest.mock import patch
    sys.path.insert(0, sys.argv[1])
    import certification, workspace
    directory, scenario = Path(sys.argv[2]), sys.argv[3]
    graph = {"obj/control.o": {"source_kind": "asm", "source": "generated/control.s", "input": "generated/control.s"}}
    profile = {"tu_profiles": {}, "compiler": {"gcc": {"path": "/pinned/gcc"}}, "compiler_environment": {}}
    calls = []

    class KilledAfterRecording(subprocess.CompletedProcess):
        def check_returncode(self):
            os.kill(os.getpid(), signal.SIGTERM)

    def run(command, cwd, env, capture_output):
        calls.append(command)
        trial = Path(cwd)
        if len(calls) == 1:
            (trial / "shiren2.ld").write_text("frozen linker\\n")
            (trial / "generated").mkdir()
            return subprocess.CompletedProcess(command, 0, b"first stdout", b"first stderr")
        if scenario == "during-command-2":
            os.kill(os.getpid(), signal.SIGTERM)
        return KilledAfterRecording(command, 0, b"second stdout", b"second stderr")

    with patch.object(workspace, "verify_manifest"), patch.object(certification, "python_command", return_value="/synthetic/python"), \\
            patch.object(certification, "build_graph", return_value=graph), \\
            patch.object(certification, "object_commands", return_value=[["synthetic-control-command"]]), \\
            patch.object(certification.subprocess, "run", side_effect=run):
        certification.reproduce_c_objects_and_link(directory, graph, profile, directory / "not-a-rom")
    sys.exit("not killed")
""")


class AbruptTerminationTests(unittest.TestCase):
    """The writer adds no deferral: a non-unwinding death leaves what the per-command rewrite left."""

    def killed(self, scenario: str) -> tuple[Path, list]:
        temporary = tempfile.TemporaryDirectory()
        self.addCleanup(temporary.cleanup)
        directory = Path(temporary.name).resolve() / "run"
        (directory / "source/config").mkdir(parents=True)
        (directory / "generated").mkdir()
        (directory / "obj").mkdir()
        (directory / "obj/control.o").write_bytes(b"expected object")
        (directory / "input-manifest.json").write_text("{}")
        (directory / "shiren2.ld").write_text("frozen linker\n")
        environment = {key: value for key, value in os.environ.items() if not key.startswith("SHIREN2_")}
        completed = subprocess.run([sys.executable, "-B", "-c", CHILD, str(TOOLS), str(directory), scenario],
                                   env=environment, capture_output=True, text=True, timeout=120)
        self.assertEqual(completed.returncode, -signal.SIGTERM, completed.stderr)
        (trial,) = (directory / "replay-evidence").iterdir()
        log = trial / "command-evidence"
        text = (log / "commands.json").read_text()
        records = json.loads(text)
        self.assertEqual(text, encoded(records))  # the exact full encoding of what it holds
        return log, records

    def test_death_during_command_2_keeps_command_1_complete_and_command_2_started(self) -> None:
        log, records = self.killed("during-command-2")
        self.assertEqual([record["argv"][0] for record in records], ["/synthetic/python", "synthetic-control-command"])
        self.assertEqual(records[0]["returncode"], 0)
        self.assertNotIn("returncode", records[1])
        self.assertEqual(sorted(path.name for path in log.iterdir()), ["001.stderr", "001.stdout", "commands.json"])

    def test_death_after_command_2_outputs_keeps_both_records_with_returncodes(self) -> None:
        log, records = self.killed("after-command-2")
        self.assertEqual([record.get("returncode") for record in records], [0, 0])
        self.assertEqual(sorted(path.name for path in log.iterdir()),
                         ["001.stderr", "001.stdout", "002.stderr", "002.stdout", "commands.json"])
        self.assertEqual((log / "002.stdout").read_bytes(), b"second stdout")  # every retained output has its record


if __name__ == "__main__":
    unittest.main()
