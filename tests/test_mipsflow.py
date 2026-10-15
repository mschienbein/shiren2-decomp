"""mipsflow: the shared flow and register-value engine (synthetic functions, no ROM needed)."""
from __future__ import annotations

import struct
import sys
import unittest
from pathlib import Path

PROJECT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(PROJECT / "tools"))
import mipsflow
from mipsflow import ENTRY, ORDINARY, RETURN, SLOT, FlowUnknown

AT, V0, A0, A1, A2, T0, S0, SP, RA = 1, 2, 4, 5, 6, 8, 16, 29, 31
NOP = 0
BASE = 0x80100000  # VRAM of ROM offset 0 in these synthetic functions
TABLE = 0x80000800  # the "vtable" a store should hold
HI, LO = TABLE >> 16, TABLE & 0xFFFF


def i_type(op: int, rs: int, rt: int, imm: int) -> int:
    return (op << 26) | (rs << 21) | (rt << 16) | (imm & 0xFFFF)


def lui(rt: int, imm: int) -> int:
    return i_type(0x0F, 0, rt, imm)


def addiu(rt: int, rs: int, imm: int) -> int:
    return i_type(0x09, rs, rt, imm)


def sw(rt: int, offset: int, base: int) -> int:
    return i_type(0x2B, base, rt, offset)


def beq(rs: int, rt: int, offset: int) -> int:
    return i_type(0x04, rs, rt, offset)


def bgezal(rs: int, offset: int) -> int:
    return i_type(0x01, rs, 0x11, offset)


def j(rom: int) -> int:
    return (0x02 << 26) | (((rom + BASE) >> 2) & 0x3FFFFFF)


def jal(vram: int) -> int:
    return (0x03 << 26) | ((vram >> 2) & 0x3FFFFFF)


def jr(rs: int) -> int:
    return (rs << 21) | 0x08


def jalr(rd: int, rs: int) -> int:
    return (rs << 21) | (rd << 11) | 0x09


def flow(*words: int, vram: bool = True) -> mipsflow.Flow:
    code = struct.pack(f">{len(words)}I", *words)
    return mipsflow.Flow(code + bytes(64), 0, len(code), BASE if vram else None)


def o(pc: int) -> tuple[int, int]:
    return pc, ORDINARY


class GraphTests(unittest.TestCase):
    def test_edges_of_each_transfer(self) -> None:
        f = flow(beq(A0, 0, 2), NOP, addiu(V0, 0, 1), jal(0x80200000), NOP, jr(RA), NOP)
        self.assertEqual(f.succ[o(0)], ((4, SLOT),))
        self.assertEqual(f.succ[4, SLOT], (o(0xC), o(8)))
        self.assertEqual(f.succ[(0x10, SLOT)], ((0xC, RETURN),))
        self.assertEqual(f.succ[0xC, RETURN], (o(0x14),))
        self.assertEqual(f.succ[(0x18, SLOT)], ())
        self.assertTrue(f.dominates(o(0xC), (0xC, RETURN)) and not f.dominates(o(8), o(0xC)))
        # A likely branch skips its slot when not taken; a link branch may call and return or fall through.
        likely = flow(i_type(0x14, A0, 0, 2), addiu(V0, 0, 1), NOP, jr(RA), NOP)
        self.assertEqual(likely.succ[o(0)], ((4, SLOT), o(8)))
        link = flow(bgezal(A1, 0x40), NOP, jr(RA), NOP)
        self.assertEqual(link.succ[4, SLOT], ((0, RETURN), o(8)))
        self.assertTrue(link.writes(o(0), RA) and link.writes((0, RETURN), T0) and not link.writes((0, RETURN), S0))

    def test_switch_dispatch_resolves_every_case(self) -> None:
        words = [i_type(0x0B, A0, V0, 2),   # 00 sltiu v0, a0, 2
                 beq(V0, 0, 9),             # 04 beq v0, zero, 0x2C
                 (A0 << 16) | (V0 << 11) | (2 << 6),  # 08 sll v0, a0, 2 (slot)
                 lui(AT, (BASE + 0x40) >> 16),        # 0C lui at, hi(table)
                 (AT << 21) | (V0 << 16) | (AT << 11) | 0x21,  # 10 addu at, at, v0
                 i_type(0x23, AT, V0, (BASE + 0x40) & 0xFFFF),  # 14 lw v0, lo(table)(at)
                 jr(V0), NOP,               # 18, 1C
                 addiu(V0, 0, 1),           # 20 case 0
                 jr(RA), NOP,               # 24, 28
                 jr(RA), NOP,               # 2C, 30 default / case 1
                 NOP, NOP, NOP]             # 34..3C
        table = struct.pack(">II", BASE + 0x20, BASE + 0x2C)
        code = struct.pack(f">{len(words)}I", *words)
        f = mipsflow.Flow(code + table, 0, len(code), BASE)
        self.assertEqual(f.switches[0x18].targets, (0x20, 0x2C))
        self.assertEqual(f.succ[0x1C, SLOT], (o(0x20), o(0x2C)))
        self.assertEqual(f.reaching(V0, o(0x24)), {o(0x20)})

    def test_bound_that_overwrites_its_index_is_refused(self) -> None:
        # SolReviewAssetStack F2 r2 finding 1: sltiu a1, a1, 1 (2CA50001) turns index 0 into 1, so the
        # dispatch reads table[1] while a bound of 1 derives only table[0].
        def code(bound: int, branch: int) -> bytes:
            words = [bound,                                  # 00 sltiu rB, a1, 1
                     branch,                                 # 04 beq rB, zero, 0x2C
                     (A1 << 16) | (V0 << 11) | (2 << 6),     # 08 sll v0, a1, 2 (slot)
                     lui(AT, (BASE + 0x40) >> 16),           # 0C
                     (AT << 21) | (V0 << 16) | (AT << 11) | 0x21,  # 10 addu at, at, v0
                     i_type(0x23, AT, V0, (BASE + 0x40) & 0xFFFF),  # 14 lw v0, lo(table)(at)
                     jr(V0), NOP,                            # 18, 1C
                     addiu(V0, 0, 1), jr(RA), NOP,           # 20 case 0, 24 jr ra, 28
                     jr(RA), NOP,                            # 2C default, 30
                     addiu(T0, 0, 0), j(0x24), NOP]          # 34 table[1] (never derived), 38, 3C
            return struct.pack(f">{len(words)}I", *words) + struct.pack(">II", BASE + 0x20, BASE + 0x34)

        with self.assertRaisesRegex(FlowUnknown, "not a GCC switch dispatch"):
            mipsflow.Flow(code(0x2CA50001, beq(A1, 0, 9)), 0, 0x40, BASE)
        # The same dispatch with a separate bound register is exhaustive for a bound of 1.
        V1 = 3
        self.assertEqual(mipsflow.Flow(code(i_type(0x0B, A1, V1, 1), beq(V1, 0, 9)), 0, 0x40, BASE).switches[0x18].targets,
                         (0x20,))

    def test_unmodellable_functions_are_refused(self) -> None:
        cases = {
            "jr that is not a switch": (jr(A0), NOP),
            "jalr without a link": (jalr(0, A2), NOP, jr(RA), NOP),
            "eret": (0x42000018, NOP),
            "transfer in a delay slot": (beq(0, 0, 2), jr(RA), NOP, NOP),
            "delay slot past the end": (NOP, jr(RA)),
            "call into the middle of the function": (jal(BASE + 8), NOP, NOP, jr(RA), NOP),
            "link branch into the middle of the function": (bgezal(A1, 1), NOP, NOP, jr(RA), NOP),
        }
        for name, words in cases.items():
            with self.subTest(name), self.assertRaises(FlowUnknown):
                flow(*words)
        with self.assertRaisesRegex(FlowUnknown, "VRAM"):
            flow(j(8), NOP, jr(RA), NOP, vram=False)

    def test_reaching_and_values(self) -> None:
        f = flow(lui(T0, HI), addiu(T0, T0, LO), i_type(0x0D, 0, A0, 0x12), (T0 << 21) | (A1 << 11) | 0x21,
                 sw(T0, 0, A0), jr(RA), NOP)
        self.assertEqual(f.value(T0, o(0x10)), TABLE)
        self.assertEqual(f.value(A0, o(0x10)), 0x12)
        self.assertEqual(f.value(A1, o(0x10)), TABLE)  # addu a1, t0, zero
        self.assertEqual(f.reaching(A2, o(0x10)), {ENTRY})
        self.assertIsNone(f.value(A2, o(0x10)))


class SourceSameTests(unittest.TestCase):
    def lw(self, rt: int, offset: int, base: int) -> int:
        return i_type(0x23, base, rt, offset)

    def test_source_follows_copies_and_displacements(self) -> None:
        f = flow(addiu(A0, SP, 0x10), (A0 << 21) | (A1 << 11) | 0x25, addiu(SP, SP, -0x20), sw(A1, 0, SP),
                 jal(0x80200000), NOP, sw(A1, 0, SP), jr(RA), NOP)
        self.assertEqual(f.source(A1, o(0xC)), (SP, o(0), 0x10))
        self.assertEqual(f.source(SP, o(0xC)), (SP, o(8), 0xFFFFFFE0))
        self.assertEqual(f.source(A1, o(0x18)), (A1, o(0x18), 0))  # the callee may change a1
        self.assertEqual(f.source(V0, o(0xC)), (V0, o(0xC), 0))
        self.assertFalse(f.same(A1, o(0xC), SP, o(0)))
        self.assertTrue(f.same(A1, o(0xC), SP, o(0), 0, 0x10))
        self.assertTrue(f.same(A1, o(0xC), SP, o(0xC), 0, 0x30))  # sp moved by -0x20 in between
        self.assertFalse(f.same(A1, o(0xC), SP, o(0xC), 0, 0x10))
        self.assertFalse(f.same(A1, o(0x18), SP, o(0), 0, 0x10))
        self.assertTrue(f.same(A0, o(0xC), A1, o(0xC)))  # both sp + 0x10

    def test_same_needs_one_run_of_the_definition_between_the_points(self) -> None:
        # 00 lw s0,0(a0); 04 lw v0,0(s0); 08 addiu a0,a0,4; 0C bne a0,a1,0x00; 10 nop; 14 sw s0,0(sp).
        f = flow(self.lw(S0, 0, A0), self.lw(V0, 0, S0), addiu(A0, A0, 4), i_type(0x05, A0, A1, -4), NOP,
                 sw(S0, 0, SP), jr(RA), NOP)
        self.assertTrue(f.same(S0, o(4), S0, o(0x14)))     # the last iteration's s0
        self.assertFalse(f.same(S0, o(0), S0, o(4)))       # the incoming s0 or the previous iteration's
        self.assertFalse(f.same(S0, o(4), A0, o(4)))
        self.assertFalse(f.same(A0, o(4), A0, o(0x14)))    # a0 advances between them


class VtableReviewCounterexampleTests(unittest.TestCase):
    """SolReviewVtableLayer-r2 P2-A..P2-H, built with the reviewer's layouts: the engine's answers must
    not let a store of T be proved where some path stores something else."""

    def test_p2a_skipped_high_word_is_not_a_constant(self) -> None:
        # BEQ a1,zero,+2; nop; LUI at,0x8000; ADDIU at,at,0x800; SW at,0(a0): the branch skips the LUI.
        f = flow(beq(A1, 0, 2), NOP, lui(AT, HI), addiu(AT, AT, LO), sw(AT, 0, A0), jr(RA), NOP)
        self.assertEqual(f.reaching(AT, o(0x10)), {o(0xC)})       # the singleton the r2 prover trusted
        self.assertEqual(f.reaching(AT, o(0xC)), {ENTRY, o(8)})   # but its high word is not on every path
        self.assertIsNone(f.value(AT, o(0x10)))

    def test_p2c_post_call_clobber_is_its_own_definition(self) -> None:
        # LUI t0; JAL helper; slot ADDIU t0,t0,lo; SW t0,0(sp): the helper may change t0.
        f = flow(lui(T0, HI), jal(0x80200000), addiu(T0, T0, LO), sw(T0, 0, SP), jr(RA), NOP)
        self.assertEqual(f.reaching(T0, o(0xC)), {(4, RETURN)})
        self.assertEqual(f.value(T0, f.ret(4)), TABLE)  # what the callee receives
        self.assertIsNone(f.value(T0, o(0xC)))

    def test_p2d_link_branch_returns_with_clobbers(self) -> None:
        f = flow(lui(T0, HI), addiu(T0, T0, LO), bgezal(A1, 0x40), NOP, sw(T0, 0, SP), jr(RA), NOP)
        self.assertEqual(f.reaching(T0, o(0x10)), {o(4), (8, RETURN)})
        self.assertIsNone(f.value(T0, o(0x10)))

    def test_p2e_unresolved_indirect_jump_is_refused(self) -> None:
        with self.assertRaisesRegex(FlowUnknown, "not a GCC switch"):
            flow(lui(T0, HI), addiu(T0, T0, LO), beq(A1, 0, 4), NOP, addiu(T0, 0, 0), jr(A2), NOP,
                 sw(T0, 0, SP), jr(RA), NOP)

    def test_p2f_dead_store_has_no_reaching_definition(self) -> None:
        # J E; nop; LUI t0; ADDIU t0; SW t0,0(sp); E: JR ra; nop.
        f = flow(j(0x14), NOP, lui(T0, HI), addiu(T0, T0, LO), sw(T0, 0, SP), jr(RA), NOP)
        self.assertEqual(f.at(0x10), [])
        self.assertEqual(f.reaching(T0, o(0x10)), frozenset())
        self.assertIsNone(f.value(T0, o(0x10)))
        self.assertFalse(f.dominates(o(0xC), o(0x10)))

    def test_p2g_jalr_zero_is_refused(self) -> None:
        with self.assertRaisesRegex(FlowUnknown, "indirect jump without a link"):
            flow(jalr(0, A2), NOP, lui(S0, HI), addiu(S0, S0, LO), sw(S0, 0, SP), jr(RA), NOP)

    def test_p2h_entered_delay_slot_falls_through(self) -> None:
        # 00 LUI; 04 ADDIU; 08 BEQ a1,zero,S; 0C nop; 10 ADDIU t0,zero,0; 14 BEQ a2,zero,D; 18 nop;
        # 1C J E; 20 D: nop; 24 S: SW t0,0(sp); 28 E: JR ra; 2C nop.
        f = flow(lui(T0, HI), addiu(T0, T0, LO), beq(A1, 0, 6), NOP, addiu(T0, 0, 0), beq(A2, 0, 2), NOP,
                 j(0x28), NOP, sw(T0, 0, SP), jr(RA), NOP)
        self.assertEqual(f.at(0x20), [o(0x20), (0x20, SLOT)])
        self.assertEqual(f.reaching(T0, o(0x24)), {o(4), o(0x10)})
        self.assertIsNone(f.value(T0, o(0x24)))


if __name__ == "__main__":
    unittest.main(verbosity=2)
