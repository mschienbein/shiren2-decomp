"""Match/adoption evidence must survive same-second concurrent invocations."""
from __future__ import annotations

import multiprocessing
import sys
import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "tools"))
import match

STAMP = "20261009T120000"


def _racing_allocation(parent: str, worker: int, barrier, results) -> None:
    # Freeze the wall clock in both spawned processes, not merely the parent.
    with patch.object(match.time, "strftime", return_value=STAMP):
        barrier.wait(timeout=30)
        directory = match.allocate_evidence_dir(Path(parent))
    with (directory / "result.json").open("x") as stream:
        stream.write(f"worker {worker}\n")
    results.put((worker, str(directory)))


class EvidenceDirectoryTests(unittest.TestCase):
    def test_same_second_allocations_preserve_existing_evidence(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            parent = Path(temporary) / "scratch/omp/match/func_A"
            with patch.object(match.time, "strftime", return_value=STAMP):
                first = match.allocate_evidence_dir(parent)
                (first / "result.json").write_text("first result\n")
                second = match.allocate_evidence_dir(parent)
                (second / "result.json").write_text("second result\n")
            self.assertNotEqual(first, second)
            self.assertEqual(first.parent, parent)
            self.assertEqual(second.parent, parent)
            self.assertTrue(first.name.startswith(STAMP + "-"))
            self.assertTrue(second.name.startswith(STAMP + "-"))
            self.assertEqual((first / "result.json").read_text(), "first result\n")
            self.assertEqual((second / "result.json").read_text(), "second result\n")

    def test_prefixed_names_sort_by_timestamp(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            parent = Path(temporary) / "scratch/omp/adopt"
            with patch.object(match.time, "strftime", side_effect=["20261009T120001", STAMP]):
                newer = match.allocate_evidence_dir(parent, prefix="func_A-")
                older = match.allocate_evidence_dir(parent, prefix="func_A-")
            self.assertTrue(older.is_dir())
            self.assertTrue(newer.is_dir())
            self.assertTrue(older.name.startswith(f"func_A-{STAMP}-"))
            self.assertEqual(sorted([newer, older]), [older, newer])

    def test_non_directory_parent_is_not_replaced(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            parent = Path(temporary) / "evidence"
            parent.write_text("keep this file\n")
            with self.assertRaises(FileExistsError):
                match.allocate_evidence_dir(parent)
            self.assertEqual(parent.read_text(), "keep this file\n")

    def test_two_processes_racing_in_one_second_keep_distinct_results(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            parent = Path(temporary) / "scratch/omp/match/func_A"
            context = multiprocessing.get_context("spawn")
            barrier = context.Barrier(2)
            results = context.Queue()
            processes = [context.Process(target=_racing_allocation, args=(str(parent), worker, barrier, results))
                         for worker in range(2)]
            try:
                for process in processes:
                    process.start()
                allocations = [results.get(timeout=30) for _ in processes]
                for process in processes:
                    process.join(timeout=30)
                    self.assertEqual(process.exitcode, 0)
                directories = [Path(directory) for _, directory in allocations]
                self.assertNotEqual(directories[0], directories[1])
                self.assertEqual(set(parent.iterdir()), set(directories))
                for worker, directory in allocations:
                    path = Path(directory)
                    self.assertTrue(path.name.startswith(STAMP + "-"))
                    self.assertEqual((path / "result.json").read_text(), f"worker {worker}\n")
            finally:
                for process in processes:
                    if process.is_alive():
                        process.terminate()
                    if process.pid is not None:
                        process.join(timeout=30)
                results.close()
                results.join_thread()


if __name__ == "__main__":
    unittest.main()
