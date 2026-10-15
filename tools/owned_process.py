"""Ownership-preserving child process groups (stdlib only; macOS kqueue).

This reuses the reviewed H14 r4 `tools/test_suite.py` invariant (`_launch`, `_observe_exit`,
`_signal_group`, `_stop_workers`, `_guard_parent`) for one child group:

* The child is a new session/group leader and receives the read end of an ownership pipe.
  Spawning, kqueue registration and the registration byte are signal-atomic.
* Leader exit is observed with kqueue NOTE_EXIT, which does not reap. The unreaped leader
  keeps its PID, and therefore its group ID, reserved until retirement.
* `retire()` is the only place that signals or reaps. It signals the group (TERM with a grace
  period while the leader still runs, then always KILL) strictly before reaping the leader.
  A retired group is never signalled again.
* Retirement runs on every path: ordinary exit 0, nonzero exit, timeout, exception and
  interrupt. Same-group descendants therefore never outlive `run_owned`.
* The child calls `guard_owner(fd)` first. It forks a guard that SIGKILLs its own group
  (`killpg(0)`, self-relative) on pipe EOF. This covers the parent dying, including by SIGKILL,
  without any process ever signalling a remembered PID. It then unblocks the stop signals it
  inherited from the signal-atomic spawn, so the TERM grace period really stops it.
"""
from __future__ import annotations

import os
import select
import signal
import subprocess
import time
from pathlib import Path

STOP_SIGNALS = {signal.SIGINT, signal.SIGTERM, signal.SIGHUP}
OWNER_FD = "{owner_fd}"


class OwnedGroup:
    def __init__(self, process: subprocess.Popen, owner_write: int, queue: select.kqueue, exited: bool):
        self.process = process
        self.owner_write: int | None = owner_write
        self.queue = queue
        self.exited = exited
        self.retired = False
        self.signals: list[tuple[int, bool]] = []  # (signum, leader_reaped_at_request) evidence
        self.errors: list[str] = []

    def wait_exit(self, timeout: float | None) -> bool:
        """Block until the leader exits (without reaping) or `timeout` elapses."""
        if self.exited:
            return True
        if self.queue.control(None, 1, timeout):
            self.exited = True
        return self.exited

    def _signal(self, signum: int) -> None:
        if self.retired:
            raise RuntimeError("cannot signal a retired group")
        if self.process.returncode is not None:
            raise RuntimeError("cannot signal a group whose leader was reaped")
        self.signals.append((signum, self.process.returncode is not None))
        try:
            os.killpg(self.process.pid, signum)
        except ProcessLookupError:
            pass
        except OSError as error:  # recorded; retirement still closes the owner pipe and reaps
            self.errors.append(f"killpg({signum}) failed: {error}")

    def retire(self, grace: float) -> None:
        """Single owner of signalling and reaping: TERM (if running), KILL, retire, then reap."""
        if self.retired:
            return
        previous = signal.pthread_sigmask(signal.SIG_BLOCK, STOP_SIGNALS)
        try:
            if not self.exited:
                self._signal(signal.SIGTERM)
                deadline = time.monotonic() + grace
                while not self.exited and time.monotonic() < deadline:
                    self.wait_exit(max(0.0, deadline - time.monotonic()))
            self._signal(signal.SIGKILL)
            if self.owner_write is not None:
                os.close(self.owner_write)
                self.owner_write = None
            self.retired = True
            self.process.wait()
            self.queue.close()
        finally:
            signal.pthread_sigmask(signal.SIG_SETMASK, previous)


def launch(argv: list[str], log, owned: list[OwnedGroup], *, cwd: Path, env: dict,
           pass_fds: tuple[int, ...] = ()) -> OwnedGroup:
    """Spawn `argv` (OWNER_FD placeholders replaced) as an owned group, registered in `owned`
    before stop signals are unblocked, so an interrupt can never orphan it."""
    owner_read, owner_write = os.pipe()
    queue = select.kqueue()
    group = None
    previous = signal.pthread_sigmask(signal.SIG_BLOCK, STOP_SIGNALS)
    try:
        argv = [str(owner_read) if a == OWNER_FD else a for a in argv]
        process = subprocess.Popen(argv, cwd=cwd, env=env, stdout=log, stderr=subprocess.STDOUT,
                                   start_new_session=True, pass_fds=(owner_read, *pass_fds))
        # Owned from the first instruction after spawn: any later failure is retired by the caller.
        group = OwnedGroup(process, owner_write, queue, exited=False)
        owned.append(group)
        try:
            queue.control([select.kevent(process.pid, filter=select.KQ_FILTER_PROC,
                                         flags=select.KQ_EV_ADD | select.KQ_EV_ONESHOT,
                                         fflags=select.KQ_NOTE_EXIT)], 0, 0)
        except ProcessLookupError:
            group.exited = True  # already a zombie: still unreaped, so its group ID stays reserved
        os.write(owner_write, b"\x01")
    except BaseException:
        if group is None:
            os.close(owner_write)
            queue.close()
        raise
    finally:
        os.close(owner_read)
        signal.pthread_sigmask(signal.SIG_SETMASK, previous)
    return group


def run_owned(argv: list[str], log_path: Path, *, cwd: Path, env: dict, timeout: float, grace: float,
              pass_fds: tuple[int, ...] = ()) -> dict:
    """Run one owned group to completion, timeout or interrupt; the group never outlives this call.
    An interrupt (SIGINT/SIGTERM/SIGHUP) retires the group and re-raises as KeyboardInterrupt."""
    started = time.monotonic()
    interrupted: list[str] = []

    def stop(signum, _frame):
        if not interrupted:
            interrupted.append(signal.Signals(signum).name)
            raise KeyboardInterrupt(f"interrupted by {interrupted[0]}")

    handlers = {signum: signal.signal(signum, stop) for signum in STOP_SIGNALS}
    owned: list[OwnedGroup] = []
    timed_out = False
    try:
        with log_path.open("w") as log:
            group = launch(argv, log, owned, cwd=cwd, env=env, pass_fds=pass_fds)
            deadline = started + timeout
            while not group.wait_exit(max(0.0, deadline - time.monotonic())):
                if time.monotonic() >= deadline:
                    timed_out = True
                    break
    finally:
        try:
            for member in owned:
                member.retire(grace)
        finally:
            for signum, handler in handlers.items():
                signal.signal(signum, handler)
    group = owned[0]
    return {"exit_code": group.process.returncode, "timed_out": timed_out, "interrupted": interrupted or None,
            "seconds": round(time.monotonic() - started, 3),
            "group_signals": [signal.Signals(s).name for s, _ in group.signals],
            "signals_after_reap": sum(1 for _, reaped in group.signals if reaped), "signal_errors": group.errors}


def guard_owner(owner_fd: int) -> None:
    """First call in an owned child: kill this whole group when the owner pipe reaches EOF."""
    if os.read(owner_fd, 1) != b"\x01":
        os.killpg(0, signal.SIGKILL)
    # Fork before any thread or toolchain descendant exists.
    if os.fork() == 0:
        try:
            for signum in STOP_SIGNALS:
                signal.signal(signum, signal.SIG_IGN)
            signal.pthread_sigmask(signal.SIG_UNBLOCK, STOP_SIGNALS)
            for fd in (0, 1, 2):
                os.close(fd)
            while os.read(owner_fd, 1):
                pass
        finally:
            # Self-relative identity: never look up or signal a remembered PID.
            os.killpg(0, signal.SIGKILL)
            os._exit(2)
    os.close(owner_fd)
    # `launch` blocks stop signals across the spawn so registration is signal-atomic, and the
    # mask is inherited through fork/exec. Unblock here so the owner's TERM grace actually
    # stops this process. A non-Python leader (uv) keeps the inherited mask and exits when this
    # child exits; KILL always follows anyway.
    signal.pthread_sigmask(signal.SIG_UNBLOCK, STOP_SIGNALS)
