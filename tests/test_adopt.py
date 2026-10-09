"""Layout edits and adoption fixtures of tools/adopt.py (C/C++ text, initialized-data bridges, BSS/COMMON, absorbs).

Matcher byte proof is exercised separately; these tests substitute only matching/target discovery.
"""
from __future__ import annotations

import io
import json
import sys
import tempfile
import unittest
from contextlib import redirect_stderr, redirect_stdout
from pathlib import Path
from types import SimpleNamespace
from unittest.mock import Mock, patch

import yaml

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "tools"))

from adopt import (Absorbed, Unit, _adopt_batch, _assert_main_bss_unchanged, _load_units, adopt, assert_text_placement,  # noqa: E402
                   carve_bss_plan, enable_bss_common, gate_admit, gate_findings, interface_candidate, interior_aliases, main,
                   restore_bss_yaml, restore_data_yaml, restore_text_yaml, split_bss_yaml, split_data_yaml, split_yaml)
from interfaces import Gate, parse_tu  # noqa: E402
from match import (SHN_ABS, SHN_COMMON, SHT_NOBITS, Section, Symbol, _plan_bss_carve, bss_carve_blockers,  # noqa: E402
                   data_owner, default_profile, external_bss_references, yaml_data_parts)
from workspace import REQUIRED_FILES, input_manifest  # noqa: E402

LAYOUT = """    subsegments:
      - [0x14400, c, game_tick]
      - [0x1443C, asm, main_14400_tail_1443c]
      - [0x15A90, c, units/main_14400/func_80042A50]
"""


class SplitYamlTests(unittest.TestCase):
    def test_unit_inside_asm_range_keeps_prefix_and_adds_tail(self) -> None:
        result = split_yaml(LAYOUT, "main_14400", "func_80041500", 0x14540, 0x20, 0x14560)
        self.assertIn("      - [0x1443C, asm, main_14400_tail_1443c]\n"
                      "      - [0x14540, c, units/main_14400/func_80041500]\n"
                      "      - [0x14560, asm, main_14400_tail_14560]\n", result)

    def test_unit_at_asm_start_replaces_it(self) -> None:
        result = split_yaml(LAYOUT, "main_14400", "func_800413FC", 0x1443C, 0x10, 0x1444C)
        self.assertNotIn("main_14400_tail_1443c", result)
        self.assertIn("      - [0x1443C, c, units/main_14400/func_800413FC]\n"
                      "      - [0x1444C, asm, main_14400_tail_1444c]\n", result)

    def test_unit_abutting_next_subsegment_adds_no_tail_and_pads_gap(self) -> None:
        result = split_yaml(LAYOUT, "main_14400", "func_80042A40", 0x15A80, 0x0C, 0x15A90)
        self.assertIn("      - [0x15A80, c, units/main_14400/func_80042A40]\n"
                      "      - {start: 0x15A8C, type: bin, name: func_80042A40_padding, linker_section_order: .text}\n"
                      "      - [0x15A90, c, units/main_14400/func_80042A50]\n", result)

    def test_unit_inside_c_subsegment_is_rejected(self) -> None:
        with self.assertRaisesRegex(ValueError, "is not inside a asm subsegment"):
            split_yaml(LAYOUT, "main_14400", "func_80041400", 0x14410, 0x8, 0x14418)

    def test_unit_crossing_next_subsegment_is_rejected(self) -> None:
        with self.assertRaisesRegex(ValueError, "starts before the unit's natural end"):
            split_yaml(LAYOUT, "main_14400", "func_80042A40", 0x15A80, 0x20, 0x15AA0)

    def test_cpp_unit_preserves_its_language_and_padding(self) -> None:
        result = split_yaml(LAYOUT, "main_14400", "cxx", 0x14540, 0x0C, 0x14550, "cpp")
        self.assertIn("[0x14540, cpp, units/main_14400/cxx]", result)
        self.assertIn("start: 0x1454C, type: bin", result)
        self.assertIn("[0x14550, asm, main_14400_tail_14550]", result)
        with self.assertRaisesRegex(ValueError, "Unsupported compiled"):
            split_yaml(LAYOUT, "main_14400", "cxx", 0x14540, 0x10, 0x14550, "cc")


class CxxAdoptionTests(unittest.TestCase):
    def test_default_profile_follows_suffix(self) -> None:
        self.assertEqual(default_profile("main_14400", Path("source.cpp")), "gxx281pm-gnu291-O2-unsigned")
        self.assertEqual(default_profile("main_14400", Path("source.c")), "gcc272-modern237-O2-unsigned")
        with self.assertRaisesRegex(ValueError, "No calibrated C\\+\\+"):
            default_profile("resident", Path("source.cpp"))

    def test_cpp_adoption_places_source_and_binds_all_configs(self) -> None:
        # Only matching/target discovery is substituted; file placement, gate use and carving are real.
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary).resolve()  # match.py records resolved participating paths
            (root / "config").mkdir()
            configs = {"matches.json": {"functions": []}, "compiler_profiles.json": {"tu_profiles": {}},
                       "images.json": {"source_bindings": {}}}
            for filename, content in configs.items():
                (root / "config" / filename).write_text(json.dumps(content))
            # adopt() checks text placement against the parent segment's SUBALIGN.
            (root / "config/shiren2.jp.yaml").write_text("options: {}\nsegments:\n  - name: main_14400\n    type: code\n"
                                                         "    start: 0x14400\n    vram: 0x800413C0\n    subalign: 1\n"
                                                         + LAYOUT + "  - [0x2000000]\n")
            (root / "config/symbol_aliases.ld").write_text("")
            candidate = root / "candidate.cpp"
            candidate.write_text('extern "C" int func_80041500(void) { return 0; }\n')
            target = SimpleNamespace(image_id="main_14400", symbol="func_80041500", rom_start=0x14540,
                                     vram_start=0x80041500, size=0x10, slot_end=0x14550)
            matched = SimpleNamespace(status="match", problems=[], participating_files=[{"path": str(candidate)}],
                                      text_bytes=0x10, instruction_bytes=0x10, slot_bytes=0x10,
                                      data_sections=[], bss_carve=None, source={"sha256": "a" * 64})
            gate = Mock()
            gate.check.return_value = []
            unit = Unit([target.symbol], candidate)
            with patch("adopt.PROJECT", root), patch("adopt.resolve_targets", return_value=[target]), \
                    patch("adopt.check", return_value=(matched, None)) as matcher:
                self.assertEqual(interface_candidate(unit)["profile"], "gxx281pm-gnu291-O2-unsigned")
                result = adopt(unit, rom=bytes(0x14550), log=[], gate=gate, index=0)
            source = "src/units/main_14400/func_80041500.cpp"
            self.assertEqual((root / source).read_text(), candidate.read_text())
            self.assertEqual(result, {"functions": 1, "instruction_bytes": 0x10})
            self.assertEqual(json.loads((root / "config/matches.json").read_text())["functions"][0]["source"], source)
            self.assertEqual(json.loads((root / "config/compiler_profiles.json").read_text())["tu_profiles"],
                             {source: "gxx281pm-gnu291-O2-unsigned"})
            self.assertEqual(json.loads((root / "config/images.json").read_text())["source_bindings"], {source: "main_14400"})
            self.assertIn("[0x14540, cpp, units/main_14400/func_80041500]", (root / "config/shiren2.jp.yaml").read_text())
            self.assertEqual(matcher.call_args.args[2], "gxx281pm-gnu291-O2-unsigned")
            gate.check.assert_called_once_with(0)
            gate.admit.assert_called_once_with(0)


BSS_LAYOUT = """segments:
  - name: main
    type: code
    start: 0x1060
    vram: 0x80025C60
    bss_size: 0x83C0
    bss_contains_common: true
    subsegments:
      - [0x1060, asm, resident]
      - {type: bss, vram: 0x80039000, name: resident}
      - {type: .bss, vram: 0x800411D0, name: units/resident/contpfs}
      - {type: bss, vram: 0x800412D0, name: resident_tail_412d0}
  - name: main_14400
    type: code
    start: 0x14400
    vram: 0x800413C0
    bss_size: 0x844F0
    subsegments:
      - [0x14400, c, game_tick]
      - {type: bss, vram: 0x801609B0, name: main_14400}
"""
RESIDENT_BSS_END, MAIN_BSS_END = 0x800413C0, 0x801E4EA0


class SplitBssYamlTests(unittest.TestCase):
    def test_unit_inside_bss_range_keeps_prefix_and_adds_tail(self) -> None:
        result = split_bss_yaml(BSS_LAYOUT, "resident", "func_8002F7C0", 0x8003D9E0, 0x8003EBB0, RESIDENT_BSS_END)
        self.assertIn("      - {type: bss, vram: 0x80039000, name: resident}\n"
                      "      - {type: .bss, vram: 0x8003D9E0, name: units/resident/func_8002F7C0}\n"
                      "      - {type: bss, vram: 0x8003EBB0, name: resident_tail_3ebb0}\n"
                      "      - {type: .bss, vram: 0x800411D0, name: units/resident/contpfs}\n", result)

    def test_unit_at_bss_start_replaces_it(self) -> None:
        result = split_bss_yaml(BSS_LAYOUT, "main_14400", "func_80041600", 0x801609B0, 0x801609C0, MAIN_BSS_END)
        self.assertNotIn("name: main_14400}", result)
        self.assertIn("      - [0x14400, c, game_tick]\n"
                      "      - {type: .bss, vram: 0x801609B0, name: units/main_14400/func_80041600}\n"
                      "      - {type: bss, vram: 0x801609C0, name: main_bss_1609c0}\n", result)

    def test_unit_abutting_next_bss_subsegment_adds_no_tail(self) -> None:
        result = split_bss_yaml(BSS_LAYOUT, "resident", "func_80026000", 0x80041100, 0x800411D0, RESIDENT_BSS_END)
        self.assertIn("      - {type: .bss, vram: 0x80041100, name: units/resident/func_80026000}\n"
                      "      - {type: .bss, vram: 0x800411D0, name: units/resident/contpfs}\n", result)

    def test_last_bss_subsegment_is_bounded_by_the_loader_extent(self) -> None:
        result = split_bss_yaml(BSS_LAYOUT, "resident", "func_80032000", 0x800412E0, 0x800413C0, RESIDENT_BSS_END)
        self.assertTrue(result.endswith("      - {type: .bss, vram: 0x800412E0, name: units/resident/func_80032000}\n"
                                        "  - name: main_14400\n" + BSS_LAYOUT.split("  - name: main_14400\n")[1]))
        with self.assertRaisesRegex(ValueError, "starts before the unit's natural end 0x800413D0"):
            split_bss_yaml(BSS_LAYOUT, "resident", "func_80032000", 0x800412E0, 0x800413D0, RESIDENT_BSS_END)

    def test_unit_crossing_next_bss_subsegment_is_rejected(self) -> None:
        with self.assertRaisesRegex(ValueError, "Next subsegment after resident starts before the unit's natural end 0x800411E0"):
            split_bss_yaml(BSS_LAYOUT, "resident", "func_80026000", 0x80041100, 0x800411E0, RESIDENT_BSS_END)

    def test_existing_tail_name_is_rejected(self) -> None:
        layout = BSS_LAYOUT.replace("name: resident_tail_412d0}", "name: resident_tail_3ebb0}")
        with self.assertRaisesRegex(ValueError, "resident_tail_3ebb0 already exists"):
            split_bss_yaml(layout, "resident", "func_8002F7C0", 0x8003D9E0, 0x8003EBB0, RESIDENT_BSS_END)

    def test_unit_inside_c_bss_carve_is_rejected(self) -> None:
        with self.assertRaisesRegex(ValueError, "is not inside a bss subsegment"):
            split_bss_yaml(BSS_LAYOUT, "resident", "func_80026000", 0x800411E0, 0x800411F0, RESIDENT_BSS_END)

    def test_bss_contains_common_is_added_once(self) -> None:
        result = enable_bss_common(BSS_LAYOUT, "main_14400")
        self.assertIn("    bss_size: 0x844F0\n    bss_contains_common: true\n    subsegments:\n      - [0x14400, c, game_tick]", result)
        self.assertEqual(enable_bss_common(result, "main_14400"), result)
        self.assertEqual(enable_bss_common(BSS_LAYOUT, "main"), BSS_LAYOUT)

    def test_existing_common_flag_is_honoured_in_any_position_or_form(self) -> None:
        # BSS-FIXED-IR-01: the parent mapping is parsed whole; the field may follow subsegments.
        disabled = (
            "    bss_contains_common: false # explicitly disabled after its subsegments\n",
            "    bss_contains_common:\n      false\n",
            "    bss_contains_common: no\n",
        )
        for late in disabled:
            with self.subTest(late=late), self.assertRaisesRegex(ValueError, "explicitly disables"):
                enable_bss_common(BSS_LAYOUT + late, "main_14400")
        reordered = BSS_LAYOUT.replace("  - name: main_14400\n", "  - bss_contains_common: false\n    name: main_14400\n")
        with self.assertRaisesRegex(ValueError, "explicitly disables"):
            enable_bss_common(reordered, "main_14400")
        enabled = BSS_LAYOUT + "    bss_contains_common: true # after subsegments\n"
        self.assertEqual(enable_bss_common(enabled, "main_14400"), enabled)
        with self.assertRaisesRegex(ValueError, "non-boolean"):
            enable_bss_common(BSS_LAYOUT + '    bss_contains_common: "true"\n', "main_14400")

    def test_common_flag_is_never_duplicated(self) -> None:
        duplicate = BSS_LAYOUT.replace("    bss_size: 0x844F0\n", "    bss_size: 0x844F0\n    bss_contains_common: true\n") \
            + "    bss_contains_common: false\n"
        with self.assertRaisesRegex(ValueError, "Duplicate YAML key"):
            enable_bss_common(duplicate, "main_14400")
        commented = BSS_LAYOUT.replace("    subsegments:\n      - [0x14400", "    # COMMON stays with each unit\n    subsegments:  # list\n      - [0x14400")
        result = enable_bss_common(commented, "main_14400")
        parent = yaml.compose(result).value[0][1].value[1]
        self.assertEqual([key.value for key, _value in parent.value].count("bss_contains_common"), 1)
        self.assertIs(yaml.safe_load(result)["segments"][1]["bss_contains_common"], True)
        self.assertEqual(yaml.safe_load(result)["segments"][1]["subsegments"], yaml.safe_load(BSS_LAYOUT)["segments"][1]["subsegments"])

    def test_unsupported_parent_shapes_fail_closed(self) -> None:
        layouts = (
            BSS_LAYOUT.replace("  - name: main_14400\n", "  - subsegments: []\n    name: main_14400\n").replace(
                "    bss_size: 0x844F0\n    subsegments:\n      - [0x14400, c, game_tick]\n"
                "      - {type: bss, vram: 0x801609B0, name: main_14400}\n", "    bss_size: 0x844F0\n"),
            BSS_LAYOUT.split("  - name: main_14400\n")[0]
            + "  - {name: main_14400, type: code, start: 0x14400, vram: 0x800413C0, subsegments: [[0x14400, c, game_tick]]}\n",
            BSS_LAYOUT + "  - name: main_14400\n    type: code\n    subsegments: []\n",
        )
        for layout in layouts:
            with self.subTest(layout=layout), self.assertRaises(ValueError):
                enable_bss_common(layout, "main_14400")


class BssCarveBlockerTests(unittest.TestCase):
    lines = BSS_LAYOUT.splitlines()
    ranges = [(0x80039000, RESIDENT_BSS_END)]

    def test_carve_inside_asm_bss_is_allowed(self) -> None:
        blockers, owner = bss_carve_blockers(self.lines, self.ranges, 0x8003D9E0, 0x8003EBB0, "units/resident/func_8002F7C0")
        self.assertEqual(blockers, [])
        self.assertEqual((owner.kind, owner.name), ("bss", "resident"))

    def test_units_own_carve_is_allowed_and_others_are_not(self) -> None:
        self.assertEqual(bss_carve_blockers(self.lines, self.ranges, 0x800411D0, 0x800412D0, "units/resident/contpfs")[0], [])
        blockers, _owner = bss_carve_blockers(self.lines, self.ranges, 0x800411D0, 0x800412D0, "units/resident/other")
        self.assertRegex(blockers[0], "not inside an asm bss subsegment")

    def test_crossing_and_out_of_range_carves_are_blocked(self) -> None:
        self.assertRegex(bss_carve_blockers(self.lines, self.ranges, 0x80041100, 0x800411E0, None)[0][0], "crosses the next BSS subsegment")
        self.assertRegex(bss_carve_blockers(self.lines, self.ranges, 0x80041300, 0x800413D0, None)[0][0], "not inside a loader-backed BSS range")


class ParsedBssBoundaryTests(unittest.TestCase):
    boundary = "      - {type: .bss, vram: 0x800411D0, name: units/resident/contpfs}"
    formats = (
        boundary + " # existing C owner",
        "      - {name: units/resident/contpfs, vram: 0x800411D0, type: .bss}",
        "      - {vram: 0x800411D0, name: units/resident/contpfs, type: .bss} # existing C owner",
        "      - name: units/resident/contpfs\n        vram: 0x800411D0 # boundary\n        type: .bss",
        "      - {name: units/resident/contpfs,\n         type: .bss,\n         vram: 0x800411D0} # existing C owner",
    )
    ranges = [(0x80039000, RESIDENT_BSS_END)]

    def test_formatted_c_boundary_never_exposes_owned_storage(self) -> None:
        for boundary in self.formats:
            with self.subTest(boundary=boundary):
                layout = BSS_LAYOUT.replace(self.boundary, boundary)
                self.assertEqual(yaml.safe_load(layout), yaml.safe_load(BSS_LAYOUT))
                blockers, owner = bss_carve_blockers(
                    layout.splitlines(), self.ranges, 0x800411E0, 0x800411F0, "units/resident/other")
                self.assertTrue(blockers)
                self.assertEqual((owner.kind, owner.name, owner.vram, owner.next_vram, owner.segment),
                                 (".bss", "units/resident/contpfs", 0x800411D0, 0x800412D0, "main"))
                with self.assertRaises(ValueError):
                    split_bss_yaml(layout, "resident", "other", 0x800411E0, 0x800411F0, RESIDENT_BSS_END)
                own, _owner = bss_carve_blockers(
                    layout.splitlines(), self.ranges, 0x800411D0, 0x800412D0, "units/resident/contpfs")
                self.assertEqual(own, [])

    def test_formatted_c_boundary_still_limits_the_previous_asm_owner(self) -> None:
        for boundary in self.formats:
            with self.subTest(boundary=boundary):
                layout = BSS_LAYOUT.replace(self.boundary, boundary)
                blockers, owner = bss_carve_blockers(
                    layout.splitlines(), self.ranges, 0x800411C0, 0x800411E0, None)
                self.assertTrue(blockers)
                self.assertEqual((owner.name, owner.next_vram), ("resident", 0x800411D0))
                with self.assertRaises(ValueError):
                    split_bss_yaml(layout, "resident", "other", 0x800411C0, 0x800411E0, RESIDENT_BSS_END)

    def test_carve_abutting_formatted_boundary_preserves_both_owners(self) -> None:
        expected = yaml.safe_load(BSS_LAYOUT)
        expected["segments"][0]["subsegments"].insert(
            2, {"type": ".bss", "vram": 0x800411C0, "name": "units/resident/other"})
        for boundary in self.formats:
            with self.subTest(boundary=boundary):
                layout = BSS_LAYOUT.replace(self.boundary, boundary)
                result = split_bss_yaml(layout, "resident", "other", 0x800411C0, 0x800411D0, RESIDENT_BSS_END)
                self.assertEqual(yaml.safe_load(result), expected)

    def test_multiline_asm_owner_is_preserved_or_replaced_as_one_mapping(self) -> None:
        original = "      - {type: bss, vram: 0x80039000, name: resident}"
        owners = (
            "      - name: resident\n        type: bss # asm owner\n        vram: 0x80039000",
            "      - {vram: 0x80039000,\n         name: resident,\n         type: bss} # asm owner",
        )
        for owner in owners:
            for start, end in ((0x80039000, 0x80039020), (0x8003D9E0, 0x8003EBB0)):
                with self.subTest(owner=owner, start=start):
                    layout = BSS_LAYOUT.replace(original, owner)
                    expected = yaml.safe_load(BSS_LAYOUT)
                    additions = [
                        {"type": ".bss", "vram": start, "name": "units/resident/other"},
                        {"type": "bss", "vram": end, "name": f"resident_tail_{end & 0xFFFFFF:x}"},
                    ]
                    subsegments = expected["segments"][0]["subsegments"]
                    if start == 0x80039000:
                        subsegments[1:2] = additions
                    else:
                        subsegments[2:2] = additions
                    result = split_bss_yaml(layout, "resident", "other", start, end, RESIDENT_BSS_END)
                    self.assertEqual(yaml.safe_load(result), expected)

    def test_reordered_parent_keys_preserve_same_segment_boundaries(self) -> None:
        layout = BSS_LAYOUT.replace("  - name: main\n    type: code", "  - type: code\n    name: main # resident image")
        blockers, owner = bss_carve_blockers(layout.splitlines(), self.ranges, 0x800412E0, RESIDENT_BSS_END, None)
        self.assertEqual(blockers, [])
        self.assertEqual((owner.segment, owner.next_vram), ("main", None))

    def test_unrecognized_bss_boundaries_fail_closed_in_matcher_and_adopter(self) -> None:
        unsupported = (
            "      - [0x800411D0, .bss, units/resident/contpfs]",
            "      - {type: .bss, name: units/resident/contpfs}",
            "      - {type: .bss, vram: 0x800411D0, name: units/resident/contpfs, align: 16}",
            "      - {type: .bss, vram: 0x800411D0, vram: 0x80041200, name: units/resident/contpfs}",
            "      - type: group\n        subsegments:\n          - {type: .bss, vram: 0x800411D0, name: units/resident/contpfs}",
            "      - {<<: {type: .bss, vram: 0x800411D0, name: units/resident/contpfs}}",
            "      - &owner {type: .bss, vram: 0x800411D0, name: units/resident/contpfs}\n      - *owner",
            "      - {type: .sbss, vram: 0x800411D0, name: units/resident/contpfs}",
        )
        for boundary in unsupported:
            with self.subTest(boundary=boundary):
                layout = BSS_LAYOUT.replace(self.boundary, boundary)
                blockers, owner = bss_carve_blockers(
                    layout.splitlines(), self.ranges, 0x8003D9E0, 0x8003EBB0, None)
                self.assertTrue(blockers)
                self.assertIsNone(owner)
                with self.assertRaises(ValueError):
                    split_bss_yaml(layout, "resident", "other", 0x8003D9E0, 0x8003EBB0, RESIDENT_BSS_END)


class BssReplacementAddressTests(unittest.TestCase):
    start, end = 0x8003D9E0, 0x8003DA00
    inner = 0x8003D9F0

    def setUp(self) -> None:
        temporary = tempfile.TemporaryDirectory()
        self.addCleanup(temporary.cleanup)
        self.root, self.build = Path(temporary.name) / "tree", Path(temporary.name) / "build"
        self.source = self.root / "src/units/resident/func_A.c"
        self.consumer = self.build / "generated/asm/func_B.s"
        files = {
            self.root / "config/shiren2.jp.yaml": BSS_LAYOUT,
            self.source: "extern int D_base;\n",
            self.build / "generated/asm/data/resident.bss.s": "dlabel D_base\ndlabel D_inner\n",
            self.build / "generated/asm/func_A.s": "glabel func_A\n  lui $a0, %hi(D_base)\nendlabel func_A\n",
            self.consumer: "glabel func_B\n  lui $a0, %hi(D_inner)\nendlabel func_B\n",
        }
        for path, text in files.items():
            path.parent.mkdir(parents=True, exist_ok=True)
            path.write_text(text)
        self.section = Section(1, ".bss", SHT_NOBITS, 3, self.start, 0, self.end - self.start, 0, 0, 16)
        self.base = Symbol("D_base", self.start, 4, 1, 1, self.section.index)
        self.known = {"D_base": self.start, "D_inner": self.inner}
        self.image = SimpleNamespace(bss_ranges=[SimpleNamespace(vram_start=0x80039000, vram_end=RESIDENT_BSS_END)])
        self.result = SimpleNamespace(symbols=["func_A"], data_sections=[{"section": ".bss"}])

    def plan(self, inner: Symbol, *, segment: str = "main") -> dict:
        linked = SimpleNamespace(symbols=[self.base, inner], section=lambda name: self.section if name == ".bss" else None)
        with patch("match.PROJECT", self.root):
            return _plan_bss_carve(self.result, linked, self.image, self.build, self.source, self.known, [],
                                   True, self.start, segment, 16, True, self.root)

    def test_unused_global_moving_inside_matching_extent_blocks_replacement(self) -> None:
        exact = self.plan(Symbol("D_inner", self.inner, 4, 1, 1, self.section.index))
        moved = self.plan(Symbol("D_inner", self.inner + 4, 4, 1, 1, self.section.index))
        self.assertEqual(exact["blockers"], [])
        for field in ("vram", "end", "tail", "global_symbols", "replaced_labels"):
            self.assertEqual(moved[field], exact[field])
        self.assertTrue(moved["blockers"])
        self.assertTrue(any("0x8003D9F4" in reason and "0x8003D9F0" in reason for reason in moved["blockers"]))
        self.assertTrue(any(str(self.consumer) in reason and "D_inner" in reason for reason in moved["blockers"]))

    def test_moved_global_is_rejected_even_without_an_external_consumer(self) -> None:
        self.consumer.write_text("glabel func_B\nendlabel func_B\n")
        carve = self.plan(Symbol("D_inner", self.inner + 4, 4, 1, 1, self.section.index))
        self.assertTrue(carve["blockers"])
        self.assertTrue(any("D_inner" in reason and "accepted address" in reason for reason in carve["blockers"]))

    def test_global_without_an_accepted_address_is_rejected(self) -> None:
        del self.known["D_inner"]
        carve = self.plan(Symbol("D_inner", self.inner, 4, 1, 1, self.section.index))
        self.assertTrue(any("D_inner" in reason and "no accepted address" in reason for reason in carve["blockers"]))

    def test_local_definition_does_not_exempt_an_external_reference(self) -> None:
        carve = self.plan(Symbol("D_inner", self.inner, 4, 0, 1, self.section.index))
        self.assertTrue(any(str(self.consumer) in reason and "D_inner" in reason for reason in carve["blockers"]))

    def test_absolute_alias_does_not_exempt_external_or_self_references(self) -> None:
        for address in (self.inner, self.inner + 4):
            with self.subTest(address=address):
                carve = self.plan(Symbol("D_inner", address, 0, 1, 0, SHN_ABS))
                self.assertTrue(any(str(self.consumer) in reason and "D_inner" in reason for reason in carve["blockers"]))
                self.assertTrue(any("its own carve" in reason and "D_inner" in reason for reason in carve["blockers"]))

    def test_bss_owner_must_belong_to_the_text_segment(self) -> None:
        carve = self.plan(Symbol("D_inner", self.inner, 4, 1, 1, self.section.index), segment="main_14400")
        self.assertTrue(any("segment main, not" in reason for reason in carve["blockers"]))

    def test_off_grid_initialized_data_blocks_a_plan_for_the_current_parent(self) -> None:
        # Its adoption would change the parent SUBALIGN this plan was computed with.
        inner = Symbol("D_inner", self.inner, 4, 1, 1, self.section.index)
        self.result.data_sections = [{"section": ".data", "rom_start": 0x10C8F1, "size": 1}, {"section": ".bss"}]
        self.assertTrue(any("not SUBALIGN(16)-aligned" in reason and "rematch" in reason for reason in self.plan(inner)["blockers"]))
        self.result.data_sections = [{"section": ".data", "rom_start": 0x10C8F0, "size": 0x10}, {"section": ".bss"}]
        self.assertEqual(self.plan(inner)["blockers"], [])


class BssReferenceTests(unittest.TestCase):
    def test_only_references_outside_the_unit_count(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            build, root = Path(temporary) / "build", Path(temporary) / "tree"
            files = {
                build / "generated/asm/nonmatchings/func_A.s": "jlabel func_A\n  lui $a0, %hi(D_1)\nendlabel func_A\n",
                build / "generated/asm/func_B.s": "glabel func_B\n  lui $a0, %hi(D_2)\n  lui $a0, %hi(D_10)\nendlabel func_B\n",
                build / "generated/asm/data/resident.bss.s": "dlabel D_1\ndlabel D_2\ndlabel D_3\n",
                build / "generated/asm/matchings/func_C.s": "glabel func_C\n  lui $a0, %hi(D_1)\nendlabel func_C\n",
                root / "src/units/resident/func_A.c": "extern int D_3;\n",
                root / "include/common.h": "/* nothing */\n",
                root / "config/symbol_aliases.ld": "PROVIDE(alias = D_3); /* D_1 */\nPROVIDE(D_1 = 0x8003DA00);\n",
            }
            for path, text in files.items():
                path.parent.mkdir(parents=True, exist_ok=True)
                path.write_text(text)
            found = external_bss_references(build, root, {"D_1", "D_2", "D_3"}, ["func_A"], root / "src/units/resident/func_A.c")
        self.assertEqual(found, {"D_2": [str(build / "generated/asm/func_B.s")], "D_3": [str(root / "config/symbol_aliases.ld")]})

    def test_cpp_units_are_references_outside_the_unit(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            build, root = Path(temporary) / "build", Path(temporary) / "tree"
            consumer = root / "src/units/main_14400/func_B.cpp"
            for path, text in {build / "generated/asm/data/main_14400.bss.s": "dlabel D_1\n",
                               consumer: 'extern "C" int D_1;\n',
                               root / "src/units/main_14400/func_A.c": "int D_1;\n"}.items():
                path.parent.mkdir(parents=True, exist_ok=True)
                path.write_text(text)
            found = external_bss_references(build, root, {"D_1"}, ["func_A"], root / "src/units/main_14400/func_A.c")
        self.assertEqual(found, {"D_1": [str(consumer)]})


class InteriorAliasTests(unittest.TestCase):
    def test_uninitialized_records_get_no_interior_aliases(self) -> None:
        known = {"D_80037004": 0x80037004, "D_8003D9F0": 0x8003D9F0, "D_8003DA00": 0x8003DA00}
        with tempfile.TemporaryDirectory() as temporary:
            (Path(temporary) / "candidate.o").write_bytes(b"")
            result = SimpleNamespace(out_dir=temporary, data_sections=[
                {"section": ".data", "vram": 0x80037000, "size": 0x10},
                {"section": ".bss", "vram": 0x8003D9E0, "size": 0x20},
                {"section": "COMMON", "vram": 0x8003DA00, "size": 4}])
            with patch("adopt.accepted_build", return_value=Path(temporary)), patch("adopt.symbol_addresses", return_value=known), \
                    patch("adopt.Elf", return_value=SimpleNamespace(symbols=[])):
                aliases = interior_aliases(result, "func_X")
        self.assertEqual(aliases, "PROVIDE(D_80037004 = 0x80037004); /* inside func_X .data */\n")


DATA_LAYOUT = """options: {}
segments:
  - name: main_14400
    type: code
    start: 0x14400
    vram: 0x800413C0
    subalign: 4
    bss_size: 0x844F0
    subsegments:
      - [0x14400, c, game_tick]
      - [0x14420, asm, main_tail]
      - [0x10BB10, data, main_data]
      - [0x10C8F4, .data, units/main_14400/neighbor]
      - [0x10C8F8, data, main_data_tail]
      - [0x11D770, rodata, main_rodata]
      - {type: bss, vram: 0x801609B0, name: main_14400}
  - {name: overlay, type: bin, start: 0x1339F0}
  - [0x2000000]
"""


class SplitDataTests(unittest.TestCase):
    def carve(self, symbol: str, section: str, start: int, size: int, text: str = DATA_LAYOUT) -> tuple[str, list]:
        preserved = []
        with patch("adopt._assert_main_bss_unchanged"):
            result = split_data_yaml(text, "main_14400", symbol, section, start, size, preserved=preserved)
        return result, preserved

    def test_recorded_failures_keep_exact_c_extent_and_aligned_asm_tails(self) -> None:
        cases = [
            ("func_801169D0", ".data", 0x11B724, 1),
            ("func_800C94E8", ".data", 0x11A6BC, 2),
            ("func_800A8114", ".rodata", 0x126618, 1),
            ("func_80100160", ".rodata", 0x12E164, 3),
            ("func_800D4838", ".rodata", 0x127848, 20),
            ("func_80067830", ".data", 0x10F4D0, 1),
            ("func_800CB408", ".rodata", 0x12728C, 8),
            ("func_8005483C", ".data", 0x10C8F1, 1),
            ("func_8005484C", ".data", 0x10C8F2, 1),
            ("func_80053808", ".data", 0x10C8E0, 1),
            ("func_80083008", ".rodata", 0x1203AC, 8),
        ]
        for symbol, section, start, size in cases:
            with self.subTest(symbol=symbol):
                result, preserved = self.carve(symbol, section, start, size)
                parts = yaml_data_parts(result)
                owned = next(part for part in parts if part.name.endswith("/" + symbol))
                self.assertEqual((owned.start, owned.end, owned.kind), (start, start + size, section))
                self.assertTrue(preserved)
                for part in parts:
                    if part.kind == section[1:] and not part.preserved and part.start >= start + size:
                        self.assertEqual(part.start % (8 if section == ".rodata" else 4), 0)
                before = yaml.safe_load(DATA_LAYOUT)["segments"][0]["subsegments"][:2]
                after = yaml.safe_load(result)["segments"][0]["subsegments"][:2]
                self.assertEqual(before, after)

    def test_adjacent_byte_owners_in_either_order_preserve_nonzero_prefix(self) -> None:
        for order in ((0x10C8F1, 0x10C8F2), (0x10C8F2, 0x10C8F1)):
            with self.subTest(order=order):
                layout = DATA_LAYOUT
                for start in order:
                    layout, _ = self.carve(f"byte_{start:x}", ".data", start, 1, layout)
                parts = [part for part in yaml_data_parts(layout) if 0x10C8F0 <= part.start <= 0x10C8F4]
                self.assertEqual([(part.start, part.end, part.preserved) for part in parts],
                                 [(0x10C8F0, 0x10C8F1, True), (0x10C8F1, 0x10C8F2, False),
                                  (0x10C8F2, 0x10C8F3, False), (0x10C8F3, 0x10C8F4, True),
                                  (0x10C8F4, 0x10C8F8, False)])
                self.assertIn("[0x10C8F4, .data, units/main_14400/neighbor]", layout)
                self.assertEqual(yaml.safe_load(layout)["segments"][0]["subalign"], 1)

    def test_aligned_carve_leaves_alignment_policy_unchanged(self) -> None:
        result, preserved = self.carve("aligned", ".data", 0x10C8E0, 4)
        self.assertEqual(preserved, [])
        self.assertEqual(yaml.safe_load(result)["segments"][0]["subalign"], 4)

    def test_rodata_four_byte_bridge_does_not_require_byte_subalign(self) -> None:
        result, _ = self.carve("rodata", ".rodata", 0x127848, 20)
        bridge = next(part for part in yaml_data_parts(result) if part.preserved)
        self.assertEqual((bridge.start, bridge.end, bridge.kind), (0x12785C, 0x127860, "rodata"))
        self.assertEqual(yaml.safe_load(result)["segments"][0]["subalign"], 4)
        self.assertIn("linker_section_order: .rodata", result)

    def test_existing_c_owner_and_section_crossings_remain_rejected(self) -> None:
        for section, start, size in ((".data", 0x10C8F4, 1), (".data", 0x10C8F2, 4),
                                     (".data", 0x11D770, 1), (".rodata", 0x1339E8, 16)):
            with self.subTest(section=section, start=start), self.assertRaises(ValueError):
                self.carve("overlap", section, start, size)

    def test_data_owner_stops_at_bss_and_ignores_opaque_bins(self) -> None:
        parts = yaml_data_parts(DATA_LAYOUT)
        self.assertEqual(parts[-1].end, 0x1339F0)
        self.assertFalse(any(part.start <= 0x1339F0 < part.end for part in parts))
        text = DATA_LAYOUT.replace("[0x10BB10, data, main_data]",
                                   "{start: 0x10BB10, type: bin, name: rsp, linker_section_order: .data}")
        self.assertFalse(any(part.start <= 0x10C8E0 < part.end for part in yaml_data_parts(text)))

    def test_renamed_or_resectioned_binary_is_not_carveable(self) -> None:
        text, _ = self.carve("byte", ".data", 0x10C8F1, 1)
        for altered in (text.replace("main_14400_data_bytes_10c8f2", "opaque"),
                        text.replace("linker_section_order: .data", "linker_section_order: .text"),
                        text.replace("linker_section_order: .data", "linker_section_order: .data, linker_section: .text")):
            with self.subTest(altered=altered), self.assertRaises(ValueError):
                self.carve("neighbor", ".data", 0x10C8F2, 1, altered)

    def test_multiline_owner_does_not_consume_neighbor_or_bss(self) -> None:
        text = DATA_LAYOUT.replace("- [0x11D770, rodata, main_rodata]",
                                   "- start: 0x11D770\n        type: rodata\n        name: main_rodata")
        result, _ = self.carve("last", ".rodata", 0x1339E8, 3, text)
        parts = yaml_data_parts(result)
        self.assertEqual(parts[-1].end, 0x1339F0)
        self.assertIn("{type: bss, vram: 0x801609B0, name: main_14400}", result)
        self.assertIn("{name: overlay, type: bin, start: 0x1339F0}", result)

    def test_unaligned_text_forbids_parent_transition(self) -> None:
        text = DATA_LAYOUT.replace("[0x14420, asm", "[0x14421, asm")
        with self.assertRaisesRegex(ValueError, "word-aligned text"):
            self.carve("byte", ".data", 0x10C8F1, 1, text)

    def test_existing_bss_carve_forbids_parent_transition(self) -> None:
        text = DATA_LAYOUT.replace("type: bss,", "type: .bss,")
        with self.assertRaisesRegex(ValueError, "precede C BSS"):
            split_data_yaml(text, "main_14400", "byte", ".data", 0x10C8F1, 1)

    def test_bridge_owner_is_available_to_matcher_but_next_c_remains_owned(self) -> None:
        layout, _ = self.carve("byte", ".data", 0x10C8F1, 1)
        with tempfile.TemporaryDirectory() as temporary:
            project = Path(temporary)
            (project / "config").mkdir()
            (project / "config/shiren2.jp.yaml").write_text(layout)
            with patch("match.PROJECT", project):
                self.assertEqual(data_owner(0x10C8F2), ("data", "main_14400_data_bytes_10c8f2"))
                self.assertEqual(data_owner(0x10C8F4), (".data", "units/main_14400/neighbor"))
                self.assertIsNone(data_owner(0x1339F0))

    def test_neighbor_labels_remain_absolute_and_half_open(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            (Path(temporary) / "candidate.o").write_bytes(b"fixture")
            result = SimpleNamespace(out_dir=temporary, data_sections=[
                {"vram": 0x801398B1, "size": 1, "section": ".data"}])
            known = {f"D_{address:X}": address for address in range(0x801398B0, 0x801398B5)}
            elf = SimpleNamespace(symbols=[SimpleNamespace(name="D_801398B1", shndx=1)])
            with patch("adopt.symbol_addresses", return_value=known), patch("adopt.accepted_build", return_value=Path(temporary)), patch("adopt.Elf", return_value=elf):
                aliases = interior_aliases(result, "byte", preserved=[(0x801398B0, 0x801398B1), (0x801398B2, 0x801398B4)])
                for address in (0x801398B0, 0x801398B2, 0x801398B3):
                    self.assertIn(f"PROVIDE(D_{address:X} = 0x{address:08X});", aliases)
                self.assertNotIn("D_801398B1", aliases)
                self.assertNotIn("D_801398B4", aliases)
                elf.symbols.append(SimpleNamespace(name="D_801398B2", shndx=1))
                with self.assertRaisesRegex(ValueError, "neighboring preserved-data label"):
                    interior_aliases(result, "byte", preserved=[(0x801398B2, 0x801398B4)])

    def test_custom_data_linker_options_are_not_silently_discarded(self) -> None:
        text = DATA_LAYOUT.replace("[0x10BB10, data, main_data]",
                                   "{start: 0x10BB10, type: data, name: main_data, linker_section: .rodata}")
        with self.assertRaisesRegex(ValueError, "explicit layout review"):
            self.carve("byte", ".data", 0x10C8F1, 1, text)

    def test_implicit_common_and_noncanonical_bss_inputs_block_transition(self) -> None:
        owner = yaml_data_parts(DATA_LAYOUT)[0]
        raw = SimpleNamespace(symbols=[], sections=[SimpleNamespace(type=SHT_NOBITS, size=0x844F0, name=".bss")])
        empty = SimpleNamespace(symbols=[], sections=[])
        common = SimpleNamespace(symbols=[SimpleNamespace(shndx=SHN_COMMON)], sections=[])
        stray = SimpleNamespace(symbols=[], sections=[SimpleNamespace(type=SHT_NOBITS, size=1, name=".sbss")])
        with tempfile.TemporaryDirectory() as temporary:
            build = Path(temporary)
            graph = {
                "raw.o": {"image_id": "main_14400", "source_kind": "asm", "input": "generated/asm/data/main_14400.bss.s"},
                "unit.o": {"image_id": "main_14400", "source_kind": "c", "input": "source/src/unit.c"},
            }
            (build / "receipt.json").write_text(json.dumps({"graph": graph}))
            for name in graph:
                (build / name).write_bytes(name.encode())
            with patch("adopt.accepted_build", return_value=build):
                with patch("adopt.Elf", side_effect=[raw, empty]):
                    _assert_main_bss_unchanged(owner)
                for other, message in ((common, "COMMON"), (stray, "BSS")):
                    with patch("adopt.Elf", side_effect=[raw, other]), self.assertRaisesRegex(ValueError, message):
                        _assert_main_bss_unchanged(owner)

    def test_flow_mapping_owner_is_replaced_through_its_closing_brace(self) -> None:
        # DT-R1: a separate closing-brace line belongs to the replaced owner.
        owners = (("- [0x10BB10, data, main_data]", "- {start: 0x10BB10,\n        type: data,\n        name: main_data\n        }",
                   ((".data", 0x10C8F1, 1), (".data", 0x10BB10, 1))),
                  ("- [0x11D770, rodata, main_rodata]", "- {start: 0x11D770, type: rodata,\n        name: main_rodata\n      } # last",
                   ((".rodata", 0x1339E8, 3),)))
        for original, flow, carves in owners:
            text = DATA_LAYOUT.replace(original, flow)
            self.assertEqual([(part.start, part.end, part.kind, part.name) for part in yaml_data_parts(text)],
                             [(part.start, part.end, part.kind, part.name) for part in yaml_data_parts(DATA_LAYOUT)])
            for section, start, size in carves:
                with self.subTest(flow=flow, start=start):
                    result, preserved = self.carve("byte", section, start, size, text)
                    expected, expected_preserved = self.carve("byte", section, start, size)
                    self.assertEqual(yaml.safe_load(result), yaml.safe_load(expected))
                    self.assertEqual(preserved, expected_preserved)
                    self.assertIn("      - [0x10C8F4, .data, units/main_14400/neighbor]\n", result)
                    self.assertIn("      - {type: bss, vram: 0x801609B0, name: main_14400}\n", result)
                    self.assertEqual(yaml_data_parts(result)[-1].end, 0x1339F0)

    def test_cpp_text_intervals_are_checked_before_parent_transition(self) -> None:
        text = DATA_LAYOUT.replace("[0x14420, asm, main_tail]", "[0x14420, cpp, units/main_14400/cxx]\n      - [0x14422, bin, opaque]")
        with self.assertRaisesRegex(ValueError, "word-aligned text"):
            self.carve("byte", ".data", 0x10C8F1, 1, text)

    def test_parent_subalign_must_be_one_editable_field(self) -> None:
        for replacement in ("    subalign:\n      4\n", "    subalign: 8\n    subalign: 4\n"):
            text = DATA_LAYOUT.replace("    subalign: 4\n", replacement)
            with self.subTest(replacement=replacement), self.assertRaisesRegex(ValueError, "single-line block-mapping field"):
                self.carve("byte", ".data", 0x10C8F1, 1, text)


class PinnedSplatDataBridgeTests(unittest.TestCase):
    def test_bin_copies_exact_unaligned_bytes_and_links_in_rodata_order(self) -> None:
        from splat.segtypes.common.code import CommonSegCode
        from splat.segtypes.linker_entry import LinkerWriter
        from splat.segtypes.segment import Segment
        from splat.util import options

        with tempfile.TemporaryDirectory() as temporary, patch.object(options, "opts", create=True):
            base = Path(temporary)
            config = {"options": {"basename": "bridge", "platform": "n64", "compiler": "KMC",
                                   "base_path": str(base), "target_path": "input.bin", "o_as_suffix": True}}
            options.initialize(config, [base / "config.yaml"])
            spec = {"name": "main_14400", "type": "code", "start": 0, "vram": 0x80000000,
                    "subalign": 1, "subsegments": [
                        [0, "rodata", "prefix"],
                        {"start": 1, "type": "bin", "name": "main_14400_rodata_bytes_1", "linker_section_order": ".rodata"},
                        [4, "rodata", "suffix"],
                    ]}
            parent = Segment.from_yaml(CommonSegCode, spec, 0, 8, None)
            bridge = parent.subsegments[1]
            bridge.split(bytes.fromhex("aa112233bb445566"))
            self.assertEqual(bridge.out_path().read_bytes(), bytes.fromhex("112233"))
            entry = bridge.get_linker_entries()[0]
            self.assertEqual((entry.section_order, entry.section_link), (".rodata", ".data"))
            writer = LinkerWriter()
            writer.add(parent, [])
            emitted = "\n".join(writer.buffer)
            self.assertIn("SUBALIGN(1)", emitted)
            self.assertIn("main_14400_rodata_bytes_1.o(.data)", emitted)
            self.assertLess(emitted.index("prefix.rodata.o"), emitted.index("main_14400_rodata_bytes_1.o"))
            self.assertLess(emitted.index("main_14400_rodata_bytes_1.o"), emitted.index("suffix.rodata.o"))


RESIDENT_LAYOUT = """options: {}
segments:
  - name: main
    type: code
    start: 0x1060
    vram: 0x80025C60
    bss_size: 0x83C0
    subsegments:
      - [0x1060, asm, resident]
      - [0x1280, c, units/resident/func_80025E80]
      - [0x13A90, data, resident_tail_13a90]
      - [0x13AE0, .rodata, units/resident/func_80025EE0]
      - [0x13F80, rodata, resident_tail_13f80]
      - {type: bss, vram: 0x80039000, name: resident}
  - name: main_14400
    type: code
    start: 0x14400
    vram: 0x800413C0
    subalign: 1
    subsegments:
      - [0x14400, c, game_tick]
  - [0x2000000]
"""
# The reviewed resident configuration (merge-011 H12): explicit byte placement.
BYTE_RESIDENT_LAYOUT = RESIDENT_LAYOUT.replace("    bss_size: 0x83C0\n", "    subalign: 1\n    bss_size: 0x83C0\n")


class ResidentSubalignTests(unittest.TestCase):
    """Resident `main` links at its splat default SUBALIGN(16) unless configured `subalign: 1`."""

    def test_word_ended_text_is_refused_where_the_parent_subalign_would_move_it(self) -> None:
        # B6/B7 GNU291 units: main [0x1060, 0x10C8), func_80025CC8 [0x10C8, 0x11B0), func_80025DB0 .text 0xCC.
        for rom, size, slot_end, moved in ((0x1060, 0x68, 0x10C8, 0x10C8), (0x10C8, 0xE8, 0x11B0, 0x10C8),
                                           (0x11B0, 0xCC, 0x1280, 0x127C)):
            with self.subTest(rom=rom), self.assertRaisesRegex(
                    ValueError, rf"Segment main links inputs at SUBALIGN\(16\): the input at ROM 0x{moved:X} would move"):
                assert_text_placement(RESIDENT_LAYOUT, rom, size, slot_end)
            assert_text_placement(BYTE_RESIDENT_LAYOUT, rom, size, slot_end)
        assert_text_placement(RESIDENT_LAYOUT, 0x1060, 0x70, 0x1070)  # KMC-rounded text stays on the grid
        assert_text_placement(RESIDENT_LAYOUT, 0x14400, 0x1C, 0x14420)  # main_14400 is SUBALIGN(1)
        with self.assertRaisesRegex(ValueError, "No splat code segment"):
            assert_text_placement(RESIDENT_LAYOUT, 0x2000000, 4, 0x2000004)

    def test_byte_placed_resident_text_keeps_its_padding_bin(self) -> None:
        result = split_yaml(BYTE_RESIDENT_LAYOUT, "resident", "func_80025DB0", 0x11B0, 0xCC, 0x1280)
        self.assertIn("      - [0x1060, asm, resident]\n"
                      "      - [0x11B0, c, units/resident/func_80025DB0]\n"
                      "      - {start: 0x127C, type: bin, name: func_80025DB0_padding, linker_section_order: .text}\n"
                      "      - [0x1280, c, units/resident/func_80025E80]\n", result)

    def test_joint_func_800354B0_data_ownership_under_the_reviewed_subalign(self) -> None:
        # Exact receipts: .data 0x50 at ROM 0x13A90 (spaces/zeroes), .rodata 0x178 at ROM 0x13F80.
        preserved: list[tuple[int, int]] = []
        layout = split_data_yaml(BYTE_RESIDENT_LAYOUT, "resident", "func_800354B0", ".data", 0x13A90, 0x50, preserved=preserved)
        layout = split_data_yaml(layout, "resident", "func_800354B0", ".rodata", 0x13F80, 0x178, preserved=preserved)
        parts = [(part.start, part.end, part.kind, part.name) for part in yaml_data_parts(layout) if part.start >= 0x13A90]
        self.assertEqual(parts, [(0x13A90, 0x13AE0, ".data", "units/resident/func_800354B0"),
                                 (0x13AE0, 0x13F80, ".rodata", "units/resident/func_80025EE0"),
                                 (0x13F80, 0x140F8, ".rodata", "units/resident/func_800354B0"),
                                 (0x140F8, 0x14100, "rodata", "main_rodata_bytes_140f8"),
                                 (0x14100, 0x14400, "rodata", "resident_tail_14100")])
        self.assertEqual(preserved, [(0x80038CF8, 0x80038D00)])
        self.assertEqual(yaml.safe_load(layout)["segments"][0]["subalign"], 1)
        self.assertIn("{start: 0x140F8, type: bin, name: main_rodata_bytes_140f8, linker_section_order: .rodata}", layout)

    def test_default_resident_subalign_still_refuses_byte_granular_data(self) -> None:
        with self.assertRaisesRegex(ValueError, "reviewed SUBALIGN transition of segment main "):
            split_data_yaml(RESIDENT_LAYOUT, "resident", "func_800354B0", ".rodata", 0x13F80, 0x178)

    def test_project_resident_layout_is_the_reviewed_byte_placement(self) -> None:
        layout = yaml.safe_load((Path(__file__).resolve().parents[1] / "config/shiren2.jp.yaml").read_text())
        resident = next(item for item in layout["segments"] if isinstance(item, dict) and item.get("name") == "main")
        self.assertEqual(resident.get("subalign"), 1)
        parts = resident["subsegments"]
        index = parts.index([0xF120, "c", "units/resident/func_80033D20"])
        # The only fill SUBALIGN(16) inserted (func_80033D20.o .text is 0xC in a 0x10 slot).
        self.assertEqual(parts[index + 1], {"start": 0xF12C, "type": "bin", "name": "func_80033D20_padding",
                                            "linker_section_order": ".text"})
        self.assertEqual(parts[index + 2], [0xF130, "c", "units/resident/func_80033D30"])


class DataBssInteractionTests(unittest.TestCase):
    """Byte-granular data layout precedes C BSS; a BSS plan never outlives its parent SUBALIGN."""
    plan = {"segment": "main_14400", "subalign": 4, "vram": 0x801609B0, "end": 0x801609BC, "tail": 0x801609C0,
            "range_end": 0x801E4EA0, "add_bss_contains_common": False, "blockers": []}

    def byte_layout(self) -> str:
        with patch("adopt._assert_main_bss_unchanged"):
            return split_data_yaml(DATA_LAYOUT, "main_14400", "first", ".data", 0x10C8F1, 1)

    def test_bss_plan_applies_only_under_its_parent_subalign(self) -> None:
        self.assertIn("{type: .bss, vram: 0x801609B0, name: units/main_14400/unit}",
                      carve_bss_plan(DATA_LAYOUT, "main_14400", "unit", dict(self.plan)))
        layout = self.byte_layout()
        self.assertEqual(yaml.safe_load(layout)["segments"][0]["subalign"], 1)
        with self.assertRaisesRegex(ValueError, r"SUBALIGN\(4\).*SUBALIGN\(1\).*rematch"):
            carve_bss_plan(layout, "main_14400", "unit", dict(self.plan))
        replanned = carve_bss_plan(layout, "main_14400", "unit", dict(self.plan, subalign=1, tail=0x801609BC))
        self.assertIn("      - {type: .bss, vram: 0x801609B0, name: units/main_14400/unit}\n"
                      "      - {type: bss, vram: 0x801609BC, name: main_bss_1609bc}\n", replanned)

    def test_bss_adopted_first_forbids_the_later_parent_transition(self) -> None:
        layout = carve_bss_plan(DATA_LAYOUT, "main_14400", "unit", dict(self.plan))
        with self.assertRaisesRegex(ValueError, "precede C BSS"):
            split_data_yaml(layout, "main_14400", "byte", ".data", 0x10C8F1, 1)

    def project(self, root: Path, layout: str) -> None:
        (root / "config").mkdir()
        configs = {"matches.json": {"functions": []}, "compiler_profiles.json": {"tu_profiles": {}},
                   "images.json": {"source_bindings": {}}}
        for filename, content in configs.items():
            (root / "config" / filename).write_text(json.dumps(content))
        (root / "config/shiren2.jp.yaml").write_text(layout)
        (root / "config/symbol_aliases.ld").write_text("")
        (root / "candidate.o").write_bytes(b"fixture")
        (root / "candidate.c").write_text("int func_80041400(void) { return 0; }\n")

    def adopt_unit(self, root: Path, data_start: int, plan: dict, *, final: dict | None = None) -> tuple[dict, list[str]]:
        target = SimpleNamespace(image_id="main_14400", symbol="func_80041400", rom_start=0x14440,
                                 vram_start=0x80041400, size=0x10, slot_end=0x14450)

        def matched(carve: dict) -> SimpleNamespace:
            return SimpleNamespace(status="match-with-data", problems=[], participating_files=[{"path": str(root / "candidate.c")}],
                                   text_bytes=0x10, instruction_bytes=0x10, slot_bytes=0x10, source={"sha256": "a" * 64},
                                   out_dir=str(root), bss_carve=carve,
                                   data_sections=[{"section": ".data", "size": 1, "vram": data_start + 0x8002CFC0, "rom_start": data_start},
                                                  {"section": ".bss", "size": 0xC, "vram": plan["vram"]}])
        gate, log = Mock(), []
        gate.check.return_value = []
        checks = [(matched(plan), None), (matched(final or plan), None)]
        with patch("adopt.PROJECT", root), patch("adopt.resolve_targets", return_value=[target]), \
                patch("adopt.check", side_effect=checks), patch("adopt._assert_main_bss_unchanged"), \
                patch("adopt.accepted_build", return_value=root), patch("adopt.symbol_addresses", return_value={}), \
                patch("adopt.Elf", return_value=SimpleNamespace(symbols=[])):
            return adopt(Unit([target.symbol], root / "candidate.c"), rom=bytes(0x14450), log=log,
                         gate=gate, index=0), log

    def test_single_unit_with_byte_data_and_a_parent_bound_bss_plan_is_refused_unwritten(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary).resolve()
            self.project(root, DATA_LAYOUT)
            before = {path.name: path.read_text() for path in (root / "config").iterdir()}
            with self.assertRaisesRegex(ValueError, r"computed under SUBALIGN\(4\).*then rematch"):
                self.adopt_unit(root, 0x10C8F1, dict(self.plan))
            self.assertEqual({path.name: path.read_text() for path in (root / "config").iterdir()}, before)
            self.assertFalse((root / "src").exists())

    def test_unit_replanned_under_the_effective_parent_is_adopted(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary).resolve()
            self.project(root, self.byte_layout())
            plan = dict(self.plan, subalign=1, tail=0x801609BC)
            counts, _log = self.adopt_unit(root, 0x10C8F2, plan)
            self.assertEqual(counts, {"functions": 1, "instruction_bytes": 0x10})
            layout = (root / "config/shiren2.jp.yaml").read_text()
            parts = [(part.start, part.end, part.kind, part.name) for part in yaml_data_parts(layout) if 0x10C8F0 <= part.start < 0x10C8F8]
            self.assertEqual(parts, [(0x10C8F0, 0x10C8F1, "data", "main_14400_data_bytes_10c8f0"),
                                     (0x10C8F1, 0x10C8F2, ".data", "units/main_14400/first"),
                                     (0x10C8F2, 0x10C8F3, ".data", "units/main_14400/func_80041400"),
                                     (0x10C8F3, 0x10C8F4, "data", "main_14400_data_bytes_10c8f3"),
                                     (0x10C8F4, 0x10C8F8, ".data", "units/main_14400/neighbor")])
            self.assertIn("      - {type: .bss, vram: 0x801609B0, name: units/main_14400/func_80041400}\n"
                          "      - {type: bss, vram: 0x801609BC, name: main_bss_1609bc}\n", layout)

    def test_canonical_recheck_must_reproduce_the_adopted_bss_plan(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary).resolve()
            self.project(root, self.byte_layout())
            before = {path.name: path.read_text() for path in (root / "config").iterdir()}
            plan = dict(self.plan, subalign=1, tail=0x801609BC)
            with self.assertRaisesRegex(ValueError, "canonical BSS plan"):
                self.adopt_unit(root, 0x10C8F2, plan, final=dict(plan, tail=0x801609C0))
            self.assertEqual({path.name: path.read_text() for path in (root / "config").iterdir()}, before)
            self.assertFalse((root / "src/units/main_14400/func_80041400.c").exists())

    def test_text_carve_moved_by_the_parent_subalign_is_refused_unwritten(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary).resolve()
            self.project(root, DATA_LAYOUT.replace("    subalign: 4\n", "    subalign: 32\n"))
            before = {path.name: path.read_text() for path in (root / "config").iterdir()}
            with self.assertRaisesRegex(ValueError, r"SUBALIGN\(32\): the input at ROM 0x14450 would move"):
                self.adopt_unit(root, 0x10C8F0, dict(self.plan, subalign=32))
            self.assertEqual({path.name: path.read_text() for path in (root / "config").iterdir()}, before)
            self.assertFalse((root / "src").exists())


ABSORB_LAYOUT = """options: {}
segments:
  - name: main_14400
    type: code
    start: 0x14400
    vram: 0x800413C0
    subalign: 4
    bss_size: 0x844F0
    subsegments:
      - [0x14400, c, units/main_14400/game_tick]
      - [0x14420, asm, main_14400_tail_14420]
      - [0x14500, c, units/main_14400/next_text]
      - [0x10BB10, data, main_data_10bb10]
      - [0x10C8F4, .data, units/main_14400/neighbor]
      - [0x10C8F8, data, main_data_10c8f8]
      - [0x11D770, rodata, main_rodata_11d770]
      - {type: bss, vram: 0x801609B0, name: main_14400}
  - {name: overlay, type: bin, start: 0x1339F0}
  - [0x2000000]
"""
# The first absorbed function starts its own asm subsegment, right after a C neighbour.
OWNER_START_LAYOUT = ABSORB_LAYOUT.replace("      - [0x14420, asm, main_14400_tail_14420]\n",
                                          "      - [0x14420, c, units/main_14400/prev_text]\n"
                                          "      - [0x14440, asm, main_14400_tail_14440]\n")
MAIN_DELTA = 0x8002CFC0
ABSORB_TARGETS = {symbol: SimpleNamespace(image_id="main_14400", symbol=symbol, rom_start=rom, vram_start=rom + MAIN_DELTA,
                                          size=size, slot_end=end)
                  for symbol, rom, size, end in (("func_80041400", 0x14440, 0x10, 0x14450),
                                                 ("func_80041410", 0x14450, 0x0C, 0x14460),
                                                 ("func_80041420", 0x14460, 0x10, 0x14470))}
JOINT = ["func_80041400", "func_80041410", "func_80041420"]
BASE_ALIASES = "PROVIDE(D_80139000 = 0x80139000); /* inside neighbor .data */\n"


class AbsorbTests(unittest.TestCase):
    """`absorbs`: a candidate replacing whole adopted units, through the real layout edits, gate and rollback."""

    def project(self, layout: str = ABSORB_LAYOUT) -> Path:
        temporary = tempfile.TemporaryDirectory()
        self.addCleanup(temporary.cleanup)
        root = Path(temporary.name).resolve()
        neighbour = "src/units/main_14400/next_text.c"
        files = {"config/matches.json": json.dumps({"functions": []}),
                 "config/compiler_profiles.json": json.dumps({"tu_profiles": {neighbour: "gcc281pm-gnu291-O2-unsigned"}}),
                 "config/images.json": json.dumps({"source_bindings": {neighbour: "main_14400"}}),
                 "config/shiren2.jp.yaml": layout, "config/symbol_aliases.ld": BASE_ALIASES,
                 neighbour: "int next_text(void) { return 0; }\n", "candidate.o": "fixture"}
        for name, text in files.items():
            (root / name).parent.mkdir(parents=True, exist_ok=True)
            (root / name).write_text(text)
        return root

    @staticmethod
    def snapshot(root: Path) -> dict[str, bytes]:
        return {path.relative_to(root).as_posix(): path.read_bytes()
                for top in ("config", "src") for path in sorted((root / top).rglob("*")) if path.is_file()}

    @staticmethod
    def gate(root: Path, candidate: Path) -> Gate:
        """The real interface gate over the project's canonical TUs plus the candidate."""
        sources = json.loads((root / "config/compiler_profiles.json").read_text())["tu_profiles"]
        audited = parse_tu(candidate.read_text(), str(candidate))
        audited.candidate = 0
        return Gate([*(parse_tu((root / source).read_text(), source) for source in sources), audited])

    def adopt(self, root: Path, functions: list[str], text: str, *, absorbs: tuple[str, ...] | list[str] = (),
              data: tuple[tuple[int, int], ...] = (), defined: tuple[str, ...] = (), known: dict[str, int] | None = None,
              text_bytes: int | None = None, final_status: str | None = None, candidate_status: str | None = None,
              log: list[str] | None = None) -> dict[str, int]:
        candidate = root / "candidates" / f"{functions[0]}-{len(functions)}.c"
        candidate.parent.mkdir(exist_ok=True)
        candidate.write_text(text)
        targets = [ABSORB_TARGETS[symbol] for symbol in functions]
        slot = targets[-1].slot_end - targets[0].rom_start
        sections = [{"section": ".data", "rom_start": start, "size": size, "vram": start + MAIN_DELTA} for start, size in data]
        matched = SimpleNamespace(status=candidate_status or ("match-with-data" if sections else "match"), problems=[],
                                  participating_files=[{"path": str(candidate)}], text_bytes=slot if text_bytes is None else text_bytes,
                                  instruction_bytes=sum(target.size for target in targets), slot_bytes=slot, data_sections=sections,
                                  bss_carve=None, source={"sha256": "a" * 64}, out_dir=str(root))
        final = SimpleNamespace(**{**vars(matched), "status": final_status or matched.status})
        elf = SimpleNamespace(symbols=[SimpleNamespace(name=name, shndx=1) for name in defined])
        with patch("adopt.PROJECT", root), patch("adopt.resolve_targets", side_effect=lambda symbols: [ABSORB_TARGETS[s] for s in symbols]), \
                patch("adopt.check", side_effect=[(matched, None), (final, None)]), patch("adopt._assert_main_bss_unchanged"), \
                patch("adopt.accepted_build", return_value=root), patch("adopt.symbol_addresses", return_value=known or {}), \
                patch("adopt.Elf", return_value=elf):
            return adopt(Unit(list(functions), candidate, absorbs=list(absorbs)), rom=bytes(0x14500),
                         log=[] if log is None else log, gate=self.gate(root, candidate), index=0)

    def test_absorbing_a_middle_text_only_unit_equals_direct_adoption(self) -> None:
        joint = "void func_80041400(void) {}\nvoid func_80041410(int value) {}\nvoid func_80041420(void) {}\n"
        absorbed, direct = self.project(), self.project()
        self.adopt(absorbed, ["func_80041410"], "int func_80041410(void) { return 1; }\n", text_bytes=0xC)
        self.assertIn("      - [0x14420, asm, main_14400_tail_14420]\n"
                      "      - [0x14450, c, units/main_14400/func_80041410]\n"
                      "      - {start: 0x1445C, type: bin, name: func_80041410_padding, linker_section_order: .text}\n"
                      "      - [0x14460, asm, main_14400_tail_14460]\n", (absorbed / "config/shiren2.jp.yaml").read_text())
        probe = absorbed / "probe.c"
        probe.write_text(joint)
        self.assertTrue(self.gate(absorbed, probe).check(0), "the absorbed TU's old definition must disagree with the joint")
        log: list[str] = []
        self.assertEqual(self.adopt(absorbed, JOINT, joint, absorbs=["func_80041410"], log=log),
                         {"functions": 3, "instruction_bytes": 0x2C})
        self.assertEqual(self.adopt(direct, JOINT, joint), {"functions": 3, "instruction_bytes": 0x2C})
        self.assertEqual(self.snapshot(absorbed), self.snapshot(direct))
        self.assertFalse((absorbed / "src/units/main_14400/func_80041410.c").exists())
        self.assertIn("      - [0x14420, asm, main_14400_tail_14420]\n"
                      "      - [0x14440, c, units/main_14400/func_80041400]\n"
                      "      - [0x14470, asm, main_14400_tail_14470]\n", (absorbed / "config/shiren2.jp.yaml").read_text())
        self.assertIn("   absorb src/units/main_14400/func_80041410.c (func_80041410): rows, TU profile, image binding, "
                      "layout and aliases retired; source removed", log)

    def test_absorbing_a_first_unit_with_owned_data_and_an_interior_alias_equals_direct_adoption(self) -> None:
        known = {"D_801398A0": 0x801398A0, "D_801398A4": 0x801398A4}
        joint = ("int D_801398A0 = 0;\nint D_801398A4 = 0;\nvoid func_80041400(void) { D_801398A0 = 1; }\n"
                 "void func_80041410(void) { D_801398A4 = 1; }\n")
        absorbed, direct = self.project(OWNER_START_LAYOUT), self.project(OWNER_START_LAYOUT)
        self.adopt(absorbed, ["func_80041400"], "int D_801398A0[2] = {0};\nvoid func_80041400(void) { D_801398A0[0] = 1; }\n",
                   data=((0x10C8E0, 8),), defined=("D_801398A0",), known=known)
        layout = (absorbed / "config/shiren2.jp.yaml").read_text()
        self.assertIn("      - [0x14420, c, units/main_14400/prev_text]\n"
                      "      - [0x14440, c, units/main_14400/func_80041400]\n"
                      "      - [0x14450, asm, main_14400_tail_14450]\n", layout)
        self.assertIn("      - [0x10BB10, data, main_data_10bb10]\n"
                      "      - [0x10C8E0, .data, units/main_14400/func_80041400]\n"
                      "      - [0x10C8E8, data, main_data_10c8e8]\n", layout)
        self.assertEqual((absorbed / "config/symbol_aliases.ld").read_text(),
                         BASE_ALIASES + "PROVIDE(D_801398A4 = 0x801398A4); /* inside func_80041400 .data */\n")
        for root, absorbs in ((absorbed, ["func_80041400"]), (direct, [])):
            self.adopt(root, JOINT[:2], joint, absorbs=absorbs, data=((0x10C8E0, 8),), defined=("D_801398A0", "D_801398A4"),
                       known=known, text_bytes=0x1C)
        self.assertEqual(self.snapshot(absorbed), self.snapshot(direct))
        self.assertEqual((absorbed / "config/symbol_aliases.ld").read_text(), BASE_ALIASES)  # the stale interior alias is gone
        self.assertEqual((absorbed / "src/units/main_14400/func_80041400.c").read_text(), joint)  # reused path, joint source
        rows = json.loads((absorbed / "config/matches.json").read_text())["functions"]
        self.assertEqual([row["symbol"] for row in rows], JOINT[:2])
        self.assertIn("      - [0x14440, c, units/main_14400/func_80041400]\n"
                      "      - {start: 0x1445C, type: bin, name: func_80041400_padding, linker_section_order: .text}\n"
                      "      - [0x14460, asm, main_14400_tail_14460]\n", (absorbed / "config/shiren2.jp.yaml").read_text())

    def test_absorbing_byte_granular_data_with_bridges_equals_direct_adoption(self) -> None:
        known = {f"D_{address:X}": address for address in range(0x801398B0, 0x801398B4)}
        joint = "char D_801398B1 = 1;\nvoid func_80041400(void) {}\nvoid func_80041410(void) { D_801398B1 = 2; }\nvoid func_80041420(void) {}\n"
        absorbed, direct = self.project(), self.project()
        self.adopt(absorbed, ["func_80041410"], "char D_801398B1 = 1;\nvoid func_80041410(void) { D_801398B1 = 2; }\n",
                   data=((0x10C8F1, 1),), defined=("D_801398B1",), known=known, text_bytes=0xC)
        self.assertIn("/* preserved bytes beside func_80041410, not C */", (absorbed / "config/symbol_aliases.ld").read_text())
        self.assertIn("{start: 0x10C8F0, type: bin, name: main_14400_data_bytes_10c8f0, linker_section_order: .data}",
                      (absorbed / "config/shiren2.jp.yaml").read_text())
        for root, absorbs in ((absorbed, ["func_80041410"]), (direct, [])):
            self.adopt(root, JOINT, joint, absorbs=absorbs, data=((0x10C8F1, 1),), defined=("D_801398B1",), known=known)
        self.assertEqual(self.snapshot(absorbed), self.snapshot(direct))
        aliases = (absorbed / "config/symbol_aliases.ld").read_text()
        self.assertNotIn("func_80041410", aliases)
        self.assertEqual(aliases.count("/* preserved bytes beside func_80041400, not C */"), 3)

    def test_recarve_then_claim_released_bridge_equals_two_direct_adoptions(self) -> None:
        # B5's 0x10C818 byte is released from the old eight-byte owner by a seven-byte re-carve.
        known = {"D_801397D8": 0x801397D8, "D_801397D9": 0x801397D9}
        old = "char D_801397D8 = 1;\nchar D_801397D9[7] = {80, 6};\nvoid func_80041410(void) {}\n"
        recarved = "extern char D_801397D8;\nchar D_801397D9[7] = {80, 6};\nvoid func_80041410(void) {}\n"
        released = "char D_801397D8 = 1;\nvoid func_80041400(void) {}\n"
        absorbed, direct = self.project(), self.project()
        self.adopt(absorbed, [JOINT[1]], old, data=((0x10C818, 8),), defined=tuple(known), known=known)
        for root, absorbs in ((absorbed, [JOINT[1]]), (direct, [])):
            self.adopt(root, [JOINT[1]], recarved, absorbs=absorbs, data=((0x10C819, 7),),
                       defined=("D_801397D9",), known=known)
            bridge = next(part for part in yaml_data_parts((root / "config/shiren2.jp.yaml").read_text())
                          if part.start == 0x10C818)
            self.assertEqual((bridge.end, bridge.kind, bridge.preserved), (0x10C819, "data", True))
            self.adopt(root, [JOINT[0]], released, data=((0x10C818, 1),), defined=("D_801397D8",), known=known)
            owners = [(part.start, part.end, part.kind, part.name) for part in
                      yaml_data_parts((root / "config/shiren2.jp.yaml").read_text()) if 0x10C818 <= part.start < 0x10C820]
            self.assertEqual(owners, [(0x10C818, 0x10C819, ".data", "units/main_14400/" + JOINT[0]),
                                      (0x10C819, 0x10C820, ".data", "units/main_14400/" + JOINT[1])])
        self.assertEqual(self.snapshot(absorbed), self.snapshot(direct))

    def test_bridge_claim_keeps_exact_byte_gates_and_rollback(self) -> None:
        root = self.project()
        self.adopt(root, [JOINT[1]], "char bytes[7] = {80, 6};\nvoid func_80041410(void) {}\n",
                   data=((0x10C819, 7),))
        before = self.snapshot(root)
        for failed_check in ("candidate_status", "final_status"):
            with self.subTest(failed_check=failed_check), self.assertRaisesRegex(ValueError, "mismatch-data"):
                self.adopt(root, [JOINT[0]], "char released = 2;\nvoid func_80041400(void) {}\n",
                           data=((0x10C818, 1),), **{failed_check: "mismatch-data"})
            self.assertEqual(self.snapshot(root), before)

    def test_claiming_or_crossing_another_c_owner_is_refused_unwritten(self) -> None:
        root = self.project()
        self.adopt(root, [JOINT[1]], "char bytes[7] = {80, 6};\nvoid func_80041410(void) {}\n",
                   data=((0x10C819, 7),))
        before = self.snapshot(root)
        for start, size in ((0x10C819, 1), (0x10C818, 2)):
            with self.subTest(start=start, size=size), self.assertRaises(ValueError):
                self.adopt(root, [JOINT[0]], "void func_80041400(void) {}\n", data=((start, size),))
            self.assertEqual(self.snapshot(root), before)

    def test_partially_contained_unit_is_refused_unwritten(self) -> None:
        root = self.project()
        self.adopt(root, JOINT[1:], "void func_80041410(void) {}\nvoid func_80041420(void) {}\n")
        before = self.snapshot(root)
        with self.assertRaisesRegex(ValueError, r"also owns \['func_80041420'\]; the candidate must list every function"):
            self.adopt(root, JOINT[:2], "void func_80041400(void) {}\nvoid func_80041410(void) {}\n", absorbs=["func_80041410"])
        self.assertEqual(self.snapshot(root), before)

    def test_unlisted_overlap_and_unknown_symbols_are_refused_unwritten(self) -> None:
        root = self.project()
        self.adopt(root, ["func_80041400"], "void func_80041400(void) {}\n")
        self.adopt(root, ["func_80041420"], "void func_80041420(void) {}\n")
        before = self.snapshot(root)
        joint = "void func_80041400(void) {}\nvoid func_80041410(void) {}\nvoid func_80041420(void) {}\n"
        for absorbs, message in (([], r"Already matched: \['func_80041400', 'func_80041420'\]"),
                                 (["func_80041400"], r"Already matched: \['func_80041420'\]"),
                                 (["func_80041410"], r"Cannot absorb func_80041410: 0 adopted main_14400 rows")):
            with self.subTest(absorbs=absorbs), self.assertRaisesRegex(ValueError, message):
                self.adopt(root, JOINT, joint, absorbs=absorbs)
            self.assertEqual(self.snapshot(root), before)

    def failing_setup(self) -> tuple[Path, str, dict[str, int]]:
        known = {"D_801398A0": 0x801398A0, "D_801398A4": 0x801398A4}
        root = self.project()
        self.adopt(root, ["func_80041410"], "int D_801398A0[2];\nvoid func_80041410(void) {}\n", data=((0x10C8E0, 8),),
                   defined=("D_801398A0",), known=known, text_bytes=0xC)
        return root, "int D_801398A0[2];\nvoid func_80041400(void) {}\nvoid func_80041410(void) {}\nvoid func_80041420(void) {}\n", known

    def test_failed_canonical_recheck_restores_absorbed_unit(self) -> None:
        root, joint, known = self.failing_setup()
        before = self.snapshot(root)
        with self.assertRaisesRegex(ValueError, r"canonical copy no longer matches \(mismatch\)"):
            self.adopt(root, JOINT, joint, absorbs=["func_80041410"], data=((0x10C8E0, 8),), defined=("D_801398A0",),
                       known=known, final_status="mismatch")
        self.assertEqual(self.snapshot(root), before)

    def test_midway_write_failure_restores_absorbed_unit(self) -> None:
        root, joint, known = self.failing_setup()
        before = self.snapshot(root)
        write_text, failed = Path.write_text, []

        def flaky(path: Path, *args, **kwargs):
            if path.name == "images.json" and not failed:  # after sources moved and two configs written
                failed.append(path)
                raise OSError("disk full")
            return write_text(path, *args, **kwargs)

        with patch.object(Path, "write_text", flaky), self.assertRaisesRegex(OSError, "disk full"):
            self.adopt(root, JOINT, joint, absorbs=["func_80041410"], data=((0x10C8E0, 8),), defined=("D_801398A0",), known=known)
        self.assertTrue(failed)
        self.assertEqual(self.snapshot(root), before)

    def test_absorbed_tu_must_be_canonical_in_the_audit(self) -> None:
        candidate = parse_tu("void f(void) {}\n", "candidate.c")
        candidate.candidate = 0
        with self.assertRaisesRegex(ValueError, "not canonical TUs of this interface audit"):
            gate_findings(Gate([candidate]), 0, {"src/units/main_14400/f.c"})

    def test_data_carve_inside_another_units_bridge_is_refused(self) -> None:
        layout = DATA_LAYOUT
        with patch("adopt._assert_main_bss_unchanged"):
            for name, start in (("first", 0x10C8F1), ("second", 0x10C8F2)):
                layout = split_data_yaml(layout, "main_14400", name, ".data", start, 1)
        for name in ("first", "second"):
            unit = Absorbed("main_14400", f"src/units/main_14400/{name}.c", ())
            with self.subTest(name=name), self.assertRaisesRegex(ValueError, "not its own bridge"):
                restore_data_yaml(layout, unit)

    def test_bss_restore_inverts_the_carve(self) -> None:
        cases = (("resident", "func_8002F7C0", 0x8003D9E0, 0x8003EBB0, RESIDENT_BSS_END),
                 ("main_14400", "func_80041600", 0x801609B0, 0x801609C0, MAIN_BSS_END))
        for image, name, start, end, bss_end in cases:
            with self.subTest(name=name):
                carved = split_bss_yaml(BSS_LAYOUT, image, name, start, end, bss_end)
                restored = restore_bss_yaml(carved, Absorbed(image, f"src/units/{image}/{name}.c", ()))
                self.assertEqual(split_bss_yaml(restored, image, name, start, end, bss_end), carved)
                if image == "resident":  # merged into the preceding asm bss
                    self.assertEqual(restored, BSS_LAYOUT)
                else:  # the replaced first subsegment gets the adopt tail name
                    self.assertIn("      - {type: bss, vram: 0x801609B0, name: main_bss_1609b0}\n", restored)

    def test_admitted_absorption_retires_the_old_tu_for_later_candidates(self) -> None:
        absorbed = "src/units/main_14400/A.c"

        def gate() -> Gate:
            # A later candidate still using the absorbed unit's old shape contradicts the joint's definition; the
            # absorbed definition sorts first, so while audited it would be the authority the stale caller agrees with.
            results = [parse_tu("int f(void) { return 1; }\n", absorbed), parse_tu("void f(int value) {}\n", "tmp/joint.c"),
                       parse_tu("int f(void);\nint g(void) { return f(); }\n", "tmp/stale_caller.c")]
            results[1].candidate, results[2].candidate = 0, 1
            return Gate(results)

        retiring = gate()
        self.assertEqual(gate_findings(retiring, 0, {absorbed}), [])
        gate_admit(retiring, 0, {absorbed})
        self.assertNotIn(absorbed, [result.tu for result in retiring.admitted])
        self.assertTrue(gate_findings(retiring, 1, set()), "the stale caller must be refused against the joint alone")
        keeping = gate()
        keeping.admit(0)
        self.assertEqual(keeping.check(1), [], "with the old definition still audited, the stale caller joins an existing conflict")

    def test_text_restore_keeps_boundaries_that_are_not_the_units_own_tail(self) -> None:
        layout = ABSORB_LAYOUT.replace("      - [0x14500, c, units/main_14400/next_text]\n",
                                       "      - [0x14460, asm, reviewed_boundary]\n      - [0x14500, c, units/main_14400/next_text]\n")
        carved = split_yaml(layout, "main_14400", "func_80041410", 0x14450, 0x0C, 0x14460)
        self.assertIn("{start: 0x1445C, type: bin, name: func_80041410_padding", carved)
        unit = Absorbed("main_14400", "src/units/main_14400/func_80041410.c", ({"symbol": "func_80041410", "rom_start": 0x14450},))
        self.assertEqual(restore_text_yaml(carved, unit), layout)

    def preview(self, *, fail_recheck: bool = False) -> tuple[str, str, dict[str, bytes], dict[str, bytes]]:
        root = self.project()
        for name in REQUIRED_FILES:
            (root / name).write_text("{}\n")
        (root / "include").mkdir()
        self.adopt(root, [JOINT[1]], "char bytes[8] = {1, 80, 6};\nvoid func_80041410(void) {}\n",
                   data=((0x10C818, 8),))
        candidates = root / "src/candidates"
        candidates.mkdir()
        (candidates / "recarve.c").write_text("char bytes[7] = {80, 6};\nvoid func_80041410(void) {}\n")
        (candidates / "claim.c").write_text("char released = 1;\nvoid func_80041400(void) {}\n")
        batch = root / "batch.json"
        batch.write_text(json.dumps([
            {"functions": [JOINT[1]], "source": str(candidates / "recarve.c"), "absorbs": [JOINT[1]],
             "includes": [str(root / "include")], "profile": "gcc281pm-gnu291-O2-unsigned"},
            {"functions": [JOINT[0]], "source": str(candidates / "claim.c"), "profile": "gcc281pm-gnu291-O2-unsigned"},
        ]))
        rom_path = root / "rom.bin"
        rom_path.write_bytes(bytes(0x14500))
        before, manifest = self.snapshot(root), input_manifest(root)
        staged_snapshots, worker_roots = [], []

        def worker(argv: list[str], *, cwd: Path, env: dict, capture_output: bool, text: bool) -> SimpleNamespace:
            worker_roots.append(cwd)
            self.assertNotEqual(cwd, root)
            self.assertEqual(argv[:3], [sys.executable, "-B", "-c"])
            self.assertEqual(Path(argv[-1]), rom_path)
            self.assertEqual(env["PYTHONDONTWRITEBYTECODE"], "1")
            self.assertTrue(capture_output and text)
            units = _load_units(Path(argv[-2]))
            self.assertEqual(units[0].source, cwd / "src/candidates/recarve.c")
            self.assertEqual(units[0].includes, [cwd / "include"])

            def collect_fixture(project: Path, extra: list[dict]) -> list:
                self.assertEqual(project, cwd)
                profiles = json.loads((project / "config/compiler_profiles.json").read_text())["tu_profiles"]
                results = [parse_tu((project / source).read_text(), source) for source in profiles]
                for index, entry in enumerate(extra):
                    candidate = Path(entry["source"])
                    result = parse_tu(candidate.read_text(), str(candidate))
                    result.candidate = index
                    results.append(result)
                return results

            def check_fixture(targets, source, profile, out, **kwargs):
                target = targets[0]
                start, size = (0x10C819, 7) if target.symbol == JOINT[1] else (0x10C818, 1)
                final = source.parent == cwd / "src/units/main_14400"
                if target.symbol == JOINT[0]:
                    owner = next(part for part in yaml_data_parts((cwd / "config/shiren2.jp.yaml").read_text())
                                 if part.start == start)
                    self.assertEqual((owner.end, owner.kind, owner.preserved),
                                     (0x10C819, ".data" if final else "data", not final))
                    self.assertIn("char bytes[7]", (cwd / "src/units/main_14400" / f"{JOINT[1]}.c").read_text())
                out.mkdir(parents=True, exist_ok=True)
                (out / "candidate.o").write_bytes(b"fixture")
                (out / "source").symlink_to(cwd, target_is_directory=True)
                result = SimpleNamespace(status="mismatch-data" if fail_recheck and final and target.symbol == JOINT[0]
                                         else "match-with-data", problems=[], participating_files=[{"path": str(source)}],
                                         text_bytes=target.slot_end - target.rom_start, instruction_bytes=target.size,
                                         slot_bytes=target.slot_end - target.rom_start, bss_carve=None,
                                         source={"sha256": "a" * 64}, out_dir=str(out),
                                         data_sections=[{"section": ".data", "rom_start": start, "size": size,
                                                         "vram": start + MAIN_DELTA, "matches_original": True}])
                (out / "result.json").write_text(json.dumps({"status": result.status, "out_dir": result.out_dir}) + "\n")
                return result, ""

            with patch("adopt.PROJECT", cwd), patch("adopt.collect", side_effect=collect_fixture), \
                    patch("adopt.resolve_targets", side_effect=lambda symbols: [ABSORB_TARGETS[s] for s in symbols]), \
                    patch("adopt.check", side_effect=check_fixture), patch("adopt._assert_main_bss_unchanged"), \
                    patch("adopt.accepted_build", return_value=cwd), patch("adopt.symbol_addresses", return_value={}), \
                    patch("adopt.Elf", return_value=SimpleNamespace(symbols=[])):
                log = []
                totals, failures = _adopt_batch(units, Path(argv[-1]).read_bytes(), log)
            staged_snapshots.append(self.snapshot(cwd))
            return SimpleNamespace(returncode=0, stdout=json.dumps({"totals": totals, "failures": failures, "log": log}), stderr="")

        stdout, stderr = io.StringIO(), io.StringIO()
        with patch("adopt.PROJECT", root), patch("adopt.CANONICAL_ROM", rom_path), \
                patch("adopt.subprocess.run", side_effect=worker), patch.object(sys, "argv", ["adopt.py", "--batch", str(batch), "--dry-run"]), \
                redirect_stdout(stdout), redirect_stderr(stderr), self.assertRaises(SystemExit) as exit_:
            main()
        self.assertEqual(exit_.exception.code, int(fail_recheck))
        self.assertEqual(self.snapshot(root), before)
        self.assertEqual(input_manifest(root), manifest)
        self.assertEqual(len(worker_roots), 1)
        self.assertFalse(worker_roots[0].exists(), "the temporary preview tree must be discarded")
        self.assertNotIn(str(worker_roots[0]), stdout.getvalue() + stderr.getvalue())
        return stdout.getvalue(), stderr.getvalue(), before, staged_snapshots[0]

    def test_dependent_dry_run_claims_recarved_bridge_and_prints_one_cumulative_diff(self) -> None:
        stdout, stderr, before, after = self.preview()
        self.assertEqual(stderr, "")
        self.assertIn("would adopt 2 function(s), 28 instruction bytes; 0 failure(s)", stdout)
        self.assertEqual(stdout.count("--- a/config/shiren2.jp.yaml\n"), 1)
        self.assertIn("-      - [0x10C818, .data, units/main_14400/func_80041410]", stdout)
        self.assertIn("+      - [0x10C818, .data, units/main_14400/func_80041400]", stdout)
        self.assertIn("+      - [0x10C819, .data, units/main_14400/func_80041410]", stdout)
        self.assertNotIn("main_14400_data_bytes_10c818", stdout, "the intermediate bridge is not the cumulative result")
        self.assertIn("--- /dev/null\n+++ b/src/units/main_14400/func_80041400.c", stdout)
        self.assertIn("--- a/src/units/main_14400/func_80041410.c", stdout)
        self.assertNotEqual(before, after)

    def test_dependent_dry_run_rechecks_and_rolls_back_only_the_failed_unit(self) -> None:
        stdout, stderr, before, after = self.preview(fail_recheck=True)
        self.assertIn("would adopt 1 function(s), 12 instruction bytes; 1 failure(s)", stdout)
        self.assertIn("canonical copy no longer matches (mismatch-data)", stderr)
        self.assertIn("main_14400_data_bytes_10c818", stdout)
        self.assertNotIn("src/units/main_14400/func_80041400.c", after)
        self.assertNotEqual(before["src/units/main_14400/func_80041410.c"], after["src/units/main_14400/func_80041410.c"])

    def test_failed_dry_run_retains_cited_result_after_snapshot_cleanup(self) -> None:
        _stdout, stderr, _before, _after = self.preview(fail_recheck=True)
        failure = next(line for line in stderr.splitlines() if "canonical copy no longer matches" in line)
        evidence = Path(failure.rsplit(": ", 1)[1])
        self.assertIn("/scratch/omp/adopt/", str(evidence))
        self.assertEqual(json.loads((evidence / "result.json").read_text())["status"], "mismatch-data")
        self.assertEqual((evidence / "candidate.o").read_bytes(), b"fixture")
        self.assertTrue((evidence / "source").is_symlink(), "evidence copying must not follow the discarded source tree")


if __name__ == "__main__":
    unittest.main()
