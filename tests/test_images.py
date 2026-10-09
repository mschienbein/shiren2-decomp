from __future__ import annotations

import copy
import json
import sys
import tempfile
import unittest
from pathlib import Path
from typing import Any

PROJECT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(PROJECT / "tools"))
from evidence import validate_matches
from images import (
    ImageInventory, image_for_rom, image_for_segment, image_for_source,
    load_inventory, policy_records, require_cpu_range, validate_inventory,
)
from rom import EXPECTED_SHA256, EXPECTED_SIZE


def document() -> dict[str, Any]:
    """Raw JSON fixture; negative cases intentionally violate field types."""
    return json.loads((PROJECT / "config/images.json").read_text(encoding="utf-8"))


class ImageInventoryTests(unittest.TestCase):
    def setUp(self) -> None:
        self.document = document()
        self.inventory = validate_inventory(self.document)

    def test_reviewed_extents_and_bss_are_distinct(self) -> None:
        self.assertIsInstance(self.inventory, ImageInventory)
        self.assertEqual(self.inventory.target_sha256, EXPECTED_SHA256)
        self.assertEqual(self.inventory.target_size, EXPECTED_SIZE)
        main = self.inventory.images["main_14400"]
        self.assertEqual((main.rom_start, main.rom_end), (0x14400, 0x1339F0))
        self.assertEqual((main.vram_start, main.vram_end), (0x800413C0, 0x801609B0))
        self.assertEqual(main.size, 0x11F5F0)
        self.assertEqual(main.vram_rom_delta, 0x8002CFC0)
        self.assertEqual([(part.vram_start, part.vram_end) for part in main.bss_ranges], [(0x801609B0, 0x801E4EA0)])
        self.assertEqual(self.inventory.images["overlay_1339f0"].bss_status, "unknown")
        self.assertEqual(self.inventory.images["overlay_1339f0"].bss_ranges, ())

    def test_incomplete_inventory_has_no_code_denominator(self) -> None:
        self.assertFalse(self.inventory.inventory_complete)
        self.assertFalse(self.inventory.cpu_inventory_complete)
        self.assertIsNone(self.inventory.code_denominator_bytes)
        tail = self.inventory.unknown_rom_ranges[0]
        self.assertEqual((tail.rom_start, tail.rom_end), (0x148460, EXPECTED_SIZE))
        for key, value in [
            ("inventory_complete", True), ("inventory_complete", 0),
            ("cpu_inventory_complete", True), ("cpu_inventory_complete", 0),
            ("code_denominator_bytes", 0), ("code_denominator_bytes", 0x148460),
        ]:
            altered = copy.deepcopy(self.document)
            altered[key] = value
            with self.subTest(key=key, value=value), self.assertRaisesRegex(ValueError, "incomplete"):
                validate_inventory(altered)

    def test_wrong_fixed_target_and_unsupported_schema_fail(self) -> None:
        for key, value in [
            ("target_sha256", "0" * 64), ("target_size", EXPECTED_SIZE - 4),
            ("target_size", True), ("schema_version", True), ("schema_version", 2),
            ("kind", "another-game"),
        ]:
            altered = copy.deepcopy(self.document)
            altered[key] = value
            with self.subTest(key=key), self.assertRaises(ValueError):
                validate_inventory(altered)

    def test_missing_or_additional_schema_fields_fail(self) -> None:
        for key in ("images", "components", "load_windows", "evidence", "unknown_rom_ranges"):
            altered = copy.deepcopy(self.document)
            altered.pop(key)
            with self.subTest(key=key), self.assertRaisesRegex(ValueError, "fields disagree"):
                validate_inventory(altered)
        self.document["claimed_c_percent"] = 100
        with self.assertRaisesRegex(ValueError, "fields disagree"):
            validate_inventory(self.document)

    def test_duplicate_and_omitted_original_images_fail(self) -> None:
        for action in ("duplicate", "omit"):
            altered = copy.deepcopy(self.document)
            if action == "duplicate":
                altered["images"].append(copy.deepcopy(altered["images"][0]))
            else:
                altered["images"].pop()
            with self.subTest(action=action), self.assertRaises(ValueError):
                validate_inventory(altered)

    def test_original_rom_storage_overlap_is_rejected(self) -> None:
        self.document["images"][3]["rom_start"] = 0x136DBC
        self.document["images"][3]["vram_end"] += 4
        with self.assertRaisesRegex(ValueError, "Overlapping original ROM image storage"):
            validate_inventory(self.document)

    def test_equal_length_changed_loader_mapping_is_rejected(self) -> None:
        overlay = self.document["images"][3]
        overlay["vram_start"] = 0x80200000
        overlay["vram_end"] = overlay["vram_start"] + overlay["rom_end"] - overlay["rom_start"]
        with self.assertRaisesRegex(ValueError, "reviewed fixed-target loader mapping"):
            validate_inventory(self.document)

    def test_invalid_image_extents_and_storage_formats_fail(self) -> None:
        for key, value in [
            ("rom_start", True), ("rom_end", 0x1000), ("rom_end", 0x14401),
            ("rom_start", -4), ("rom_end", EXPECTED_SIZE + 4),
            ("vram_start", "0x80025C00"), ("vram_end", 0x100000004),
            ("vram_end", 0x80039004), ("storage_format", "compressed"),
            ("confidence", "complete-text"),
        ]:
            altered = copy.deepcopy(self.document)
            altered["images"][0][key] = value
            with self.subTest(key=key, value=value), self.assertRaises(ValueError):
                validate_inventory(altered)

    def test_overlays_share_explicit_slot_but_have_distinct_storage(self) -> None:
        first, second = (self.inventory.images[name] for name in ("overlay_1339f0", "overlay_136dc0"))
        self.assertEqual(first.vram_start, second.vram_start)
        self.assertEqual(first.load_slot, second.load_slot)
        self.assertEqual(first.rom_end, second.rom_start)
        self.document["images"][3]["load_slot"] = "unrelated_slot"
        with self.assertRaisesRegex(ValueError, "explicit shared load slot"):
            validate_inventory(self.document)

    def test_bss_cannot_become_stored_bytes_or_overlap_initialized_ram(self) -> None:
        for change in ("rom", "overlap", "changed_mapping", "unknown_status"):
            altered = copy.deepcopy(self.document)
            if change == "rom":
                altered["images"][0]["bss_ranges"][0]["rom_start"] = 0x14400
            elif change == "overlap":
                altered["images"][0]["bss_ranges"][0]["vram_start"] = 0x80038FFC
            elif change == "changed_mapping":
                altered["images"][0]["bss_ranges"][0]["vram_end"] -= 4
            else:
                altered["images"][0]["bss_status"] = "unknown"
            with self.subTest(change=change), self.assertRaises(ValueError):
                validate_inventory(altered)

    def test_malformed_nested_statuses_fail_with_policy_errors(self) -> None:
        for key in ("bss_status", "split_status"):
            for value in ([], {}, True, None):
                altered = copy.deepcopy(self.document)
                altered["images"][0][key] = value
                with self.subTest(key=key, value=value), self.assertRaises(ValueError):
                    validate_inventory(altered)

    def test_evidence_requires_valid_identifiers_spans_paths_and_hashes(self) -> None:
        for key, value in [
            ("evidence_id", "../escape"), ("rom_start", 0x1001),
            ("rom_end", EXPECTED_SIZE + 4), ("sha256", "not-a-hash"),
            ("reference", "docs/../outside.md"), ("reference", "/tmp/evidence.md"),
            ("confidence", "guessed"),
        ]:
            altered = copy.deepcopy(self.document)
            altered["evidence"][0][key] = value
            with self.subTest(key=key), self.assertRaises(ValueError):
                validate_inventory(altered)
        self.document["evidence"].append(copy.deepcopy(self.document["evidence"][0]))
        with self.assertRaisesRegex(ValueError, "Duplicate loader evidence"):
            validate_inventory(self.document)

    def test_missing_loader_reference_fails(self) -> None:
        self.document["images"][1]["evidence"].append("unreviewed_transfer")
        with self.assertRaisesRegex(ValueError, "unknown loader evidence"):
            validate_inventory(self.document)

    def test_rsp_component_keeps_parent_storage_and_execution_separate(self) -> None:
        rsp = self.inventory.components[0]
        self.assertEqual(rsp.parent_image_id, "main_14400")
        self.assertEqual((rsp.rom_start, rsp.rom_end), (0x109A50, 0x109B20))
        self.assertEqual((rsp.storage_vram_start, rsp.storage_vram_end), (0x80136A10, 0x80136AE0))
        self.assertEqual((rsp.execution_start, rsp.execution_end), (0x04001000, 0x040010D0))
        for key, value in [
            ("parent_image_id", "resident"), ("storage_vram_start", 0x80136A14),
            ("execution_end", 0x040010D4), ("cpu_c_excluded", False),
            ("rom_start", 0x109A54), ("kind", "cpu"),
        ]:
            altered = copy.deepcopy(self.document)
            altered["components"][0][key] = value
            with self.subTest(key=key), self.assertRaises(ValueError):
                validate_inventory(altered)

    def test_provisional_cpu_hints_cannot_claim_rsp_or_final_boundaries(self) -> None:
        for change in ("rsp", "outside", "confidence"):
            altered = copy.deepcopy(self.document)
            hint = altered["images"][1]["provisional_cpu_ranges"][0]
            if change == "rsp":
                hint["rom_end"] = 0x109AA0
            elif change == "outside":
                hint["rom_start"] = 0x1000
            else:
                hint["confidence"] = "confirmed"
            with self.subTest(change=change), self.assertRaises(ValueError):
                validate_inventory(altered)

    def test_temporary_boot_window_cannot_claim_unique_image_ownership(self) -> None:
        for key, value in [
            ("unique_image", True), ("unique_image", 0),
            ("rom_end", 0x14400), ("purpose", "permanent-code-image"),
        ]:
            altered = copy.deepcopy(self.document)
            altered["load_windows"][0][key] = value
            with self.subTest(key=key), self.assertRaises(ValueError):
                validate_inventory(altered)

    def test_unknown_tail_cannot_be_silently_omitted_or_classified(self) -> None:
        for change in ("omit", "overlap", "truncate"):
            altered = copy.deepcopy(self.document)
            if change == "omit":
                altered["unknown_rom_ranges"] = []
            elif change == "overlap":
                altered["unknown_rom_ranges"][0]["rom_start"] -= 4
            else:
                altered["unknown_rom_ranges"][0]["rom_end"] -= 4
            with self.subTest(change=change), self.assertRaises(ValueError):
                validate_inventory(altered)

    def test_source_and_segment_bindings_reject_escapes_and_ambiguity(self) -> None:
        for source in [
            "src/../outside.c", "./src/unit.c", "src//unit.c",
            "/src/unit.c", "src\\unit.c", "include/unit.c", "src/unit.s",
        ]:
            altered = copy.deepcopy(self.document)
            altered["source_bindings"] = {source: "resident"}
            with self.subTest(source=source), self.assertRaises(ValueError):
                validate_inventory(altered)
        self.document["images"][1]["splat_segments"] = ["main"]
        with self.assertRaisesRegex(ValueError, "Ambiguous splat segment"):
            validate_inventory(self.document)

    def test_main_split_can_be_promoted_without_enabling_overlay_linkage(self) -> None:
        self.document["images"][1]["split_status"] = "active"
        self.assertEqual(validate_inventory(self.document).images["main_14400"].split_status, "active")
        self.document["images"][2]["splat_segments"] = ["overlay_1339f0"]
        self.document["images"][2]["split_status"] = "active"
        with self.assertRaisesRegex(ValueError, "Overlay split/linkage is unsupported"):
            validate_inventory(self.document)

    def test_storage_only_b_has_exact_mapping_and_no_cpu_or_bss_claim(self) -> None:
        overlay = self.inventory.images["overlay_136dc0"]
        self.assertEqual(overlay.split_status, "active")
        self.assertEqual(overlay.splat_segments, ("overlay_b_136dc0_storage",))
        self.assertEqual((overlay.rom_start, overlay.rom_end), (0x136DC0, 0x148460))
        self.assertEqual((overlay.vram_start, overlay.vram_end), (0x801E4EA0, 0x801F6540))
        self.assertEqual(overlay.size, 71328)
        self.assertEqual(overlay.vram_rom_delta, 0x800AE0E0)
        self.assertEqual(overlay.bss_status, "unknown")
        self.assertEqual(overlay.bss_ranges, ())
        self.assertEqual(overlay.provisional_cpu_ranges, ())
        self.assertEqual(self.inventory.images["overlay_1339f0"].split_status, "opaque")
        self.assertFalse(self.inventory.cpu_inventory_complete)
        self.assertIsNone(self.inventory.code_denominator_bytes)

    def test_opaque_b_remains_supported_without_segment_identity(self) -> None:
        self.document["images"][3]["split_status"] = "opaque"
        self.document["images"][3]["splat_segments"] = []
        inventory = validate_inventory(self.document)
        self.assertEqual(inventory.images["overlay_136dc0"].split_status, "opaque")
        self.assertIsNone(image_for_segment("overlay_b_136dc0_storage", inventory))

    def test_other_b_linkage_shapes_are_rejected(self) -> None:
        for status, segments in [
            ("planned", ["overlay_b_136dc0_storage"]),
            ("active", ["overlay_136dc0"]),
            ("active", ["overlay_b_136dc0_storage", "extra_b_segment"]),
        ]:
            altered = copy.deepcopy(self.document)
            altered["images"][3]["split_status"] = status
            altered["images"][3]["splat_segments"] = segments
            with self.subTest(status=status, segments=segments), self.assertRaisesRegex(ValueError, "storage-only overlay B"):
                validate_inventory(altered)

    def test_shared_slot_does_not_allow_active_a_with_b(self) -> None:
        self.document["images"][2]["split_status"] = "active"
        self.document["images"][2]["splat_segments"] = ["overlay_a_storage"]
        with self.assertRaisesRegex(ValueError, "unsupported for overlay_1339f0"):
            validate_inventory(self.document)

    def test_storage_only_overlays_cannot_bind_c_sources(self) -> None:
        for name in ("overlay_1339f0", "overlay_136dc0"):
            altered = copy.deepcopy(self.document)
            altered["source_bindings"]["src/overlay_binding.c"] = name
            with self.subTest(image=name), self.assertRaisesRegex(ValueError, "Overlay source bindings are unsupported"):
                validate_inventory(altered)

    def test_cpp_sources_bind_and_cannot_share_a_c_unit_object(self) -> None:
        self.document["source_bindings"]["src/units/main_14400/cxx_unit.cpp"] = "main_14400"
        inventory = validate_inventory(self.document)
        self.assertEqual(image_for_source("src/units/main_14400/cxx_unit.cpp", inventory).image_id, "main_14400")
        self.document["source_bindings"]["src/units/main_14400/cxx_unit.c"] = "main_14400"
        with self.assertRaisesRegex(ValueError, "share one object path"):
            validate_inventory(self.document)
        with self.assertRaisesRegex(ValueError, "ending in .c or .cpp"):
            image_for_source("src/units/main_14400/cxx_unit.cc", inventory)

    def test_b_clear_leads_cannot_be_promoted_into_bss(self) -> None:
        overlay = self.document["images"][3]
        overlay["bss_status"] = "reviewed"
        overlay["bss_ranges"] = [{
            "vram_start": 0x801F6540, "vram_end": 0x801F6544,
            "evidence": ["overlay_later_transfer"],
        }]
        with self.assertRaisesRegex(ValueError, "reviewed BSS mapping"):
            validate_inventory(self.document)

    def test_image_digest_shape_is_not_original_byte_proof(self) -> None:
        self.document["images"][3]["rom_sha256"] = "0" * 64
        self.assertEqual(validate_inventory(self.document).images["overlay_136dc0"].rom_sha256, "0" * 64)

    def test_explicit_json_loading_rejects_duplicate_fields(self) -> None:
        self.assertEqual(load_inventory(PROJECT / "config/images.json").target_sha256, EXPECTED_SHA256)
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "images.json"
            raw = json.dumps(self.document)
            path.write_text(raw.replace('"schema_version": 1', '"schema_version": 1, "schema_version": 1'), encoding="utf-8")
            with self.assertRaisesRegex(ValueError, "Duplicate JSON field"):
                load_inventory(path)

    def test_parsed_records_are_immutable_and_independent_of_input(self) -> None:
        self.document["images"][0]["load_slot"] = "altered"
        self.assertEqual(self.inventory.images["resident"].load_slot, "resident")
        with self.assertRaises(TypeError):
            self.inventory.images["extra"] = self.inventory.images["resident"]
        with self.assertRaises(TypeError):
            self.inventory.source_bindings["src/extra.c"] = "resident"


class ImageHelperTests(unittest.TestCase):
    def setUp(self) -> None:
        self.inventory = load_inventory(PROJECT / "config/images.json")

    def test_lookup_uses_original_storage_with_exclusive_ends(self) -> None:
        for offset, expected in [
            (0x1000, "resident"), (0x143FF, "resident"), (0x14400, "main_14400"),
            (0x109A50, "main_14400"), (0x1339EF, "main_14400"),
            (0x1339F0, "overlay_1339f0"), (0x136DBF, "overlay_1339f0"),
            (0x136DC0, "overlay_136dc0"), (0x14845F, "overlay_136dc0"),
        ]:
            with self.subTest(offset=hex(offset)):
                self.assertEqual(image_for_rom(offset, self.inventory).image_id, expected)
        for offset in (0, 0x40, 0x148460, EXPECTED_SIZE - 1):
            self.assertIsNone(image_for_rom(offset, self.inventory))
        for offset in (-1, True, EXPECTED_SIZE, "0x14400"):
            with self.subTest(offset=offset), self.assertRaises(ValueError):
                image_for_rom(offset, self.inventory)

    def test_boot_copy_and_rsp_do_not_duplicate_policy_ownership(self) -> None:
        records = policy_records(self.inventory)
        self.assertEqual(set(records), {"resident", "main_14400", "overlay_1339f0", "overlay_136dc0"})
        self.assertNotIn("ipl3_boot_window", records)
        self.assertNotIn("rsp_boot", records)
        self.assertEqual(image_for_rom(0x50000, self.inventory).image_id, "main_14400")
        self.assertEqual(records["main_14400"]["vram_start"], 0x800413C0)

    def test_policy_records_are_validate_matches_compatible(self) -> None:
        # Synthetic C declarations test mapping policy, not actual code provenance.
        matches = [
            {"image_id": name, "symbol": "fixture_function", "source": "src/fixture.c",
             "rom_start": image.rom_start, "vram_start": image.vram_start, "size": 16}
            for name, image in self.inventory.images.items()
        ]
        self.assertEqual(validate_matches(matches, policy_records(self.inventory)), {"matched_c_functions": 4, "matched_c_bytes": 64})
        matches[1]["vram_start"] += 4
        with self.assertRaisesRegex(ValueError, "inconsistent ROM/VRAM"):
            validate_matches(matches, policy_records(self.inventory))

    def test_segment_and_explicit_source_bindings_do_not_guess_identity(self) -> None:
        for segment, expected in [("entry", "resident"), ("main", "resident"), ("main_14400", "main_14400")]:
            self.assertEqual(image_for_segment(segment, self.inventory).image_id, expected)
        self.assertIsNone(image_for_segment("remaining", self.inventory))
        self.assertIsNone(image_for_segment("overlay_1339f0", self.inventory))
        self.assertEqual(image_for_segment("overlay_b_136dc0_storage", self.inventory).image_id, "overlay_136dc0")
        self.assertIsNone(image_for_segment("overlay_136dc0_initialized_opaque", self.inventory))
        self.assertEqual(image_for_source("src/hardware_ai_length.c", self.inventory).image_id, "resident")
        self.assertIsNone(image_for_source("src/unbound.c", self.inventory))
        for segment in ("../main", "main/child", "main\\child"):
            with self.subTest(segment=segment), self.assertRaises(ValueError):
                image_for_segment(segment, self.inventory)
        with self.assertRaises(ValueError):
            image_for_source("src/../outside.c", self.inventory)

    def test_cpu_policy_rejects_rsp_intersections_and_invalid_ranges(self) -> None:
        for start, size in [(0x109A50, 4), (0x109B1C, 8), (0x109A4C, 8), (0x109A40, 0xF0)]:
            with self.subTest(start=hex(start)), self.assertRaisesRegex(ValueError, "confirmed non-CPU"):
                require_cpu_range("main_14400", start, size, self.inventory)
        for image, start, size in [
            ("main_14400", 0x143FC, 16), ("resident", 0x14400, 16),
            ("unknown", 0x14400, 16), ("resident", 0x12C1, 16),
            ("resident", 0x12C0, 0), ("resident", 0x12C0, True),
            ("resident", True, 16), ("resident", 0x12C0, -4),
        ]:
            with self.subTest(image=image, start=start, size=size), self.assertRaises(ValueError):
                require_cpu_range(image, start, size, self.inventory)

    def test_provisional_hints_never_become_authoritative_text_boundaries(self) -> None:
        self.assertEqual(require_cpu_range("resident", 0x12C0, 16, self.inventory).image_id, "resident")
        self.assertEqual(require_cpu_range("main_14400", 0x109A40, 16, self.inventory).image_id, "main_14400")
        # Immediately after the RSP blob is outside the provisional CPU hint.
        # The helper permits storage policy only; independent text proof is needed.
        self.assertEqual(require_cpu_range("main_14400", 0x109B20, 16, self.inventory).image_id, "main_14400")
        self.assertEqual(require_cpu_range("overlay_136dc0", 0x136DC0, 16, self.inventory).image_id, "overlay_136dc0")


if __name__ == "__main__":
    unittest.main()
