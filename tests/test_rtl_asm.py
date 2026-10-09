"""Operand-sensitive instruction evidence, including reviewed delayed emissions."""
from __future__ import annotations

import sys
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "tools"))
from rtl_asm import encoding_options


def matches(assembly: str, word: int) -> bool:
    options = encoding_options(assembly)
    return options is not None and any(word & mask == expected for mask, expected in options)


class EncodingTests(unittest.TestCase):
    def test_concrete_instructions_check_every_bit(self) -> None:
        examples = [
            ("addu $2,$3,$4", 0x00641021),
            ("daddu $2,$3,$4", 0x0064102D),
            ("sll $2,$3,4", 0x00031100),
            ("dsra32 $2,$3,4", 0x0003113F),
            ("sllv $2,$3,$4", 0x00831004),
            ("addiu $sp,$sp,-32", 0x27BDFFE0),
            ("ori $2,$3,0xabcd", 0x3462ABCD),
            ("lui $2,0x1234", 0x3C021234),
            ("lw $2,16($sp)", 0x8FA20010),
            ("sw $2,-4($sp)", 0xAFA2FFFC),
            ("lbu $2,137($4)", 0x90820089),
            ("j $31", 0x03E00008),
            ("jalr $4", 0x0080F809),
            ("jalr $2,$4", 0x00801009),
            ("mflo $2", 0x00001012),
            ("mthi $3", 0x00600011),
            ("multu $3,$4", 0x00640019),
            ("div $0,$3,$4", 0x0064001A),
            ("mfc1 $2,$f4", 0x44022000),
            ("mtc1 $2,$f4", 0x44822000),
            ("mfc0 $2,$12", 0x40026000),
            ("cfc1 $2,$31", 0x4442F800),
            ("lwc1 $f2,16($sp)", 0xC7A20010),
            ("sdc1 $f2,16($sp)", 0xF7A20010),
            ("add.s $f2,$f4,$f6", 0x46062080),
            ("mul.d $f2,$f4,$f6", 0x46262082),
            ("mov.d $f2,$f4", 0x46202086),
            ("c.eq.s $f4,$f6", 0x46062032),
            ("cvt.d.w $f2,$f4", 0x468020A1),
            ("trunc.w.s $f2,$f4", 0x4600208D),
        ]
        for assembly, word in examples:
            with self.subTest(assembly=assembly):
                self.assertEqual(encoding_options(assembly), ((0xFFFFFFFF, word),))
                self.assertTrue(matches(assembly, word))
                for bit in range(32):
                    self.assertFalse(matches(assembly, word ^ (1 << bit)), f"bit {bit}")

    def test_register_aliases_and_comments(self) -> None:
        self.assertEqual(encoding_options("addu $v0,$s8,$ra # 19 addsi"),
                         encoding_options("addu $2,$30,$31"))
        self.assertEqual(encoding_options("lw $v0,($sp)"), encoding_options("lw $2,0($29)"))
        self.assertEqual(encoding_options("\tli $2,1 # 1 # 32 movsi"), encoding_options("li $2,1"))

    def test_move_alternatives_are_operand_bound(self) -> None:
        for word in (0x00601021, 0x00601025, 0x0060102D):
            self.assertTrue(matches("move $2,$3", word))
            self.assertFalse(matches("move $2,$4", word))
            self.assertFalse(matches("move $4,$3", word))
        self.assertFalse(matches("move $2,$3", 0))
        self.assertFalse(matches("move $2,$3", 0x00601020))
        self.assertTrue(matches("neg $2,$3", 0x00031022))
        self.assertTrue(matches("negu $2,$3", 0x00031023))
        self.assertTrue(matches("not $2,$3", 0x00601027))
        self.assertFalse(matches("not $2,$3", 0x00641027))

    def test_small_and_large_li(self) -> None:
        for word in (0x24020001, 0x34020001):
            self.assertTrue(matches("li $2,1", word))
            self.assertFalse(matches("li $2,2", word))
        self.assertTrue(matches("li $2,-1", 0x2402FFFF))
        self.assertFalse(matches("li $2,-1", 0x3402FFFF))
        self.assertTrue(matches("li $2,65535", 0x3402FFFF))
        self.assertFalse(matches("li $2,65535", 0x2402FFFF))
        self.assertTrue(matches("li $2,0x12345678", 0x3C021234))
        self.assertTrue(matches("li $2,0x12345678", 0x34425678))
        self.assertFalse(matches("li $2,0x12345678", 0x3C031234))
        self.assertFalse(matches("li $2,0x12345678", 0x34425679))
        self.assertEqual(encoding_options("li $2,0x01650000"), ((0xFFFFFFFF, 0x3C020165),))

    def test_add_immediate_macro(self) -> None:
        self.assertEqual(encoding_options("addu $17,$sp,64"), ((0xFFFFFFFF, 0x27B10040),))
        self.assertFalse(matches("addu $17,$sp,64", 0x27B10044))
        self.assertIsNone(encoding_options("addu $17,$sp,65536"))

    def test_subtract_immediate_macros_check_negated_constant(self) -> None:
        for source, word in (
            ("subu $sp,$sp,32", 0x27BDFFE0),
            ("sub $2,$3,16", 0x2062FFF0),
            ("dsubu $2,$3,-16", 0x64620010),
            ("dsub $2,$3,32768", 0x60628000),
        ):
            with self.subTest(source=source):
                self.assertEqual(encoding_options(source), ((0xFFFFFFFF, word),))
                self.assertFalse(matches(source, word ^ 1))
                self.assertFalse(matches(source, word ^ (1 << 16)))
        self.assertIsNone(encoding_options("subu $sp,$sp,32769"))
        self.assertIsNone(encoding_options("subu $sp,$sp,-32768"))
        self.assertIsNone(encoding_options("subu $sp,$sp,symbol"))

    def test_numeric_literal_base_is_not_guessed(self) -> None:
        self.assertTrue(matches("li $2,10", 0x2402000A))
        self.assertTrue(matches("li $2,0x10", 0x24020010))
        for source in ("li $2,010", "li $2,-010", "lw $2,010($sp)",
                       "addiu $2,$3,010", "sll $2,$3,010"):
            self.assertIsNone(encoding_options(source), source)

    def test_relocation_only_wildcards_immediate(self) -> None:
        cases = [
            ("lui $2,%hi(object+4)", 0x3C020000),
            ("addiu $2,$2,%lo(object+4)", 0x24420000),
            ("lw $2,%lo(object)($3)", 0x8C620000),
            ("beq $2,$3,.L2", 0x10430000),
            ("bgez $2,.L2", 0x04410000),
            ("bc1t .L2", 0x45010000),
        ]
        for assembly, word in cases:
            with self.subTest(assembly=assembly):
                self.assertEqual(encoding_options(assembly), ((0xFFFF0000, word),))
                self.assertTrue(matches(assembly, word | 0xABCD))
                for bit in range(16, 32):
                    self.assertFalse(matches(assembly, word ^ (1 << bit)))
        self.assertEqual(encoding_options("jal func_8002AAB0"), ((0xFC000000, 0x0C000000),))
        self.assertTrue(matches("jal func_8002AAB0", 0x0C00AAAC))
        self.assertFalse(matches("jal func_8002AAB0", 0x0800AAAC))

    def test_absolute_memory_macros(self) -> None:
        self.assertEqual(encoding_options("sw $2,D_80036F80"),
                         ((0xFFFF0000, 0x3C010000), (0xFFFF0000, 0xAC220000)))
        self.assertTrue(matches("lw $2,object", 0x3C028003))
        self.assertTrue(matches("lw $2,object", 0x8C426F80))
        self.assertFalse(matches("lw $2,object", 0xAC226F80))
        self.assertFalse(matches("sw $2,object", 0xAC236F80))
        self.assertEqual(encoding_options("sw $2,0x12348000"),
                         ((0xFFFFFFFF, 0x3C011235), (0xFFFFFFFF, 0xAC228000)))
        self.assertFalse(matches("sw $2,0x12348000", 0x3C011234))
        self.assertFalse(matches("sw $2,0x12348000", 0xAC227FFF))
        self.assertTrue(matches("sw $2,object($3)", 0x00230821))
        self.assertFalse(matches("sw $2,object($3)", 0x00240821))
        self.assertTrue(matches("lwc1 $f2,object", 0xC4228000))
        self.assertFalse(matches("lwc1 $f2,object", 0xC4238000))

    def test_la_macro(self) -> None:
        self.assertEqual(encoding_options("la $2,object"),
                         ((0xFFFF0000, 0x3C020000), (0xFFFF0000, 0x24420000)))
        self.assertFalse(matches("la $2,object", 0x24430000))
        self.assertEqual(encoding_options("la $2,16($sp)"), ((0xFFFFFFFF, 0x27A20010),))
        self.assertIsNone(encoding_options("la $2,object($sp)"))

    def test_review_controller_init_sw_and_jal_cannot_swap(self) -> None:
        # Real mapping.s lines 57..59: UID32 li, UID34 sw, UID37 jal.
        store = "sw $2,D_80036F80 # 34 movsi_internal2/8"
        call = "jal func_8002AAB0 # 37 call_value_internal1"
        self.assertTrue(matches(store, 0x3C018003))
        self.assertTrue(matches(store, 0xAC226F80))
        self.assertFalse(matches(store, 0x0C00AAAC))
        self.assertTrue(matches(call, 0x0C00AAAC))
        self.assertFalse(matches(call, 0xAC226F80))
        self.assertFalse(matches("li $2,1", 0x3C018003))

    def test_review_return_and_lbu_cannot_swap(self) -> None:
        load = "lbu $2,137($4) # 11 zero_extendqisi2/2"
        ret = "j $31 # 23 return"
        self.assertTrue(matches(ret, 0x03E00008))
        self.assertFalse(matches(load, 0x03E00008))
        self.assertTrue(matches(load, 0x90820089))
        self.assertFalse(matches(ret, 0x90820089))
        self.assertFalse(matches("mtc1 $2,$f4", 0xC7A40010))

    def test_nop_is_not_universal(self) -> None:
        self.assertEqual(encoding_options("nop"), ((0xFFFFFFFF, 0),))
        for assembly in ("addu $2,$3,$4", "li $2,0", "move $2,$0", "jal target", "sw $2,object"):
            self.assertFalse(matches(assembly, 0), assembly)
        self.assertFalse(matches("nop", 0x24020001))
        # sll zero,zero,0 really is the same encoding, not an escape hatch.
        self.assertTrue(matches("sll $0,$0,0", 0))

    def test_unknown_and_malformed_forms_fail_closed(self) -> None:
        for assembly in (
            "", "# only comment", ".set reorder", "label:", "mystery $2,$3",
            "nop $2", "addu $2,$3", "addu $2,$3,$4,$5", "addu $2,,$3",
            "sll $2,$3,32", "sll $2,$3,-1", "ori $2,$3,65536", "ori $2,$3,-1",
            "addiu $2,$3,-32769", "li $2,0x100000000", "li $2,thing",
            "lw $f2,4($sp)", "lwc1 $2,4($sp)", "lw $2,4($f2)", "lw $2,$f4",
            "move $32,$2", "move $-1,$2", "move $f2,$3", "move $2,sp",
            "mtc1 $2,$4", "mtc1 $f2,$f4", "cfc1 $2,$ra", "mfc0 $2,$f4",
            "add.s $2,$f4,$f6", "add.w $f2,$f4,$f6", "cvt.w.l $f2,$f4",
            "c.eq.s $f2,$f4,$f6", "lw $2,%got(object)($gp)", "lui $2,%lo(object)",
            "lw $2,%lo(object+other)($3)", "j 0x80001234", "beq $2,$3,16",
            "div $2,$3", "div $2,$3,$4", "sw $at,object", "sw $2,object($at)",
            "lw $2,object; nop", "li $2,1 trailing", "LW $2,0($sp)",
        ):
            with self.subTest(assembly=assembly):
                self.assertIsNone(encoding_options(assembly))


if __name__ == "__main__":
    unittest.main()
