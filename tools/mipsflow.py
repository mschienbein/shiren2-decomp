"""Control flow and register values inside one catalogued MIPS (VR4300) function.

Author: opus-09 (Anthropic, claude-opus-5-5, xhigh). This is the one engine behind every proof that
reasons about paths or register values: assetformat's transfer and frame reads, and gxxvtable's vtable
installs. When it cannot model a function exactly it raises FlowUnknown; it never approximates.

Nodes. A node is (pc, context):
  (pc, ORDINARY)  the instruction at pc, run in sequence or as a branch target;
  (pc, SLOT)      the instruction at pc, run as the delay slot of the transfer at pc - 4;
  (pc, RETURN)    the return of the callee of the call at pc. It may change every register in
                  CLOBBERED, and it is a definition separate from the call's delay slot;
  ENTRY           the function entry, where every register holds its incoming value.
A delay-slot address therefore has two nodes. A branch that targets it runs it as ORDINARY, and
execution then falls through to pc + 4, while the owner's own path runs it as SLOT.

Edges (pc is the transfer, slot = (pc + 4, SLOT)):
  ordinary non-transfer        -> (pc + 4, ORDINARY)
  any transfer                 -> slot; a likely branch also -> (pc + 8, ORDINARY) (not taken)
  slot of a branch             -> target, (pc + 8); likely: target
  slot of j                    -> target
  slot of jal / jalr rd!=0     -> (pc, RETURN) -> (pc + 8)
  slot of bltzal / bgezal      -> (pc, RETURN) -> (pc + 8), and (pc + 8) (not taken); likely: RETURN only
  slot of a switch `jr`        -> every target of its derived jump table (switch_table)
  slot of `jr ra`              -> function exit
A target outside the function leaves it (a tail call or shared code); so does running past its end.

Refused (FlowUnknown): a `jr` that is not a GCC switch dispatch, `jalr` with rd = 0 (an indirect
jump), eret, a transfer inside a delay slot, a delay slot past the function end, `j` when the
function's VRAM is unknown, and a call or link branch into the middle of the function itself.

Queries: reachable nodes from ENTRY, reach/dominates/between on paths, reaching(reg, node) (the
definitions of a register that may reach a node along entry-reachable paths), value(reg, node) (a
register's value when every reaching definition is a constant computation that gives the same 32-bit
value), source(reg, node) (the register and point a value was moved or displaced from) and same(...)
(two register values provably equal). Traps, syscall and break are ordinary instructions here.
"""
from __future__ import annotations

import struct
from typing import NamedTuple

ORDINARY, SLOT, RETURN = 0, 1, 2
Node = tuple[int, int]
ENTRY: Node = (-1, -1)
RA, SP = 31, 29
# Registers a callee may change, i.e. every register except zero, s0-s7, gp, sp and fp. k0/k1 are
# included because exception handlers may change them at any time.
CLOBBERED = frozenset({*range(1, 16), 24, 25, 26, 27, 31})

_BRANCHES = frozenset({0x04, 0x05, 0x06, 0x07})          # beq, bne, blez, bgtz
_LIKELY = frozenset({0x14, 0x15, 0x16, 0x17})            # beql, bnel, blezl, bgtzl
_SPECIAL_NO_RD = frozenset({0x08, 0x0C, 0x0D, 0x0F, 0x11, 0x13, *range(0x18, 0x20), *range(0x30, 0x37)})
_WRITES_RT = frozenset({*range(0x08, 0x10), 0x18, 0x19, 0x1A, 0x1B, *range(0x20, 0x28), 0x30, 0x34, 0x37, 0x38, 0x3C})
_ERET = 0x42000018


class FlowUnknown(Exception):
    """The function's flow cannot be modelled exactly."""


def _u32(data: bytes, offset: int) -> int:
    return struct.unpack_from(">I", data, offset)[0]


def _sext16(value: int) -> int:
    return ((value & 0xFFFF) ^ 0x8000) - 0x8000


def written(word: int) -> int | None:
    """The general register an instruction writes (each writes at most one; zero is never written),
    else None. A call writes only its link register; the callee's changes belong to its RETURN node."""
    op = word >> 26
    if op == 0:
        reg = None if word & 63 in _SPECIAL_NO_RD else (word >> 11) & 31
    elif op == 0x01:
        reg = RA if (word >> 16) & 31 in (0x10, 0x11, 0x12, 0x13) else None
    elif op == 0x03:
        reg = RA
    elif op in (0x10, 0x11, 0x12, 0x13):  # mfcz, dmfcz, cfcz
        reg = (word >> 16) & 31 if (word >> 21) & 31 in (0, 1, 2) else None
    else:
        reg = (word >> 16) & 31 if op in _WRITES_RT else None
    return reg or None


def transfer(word: int, pc: int, vram_base: int | None) -> tuple[str, int | None] | None:
    """(kind, ROM target) of a transfer of control at `pc`, else None. kind is branch, likely, jump,
    call, link (bltzal/bgezal), link_likely, return (`jr ra`), jr (any other `jr`), jalr_zero or eret.
    `vram_base` is the function's VRAM minus its ROM start; without it a `j` target is None."""
    op, rs, rt = word >> 26, (word >> 21) & 31, (word >> 16) & 31
    relative = pc + 4 + (_sext16(word) << 2)
    if op in _BRANCHES or (op == 0x01 and rt in (0x00, 0x01)):
        return "branch", relative
    if op in _LIKELY or (op == 0x01 and rt in (0x02, 0x03)):
        return "likely", relative
    if op == 0x01 and rt in (0x10, 0x11):
        return "link", relative
    if op == 0x01 and rt in (0x12, 0x13):
        return "link_likely", relative
    if op in (0x10, 0x11, 0x12) and rs == 8:  # bczf/t, bczfl/tl
        return ("likely" if rt & 2 else "branch"), relative
    if op == 0x02:
        if vram_base is None:
            return "jump", None
        return "jump", ((((pc + vram_base + 4) & 0xF0000000) | (word & 0x3FFFFFF) << 2) - vram_base) & 0xFFFFFFFF
    if op == 0x03:
        target = None if vram_base is None else \
            ((((pc + vram_base + 4) & 0xF0000000) | (word & 0x3FFFFFF) << 2) - vram_base) & 0xFFFFFFFF
        return "call", target
    if op == 0 and word & 63 == 0x09:
        return ("jalr_zero" if (word >> 11) & 31 == 0 else "call"), None
    if op == 0 and word & 63 == 0x08:
        return ("return" if rs == RA else "jr"), None
    if word == _ERET:
        return "eret", None
    return None


class Switch(NamedTuple):
    table: int                # ROM of the jump table
    count: int                # its entries (the sltiu bound)
    targets: tuple[int, ...]  # ROM targets, all inside the function
    bound: int                # site of the sltiu


def switch_table(original: bytes, low: int, high: int, vram_base: int, jr: int) -> Switch:
    """The GCC switch dispatch that ends at the `jr rJ` (rJ != ra) at `jr`, else FlowUnknown.

    This is opus-05's reviewed shape: jr-4 `lw rJ, lo(rA)`, jr-8 `addu rA, rA, rS` (either operand
    order), jr-12 `lui rA, hi`, jr-16 `sll rS, rI, 2` with rS not zero or rA. The bound is
    `sltiu rB, rI, N` with rB not zero or rI (the index must survive the test: SolReviewAssetStack F2 r2
    P2-1) at B and `beq rB, zero` (or `beq zero, rB`) at B+4, where B = jr-0x18 (the
    sll is the delay slot) or B = jr-0x1C (the slot is no transfer and does not write rI). No branch
    or j of the function lands in (B, jr]. The table is N words at VRAM (hi << 16) + sext(lo), each
    a word-aligned target inside [low, high). The bound makes the target list exhaustive."""
    def word(at: int) -> int:
        return _u32(original, at) if low <= at < high and at % 4 == 0 else -1

    jw, lw, addu, lui, sll = (word(jr - k) for k in (0, 4, 8, 12, 16))
    rj, ra_, rs_, ri = (jw >> 21) & 31, (lw >> 21) & 31, (sll >> 11) & 31, (sll >> 16) & 31
    shape = (jw >= 0 and jw >> 26 == 0 and jw & 63 == 0x08 and rj not in (0, RA)
             and lw >= 0 and lw >> 26 == 0x23 and (lw >> 16) & 31 == rj and ra_ != 0
             and addu >= 0 and addu >> 26 == 0 and addu & 0x7FF == 0x21 and (addu >> 11) & 31 == ra_
             and sorted(((addu >> 21) & 31, (addu >> 16) & 31)) == sorted((ra_, rs_))
             and lui >= 0 and lui >> 16 == (0x0F << 10 | ra_)
             and sll >= 0 and sll >> 21 == 0 and sll & 0x7FF == (2 << 6) and rs_ not in (0, ra_))
    bound = None
    for b in (jr - 0x18, jr - 0x1C):
        s, branch = word(b), word(b + 4)
        rb = (s >> 16) & 31
        if (shape and s >= 0 and s >> 26 == 0x0B and (s >> 21) & 31 == ri and rb not in (0, ri) and 0 < s & 0xFFFF < 0x8000
                and branch >= 0 and branch >> 26 == 0x04 and {(branch >> 21) & 31, (branch >> 16) & 31} == {rb, 0}):
            slot = word(b + 8)
            if b == jr - 0x18 or (slot >= 0 and transfer(slot, b + 8, vram_base) is None and written(slot) != ri):
                bound = b
                break
    if bound is None:
        raise FlowUnknown(f"jr {jr:#x} is not a GCC switch dispatch")
    for at in range(low, high, 4):
        control = transfer(_u32(original, at), at, vram_base)
        if control and control[0] not in ("call", "return", "jr", "jalr_zero", "eret") and control[1] is not None \
                and bound < control[1] <= jr:
            raise FlowUnknown(f"{at:#x} branches into the switch dispatch {bound:#x}..{jr:#x}")
    count = _u32(original, bound) & 0xFFFF
    table = ((lui & 0xFFFF) << 16) + _sext16(lw) - vram_base
    words = original[table:table + 4 * count] if table >= 0 else b""
    targets = tuple(_u32(words, 4 * i) - vram_base for i in range(len(words) // 4))
    if len(words) != 4 * count or any(t % 4 or not low <= t < high for t in targets):
        raise FlowUnknown(f"jr {jr:#x} table {table:#x} has a target outside its function")
    return Switch(table, count, targets, bound)


class Flow:
    """The node graph of the function [low, high) of `original` (see the module docstring)."""

    def __init__(self, original: bytes, low: int, high: int, vram_base: int | None) -> None:
        self.original, self.low, self.high, self.vram_base = original, low, high, vram_base
        self.switches: dict[int, Switch] = {}
        transfers: dict[int, tuple[str, int | None]] = {}
        for pc in range(low, high, 4):
            control = transfer(_u32(original, pc), pc, vram_base)
            if control is None:
                continue
            kind, target = control
            if kind in ("jalr_zero", "eret"):
                raise FlowUnknown(f"{pc:#x} is an indirect jump without a link or an eret")
            if kind == "jump" and target is None:
                raise FlowUnknown(f"{pc:#x} is a j whose target needs the function's VRAM")
            if kind in ("call", "link", "link_likely") and target is not None and low < target < high:
                raise FlowUnknown(f"{pc:#x} calls into the middle of its own function")
            if pc + 4 >= high or transfer(_u32(original, pc + 4), pc + 4, vram_base) is not None:
                raise FlowUnknown(f"{pc:#x} has its delay slot past the function or a transfer in it")
            transfers[pc] = control
        for pc, (kind, _) in transfers.items():
            if kind == "jr":
                if vram_base is None:
                    raise FlowUnknown(f"jr {pc:#x} in a function without a catalogued VRAM")
                self.switches[pc] = switch_table(original, low, high, vram_base, pc)
        for jr, switch in self.switches.items():
            for other in self.switches.values():
                if any(switch.bound < t <= jr for t in other.targets):
                    raise FlowUnknown(f"a switch table target enters the switch dispatch {switch.bound:#x}..{jr:#x}")
        self.transfers = transfers
        self.succ: dict[Node, tuple[Node, ...]] = {ENTRY: ((low, ORDINARY),)}

        def inside(target: int | None) -> tuple[Node, ...]:
            return ((target, ORDINARY),) if target is not None and low <= target < high else ()

        for pc in range(low, high, 4):
            after = inside(pc + 4)
            control = transfers.get(pc)
            if control is None:
                self.succ[pc, ORDINARY] = after
                continue
            kind, target = control
            slot, past = (pc + 4, SLOT), inside(pc + 8)
            self.succ[pc, ORDINARY] = (slot, *past) if kind in ("likely", "link_likely") else (slot,)
            if kind == "branch":
                self.succ[slot] = (*inside(target), *past)
            elif kind in ("likely", "jump"):
                self.succ[slot] = inside(target)
            elif kind == "call":
                self.succ[slot] = ((pc, RETURN),)
                self.succ[pc, RETURN] = past
            elif kind == "link":
                self.succ[slot] = ((pc, RETURN), *past)
                self.succ[pc, RETURN] = past
            elif kind == "link_likely":
                self.succ[slot] = ((pc, RETURN),)
                self.succ[pc, RETURN] = past
            elif kind == "jr":
                self.succ[slot] = tuple(dict.fromkeys((t, ORDINARY) for t in self.switches[pc].targets))
            else:  # return
                self.succ[slot] = ()
        self.pred: dict[Node, list[Node]] = {}
        for node, targets in self.succ.items():
            for target in targets:
                self.pred.setdefault(target, []).append(node)
        self.reachable = frozenset(self.reach(ENTRY))
        self._reaching: dict[tuple[int, Node], frozenset[Node]] = {}
        self._values: dict[tuple[int, Node], int | None] = {}

    def word(self, node: Node) -> int:
        return _u32(self.original, node[0])

    def writes(self, node: Node, reg: int) -> bool:
        """Whether the node may define `reg` (ENTRY defines every register)."""
        if reg == 0:
            return False
        if node == ENTRY:
            return True
        if node[1] == RETURN:
            return reg in CLOBBERED
        return written(self.word(node)) == reg

    def at(self, pc: int) -> list[Node]:
        """The entry-reachable nodes that run the instruction at `pc`."""
        return [node for node in ((pc, ORDINARY), (pc, SLOT)) if node in self.reachable]

    def ret(self, call: int) -> Node:
        """The RETURN node of the call (jal, jalr, bltzal, bgezal) at `call`."""
        node = (call, RETURN)
        if node not in self.succ:
            raise FlowUnknown(f"{call:#x} is not a call")
        return node

    def reach(self, start: Node, avoid: Node | None = None) -> set[Node]:
        """Nodes reachable from `start` (itself included) without running `avoid`."""
        seen: set[Node] = set()
        todo = [start]
        while todo:
            node = todo.pop()
            if node in seen or node == avoid:
                continue
            seen.add(node)
            todo.extend(self.succ.get(node, ()))
        return seen

    def dominates(self, a: Node, b: Node) -> bool:
        """Whether b is entry-reachable and every path from ENTRY to b runs a."""
        return b in self.reachable and (a == b or b not in self.reach(ENTRY, a))

    def between(self, first: Node, last: Node) -> set[Node]:
        """Nodes run on some path from `first` to `last` that does not run `first` again (`last` itself
        only when such a path runs it earlier, i.e. it lies on a cycle)."""
        forward = set().union(set(), *(self.reach(node, first) for node in self.succ.get(first, ())))
        seen: set[Node] = set()
        todo = list(self.pred.get(last, ()))
        while todo:
            node = todo.pop()
            if node in seen or node == first:
                continue
            seen.add(node)
            todo.extend(self.pred.get(node, ()))
        return forward & seen

    def reaching(self, reg: int, node: Node) -> frozenset[Node]:
        """Definitions of `reg` that may reach `node` (before it runs) along entry-reachable paths: ENTRY
        for the incoming value, (call, RETURN) for a callee's change. Empty if `node` is unreachable;
        never empty otherwise (except for zero, which has no definitions)."""
        key = (reg, node)
        if key not in self._reaching:
            found: set[Node] = set()
            seen: set[Node] = set()
            todo = list(self.pred.get(node, ())) if node in self.reachable and reg else []
            while todo:
                current = todo.pop()
                if current in seen or current not in self.reachable:
                    continue
                seen.add(current)
                if self.writes(current, reg):
                    found.add(current)
                else:
                    todo.extend(self.pred.get(current, ()))
            self._reaching[key] = frozenset(found)
        return self._reaching[key]

    def value(self, reg: int, node: Node) -> int | None:
        """The 32-bit value of `reg` before `node` runs, when every reaching definition computes the same
        constant by lui, addiu/daddiu, ori, or addu/daddu/or of known values (moves included); else None."""
        if reg == 0:
            return 0
        key = (reg, node)
        if key in self._values:
            return self._values[key]
        self._values[key] = None  # a definition cycle stays unknown
        results = set()
        defs = self.reaching(reg, node)
        for d in defs:
            results.add(None if d == ENTRY or d[1] == RETURN else self._computed(d))
            if None in results or len(results) > 1:
                break
        value = results.pop() if len(results) == 1 and defs else None
        self._values[key] = value
        return value

    def _computed(self, node: Node) -> int | None:
        word = self.word(node)
        op, rs, rt, imm = word >> 26, (word >> 21) & 31, (word >> 16) & 31, word & 0xFFFF
        if op == 0x0F:  # lui
            return imm << 16
        if op in (0x09, 0x19):  # addiu, daddiu
            base = self.value(rs, node)
            return None if base is None else (base + _sext16(imm)) & 0xFFFFFFFF
        if op == 0x0D:  # ori
            base = self.value(rs, node)
            return None if base is None else base | imm
        if op == 0 and word & 63 in (0x21, 0x2D, 0x25) and (word >> 6) & 31 == 0:  # addu, daddu, or
            left, right = self.value(rs, node), self.value(rt, node)
            if left is None or right is None:
                return None
            return (left + right) & 0xFFFFFFFF if word & 63 != 0x25 else left | right
        return None

    def _moved(self, node: Node) -> tuple[int, int] | None:
        """(source register, displacement) when the instruction at `node` copies or displaces one register:
        addiu/daddiu rt, rs, imm, or addu/daddu/or rd, rs, zero (either operand order)."""
        word = self.word(node)
        op, rs, rt = word >> 26, (word >> 21) & 31, (word >> 16) & 31
        if op in (0x09, 0x19):
            return rs, _sext16(word)
        if op == 0 and word & 63 in (0x21, 0x2D, 0x25) and (word >> 6) & 31 == 0 and 0 in (rs, rt):
            return rs or rt, 0
        return None

    def source(self, reg: int, node: Node) -> tuple[int, Node, int]:
        """(reg0, node0, k): the value of `reg` before `node` is the value of reg0 before node0 plus k
        (mod 2^32), followed back through reaching sets that are exactly one copy or displacement
        (_moved). The value before node0 is the one of the latest run of node0 before node. reg0 = 0
        means the constant k. It stops at ENTRY, RETURN, several definitions, any other definition or
        a cycle."""
        k, seen = 0, set()
        while reg and (reg, node) not in seen:
            seen.add((reg, node))
            defs = self.reaching(reg, node)
            if len(defs) != 1:
                break
            (d,) = defs
            moved = None if d == ENTRY or d[1] == RETURN else self._moved(d)
            if moved is None:
                break
            reg, node, k = moved[0], d, k + moved[1]
        return reg, node, k & 0xFFFFFFFF

    def same(self, reg_a: int, node_a: Node, reg_b: int, node_b: Node, k_a: int = 0, k_b: int = 0) -> bool:
        """Whether the value of reg_a before node_a plus k_a equals the value of reg_b before node_b plus
        k_b whenever both run (mod 2^32). Both sides are first reduced by source(); then the registers
        must agree, the offsets agree, and the reaching sets of that register at both points be
        identical and nonempty; and either that set is {ENTRY} (the incoming value, never redefined) or
        one point dominates the other, does not write the register itself and nothing on a path between
        them writes it. Otherwise False."""
        (ra, na, ka), (rb, nb, kb) = self.source(reg_a, node_a), self.source(reg_b, node_b)
        if ra != rb or (ka + k_a - kb - k_b) & 0xFFFFFFFF:
            return False
        if ra == 0:
            return na in self.reachable and nb in self.reachable
        defs = self.reaching(ra, na)
        if not defs or defs != self.reaching(ra, nb):
            return False
        if defs == {ENTRY}:
            return True
        for first, last in ((na, nb), (nb, na)):
            if self.dominates(first, last) and not self.writes(first, ra) and \
                    not any(self.writes(node, ra) for node in self.between(first, last)):
                return True
        return False
