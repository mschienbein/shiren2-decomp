"""Layout edits made by tools/adopt.py when carving a C unit out of an asm range."""
from __future__ import annotations

import sys
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "tools"))

from adopt import split_yaml  # noqa: E402

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


if __name__ == "__main__":
    unittest.main()
