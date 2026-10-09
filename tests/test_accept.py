"""Acceptance policy fixtures: real receipt/review files, no builds or promotion.

Only external build/test/verify/progress commands and live manifest verification
are substituted; prepare's classification and finalize's review gate run normally.
"""
from __future__ import annotations

import copy
import hashlib
import io
import json
import os
import sys
import tempfile
import threading
import time
import unittest
from contextlib import redirect_stdout
from pathlib import Path
from unittest.mock import patch

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "tools"))
import accept
import certification


def match(symbol: str, source: str, index: int, image_id: str = "resident") -> dict:
    return {"image_id": image_id, "symbol": symbol, "source": source,
            "rom_start": 0x1000 + index * 8, "size": 8}


def receipt(matches: list[dict], sources: dict[str, str], dependencies: dict[str, list[str]] | None = None) -> dict:
    files = {path: {"sha256": hashlib.sha256(text.encode()).hexdigest(), "size": len(text.encode())}
             for path, text in sources.items()}
    c = [m for m in matches if m["source"].endswith(".c")]
    cpp = [m for m in matches if m["source"].endswith(".cpp")]
    coverage = {"matched_c_functions": len(c), "matched_c_bytes": sum(m["size"] for m in c)}
    if cpp:
        coverage.update(matched_cpp_functions=len(cpp), matched_cpp_bytes=sum(m["size"] for m in cpp),
                        matched_c_and_cpp_functions=len(matches), matched_c_and_cpp_bytes=sum(m["size"] for m in matches))
    return {"c_matches": matches, "input_manifest": {"schema_version": 1, "files": files,
            "sha256": hashlib.sha256(json.dumps(files, sort_keys=True).encode()).hexdigest()}, "coverage": coverage,
            "dependencies": dependencies if dependencies is not None else {m["source"]: [m["source"]] for m in matches}}


class AcceptanceReviewTests(unittest.TestCase):
    name = "fixture"

    def setUp(self) -> None:
        self.temporary = tempfile.TemporaryDirectory()
        self.addCleanup(self.temporary.cleanup)
        self.root = Path(self.temporary.name).resolve()
        self.work = self.root / "scratch/omp/accept" / self.name
        self.prior = receipt([
            match("unchanged", "src/unchanged.c", 0),
            match("edited", "src/edited.c", 1),
            match("moved", "src/moved.c", 2),
            match("absorbed", "src/absorbed.c", 3),
            match("reused", "src/joint.c", 4),
            match("edited_cpp", "src/edited.cpp", 5),
        ], {"src/unchanged.c": "int unchanged(void) { return 0; }\n",
            "src/edited.c": "int edited(void) { return 1; }\n",
            "src/moved.c": "int moved(void) { return 2; }\n",
            "src/absorbed.c": "int absorbed(void) { return 3; }\n",
            "src/joint.c": "int reused(void) { return 4; }\n",
            "src/edited.cpp": 'extern "C" int edited_cpp() { return 5; }\n'})
        self.current = receipt([
            match("unchanged", "src/unchanged.c", 0),
            match("edited", "src/edited.c", 1),
            match("moved", "src/renamed.c", 2),
            match("absorbed", "src/joint.c", 3),
            match("reused", "src/joint.c", 4),
            match("edited_cpp", "src/edited.cpp", 5),
            match("new", "src/new.c", 6),
        ], {"src/unchanged.c": "int unchanged(void) { return 0; }\n",
            "src/edited.c": "int edited(void) { return 7; }\n",
            "src/renamed.c": "int moved(void) { return 2; }\n",
            "src/joint.c": "int absorbed(void) { return 3; }\nint reused(void) { return 4; }\n",
            "src/edited.cpp": 'extern "C" int edited_cpp() { return 8; }\n',
            "src/new.c": "int new(void) { return 6; }\n"})
        self.write_json("build/prior/receipt.json", self.prior)
        self.write_json("build/latest.json", {"receipt": "build/prior/receipt.json",
                                             "receipt_sha256": accept._sha256(self.root / "build/prior/receipt.json")})
        for suffix in ("a", "b"):
            self.write_json(f"build/{self.name}-{suffix}/receipt.json", self.current)
            self.write_json(f"build/{self.name}-{suffix}/input-manifest.json", self.current["input_manifest"])
        (self.root / "docs").mkdir()
        for key, value in (("PROJECT", self.root), ("python_command", lambda: sys.executable)):
            handle = patch.object(accept, key, value)
            handle.start()
            self.addCleanup(handle.stop)
        handle = patch.object(accept, "_run", side_effect=self.run_command)
        self.commands = handle.start()
        self.addCleanup(handle.stop)
        handle = patch.object(accept, "verify_manifest")
        self.verify_manifest = handle.start()
        self.addCleanup(handle.stop)

    def write_json(self, relative: str, data: dict) -> Path:
        path = self.root / relative
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_text(json.dumps(data, indent=2) + "\n")
        return path

    def run_command(self, argv: list[str], log: Path, env: dict | None = None) -> tuple[int, float]:
        log.write_text("Ran 1 test in 0.001s\n\nOK\n" if "unittest" in argv else "fixture command\n")
        return 0, 0.001

    def prepare(self) -> dict:
        prepared = accept.read_json(accept.prepare(self.name))
        self.commands.reset_mock()
        return prepared

    def review(self, prepared: dict, symbols: list[str], *, shard: bool = False) -> Path:
        data = {"status": "PASS", "reviewer": "independent-fixture",
                "receipt_hashes": {key: value["sha256"] for key, value in prepared["receipts"].items()},
                "current_frozen_manifest_sha256": self.current["input_manifest"]["sha256"],
                "coverage": self.current["coverage"], "reviewed_functions": symbols}
        return self.write_json(f"scratch/omp/accept/{self.name}/{'shard' if shard else 'review'}.json", data)

    def prepare_header_change(self, header: str) -> dict:
        matches = [match("first", "src/shared.c", 0), match("second", "src/shared.c", 1),
                   match("cpp", "src/shared.cpp", 2), match("untouched", "src/untouched.c", 3)]
        sources = {
            "src/shared.c": '#include "outer.h"\nint first(void) { return VALUE; }\nint second(void) { return VALUE; }\n',
            "src/shared.cpp": '#include "outer.h"\nextern "C" int cpp() { return VALUE; }\n',
            "src/untouched.c": "int untouched(void) { return 0; }\n",
            "include/outer.h": '#include "inner.h"\n',
            "include/inner.h": "#define VALUE 1\n",
            "include/unrelated.h": "#define UNRELATED 1\n",
        }
        dependencies = {
            source: [source, "include/outer.h", "include/inner.h"]
            for source in ("src/shared.c", "src/shared.cpp")
        }
        dependencies["src/untouched.c"] = ["src/untouched.c"]
        self.prior = receipt(matches, sources, dependencies)
        self.current = receipt(matches, {**sources, header: sources[header].replace("1", "2")}, dependencies)
        prior_path = self.write_json("build/prior/receipt.json", self.prior)
        self.write_json("build/latest.json", {"receipt": "build/prior/receipt.json",
                                             "receipt_sha256": accept._sha256(prior_path)})
        for suffix in ("a", "b"):
            self.write_json(f"build/{self.name}-{suffix}/receipt.json", self.current)
            self.write_json(f"build/{self.name}-{suffix}/input-manifest.json", self.current["input_manifest"])
        return self.prepare()

    def test_header_only_change_requires_review_of_every_dependent_c_and_cpp_function(self) -> None:
        prepared = self.prepare_header_change("include/inner.h")
        self.assertEqual([m["symbol"] for m in prepared["changed_functions"]], ["first", "second", "cpp"])
        for changed in prepared["changed_functions"]:
            self.assertEqual(changed["changed_dependencies"], ["include/inner.h"])
            source = changed["source"]
            self.assertEqual(self.prior["input_manifest"]["files"][source],
                             self.current["input_manifest"]["files"][source])
        self.assertFalse(any("changed_dependencies" in m for m in self.current["c_matches"]))
        self.assertEqual(prepared["new_functions"], [])
        self.assertEqual(prepared["new_instruction_bytes"], 0)
        # A header-only change still reaches the existing gate before any
        # manifest check or promotion, including every function in a shared TU.
        self.review(prepared, ["first", "cpp"])
        with self.assertRaisesRegex(ValueError, "new or changed function.*'second'"):
            accept.finalize(self.name, [])
        self.commands.assert_not_called()
        self.verify_manifest.assert_not_called()
        self.review(prepared, ["first", "second", "cpp"])
        validation = accept.read_json(accept.finalize(self.name, []))
        self.assertEqual(validation["changed_functions"], prepared["changed_functions"])
        self.assertEqual(validation["new_C_functions"], 0)
        self.assertEqual(validation["new_unique_C_instruction_bytes"], 0)
        self.assertEqual(validation["new_CPP_functions"], 0)
        self.assertEqual(validation["new_unique_CPP_instruction_bytes"], 0)
        self.verify_manifest.assert_called_once_with(self.root, self.current["input_manifest"])

    def test_unrelated_header_change_does_not_require_source_rereview(self) -> None:
        prepared = self.prepare_header_change("include/unrelated.h")
        self.assertNotEqual(self.prior["input_manifest"]["files"]["include/unrelated.h"],
                            self.current["input_manifest"]["files"]["include/unrelated.h"])
        self.assertEqual(prepared["changed_functions"], [])
        self.assertEqual(prepared["new_functions"], [])
        self.review(prepared, [])
        validation = accept.read_json(accept.finalize(self.name, []))
        self.assertEqual(validation["changed_functions"], [])
        self.assertEqual(self.commands.call_count, 3)
        self.verify_manifest.assert_called_once_with(self.root, self.current["input_manifest"])

    def test_added_and_removed_dependencies_are_recorded_even_with_unchanged_file_hashes(self) -> None:
        matches = [match("function", "src/function.c", 0)]
        sources = {"src/function.c": "int function(void) { return 0; }\n",
                   "include/old.h": "#define VALUE 1\n", "include/new.h": "#define VALUE 1\n"}
        cases = [
            (["include/old.h"], ["include/old.h", "include/new.h"], ["include/new.h"]),
            (["include/old.h"], [], ["include/old.h"]),
            (["include/old.h"], ["include/new.h"], ["include/new.h", "include/old.h"]),
        ]
        for old, current, expected in cases:
            with self.subTest(old=old, current=current):
                prior = receipt(matches, sources, {"src/function.c": ["src/function.c", *old]})
                after = receipt(matches, sources, {"src/function.c": ["src/function.c", *current]})
                self.assertEqual(accept._function_changes(after, prior),
                                 ([], [{**matches[0], "changed_dependencies": expected}]))

    def test_dependency_order_and_manifest_metadata_do_not_require_rereview(self) -> None:
        matches = [match("function", "src/function.c", 0)]
        sources = {"src/function.c": "int function(void) { return 0; }\n", "include/common.h": ""}
        prior = receipt(matches, sources, {"src/function.c": list(sources)})
        current = copy.deepcopy(prior)
        current["dependencies"]["src/function.c"].reverse()
        current["input_manifest"]["files"]["include/common.h"]["note"] = "same bytes"
        self.assertEqual(accept._function_changes(current, prior), ([], []))

    def test_missing_frozen_dependency_evidence_does_not_silently_mean_unchanged(self) -> None:
        baseline = receipt([match("function", "src/function.c", 0)],
                           {"src/function.c": "int function(void) { return 0; }\n", "include/common.h": ""},
                           {"src/function.c": ["src/function.c", "include/common.h"]})
        for side in ("prior", "current"):
            for missing in ("closure", "file", "hash"):
                with self.subTest(side=side, missing=missing):
                    prior, current = copy.deepcopy(baseline), copy.deepcopy(baseline)
                    changed = prior if side == "prior" else current
                    if missing == "closure":
                        del changed["dependencies"]["src/function.c"]
                    elif missing == "file":
                        del changed["input_manifest"]["files"]["include/common.h"]
                    else:
                        del changed["input_manifest"]["files"]["include/common.h"]["sha256"]
                    with self.assertRaises(KeyError):
                        accept._function_changes(current, prior)

    def test_prepare_uses_frozen_paths_and_hashes_including_absorbed_and_reused_units(self) -> None:
        # No live src/ tree exists. Same-size edits, path-only moves with equal
        # hashes, and every function sharing the changed joint TU still count.
        prepared = self.prepare()
        self.assertEqual([m["symbol"] for m in prepared["new_functions"]], ["new"])
        self.assertEqual([m["symbol"] for m in prepared["changed_functions"]],
                         ["edited", "moved", "absorbed", "reused", "edited_cpp"])
        self.assertEqual(prepared["new_instruction_bytes"], 8)
        self.assertEqual(prepared["coverage"], self.current["coverage"])
        self.assertEqual([m["source"] for m in prepared["changed_functions"]],
                         ["src/edited.c", "src/renamed.c", "src/joint.c", "src/joint.c", "src/edited.cpp"])
        self.assertTrue(prepared["suite"]["passed"])

    def test_unchanged_sources_and_metadata_only_changes_do_not_need_rereview(self) -> None:
        current = copy.deepcopy(self.prior)
        current["c_matches"][0]["evidence"] = "updated note, same frozen source"
        self.assertEqual(accept._function_changes(current, self.prior), ([], []))

    def test_language_path_change_with_identical_content_needs_review(self) -> None:
        text = "int function(void) { return 0; }\n"
        prior = receipt([match("function", "src/function.c", 0)], {"src/function.c": text})
        current = receipt([match("function", "src/function.cpp", 0)], {"src/function.cpp": text})
        new, changed = accept._function_changes(current, prior)
        self.assertEqual(new, [])
        self.assertEqual(changed, [{**current["c_matches"][0],
                                    "changed_dependencies": ["src/function.c", "src/function.cpp"]}])

    def test_function_identity_includes_image_and_defaults_old_rows_to_resident(self) -> None:
        prior = copy.deepcopy(self.prior)
        del prior["c_matches"][0]["image_id"]
        current = copy.deepcopy(self.prior)
        current["c_matches"].append(match("unchanged", "src/main.c", 7, image_id="main_14400"))
        current["input_manifest"]["files"]["src/main.c"] = current["input_manifest"]["files"]["src/unchanged.c"]
        new, changed = accept._function_changes(current, prior)
        self.assertEqual(new, [current["c_matches"][-1]])
        self.assertEqual(changed, [])

    def test_missing_frozen_source_hash_does_not_silently_mean_unchanged(self) -> None:
        for side in ("prior", "current"):
            with self.subTest(side=side):
                prior, current = copy.deepcopy(self.prior), copy.deepcopy(self.current)
                del (prior if side == "prior" else current)["input_manifest"]["files"]["src/edited.c"]
                with self.assertRaises(KeyError):
                    accept._function_changes(current, prior)

    def test_finalize_refuses_each_unreviewed_changed_function_before_promotion(self) -> None:
        prepared = self.prepare()
        required = prepared["new_functions"] + prepared["changed_functions"]
        for omitted in prepared["changed_functions"]:
            with self.subTest(symbol=omitted["symbol"]):
                self.review(prepared, [m["symbol"] for m in required if m != omitted])
                with self.assertRaisesRegex(ValueError, f"new or changed function.*{omitted['symbol']}"):
                    accept.finalize(self.name, [])
                self.commands.assert_not_called()
                self.verify_manifest.assert_not_called()

    def test_finalize_still_refuses_an_unreviewed_new_function(self) -> None:
        prepared = self.prepare()
        self.review(prepared, [m["symbol"] for m in prepared["changed_functions"]])
        with self.assertRaisesRegex(ValueError, "new or changed function.*'new'"):
            accept.finalize(self.name, [])
        self.commands.assert_not_called()

    def test_finalize_requires_review_even_when_there_are_no_new_functions(self) -> None:
        current = copy.deepcopy(self.current)
        current["c_matches"] = current["c_matches"][:-1]
        for suffix in ("a", "b"):
            self.write_json(f"build/{self.name}-{suffix}/receipt.json", current)
        self.current = current
        prepared = self.prepare()
        self.assertEqual(prepared["new_functions"], [])
        self.review(prepared, [])
        with self.assertRaisesRegex(ValueError, "5 new or changed function"):
            accept.finalize(self.name, [])
        self.commands.assert_not_called()

    def test_finalize_accepts_review_union_and_keeps_changed_functions_out_of_new_credit(self) -> None:
        prepared = self.prepare()
        self.review(prepared, ["new", "moved"])
        shard = self.review(prepared, ["edited", "absorbed", "reused", "edited_cpp"], shard=True)
        validation = accept.read_json(accept.finalize(self.name, [shard]))
        self.assertEqual(validation["changed_functions"], prepared["changed_functions"])
        self.assertEqual(validation["new_functions"], prepared["new_functions"])
        self.assertEqual(validation["new_C_functions"], 1)
        self.assertEqual(validation["new_unique_C_instruction_bytes"], 8)
        self.assertEqual(validation["new_CPP_functions"], 0)
        self.assertEqual(validation["new_unique_CPP_instruction_bytes"], 0)
        self.assertEqual(self.commands.call_count, 3)
        self.verify_manifest.assert_called_once_with(self.root, self.current["input_manifest"])

    def test_finalize_refuses_an_old_prepare_without_changed_function_evidence(self) -> None:
        prepared = self.prepare()
        self.review(prepared, [m["symbol"] for m in prepared["new_functions"]])
        del prepared["changed_functions"]
        self.write_json(f"scratch/omp/accept/{self.name}/prepare.json", prepared)
        with self.assertRaises(KeyError):
            accept.finalize(self.name, [])
        self.commands.assert_not_called()


class PrepareBuildModeTests(unittest.TestCase):
    """`prepare`'s two fresh builds, with the build runner mocked (no toolchain runs)."""
    name = "modes"
    setUp = AcceptanceReviewTests.setUp
    write_json = AcceptanceReviewTests.write_json

    def run_command(self, argv: list[str], log: Path, env: dict | None = None) -> tuple[int, float]:
        if "unittest" in argv:
            with self.lock:
                self.events.append(("suite", None))
            log.write_text("Ran 1 test in 0.001s\n\nOK\n")
            return 0, 0.5
        run = argv[argv.index("--name") + 1]
        with self.lock:
            self.events.append(("start", run))
            self.environments[run] = env
            self.active += 1
            self.peak = max(self.peak, self.active)
        try:
            log.write_text("fixture build\n")
            return self.build(run)
        finally:
            with self.lock:
                self.active -= 1
                self.events.append(("end", run))

    def build(self, run: str) -> tuple[int, float]:
        if self.together is not None:
            self.together.wait()  # BrokenBarrierError unless both builds are in flight at once
        if run not in self.results:
            time.sleep(self.linger)  # a healthy build is still running when the other one fails
        result = self.results.get(run, (0, 1.25 if run.endswith("-a") else 2.5))
        if isinstance(result, BaseException):
            raise result
        return result

    def start(self, name: str, *, together: bool = False, results: dict | None = None, receipts: dict | None = None) -> None:
        """`receipts` maps a suffix to "missing" (no receipt.json) or "directory" (unreadable receipt.json)."""
        self.events, self.environments, self.active, self.peak = [], {}, 0, 0
        self.lock = threading.Lock()
        self.together = threading.Barrier(2, timeout=10) if together else None
        self.results = {f"{name}-{key}": value for key, value in (results or {}).items()}
        self.linger = 0.3 if results or receipts else 0
        for suffix in ("a", "b"):
            state = (receipts or {}).get(suffix, "present")
            if state == "present":
                self.write_json(f"build/{name}-{suffix}/receipt.json", self.current)
            elif state == "directory":
                (self.root / f"build/{name}-{suffix}/receipt.json").mkdir(parents=True)

    def ended(self) -> set[str]:
        return {run for kind, run in self.events if kind == "end"}

    def expected_receipts(self, name: str) -> dict:
        digest = accept._sha256(self.root / f"build/{name}-a/receipt.json")
        return {f"{name}-a": {"path": f"build/{name}-a/receipt.json", "sha256": digest, "elapsed_seconds": 1.25},
                f"{name}-b": {"path": f"build/{name}-b/receipt.json", "sha256": digest, "elapsed_seconds": 2.5}}

    def test_default_runs_both_builds_at_once_before_the_suite(self) -> None:
        self.start("together", together=True)
        prepared = accept.read_json(accept.prepare("together"))
        self.assertEqual(self.peak, 2)
        self.assertEqual(self.events[-1], ("suite", None))
        self.assertEqual(self.ended(), {"together-a", "together-b"})
        self.assertEqual(prepared["builds"]["mode"], "concurrent")
        self.assertGreaterEqual(prepared["builds"]["wall_seconds"], 0)
        self.assertEqual(prepared["receipts"], self.expected_receipts("together"))
        self.assertEqual(list(prepared["receipts"]), ["together-a", "together-b"])
        for suffix in ("a", "b"):
            log = (self.root / f"scratch/omp/accept/together/build-{suffix}.log").read_text()
            self.assertEqual(log, "fixture build\n")

    def test_sequential_builds_never_overlap_and_record_the_same_receipts(self) -> None:
        self.start("serial")
        prepared = accept.read_json(accept.prepare("serial", concurrent_builds=False))
        self.assertEqual(self.peak, 1)
        self.assertEqual(self.events, [("start", "serial-a"), ("end", "serial-a"),
                                       ("start", "serial-b"), ("end", "serial-b"), ("suite", None)])
        self.assertEqual(prepared["builds"]["mode"], "sequential")
        self.assertEqual(prepared["receipts"], self.expected_receipts("serial"))
        self.start("parallel", together=True)
        concurrent = accept.read_json(accept.prepare("parallel"))
        ignored = {"name", "prepared_at", "receipts", "builds", "suite", "review_required"}
        self.assertEqual({k: v for k, v in prepared.items() if k not in ignored},
                         {k: v for k, v in concurrent.items() if k not in ignored})
        self.assertEqual(json.dumps(prepared["receipts"]).replace("serial", "parallel"), json.dumps(concurrent["receipts"]))

    def test_a_failed_build_fails_prepare_and_records_nothing(self) -> None:
        cases = [("a", {"a": (3, 1.0)}), ("b", {"b": (4, 1.0)}), ("ab", {"a": (3, 1.0), "b": (4, 1.0)})]
        for concurrent in (True, False):
            for failed, results in cases:
                name = f"fail-{failed}-{'c' if concurrent else 's'}"
                with self.subTest(concurrent=concurrent, failed=failed):
                    self.start(name, together=concurrent, results=results)
                    work = self.root / "scratch/omp/accept" / name
                    messages = [f"build {name}-{suffix} failed (exit {results[suffix][0]}); see {work / f'build-{suffix}.log'}"
                                for suffix in ("a", "b") if suffix in results]
                    with self.assertRaises(ValueError) as raised:
                        accept.prepare(name, concurrent_builds=concurrent)
                    self.assertEqual(str(raised.exception), "; ".join(messages if concurrent else messages[:1]))
                    self.assertFalse((work / "prepare.json").exists())
                    self.assertFalse((work / "suite.log").exists())
                    self.assertNotIn(("suite", None), self.events)
                    self.assertEqual(self.active, 0)
                    # Concurrent mode waits for both builds; sequential mode never starts B after A fails.
                    stopped_early = not concurrent and "a" in results
                    self.assertEqual(self.ended(), {f"{name}-a"} if stopped_early else {f"{name}-a", f"{name}-b"})

    def test_a_build_runner_error_waits_for_the_other_build_then_propagates(self) -> None:
        for failed in ("a", "b"):
            name = f"error-{failed}"
            with self.subTest(failed=failed):
                self.start(name, together=True, results={failed: OSError(f"cannot run {failed}")})
                with self.assertRaisesRegex(OSError, f"cannot run {failed}"):
                    accept.prepare(name)
                self.assertEqual(self.ended(), {f"{name}-a", f"{name}-b"})
                self.assertEqual(self.active, 0)
                self.assertNotIn(("suite", None), self.events)
                self.assertFalse((self.root / "scratch/omp/accept" / name / "prepare.json").exists())

    def test_a_receipt_error_keeps_serial_precedence(self) -> None:
        cases = [("a", state, b_result) for state in ("missing", "directory") for b_result in ((0, 2.5), (4, 1.0))]
        cases += [("b", "missing", (3, 1.0)), ("b", "missing", (0, 1.25))]
        for concurrent in (True, False):
            for broken, state, other in cases:
                other_suffix = "b" if broken == "a" else "a"
                name = f"receipt-{broken}-{state}-{other[0]}-{'c' if concurrent else 's'}"
                with self.subTest(concurrent=concurrent, broken=broken, state=state, other=other):
                    self.start(name, together=concurrent, results={other_suffix: other}, receipts={broken: state})
                    work = self.root / "scratch/omp/accept" / name
                    with self.assertRaises((OSError, ValueError)) as raised:
                        accept.prepare(name, concurrent_builds=concurrent)
                    a_nonzero = broken == "b" and other[0] != 0
                    if a_nonzero:  # A's exit decides first, in both modes
                        self.assertIsInstance(raised.exception, ValueError)
                        self.assertRegex(str(raised.exception), rf"^build {name}-a failed \(exit {other[0]}\)")
                    else:  # the broken receipt's own error, before anything later in A/B order
                        self.assertIsInstance(raised.exception, IsADirectoryError if state == "directory" else FileNotFoundError)
                        self.assertIn(f"build/{name}-{broken}/receipt.json", str(raised.exception))
                    stopped_early = not concurrent and (broken == "a" or a_nonzero)
                    self.assertEqual(self.ended(), {f"{name}-a"} if stopped_early else {f"{name}-a", f"{name}-b"})
                    self.assertFalse((work / "prepare.json").exists())
                    self.assertNotIn(("suite", None), self.events)

    def test_shared_replay_evidence_root_is_split_per_build(self) -> None:
        self.start("unset")
        with patch.dict(os.environ, {"SHIREN2_REPLAY_EVIDENCE_ROOT": ""}):
            accept.prepare("unset")
        self.assertEqual(self.environments, {"unset-a": None, "unset-b": None})
        self.start("shared")
        root = self.root / "replays"
        with patch.dict(os.environ, {"SHIREN2_REPLAY_EVIDENCE_ROOT": str(root)}):
            accept.prepare("shared")
            for run, env in self.environments.items():
                self.assertEqual(env, {**os.environ, "SHIREN2_REPLAY_EVIDENCE_ROOT": str(root / run)})
        self.assertEqual(set(self.environments), {"shared-a", "shared-b"})

    def test_replay_root_checks_are_unchanged_for_each_build(self) -> None:
        """Run the real replay-evidence helpers with the environment each build child receives."""
        real = self.root / "real-replays"
        real.mkdir()
        alias = self.root / "alias-replays"
        alias.symlink_to(real, target_is_directory=True)
        for concurrent in (True, False):
            for label, root, accepted in (("symlink", str(alias), False), ("relative", "replays", False), ("ordinary", str(real), True)):
                name = f"root-{label}-{'c' if concurrent else 's'}"
                with self.subTest(concurrent=concurrent, root=label):
                    self.start(name, together=concurrent)
                    with patch.dict(os.environ, {"SHIREN2_REPLAY_EVIDENCE_ROOT": root}):
                        accept.prepare(name, concurrent_builds=concurrent)
                        inherited = dict(os.environ)
                    for run, env in self.environments.items():
                        self.assertEqual(env is None, not accepted)
                        with patch.dict(os.environ, env or inherited, clear=True):
                            if not accepted:
                                with self.assertRaisesRegex(ValueError, "private root"):
                                    with certification.retained_trial(self.root / "build" / run, "probe"):
                                        self.fail("a refused replay root was accepted")
                                with self.assertRaisesRegex(ValueError, "private root"):
                                    certification.record_observed_output(["probe"], "", 0)
                                continue
                            with certification.retained_trial(self.root / "build" / run, "probe") as trial:
                                self.assertEqual(Path(trial).parent, real / run)
                            certification.record_observed_output(["probe"], "out", 0)
                    if not accepted:
                        self.assertEqual(sorted(path.name for path in real.iterdir() if path.name.startswith(name)), [])
                    else:
                        self.assertEqual(sorted(path.name for path in real.iterdir() if path.name.startswith(name)), [f"{name}-a", f"{name}-b"])

    def test_cli_selects_and_reports_the_build_mode(self) -> None:
        for argv, mode in ((["prepare", "--name", "cli-c"], "concurrent"),
                           (["prepare", "--name", "cli-s", "--sequential-builds"], "sequential")):
            with self.subTest(mode=mode):
                self.start(argv[2], together=mode == "concurrent")
                output = io.StringIO()
                with patch.object(sys, "argv", ["accept.py", *argv]), redirect_stdout(output), \
                        self.assertRaises(SystemExit) as exited:
                    accept.main()
                self.assertEqual(exited.exception.code, 0)
                prepared = accept.read_json(self.root / "scratch/omp/accept" / argv[2] / "prepare.json")
                self.assertEqual(prepared["builds"]["mode"], mode)
                self.assertIn(f'"mode": "{mode}"', output.getvalue())


if __name__ == "__main__":
    unittest.main()
