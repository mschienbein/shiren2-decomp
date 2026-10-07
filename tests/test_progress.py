"""Accounting fixtures exercise policy; live publication/ROM proof is separate.

The expensive parser/compiler/image adapters are replaced with fixed tiny original
CPU references. Pointer, manifests, artifacts, reviews, history and reports are
real private files; counting, bounds, hashes and no-op behavior are not mocked.
"""
from __future__ import annotations

import copy
import hashlib
import json
import shutil
import sys
import tempfile
import unittest
from pathlib import Path
from types import MappingProxyType, SimpleNamespace
from unittest.mock import patch

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "tools"))
import progress
from images import ImageInventory, ImageRecord, ProvisionalCpuRange
from packet import FunctionCatalogue
from rom import EXPECTED_SHA256
from workspace import REQUIRED_FILES, input_manifest


class ProgressTests(unittest.TestCase):
    def setUp(self) -> None:
        self.temporary = tempfile.TemporaryDirectory()
        self.addCleanup(self.temporary.cleanup)
        self.root = Path(self.temporary.name).resolve()
        self.directory = self.root / "build/accepted"
        self.source = self.directory / "source"
        self.source.mkdir(parents=True)
        for name in REQUIRED_FILES:
            (self.source / name).write_text("fixture\n")
        self.original = bytearray(0x3000)
        self.original[0x1000:0x1008] = bytes.fromhex("03e0000800000000")
        self.original[0x2000:0x2010] = bytes.fromhex("0c0097000000000003e0000800000000")
        self.reference = self.root / "original.z64"
        self.reference.write_bytes(self.original)
        self.images = {name: self.image(name, start, address, end)
                       for name, start, address, end in [("resident", 0x1000, 0x80025C00, 0x1010),
                                                        ("main_14400", 0x2000, 0x800413C0, 0x2020)]}
        self.inventory = ImageInventory(EXPECTED_SHA256, len(self.original), False, False, None,
            "fixture only", MappingProxyType(self.images), (), (), (),
            MappingProxyType({"src/sdk.c": "resident"}), MappingProxyType({}))
        self.matches = [{"image_id": "resident", "symbol": "sdk", "source": "src/sdk.c",
                         "rom_start": 0x1000, "vram_start": 0x80025C00, "size": 8}]
        self.graph = {"obj/source/src/sdk.o": {"image_id": "resident", "source_kind": "c",
                      "source": "src/sdk.c", "input": "source/src/sdk.c"},
                      "obj/generated/asm/game.o": {"image_id": "main_14400", "source_kind": "asm",
                      "source": "generated/asm/game.s", "input": "generated/asm/game.s"}}
        self.profile = {"tu_profiles": {"src/sdk.c": "fixture"}}
        (self.source / "src").mkdir()
        (self.source / "src/sdk.c").write_text("fixture readable source\n")
        (self.source / "config").mkdir()
        (self.source / "config/matches.json").write_text(json.dumps({"functions": self.matches}))
        (self.source / "config/images.json").write_text("{}")
        (self.source / "config/compiler_profiles.json").write_text(json.dumps(self.profile))
        self.manifest = input_manifest(self.source)
        (self.directory / "input-manifest.json").write_text(json.dumps(self.manifest))
        for path in self.source.iterdir():
            if path.is_dir():
                shutil.copytree(path, self.root / path.name)
            else:
                shutil.copy2(path, self.root / path.name)
        for name in progress.REQUIRED_ARTIFACTS - {"input-manifest.json", "shiren2.z64"}:
            (self.directory / name).write_text("fixture " + name)
        (self.directory / "shiren2.z64").write_bytes(self.original)
        self.receipt_path = self.directory / "receipt.json"
        body = hashlib.sha256(self.original[0x1000:0x1008]).hexdigest()
        self.receipt = {key: {} for key in progress.REQUIRED_RECEIPT_FIELDS}
        self.receipt.update(schema_version=3, status="byte-identical", target_sha256=EXPECTED_SHA256,
            input_manifest=self.manifest, c_matches=self.matches, graph=self.graph, profile=self.profile,
            coverage={"matched_c_functions": 1, "matched_c_bytes": 8, "description": "fixture reviewed C"},
            objects={"obj/source/src/sdk.o": "1" * 64}, image_inventory={"inventory": "fixture"},
            c_attributions=[{**self.matches[0], "object": "obj/source/src/sdk.o", "profile_id": "fixture",
                             "reference_sha256": body, "linked_sha256": body}],
            reproduction={"c_objects": {"obj/source/src/sdk.o": {"source": "src/sdk.c",
                            "profile_id": "fixture", "object_sha256": "1" * 64}}},
            artifacts={name: progress.sha256(self.directory / name) for name in progress.REQUIRED_ARTIFACTS})
        self.receipt_path.write_text(json.dumps(self.receipt))
        (self.root / "build/latest.json").write_text(json.dumps({"schema_version": 3,
            "receipt": "build/accepted/receipt.json", "receipt_sha256": progress.sha256(self.receipt_path)}))
        (self.root / "docs").mkdir()
        (self.root / "scratch").mkdir()
        self.review_path = self.root / "scratch/review.json"
        self.review_path.write_text(json.dumps({"status": "PASS-fixture", "receipt_hashes": {
            "accepted": progress.sha256(self.receipt_path)}, "current_frozen_manifest_sha256": self.manifest["sha256"],
            "coverage": self.receipt["coverage"]}))
        self.log_path = self.root / "scratch/tests.log"
        self.log_path.write_text("fixture suite evidence\n")
        self.semantic_path = self.root / "scratch/semantic.json"
        self.semantic_path.write_text(json.dumps({"status": "PASS-fixture"}))
        self.validation_path = self.root / "docs/accepted-validation.json"
        self.validation = {"schema_version": 1, "kind": "accepted-v3-integration-checkpoint",
            "status": "accepted", "target_sha256": EXPECTED_SHA256,
            "accepted_receipt": progress._reference(self.root, self.receipt_path),
            "frozen_manifest": {"sha256": self.manifest["sha256"]},
            "coverage": {"matched_c_functions": 1, "matched_c_instruction_bytes": 8},
            "promotion": {"serial": True, "exit_code": 0, "source_current_at_acceptance": True,
                          "latest_pointer_sha256": progress.sha256(self.root / "build/latest.json")},
            "independent_whole_rom_review": progress._reference(self.root, self.review_path),
            "full_suite": {"passed": True, "tests": 2, "log": "scratch/tests.log",
                           "log_sha256": progress.sha256(self.log_path)},
            "independent_new_function_reviews": {"sdk": progress._reference(self.root, self.semantic_path)}}
        self.validation_path.write_text(json.dumps(self.validation))
        functions = {}
        for image, symbol, start, address, size in [("resident", "sdk", 0x1000, 0x80025C00, 8),
                                                  ("main_14400", "game", 0x2000, 0x800413C0, 16)]:
            functions[image, symbol] = SimpleNamespace(image_id=image, symbol=symbol, rom_start=start,
                vram_start=address, size=size, expected_sha256=hashlib.sha256(self.original[start:start+size]).hexdigest(),
                handwritten_annotation=image == "resident", internal_aliases=["sdk_alias"] if image == "resident" else [])
        self.catalogue = FunctionCatalogue(SimpleNamespace(receipt_sha256=progress.sha256(self.receipt_path)),
                                            self.inventory, MappingProxyType(functions))
        self.patch("attest", return_value={"sha256": EXPECTED_SHA256})
        self.patch("build_graph", side_effect=lambda path: copy.deepcopy(self.graph))
        self.patch("image_evidence", return_value=(self.inventory, {"inventory": "fixture"}))
        self.patch("elf_code", side_effect=lambda path, start, size: (
            bytes(self.original[(start - 0x80025C00 + 0x1000) if start < 0x80040000 else (start - 0x800413C0 + 0x2000):
                                ((start - 0x80025C00 + 0x1000) if start < 0x80040000 else (start - 0x800413C0 + 0x2000)) + size]), 0))
        self.enumerator = self.patch("enumerate_functions", return_value=self.catalogue)

    def patch(self, name: str, **kwargs):
        handle = patch.object(progress, name, **kwargs)
        self.addCleanup(handle.stop)
        return handle.start()

    def image(self, name: str, start: int, address: int, end: int) -> ImageRecord:
        return ImageRecord(name, start, start + 0x800, address, address + 0x800, "0" * 64,
            "uncompressed-linear", name, "fixture", "fixture", (), "unknown", (), (), "active",
            (ProvisionalCpuRange(start, end, "provisional", (), "fixture"),), ())

    def record(self, **kwargs):
        return progress.record(self.validation_path, root=self.root, reference=self.reference, **kwargs)

    def show(self):
        return progress.show(root=self.root, reference=self.reference)

    def accepted(self):
        return progress.live_receipt(self.root, self.reference)

    def rewrite_receipt_and_pointer(self) -> None:
        self.receipt_path.write_text(json.dumps(self.receipt))
        pointer = self.root / "build/latest.json"
        pointer.write_text(json.dumps({"schema_version": 3, "receipt": "build/accepted/receipt.json",
                                      "receipt_sha256": progress.sha256(self.receipt_path)}))

    def test_initial_math_null_full_game_and_padding_zero_credit(self):
        result = self.record()
        report = result["report"]
        self.assertTrue(result["appended"])
        self.assertEqual(report["coverage"]["provisional_catalogued_instruction_bytes"], 24)
        self.assertAlmostEqual(report["coverage"]["mapped_catalogue_c_percent"], 100 * 8 / 24)
        self.assertIsNone(report["coverage"]["complete_game_percent"])
        self.assertEqual(report["coverage"]["images"]["resident"]["mapped_catalogue_c_percent"], 100)
        audit = report["catalogue_audit"]["images"]
        self.assertEqual(sum(row["gap_bytes"] for row in audit.values()), 24)
        self.assertEqual(sum(row["zero_words_inside_functions"] for row in audit.values()), 3)
        self.assertEqual(sum(row["internal_aliases"] for row in audit.values()), 1)
        self.assertEqual(sum(row["handwritten_annotated_bytes"] for row in audit.values()), 8)
        self.assertIn("Complete-game denominator and percentage: unknown", (self.root / "docs/PROGRESS.md").read_text())

    def test_duplicate_record_is_exact_noop_and_show_never_recatalogues(self):
        self.record()
        paths = [self.root / "docs" / name for name in ["progress-history.jsonl", "progress.json", "PROGRESS.md"]]
        before = {str(p): (p.read_bytes(), p.stat().st_mtime_ns) for p in paths}
        self.assertFalse(self.record()["appended"])
        self.show()
        self.show()
        self.assertEqual(self.enumerator.call_count, 1)
        self.assertEqual(before, {str(p): (p.read_bytes(), p.stat().st_mtime_ns) for p in paths})

    def test_current_staged_source_receives_no_credit(self):
        self.record()
        (self.root / "src/sdk.c").write_text("unaccepted working candidate\n")
        (self.root / "src/new.c").write_text("unaccepted new C\n")
        report = self.show()
        self.assertEqual(report["coverage"]["matched_c_instruction_bytes"], 8)
        self.assertFalse(report["freshness"]["current_tree_matches_accepted_snapshot"])
        self.assertTrue(report["freshness"]["frozen_accepted_inputs_valid"])
        self.assertIn("src/new.c", report["freshness"]["added"])
        self.assertFalse(self.record()["appended"])

    def test_changed_receipt_without_matching_pointer_is_rejected(self):
        self.receipt_path.write_text("{}");
        with self.assertRaisesRegex(ValueError, "Changed progress evidence"):
            self.record()

    def test_changed_coverage_with_updated_pointer_fails_accounting(self):
        self.receipt["coverage"]["matched_c_bytes"] = 12
        self.rewrite_receipt_and_pointer()
        with self.assertRaisesRegex(ValueError, "coverage disagrees"):
            self.record()

    def test_forged_pointer_and_validation_cannot_reuse_old_independent_review(self):
        self.receipt["created_at"] = "different unreviewed receipt"
        self.rewrite_receipt_and_pointer()
        self.validation["accepted_receipt"] = progress._reference(self.root, self.receipt_path)
        self.validation["promotion"]["latest_pointer_sha256"] = progress.sha256(self.root / "build/latest.json")
        self.validation_path.write_text(json.dumps(self.validation))
        with self.assertRaisesRegex(ValueError, "Independent full-receipt review"):
            self.record()

    def test_trial_companion_receipt_is_not_accepted(self):
        self.validation["accepted_receipt"]["path"] = "build/trial/receipt.json"
        self.validation_path.write_text(json.dumps(self.validation))
        with self.assertRaisesRegex(ValueError, "does not certify"):
            self.record()

    def test_assembly_graph_never_supplies_c_credit(self):
        self.graph["obj/source/src/sdk.o"]["source_kind"] = "asm"
        self.receipt["graph"] = self.graph
        self.rewrite_receipt_and_pointer()
        with self.assertRaises(ValueError):
            self.record()

    def test_missing_or_failed_semantic_review_is_rejected(self):
        self.semantic_path.write_text(json.dumps({"status": "candidate"}))
        self.validation["independent_new_function_reviews"]["sdk"] = progress._reference(self.root, self.semantic_path)
        self.validation_path.write_text(json.dumps(self.validation))
        with self.assertRaisesRegex(ValueError, "has not passed"):
            self.record()

    def test_changed_test_log_or_accepted_rom_is_rejected(self):
        self.log_path.write_text("tampered")
        with self.assertRaisesRegex(ValueError, "Changed progress evidence"):
            self.record()
        self.log_path.write_text("fixture suite evidence\n")
        (self.directory / "shiren2.z64").write_bytes(b"tampered")
        with self.assertRaisesRegex(ValueError, "Changed accepted artifact"):
            self.record()

    def test_frozen_source_mutation_is_rejected_but_current_mutation_is_not(self):
        (self.source / "src/sdk.c").write_text("tampered frozen source")
        with self.assertRaisesRegex(ValueError, "manifest mismatch"):
            self.record()

    def test_stored_denominator_mutation_is_rejected(self):
        report = self.record()["report"]
        path = self.root / report["denominator"]["path"]
        path.write_text("{}")
        with self.assertRaisesRegex(ValueError, "Changed progress evidence"):
            self.show()

    def test_overlap_duplicate_alias_and_invalid_original_hash_fail(self):
        d = progress.freeze_catalogue(self.accepted(), progress._reference(self.root, self.validation_path))
        for change in ["overlap", "alias", "hash"]:
            bad = copy.deepcopy(d["identity"])
            if change == "overlap":
                bad["functions"].insert(0, copy.deepcopy(bad["functions"][0]))
            elif change == "alias":
                bad["functions"][0]["internal_aliases"] = [bad["functions"][0]["symbol"]]
            else:
                bad["functions"][0]["sha256"] = "0" * 64
            with self.assertRaises(ValueError):
                progress.audit_catalogue(bad, bytes(self.original))

    def test_match_outside_denominator_requires_explicit_revision(self):
        accepted = self.accepted()
        d = progress.freeze_catalogue(accepted, progress._reference(self.root, self.validation_path))
        d["identity"]["functions"] = [f for f in d["identity"]["functions"] if f["image_id"] != "resident"]
        d["denominator_id"] = progress.json_digest(d["identity"])
        d["audit"] = progress.audit_catalogue(d["identity"], bytes(self.original))
        with self.assertRaisesRegex(ValueError, "outside the immutable denominator"):
            progress.calculate(accepted, d)

    def test_explicit_revision_preserves_original_file_and_history_identity(self):
        first = self.record()["report"]
        old_path = self.root / first["denominator"]["path"]
        old = old_path.read_bytes()
        self.catalogue["main_14400", "game"].handwritten_annotation = True
        with self.assertRaisesRegex(ValueError, "reviewed reason"):
            self.record(revise_denominator=True)
        second = self.record(revise_denominator=True, reason="Reviewed annotation reclassification")["report"]
        self.assertNotEqual(first["denominator"]["id"], second["denominator"]["id"])
        self.assertEqual(old_path.read_bytes(), old)
        history = progress._history(self.root / "docs/progress-history.jsonl")
        self.assertEqual(len(history), 2)
        self.assertEqual(history[1]["denominator_change"]["previous_id"], first["denominator"]["id"])
        self.assertEqual(history[1]["matched_c_byte_delta"], 0)
        self.assertFalse(self.record()["appended"])

    def test_history_tampering_fails_and_missing_report_recovers_without_append(self):
        self.record()
        history_path = self.root / "docs/progress-history.jsonl"
        history = history_path.read_bytes()
        (self.root / "docs/progress.json").unlink()
        self.assertFalse(self.record()["appended"])
        self.assertEqual(history_path.read_bytes(), history)
        value = json.loads(history)
        value["coverage"]["matched_c_instruction_bytes"] += 4
        history_path.write_text(json.dumps(value) + "\n")
        with self.assertRaisesRegex(ValueError, "history chain"):
            self.show()

    def test_output_and_validation_paths_cannot_escape_or_use_symlinks(self):
        with self.assertRaises(ValueError):
            self.record(output_dir=self.root / "src/output")
        link = self.root / "docs/review-link.json"
        link.symlink_to(self.validation_path)
        with self.assertRaisesRegex(ValueError, "Symlinks"):
            progress.record(link, root=self.root, reference=self.reference)

    def test_empty_catalogue_percent_is_unknown_and_all_zero_labels_are_disclosed(self):
        identity = progress._catalogue_identity(self.catalogue)
        identity["functions"] = []
        audit = progress.audit_catalogue(identity, bytes(self.original))
        self.assertEqual(audit["instruction_bytes"], 0)
        self.original[0x1000:0x1008] = b"\0" * 8
        identity = progress._catalogue_identity(self.catalogue)
        f = next(f for f in identity["functions"] if f["image_id"] == "resident")
        f["sha256"] = hashlib.sha256(b"\0" * 8).hexdigest()
        audit = progress.audit_catalogue(identity, bytes(self.original))
        self.assertEqual(audit["images"]["resident"]["all_zero_functions"], 1)


if __name__ == "__main__":
    unittest.main()
