"""Bridge-spanning adoption, alias retirement and inverse ownership at the H11 boundaries."""
from __future__ import annotations

import json
import sys
import tempfile
import unittest
from pathlib import Path
from types import SimpleNamespace
from unittest.mock import patch

import yaml

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "tools"))

import adopt
from interfaces import Gate, parse_tu
from match import yaml_data_parts

DELTA = 0x8002CFC0
HEADER = """options: {}
segments:
  - name: main_14400
    type: code
    start: 0x14400
    vram: 0x800413C0
    subalign: 1
    subsegments:
      - [0x14400, asm, main_14400_tail_14400]
      - [0x11D770, rodata, main_rodata_11d770]
"""
FOOTER = """      - {type: bss, vram: 0x801609B0, name: main_14400}
  - {name: overlay, type: bin, start: 0x1339F0}
  - [0x2000000]
"""
CASES = (
    {
        "symbol": "func_800CC0E8", "start": 0x1272E4, "end": 0x12730F,
        "bridge": (0x1272E1, 0x1272E8), "neighbor": "func_800CBFC0",
        "before": """      - [0x1272B8, .rodata, units/main_14400/func_800CBFC0]
      - {start: 0x1272E1, type: bin, name: main_14400_rodata_bytes_1272e1, linker_section_order: .rodata}
      - [0x1272E8, rodata, main_rodata_1272e8]
      - [0x127310, .rodata, units/main_14400/func_800CC934]
""",
        "after": """      - [0x1272B8, .rodata, units/main_14400/func_800CBFC0]
      - {start: 0x1272E1, type: bin, name: main_14400_rodata_bytes_1272e1, linker_section_order: .rodata}
      - [0x1272E4, .rodata, units/main_14400/func_800CC0E8]
      - {start: 0x12730F, type: bin, name: main_14400_rodata_bytes_12730f, linker_section_order: .rodata}
      - [0x127310, .rodata, units/main_14400/func_800CC934]
""",
        "preserved": [(0x1272E1, 0x1272E4), (0x12730F, 0x127310)],
    },
    {
        "symbol": "func_80110258", "start": 0x130448, "end": 0x13048C,
        "bridge": (0x130488, 0x13048C), "neighbor": "func_801111E8",
        "before": """      - [0x12FF18, .rodata, units/main_14400/func_8010C47C]
      - {start: 0x130084, type: bin, name: main_14400_rodata_bytes_130084, linker_section_order: .rodata}
      - [0x130088, rodata, main_rodata_130088]
      - {start: 0x130488, type: bin, name: main_14400_rodata_bytes_130488, linker_section_order: .rodata}
      - [0x13048C, .rodata, units/main_14400/func_801111E8]
      - [0x130558, rodata, main_rodata_130558]
""",
        "after": """      - [0x12FF18, .rodata, units/main_14400/func_8010C47C]
      - {start: 0x130084, type: bin, name: main_14400_rodata_bytes_130084, linker_section_order: .rodata}
      - [0x130088, rodata, main_rodata_130088]
      - [0x130448, .rodata, units/main_14400/func_80110258]
      - [0x13048C, .rodata, units/main_14400/func_801111E8]
      - [0x130558, rodata, main_rodata_130558]
""",
        "preserved": [],
    },
)


def layout(case: dict, key: str = "before") -> str:
    return HEADER + case[key] + FOOTER


def known_labels(case: dict) -> dict[str, int]:
    addresses = {case["start"], case["start"] + 1, case["end"] - 1, case["end"],
                 *range(*case["bridge"]), case["bridge"][1]}
    return {f"D_{address + DELTA:X}": address + DELTA for address in sorted(addresses)}


def original_aliases(case: dict) -> str:
    return "PROVIDE(manual = 0x80100000); /* reviewed */\n" + "".join(
        f"PROVIDE(D_{address + DELTA:X} = 0x{address + DELTA:08X}); /* preserved bytes beside {case['neighbor']}, not C */\n"
        for address in range(*case["bridge"]))


class BridgeSpanningTests(unittest.TestCase):
    def test_recorded_layouts_carve_exactly_and_restore_without_changing_neighbors(self) -> None:
        for case in CASES:
            with self.subTest(symbol=case["symbol"]):
                preserved, consumed = [], []
                carved = adopt.split_data_yaml(layout(case), "main_14400", case["symbol"], ".rodata",
                                               case["start"], case["end"] - case["start"],
                                               preserved=preserved, consumed_bridges=consumed)
                self.assertEqual(carved, layout(case, "after"))
                self.assertEqual(preserved, [(start + DELTA, end + DELTA) for start, end in case["preserved"]])
                self.assertEqual(consumed, [(start + DELTA, end + DELTA) for start, end in [case["bridge"]]])
                unit = adopt.Absorbed("main_14400", f"src/units/main_14400/{case['symbol']}.c", ())
                restored = adopt.restore_data_yaml(carved, unit)
                self.assertEqual(restored, layout(case))
                self.assertEqual(adopt.split_data_yaml(restored, "main_14400", case["symbol"], ".rodata",
                                                      case["start"], case["end"] - case["start"]), carved)

    def test_multiline_nodes_across_the_run_are_replaced_whole(self) -> None:
        case = CASES[0]
        before = layout(case).replace(
            "{start: 0x1272E1, type: bin, name: main_14400_rodata_bytes_1272e1, linker_section_order: .rodata}",
            "{start: 0x1272E1, type: bin,\n        name: main_14400_rodata_bytes_1272e1,\n        linker_section_order: .rodata\n      }").replace(
            "[0x1272E8, rodata, main_rodata_1272e8]",
            "start: 0x1272E8\n        type: rodata\n        name: main_rodata_1272e8")
        result = adopt.split_data_yaml(before, "main_14400", case["symbol"], ".rodata", case["start"], 0x2B)
        self.assertEqual(yaml.safe_load(result), yaml.safe_load(layout(case, "after")))

    def test_both_sections_cross_arbitrary_contiguous_runs_and_keep_tail_bytes(self) -> None:
        for kind, alignment in (("data", 4), ("rodata", 8)):
            with self.subTest(kind=kind):
                before = HEADER + f"""      - [0x127280, {kind}, prefix]
      - {{start: 0x1272A0, type: bin, name: main_14400_{kind}_bytes_1272a0, linker_section_order: .{kind}}}
      - [0x1272A4, {kind}, middle]
      - [0x1272B0, {kind}, last]
      - [0x1272E0, .{kind}, units/main_14400/neighbor]
""" + FOOTER
                preserved = []
                after = adopt.split_data_yaml(before, "main_14400", "run", f".{kind}", 0x12729C, 0x1D,
                                              preserved=preserved)
                parts = [(part.start, part.end, part.kind, part.preserved) for part in yaml_data_parts(after)
                         if 0x127280 <= part.start < 0x1272E0]
                tail = (0x1272B9 + alignment - 1) // alignment * alignment
                prefix = 0x12729C // alignment * alignment
                expected = [(0x127280, prefix, kind, False)]
                if prefix < 0x12729C:
                    expected.append((prefix, 0x12729C, kind, True))
                expected.extend([(0x12729C, 0x1272B9, f".{kind}", False),
                                 (0x1272B9, tail, kind, True), (tail, 0x1272E0, kind, False)])
                self.assertEqual(parts, expected)

    def test_a_partially_consumed_final_bridge_keeps_its_suffix_and_roundtrips(self) -> None:
        case = CASES[1]
        carved = adopt.split_data_yaml(layout(case), "main_14400", case["symbol"], ".rodata", case["start"], 0x42)
        parts = [(part.start, part.end, part.kind, part.preserved) for part in yaml_data_parts(carved)
                 if 0x130448 <= part.start < 0x13048C]
        self.assertEqual(parts, [(0x130448, 0x13048A, ".rodata", False), (0x13048A, 0x13048C, "rodata", True)])
        unit = adopt.Absorbed("main_14400", f"src/units/main_14400/{case['symbol']}.c", ())
        self.assertEqual(adopt.restore_data_yaml(carved, unit), layout(case))

    def test_short_spans_restore_using_their_own_alignment_bridges_and_tail(self) -> None:
        for case, start, end in ((CASES[0], 0x1272E4, 0x1272E8), (CASES[0], 0x1272E4, 0x1272E9),
                                 (CASES[1], 0x130484, 0x13048C)):
            with self.subTest(start=start, end=end):
                carved = adopt.split_data_yaml(layout(case), "main_14400", case["symbol"], ".rodata", start, end - start)
                unit = adopt.Absorbed("main_14400", f"src/units/main_14400/{case['symbol']}.c", ())
                self.assertEqual(adopt.restore_data_yaml(carved, unit), layout(case))

    def test_foreign_or_c_owned_parts_anywhere_in_the_run_are_refused(self) -> None:
        case = CASES[0]
        original = "[0x1272E8, rodata, main_rodata_1272e8]"
        foreign = (
            "[0x1272E8, .rodata, units/main_14400/owned]",
            "[0x1272E8, data, foreign]",
            "[0x1272E8, c, units/main_14400/owned]",
            "[0x1272E8, cpp, units/main_14400/owned]",
            "{start: 0x1272E8, type: bin, name: foreign, linker_section_order: .rodata}",
            "{start: 0x1272E8, type: bin, name: main_14400_rodata_bytes_1272e8, linker_section_order: .text}",
            "{start: 0x1272E8, type: bin, name: main_14400_rodata_bytes_1272e8, linker_section_order: .rodata, subalign: 1}",
            "{start: 0x1272E8, type: rodata, name: foreign, linker_section: .data}",
            "[0x1272E8, rodata, foreign, {subalign: 1}]",
        )
        for entry in foreign:
            with self.subTest(entry=entry), self.assertRaises(ValueError):
                adopt.split_data_yaml(layout(case).replace(original, entry), "main_14400", case["symbol"], ".rodata",
                                      case["start"], 0x2B)
        # An opaque middle node must not disappear from the eligible-part traversal.
        hole = layout(case).replace(original, "[0x1272E8, bin, opaque]\n      - [0x1272F0, rodata, tail]")
        with self.assertRaises(ValueError):
            adopt.split_data_yaml(hole, "main_14400", case["symbol"], ".rodata", case["start"], 0x2B)
        for case in CASES:
            with self.subTest(overrun=case["symbol"]), self.assertRaises(ValueError):
                adopt.split_data_yaml(layout(case), "main_14400", case["symbol"], ".rodata",
                                      case["start"], case["end"] - case["start"] + 2)

    def test_contiguous_addresses_do_not_allow_crossing_parent_segments(self) -> None:
        text = HEADER + """      - [0x127280, rodata, first]
  - name: next
    type: code
    start: 0x1272E8
    vram: 0x801542A8
    subalign: 1
    subsegments:
      - [0x1272E8, rodata, second]
""" + FOOTER
        with self.assertRaises(ValueError):
            adopt.split_data_yaml(text, "main_14400", "cross", ".rodata", 0x1272E4, 8)


class BridgeAliasAdoptionTests(unittest.TestCase):
    def project(self, case: dict) -> Path:
        temporary = tempfile.TemporaryDirectory()
        self.addCleanup(temporary.cleanup)
        root = Path(temporary.name).resolve()
        files = {"config/matches.json": json.dumps({"functions": []}),
                 "config/compiler_profiles.json": json.dumps({"tu_profiles": {}}),
                 "config/images.json": json.dumps({"source_bindings": {}}),
                 "config/shiren2.jp.yaml": layout(case), "config/symbol_aliases.ld": original_aliases(case),
                 "candidate.o": "fixture"}
        for name, content in files.items():
            (root / name).parent.mkdir(parents=True, exist_ok=True)
            (root / name).write_text(content)
        return root

    def adopt_case(self, root: Path, case: dict, *, absorbs: bool = False, start: int | None = None,
                   end: int | None = None, final_status: str = "match-with-data") -> None:
        symbol = case["symbol"]
        start = case["start"] if start is None else start
        end = case["end"] if end is None else end
        candidate = root / "candidate.c"
        candidate.write_text(f"void {symbol}(void) {{}}\n")
        text_rom = int(symbol[5:], 16) - DELTA
        target = SimpleNamespace(image_id="main_14400", symbol=symbol, rom_start=text_rom,
                                 vram_start=text_rom + DELTA, size=16, slot_end=text_rom + 16)
        result = SimpleNamespace(status="match-with-data", image_id=target.image_id, problems=[], participating_files=[{"path": str(candidate)}],
                                 text_bytes=16, instruction_bytes=16, slot_bytes=16, bss_carve=None, generated_functions=[],
                                 source={"sha256": "a" * 64}, out_dir=str(root),
                                 data_sections=[{"section": ".rodata", "rom_start": start, "size": end - start,
                                                 "vram": start + DELTA}])
        final = SimpleNamespace(**{**vars(result), "status": final_status})
        source = f"src/units/main_14400/{symbol}.c"
        audited = parse_tu(candidate.read_text(), str(candidate))
        audited.candidate = 0
        gate = Gate([*([parse_tu(candidate.read_text(), source)] if absorbs else []), audited])
        defined = {f"D_{address + DELTA:X}" for address in (start, case["bridge"][0]) if start <= address < end}
        with patch("adopt.PROJECT", root), patch("adopt.resolve_targets", return_value=[target]), \
                patch("adopt.check", side_effect=[(result, None), (final, None)]), \
                patch("adopt.accepted_build", return_value=root), patch("adopt.symbol_addresses", return_value=known_labels(case)), \
                patch("adopt.Elf", return_value=SimpleNamespace(symbols=[SimpleNamespace(name=name, shndx=1) for name in defined])):
            adopt.adopt(adopt.Unit([symbol], candidate, absorbs=[symbol] if absorbs else []),
                        rom=bytes(target.slot_end), log=[], gate=gate, index=0)

    def test_consumed_aliases_are_rebound_and_absorption_roundtrips(self) -> None:
        for case in CASES:
            with self.subTest(symbol=case["symbol"]):
                root = self.project(case)
                self.adopt_case(root, case)
                aliases = (root / "config/symbol_aliases.ld").read_text()
                self.assertNotIn(f"beside {case['neighbor']},", aliases)
                self.assertIn("PROVIDE(manual = 0x80100000); /* reviewed */", aliases)
                self.assertNotIn(f"PROVIDE(D_{case['start'] + DELTA:X} ", aliases)
                for address in range(*case["bridge"]):
                    if address in {case["start"], case["bridge"][0]} and case["start"] <= address < case["end"]:
                        self.assertNotIn(f"PROVIDE(D_{address + DELTA:X} ", aliases)
                    else:
                        self.assertEqual(aliases.count(f"PROVIDE(D_{address + DELTA:X} "), 1)
                        comment = f"inside {case['symbol']} .rodata" if case["start"] <= address else f"preserved bytes beside {case['symbol']}, not C"
                        self.assertIn(f"0x{address + DELTA:08X}); /* {comment} */", aliases)
                before = {name: (root / name).read_bytes() for name in adopt.CONFIG_FILES}
                self.adopt_case(root, case, absorbs=True)
                self.assertEqual({name: (root / name).read_bytes() for name in adopt.CONFIG_FILES}, before)

    def test_absorption_rebuilds_neighbor_aliases_even_when_the_new_carve_moves_away(self) -> None:
        for case, start, end in ((CASES[0], 0x1272F0, 0x127308), (CASES[1], 0x130448, 0x130480)):
            with self.subTest(symbol=case["symbol"]):
                root = self.project(case)
                self.adopt_case(root, case)
                self.adopt_case(root, case, absorbs=True, start=start, end=end)
                aliases = (root / "config/symbol_aliases.ld").read_text()
                for address in range(*case["bridge"]):
                    line = (f"PROVIDE(D_{address + DELTA:X} = 0x{address + DELTA:08X}); "
                            f"/* preserved bytes beside {case['neighbor']}, not C */")
                    self.assertEqual(aliases.count(line), 1)
                parts = yaml_data_parts((root / "config/shiren2.jp.yaml").read_text())
                bridge = next(part for part in parts if part.start == case["bridge"][0])
                self.assertEqual((bridge.end, bridge.kind, bridge.preserved), (case["bridge"][1], "rodata", True))
                owned = next(part for part in parts if part.name.endswith("/" + case["symbol"]))
                self.assertEqual((owned.start, owned.end, owned.kind), (start, end, ".rodata"))

    def test_recheck_failure_leaves_original_aliases_and_layout_intact(self) -> None:
        for case in CASES:
            with self.subTest(symbol=case["symbol"]):
                root = self.project(case)
                before = {name: (root / name).read_bytes() for name in adopt.CONFIG_FILES}
                with self.assertRaisesRegex(ValueError, "canonical copy no longer matches"):
                    self.adopt_case(root, case, final_status="mismatch-data")
                self.assertEqual({name: (root / name).read_bytes() for name in adopt.CONFIG_FILES}, before)
                self.assertFalse((root / f"src/units/main_14400/{case['symbol']}.c").exists())

    def test_retirement_is_half_open_and_preserves_manual_and_interior_aliases(self) -> None:
        lines = (
            "PROVIDE(before = 0x80100000); /* preserved bytes beside old, not C */\n"
            "PROVIDE(start = 0x80100001); /* preserved bytes beside old, not C */\n"
            "PROVIDE(end = 0x80100004); /* preserved bytes beside old, not C */\n"
            "PROVIDE(manual = 0x80100002); /* reviewed */\n"
            "PROVIDE(inside = 0x80100003); /* inside neighbor .rodata */\n")
        self.assertEqual(adopt._retire_bridge_aliases(lines, [(0x80100001, 0x80100004)], "main_14400"),
                         lines.replace("PROVIDE(start = 0x80100001); /* preserved bytes beside old, not C */\n", ""))

    def test_overlay_aliases_keep_only_own_image_labels(self) -> None:
        # Same-VRAM overlays (A delta 0x800B14B0, B 0x800AE0E0): an A carve keeps only A-qualified labels;
        # B labels, unqualified slot absolutes and splat segment symbols at the same address are not its labels.
        known = {"jtbl_801E7EF0_136A40": 0x801E7EF0, "func_801E7FB4_139ED4": 0x801E7FB4, "D_801E7F00": 0x801E7F00,
                 "overlay_1339f0_DATA_END": 0x801E7EF0, "overlay_136dc0_TEXT_START": 0x801E7EF0,
                 "D_801E7EF4_136A44.NON_MATCHING": 0x801E7EF4}
        bridge = [(0x801E7EF0, 0x801E8000)]
        self.assertEqual(adopt._bridge_aliases(known, set(), "unit", bridge, "overlay_1339f0"),
                         "PROVIDE(jtbl_801E7EF0_136A40 = 0x801E7EF0); /* preserved bytes beside unit, not C */\n")
        self.assertEqual(adopt._bridge_aliases(known, set(), "unit", bridge, "overlay_136dc0"),
                         "PROVIDE(func_801E7FB4_139ED4 = 0x801E7FB4); /* preserved bytes beside unit, not C */\n")
        self.assertEqual(adopt._bridge_aliases(known, set(), "unit", bridge, "main_14400"),
                         "PROVIDE(D_801E7F00 = 0x801E7F00); /* preserved bytes beside unit, not C */\n")
        lines = ("PROVIDE(D_801E7EF0_136A40 = 0x801E7EF0); /* preserved bytes beside a, not C */\n"
                 "PROVIDE(D_801E7EF0_139E10 = 0x801E7EF0); /* preserved bytes beside b, not C */\n")
        self.assertEqual(adopt._retire_bridge_aliases(lines, [(0x801E7EF0, 0x801E7EF4)], "overlay_1339f0"),
                         "PROVIDE(D_801E7EF0_139E10 = 0x801E7EF0); /* preserved bytes beside b, not C */\n")


if __name__ == "__main__":
    unittest.main()
