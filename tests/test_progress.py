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
        # Default fixture: a receipt accepted before the complete-game ROM map existed.
        self.receipt = {key: {} for key in progress.REQUIRED_RECEIPT_FIELDS - {"rom_map"}}
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
        self.handwritten = {"schema_version": 1, "target_sha256": EXPECTED_SHA256,
                            "functions": []}
        self.write_handwritten()
        self.patch("attest", return_value={"sha256": EXPECTED_SHA256})
        self.patch("build_graph", side_effect=lambda path: copy.deepcopy(self.graph))
        self.patch("image_evidence", return_value=(self.inventory, {"inventory": "fixture"}))
        self.patch("elf_code", side_effect=lambda path, start, size, section_name=None: (
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

    def write_handwritten(self):
        (self.root / "config/handwritten.json").write_text(json.dumps(self.handwritten))

    def classify_game(self):
        f = self.catalogue["main_14400", "game"]
        self.hw_review = {"reviewer_family": "anthropic-claude (Opus)",
                          "functions": [{"symbol": "game", "verdict": "confirmed"}]}
        evidence = self.root / "docs/evidence/handwritten"
        evidence.mkdir(parents=True)
        self.hw_source = {"family": "gpt", "results": [
            {"functions": ["game"], "status": "handwritten", "image": "main_14400"}]}
        self.hw_source_path = evidence / "handwritten-source.json"
        self.hw_source_path.write_text(json.dumps(self.hw_source))
        self.hw_review_path = evidence / "handwritten-review.json"
        self.hw_review_path.write_text(json.dumps(self.hw_review))
        self.handwritten["functions"] = [{"image_id": f.image_id, "symbol": f.symbol,
            "rom_start": f.rom_start, "size": f.size, "expected_sha256": f.expected_sha256,
            "sdk_routines": ["fixture_entry"], "sdk_source": "fixture/entry.s",
            "evidence": ["JAL 0x80025C00; NOP; JR RA; NOP."], "classifier_family": "gpt",
            "classification_source": progress._reference(self.root, self.hw_source_path),
            "supporting_evidence": [],
            "reviews": [{**progress._reference(self.root, self.hw_review_path),
                         "family": "claude", "items": "functions"}]}]
        self.write_handwritten()
        accepted = self.accepted()
        denominator = progress.freeze_catalogue(accepted, progress._reference(self.root, self.validation_path))
        return accepted, denominator

    def rewrite_classification_source(self):
        self.hw_source_path.write_text(json.dumps(self.hw_source))
        self.handwritten["functions"][0]["classification_source"] = progress._reference(self.root, self.hw_source_path)

    def accepted(self):
        return progress.live_receipt(self.root, self.reference)

    def rewrite_receipt_and_pointer(self) -> None:
        self.receipt_path.write_text(json.dumps(self.receipt))
        pointer = self.root / "build/latest.json"
        pointer.write_text(json.dumps({"schema_version": 3, "receipt": self.receipt_path.relative_to(self.root).as_posix(),
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
        self.assertIsNone(report["complete_game"])
        self.assertNotIn("rom_map", self.receipt)
        self.assertIsNone(self.show()["complete_game"])

    def test_cpp_numerator_stays_separate_from_c_with_an_exact_sum(self):
        report = self.record()["report"]
        accepted = self.accepted()
        denominator = progress.read_json(self.root / report["denominator"]["path"])
        cpp = {"image_id": "main_14400", "symbol": "game", "source": "src/game.cpp",
               "rom_start": 0x2000, "vram_start": 0x800413C0, "size": 16}
        accepted.document["c_matches"].append(cpp)
        accepted.document["c_attributions"].append({**cpp, "reference_sha256": hashlib.sha256(self.original[0x2000:0x2010]).hexdigest()})
        accepted.document["coverage"].update(matched_cpp_functions=1, matched_cpp_bytes=16,
                                            matched_c_and_cpp_functions=2, matched_c_and_cpp_bytes=24)
        counts = progress.calculate(accepted, denominator)
        self.assertEqual((counts["matched_c_functions"], counts["matched_c_instruction_bytes"]), (1, 8))
        self.assertEqual((counts["matched_cpp_functions"], counts["matched_cpp_instruction_bytes"]), (1, 16))
        self.assertEqual((counts["matched_c_and_cpp_functions"], counts["matched_c_and_cpp_instruction_bytes"]), (2, 24))
        self.assertEqual(counts["images"]["resident"]["matched_cpp_instruction_bytes"], 0)
        self.assertEqual(counts["images"]["main_14400"]["matched_c_instruction_bytes"], 0)
        self.assertEqual(counts["mapped_catalogue_c_and_cpp_percent"], 100)
        self.assertIsNone(counts["complete_game_percent"])
        accounting = progress.handwritten_accounting(accepted, denominator, counts)["accounting"]
        self.assertEqual(accounting["matched_c_instruction_bytes"], 8)
        self.assertEqual(accounting["matched_cpp_instruction_bytes"], 16)
        self.assertEqual(accounting["classified_handwritten_instruction_bytes"], 0)
        self.assertEqual(accounting["accounted_instruction_bytes"], 24)
        accepted.document["coverage"]["matched_c_and_cpp_bytes"] = 25
        with self.assertRaisesRegex(ValueError, "inconsistent accepted C\\+C\\+\\+ numerator"):
            progress.calculate(accepted, denominator)

    def test_c_only_receipt_cannot_forge_cpp_counters(self):
        self.receipt["coverage"].update(matched_cpp_functions=1, matched_cpp_bytes=16)
        self.rewrite_receipt_and_pointer()
        with self.assertRaisesRegex(ValueError, "coverage counter inventory"):
            self.accepted()

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

    def test_image_scoped_revision_retains_unselected_boundaries_and_history(self):
        first = self.record()["report"]
        old_path = self.root / first["denominator"]["path"]
        old_bytes = old_path.read_bytes()
        old = json.loads(old_bytes)
        # Fresh assembly has changed the resident boundary/name; only main is reviewed.
        sdk = self.catalogue["resident", "sdk"]
        sdk.symbol, sdk.size, sdk.internal_aliases = "sdk_regenerated", 4, []
        sdk.expected_sha256 = hashlib.sha256(self.original[0x1000:0x1004]).hexdigest()
        self.catalogue["main_14400", "game"].handwritten_annotation = True
        second = self.record(revise_denominator=True, revise_images=("main_14400",),
                             reason="Review main only")["report"]
        revised = progress.read_json(self.root / second["denominator"]["path"])
        resident = lambda document: [row for row in document["identity"]["functions"] if row["image_id"] == "resident"]
        self.assertEqual(resident(revised), resident(old))
        self.assertTrue(next(row for row in revised["identity"]["functions"] if row["image_id"] == "main_14400")["handwritten_annotation"])
        self.assertEqual(revised["origin"]["revised_images"], ["main_14400"])
        self.assertEqual(old_path.read_bytes(), old_bytes)
        self.assertEqual(self.show()["coverage"]["matched_c_instruction_bytes"], 8)

    def test_image_scope_requires_explicit_revision_history_and_known_images(self):
        with self.assertRaisesRegex(ValueError, "explicit denominator revision"):
            self.record(revise_images=("resident",))
        with self.assertRaisesRegex(ValueError, "existing denominator history"):
            self.record(revise_denominator=True, revise_images=("resident",), reason="review")
        self.record()
        with self.assertRaisesRegex(ValueError, "known images"):
            self.record(revise_denominator=True, revise_images=("missing",), reason="review")

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
        identity = progress.catalogue_identity(self.catalogue.inventory, self.catalogue)
        identity["functions"] = []
        audit = progress.audit_catalogue(identity, bytes(self.original))
        self.assertEqual(audit["instruction_bytes"], 0)
        self.original[0x1000:0x1008] = b"\0" * 8
        identity = progress.catalogue_identity(self.catalogue.inventory, self.catalogue)
        f = next(f for f in identity["functions"] if f["image_id"] == "resident")
        f["sha256"] = hashlib.sha256(b"\0" * 8).hexdigest()
        audit = progress.audit_catalogue(identity, bytes(self.original))
        self.assertEqual(audit["images"]["resident"]["all_zero_functions"], 1)

    def rom_map_section(self, map_id: str = "a" * 64, research: int = 92) -> dict:
        size = 0x2000000
        identity = progress.catalogue_identity(self.catalogue.inventory, self.catalogue)
        return {"path": "config/rom_map.json", "sha256": "b" * 64, "rules_sha256": "c" * 64,
                "cpu_catalogue_id": progress.json_digest(identity),
                "partition_sha256": "e" * 64, "evidence_policy_sha256": "f" * 64,
                "map_id": map_id, "rom_size": size, "classes": {"cpu_text_function": {"ranges": 2, "bytes": 24}},
                "status_bytes": {"asset_raw:open": size - research - 24, "cpu_code:matched_c": 8,
                                 "cpu_code:open": 16, "padding:closed": research},
                "cpu_code": {"functions": 2, "instruction_bytes": 24, "matched_c_functions": 1,
                             "matched_c_bytes": 8, "open_functions": 1, "open_bytes": 16},
                "asset_compressed": {"streams": 1, "stream_bytes": 0, "decoded_bytes": 0,
                                     "roundtrip_streams": 0, "roundtrip_stream_bytes": 0},
                "research_classified_bytes": research, "research_open_bytes": size - research,
                "build_source_bytes": 8, "credit_rule": "unit fixture, never real acceptance"}

    def install_fixture_map_loader(self):
        identity = progress.catalogue_identity(self.catalogue.inventory, self.catalogue)
        ranges = tuple((f["rom_start"], f["rom_start"] + f["size"], "cpu_text_function", "catalogue")
                       for f in identity["functions"])
        frozen = progress.rommap.RomMap("a" * 64, "b" * 64, progress.json_digest(identity),
                                       ranges, "c" * 64, "d" * 64)
        loader = patch.object(progress.rommap, "load", return_value=frozen)
        self.addCleanup(loader.stop)
        return loader.start()

    def accept_receipt(self, section: dict, created_at: str = "first") -> None:
        """Synthetic unit fixture only; preserve independent report/reference checks."""
        previous = self.directory
        self.directory = self.root / "build" / created_at
        shutil.copytree(previous, self.directory)
        self.source = self.directory / "source"
        self.receipt_path = self.directory / "receipt.json"
        previous_review = self.review_path
        self.review_path = self.root / "scratch" / f"review-{created_at}.json"
        shutil.copy2(previous_review, self.review_path)
        self.validation_path = self.root / "docs" / f"validation-{created_at}.json"
        self.receipt["rom_map"] = section
        self.receipt["created_at"] = created_at
        self.rewrite_receipt_and_pointer()
        digest = progress.sha256(self.receipt_path)
        review = json.loads(self.review_path.read_text())
        review["receipt_hashes"] = {self.directory.name: digest}
        self.review_path.write_text(json.dumps(review))
        self.catalogue.baseline.receipt_sha256 = digest
        self.validation["accepted_receipt"] = progress._reference(self.root, self.receipt_path)
        self.validation["promotion"]["latest_pointer_sha256"] = progress.sha256(self.root / "build/latest.json")
        self.validation["independent_whole_rom_review"] = progress._reference(self.root, self.review_path)
        self.validation_path.write_text(json.dumps(self.validation))

    def test_complete_game_section_is_separate_from_matched_c_and_rechecked(self):
        self.install_fixture_map_loader()
        report_function = self.patch("rom_map_report", side_effect=lambda *a, **k: copy.deepcopy(self.receipt["rom_map"]))
        self.accept_receipt(self.rom_map_section())
        report = self.record()["report"]
        complete = report["complete_game"]
        self.assertEqual(report["schema_version"], 3)
        self.assertAlmostEqual(complete["matched_c_percent_of_cpu_code"], 100 * 8 / 24)
        self.assertEqual(complete["accepted_build_source_bytes"], 8)
        self.assertEqual(complete["research_classified_bytes"], 92)
        self.assertFalse(complete["whole_denominator_closed"])
        self.assertNotIn("complete_game_closed_percent", complete)
        self.assertEqual(complete["accepted_source_receipt"], report["accepted_receipt"])
        self.assertEqual(report["coverage"]["matched_c_instruction_bytes"], 8)
        self.assertEqual(self.show()["complete_game"], complete)
        report_function.side_effect = lambda *a, **k: self.rom_map_section(research=93)
        with self.assertRaisesRegex(ValueError, "complete-game ROM map accounting disagrees"):
            self.show()

    def test_rom_map_change_requires_an_explicit_reviewed_revision(self):
        self.install_fixture_map_loader()
        self.patch("rom_map_report", side_effect=lambda *a, **k: copy.deepcopy(self.receipt["rom_map"]))
        self.accept_receipt(self.rom_map_section("a" * 64))
        self.record()
        self.accept_receipt(self.rom_map_section("e" * 64), created_at="second")
        with self.assertRaisesRegex(ValueError, "ROM map changed"):
            self.record()
        with self.assertRaisesRegex(ValueError, "reviewed reason"):
            self.record(revise_denominator=True)
        self.assertTrue(self.record(revise_denominator=True, reason="Reviewed overlay catalogue revision")["appended"])
        history = progress._history(self.root / "docs/progress-history.jsonl")
        self.assertEqual(history[-1]["rom_map_change"], {"previous_map_id": "a" * 64, "new_map_id": "e" * 64,
                                                        "reason": "Reviewed overlay catalogue revision"})
        self.assertEqual(history[-1]["schema_version"], 3)

    def test_real_catalogue_check_rejects_mismatch_in_record_and_show(self):
        loader = self.install_fixture_map_loader()
        self.patch("rom_map_report", side_effect=lambda *a, **k: copy.deepcopy(self.receipt["rom_map"]))
        self.accept_receipt(self.rom_map_section())
        self.record()
        frozen = loader.return_value
        loader.return_value = progress.rommap.RomMap(frozen.map_id, frozen.rules_sha256, "0" * 64,
            frozen.ranges, frozen.partition_sha256, frozen.evidence_policy_sha256)
        # Only loading is isolated for the tiny ROM fixture. calculate executes
        # the real check_catalogue; rom_map_report cannot mask either rejection.
        for operation in (self.record, self.show):
            with self.subTest(operation=operation.__name__), self.assertRaisesRegex(ValueError, "frozen CPU catalogue"):
                operation()

    def test_handwritten_schema_refuses_malformed_records(self):
        self.classify_game()
        changes = [
            lambda d: d.update(schema_version=True),
            lambda d: d.update(schema_version=2),
            lambda d: d.update(extra="unsupported"),
            lambda d: d.update(denominator_id="0" * 64),
            lambda d: d.update(functions={}),
            lambda d: d["functions"][0].pop("symbol"),
            lambda d: d["functions"][0].update(size=True),
            lambda d: d["functions"][0].update(size=0),
            lambda d: d["functions"][0].update(rom_start=1),
            lambda d: d["functions"][0].update(expected_sha256="A" * 64),
            lambda d: d["functions"][0].update(evidence=[]),
            lambda d: d["functions"][0].update(sdk_routines="not a list"),
            lambda d: d["functions"][0].update(sdk_source={}),
            lambda d: d["functions"][0].update(classifier_family="unknown"),
            lambda d: d["functions"][0].pop("classification_source"),
            lambda d: d["functions"][0].update(classification_source=None),
            lambda d: d["functions"][0]["classification_source"].update(sha256="bad"),
            lambda d: d["functions"][0]["classification_source"].update(path=""),
            lambda d: d["functions"][0]["classification_source"].update(extra="unsupported"),
            lambda d: d["functions"][0].update(supporting_evidence={}),
            lambda d: d["functions"][0].update(supporting_evidence=[{"path": "missing-digest"}]),
            lambda d: d["functions"][0].update(reviews=[]),
            lambda d: d["functions"][0]["reviews"][0].update(items="summary"),
        ]
        for index, change in enumerate(changes):
            with self.subTest(case=index):
                bad = copy.deepcopy(self.handwritten)
                change(bad)
                with self.assertRaises(ValueError):
                    progress.validate_handwritten_schema(bad)

    def test_handwritten_survives_unrelated_split_after_validation_is_written(self):
        self.classify_game()
        functions = dict(self.catalogue)
        helper = SimpleNamespace(image_id="main_14400", symbol="helper", rom_start=0x2010,
            vram_start=0x800413D0, size=16, expected_sha256=hashlib.sha256(bytes(16)).hexdigest(),
            handwritten_annotation=False, internal_aliases=[])
        functions["main_14400", "helper"] = helper
        self.enumerator.return_value = FunctionCatalogue(
            self.catalogue.baseline, self.inventory, MappingProxyType(functions))
        first = self.record()["report"]
        classification = (self.root / "config/handwritten.json").read_bytes()
        validation = self.validation_path.read_bytes()
        old_catalogue = self.root / first["denominator"]["path"]
        old_bytes = old_catalogue.read_bytes()
        helper.size = 8
        helper.expected_sha256 = hashlib.sha256(bytes(8)).hexdigest()
        functions["main_14400", "helper_tail"] = SimpleNamespace(
            **{**vars(helper), "symbol": "helper_tail", "rom_start": 0x2018, "vram_start": 0x800413D8})
        # finalize has already written the validation file before invoking record.
        second = self.record(revise_denominator=True, reason="Reviewed unrelated helper split")["report"]
        self.assertNotEqual(first["denominator"]["id"], second["denominator"]["id"])
        self.assertEqual(second["accounting"]["validated_denominator_id"], second["denominator"]["id"])
        self.assertEqual(second["accounting"]["classified_handwritten_functions"], 1)
        self.assertEqual(second["accounting"]["classified_handwritten_instruction_bytes"], 16)
        self.assertEqual(second["accounting"]["accounted_instruction_bytes"], 24)
        self.assertEqual(second["coverage"]["provisional_catalogued_instruction_bytes"], 40)
        self.assertEqual(old_catalogue.read_bytes(), old_bytes)
        self.assertEqual(self.validation_path.read_bytes(), validation)
        self.assertEqual((self.root / "config/handwritten.json").read_bytes(), classification)
        history = progress._history(self.root / "docs/progress-history.jsonl")
        self.assertEqual(len(history), 2)
        self.assertEqual(history[-1]["denominator_change"]["previous_id"], first["denominator"]["id"])
        self.assertEqual(history[-1]["matched_c_byte_delta"], 0)
        self.assertEqual(self.show()["accounting"], second["accounting"])
        self.assertFalse(self.record(revise_denominator=True, reason="Reviewed unrelated helper split")["appended"])

    def test_changed_classified_boundaries_do_not_publish_progress(self):
        self.classify_game()
        self.record()
        paths = [self.root / "docs" / name for name in
                 ("progress.json", "progress-history.jsonl", "PROGRESS.md")]
        before = {path: path.read_bytes() for path in paths}
        for change in ("move", "split", "merge", "rename"):
            with self.subTest(change=change):
                functions = copy.deepcopy(dict(self.catalogue))
                function = functions["main_14400", "game"]
                if change == "move":
                    function.rom_start += 4
                    function.vram_start += 4
                elif change == "split":
                    function.size = 12
                    functions["main_14400", "tail"] = SimpleNamespace(
                        **{**vars(function), "symbol": "tail", "rom_start": 0x200C,
                           "vram_start": 0x800413CC, "size": 4,
                           "expected_sha256": hashlib.sha256(bytes(4)).hexdigest()})
                elif change == "merge":
                    function.size = 20
                else:
                    del functions["main_14400", "game"]
                    function.symbol = "renamed"
                    functions["main_14400", "renamed"] = function
                function.expected_sha256 = hashlib.sha256(
                    self.original[function.rom_start:function.rom_start + function.size]).hexdigest()
                self.enumerator.return_value = FunctionCatalogue(
                    self.catalogue.baseline, self.inventory, MappingProxyType(functions))
                with self.assertRaisesRegex(ValueError, "catalogue function.*game.*denominator.*review"):
                    self.record(revise_denominator=True, reason=f"Reviewed {change}")
                self.assertEqual({path: path.read_bytes() for path in paths}, before)

    def test_handwritten_catalogue_and_byte_identity_are_bound(self):
        accepted, denominator = self.classify_game()
        for field, value in [("image_id", "resident"), ("symbol", "missing"), ("rom_start", 0x2004),
                             ("size", 12), ("expected_sha256", "0" * 64)]:
            with self.subTest(field=field):
                bad = copy.deepcopy(self.handwritten)
                bad["functions"][0][field] = value
                with self.assertRaisesRegex(ValueError, "catalogue function"):
                    progress.validate_handwritten(accepted, denominator, bad)
        with self.assertRaisesRegex(ValueError, "catalogue identity"):
            progress.validate_handwritten(accepted, denominator, {**self.handwritten, "target_sha256": "0" * 64})
        bad_denominator = {**denominator, "denominator_id": "0" * 64}
        with self.assertRaisesRegex(ValueError, "catalogue identity"):
            progress.validate_handwritten(accepted, bad_denominator, self.handwritten)
        original = bytearray(accepted.original)
        original[0x2000] ^= 1
        bad_accepted = SimpleNamespace(root=self.root, original=bytes(original), document=accepted.document)
        with self.assertRaisesRegex(ValueError, "instruction bytes disagree"):
            progress.validate_handwritten(bad_accepted, denominator, self.handwritten)

    def test_handwritten_duplicate_and_compiled_interval_overlap_are_refused(self):
        accepted, denominator = self.classify_game()
        bad = copy.deepcopy(self.handwritten)
        bad["functions"].append(copy.deepcopy(bad["functions"][0]))
        with self.assertRaisesRegex(ValueError, "Duplicate or overlapping"):
            progress.validate_handwritten(accepted, denominator, bad)
        for source in ("src/other.c", "src/other.cpp"):
            with self.subTest(source=source):
                accepted.document["c_matches"].append({"image_id": "main_14400", "symbol": "other",
                                                       "rom_start": 0x2004, "size": 4, "source": source})
                with self.assertRaisesRegex(ValueError, "overlaps accepted C/C\\+\\+"):
                    progress.validate_handwritten(accepted, denominator, self.handwritten)
                accepted.document["c_matches"].pop()
        accepted.document["c_matches"].append({"image_id": "main_14400", "symbol": "game",
                                               "rom_start": 0x2000, "size": 16, "source": "src/game.c"})
        with self.assertRaisesRegex(ValueError, "overlaps accepted"):
            progress.validate_handwritten(accepted, denominator, self.handwritten)

    def test_handwritten_changed_missing_or_escaping_review_is_refused(self):
        accepted, denominator = self.classify_game()
        self.hw_review_path.write_text("{}")
        with self.assertRaisesRegex(ValueError, "Changed progress evidence"):
            progress.validate_handwritten(accepted, denominator, self.handwritten)
        for path in ("docs/evidence/handwritten/missing.json", "../review.json", "/review.json"):
            with self.subTest(path=path):
                bad = copy.deepcopy(self.handwritten)
                bad["functions"][0]["reviews"][0]["path"] = path
                with self.assertRaisesRegex(ValueError, "Missing regular|under docs/evidence"):
                    progress.validate_handwritten(accepted, denominator, bad)

    def test_handwritten_same_family_and_forged_family_are_refused(self):
        accepted, denominator = self.classify_game()
        self.hw_review["reviewer_family"] = "GPT (Astra)"
        self.hw_review_path.write_text(json.dumps(self.hw_review))
        ref = self.handwritten["functions"][0]["reviews"][0]
        ref.update(progress._reference(self.root, self.hw_review_path))
        with self.assertRaisesRegex(ValueError, "reviewer family disagrees"):
            progress.validate_handwritten(accepted, denominator, self.handwritten)
        ref["family"] = "gpt"
        with self.assertRaisesRegex(ValueError, "cross-family review"):
            progress.validate_handwritten(accepted, denominator, self.handwritten)
        self.handwritten["functions"][0]["classifier_family"] = "claude"
        with self.assertRaisesRegex(ValueError, "classifier family disagrees"):
            progress.validate_handwritten(accepted, denominator, self.handwritten)

    def test_handwritten_classification_source_hash_path_and_support_are_bound(self):
        accepted, denominator = self.classify_game()
        self.hw_source_path.write_text("{}")
        with self.assertRaisesRegex(ValueError, "Changed progress evidence"):
            progress.validate_handwritten(accepted, denominator, self.handwritten)
        self.rewrite_classification_source()
        for path in ("docs/evidence/handwritten/missing.json", "../source.json", "/source.json",
                     "scratch/handwritten-source.json", "docs/evidence/handwritten/../../../source.json"):
            with self.subTest(path=path):
                bad = copy.deepcopy(self.handwritten)
                bad["functions"][0]["classification_source"]["path"] = path
                with self.assertRaisesRegex(ValueError, "Missing regular|under docs/evidence|project-relative"):
                    progress.validate_handwritten(accepted, denominator, bad)
        link = self.hw_source_path.with_name("link.json")
        link.symlink_to(self.hw_source_path)
        bad = copy.deepcopy(self.handwritten)
        bad["functions"][0]["classification_source"]["path"] = link.relative_to(self.root).as_posix()
        with self.assertRaisesRegex(ValueError, "Symlinks"):
            progress.validate_handwritten(accepted, denominator, bad)
        self.handwritten["functions"][0]["supporting_evidence"] = [
            {**progress._reference(self.root, self.hw_source_path), "sha256": "0" * 64}]
        with self.assertRaisesRegex(ValueError, "Changed progress evidence"):
            progress.validate_handwritten(accepted, denominator, self.handwritten)

    def test_handwritten_classification_source_requires_unique_handwritten_row(self):
        accepted, denominator = self.classify_game()
        row = copy.deepcopy(self.hw_source["results"][0])
        for rows in ([], [{**row, "functions": ["other"]}], [row, row],
                     [{**row, "status": "mismatch"}], [{**row, "image": "resident"}],
                     [{**row, "functions": "game"}], [None]):
            with self.subTest(rows=rows):
                self.hw_source["results"] = rows
                self.rewrite_classification_source()
                with self.assertRaisesRegex(ValueError, "Source does not classify|Invalid handwritten classification"):
                    progress.validate_handwritten(accepted, denominator, self.handwritten)

    def test_handwritten_source_family_cannot_be_missing_unknown_or_relabelled(self):
        accepted, denominator = self.classify_game()
        for family in (None, "unknown", "opus"):
            with self.subTest(family=family):
                self.hw_source["family"] = family
                self.rewrite_classification_source()
                with self.assertRaisesRegex(ValueError, "source family|classifier family disagrees"):
                    progress.validate_handwritten(accepted, denominator, self.handwritten)

    def test_handwritten_list_source_requires_matching_hashed_authorship_attestation(self):
        accepted, denominator = self.classify_game()
        self.hw_source = self.hw_source["results"]
        self.rewrite_classification_source()
        self.hw_review.update(records="scratch/handwritten-source.json", author_reviewed="Astra (GPT)")
        self.hw_review_path.write_text(json.dumps(self.hw_review))
        ref = self.handwritten["functions"][0]["reviews"][0]
        ref.update(progress._reference(self.root, self.hw_review_path))
        counts = progress.validate_handwritten(accepted, denominator, self.handwritten)
        self.assertEqual(counts["main_14400"], {"functions": 1, "instruction_bytes": 16})
        for changes in ({"author_reviewed": None}, {"author_reviewed": "opus"},
                        {"records": "scratch/unrelated.json"}):
            with self.subTest(changes=changes):
                changed = {**self.hw_review, **changes}
                self.hw_review_path.write_text(json.dumps(changed))
                ref.update(progress._reference(self.root, self.hw_review_path))
                with self.assertRaisesRegex(ValueError, "classifier family disagrees"):
                    progress.validate_handwritten(accepted, denominator, self.handwritten)
        self.hw_review["reviewer_family"] = "GPT (Astra)"
        self.hw_review_path.write_text(json.dumps(self.hw_review))
        ref.update(progress._reference(self.root, self.hw_review_path), family="gpt")
        self.handwritten["functions"][0]["classifier_family"] = "claude"
        with self.assertRaisesRegex(ValueError, "classifier family disagrees"):
            progress.validate_handwritten(accepted, denominator, self.handwritten)
        self.handwritten["functions"][0]["classifier_family"] = "gpt"
        with self.assertRaisesRegex(ValueError, "cross-family review"):
            progress.validate_handwritten(accepted, denominator, self.handwritten)

    def test_handwritten_uncertain_missing_or_duplicate_verdict_is_refused(self):
        accepted, denominator = self.classify_game()
        for items in ([{"symbol": "game", "verdict": "uncertain"}],
                      [{"symbol": "unrelated", "verdict": "confirmed"}],
                      [{"symbol": "game", "verdict": "confirmed"}] * 2):
            with self.subTest(items=items):
                self.hw_review["functions"] = items
                self.hw_review_path.write_text(json.dumps(self.hw_review))
                self.handwritten["functions"][0]["reviews"][0].update(progress._reference(self.root, self.hw_review_path))
                with self.assertRaisesRegex(ValueError, "does not confirm"):
                    progress.validate_handwritten(accepted, denominator, self.handwritten)

    def test_handwritten_item_verdict_not_unrelated_overall_status_controls_credit(self):
        accepted, denominator = self.classify_game()
        self.handwritten["functions"][0]["classifier_family"] = "claude"
        self.hw_source["family"] = "opus"
        self.rewrite_classification_source()
        self.hw_review = {"reviewer_family": "astra", "status": "FAIL",
                          "item_verdicts": [{"symbol": "game", "verdict": "confirmed",
                                             "classification": "handwritten"}]}
        self.hw_review_path.write_text(json.dumps(self.hw_review))
        self.handwritten["functions"][0]["reviews"] = [{**progress._reference(self.root, self.hw_review_path),
                                                       "family": "gpt", "items": "item_verdicts"}]
        counts = progress.validate_handwritten(accepted, denominator, self.handwritten)
        self.assertEqual(counts["main_14400"], {"functions": 1, "instruction_bytes": 16})
        self.assertEqual(counts["resident"], {"functions": 0, "instruction_bytes": 0})

    def test_show_adds_live_handwritten_credit_without_rewriting_c_history(self):
        initial = self.record()["report"]
        paths = [self.root / "docs" / name for name in ("progress.json", "progress-history.jsonl", "PROGRESS.md")]
        before = {p: p.read_bytes() for p in paths}
        self.classify_game()
        shown = self.show()
        self.assertEqual(shown["coverage"], initial["coverage"])
        self.assertEqual(shown["accounting"]["classified_handwritten_instruction_bytes"], 16)
        self.assertEqual(shown["accounting"]["matched_c_instruction_bytes"], 8)
        self.assertEqual(shown["accounting"]["matched_cpp_instruction_bytes"], 0)
        self.assertEqual(shown["accounting"]["accounted_instruction_bytes"], 24)
        self.assertEqual(shown["accounting"]["mapped_catalogue_accounted_percent"], 100)
        self.assertEqual(before, {p: p.read_bytes() for p in paths})
        self.hw_review_path.write_text("{}")
        with self.assertRaisesRegex(ValueError, "Changed progress evidence"):
            self.show()

    def test_accounting_sums_c_cpp_and_handwritten_once_per_image(self):
        self.record()
        accepted, denominator = self.classify_game()
        cpp = {"image_id": "main_14400", "symbol": "cpp", "source": "src/game.cpp",
               "rom_start": 0x2010, "vram_start": 0x800413D0, "size": 8}
        digest = hashlib.sha256(accepted.original[0x2010:0x2018]).hexdigest()
        denominator["identity"]["functions"].append({k: v for k, v in cpp.items() if k != "source"} |
            {"sha256": digest, "handwritten_annotation": False, "internal_aliases": []})
        denominator["denominator_id"] = progress.json_digest(denominator["identity"])
        denominator["audit"] = progress.audit_catalogue(denominator["identity"], accepted.original)
        accepted.document["c_matches"].append(cpp)
        accepted.document["c_attributions"].append({**cpp, "reference_sha256": digest})
        accepted.document["coverage"].update(matched_cpp_functions=1, matched_cpp_bytes=8,
                                             matched_c_and_cpp_functions=2, matched_c_and_cpp_bytes=16)
        counts = progress.calculate(accepted, denominator)
        before = copy.deepcopy(counts)
        result = progress.handwritten_accounting(accepted, denominator, counts)["accounting"]
        self.assertEqual(counts, before)
        self.assertEqual(result["accounted_instruction_bytes"], 32)
        self.assertEqual(result["mapped_catalogue_accounted_percent"], 100)
        self.assertEqual(result["images"]["resident"]["accounted_instruction_bytes"], 8)
        main = result["images"]["main_14400"]
        self.assertEqual((main["matched_c_instruction_bytes"], main["matched_cpp_instruction_bytes"],
                          main["classified_handwritten_instruction_bytes"], main["accounted_instruction_bytes"]),
                         (0, 8, 16, 24))
        self.assertEqual(result["provisional_catalogued_instruction_bytes"], 32)

    def test_unmapped_accounting_denominator_stays_unknown(self):
        accepted = self.accepted()
        denominator = progress.freeze_catalogue(accepted, progress._reference(self.root, self.validation_path))
        denominator["identity"]["functions"] = []
        denominator["denominator_id"] = progress.json_digest(denominator["identity"])
        denominator["audit"] = progress.audit_catalogue(denominator["identity"], accepted.original)
        accepted.document["c_matches"] = []
        accepted.document["coverage"].update(matched_c_functions=0, matched_c_bytes=0)
        counts = progress.calculate(accepted, denominator)
        result = progress.handwritten_accounting(accepted, denominator, counts)["accounting"]
        self.assertIsNone(result["mapped_catalogue_accounted_percent"])
        for row in result["images"].values():
            self.assertIsNone(row["mapped_catalogue_accounted_percent"])
            self.assertEqual(row["accounted_instruction_bytes"], 0)

if __name__ == "__main__":
    unittest.main()
