from __future__ import annotations

import copy
import sys
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "tools"))
from evidence import require_inventory, validate_attributions, validate_matches


PILOT = {
    "symbol": "func_80025EC0",
    "source": "src/hardware_ai_length.c",
    "rom_start": 0x12C0,
    "vram_start": 0x80025EC0,
    "size": 16,
    "evidence": "Reviewed four-instruction pilot",
}
OBJECT = "obj/source/src/hardware_ai_length.o"
GRAPH = {OBJECT: {"source": PILOT["source"], "source_kind": "c", "image_id": "resident"}}
ATTRIBUTION = {
    "symbol": PILOT["symbol"],
    "source": PILOT["source"],
    "object": OBJECT,
    "vram_start": PILOT["vram_start"],
    "size": PILOT["size"],
}
OVERLAYS = {
    "overlay_1339f0": {"rom_start": 0x1339F0, "rom_end": 0x136DC0, "vram_start": 0x801E4EA0},
    "overlay_136dc0": {"rom_start": 0x136DC0, "rom_end": 0x148460, "vram_start": 0x801E4EA0},
}


def overlays() -> list[dict[str, object]]:
    return [
        {"image_id": name, "symbol": "shared_name", "source": "src/overlays/shared.c", "rom_start": image["rom_start"], "vram_start": image["vram_start"], "size": 16}
        for name, image in OVERLAYS.items()
    ]


class InventoryTests(unittest.TestCase):
    def test_exact_inventory_and_empty_expected_inventory(self) -> None:
        require_inventory({"cc1": "hash", "gcc": "hash"}, {"gcc", "cc1"}, "compiler")
        require_inventory({}, set(), "intentionally empty")

    def test_omitted_or_extra_evidence_fails(self) -> None:
        for actual in [{}, {"gcc": "hash"}, {"gcc": "hash", "cc1": "hash", "undeclared": "hash"}]:
            with self.subTest(actual=actual), self.assertRaisesRegex(ValueError, "inventory disagrees"):
                require_inventory(actual, {"gcc", "cc1"}, "compiler")

    def test_wrong_inventory_types_fail(self) -> None:
        with self.assertRaisesRegex(ValueError, "dictionary"):
            require_inventory([], {"gcc"}, "compiler")
        with self.assertRaisesRegex(ValueError, "strings"):
            require_inventory({1: "hash"}, {"gcc"}, "compiler")


class MatchTests(unittest.TestCase):
    def test_current_manifest_shape_and_explicit_resident_are_compatible(self) -> None:
        self.assertEqual(validate_matches([PILOT]), {"matched_c_functions": 1, "matched_c_bytes": 16})
        self.assertEqual(validate_matches([{**PILOT, "image_id": "resident"}]), validate_matches([PILOT]))
        self.assertEqual(validate_matches([]), {"matched_c_functions": 0, "matched_c_bytes": 0})

    def test_distinct_functions_in_one_source_and_adjacent_ranges(self) -> None:
        next_match = {**PILOT, "symbol": "another_function", "rom_start": 0x12D0, "vram_start": 0x80025ED0}
        self.assertEqual(validate_matches([next_match, PILOT]), {"matched_c_functions": 2, "matched_c_bytes": 32})

    def test_duplicate_symbol_or_function_interval_fails(self) -> None:
        for other in [dict(PILOT), {**PILOT, "rom_start": 0x12D0, "vram_start": 0x80025ED0}, {**PILOT, "symbol": "alias_function"}]:
            with self.subTest(other=other), self.assertRaises(ValueError):
                validate_matches([PILOT, other])

    def test_partial_range_overlap_fails(self) -> None:
        overlapping = {**PILOT, "symbol": "overlap", "rom_start": 0x12C8, "vram_start": 0x80025EC8}
        with self.assertRaisesRegex(ValueError, "Overlapping original ROM C ranges"):
            validate_matches([PILOT, overlapping])

    def test_invalid_extent_type_alignment_or_size_fails(self) -> None:
        changes = [
            ("size", True), ("size", 16.0), ("size", "16"), ("size", 0), ("size", -4), ("size", 6),
            ("rom_start", True), ("rom_start", -4), ("rom_start", 0x12C1),
            ("vram_start", True), ("vram_start", 0x80025EC1), ("vram_start", 0x100000000),
        ]
        for key, value in changes:
            with self.subTest(key=key, value=value), self.assertRaises(ValueError):
                validate_matches([{**PILOT, key: value}])

    def test_function_range_outside_resident_or_wrong_mapping_fails(self) -> None:
        cases = [
            {**PILOT, "vram_start": 0x80025EC4},
            {**PILOT, "rom_start": 0xFFC, "vram_start": 0x80025BFC},
            {**PILOT, "rom_start": 0x12338, "vram_start": 0x80036F38},
        ]
        for match in cases:
            with self.subTest(match=match), self.assertRaises(ValueError):
                validate_matches([match])

    def test_relative_c_source_policy_and_required_fields(self) -> None:
        for source in ["src/../outside.c", "src//unit.c", "./src/unit.c", "/src/unit.c", "src\\unit.c", "include/unit.c", "src/unit.s", "src"]:
            with self.subTest(source=source), self.assertRaises(ValueError):
                validate_matches([{**PILOT, "source": source}])
        missing = dict(PILOT)
        missing.pop("size")
        with self.assertRaisesRegex(ValueError, "missing fields"):
            validate_matches([missing])

    def test_overlays_share_ram_and_symbol_but_not_original_rom(self) -> None:
        self.assertEqual(validate_matches(overlays(), OVERLAYS), {"matched_c_functions": 2, "matched_c_bytes": 32})
        with self.assertRaisesRegex(ValueError, "Unknown image"):
            validate_matches(overlays())

    def test_explicit_image_mapping_and_extents_are_checked(self) -> None:
        wrong = overlays()
        wrong[1]["vram_start"] = 0x801E4EA4
        with self.assertRaisesRegex(ValueError, "inconsistent ROM/VRAM"):
            validate_matches(wrong, OVERLAYS)
        for field, value in [("rom_start", True), ("rom_end", 0x1339F0), ("vram_end", 0x801E4EA4)]:
            images = copy.deepcopy(OVERLAYS)
            images["overlay_1339f0"][field] = value
            with self.subTest(field=field), self.assertRaises(ValueError):
                validate_matches(overlays(), images)

    def test_original_rom_alias_images_and_function_aliases_fail(self) -> None:
        images = copy.deepcopy(OVERLAYS)
        images["alias"] = dict(images["overlay_1339f0"])
        with self.assertRaisesRegex(ValueError, "Overlapping original ROM image ranges"):
            validate_matches([], images)
        matches = overlays()
        matches[1]["rom_start"] = matches[0]["rom_start"]
        with self.assertRaisesRegex(ValueError, "Overlapping original ROM C ranges"):
            validate_matches(matches, OVERLAYS)


class AttributionTests(unittest.TestCase):
    def test_valid_pilot_allows_extra_diagnostics(self) -> None:
        attribution = {**ATTRIBUTION, "reference_sha256": "reference", "linked_sha256": "linked", "map_section": ".text"}
        validate_attributions([PILOT], [attribution], GRAPH)
        validate_attributions([], [], {})

    def test_several_functions_in_one_c_object(self) -> None:
        second = {**PILOT, "symbol": "second", "rom_start": 0x12D0, "vram_start": 0x80025ED0}
        attribution = {**ATTRIBUTION, "symbol": "second", "vram_start": second["vram_start"]}
        validate_attributions([PILOT, second], [ATTRIBUTION, attribution], GRAPH)

    def test_overlay_image_disambiguates_identical_symbols_and_sources(self) -> None:
        matches = overlays()
        graph = {}
        records = []
        for index, match in enumerate(matches):
            object_id = f"obj/overlay_{index}/shared.o"
            graph[object_id] = {"source": match["source"], "source_kind": "c", "image_id": match["image_id"]}
            records.append({**match, "object": object_id})
        validate_attributions(matches, records, graph)
        records[0]["object"] = records[1]["object"]
        with self.assertRaisesRegex(ValueError, "declared C object"):
            validate_attributions(matches, records, graph)

    def test_missing_extra_and_duplicate_attributions_fail(self) -> None:
        cases = [[], [ATTRIBUTION, dict(ATTRIBUTION)], [{**ATTRIBUTION, "symbol": "unaccepted"}]]
        for attributions in cases:
            with self.subTest(attributions=attributions), self.assertRaises(ValueError):
                validate_attributions([PILOT], attributions, GRAPH)

    def test_wrong_core_attribution_fields_fail(self) -> None:
        for key, value in [
            ("source", "src/another.c"), ("object", "obj/source/another.o"), ("image_id", "other"),
            ("vram_start", 0x80025EC4), ("vram_start", True), ("size", 12), ("size", True),
        ]:
            with self.subTest(key=key), self.assertRaises(ValueError):
                validate_attributions([PILOT], [{**ATTRIBUTION, key: value}], GRAPH)
        record = dict(ATTRIBUTION)
        record.pop("object")
        with self.assertRaisesRegex(ValueError, "missing fields"):
            validate_attributions([PILOT], [record], GRAPH)

    def test_declared_graph_c_source_kind_and_image_are_authoritative(self) -> None:
        cases = [
            {},
            {OBJECT: {"source": "generated/original.s", "source_kind": "asm"}},
            {OBJECT: {"source": "generated/original.bin", "source_kind": "binary"}},
            {OBJECT: {"source": "src/another.c", "source_kind": "c"}},
            {OBJECT: {"source": PILOT["source"], "source_kind": "c", "image_id": "other"}},
        ]
        for graph in cases:
            with self.subTest(graph=graph), self.assertRaisesRegex(ValueError, "declared C object"):
                validate_attributions([PILOT], [ATTRIBUTION], graph)

    def test_ambiguous_graph_objects_and_path_escapes_fail(self) -> None:
        ambiguous = {**GRAPH, "obj/duplicate.o": dict(GRAPH[OBJECT])}
        with self.assertRaisesRegex(ValueError, "Ambiguous declared C objects"):
            validate_attributions([PILOT], [ATTRIBUTION], ambiguous)
        with self.assertRaisesRegex(ValueError, "without escapes"):
            validate_attributions([PILOT], [ATTRIBUTION], {"obj/../escape.o": GRAPH[OBJECT]})
        with self.assertRaisesRegex(ValueError, "canonical relative path"):
            validate_attributions([PILOT], [{**ATTRIBUTION, "object": "/absolute.o"}], GRAPH)


if __name__ == "__main__":
    unittest.main()
