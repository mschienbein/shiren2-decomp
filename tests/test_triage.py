"""Detector edge cases for tools/triage.py on tiny inline assembly fixtures."""
from __future__ import annotations

import sys
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "tools"))
from packet import _parse_functions
from triage import Region, analyse, is_literal

REGIONS = [
    Region("main_14400:text", 0x800413C0, 0x80136A10),
    Region("main_14400:data", 0x80136A10, 0x801609B0),
    Region("main_14400:bss", 0x801609B0, 0x801E4EA0),
]


def run(body: str) -> dict:
    [(symbol, _assembly, words, _handwritten)] = _parse_functions(f"glabel func_80046D50\n{body}endlabel func_80046D50\n")
    return analyse(symbol, words, REGIONS)


class TriageDetectorTests(unittest.TestCase):
    def test_jr_ra_is_not_a_jump_table(self) -> None:
        result = run(
            "    /* 19D90 80046D50 03E00008 */  jr         $ra\n"
            "    /* 19D94 80046D54 00000000 */   nop\n"
        )
        self.assertEqual(result["jump_tables"], [])
        self.assertEqual(result["indirect_jumps"], [])

    def test_direct_jump_table_in_carrier(self) -> None:
        result = run(
            "    /* 19D90 80046D50 2C620007 */  sltiu      $v0, $v1, 0x7\n"
            "    /* 19D94 80046D54 00031080 */  sll        $v0, $v1, 2\n"
            "    /* 19D98 80046D58 3C018015 */  lui        $at, %hi(jtbl_8014A980)\n"
            "    /* 19D9C 80046D5C 00220821 */  addu       $at, $at, $v0\n"
            "    /* 19DA0 80046D60 8C22A980 */  lw         $v0, %lo(jtbl_8014A980)($at)\n"
            "    /* 19DA4 80046D64 00400008 */  jr         $v0\n"
            "    /* 19DA8 80046D68 00000000 */   nop\n"
            "    /* 19DAC 80046D6C 03E00008 */  jr         $ra\n"
            "    /* 19DB0 80046D70 00000000 */   nop\n"
        )
        [table] = result["jump_tables"]
        self.assertEqual((table["table"], table["method"], table["region"]), ("jtbl_8014A980", "direct", "main_14400:data"))
        self.assertEqual(result["indirect_jumps"], [])

    def test_hoisted_jump_table_base(self) -> None:
        result = run(
            "    /* 19D90 80046D50 3C1E8015 */  lui        $fp, %hi(jtbl_8014CC30)\n"
            "    /* 19D94 80046D54 27DECC30 */  addiu      $fp, $fp, %lo(jtbl_8014CC30)\n"
            "    /* 19D98 80046D58 00041080 */  sll        $v0, $a0, 2\n"
            "    /* 19D9C 80046D5C 005E1021 */  addu       $v0, $v0, $fp\n"
            "    /* 19DA0 80046D60 8C420000 */  lw         $v0, 0x0($v0)\n"
            "    /* 19DA4 80046D64 00400008 */  jr         $v0\n"
            "    /* 19DA8 80046D68 00000000 */   nop\n"
        )
        [table] = result["jump_tables"]
        self.assertEqual((table["table"], table["method"], table["address"]), ("jtbl_8014CC30", "hoisted", "8014CC30"))

    def test_jr_t9_function_pointer_is_indirect_not_table(self) -> None:
        result = run(
            "    /* 19D90 80046D50 3C048015 */  lui        $a0, %hi(D_80150000)\n"
            "    /* 19D94 80046D54 24840000 */  addiu      $a0, $a0, %lo(D_80150000)\n"
            "    /* 19D98 80046D58 8C990004 */  lw         $t9, 0x4($a0)\n"
            "    /* 19D9C 80046D5C 03200008 */  jr         $t9\n"
            "    /* 19DA0 80046D60 00000000 */   nop\n"
        )
        self.assertEqual(result["jump_tables"], [])
        self.assertEqual(result["indirect_jumps"], ["80046D5C:jr $t9"])
        self.assertEqual(result["carrier_address_args"], ["D_80150000"])

    def test_carrier_float_literal_and_indexed_table(self) -> None:
        result = run(
            "    /* 19D90 80046D50 3C018015 */  lui        $at, %hi(D_8014C0E0)\n"
            "    /* 19D94 80046D54 D420C0E0 */  ldc1       $fv0, %lo(D_8014C0E0)($at)\n"
            "    /* 19D98 80046D58 3C018014 */  lui        $at, %hi(D_8013B680)\n"
            "    /* 19D9C 80046D5C 00220821 */  addu       $at, $at, $v0\n"
            "    /* 19DA0 80046D60 C420B680 */  lwc1       $fv0, %lo(D_8013B680)($at)\n"
        )
        direct, indexed = result["float_loads"]
        self.assertEqual((direct["symbol"], direct["region"], direct["double"], direct["indexed"]), ("D_8014C0E0", "main_14400:data", True, False))
        self.assertTrue(indexed["indexed"])
        self.assertTrue(result["uses_fpu"] and result["uses_double"])
        floor = {"main_14400:data": "8014A980"}
        self.assertTrue(is_literal(direct, set(), floor))
        self.assertFalse(is_literal(direct, {"D_8014C0E0"}, floor))
        self.assertFalse(is_literal(indexed, set(), floor))
        below = dict(direct, address="8013B7A8", symbol="D_8013B7A8")
        self.assertFalse(is_literal(below, set(), floor))

    def test_bss_float_load_is_not_literal(self) -> None:
        result = run(
            "    /* 19D90 80046D50 3C018017 */  lui        $at, %hi(D_80170000)\n"
            "    /* 19D94 80046D54 C4200000 */  lwc1       $fv0, %lo(D_80170000)($at)\n"
        )
        [load] = result["float_loads"]
        self.assertEqual(load["region"], "main_14400:bss")
        self.assertFalse(is_literal(load, set(), {}))

    def test_division_break_is_not_system_but_cop0_is(self) -> None:
        result = run(
            "    /* 19D90 80046D50 0085001A */  div        $zero, $a0, $a1\n"
            "    /* 19D94 80046D54 0007000D */  break      7\n"
            "    /* 19D98 80046D58 40086000 */  mfc0       $t0, $12\n"
        )
        self.assertEqual(result["system_instructions"], ["mfc0"])
        self.assertEqual(result["div"], 1)

    def test_backward_branch_counts_as_loop(self) -> None:
        result = run(
            "  .L80046D50:\n"
            "    /* 19D90 80046D50 2484FFFF */  addiu      $a0, $a0, -0x1\n"
            "    /* 19D94 80046D54 1480FFFE */  bnez       $a0, .L80046D50\n"
            "    /* 19D98 80046D58 00000000 */   nop\n"
        )
        self.assertEqual(result["loops"], 1)

    def test_relocation_name_must_match_immediate(self) -> None:
        with self.assertRaises(ValueError):
            run("    /* 19D90 80046D50 3C018015 */  lui        $at, %hi(D_8013B680)\n")


if __name__ == "__main__":
    unittest.main()
