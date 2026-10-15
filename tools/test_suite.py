#!/usr/bin/env python3
"""Run unittest with isolated LiveVerificationTests process shards.

    python tools/test_suite.py --jobs 1
    python tools/accept.py prepare --name NAME --test-jobs 2

Non-live tests run serially before live workers. Live IDs use longest-processing-
time-first (LPT) placement from test_suite_timings.json, with deterministic ties
and discovery order inside each shard. Unknown tests cost a conservative 1800s
(thirty minutes, above the slowest recorded test). Timings are estimates only.
Budget jobs * (private core + peak replay/build outputs), plus logs and evidence
normally retained beside shared receipts or in an explicit opt-in root.
TMPDIR/TMP/TEMP are private to every worker. With SHIREN2_REPLAY_EVIDENCE_ROOT
unset, trials use the verified directory's replay-evidence/ and follow its
lifetime; observed-tool recording stays disabled.
A valid opt-in root is suffixed by worker name; invalid values pass through for
the replay helpers to reject. No replay pruning hook is installed by this runner.
Raw logs, per-test durations, worker wall times, plans and results are retained
under --work-dir (default: a fresh scratch/test-suite-* directory). Child output
streams live with prefixes, leaving exactly one unprefixed aggregate Ran summary.
Subtest outcomes never inflate the test count; runner errors are separate counts.
"""
from __future__ import annotations

import argparse
import json
import math
import os
import select
import signal
import subprocess
import sys
import tempfile
import threading
import time
import traceback
import unittest
from collections import Counter
from dataclasses import dataclass, field
from pathlib import Path

_MAX_JOBS = 32
_LIVE_CLASS = "test_verification.LiveVerificationTests"
_COUNTS = ("tests", "failures", "errors", "skipped", "expectedFailures", "unexpectedSuccesses")
_OUTPUT_LOCK = threading.Lock()
_STOP_SIGNALS = (signal.SIGTERM, signal.SIGINT)


def _jobs(value: str) -> int:
    number = int(value)
    if not 1 <= number <= _MAX_JOBS:
        raise argparse.ArgumentTypeError(f"jobs must be between 1 and {_MAX_JOBS}")
    return number


def _flatten(suite):
    for test in suite:
        if isinstance(test, unittest.TestSuite):
            yield from _flatten(test)
        else:
            yield test


def _discover(start: Path):
    loader = unittest.TestLoader()
    tests = list(_flatten(loader.discover(str(start))))
    ids = [test.id() for test in tests]
    if len(set(ids)) != len(ids):
        raise ValueError("Duplicate discovered test IDs")
    return tests, ids


def _timings() -> dict:
    data = json.loads(Path(__file__).with_name("test_suite_timings.json").read_text())
    seconds = data["test_seconds"]
    values = [data["unknown_test_seconds"], *seconds.values()]
    if (data["schema_version"] != 1 or not isinstance(seconds, dict)
            or any(not isinstance(key, str) for key in seconds)
            or any(type(value) not in (int, float) or not math.isfinite(value) or value <= 0
                   for value in values)):
        raise ValueError("Invalid test scheduling timings")
    return data


def _partition(ids: list[str], live: list[str], jobs: int,
               timings: dict | None = None) -> list[list[str]]:
    if not 1 <= jobs <= _MAX_JOBS:
        raise ValueError(f"jobs must be between 1 and {_MAX_JOBS}")
    if (any(not isinstance(name, str) for name in [*ids, *live])
            or len(set(ids)) != len(ids) or len(set(live)) != len(live)
            or not set(live) <= set(ids)):
        raise ValueError("Invalid discovered test partition")
    timing = _timings() if timings is None else timings
    def cost(name):
        return timing["test_seconds"].get(name, timing["unknown_test_seconds"])

    live_set = set(live)
    groups = [[name for name in ids if name not in live_set]]
    shards = [set() for _ in range(min(jobs, len(live)))]
    loads = [0.0] * len(shards)
    # Equal costs use the full ID; equal loads use the lowest worker index.
    for name in sorted(live, key=lambda name: (-cost(name), name)):
        index = min(range(len(shards)), key=lambda index: (loads[index], index))
        shards[index].add(name)
        loads[index] += cost(name)
    groups.extend([name for name in ids if name in shard] for shard in shards)
    if Counter(name for group in groups for name in group) != Counter(ids):
        raise ValueError("Test partition omits or duplicates tests")
    return groups


def _guard_parent(owner_fd: int) -> None:
    # The runner alone owns the writer. EOF also covers death before startup.
    if os.read(owner_fd, 1) != b"\x01":
        os.killpg(0, signal.SIGKILL)
    # Fork before discovery can start threads or toolchain descendants. A thread
    # would disappear with an early-exiting leader, leaving its descendants.
    if os.fork() == 0:
        try:
            for signum in _STOP_SIGNALS:
                signal.signal(signum, signal.SIG_IGN)
            signal.pthread_sigmask(signal.SIG_UNBLOCK, _STOP_SIGNALS)
            for fd in (0, 1, 2):
                os.close(fd)
            while os.read(owner_fd, 1):
                pass
        finally:
            # Self-relative identity: never look up or signal a remembered PID.
            os.killpg(0, signal.SIGKILL)
            os._exit(2)
    os.close(owner_fd)


def _child(start: Path, directory: Path, plan_path: Path | None, shard: int) -> int:
    # Spawn/registration and helper-thread startup are signal-atomic in the parent.
    signal.pthread_sigmask(signal.SIG_UNBLOCK, _STOP_SIGNALS)
    began = time.monotonic()
    tests, ids = _discover(start)
    if plan_path is None:
        live = [test.id() for test in tests
                if f"{type(test).__module__}.{type(test).__qualname__}" == _LIVE_CLASS]
        (directory / "result.json").write_text(json.dumps({
            "ids": ids, "live": live, "wall_seconds": time.monotonic() - began}) + "\n")
        return 0
    plan = json.loads(plan_path.read_text())
    if ids != plan["ids"]:
        raise ValueError("Discovered tests changed between planning and execution")
    selected = plan["groups"][shard]
    wanted = set(selected)
    suite = unittest.TestSuite(test for test in tests if test.id() in wanted)
    if [test.id() for test in _flatten(suite)] != selected:
        raise ValueError("Worker test selection omits or duplicates tests")
    durations = []
    with (directory / "events.jsonl").open("w", encoding="utf-8") as events:
        class Result(unittest.TextTestResult):
            def startTest(self, test):
                super().startTest(test)
                self.test_began = time.monotonic()
                events.write(json.dumps(test.id()) + "\n")
                events.flush()

            def stopTest(self, test):
                durations.append({"test": test.id(),
                                  "wall_seconds": time.monotonic() - self.test_began})
                super().stopTest(test)

        result = unittest.TextTestRunner(
            verbosity=2, resultclass=Result, warnings=None if sys.warnoptions else "default").run(suite)
    payload = {"tests": result.testsRun, "success": result.wasSuccessful(),
               "failures": len(result.failures), "errors": len(result.errors),
               "skipped": len(result.skipped), "expectedFailures": len(result.expectedFailures),
               "unexpectedSuccesses": len(result.unexpectedSuccesses),
               "test_durations": durations, "wall_seconds": time.monotonic() - began}
    (directory / "result.json").write_text(json.dumps(payload, indent=2) + "\n")
    return 0 if result.wasSuccessful() else 1


def _environment(temporary: Path, name: str) -> dict[str, str]:
    env = {**os.environ, "TMPDIR": str(temporary), "TMP": str(temporary),
           "TEMP": str(temporary), "PYTHONUNBUFFERED": "1"}
    root = env.get("SHIREN2_REPLAY_EVIDENCE_ROOT")
    if root and Path(root).is_absolute() and not Path(root).is_symlink():
        env["SHIREN2_REPLAY_EVIDENCE_ROOT"] = str(Path(root) / name)
    return env


def _line(name: str, line: bytes) -> None:
    # One locked write per complete line prevents concurrent worker interleaving.
    text = line.decode("utf-8", errors="backslashreplace")
    with _OUTPUT_LOCK:
        print(f"[{name}] {text}", end="" if text.endswith("\n") else "\n", flush=True)


@dataclass
class _Worker:
    process: subprocess.Popen
    began: float
    owner_fd: int | None
    exit_queue: select.kqueue
    thread: threading.Thread | None = None
    waiter: threading.Thread | None = None
    wall_seconds: float = 0.0
    stream_errors: list[str] = field(default_factory=list)
    exited: threading.Event = field(default_factory=threading.Event)
    observer_done: threading.Event = field(default_factory=threading.Event)
    retired: bool = False

    def wait(self) -> int:
        if self.waiter is not None and self.waiter.ident is not None:
            # Event.wait remains interruptible without Thread.join's interrupted
            # lock bookkeeping prematurely marking the observer as stopped.
            self.observer_done.wait()
        _stop_workers([self])
        assert self.process.returncode is not None
        return self.process.returncode


def _observe_exit(worker: _Worker) -> None:
    # NOTE_EXIT observes without reaping. The zombie reserves the leader PID
    # until group retirement, even when later shards finish much earlier.
    try:
        worker.exit_queue.control(None, 1, None)
        worker.wall_seconds = time.monotonic() - worker.began
        worker.exited.set()
    except OSError as error:
        worker.stream_errors.append(f"Worker exit observation failed: {error}")
    finally:
        worker.observer_done.set()


def _stream(worker: _Worker, directory: Path, log) -> None:
    pending = b""
    assert worker.process.stdout is not None
    try:
        with log, worker.process.stdout as stream:
            while chunk := os.read(stream.fileno(), 65536):
                log.write(chunk)
                log.flush()
                lines = (pending + chunk).split(b"\n")
                pending = lines.pop()
                for line in lines:
                    _line(directory.name, line + b"\n")
            if pending:
                _line(directory.name, pending)
    except (OSError, ValueError) as error:
        worker.stream_errors.append(f"Output streaming failed: {error}")


def _signal_group(worker: _Worker, signum: int) -> bool:
    if worker.retired:
        raise RuntimeError("Cannot signal a retired worker group")
    try:
        os.killpg(worker.process.pid, signum)
        return True
    except ProcessLookupError:
        return False
    except OSError as error:
        worker.stream_errors.append(f"Worker group signal {signum} failed: {error}")
        return False


def _stop_workers(launched: list[_Worker]) -> None:
    # Single owner of all group signalling, retirement and reaping. No Popen
    # poll/wait is allowed elsewhere; neither a signal nor an error may split
    # this sequence and later revisit a freed group identity.
    pending = [worker for worker in launched if not worker.retired]
    previous_mask = signal.pthread_sigmask(signal.SIG_BLOCK, _STOP_SIGNALS)
    try:
        running = False
        for worker in pending:
            if not worker.exited.is_set():
                running = _signal_group(worker, signal.SIGTERM) or running
        if running:
            time.sleep(1.0)
        for worker in pending:
            _signal_group(worker, signal.SIGKILL)
            # EOF is also a fallback if signalling failed, and the guard
            # survives TERM and leader exit until the group is killed.
            if worker.owner_fd is not None:
                os.close(worker.owner_fd)
                worker.owner_fd = None
            worker.retired = True
        for worker in pending:
            worker.process.wait()
            if worker.waiter is not None and worker.waiter.ident is not None:
                worker.waiter.join()
            worker.exit_queue.close()
        # All groups are retired before any stdout reader can block on EOF.
        for worker in pending:
            if worker.thread is not None and worker.thread.ident is not None:
                worker.thread.join()
            if worker.process.stdout is not None:
                worker.process.stdout.close()
    finally:
        signal.pthread_sigmask(signal.SIG_SETMASK, previous_mask)


def _launch(start: Path, root: Path, name: str, launched: list[_Worker],
            plan: Path | None = None, shard: int = 0):
    directory = root / name
    directory.mkdir()
    temporary = directory / "tmp"
    temporary.mkdir()
    command = [sys.executable, "-u", str(Path(__file__).resolve()), "--start-directory", str(start),
               "--child-directory", str(directory)]
    if plan is not None:
        command.extend(["--plan", str(plan), "--shard", str(shard)])
    log = (directory / "output.log").open("wb")
    exit_queue = select.kqueue()
    owner_read, owner_write = os.pipe()
    command.extend(["--owner-fd", str(owner_read)])
    worker = None
    began = time.monotonic()
    previous_mask = signal.pthread_sigmask(signal.SIG_BLOCK, _STOP_SIGNALS)
    try:
        process = subprocess.Popen(command, stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
                                   env=_environment(temporary, name), start_new_session=True,
                                   pass_fds=(owner_read,))
        worker = _Worker(process, began, owner_write, exit_queue)
        launched.append(worker)
        # The child waits for the byte below before forking its guard or
        # discovering tests, so even an empty shard cannot outrun registration.
        exit_queue.control([select.kevent(process.pid, filter=select.KQ_FILTER_PROC,
                                         flags=select.KQ_EV_ADD | select.KQ_EV_ONESHOT,
                                         fflags=select.KQ_NOTE_EXIT)], 0, 0)
        worker.waiter = threading.Thread(target=_observe_exit, args=(worker,))
        worker.waiter.start()
        worker.thread = threading.Thread(target=_stream, args=(worker, directory, log))
        worker.thread.start()
        os.write(owner_write, b"\x01")
    except BaseException:
        log.close()
        if worker is None:
            os.close(owner_write)
            exit_queue.close()
        raise
    finally:
        os.close(owner_read)
        signal.pthread_sigmask(signal.SIG_SETMASK, previous_mask)
    return worker, directory


def _collect(worker: _Worker, directory: Path, selected: list[str]) -> tuple[int, bool, dict]:
    code = worker.wait()
    errors = list(worker.stream_errors)
    started = []
    result = {}
    try:
        with (directory / "events.jsonl").open(encoding="utf-8") as events:
            for line in events:
                name = json.loads(line)
                if not isinstance(name, str):
                    raise ValueError("Invalid started test ID")
                started.append(name)
    except (OSError, ValueError, TypeError) as error:
        errors.append(f"Worker events unavailable or invalid: {error}")
    try:
        result = json.loads((directory / "result.json").read_text())
        if not isinstance(result, dict):
            result = {}
            raise ValueError("Worker result must be an object")
        if (any(type(result.get(key)) is not int or result[key] < 0 for key in _COUNTS)
                or type(result.get("success")) is not bool):
            raise ValueError("Invalid worker result counts or success")
    except (OSError, ValueError, TypeError) as error:
        errors.append(f"Worker result unavailable or invalid: {error}")
    if result.get("tests") != len(started) or started != selected:
        errors.append("Worker result count or test IDs disagree with its assigned tests")
    # Preserve every usable worker count even if assignment, events or another field
    # is invalid. Runner protocol errors must never masquerade as unittest errors.
    counts = {key: result[key] for key in _COUNTS
              if type(result.get(key)) is int and result[key] >= 0}
    counts.setdefault("tests", len(started))
    success = (not errors and code == 0 and result.get("success") is True
               and not any(counts.get(key, 0) for key in ("failures", "errors", "unexpectedSuccesses")))
    if code != 0 and not errors and result.get("success") is True:
        errors.append(f"Worker exit {code} disagrees with successful result")
    for error in errors:
        _line(directory.name, f"Worker failed (exit {code}): {error}; evidence: {directory}".encode())
    collected = {**counts, "success": success, "runner_errors": errors, "exit_code": code,
                 "assigned_ids": selected, "started_ids": started,
                 "worker_wall_seconds": worker.wall_seconds,
                 "test_durations": result.get("test_durations", [])}
    (directory / "collection.json").write_text(json.dumps(collected, indent=2) + "\n")
    return counts["tests"], success, collected


def _run(start: Path, root: Path, jobs: int) -> int:
    began = time.monotonic()
    total = 0
    success = True
    outcomes = Counter({key: 0 for key in _COUNTS if key != "tests"})
    runner_errors = []
    workers = {}
    launched = []
    stopping = False
    error_message = None

    def interrupted(signum, frame):
        nonlocal stopping
        if not stopping:
            stopping = True
            raise KeyboardInterrupt(f"Interrupted by {signal.Signals(signum).name}")

    previous_handlers = {signum: signal.signal(signum, interrupted) for signum in _STOP_SIGNALS}
    try:
        discovery, directory = _launch(start, root, "discovery", launched)
        code = discovery.wait()
        launched.remove(discovery)
        workers["discovery"] = {"exit_code": code, "worker_wall_seconds": discovery.wall_seconds}
        if code or discovery.stream_errors:
            raise ValueError(f"Test discovery failed (exit {code}): {discovery.stream_errors}")
        found = json.loads((directory / "result.json").read_text())
        timing = _timings()
        groups = _partition(found["ids"], found["live"], jobs, timing)
        plan = root / "plan.json"
        plan.write_text(json.dumps({
            "schema_version": 1, "ids": found["ids"], "groups": groups,
            "scheduling": "lpt", "timings": timing,
            "estimated_group_seconds": [sum(timing["test_seconds"].get(
                name, timing["unknown_test_seconds"]) for name in group) for group in groups]
        }, indent=2) + "\n")
        phases = [[0], list(range(1, len(groups)))]
        for phase in phases:
            children = []
            for index in phase:
                name = "serial" if index == 0 else f"live-{index}"
                worker, directory = _launch(start, root, name, launched, plan, index)
                children.append((worker, directory, groups[index]))
            for worker, directory, selected in children:
                count, passed, result = _collect(worker, directory, selected)
                launched.remove(worker)
                workers[directory.name] = result
                total += count
                success = success and passed
                runner_errors.extend(f"{directory.name}: {error}" for error in result["runner_errors"])
                outcomes.update({key: result.get(key, 0) for key in _COUNTS if key != "tests"})
    except BaseException as error:
        error_message = str(error) or type(error).__name__
        success = False
        runner_errors.append(error_message)
    finally:
        stopping = True
        try:
            _stop_workers(launched)
        finally:
            for signum, handler in previous_handlers.items():
                signal.signal(signum, handler)
    if error_message is not None:
        _line("runner", f"{error_message}; evidence: {root}".encode())
    elapsed = time.monotonic() - began
    payload = {"schema_version": 1, "tests": total, **outcomes, "success": success,
               "runner_errors": runner_errors, "wall_seconds": elapsed, "workers": workers}
    (root / "result.json").write_text(json.dumps(payload, indent=2) + "\n")
    print(f"\nRan {total} tests in {elapsed:.3f}s\n", flush=True)
    details = [f"{key}={value}" for key, value in outcomes.items() if value]
    if runner_errors:
        details.append(f"runner_errors={len(runner_errors)}")
    print(("OK" if success else "FAILED") + (f" ({', '.join(details)})" if details else ""), flush=True)
    return 0 if success else 1


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--jobs", type=_jobs, default=1)
    parser.add_argument("--start-directory", type=Path, default=Path("tests"))
    parser.add_argument("--work-dir", type=Path, help="new directory for retained worker evidence")
    parser.add_argument("--child-directory", type=Path, help=argparse.SUPPRESS)
    parser.add_argument("--owner-fd", type=int, help=argparse.SUPPRESS)
    parser.add_argument("--plan", type=Path, help=argparse.SUPPRESS)
    parser.add_argument("--shard", type=int, default=0, help=argparse.SUPPRESS)
    args = parser.parse_args()
    start = args.start_directory.resolve()
    if args.child_directory is not None:
        try:
            if args.owner_fd is None:
                raise ValueError("Worker requires its runner's ownership pipe")
            _guard_parent(args.owner_fd)
            return _child(start, args.child_directory, args.plan, args.shard)
        except Exception:
            traceback.print_exc()
            return 2
    if args.work_dir is None:
        scratch = Path("scratch")
        scratch.mkdir(exist_ok=True)
        root = Path(tempfile.mkdtemp(prefix="test-suite-", dir=scratch)).resolve()
    else:
        root = args.work_dir.resolve()
        root.mkdir(parents=True, exist_ok=False)
    print(f"Suite evidence: {root}", flush=True)
    return _run(start, root, args.jobs)


if __name__ == "__main__":
    sys.exit(main())
