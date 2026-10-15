"""g++ 2.x vtable witnesses: classified from the original, never credited (VTABLES-CLASSIFY-NO-CREDIT).

Replay runs against the base partition's cpu_data rows; rommap's witness split (not this module)
carves the replayed intervals out. These tests therefore hold before and after that split: ROM
cases rebuild the base rows by merging cpu_data and vtable rows, and simulate the split locally.
"""
from __future__ import annotations

import collections
import copy
import hashlib
import json
import os
import shutil
import struct
import sys
import tempfile
import unittest
from dataclasses import replace
from pathlib import Path
from unittest.mock import patch

PROJECT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(PROJECT / "tools"))
import gxxvtable  # noqa: E402
import mipsflow  # noqa: E402
import rommap  # noqa: E402
from images import load_inventory  # noqa: E402

PRIVATE_ROM = Path(os.environ.get("H21_TEST_ROM", str(PROJECT.parent / "inputs/r1-build/shiren2.z64")))
TARGET = "f" * 64
MAIN, OVA, OVB = "main_14400", "overlay_a", "overlay_b"


def lui(rt: int, imm: int) -> int:
    return 0x3C000000 | rt << 16 | imm


def addiu(rt: int, rs: int, imm: int) -> int:
    return 0x24000000 | rs << 21 | rt << 16 | imm & 0xFFFF


def sw(rt: int, base: int) -> int:
    return 0xAC000000 | base << 21 | rt << 16


def split(ranges, vtables: dict) -> list:
    """Local stand-in for the witness split: carve replayed intervals out of their cpu_data rows."""
    rows = []
    for start, end, cls, ref in ranges:
        inside = sorted((a, b, i) for (a, b), i in vtables.items() if start <= a < b <= end)
        if cls != "cpu_data" or not inside:
            rows.append((start, end, cls, ref))
            continue
        cursor = start
        for a, b, table in inside:
            rows += [(cursor, a, cls, ref)] if a > cursor else []
            rows.append((a, b, gxxvtable.CLASS, table))
            cursor = b
        rows += [(cursor, end, cls, ref)] if cursor < end else []
    return rows


def lw(rt: int, base: int, offset: int = 0) -> int:
    return 0x8C000000 | base << 21 | rt << 16 | offset & 0xFFFF


def move(rd: int, rs: int) -> int:
    return rs << 21 | rd << 11 | 0x2D  # daddu rd, rs, zero


def jal(vram: int) -> int:
    return 0x0C000000 | (vram >> 2) & 0x3FFFFFF


def beq(rs: int, rt: int, words: int) -> int:
    return 0x10000000 | rs << 21 | rt << 16 | words & 0xFFFF


def bgezal(rs: int, words: int) -> int:
    return 0x04110000 | rs << 21 | words & 0xFFFF


def j(vram: int) -> int:
    return 0x08000000 | (vram >> 2) & 0x3FFFFFF


def jr(rs: int) -> int:
    return rs << 21 | 0x08


def jalr(rd: int, rs: int) -> int:
    return rs << 21 | rd << 11 | 0x09


def sw_at(rt: int, offset: int, base: int) -> int:
    return 0xAC000000 | base << 21 | rt << 16 | offset & 0xFFFF


AT, A0, A1, A2, T0, T1, S0, S1, SP, RA = 1, 4, 5, 6, 8, 9, 16, 17, 29, 31
CTOR = 0x800001C0  # VRAM of `ctor` (ROM 0x1C0)

# `install` at ROM 0x140 (vram 0x80000140):
#   0x150 A into this+0; 0x15C C into this+8; then ctor(a1) and 0x174 B into a1+4, bound by ctor's
#   own receiver install at member 4.
INSTALL = [0, lui(AT, 0x8000), addiu(AT, AT, 0x800), 0, sw(AT, A0),
           lui(T1, 0x8000), addiu(T1, T1, 0x840), sw_at(T1, 8, A0),
           move(S0, A1), jal(CTOR), move(A0, S0),
           lui(T0, 0x8000), addiu(T0, T0, 0x820), sw_at(T0, 4, S0), jr(RA), 0]
# `ctor` at ROM 0x1C0: A into this+4 from the delay slot of its return.
CTOR_WORDS = [lui(AT, 0x8000), addiu(AT, AT, 0x800), jr(RA), sw_at(AT, 4, A0), 0, 0, 0, 0]


class Synthetic:
    """A three-image original: resident main plus two overlays sharing one vram window.

    main 0x800 A = {0,0,0} {0,0,f0} {0,0,f1}; 8 zero bytes; 0x820 B = {0,0,0} {0,0,f1}; 16 zero
    bytes; 0x840 C = {-8,0,0} {-8,0,f0} {0,0,f1}; zeros. overlay_a 0x1800 D = {0,0,0} {0,0,oa}
    {0,0,f0}. `install` and `ctor` install A, B and C (receiver installs and one bound install);
    `oa` installs D into this."""

    def __init__(self, install: list[int] | None = None) -> None:
        self.original = bytearray(0x3000)
        self.images = [{"image_id": MAIN, "rom_start": 0, "rom_end": 0x1000, "vram_start": 0x80000000},
                       {"image_id": OVA, "rom_start": 0x1000, "rom_end": 0x2000, "vram_start": 0x80100000},
                       {"image_id": OVB, "rom_start": 0x2000, "rom_end": 0x3000, "vram_start": 0x80100000}]
        self.functions = []
        self.function(MAIN, 0x100, "f0", [0] * 8)
        self.function(MAIN, 0x120, "f1", [0] * 8)
        self.function(MAIN, 0x140, "install", install or INSTALL)
        self.function(MAIN, 0x1C0, "ctor", CTOR_WORDS)
        self.function(OVA, 0x1100, "oa", [lui(1, 0x8010), addiu(1, 1, 0x800), sw(1, 4)] + [0] * 5)
        self.function(OVB, 0x2080, "ob", [0] * 8)
        self.entries(0x800, [(0, 0), (0, 0x80000100), (0, 0x80000120)])
        self.entries(0x820, [(0, 0), (0, 0x80000120)])
        self.entries(0x840, [(-8, 0), (-8, 0x80000100), (0, 0x80000120)])
        self.entries(0x1800, [(0, 0), (0, 0x80100100), (0, 0x80000100)])
        self.rows = [[0x800, 0x900, "cpu_data", MAIN], [0x1800, 0x1900, "cpu_data", OVA]]

    def function(self, image_id: str, rom_start: int, symbol: str, words: list[int]) -> None:
        image = next(i for i in self.images if i["image_id"] == image_id)
        for k, word in enumerate(words):
            struct.pack_into(">I", self.original, rom_start + 4 * k, word)
        self.functions.append({"image_id": image_id, "symbol": symbol, "rom_start": rom_start, "size": 4 * len(words),
                               "vram_start": image["vram_start"] + rom_start - image["rom_start"]})

    def entries(self, at: int, rows: list[tuple[int, int]]) -> None:
        for k, (delta, pfn) in enumerate(rows):
            struct.pack_into(">hhI", self.original, at + 8 * k, delta, 0, pfn)

    @property
    def catalogue(self) -> dict:
        return {"functions": self.functions, "images": self.images}

    def scan(self) -> list[dict]:
        return gxxvtable.scan(bytes(self.original), self.catalogue, self.rows)

    def installs(self, start: int = 0x800, symbol: str = "install") -> list[list]:
        """The proven installs of the table at `start` that `symbol` performs."""
        row = next((r for r in self.scan() if r["start"] == start), None)
        return [i for i in row["installs"] if i[2] == symbol] if row else []

    def document(self) -> dict:
        return {"schema": gxxvtable.SCHEMA, "target_sha256": TARGET, "vtables": self.scan()}

    def replay(self, document: dict, original: bytes | None = None) -> dict:
        return gxxvtable.replay(bytes(original if original is not None else self.original), document,
                                self.catalogue, self.rows, TARGET)


def body(*words: int) -> list[int]:
    """An `install` function body, padded to 16 words."""
    return list(words) + [0] * (16 - len(words))


class SyntheticWitnessTests(unittest.TestCase):
    def setUp(self) -> None:
        self.case = Synthetic()
        self.document = self.case.document()

    def rows(self) -> dict:
        return {row["id"]: row for row in self.document["vtables"]}

    def assertRejected(self, document: dict, original: bytes | None = None, message: str = "") -> None:
        with self.assertRaisesRegex(gxxvtable.VtableError, message):
            self.case.replay(document, original)

    def test_positive_layout_targets_and_extent(self) -> None:
        rows = self.rows()
        self.assertEqual(self.case.replay(self.document), {(0x800, 0x818): "VT_main_14400_800",
                                                           (0x820, 0x830): "VT_main_14400_820",
                                                           (0x840, 0x858): "VT_main_14400_840",
                                                           (0x1800, 0x1818): "VT_overlay_a_1800"})
        self.assertEqual(rows["VT_main_14400_800"]["slots"], [[0, "f0"], [0, "f1"]])
        self.assertEqual(rows["VT_main_14400_840"]["header_delta"], -8)
        self.assertEqual(rows["VT_main_14400_840"]["slots"], [[-8, "f0"], [0, "f1"]])
        self.assertEqual(rows["VT_overlay_a_1800"]["slots"], [[0, "oa"], [0, "f0"]])
        self.assertEqual(rows["VT_main_14400_800"]["start_refs"], [[0x148, "install"], [0x1C4, "ctor"]])
        self.assertEqual([r["installs"] for r in rows.values()], [
            [[0x150, "ordinary", "install", 0, "this", None], [0x1CC, "slot", "ctor", 4, "this", None]],
            [[0x174, "ordinary", "install", 4, "argument", [0x164, "ctor"]]],
            [[0x15C, "ordinary", "install", 8, "this", None]],
            [[0x1108, "ordinary", "oa", 0, "this", None]]])
        self.assertEqual({row["bound"] for row in rows.values()}, {"entry"})
        self.assertEqual(split([tuple(r) for r in self.case.rows], self.case.replay(self.document))[:4], [
            (0x800, 0x818, gxxvtable.CLASS, "VT_main_14400_800"), (0x818, 0x820, "cpu_data", MAIN),
            (0x820, 0x830, gxxvtable.CLASS, "VT_main_14400_820"), (0x830, 0x840, "cpu_data", MAIN)])

    def test_zero_fill_and_zero_only_blocks_never_qualify(self) -> None:
        # The 8 zero bytes after A and a code-referenced all-zero block are not tables.
        self.case.function(MAIN, 0x1E0, "zero_install", [lui(1, 0x8000), addiu(1, 1, 0x880), sw(1, 4), 0])
        self.assertNotIn(0x880, {row["start"] for row in self.case.scan()})
        with self.assertRaisesRegex(gxxvtable.VtableError, "no slot"):
            gxxvtable.parse(bytes(self.case.original), MAIN, 0x880, 0x900, 0x80000000,
                            gxxvtable._function_starts(self.case.functions), 0x900)
        for start, end, position in ((0x818, 0x820, 1), (0x880, 0x888, 3)):
            document = copy.deepcopy(self.document)
            document["vtables"].insert(position, {
                "id": gxxvtable.table_id(MAIN, start), "image_id": MAIN, "start": start, "end": end,
                "header_delta": 0, "slots": [], "bound": "entry", "start_refs": [], "installs": [],
                "sha256": hashlib.sha256(bytes(8)).hexdigest()})
            with self.subTest(start=hex(start)):
                self.assertRejected(document)

    def test_non_function_slot_fails(self) -> None:
        cases = {"mid-function": 0x80000104, "data in the gap": 0x80000880, "uncatalogued": 0x80000200,
                 "other overlay function": None, "null": 0}
        for name, pfn in cases.items():
            original = bytearray(self.case.original)
            if pfn is None:
                struct.pack_into(">I", original, 0x1814, 0x80100080)  # D slot 2 -> ob (overlay_b only)
            else:
                struct.pack_into(">I", original, 0x814, pfn)  # A slot 2
            with self.subTest(name=name):
                self.assertRejected(self.document, original, "extent/layout disagrees")
        document = copy.deepcopy(self.document)
        document["vtables"][0]["slots"][1][1] = "f0"
        self.assertRejected(document, message="extent/layout disagrees")

    def test_overlay_slot_resolves_only_within_its_own_image_or_resident(self) -> None:
        self.case.function(OVA, 0x1080, "oa_twin", [0] * 8)
        struct.pack_into(">I", self.case.original, 0x1814, 0x80100080)
        row = next(r for r in self.case.scan() if r["image_id"] == OVA)
        self.assertEqual(row["slots"], [[0, "oa"], [0, "oa_twin"]])

    def test_tampered_layout_fails(self) -> None:
        tampers = {"slot index half": (0x80A, ">h", 1), "header pfn": (0x804, ">I", 0x80000100),
                   "header index": (0x802, ">h", 2), "positive header": (0x800, ">h", 8),
                   "slot delta outside header..0": (0x808, ">h", 4), "unaligned slot delta": (0x848, ">h", -6)}
        for name, (at, fmt, value) in tampers.items():
            original = bytearray(self.case.original)
            struct.pack_into(fmt, original, at, value)
            with self.subTest(name=name):
                self.assertRejected(self.document, original)
        edits = {"header_delta": lambda r: r.update(header_delta=-8), "slots": lambda r: r["slots"].pop(),
                 "bound": lambda r: r.update(bound="gap_end"), "sha256": lambda r: r.update(sha256="0" * 64),
                 "start_refs": lambda r: r.update(start_refs=[[0x14C, "install"]]),
                 "install member": lambda r: r["installs"][0].__setitem__(3, 4),
                 "install context": lambda r: r["installs"][0].__setitem__(1, "slot"),
                 "install origin": lambda r: r["installs"][0].__setitem__(4, "stack"),
                 "install dropped": lambda r: r["installs"].pop(),
                 "no installs": lambda r: r.update(installs=[]),
                 "misaligned start": lambda r: r.update(start=0x804, id=gxxvtable.table_id(MAIN, 0x804)),
                 "id": lambda r: r.update(id="VT_main_14400_801"), "image": lambda r: r.update(image_id=OVA),
                 "extra field": lambda r: r.update(owner="src/x.c")}
        for name, edit in edits.items():
            document = copy.deepcopy(self.document)
            edit(document["vtables"][0])
            with self.subTest(name=name):
                self.assertRejected(document)
        document = copy.deepcopy(self.document)
        document["vtables"][1]["installs"][0][5] = [0x164, "f0"]
        self.assertRejected(document, message="proven vptr installs")
        for key, value in (("schema", "other"), ("target_sha256", "0" * 64), ("vtables", {})):
            document = copy.deepcopy(self.document)
            document[key] = value
            with self.subTest(key=key):
                self.assertRejected(document, message="fixed target")
        # The materialization is the start's code anchor: without it nothing replays.
        original = bytearray(self.case.original)
        struct.pack_into(">I", original, 0x148, addiu(1, 1, 0x808))
        self.assertRejected(self.document, original, "code-materialized")

    def install_variant(self, words: dict[int, int]) -> Synthetic:
        install = list(INSTALL)
        for index, word in words.items():
            install[index] = word
        return Synthetic(install)

    def test_store_removed_unrelated_base_or_redefined_register_is_no_install(self) -> None:
        """SolReviewVtableLayer P2: a materialization alone, without a proven vptr store, anchors nothing."""
        variants = {
            "store removed": {4: 0},
            "store through an unrelated (unset) register": {4: sw(AT, 10)},
            "store through a pointer loaded from memory": {0: lw(10, A1), 4: sw(AT, 10)},
            "register redefined before the store": {3: addiu(AT, AT, 8)},
            "register redefined by a load before the store": {3: lw(AT, A1)},
        }
        for name, words in variants.items():
            case = self.install_variant(words)
            with self.subTest(name=name):
                self.assertEqual(case.installs(), [])
                self.assertEqual(case.installs(symbol="ctor"), [[0x1CC, "slot", "ctor", 4, "this", None]])
                with self.assertRaisesRegex(gxxvtable.VtableError, "proven vptr installs"):
                    case.replay(self.document)
        # Without ctor's own receiver install (its sw removed) A, B and the binding all go.
        case = Synthetic()
        case.function(MAIN, 0x1C0, "ctor", CTOR_WORDS[:3] + [0] * 5)
        self.assertEqual([r["start"] for r in case.scan()], [0x800, 0x840, 0x1800])
        self.assertEqual(case.installs(0x820), [])

    def test_receiver_installs_through_copies_and_displacements(self) -> None:
        cases = {
            "this copied to s0, store in a call's delay slot": (
                body(move(S0, A0), lui(AT, 0x8000), addiu(AT, AT, 0x800), jal(0x80000100), sw(AT, S0)),
                [[0x150, "slot", "install", 0, "this", None]]),
            "materialized in a call's delay slot, stored after the call (the callee's clobber)": (
                body(move(S0, A0), lui(AT, 0x8000), jal(0x80000100), addiu(AT, AT, 0x800), 0, sw(AT, S0)), None),
            "store in the delay slot of a call that receives the object": (
                body(lui(AT, 0x8000), addiu(AT, AT, 0x800), jal(0x80000100), sw(AT, A0)),
                [[0x14C, "slot", "install", 0, "this", None]]),
            "subobject: a displaced copy of this": (
                body(addiu(S0, A0, 16), lui(AT, 0x8000), addiu(AT, AT, 0x800), sw_at(AT, 4, S0), jr(RA), 0),
                [[0x14C, "ordinary", "install", 20, "this", None]]),
        }
        for name, (words, expected) in cases.items():
            with self.subTest(name=name):
                case = Synthetic(words)
                if expected is None:  # the call clobbers at: the value at the sw is the callee's
                    self.assertEqual(case.installs(), [])
                else:
                    self.assertEqual(case.installs(), expected)
        # A negative member is no member of the received object.
        case = Synthetic(body(lui(AT, 0x8000), addiu(AT, AT, 0x800), sw_at(AT, -4, A0), jr(RA), 0))
        self.assertEqual(case.installs(), [])

    # -- SolReviewVtableLayer-r2 P2-A..P2-H: one negative per counterexample (the reviewer's words) --
    # Each places the counterexample in `install` with a receiver base (a0, or s0 = a0), so only the
    # finding under test can refuse it, and checks a positive control without the defect.

    def assertNoInstall(self, words: list[int], control: list[int], store: int) -> None:
        case = Synthetic(body(*words))
        self.assertEqual([i for i in case.installs() if i[0] == store], [])
        self.assertEqual([i[0] for i in Synthetic(body(*control)).installs()], [store])

    def test_p2a_incoming_high_word_must_be_proved(self) -> None:
        """P2-A: "At Synthetic ROM 0x140..0x150 use BEQ a1,zero,+2; nop; LUI at,0x8000; ADDIU
        at,at,0x800; SW at,0(a0). The taken branch jumps to 0x14C and skips the LUI at 0x148. ...
        That path can store an unknown/non-table value. Require all-path source/high-word value
        provenance, including call clobbers, before treating ADDIU/ORI as a table materialization." """
        self.assertNoInstall([beq(A1, 0, 2), 0, lui(AT, 0x8000), addiu(AT, AT, 0x800), sw(AT, A0)],
                             [0, 0, lui(AT, 0x8000), addiu(AT, AT, 0x800), sw(AT, A0)], 0x150)
        # Including call clobbers: the high word comes from before a call, the low word after it.
        self.assertNoInstall([move(S0, A0), lui(AT, 0x8000), jal(0x80000100), 0, addiu(AT, AT, 0x800), sw(AT, S0)],
                             [move(S0, A0), jal(0x80000100), 0, lui(AT, 0x8000), addiu(AT, AT, 0x800), sw(AT, S0)],
                             0x154)

    def test_p2b_stack_static_and_abi_bases_need_member_evidence(self) -> None:
        """P2-B: "Change only the LUI high word in the new static positive fixture from 0x8000 to
        0xA430 ...: its accepted install now targets 0xA43008F8 ... A spill/outgoing-argument write
        also satisfies the SP shortcut. Require positively identified object/member destinations, not
        a register-origin tag. a1-a3/v0 tags likewise identify ABI locations, not pointer/class types
        by themselves." """
        table = [lui(AT, 0x8000), addiu(AT, AT, 0x800)]
        unbound = {
            "static (r2 fixture)": table + [lui(11, 0x8000), sw_at(AT, 0x8F8, 11)],
            "static MMIO (0xA430)": table + [lui(11, 0xA430), sw_at(AT, 0x8F8, 11)],
            "stack spill / outgoing argument": table + [sw_at(AT, 0x18, SP)],
            "pointer argument a1": table + [sw(AT, A1)],
            "call result v0": [jal(0x80000100), 0] + table + [sw(AT, 2)],
        }
        for name, words in unbound.items():
            with self.subTest(name=name):
                self.assertEqual(Synthetic(body(*words)).installs(), [])
        # Member evidence: the same object is ctor's receiver, and ctor installs a table at member 4.
        bound = {
            "static object passed to ctor": (
                [lui(S0, 0x8000), addiu(S0, S0, 0x8F0), jal(CTOR), move(A0, S0)] + table + [sw_at(AT, 4, S0)],
                [[0x158, "ordinary", "install", 4, "static", [0x148, "ctor"]]]),
            "stack object passed to ctor": (
                [addiu(SP, SP, -0x20), jal(CTOR), addiu(A0, SP, 0x10)] + table + [sw_at(AT, 0x14, SP)],
                [[0x154, "ordinary", "install", 4, "stack", [0x144, "ctor"]]]),
            "call result passed to ctor": (
                [jal(0x80000100), 0, move(S0, 2), jal(CTOR), move(A0, S0)] + table + [sw_at(AT, 4, S0)],
                [[0x15C, "ordinary", "install", 4, "call_result", [0x14C, "ctor"]]]),
        }
        for name, (words, expected) in bound.items():
            with self.subTest(name=name):
                self.assertEqual(Synthetic(body(*words)).installs(), expected)
        refused = {
            "MMIO object passed to ctor": [lui(S0, 0xA430), jal(CTOR), move(A0, S0)] + table + [sw_at(AT, 4, S0)],
            "another member than ctor's": [lui(S0, 0x8000), addiu(S0, S0, 0x8F0), jal(CTOR), move(A0, S0)] + table
                                          + [sw_at(AT, 8, S0)],
            "another stack object": [addiu(SP, SP, -0x20), jal(CTOR), addiu(A0, SP, 0x10)] + table
                                    + [sw_at(AT, 0x18, SP)],
            "call and store on different paths": [beq(A2, 0, 3), addiu(A0, SP, 0x10), jal(CTOR), 0] + table
                                                 + [sw_at(AT, 0x14, SP)],
            "callee without a receiver install": [jal(0x80000100), addiu(A0, SP, 0x10)] + table
                                                 + [sw_at(AT, 0x14, SP)],
        }
        for name, words in refused.items():
            with self.subTest(name=name):
                self.assertEqual(Synthetic(body(*words)).installs(), [])

    def test_p2c_call_slot_materialization_is_not_the_callee_clobber(self) -> None:
        """P2-C: "For LUI t0,%hi(T); JAL helper; m: ADDIU t0,t0,%lo(T) in the delay slot; SW t0,0(sp),
        let helper overwrite t0 and return. defines(m,t0) collapses the instruction's pre-call write
        and the callee's later clobber into the same PC. ... Give the post-call clobber a distinct
        definition/node." """
        self.assertNoInstall([move(S0, A0), lui(T0, 0x8000), jal(0x80000100), addiu(T0, T0, 0x800), sw(T0, S0)],
                             [move(S0, A0), lui(T0, 0x8000), jal(0x80000100), addiu(S1, T0, 0x800), sw(S1, S0)],
                             0x150)

    def test_p2d_branch_and_link_returns_with_clobbers(self) -> None:
        """P2-D: "BLTZAL/BGEZAL and likely link variants are dispatched as branches and never mark a
        returning call clobber. Materialize T in t0, then BGEZAL a1,helper; nop; SW t0,0(sp). A taken
        external helper can overwrite t0 and return to the store." """
        self.assertNoInstall([move(S0, A0), lui(T0, 0x8000), addiu(T0, T0, 0x800), bgezal(A1, 0x40), 0, sw(T0, S0)],
                             [move(S0, A0), lui(T0, 0x8000), addiu(T0, T0, 0x800), beq(A1, 0, 1), 0, sw(T0, S0)],
                             0x154)

    def test_p2e_unresolved_indirect_jump_targets_fail_closed(self) -> None:
        """P2-E: "A runtime argument/computed JR target need not appear in any scanned absolute
        code/data word. Use LUI t0,%hi(T); m:ADDIU t0,t0,%lo(T); BEQ a1,zero,S; nop; ADDIU t0,zero,0;
        JR a2; nop; S:SW t0,0(sp). Let S be absent from absolute references, a1 nonzero and a2=S. The
        actual path stores zero." """
        self.assertNoInstall([lui(T0, 0x8000), addiu(T0, T0, 0x800), beq(A1, 0, 4), 0, addiu(T0, 0, 0), jr(A2), 0,
                              sw(T0, A0), jr(RA), 0],
                             [lui(T0, 0x8000), addiu(T0, T0, 0x800), beq(A1, 0, 4), 0, addiu(T0, 0, 0), jr(RA), 0,
                              sw(T0, A0), jr(RA), 0], 0x15C)

    def test_p2f_dead_installs_are_not_witnesses(self) -> None:
        """P2-F: "For J E; nop; LUI t0,%hi(T); m:ADDIU t0,t0,%lo(T); SW t0,0(sp); E:JR ra; nop, the
        candidate and store are dead. ... Require forward entry reachability for witness sites before
        accepting their singleton proof." """
        self.assertNoInstall([j(0x80000154), 0, lui(T0, 0x8000), addiu(T0, T0, 0x800), sw(T0, A0), jr(RA), 0],
                             [beq(A1, 0, 4), 0, lui(T0, 0x8000), addiu(T0, T0, 0x800), sw(T0, A0), jr(RA), 0], 0x150)

    def test_p2g_zero_link_jalr_is_an_indirect_jump(self) -> None:
        """P2-G: "The decoder does not inspect JALR's rd. JALR zero,a2; nop; LUI s0,%hi(T); m:ADDIU
        s0,s0,%lo(T); S:SW s0,0(sp), with a2=S, establishes no link and bypasses m. ... Decode rd=0 as
        an indirect jump/return with target semantics, not a returning call." """
        self.assertNoInstall([jalr(0, A2), 0, lui(S0, 0x8000), addiu(S0, S0, 0x800), sw(S0, A0), jr(RA), 0],
                             [0, 0, lui(S0, 0x8000), addiu(S0, S0, 0x800), sw(S0, A0), jr(RA), 0], 0x150)
        with self.assertRaises(mipsflow.FlowUnknown):
            mipsflow.Flow(struct.pack(">4I", jalr(0, A2), 0, jr(RA), 0), 0, 16, 0x80000000)

    def test_p2h_ordinary_entry_into_a_delay_slot_falls_through(self) -> None:
        """P2-H: "Function-relative layout: 00 LUI t0,%hi(T); 04 m:ADDIU t0,t0,%lo(T); 08 BEQ
        a1,zero,S; 0C nop; 10 ADDIU t0,zero,0; 14 BEQ a2,zero,D; 18 nop; 1C J E; 20 D:nop; 24
        S:SW t0,0(sp); 28 E:JR ra; 2C nop. With a1 nonzero/a2 zero, execution enters D normally, falls
        through and stores zero at S." """
        words = [lui(T0, 0x8000), addiu(T0, T0, 0x800), beq(A1, 0, 6), 0, addiu(T0, 0, 0), beq(A2, 0, 2), 0,
                 j(0x80000168), 0, sw(T0, A0), jr(RA), 0]
        control = list(words)
        control[4] = addiu(17, 0, 0)  # the zero goes to s1, not t0
        self.assertNoInstall(words, control, 0x164)
        flow = gxxvtable._Function(bytes(Synthetic(body(*words)).original), Synthetic().functions[2], {}).flow
        self.assertIn((0x160, mipsflow.ORDINARY), flow.reachable)  # D entered normally
        self.assertIn((0x164, mipsflow.ORDINARY), flow.succ[(0x160, mipsflow.ORDINARY)])

    def test_extent_overrunning_a_neighbouring_object_fails(self) -> None:
        for end in (0x820, 0x830):  # A over its zero fill; A through B's start
            document = copy.deepcopy(self.document)
            row = document["vtables"][0]
            row.update(end=end, slots=row["slots"] + [[0, "f1"]] * ((end - 0x818) // 8),
                       sha256=hashlib.sha256(bytes(self.case.original[0x800:end])).hexdigest())
            del document["vtables"][1]
            with self.subTest(end=hex(end)):
                self.assertRejected(document)
        # A data word naming A+0x10 makes it a separate labelled object: A's pinned extent overruns it.
        original = bytearray(self.case.original)
        struct.pack_into(">I", original, 0x8F0, 0x80000810)
        self.assertRejected(self.document, original, "extent/layout disagrees")
        self.case.original = original
        row = self.case.scan()[0]
        self.assertEqual((row["end"], row["bound"], row["slots"]), (0x810, "reference", [[0, "f0"]]))
        # Overlapping or unordered rows, a table crossing its gap end and a non-cpu_data parent fail.
        document = copy.deepcopy(self.document)
        document["vtables"][0:2] = document["vtables"][1::-1]
        self.assertRejected(document, message="overlap or are unordered")
        self.case.original = bytearray(Synthetic().original)
        self.case.rows = [[0x800, 0x810, "cpu_data", MAIN], [0x810, 0x900, "cpu_data", MAIN],
                          [0x1800, 0x1900, "cpu_data", OVA]]
        self.assertRejected(self.document)
        self.assertEqual(self.case.scan()[0]["bound"], "gap_end")
        self.case.rows = [[0x800, 0x900, "image_opaque", MAIN], [0x1800, 0x1900, "cpu_data", OVA]]
        self.assertRejected(self.document, message="not inside a cpu_data gap")
        self.case.rows = [[0x800, 0x900, "cpu_data", "catalogue"]]
        self.assertRejected(self.document, message="does not name its image")


@unittest.skipUnless(PRIVATE_ROM.is_file(), "Private copied original required")
class RomWitnessTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls) -> None:
        cls.original = PRIVATE_ROM.read_bytes()
        cls.inventory = load_inventory(PROJECT / "config/images.json")
        cls.rules_bytes = (PROJECT / rommap.RULES_PATH).read_bytes()
        cls.rules = json.loads(cls.rules_bytes)
        cls.document = json.loads((PROJECT / rommap.MAP_PATH).read_text())
        cls.rom_map = rommap.load(PROJECT, cls.original)
        cls.evidence = rommap.load_evidence(cls.original, cls.rules)
        cls.catalogue = cls.document["cpu_catalogue"]
        # The base partition before every witness split (vtables and cpu-data padding pieces restored).
        cls.base = [tuple(row) for row in rommap.unsplit_rows(cls.document["ranges"], cls.document["splits"])]
        cls.vtables = rommap.cpu_vtables(cls.original, cls.catalogue, cls.evidence, cls.base)
        cls.split_map = replace(cls.rom_map, ranges=tuple(split(cls.base, cls.vtables)))

    def replay(self, document: dict, original: bytes | None = None) -> dict:
        return gxxvtable.replay(original or self.original, document, self.catalogue, self.base, rommap.EXPECTED_SHA256)

    def validate(self, document: dict) -> rommap.RomMap:
        return rommap.validate(document, self.original, self.inventory, self.rules,
                               rules_sha256=hashlib.sha256(self.rules_bytes).hexdigest())

    def test_pinned_tables_replay_against_the_base_cpu_data_rows(self) -> None:
        pinned = self.evidence[gxxvtable.INPUT]["vtables"]
        self.assertEqual(self.vtables, {(row["start"], row["end"]): row["id"] for row in pinned})
        self.assertEqual(len(self.vtables), 408)
        self.assertEqual(sum(b - a for a, b in self.vtables), 38168)
        self.assertEqual([r[:2] for r in self.base if r[2] == "cpu_data"],
                         [(0x12340, 0x14400), (0x10BB10, 0x11D0A0), (0x11D770, 0x1339F0),
                          (0x136380, 0x136DC0), (0x147030, 0x148460)])
        self.assertEqual(sum(e - s for s, e, *_ in self.split_map.ranges), sum(e - s for s, e, *_ in self.rom_map.ranges))
        self.assertEqual(rommap.research_status(gxxvtable.CLASS), "classified_binary")
        self.assertEqual(rommap.CLASSES[gxxvtable.CLASS], "cpu_data")
        with tempfile.TemporaryDirectory() as temporary:  # the validator's bytes are part of the policy
            changed = Path(temporary) / "gxxvtable.py"
            changed.write_bytes(Path(gxxvtable.__file__).read_bytes() + b"\n# changed validator\n")
            with patch.object(gxxvtable, "__file__", str(changed)):
                document = rommap._document(self.original, self.document["ranges"],
                                            rules_sha256=self.document["rules_sha256"],
                                            catalogue_id=self.document["cpu_catalogue_id"],
                                            catalogue=self.document["cpu_catalogue"])
        self.assertEqual(document["partition_sha256"], self.document["partition_sha256"])
        self.assertNotEqual(document["evidence_policy_sha256"], self.document["evidence_policy_sha256"])

    def test_every_slot_is_a_co_resident_catalogued_function_start(self) -> None:
        by_symbol = {f["symbol"]: f for f in self.catalogue["functions"]}
        images = {i["image_id"]: i for i in self.catalogue["images"]}
        rows = self.evidence[gxxvtable.INPUT]["vtables"]
        for row in rows:
            image = images[row["image_id"]]
            self.assertEqual(struct.unpack_from(">hhI", self.original, row["start"]), (row["header_delta"], 0, 0))
            self.assertEqual((row["start"] + image["vram_start"] - image["rom_start"]) % 8, 0)
            self.assertEqual(row["end"] - row["start"], 8 * (len(row["slots"]) + 1))
            for k, (delta, symbol) in enumerate(row["slots"], 1):
                function = by_symbol[symbol]
                self.assertIn(function["image_id"], gxxvtable.visible(row["image_id"]))
                self.assertTrue(row["header_delta"] <= delta <= 0 and not delta % 4)
                self.assertEqual(struct.unpack_from(">hhI", self.original, row["start"] + 8 * k),
                                 (delta, 0, function["vram_start"]))
        self.assertTrue(any(s == "func_800D8F80" for row in rows for _, s in row["slots"]))  # __pure_virtual
        offset = [row for row in rows if row["header_delta"] == -132]
        self.assertTrue(offset and any(d for row in offset for d, _ in row["slots"]))
        self.assertEqual({row["image_id"] for row in rows}, {"main_14400", "overlay_1339f0", "overlay_136dc0"})

    def test_every_pinned_start_is_a_proven_vptr_install(self) -> None:
        rows = self.evidence[gxxvtable.INPUT]["vtables"]
        by_symbol = {f["symbol"]: f for f in self.catalogue["functions"]}
        origins = collections.Counter()
        for row in rows:
            self.assertTrue(row["installs"], row["id"])
            for store, context, symbol, member, origin, binding in row["installs"]:
                word = struct.unpack_from(">I", self.original, store)[0]
                low = by_symbol[symbol]["rom_start"]
                self.assertIn(symbol, {s for _, s in row["start_refs"]})
                self.assertTrue(low <= store < low + by_symbol[symbol]["size"])
                self.assertEqual(word >> 26, 0x2B)  # sw rt, off(base)
                self.assertIn(context, ("ordinary", "slot"))
                self.assertTrue(member >= 0 and not member % 4)
                self.assertEqual(binding is None, origin == "this")
                origins[origin] += 1
        self.assertEqual(origins, {"this": 974, "stack": 106, "call_result": 31, "static": 25, "merged": 22})
        # func_8011D340: `sw v0, 8(s0)` at 0xF03A0 with s0 = this.
        first = next(r for r in rows if r["id"] == "VT_main_14400_11bfd0")
        self.assertEqual(first["installs"], [[0xF03A0, "ordinary", "func_8011D340", 8, "this", None]])
        # The static widget D_801425F0 (SolReviewVtableLayer-r2: func_8009EEB0.c, +0x4C field) is passed to
        # func_800953C0, whose own receiver install writes a vtable at member 0x4C.
        widget = next(r for r in rows if r["id"] == "VT_main_14400_11ce90")
        self.assertEqual(widget["installs"],
                         [[0x71F14, "ordinary", "func_8009EEB0", 0x4C, "static", [0x71F04, "func_800953C0"]]])
        self.assertIn([0x4C, "this"], [[i[3], i[4]] for r in rows for i in r["installs"] if i[2] == "func_800953C0"])

    def test_excluded_candidates_and_fills_stay_open(self) -> None:
        pinned = {a for a, _ in self.vtables}
        for start in (0x12C190, 0x12BEC0, 0x1335A8):  # slot targets matched since b9 (r6): now pinned
            self.assertIn(start, pinned)
        for start in (0x11CF20, 0x11CF70):  # static MenuCursor objects: no proven member (no binding call)
            self.assertNotIn(start, pinned)
            self.assertEqual(next(r for r in self.split_map.ranges if r[0] <= start < r[1])[2], "cpu_data")
        vtables = self.split_map.of_class(gxxvtable.CLASS)
        fills = [(a[1], b[0]) for a, b in zip(vtables, vtables[1:])
                 if a[1] < b[0] and not self.original[a[1]:b[0]].strip(b"\0")]
        self.assertEqual(self.evidence[gxxvtable.INPUT]["open_residue"]["inter_table_zero_fill"]["pieces"], len(fills))
        self.assertTrue(all((*fill, "cpu_data") in {r[:3] for r in self.split_map.ranges} for fill in fills))
        report = rommap.account(self.split_map, self.original, PROJECT, [], self.rules)
        self.assertEqual(report["status_bytes"]["cpu_data:classified_binary"], 38168)
        self.assertEqual(report["status_bytes"]["cpu_data:open"], 177984 - 38168)
        self.assertNotIn("cpu_data:owned_c", report["status_bytes"])

    def test_forged_vtable_rows_fail_validation(self) -> None:
        ranges = self.document["ranges"]
        index = next(i for i, r in enumerate(ranges) if r[2] == "cpu_data")
        start = ranges[index][0]
        for name, ref in (("wrong ref", "main_14400"), ("table id of another start", "VT_main_14400_0"),
                          ("rehashed table id", rommap._vtable_ref(self.inventory, start))):
            document = copy.deepcopy(self.document)
            document["ranges"][index][2:4] = [gxxvtable.CLASS, ref]
            document = rommap._document(self.original, document["ranges"], rules_sha256=document["rules_sha256"],
                                        catalogue_id=document["cpu_catalogue_id"], catalogue=document["cpu_catalogue"],
                                        splits=document["splits"])
            with self.subTest(name=name), self.assertRaises(rommap.MapError):
                self.validate(document)

    def test_tampered_original_or_evidence_fails(self) -> None:
        document = self.evidence[gxxvtable.INPUT]
        row = document["vtables"][0]
        original = bytearray(self.original)
        original[row["start"] + 12] ^= 4  # first slot pfn
        with self.assertRaises(gxxvtable.VtableError):
            self.replay(document, bytes(original))
        forged = copy.deepcopy(document)
        forged["vtables"][0]["end"] += 8
        with self.assertRaisesRegex(rommap.MapError, "Vtable witness"):
            rommap.cpu_vtables(self.original, self.catalogue, {gxxvtable.INPUT: forged}, self.base)
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            shutil.copytree(PROJECT / "config", root / "config")
            path = root / "config/rom-evidence" / gxxvtable.INPUT
            path.write_text(path.read_text().replace('"bound": "entry"', '"bound": "gap_end"', 1))
            with self.assertRaisesRegex(rommap.MapError, "identity changed: " + gxxvtable.INPUT):
                rommap.load_evidence(self.original, self.rules, root)

    def test_owned_data_can_never_take_vtable_credit(self) -> None:
        start, end, *_ = self.split_map.of_class(gxxvtable.CLASS)[0]
        with self.assertRaisesRegex(rommap.MapError, "compiler-generated vtable"):
            rommap.refuse_vtable_credit(self.split_map, [(start - 4, start + 4)])
        rommap.refuse_vtable_credit(self.split_map, [(start - 4, start)])
        handwritten = rommap._handwritten(PROJECT, self.original, [])
        first = next(r for r in self.split_map.of_class("cpu_text_function") if (r[0], r[1]) not in handwritten)
        match = {"image_id": "main_14400", "symbol": "func_owner", "source": "src/vt_owner.c",
                 "rom_start": first[0], "size": first[1] - first[0]}
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            (root / "config").mkdir()
            (root / "src").mkdir()
            for relative in (rommap.HANDWRITTEN_PATH, rommap.CATALOGUE_PATH):
                shutil.copy2(PROJECT / relative, root / relative)
            shutil.copytree(PROJECT / "docs/evidence", root / "docs/evidence")  # handwritten proof inputs
            (root / "src/vt_owner.c").write_text("/* owner */\n")
            with patch.object(rommap, "splat_rows", return_value=[(start, end, ".rodata", "vt_owner")]), \
                    self.assertRaisesRegex(rommap.MapError, "compiler-generated vtable"):
                rommap.account(self.split_map, self.original, root, [match], self.rules, root)


if __name__ == "__main__":
    unittest.main(verbosity=2)
