#!/usr/bin/env python3
"""Positive per-range format witnesses for located stored-tail assets; classification only.

A located map range (owner window known, format open) is research-classified only when a
reviewed witness in `config/rom-evidence/asset-formats.json` replays here against the original:

- the witness names the exact map rows it closes (class, ref and content hash), optional
  read-only context rows, and catalogued code immediates that form those intervals;
- a registered format parser consumes the original bytes and returns typed spans that tile
  every closed range exactly; `pad` spans are only short all-zero alignment fill;
- at least 1/MIN_EVIDENCE_SHARE of every closed range is nonzero bytes or fields whose zero
  value the parser itself requires (REQUIRED_ZERO); an all-zero or zero-dominated range fails
  even when every byte carries a typed label;
- every pinned consumer is a catalogued function whose original instructions are unchanged, and
  each pinned load/store/address instruction agrees with the parser: every instance of the
  struct it reads has typed fields covering exactly that offset and width (a load of the
  consumer's own stack copy of a whole-instance transfer counts at its offset in that copy);
- the span inventory equals the reviewed count and digest.

Zero bytes, extents or geometry alone never qualify. Nothing here edits the map, grants C/C++
credit or counts as roundtrip.

Parser contract: ``parse(original, ranges, params) -> list[Span]``. ``ranges`` are the exact
closed intervals of one witness in record order; ``params`` is the record's read-only parameters
plus ``context`` (the record's context intervals). Spans name fields ``struct.member`` with the
start of their struct instance as ``base``; the framework, not the parser, checks tiling, padding,
consumer reads and the pinned inventory.
"""
from __future__ import annotations

import bisect
import hashlib
import json
import struct
from types import MappingProxyType
from typing import Any, Callable, Mapping, NamedTuple, Sequence

import mipsflow

SCHEMA = "shiren2-asset-format-witnesses-v1"
INPUT = "asset-formats.json"
PAD = "pad"
MAX_PAD = 16       # a pad span is shorter than this and all zero
MAX_PAD_SHARE = 8  # pad bytes are at most 1/MAX_PAD_SHARE of every closed range
MIN_EVIDENCE_SHARE = 4  # nonzero or required-zero bytes are at least 1/MIN_EVIDENCE_SHARE of every closed range
# Fields whose zero value the registered parser itself requires (it rejects any other value).
# check_tiling re-checks them and counts them as evidence; any other zero byte, typed or not,
# is not evidence that the range has the format.
REQUIRED_ZERO = frozenset({
    "bank.flags", "tune.reserved", "wavetable.type", "wavetable.flags",  # unrelocated libmus bank state
    "toolrec.info_reserved", "toolrec.data_odd_pad",                     # sound-tool record constants
    "hvq2_bits.fill", "rle_code.end",                                    # HVQ2 word fill, RLE end code
})


class AssetFormatError(ValueError):
    """A witness record, parser result or consumer cross-check failed."""


class Span(NamedTuple):
    start: int
    end: int
    field: str  # "struct.member", or PAD for alignment fill
    base: int   # start of the struct instance this field belongs to


Interval = tuple[int, int]
Parser = Callable[[bytes, tuple[Interval, ...], Mapping[str, Any]], list[Span]]
Immediate = Callable[[bytes, int], int]

# Load/store opcode -> access width. Address formation (addiu base, offset) carries its width.
_ACCESS = {0x20: 1, 0x24: 1, 0x28: 1, 0x21: 2, 0x25: 2, 0x29: 2, 0x23: 4, 0x2B: 4, 0x31: 4, 0x39: 4}
_ADDIU = 0x09
_SP = 29
# Loads that may pin a field of a consumer's stack copy (lb, lh, lw, lbu, lhu, lwc1); stores never do.
_FRAME_LOADS = frozenset({0x20, 0x21, 0x23, 0x24, 0x25, 0x31})
# Store opcode -> (bytes it may write below its address, bytes from its address): sb, sh, swl, sw,
# sdl, sdr, swr, sc, swc1, scd, sdc1, sd (the unaligned forms and conditional stores conservatively).
_STORES = {0x28: (0, 1), 0x29: (0, 2), 0x2A: (0, 4), 0x2B: (0, 4), 0x2C: (0, 8), 0x2D: (7, 1), 0x2E: (3, 1),
           0x38: (0, 4), 0x39: (0, 4), 0x3C: (0, 8), 0x3D: (0, 8), 0x3F: (0, 8)}
# SPECIAL functions that write no rd (jr, syscall, break, sync, mthi, mtlo, mult/div, traps).
_SPECIAL_NO_RD = frozenset({0x08, 0x0C, 0x0D, 0x0F, 0x11, 0x13, *range(0x18, 0x20), *range(0x30, 0x37)})
# I-type opcodes that write rt (immediate ALU, lui, loads, ll/lld, sc/scd).
_WRITES_RT = frozenset({*range(0x08, 0x10), 0x18, 0x19, 0x1A, 0x1B, *range(0x20, 0x28), 0x30, 0x34, 0x37, 0x38, 0x3C})
_BRANCHES = frozenset({0x04, 0x05, 0x06, 0x07})          # beq, bne, blez, bgtz
_LIKELY_BRANCHES = frozenset({0x14, 0x15, 0x16, 0x17})   # beql, bnel, blezl, bgtzl
TRANSFER_SIZE_REACH = 0x14  # a transfer's size argument may precede its jal by up to five instructions
_JAL = 0x03
_BYTE_LOADS = frozenset({0x20, 0x24})  # lb, lbu
_MAIN_IMAGE = "main_14400"


def _u32(data: bytes, offset: int) -> int:
    return struct.unpack_from(">I", data, offset)[0]


def _record(value: object, label: str, required: set[str], optional: frozenset[str] = frozenset()) -> dict[str, Any]:
    if not isinstance(value, dict) or not required <= set(value) <= required | optional:
        raise AssetFormatError(f"{label} fields do not match the schema")
    return value


def _nat(value: object, label: str) -> int:
    if type(value) is not int or value < 0:
        raise AssetFormatError(f"{label} must be a nonnegative integer")
    return value


def span_digest(spans: Sequence[Span]) -> str:
    """Digest of the exact sorted span inventory (start, end, field, base)."""
    rows = [[s.start, s.end, s.field, s.base] for s in spans]
    return hashlib.sha256(json.dumps(rows, separators=(",", ":")).encode()).hexdigest()


def check_tiling(original: bytes, spans: Sequence[Span], ranges: Sequence[Interval]) -> list[Span]:
    """Sorted spans that tile every closed range exactly; pads are short zero fill; zero bytes
    the format does not require never dominate a range."""
    if not isinstance(spans, list) or any(not isinstance(s, tuple) or len(s) != 4 for s in spans):
        raise AssetFormatError("Parser must return a list of (start, end, field, base) spans")
    ordered = sorted(Span(*s) for s in spans)
    for s in ordered:
        if any(type(v) is not int for v in (s.start, s.end, s.base)) or not isinstance(s.field, str) or not s.start < s.end:
            raise AssetFormatError(f"Malformed span {s!r}"[:200])
        if s.field != PAD and (s.field.count(".") != 1 or not all(s.field.split(".")) or s.base > s.start):
            raise AssetFormatError(f"Span field must be struct.member inside its instance: {s.field}")
    for left, right in zip(ordered, ordered[1:]):
        if right.start < left.end:
            raise AssetFormatError(f"Spans overlap at {right.start:#x}")
    # Spans are disjoint and sorted, so each range's spans are one contiguous run found by bisection.
    starts = [s.start for s in ordered]
    covered = 0
    for start, end in ranges:
        index = bisect.bisect_left(starts, start)
        if index and ordered[index - 1].end > start:
            raise AssetFormatError(f"Spans do not tile {start:#x}..{end:#x} at {start:#x}")
        cursor, typed, padded, required = start, 0, 0, 0
        while index < len(ordered) and ordered[index].start < end:
            s = ordered[index]
            if s.start != cursor or s.end > end:
                raise AssetFormatError(f"Spans do not tile {start:#x}..{end:#x} at {cursor:#x}")
            if s.field == PAD:
                if s.end - s.start >= MAX_PAD or any(original[s.start:s.end]):
                    raise AssetFormatError(f"Pad {s.start:#x}..{s.end:#x} is not short zero alignment fill")
                padded += s.end - s.start
            else:
                typed += s.end - s.start
                if s.field in REQUIRED_ZERO:
                    if any(original[s.start:s.end]):
                        raise AssetFormatError(f"Required-zero field {s.field} at {s.start:#x} is not zero")
                    required += s.end - s.start
            cursor = s.end
            covered += 1
            index += 1
        if cursor != end:
            raise AssetFormatError(f"Spans do not reach the end of {start:#x}..{end:#x}")
        if not typed or padded * MAX_PAD_SHARE > end - start:
            raise AssetFormatError(f"{start:#x}..{end:#x} is not positively typed")
        nonzero = end - start - original.count(0, start, end)
        if (nonzero + required) * MIN_EVIDENCE_SHARE < end - start:
            raise AssetFormatError(f"{start:#x}..{end:#x} is dominated by zero bytes its format does not require")
    if covered != len(ordered):
        raise AssetFormatError("Parser returned spans outside the closed ranges")
    return ordered


def _check_function(original: bytes, row: Mapping[str, Any], functions: Mapping[tuple[str, int, int], str],
                    label: str) -> tuple[int, int]:
    start, size = _nat(row["rom_start"], "rom_start"), _nat(row["size"], "size")
    key = (row["symbol"], start, size)
    if functions.get(key) != row["sha256"] or hashlib.sha256(original[start:start + size]).hexdigest() != row["sha256"]:
        raise AssetFormatError(f"{label} {row['symbol']} is not an unchanged catalogued function")
    return start, start + size


def _hex_word(value: object) -> int:
    """An 8-digit hex instruction word, or -1."""
    if not isinstance(value, str) or len(value) != 8 or any(c not in "0123456789abcdefABCDEF" for c in value):
        return -1
    return int(value, 16)


_CALLER_SAVED = frozenset({*range(1, 16), 24, 25, 31})  # at, v0-v1, a0-a3, t0-t9, ra: a callee may change them


def _writes_reg(word: int, reg: int) -> bool:
    """Whether an instruction may write GPR `reg` (conservative: any GPR-writing form with that
    destination; a call may change every caller-saved register)."""
    op = word >> 26
    if op == 0x03 or (op == 0 and word & 63 == 0x09) or (op == 0x01 and (word >> 16) & 31 in (0x10, 0x11, 0x12, 0x13)):
        return reg in _CALLER_SAVED or (op == 0 and (word >> 11) & 31 == reg)
    if op == 0:
        return (word >> 11) & 31 == reg and word & 63 not in _SPECIAL_NO_RD
    if op in (0x10, 0x11, 0x12):  # mfcz, dmfcz, cfcz write rt
        return (word >> 21) & 31 in (0, 1, 2) and (word >> 16) & 31 == reg
    return op in _WRITES_RT and (word >> 16) & 31 == reg


def _writes_sp(word: int) -> bool:
    """Whether an instruction may write sp."""
    return _writes_reg(word, _SP)


def _control(word: int, site: int, vram_base: int | None) -> tuple[str, int | None] | None:
    """(kind, ROM target) of a transfer of control at `site`, else None.

    kind is branch, likely (delay slot only on the taken edge), jump, call, return or indirect;
    `vram_base` is the function's vram minus its ROM start (None: j targets are unknown, so indirect).
    """
    op, rs, rt = word >> 26, (word >> 21) & 31, (word >> 16) & 31
    relative = site + 4 + ((((word & 0xFFFF) ^ 0x8000) - 0x8000) << 2)
    if op in _BRANCHES or (op == 0x01 and rt in (0x00, 0x01, 0x10, 0x11)):
        return "branch", relative
    if op in _LIKELY_BRANCHES or (op == 0x01 and rt in (0x02, 0x03, 0x12, 0x13)):
        return "likely", relative
    if op in (0x10, 0x11, 0x12) and rs == 8:  # bcz(f/t)(l)
        return ("likely" if word & 0x20000 else "branch"), relative
    if op == 0x02:
        if vram_base is None:
            return "indirect", None
        return "jump", ((((site + vram_base + 4) & 0xF0000000) | (word & 0x3FFFFFF) << 2) - vram_base) & 0xFFFFFFFF
    if op == 0x03 or (op == 0 and word & 63 == 0x09):
        return "call", None
    if op == 0 and word & 63 == 0x08:
        return ("return" if rs == 31 else "indirect"), None
    return None


def _flow(original: bytes, low: int, high: int, vram_base: int | None) -> mipsflow.Flow | None:
    """The function's flow graph (mipsflow, the shared engine), or None when it cannot be modelled
    exactly (an indirect jump that is not a switch, jalr without link, a transfer in a delay slot, a
    j without the function's VRAM, ...): every proof that needs it then fails closed."""
    try:
        return mipsflow.Flow(original, low, high, vram_base)
    except mipsflow.FlowUnknown:
        return None


def _received(flow: mipsflow.Flow, reg: int, call: int, site: int) -> bool:
    """Whether the callee of the call at `call` receives in `reg`, on every entry-reachable path, the
    value the instruction at `site` wrote (argument liveness through callee entry: the definitions of
    `reg` reaching the callee, after the delay slot, are all that instruction)."""
    defs = flow.reaching(reg, flow.ret(call))
    return bool(defs) and all(node[0] == site and node[1] != mipsflow.RETURN for node in defs)


def _frame_displacement(original: bytes, reads: Sequence[Mapping[str, Any]], read: Mapping[str, Any],
                        low: int, high: int, flow: mipsflow.Flow | None) -> int:
    """Stack displacement of the consumer's copy that a "frame" access loads (see check_reads)."""
    pin = _record(read["frame"], "Consumer frame", {"site", "word"})
    site, word, offset, width = read["site"], _hex_word(read["word"]), read["offset"], read["width"]
    frame_site, frame_word = _nat(pin["site"], "frame site"), _hex_word(pin["word"])
    disp = frame_word & 0xFFFF
    if (frame_site % 4 or not low <= frame_site < high or frame_word < 0 or _u32(original, frame_site) != frame_word
            or frame_word >> 26 != _ADDIU or (frame_word >> 21) & 31 != _SP
            or not 4 <= (frame_word >> 16) & 31 <= 7 or disp & 0x8000):
        raise AssetFormatError(f"Consumer frame {frame_site:#x} is not an addiu aN, sp, disp argument")
    copies = [r for r in reads if r is not read and isinstance(r, dict) and r.get("struct") == read["struct"]
              and isinstance(r.get("transfer_call"), dict)
              and r["transfer_call"].get("site") in (frame_site - 4, frame_site + 4)]
    if len(copies) != 1:
        raise AssetFormatError(f"Consumer frame {frame_site:#x} is not next to a pinned {read['struct']} transfer call")
    copy = copies[0]
    call, size_site, size = copy["transfer_call"]["site"], copy.get("site"), copy.get("width")
    if frame_site == size_site or (frame_word >> 16) & 31 == (_hex_word(copy.get("word")) >> 16) & 31 \
            or type(size) is not int:
        raise AssetFormatError(f"Consumer frame {frame_site:#x} is its transfer's size argument")
    if (word >> 26 not in _FRAME_LOADS or (word >> 21) & 31 != _SP or site <= call + 4
            or type(width) is not int or offset + width > size):
        raise AssetFormatError(f"Consumer frame access {site:#x} is not a later sp load inside the {size}-byte copy")
    # The copy must have run, into this stack frame, whenever the load executes. The destination the
    # callee receives is this frame word's on every path (a loop through the call, a delay slot that
    # rewrites it or a skipped formation all fail: SolReviewAssetStack F2 P2-2), with no sp write
    # between them; the call's return dominates every execution of the load; and no path from it to
    # the load moves sp, so disp addresses the copied bytes.
    if flow is None:
        raise AssetFormatError(f"Consumer frame access {site:#x} lies in a function whose flow has indirect jumps "
                               f"or cannot be modelled")
    if (call, mipsflow.RETURN) not in flow.succ:
        raise AssetFormatError(f"Consumer frame {frame_site:#x} is not next to a pinned {read['struct']} transfer call")
    ret = flow.ret(call)
    if not _received(flow, (frame_word >> 16) & 31, call, frame_site) or any(
            flow.writes(node, _SP) for d in flow.reaching((frame_word >> 16) & 31, ret) for node in flow.between(d, ret)):
        raise AssetFormatError(f"Consumer frame {frame_site:#x} is not a fresh destination of every copy")
    loads = flow.at(site)
    if not loads or not all(flow.dominates(ret, node) for node in loads):
        raise AssetFormatError(f"Consumer frame access {site:#x}: the copy does not precede the load on every path")
    moved = sorted(n[0] for node in loads for n in flow.between(ret, node) if flow.writes(n, _SP))
    if moved:
        raise AssetFormatError(f"Consumer {moved[0]:#x} moves sp between the copy and the frame access {site:#x}")
    for at in range(low, high, 4):
        value = _u32(original, at)
        if _writes_sp(value) and (value >> 26 != _ADDIU or (value >> 21) & 31 != _SP):
            raise AssetFormatError(f"Consumer {at:#x} writes sp other than by addiu sp, sp, imm")
        store = _STORES.get(value >> 26)
        if store and (value >> 21) & 31 == _SP:
            address = (value & 0xFFFF) - ((value & 0x8000) << 1)
            if address - store[0] < disp + size and disp < address + store[1]:
                raise AssetFormatError(f"Consumer store {at:#x} overlaps the stack copy at sp+{disp:#x}")
    return disp


class CallIndex:
    """Catalogue rows that resolve `jal` targets for fetch_call reads: a target VRAM resolves to the
    caller's own image, else the main image, else the unique row of any image; otherwise it is
    unresolved. `replay` builds one from the catalogue it is given."""

    def __init__(self, rows: Sequence[Mapping[str, Any]]) -> None:
        self.rows = {(r["symbol"], r["rom_start"], r["size"]): r for r in rows if type(r.get("vram_start")) is int}
        self.by_vram: dict[int, list[Mapping[str, Any]]] = {}
        for r in self.rows.values():
            self.by_vram.setdefault(r["vram_start"], []).append(r)
        self._free: dict[tuple, str | None] = {}

    def row(self, key: tuple[str, int, int]) -> Mapping[str, Any] | None:
        return self.rows.get(key)

    def resolve_vram(self, caller: Mapping[str, Any], target: int) -> Mapping[str, Any] | None:
        candidates = self.by_vram.get(target, [])
        for pick in ([r for r in candidates if r["image_id"] == caller["image_id"]],
                     [r for r in candidates if r["image_id"] == _MAIN_IMAGE], candidates):
            if len(pick) == 1:
                return pick[0]
        return None

    def resolve(self, caller: Mapping[str, Any], word: int) -> Mapping[str, Any] | None:
        """Callee of the `jal` word in `caller` (the target keeps the caller's top four VRAM bits)."""
        return self.resolve_vram(caller, ((word & 0x3FFFFFF) << 2) | (caller["vram_start"] & 0xF0000000))

    def fetch_free(self, original: bytes, row: Mapping[str, Any], fetcher: tuple[str, int, int]) -> str | None:
        """None when nothing in the transitive call closure of `row` can fetch: every function in it
        (visited set, cycles allowed) is catalogued and unchanged, has a flow mipsflow can model (so
        every `jr` other than `jr ra` is a switch dispatch whose targets stay inside it), has no jalr,
        link branch or unresolved jal or tail jump, and none of them is or calls the fetcher.
        Otherwise the reason."""
        start_key = (row["symbol"], row["rom_start"], row["size"])
        if (start_key, fetcher) in self._free:
            return self._free[start_key, fetcher]
        seen: set[tuple[str, int, int]] = set()
        stack, reason = [row], None
        while stack and reason is None:
            r = stack.pop()
            key = (r["symbol"], r["rom_start"], r["size"])
            if key in seen:
                continue
            seen.add(key)
            lo, hi = r["rom_start"], r["rom_start"] + r["size"]
            if key == fetcher:
                reason = f"{r['symbol']} is the fetcher"
            elif hashlib.sha256(original[lo:hi]).hexdigest() != r["sha256"]:
                reason = f"{r['symbol']} is not its catalogued bytes"
            if reason is None:
                try:
                    _switches(original, lo, hi, r["vram_start"] - lo)
                except AssetFormatError as error:
                    reason = f"{r['symbol']}: {error}"
            for at in range(lo, hi, 4) if reason is None else ():
                w = _u32(original, at)
                control = _control(w, at, r["vram_start"] - lo)
                if (w >> 26 == 0 and w & 63 == 0x09) or (w >> 26 == 0x01 and (w >> 16) & 31 in (0x10, 0x11, 0x12, 0x13)):
                    reason = f"{r['symbol']} has an indirect or link call at {at:#x}"
                elif w >> 26 == _JAL:
                    callee = self.resolve(r, w)
                    if callee is None:
                        reason = f"{r['symbol']} call {at:#x} has no unique catalogued target"
                    else:
                        stack.append(callee)
                elif control and control[0] == "jump" and not lo <= control[1] < hi:
                    callee = self.resolve_vram(r, control[1] + r["vram_start"] - lo)
                    if callee is None or callee["rom_start"] != control[1]:
                        reason = f"{r['symbol']} tail jump {at:#x} has no unique catalogued target"
                    else:
                        stack.append(callee)
                if reason:
                    break
        self._free[start_key, fetcher] = reason
        return reason


def _fetch_stop(word: int) -> bool:
    """Forms a fetch window may not contain and a fetch-free closure may not call through: jalr,
    link branches (bltzal family), traps, syscall, break, eret."""
    op = word >> 26
    if op == 0:
        return word & 63 in (0x09, 0x0C, 0x0D) or 0x30 <= word & 63 <= 0x36
    if op == 0x01:
        return (word >> 16) & 31 not in (0x00, 0x01, 0x02, 0x03)
    return word == 0x42000018  # eret


def _switches(original: bytes, low: int, high: int, vram_base: int | None) -> dict[int, mipsflow.Switch]:
    """The switch dispatches of one function, {jr: mipsflow.Switch}, from the shared engine's flow
    graph: mipsflow.Flow derives each `jr` other than `jr ra` with mipsflow.switch_table (the GCC
    dispatch shape) and rejects a table target that enters any dispatch. AssetFormatError when the
    function cannot be modelled exactly."""
    try:
        return dict(mipsflow.Flow(original, low, high, vram_base).switches)
    except mipsflow.FlowUnknown as error:
        raise AssetFormatError(str(error)) from error


def _jump_tables(original: bytes, consumer: Mapping[str, Any], low: int, high: int, vram: int | None) -> set[int]:
    """ROM targets of the consumer's switch tables. The consumer's flow must be one mipsflow can
    model (_switches), and every switch `jr` has exactly one "jump_tables" record {jr, site, count,
    words_sha256} whose site and count are the derived ones."""
    try:
        switches = _switches(original, low, high, None if vram is None else vram - low)
    except AssetFormatError as error:
        raise AssetFormatError(f"Consumer {consumer['symbol']}: {error}") from error
    records = consumer.get("jump_tables", [])
    if not isinstance(records, list) or (records and not switches):
        raise AssetFormatError(f"Consumer {consumer['symbol']} pins jump tables but has no indirect jr")
    pinned = {}
    for record in records:
        record = _record(record, "Jump table", {"jr", "site", "count", "words_sha256"})
        pinned.setdefault(record["jr"], []).append(record)
    if set(pinned) != set(switches) or any(len(v) != 1 for v in pinned.values()):
        raise AssetFormatError(f"Consumer {consumer['symbol']} has {len(switches)} indirect jr but not one pinned "
                               f"jump table each")
    targets: set[int] = set()
    for jr, switch in switches.items():
        record = pinned[jr][0]
        words = original[switch.table:switch.table + 4 * switch.count]
        if (record["site"], record["count"]) != (switch.table, switch.count) or \
                hashlib.sha256(words).hexdigest() != record["words_sha256"]:
            raise AssetFormatError(f"Jump table of jr {jr:#x} in {consumer['symbol']} is not the derived "
                                   f"{switch.table:#x}/{switch.count} with its pinned words")
        targets |= set(switch.targets)
    return targets


def _fetch_window(original: bytes, consumer: Mapping[str, Any], low: int, high: int, entry: int, site: int,
                  fetcher: tuple[str, int, int], calls: CallIndex, tables: set[int]) -> int:
    """Fetcher calls executed on every path from `entry` to the fetch at `site` (see check_reads):
    the reached window is walked forward, then counted in reverse topological (address) order."""
    base = consumer["vram_start"] - consumer["rom_start"]
    for at in range(low, high, 4):
        control = _control(_u32(original, at), at, base)
        if (not entry <= at < site and control and control[0] in ("branch", "likely", "jump")
                and entry < control[1] <= site):
            raise AssetFormatError(f"Consumer {at:#x} branches into the fetch window {entry:#x}..{site:#x}")
    inside = sorted(t for t in tables if entry < t <= site)
    if inside:
        raise AssetFormatError(f"Jump table target {inside[0]:#x} enters the fetch window {entry:#x}..{site:#x}")

    def slot(at: int) -> None:
        word = _u32(original, at)
        if at >= site or _control(word, at, base) is not None or _fetch_stop(word):
            raise AssetFormatError(f"Fetch window {entry:#x}..{site:#x} has a transfer or the fetch in a delay slot")

    def edges(at: int) -> tuple[int, tuple[int, ...]]:
        """(fetches by the instruction at `at`, successors) along the window's forward edges."""
        word = _u32(original, at)
        control = _control(word, at, base)
        if _fetch_stop(word) or (control and control[0] in ("return", "indirect")):
            raise AssetFormatError(f"Fetch window {entry:#x}..{site:#x} has a jr, jalr, trap or link branch at {at:#x}")
        if control is None:
            return 0, (at + 4,)
        if control[0] == "call":
            callee = calls.resolve(consumer, word)
            if callee is None:
                raise AssetFormatError(f"Fetch window call {at:#x} has no unique catalogued target")
            own = (callee["symbol"], callee["rom_start"], callee["size"]) == fetcher
            reason = None if own else calls.fetch_free(original, callee, fetcher)
            if reason:
                raise AssetFormatError(f"Fetch window call {at:#x} may fetch: {reason}")
            slot(at + 4)
            return int(own), (at + 8,)
        kind, to = control
        if not at + 8 <= to <= site:
            raise AssetFormatError(f"Fetch window branch {at:#x} does not go forward inside the window")
        slot(at + 4)
        return 0, ((to,) if kind == "jump" else (to, at + 8))

    graph: dict[int, tuple[int, tuple[int, ...]]] = {}
    todo = [entry]
    while todo:
        at = todo.pop()
        if at in graph or at == site:
            continue
        if not entry <= at < site:
            raise AssetFormatError(f"A path from {entry:#x} leaves the fetch window before {site:#x}")
        graph[at] = edges(at)
        todo.extend(graph[at][1])
    counts: dict[int, frozenset[int]] = {site: frozenset({0})}
    for at in sorted(graph, reverse=True):   # every edge goes to a higher address
        own, successors = graph[at]
        counts[at] = frozenset(own + count for successor in successors for count in counts[successor])
    if len(counts[entry]) != 1:
        raise AssetFormatError(f"Paths from {entry:#x} reach the fetch at {site:#x} after different fetch counts")
    return next(iter(counts[entry]))


def _fetch_offset(original: bytes, consumer: Mapping[str, Any], read: Mapping[str, Any], low: int, high: int,
                  functions: Mapping[tuple[str, int, int], str], calls: CallIndex | None, tables: set[int]) -> int:
    """Instance offset of the byte a "fetch_call" read fetches (see check_reads)."""
    rec = _record(read["fetch_call"], "Fetch call", {"fetcher", "entry", "base"})
    pin = _record(rec["fetcher"], "Fetcher", {"symbol", "rom_start", "size", "sha256", "load"})
    if calls is None:
        raise AssetFormatError("fetch_call reads need the catalogue call index")
    f_low, f_high = _check_function(original, pin, functions, "Fetcher")
    fetcher = (pin["symbol"], f_low, f_high - f_low)
    consumer_row = calls.row((consumer["symbol"], low, high - low))
    if calls.row(fetcher) is None or consumer_row is None:
        raise AssetFormatError(f"Fetch {read['site']:#x}: consumer or fetcher is not in the call index")
    load = _record(pin["load"], "Fetcher load", {"site", "word"})
    loads = [at for at in range(f_low, f_high, 4) if _u32(original, at) >> 26 in _BYTE_LOADS]
    if loads != [load["site"]] or _u32(original, loads[0]) != _hex_word(load["word"]):
        raise AssetFormatError(f"Fetcher {pin['symbol']} does not have exactly the pinned byte load")
    word = _u32(original, read["site"])
    callee = calls.resolve(consumer_row, word) if word >> 26 == _JAL else None
    if callee is None or (callee["symbol"], callee["rom_start"], callee["size"]) != fetcher:
        raise AssetFormatError(f"Consumer fetch {read['site']:#x} is not a jal to its fetcher")
    entry = _record(rec["entry"], "Fetch entry", {"site", "word"})
    at = _nat(entry["site"], "fetch entry site")
    if at % 4 or not low <= at <= read["site"] or _u32(original, at) != _hex_word(entry["word"]):
        raise AssetFormatError(f"Fetch entry {at:#x} is not a pinned instruction before its fetch")
    return _nat(rec["base"], "fetch base") + _fetch_window(original, consumer_row, low, high, at, read["site"],
                                                           fetcher, calls, tables)


def check_reads(original: bytes, consumers: Sequence[Mapping[str, Any]], ordered: Sequence[Span],
                functions: Mapping[tuple[str, int, int], str],
                vrams: Mapping[tuple[str, int, int], int] | None = None,
                calls: CallIndex | None = None) -> set[tuple[int, str]]:
    """Every pinned consumer access lands on typed fields of every instance of its struct.

    An access whose struct has no instance in these spans is not checked here; `replay`
    requires every access of a format to match in at least one of its witnesses, and every
    witness to match at least one access. Returns the (site, struct) accesses that matched. A span's `base` is
    the address the consumer adds its immediate to (the struct or array-entry start).

    A read with "transfer_call" pins a whole-instance transfer instead of a field access: the site
    is `addiu aN, zero, width` (the size argument of a copy or PI call) in the delay slot of the
    pinned `jal` or before it. A site before the call is the instruction just before it, or up to
    TRANSFER_SIZE_REACH bytes earlier with no transfer of control between them; either way the
    consumer's flow graph must be known (`vrams` gives its vram) and the size must be live into the
    callee: it dominates the call, and nothing on a path from it to the call (a loop through the call
    included) or in the call's delay slot writes aN. Its offset is 0, and every instance of the
    struct is exactly `width` bytes of contiguous typed fields.

    A read with "frame" {site, word} pins a load of the consumer's own stack copy of an instance.
    The frame word is `addiu aN, sp, disp` (disp without sign bit, aN not the size register) on the
    other side of the jal of a transfer_call read for the same struct in the same consumer. The
    access is a load (lb, lh, lw, lbu, lhu, lwc1) with base sp and immediate disp + offset, after
    the call's delay slot, with offset + width inside the transfer. The consumer's flow graph must be
    known; a frame word before the call must be live into the callee as above, with no sp write on
    those paths or in the delay slot either (each copy gets a fresh destination of its own sp). The
    call and the frame word dominate the load, no instruction on a path from the call to the load
    writes sp, the consumer writes sp only by `addiu sp, sp, imm`, and no sp-relative store
    (conditional stores included) overlaps [disp, disp + transfer width).

    A read with "fetch_call" {fetcher, entry, base} pins a one-byte fetch of a byte stream: the site
    is a `jal` that `calls` resolves to the fetcher, an unchanged catalogued function whose only
    byte load (lb/lbu) is the pinned load; the width is 1. The offset is derived, not asserted: it
    is `base` plus the number of fetcher calls on every path from the pinned `entry` instruction to
    the site. That window [entry, site) is a forward DAG inside the consumer: no jr, jalr, link
    branch, trap, syscall, break or eret; every branch and j goes forward to at most the site; no
    delay slot holds a transfer; no branch from outside it lands in (entry, site]; every path
    reaches the site with the same fetch count; every other call in it has a fetch-free transitive
    closure (CallIndex.fetch_free). A consumer with fetch_call reads has a flow mipsflow can model:
    each `jr` other than `jr ra` is a switch dispatch (mipsflow.switch_table) with exactly one
    "jump_tables" record {jr, site, count, words_sha256} whose site and count are the derived ones,
    and no table target lies in any window's (entry, site]. fetch_call cannot be combined with
    transfer_call, frame or size_site.
    """
    instances: dict[str, dict[int, list[Span]]] = {}
    for s in ordered:
        if s.field != PAD:
            instances.setdefault(s.field.split(".")[0], {}).setdefault(s.base, []).append(s)
    matched: set[tuple[int, str]] = set()
    for consumer in consumers:
        _record(consumer, "Consumer", {"symbol", "image_id", "rom_start", "size", "sha256", "source", "reads"},
                frozenset({"jump_tables"}))
        low, high = _check_function(original, consumer, functions, "Consumer")
        vram = (vrams or {}).get((consumer["symbol"], consumer["rom_start"], consumer["size"]))
        flow = _flow(original, low, high, None if vram is None else vram - low)
        if not consumer["reads"]:
            raise AssetFormatError(f"Consumer {consumer['symbol']} pins no access")
        tables = None
        for read in consumer["reads"]:
            _record(read, "Consumer access", {"site", "word", "struct", "offset", "width"},
                    frozenset({"size_site", "size_scale", "transfer_call", "frame", "fetch_call"}))
            site, offset, name, width = _nat(read["site"], "site"), _nat(read["offset"], "offset"), read["struct"], read["width"]
            if not isinstance(read["word"], str) or len(read["word"]) != 8 or not isinstance(name, str):
                raise AssetFormatError(f"Consumer access {site:#x} must pin an 8-digit hex word and a struct name")
            word = int(read["word"], 16)
            if site % 4 or not low <= site < high or _u32(original, site) != word:
                raise AssetFormatError(f"Consumer access {site:#x} is not the pinned instruction")
            op, rs, imm = word >> 26, (word >> 21) & 31, word & 0xFFFF
            transfer = "transfer_call" in read
            fetch = "fetch_call" in read
            if fetch:
                if transfer or "frame" in read or "size_site" in read or type(width) is not int or width != 1:
                    raise AssetFormatError(f"Consumer fetch {site:#x} is not a plain one-byte fetch")
                if tables is None:
                    tables = _jump_tables(original, consumer, low, high, vram)
                if offset != _fetch_offset(original, consumer, read, low, high, functions, calls, tables):
                    raise AssetFormatError(f"Consumer fetch {site:#x} offset is not its derived fetch position")
            disp = 0
            if "frame" in read:
                if transfer or "size_site" in read:
                    raise AssetFormatError(f"Consumer frame access {site:#x} is not a plain load")
                disp = _frame_displacement(original, consumer["reads"], read, low, high, flow)
            if transfer:
                call = _record(read["transfer_call"], "Transfer call", {"site", "word"})
                call_site = _nat(call["site"], "transfer call site")
                call_word = int(call["word"], 16) if isinstance(call["word"], str) and len(call["word"]) == 8 else -1
                if ("size_site" in read or not (site == call_site + 4 or call_site - TRANSFER_SIZE_REACH <= site < call_site)
                        or not low <= call_site < high or call_word >> 26 != 3 or _u32(original, call_site) != call_word):
                    raise AssetFormatError(f"Consumer transfer {site:#x} is not next to its pinned call")
                if (op != _ADDIU or rs != 0 or not 4 <= (word >> 16) & 31 <= 7 or imm & 0x8000
                        or type(width) is not int or imm != width or offset != 0):
                    raise AssetFormatError(f"Consumer transfer {site:#x} is not an addiu aN, zero, size argument")
                if site < call_site:
                    # The size the callee receives (SolReviewAssetStack F2 P2-1): every path to the call
                    # runs the size instruction and keeps its register through the delay slot; a size
                    # further back than the instruction before the call needs straight-line code too.
                    if (flow is None or not _received(flow, (word >> 16) & 31, call_site, site)
                            or any(_control(_u32(original, at), at, None) for at in range(site + 4, call_site, 4))):
                        raise AssetFormatError(f"Consumer transfer {site:#x} does not reach its call unchanged on every path")
            elif fetch:
                pass
            elif op in _ACCESS:
                if width != _ACCESS[op] or "size_site" in read:
                    raise AssetFormatError(f"Consumer access {site:#x} width disagrees with its opcode")
            elif op == _ADDIU and rs != 0:
                if width is not None and (type(width) is not int or width <= 0):
                    raise AssetFormatError(f"Consumer address {site:#x} has an invalid width")
                if "size_site" in read:
                    size_site, scale = _nat(read["size_site"], "size_site"), _nat(read["size_scale"], "size_scale")
                    size_word = _u32(original, size_site) if size_site % 4 == 0 and low <= size_site < high else 0
                    if (size_word >> 26 != _ADDIU or (size_word >> 21) & 31 or (size_word & 0xFFFF) & 0x8000
                            or (size_word & 0xFFFF) * scale != width):
                        raise AssetFormatError(f"Consumer address {site:#x} size operand disagrees")
            else:
                raise AssetFormatError(f"Consumer access {site:#x} is not a load, store or member address")
            if not transfer and not fetch and (imm & 0x8000 or imm != offset + disp):
                raise AssetFormatError(f"Consumer access {site:#x} offset disagrees")
            group = instances.get(name)
            if not group:
                continue
            for base, members in group.items():
                target = base + offset
                index = next((i for i, m in enumerate(members) if m.start == target), None)
                if index is None:
                    raise AssetFormatError(f"{name} at {base:#x} has no field at +{offset:#x} ({site:#x})")
                if width is None:
                    continue
                stop = target + width
                while members[index].end < stop:
                    if index + 1 == len(members) or members[index + 1].start != members[index].end:
                        raise AssetFormatError(f"{name} at {base:#x} fields do not cover +{offset:#x}/{width}")
                    index += 1
                if members[index].end != stop:
                    raise AssetFormatError(f"{name} at {base:#x} fields do not end at +{offset:#x}+{width}")
                if transfer and members[-1].end != stop:
                    raise AssetFormatError(f"{name} at {base:#x} is larger than its {width}-byte transfer")
            matched.add((site, name))
    return matched


def _interval(original: bytes, row: object, rows: Mapping[Interval, tuple[str, str]], label: str,
              base_rows: Mapping[Interval, tuple[str, str]] | None = None,
              *, context: bool = False) -> tuple[Interval, Interval | None]:
    """An exact map row, and for a range record with "within" its pre-split parent row.

    A range record names an exact (post-split) row; with "within" {start, end} it is a proper
    sub-range of that exact pre-split row of the same class and ref. A context record names an exact
    post-split row or an exact pre-split row (so a context never follows split edges).
    """
    row = _record(row, label, {"start", "end", "class", "ref", "sha256"},
                  frozenset() if context else frozenset({"within"}))
    interval = (_nat(row["start"], "start"), _nat(row["end"], "end"))
    kind = (row["class"], row["ref"])
    base_rows = rows if base_rows is None else base_rows
    within = None
    if "within" in row:
        parent = _record(row["within"], f"{label} within", {"start", "end"})
        within = (_nat(parent["start"], f"{label} within start"), _nat(parent["end"], f"{label} within end"))
        if base_rows.get(within) != kind or not within[0] <= interval[0] < interval[1] <= within[1] or within == interval:
            raise AssetFormatError(f"{label} {interval[0]:#x}..{interval[1]:#x} is not a proper sub-range of "
                                   f"that exact map row")
    if rows.get(interval) != kind and not (context and base_rows.get(interval) == kind):
        raise AssetFormatError(f"{label} {interval[0]:#x}..{interval[1]:#x} is not that exact map row")
    if hashlib.sha256(original[interval[0]:interval[1]]).hexdigest() != row["sha256"]:
        raise AssetFormatError(f"{label} {interval[0]:#x} content changed")
    return interval, within


def _anchor_value(original: bytes, anchor: Mapping[str, Any], catalogue: Mapping[tuple[str, int, int], str],
                  immediate: Immediate, tables: Mapping[str, Mapping[str, Any]], label: str,
                  owner_words: Mapping[str, Mapping[int, tuple[int, int]]] | None = None) -> int:
    """Value an anchor forms: a code immediate inside a catalogued function, an owner-table pair
    word, or an owner word of a map rule (its start or end, as the rule walk resolves it)."""
    site = _nat(anchor.get("site"), f"{label} site")
    if anchor.get("kind") == "immediate":
        _record(anchor, label, {"kind", "site", "function", "target", "index", "edge"})
        low, high = _check_function(original, _record(anchor["function"], f"{label} function",
                                                      {"symbol", "rom_start", "size", "sha256"}), catalogue, label)
        if not low <= site < high:
            raise AssetFormatError(f"{label} {site:#x} lies outside its function")
        return immediate(original, site)
    if anchor.get("kind") == "table_word":
        _record(anchor, label, {"kind", "site", "table", "target", "index", "edge"})
        table = tables.get(anchor["table"])
        if table is None or not table["rom_start"] <= site < table["rom_end"]:
            raise AssetFormatError(f"{label} {site:#x} is not inside owner table {anchor['table']!r}")
        offset = (site - table["rom_start"]) % table["record_size"] - (4 if anchor["edge"] == "end" else 0)
        if site % 4 or offset not in table["pair_offsets"]:
            raise AssetFormatError(f"{label} {site:#x} is not a {anchor['edge']} word of an owner pair")
        return _u32(original, site)
    if anchor.get("kind") == "owner_word":
        _record(anchor, label, {"kind", "site", "rule", "target", "index", "edge"})
        words = (owner_words or {}).get(anchor["rule"]) if isinstance(anchor["rule"], str) else None
        if words is None or site not in words or anchor["edge"] not in ("start", "end"):
            raise AssetFormatError(f"{label} {site:#x} is not an owner word of rule {anchor['rule']!r}")
        return words[site][anchor["edge"] == "end"]
    raise AssetFormatError(f"{label} kind is unsupported")


def replay(original: bytes, document: object, functions: Sequence[Mapping[str, Any]],
           rows: Mapping[Interval, tuple[str, str]], immediate: Immediate, target_sha256: str,
           tables: Mapping[str, Mapping[str, Any]] | None = None,
           owner_words: Mapping[str, Mapping[int, tuple[int, int]]] | None = None,
           base_rows: Mapping[Interval, tuple[str, str]] | None = None) -> dict[Interval, str]:
    """Replay every witness; {closed interval: witness id}. Raises AssetFormatError.

    Anchors are {"kind": "immediate", "site", "function": {symbol, rom_start, size, sha256}, ...}
    (a code immediate inside an unchanged catalogued function, resolved by `immediate`),
    {"kind": "table_word", "site", "table": rule id, ...} (a start/end word of a pair in an owner
    table rule, given by `tables`) or {"kind": "owner_word", "site", "rule": rule id, ...} (a word
    that the caller's validated walk of that map rule resolves to a (start, end) window, given by
    `owner_words` as {rule id: {site: (start, end)}}); all carry "target" (range|context|within),
    "index" and "edge". `rows` is the (split) map partition and `base_rows` the partition before
    witness sub-range splits (default `rows`). A range record with "within" closes a proper
    sub-range of a pre-split row; a "within" anchor resolves against ranges[index]'s parent row and,
    as a start anchor, anchors that sub-range. Its parser receives params["within"]: the parent of
    each range, or None for a whole row.
    """
    tables = tables or {}
    doc = _record(document, "Asset format evidence", {"schema", "target_sha256", "formats", "witnesses"})
    if doc["schema"] != SCHEMA or doc["target_sha256"] != target_sha256:
        raise AssetFormatError("Asset format evidence does not describe the fixed target")
    catalogue = {(f["symbol"], f["rom_start"], f["size"]): f["sha256"] for f in functions}
    vrams = {(f["symbol"], f["rom_start"], f["size"]): f["vram_start"] for f in functions
             if type(f.get("vram_start")) is int}
    calls = CallIndex(functions)
    formats = doc["formats"]
    if not isinstance(formats, dict) or not isinstance(doc["witnesses"], list):
        raise AssetFormatError("Asset format evidence is malformed")
    for name, fmt in formats.items():
        _record(fmt, f"Format {name}", {"consumers", "note"})
        if name not in PARSERS or not isinstance(fmt["consumers"], list) or not fmt["consumers"]:
            raise AssetFormatError(f"Format {name} has no registered parser or no consumer")
    closed: dict[Interval, str] = {}
    used: set[Interval] = set()
    ids: set[str] = set()
    matched: dict[str, set[tuple[int, str]]] = {name: set() for name in formats}
    for record in doc["witnesses"]:
        _record(record, "Witness", {"id", "format", "ranges", "context", "anchors", "params", "span_count",
                                    "span_digest", "note"})
        wid = record["id"]
        if not isinstance(wid, str) or not wid or wid in ids or record["format"] not in formats:
            raise AssetFormatError(f"Witness {wid!r} identity or format is invalid")
        ids.add(wid)
        named = [_interval(original, r, rows, f"{wid} range", base_rows) for r in record["ranges"]]
        ranges, withins = [interval for interval, _ in named], [within for _, within in named]
        context = [_interval(original, r, rows, f"{wid} context", base_rows, context=True)[0] for r in record["context"]]
        if not ranges or len(set(ranges + context)) != len(ranges) + len(context) or used & set(ranges):
            raise AssetFormatError(f"Witness {wid} ranges are empty, repeated or already closed")
        anchored: set[Interval] = set()
        for anchor in record["anchors"]:
            label = f"{wid} anchor"
            if not isinstance(anchor, dict) or anchor.get("edge") not in ("start", "end"):
                raise AssetFormatError(f"{label} edge is invalid")
            pool = {"range": ranges, "context": context, "within": withins}.get(anchor.get("target"))
            index = anchor.get("index")
            if pool is None or type(index) is not int or not 0 <= index < len(pool) or pool[index] is None:
                raise AssetFormatError(f"{label} target is invalid")
            interval = pool[index]
            if (_anchor_value(original, anchor, catalogue, immediate, tables, label, owner_words)
                    != interval[anchor["edge"] == "end"]):
                raise AssetFormatError(f"{label} {anchor['site']:#x} does not form its interval")
            if anchor["edge"] == "start":
                anchored.add(ranges[index] if anchor["target"] == "within" else interval)
        if not set(ranges) <= anchored:
            raise AssetFormatError(f"{wid}: every closed range needs a code-immediate or owner-table start anchor")
        params = record["params"]
        if not isinstance(params, dict) or "context" in params or "within" in params:
            raise AssetFormatError(f"{wid} params are invalid")
        extra = {"within": tuple(withins)} if any(withins) else {}
        try:
            spans = PARSERS[record["format"]](original, tuple(ranges),
                                               MappingProxyType({**params, "context": tuple(context), **extra}))
        except AssetFormatError:
            raise
        except (struct.error, IndexError, KeyError, TypeError, ValueError) as error:
            raise AssetFormatError(f"{wid} parser failed: {error}") from error
        ordered = check_tiling(original, spans, ranges)
        sites = check_reads(original, formats[record["format"]]["consumers"], ordered, catalogue, vrams, calls)
        if not sites:
            raise AssetFormatError(f"{wid} matches no consumer access")
        matched[record["format"]] |= sites
        if len(ordered) != record["span_count"] or span_digest(ordered) != record["span_digest"]:
            raise AssetFormatError(f"{wid} span inventory differs from the reviewed witness")
        for interval in ranges:
            closed[interval] = wid
        used |= set(ranges)
    for name, fmt in formats.items():
        pinned = {(read["site"], read["struct"]) for consumer in fmt["consumers"] for read in consumer["reads"]}
        if not matched[name] or pinned - matched[name]:
            raise AssetFormatError(f"Format {name} has a consumer access that no witness exercises")
    return closed


# ---------------------------------------------------------------------------- libmus banks

PTRBANK_MAGIC = b"N64 PtrTablesV2\0"
WAVEBANK_MAGIC = b"N64 WaveTables \0"
PTRBANK_HEADER = 0x30
WAVETABLE_SIZE = 20
LOOP_SIZE = 44
ADPCM_FRAME = 9      # bytes per 16-sample frame (libultra ADPCMFBYTES)
ADPCM_SAMPLES = 16   # samples per frame (ADPCMFSIZE)
ADPCM_ORDER = 2
WAVE_ALIGN = 16      # sample data alignment inside the wave bank
# Alignment of each bank structure (relative to the bank start); fill before a structure is minimal.
_BANK_ALIGN = {"bank": 16, "wavetable": 8, "book": 8, "loop": 4, "wave_slot": 4, "semitone": 1, "tune": 4}


def _libmus_bank(original: bytes, ranges: tuple[Interval, ...], params: Mapping[str, Any]) -> list[Span]:
    """libmus "N64 PtrTablesV2" bank, and optionally its "N64 WaveTables" sample bank.

    `ranges` is (ptr bank,) with the wave bank as the single context interval, or
    (ptr bank, wave bank). Layout (consumers func_8012BDD0, func_8012C300, func_8012EEA4,
    func_8012EA60): a 0x30-byte header {magic[16], flags, pad[12], count, semitones, tunes, waves};
    `count` wave-slot offsets to 20-byte wave tables {base, len, type, flags, pad[2], loop, book};
    ADPCM books {order, npredictors, s16 coefficients[order * npredictors * 8]}; 44-byte loops
    {start, end, count, s16 state[16]}; `count` semitone bytes; `count` 4-byte tunes {s8 cents,
    3 reserved}. Wave bases are offsets into the wave bank; every sample is a run of 9-byte
    ADPCM frames whose predictor index (low header nibble) is below its book's npredictors,
    except the wave-bank offsets listed in params["tool_record_samples"], whose bytes must parse
    completely as `_tool_records` (and which may only be listed when the wave bank is closed).
    """
    if len(ranges) == 2 and not params["context"]:
        (p0, p1), (w0, w1), close_wave = ranges[0], ranges[1], True
    elif len(ranges) == 1 and len(params["context"]) == 1:
        (p0, p1), (w0, w1), close_wave = ranges[0], params["context"][0], False
    else:
        raise AssetFormatError("libmus bank takes (ptr bank, wave bank) or (ptr bank) with the wave bank as context")
    tool_samples = params.get("tool_record_samples", [])
    if (not set(params) <= {"context", "tool_record_samples"} or not isinstance(tool_samples, list)
            or any(type(v) is not int for v in tool_samples) or len(set(tool_samples)) != len(tool_samples)
            or (tool_samples and not close_wave)):
        raise AssetFormatError("libmus bank parameters are only distinct tool_record_samples of a closed wave bank")
    size = p1 - p0
    typed: list[Span] = []

    def field(offset: int, length: int, name: str, base: int) -> None:
        if not (0 <= base <= offset and 0 < length and offset + length <= size):
            raise AssetFormatError(f"libmus field {name} at +{offset:#x} escapes the bank")
        typed.append(Span(p0 + offset, p0 + offset + length, name, p0 + base))

    def word(offset: int) -> int:
        if not 0 <= offset <= size - 4:
            raise AssetFormatError(f"libmus offset +{offset:#x} escapes the bank")
        return _u32(original, p0 + offset)

    if original[p0:p0 + 16] != PTRBANK_MAGIC or word(0x10) != 0 or any(original[p0 + 0x14:p0 + 0x20]):
        raise AssetFormatError("libmus ptr bank header is not an unrelocated PtrTablesV2 header")
    count, semitones, tunes, waves = word(0x20), word(0x24), word(0x28), word(0x2C)
    if not 0 < count < 4096:
        raise AssetFormatError("libmus wave count is implausible")
    for offset, length, name in ((0, 16, "bank.magic"), (0x10, 4, "bank.flags"), (0x20, 4, "bank.count"),
                                 (0x24, 4, "bank.semitones"), (0x28, 4, "bank.tunes"), (0x2C, 4, "bank.waves")):
        field(offset, length, name, 0)
    typed.append(Span(p0 + 0x14, p0 + 0x20, PAD, p0 + 0x14))
    tables: dict[int, None] = {}
    for i in range(count):
        slot = waves + 4 * i
        field(slot, 4, "wave_slot.offset", slot)
        tables.setdefault(word(slot))
        field(semitones + i, 1, "semitone.value", semitones + i)
        field(tunes + 4 * i, 1, "tune.cents", tunes + 4 * i)
        field(tunes + 4 * i + 1, 3, "tune.reserved", tunes + 4 * i)
        if any(original[p0 + tunes + 4 * i + 1:p0 + tunes + 4 * i + 4]):
            raise AssetFormatError("libmus tune reserved bytes are not the unconverted file state")
    books: dict[int, int] = {}
    loops: set[int] = set()
    extents: dict[tuple[int, int], int] = {}
    for table in tables:
        base, length, kind, flags = word(table), word(table + 4), original[p0 + table + 8], original[p0 + table + 9]
        loop, book = word(table + 12), word(table + 16)
        if kind != 0 or flags != 0 or any(original[p0 + table + 10:p0 + table + 12]) or not book:
            raise AssetFormatError("libmus wave table is not an unrelocated ADPCM entry")
        for offset, width, name in ((0, 4, "wavetable.base"), (4, 4, "wavetable.len"), (8, 1, "wavetable.type"),
                                    (9, 1, "wavetable.flags"), (12, 4, "wavetable.loop"), (16, 4, "wavetable.book")):
            field(table + offset, width, name, table)
        typed.append(Span(p0 + table + 10, p0 + table + 12, PAD, p0 + table + 10))
        if book not in books:
            order, predictors = word(book), word(book + 4)
            if order != ADPCM_ORDER or not 0 < predictors <= 16:
                raise AssetFormatError("libmus ADPCM book is not order-2 with 1..16 predictors")
            field(book, 4, "book.order", book)
            field(book + 4, 4, "book.npredictors", book)
            field(book + 8, 16 * order * predictors, "book.coefficients", book)
            books[book] = predictors
        if length <= 0 or length % ADPCM_FRAME or base & 0xFF000000:
            raise AssetFormatError("libmus wave length is not whole ADPCM frames at a bank offset")
        samples = length // ADPCM_FRAME * ADPCM_SAMPLES
        if loop:
            start, end = word(loop), word(loop + 4)
            if not start < end <= samples:
                raise AssetFormatError("libmus loop does not lie inside its wave")
            if loop not in loops:
                for offset, width, name in ((0, 4, "loop.start"), (4, 4, "loop.end"), (8, 4, "loop.count"),
                                            (12, 32, "loop.state")):
                    field(loop + offset, width, name, loop)
                loops.add(loop)
        key = (base, length)
        extents[key] = min(extents.get(key, books[book]), books[book])
    # Alignment fill between bank structures is minimal and becomes `pad` (the framework checks zero).
    typed.sort()
    spans: list[Span] = []
    cursor = p0
    for s in typed:
        if s.start < cursor:
            raise AssetFormatError(f"libmus bank structures overlap at {s.start:#x}")
        if s.start > cursor:
            align = _BANK_ALIGN.get(s.field.split(".")[0], 1) if s.field != PAD else 1
            if (s.start - p0) % align or s.start - cursor >= align or s.base != s.start:
                raise AssetFormatError(f"libmus bank gap {cursor:#x}..{s.start:#x} is not minimal alignment fill")
            spans.append(Span(cursor, s.start, PAD, cursor))
        spans.append(s)
        cursor = s.end
    if cursor != p1:
        raise AssetFormatError("libmus bank structures do not end at the bank end")
    # Wave extents: disjoint, WAVE_ALIGN-aligned after the 16-byte header, minimal fill, and the
    # final fill reaches the wave bank end. This types the ptr bank's base/len fields.
    wave_size = w1 - w0
    cursor = len(WAVEBANK_MAGIC)
    gaps: list[Interval] = []
    for base, length in sorted(extents):
        if base < cursor or base % WAVE_ALIGN or base - cursor >= WAVE_ALIGN or base + length > wave_size:
            raise AssetFormatError(f"libmus wave {base:#x} is not the next aligned sample in the wave bank")
        if base > cursor:
            gaps.append((cursor, base))
        cursor = base + length
    if (wave_size - cursor) >= WAVE_ALIGN or wave_size % WAVE_ALIGN or w0 % WAVE_ALIGN:
        raise AssetFormatError("libmus samples do not reach the aligned wave bank end")
    if cursor < wave_size:
        gaps.append((cursor, wave_size))
    if close_wave:
        if original[w0:w0 + 16] != WAVEBANK_MAGIC:
            raise AssetFormatError("libmus wave bank header is not N64 WaveTables")
        spans.append(Span(w0, w0 + 16, "wavebank.magic", w0))
        tool_bases = set(tool_samples)
        for (base, length), predictors in sorted(extents.items()):
            if base in tool_bases:
                spans.extend(_tool_records(original, w0 + base, w0 + base + length))
                tool_bases.discard(base)
                continue
            headers = original[w0 + base:w0 + base + length:ADPCM_FRAME]
            if max(b & 15 for b in headers) >= predictors:
                raise AssetFormatError(f"libmus sample {base:#x} has an ADPCM frame predictor beyond its book")
            spans.append(Span(w0 + base, w0 + base + length, "sample.adpcm_frames", w0 + base))
        if tool_bases:
            raise AssetFormatError("libmus tool_record_samples names an offset that is not a wave extent")
        spans.extend(Span(w0 + a, w0 + b, PAD, w0 + a) for a, b in gaps)
    return spans


TOOL_ORDER = 2


def _tool_records(original: bytes, start: int, end: int) -> list[Span]:
    """PC sound-tool sample records written into one sample slot, truncated at the slot end.

    Each record is little-endian chunks: "SAMP" size, "NAME" (NUL-terminated .aif path),
    "INFO" {reserved 0, adpcm_bytes, word, timestamp}, "CDBK" {order 2, npredictors,
    s16 coefficients[order * npredictors * 8]}, optional "LOOP" {start, end, count, state[32]}
    and "DATA" sized adpcm_bytes rounded up to even. SAMP size counts NAME, INFO, CDBK and DATA
    in full and LOOP by its 8-byte header only (the one LOOP record observed). A record either
    carries its whole frames (each predictor below npredictors) or none, the next record
    following at once; an odd adpcm_bytes adds one zero pad byte after the frames (or in their
    place). The last record may be cut by the slot end: its whole frames are typed
    `data_frames_truncated` and any final partial frame `data_partial_frame_truncated`; its
    declared sizes then run past the slot. One zero byte may precede the first record.
    """
    spans: list[Span] = []

    def le(offset: int) -> int:
        if offset + 4 > end:
            raise AssetFormatError(f"tool record field at {offset:#x} runs past the slot")
        return struct.unpack_from("<I", original, offset)[0]

    def chunk(offset: int, tag: bytes, member: str, base: int) -> int:
        if original[offset:offset + 4] != tag or offset + 8 > end:
            raise AssetFormatError(f"tool record at {base:#x} lacks its {tag.decode()} chunk at {offset:#x}")
        size = le(offset + 4)
        spans.append(Span(offset, offset + 4, f"toolrec.{member}_tag", base))
        spans.append(Span(offset + 4, offset + 8, f"toolrec.{member}_size", base))
        return size

    def field(offset: int, length: int, member: str, base: int) -> None:
        if not start <= offset < offset + length <= end:
            raise AssetFormatError(f"tool record field {member} at {offset:#x} runs past the slot")
        spans.append(Span(offset, offset + length, f"toolrec.{member}", base))

    def frames_ok(offset: int, length: int, predictors: int) -> bool:
        headers = original[offset:offset + length:ADPCM_FRAME]
        return not headers or max(b & 15 for b in headers) < predictors

    pos = start
    if original[pos] == 0 and original[pos + 1:pos + 5] == b"SAMP":
        spans.append(Span(pos, pos + 1, PAD, pos))
        pos += 1
    records = 0
    while pos < end:
        record = pos
        samp_size = chunk(record, b"SAMP", "samp", record)
        cursor = record + 8
        name_size = chunk(cursor, b"NAME", "name", record)
        path = original[cursor + 8:cursor + 8 + name_size]
        if (name_size < 6 or len(path) != name_size or path[-1] != 0 or b"\0" in path[:-1]
                or not path[:-1].lower().endswith(b".aif") or any(not 0x20 <= b < 0x7F for b in path[:-1])):
            raise AssetFormatError(f"tool record at {record:#x} NAME is not a NUL-terminated .aif path")
        field(cursor + 8, name_size, "name_path", record)
        cursor += 8 + name_size
        if chunk(cursor, b"INFO", "info", record) != 16 or le(cursor + 8) != 0:
            raise AssetFormatError(f"tool record at {record:#x} INFO is not 16 bytes with a zero first word")
        adpcm = le(cursor + 12)
        for offset, member in ((8, "info_reserved"), (12, "info_adpcm_bytes"), (16, "info_word2"), (20, "info_timestamp")):
            field(cursor + offset, 4, member, record)
        cursor += 24
        book_size = chunk(cursor, b"CDBK", "cdbk", record)
        order, predictors = le(cursor + 8), le(cursor + 12)
        if order != TOOL_ORDER or not 0 < predictors <= 16 or book_size != 8 + 16 * order * predictors:
            raise AssetFormatError(f"tool record at {record:#x} CDBK is not an order-2 codebook")
        field(cursor + 8, 4, "cdbk_order", record)
        field(cursor + 12, 4, "cdbk_npredictors", record)
        field(cursor + 16, book_size - 8, "cdbk_coefficients", record)
        cursor += 8 + book_size
        if adpcm <= 0 or adpcm % ADPCM_FRAME:
            raise AssetFormatError(f"tool record at {record:#x} INFO byte count is not whole ADPCM frames")
        counted = (8 + name_size) + 24 + (8 + book_size)
        if original[cursor:cursor + 4] == b"LOOP":
            if chunk(cursor, b"LOOP", "loop", record) != LOOP_SIZE:
                raise AssetFormatError(f"tool record at {record:#x} LOOP is not 44 bytes")
            loop_start, loop_end = le(cursor + 8), le(cursor + 12)
            if not loop_start < loop_end <= adpcm // ADPCM_FRAME * ADPCM_SAMPLES:
                raise AssetFormatError(f"tool record at {record:#x} LOOP lies outside its frames")
            for offset, length, member in ((8, 4, "loop_start"), (12, 4, "loop_end"), (16, 4, "loop_count"),
                                           (20, 32, "loop_state")):
                field(cursor + offset, length, member, record)
            cursor += 8 + LOOP_SIZE
            counted += 8
        data_size = chunk(cursor, b"DATA", "data", record)
        odd = adpcm & 1
        if data_size != adpcm + odd or samp_size != counted + 8 + data_size:
            raise AssetFormatError(f"tool record at {record:#x} SAMP/DATA sizes disagree with INFO")
        payload = cursor + 8
        records += 1
        follows = payload + odd
        if original[follows:follows + 4] == b"SAMP" and original[payload:follows] == bytes(odd):
            if odd:
                field(payload, 1, "data_odd_pad", record)
            pos = follows
            continue
        if payload + data_size <= end:
            if not frames_ok(payload, adpcm, predictors) or original[payload + adpcm:payload + data_size] != bytes(odd):
                raise AssetFormatError(f"tool record at {record:#x} frames are not ADPCM of its codebook")
            field(payload, adpcm, "data_frames", record)
            if odd:
                field(payload + adpcm, 1, "data_odd_pad", record)
            pos = payload + data_size
            continue
        whole = min(adpcm, end - payload) // ADPCM_FRAME * ADPCM_FRAME
        partial = min(adpcm, end - payload) - whole
        if not frames_ok(payload, whole + partial, predictors):
            raise AssetFormatError(f"tool record at {record:#x} truncated frames are not ADPCM of its codebook")
        if whole:
            field(payload, whole, "data_frames_truncated", record)
        if partial:
            field(payload + whole, partial, "data_partial_frame_truncated", record)
        pos = payload + whole + partial
        if pos != end:
            raise AssetFormatError(f"tool record at {record:#x} truncation does not end the slot")
    if not records or pos != end:
        raise AssetFormatError(f"tool records do not exactly fill the slot {start:#x}..{end:#x}")
    return spans


# ---------------------------------------------------------------------------- HVQ2 still pictures

HVQ2_VERSION = b"HVQ2 1.0" + bytes(8)
HVQ2_HEADER = 0x64
# Chunk roles by header slot (offsets[13] at +0x24, then the three fixed-length chunks at +0x58),
# named for the func_8013591C stream they initialise and the callee that reads it.
HVQ2_CHUNKS = ("mode_luma", "mode_chroma", "moderun_luma", "moderun_chroma", "dc_luma", "dc_u", "dc_v",
               "dcrun_luma", "dcrun_u", "dcrun_v", "scale_luma", "scale_u", "scale_v",
               "fixed_luma", "fixed_u", "fixed_v")
HVQ2_BIT_CHUNKS = 13
HVQ2_FIRST_NODE = 0x100  # D_801D1692 restarts here for every tree; leaves are byte symbols
HVQ2_NODES = 0x200       # Tree.nodes[512]
HVQ2_RAW_MODE = 8        # func_801332A4: a block of mode 8 copies 16 raw bytes
HVQ2_BLOCK = 16          # samples per 4x4 block


class _BitReader:
    """MSB-first bits of one chunk payload, as read_bit takes them from its big-endian words."""

    def __init__(self, original: bytes, payload: int, length: int, name: str) -> None:
        self.original, self.payload, self.length, self.name, self.used = original, payload, length, name, 0

    def bit(self) -> int:
        if self.used >= 8 * self.length:
            raise AssetFormatError(f"HVQ2 {self.name} stream at {self.payload:#x} is read past its length")
        byte, index = self.original[self.payload + self.used // 8], self.used % 8
        self.used += 1
        return (byte >> (7 - index)) & 1


_Tree = tuple[int, dict[int, tuple[int, int]]]


def _hvq2_tree(stream: _BitReader) -> _Tree:
    """func_80132050: 1 = internal node (left then right subtree), 0 = an 8-bit leaf symbol."""
    nodes: dict[int, tuple[int, int]] = {}

    def node() -> int:
        if stream.bit():
            index = HVQ2_FIRST_NODE + len(nodes)
            if index >= HVQ2_NODES:
                raise AssetFormatError(f"HVQ2 {stream.name} tree overflows Tree.nodes")
            nodes[index] = (0, 0)
            left = node()
            nodes[index] = (left, node())
            return index
        value = 0
        for _ in range(8):
            value = value << 1 | stream.bit()
        return value

    return node(), nodes


def _hvq2_leaf(tree: _Tree | None, stream: _BitReader) -> int:
    """decode_symbol / find_leaf: one bit per internal node, 1 selects the right child."""
    if tree is None:
        raise AssetFormatError(f"HVQ2 {stream.name} needs a tree its picture does not build")
    node, nodes = tree
    while node >= HVQ2_FIRST_NODE:
        node = nodes[node][stream.bit()]
    return node


def _hvq2_picture(original: bytes, start: int, end: int) -> list[Span]:
    """One HVQ2 still picture decoded exactly as func_8013591C and its callees consume it.

    Header (big-endian): version[16] "HVQ2 1.0", file_size, h_sampling (2), v_sampling (1 or 2),
    width, height, nest_x, nest_y (u16), nest_mode, dc_shift (u8), work_units, offsets[16]. Each
    offset names a chunk {u32 length; payload[length]} padded to 4; the chunks tile the picture
    from +0x64 to file_size, and the range is file_size rounded up to 16 with zero fill. The 13
    bit chunks are MSB-first word streams (Huffman trees, block modes, DC deltas, scales); the 3
    fixed chunks hold per block either `mode` 16-bit basis codes or, for mode 8, 16 raw bytes.
    The planes, DC values, scales and fixed items are decoded in the game's order; every stream
    must be consumed to its last byte (unread low bits zero) and every fixed chunk exactly.
    work_units (not read by the decoder) must equal the sum over blocks of 1 + mode (2 for a raw
    block), and the nest window func_80132C5C copies must lie inside the luma plane.
    """
    spans: list[Span] = []

    def field(offset: int, length: int, member: str) -> None:
        spans.append(Span(start + offset, start + offset + length, f"hvq2.{member}", start))

    if end - start < HVQ2_HEADER or original[start:start + 16] != HVQ2_VERSION:
        raise AssetFormatError(f"HVQ2 picture at {start:#x} lacks its version string")
    size = _u32(original, start + 0x10)
    if not HVQ2_HEADER <= size <= end - start or (size + 15) & ~15 != end - start:
        raise AssetFormatError(f"HVQ2 picture at {start:#x} file_size does not fill its range")
    h_sampling, v_sampling, width, height, nest_x, nest_y, nest_mode, shift, work = struct.unpack_from(
        ">BBHHHHBBI", original, start + 0x14)
    offsets = struct.unpack_from(">16I", original, start + 0x24)
    if h_sampling != 2 or v_sampling not in (1, 2) or not width or not height or width % 8 \
            or height % (4 * v_sampling) or shift > 8:
        raise AssetFormatError(f"HVQ2 picture at {start:#x} geometry is not one the decoder handles")
    luma_w, luma_h = width >> 2, height >> 2
    nest_w, nest_h = (70, 38) if nest_mode == 8 else (38, 70)
    if nest_x + min(luma_w, nest_w) > luma_w or nest_y + min(luma_h, nest_h) > luma_h:
        raise AssetFormatError(f"HVQ2 picture at {start:#x} nest window leaves the luma plane")
    for offset, length, member in ((0, 16, "version"), (0x10, 4, "file_size"), (0x14, 1, "h_sampling"),
                                   (0x15, 1, "v_sampling"), (0x16, 2, "width"), (0x18, 2, "height"),
                                   (0x1A, 2, "nest_x"), (0x1C, 2, "nest_y"), (0x1E, 1, "nest_mode"),
                                   (0x1F, 1, "dc_shift"), (0x20, 4, "work_units")):
        field(offset, length, member)
    for index, name in enumerate(HVQ2_CHUNKS):
        field(0x24 + 4 * index, 4, f"{name}_offset")

    lengths = [0] * len(HVQ2_CHUNKS)
    cursor = HVQ2_HEADER
    for index in sorted(range(len(HVQ2_CHUNKS)), key=lambda i: offsets[i]):
        if offsets[index] != cursor or cursor + 4 > size:
            raise AssetFormatError(f"HVQ2 picture at {start:#x} chunks do not tile it at +{cursor:#x}")
        chunk = start + cursor
        lengths[index] = _u32(original, chunk)
        spans.append(Span(chunk, chunk + 4, "hvq2_chunk.length", chunk))
        cursor = (cursor + 4 + lengths[index] + 3) & ~3
        if cursor > size:
            raise AssetFormatError(f"HVQ2 picture at {start:#x} chunk {HVQ2_CHUNKS[index]} runs past file_size")
    if cursor != size:
        raise AssetFormatError(f"HVQ2 picture at {start:#x} chunks do not end at file_size")
    if size < end - start:
        spans.append(Span(start + size, end, PAD, start + size))

    streams = [_BitReader(original, start + offsets[i] + 4, lengths[i], HVQ2_CHUNKS[i])
               for i in range(HVQ2_BIT_CHUNKS)]

    def tree(index: int) -> _Tree | None:
        return _hvq2_tree(streams[index]) if lengths[index] else None

    mode_tree, run_tree, dc_tree, scale_tree = tree(0), tree(2), tree(4), tree(10)
    area = luma_w * luma_h
    half_w = luma_w >> 1
    chroma_h = luma_h >> 1 if v_sampling == 2 else luma_h
    chroma_area = half_w * chroma_h

    # func_80132324: block modes with zero runs, luma then packed chroma (U low, V high nibble).
    luma: list[int] = []
    while len(luma) < area:
        value = _hvq2_leaf(mode_tree, streams[0])
        luma += [0] * (_hvq2_leaf(run_tree, streams[2]) + 1) if value == 0 else [value]
    chroma_u: list[int] = []
    chroma_v: list[int] = []
    while len(chroma_u) < chroma_area:
        code = _hvq2_leaf(mode_tree, streams[1])
        if code == 0:
            run = _hvq2_leaf(run_tree, streams[3]) + 1
            chroma_u += [0] * run
            chroma_v += [0] * run
        else:
            chroma_u.append(code & 15)
            chroma_v.append(code >> 4)
    if len(luma) != area or len(chroma_u) != chroma_area:
        raise AssetFormatError(f"HVQ2 picture at {start:#x} mode run overruns its plane")

    # func_801328A8 -> func_80132684: one DC call per block; zero opens a run, extremes escape.
    high, low = 0x7F << shift, -(0x80 << shift)
    for channel, calls in ((0, area), (1, chroma_area), (2, chroma_area)):
        values, runs, pending = streams[4 + channel], streams[7 + channel], 0
        for _ in range(calls):
            if pending:
                pending -= 1
                continue
            value = ((_hvq2_leaf(dc_tree, values) ^ 0x80) - 0x80) << shift
            if value == 0:
                pending = _hvq2_leaf(run_tree, runs)
            elif value in (low, high):
                while True:
                    step = ((_hvq2_leaf(dc_tree, values) ^ 0x80) - 0x80) << shift
                    if low < step < high:
                        break

    # func_8013517C order: per macroblock two luma blocks of each luma row it covers, then U, V.
    order: list[list[int]] = [[], [], []]
    for row in range(chroma_h):
        for column in range(half_w):
            for luma_row in range(row * v_sampling, row * v_sampling + v_sampling):
                order[0] += luma[luma_row * luma_w + 2 * column:luma_row * luma_w + 2 * column + 2]
            order[1].append(chroma_u[row * half_w + column])
            order[2].append(chroma_v[row * half_w + column])
    units = sum(1 + (2 if mode == HVQ2_RAW_MODE else mode) for modes in order for mode in modes)
    if units != work:
        raise AssetFormatError(f"HVQ2 picture at {start:#x} work_units {work} is not its block total {units}")

    # func_801332A4 / func_80132E00: per coded block, one scale symbol and one basis code per basis.
    for channel, modes in enumerate(order):
        fixed = start + offsets[HVQ2_BIT_CHUNKS + channel] + 4
        item = fixed
        for mode in modes:
            if mode == HVQ2_RAW_MODE:
                spans.extend(Span(item + i, item + i + 1, "hvq2_rawpel.value", item + i) for i in range(HVQ2_BLOCK))
                item += HVQ2_BLOCK
                continue
            for _ in range(mode):
                _hvq2_leaf(scale_tree, streams[10 + channel])
                spans.append(Span(item, item + 2, "hvq2_basis.code", item))
                item += 2
        if item - fixed != lengths[HVQ2_BIT_CHUNKS + channel]:
            raise AssetFormatError(f"HVQ2 picture at {start:#x} {HVQ2_CHUNKS[HVQ2_BIT_CHUNKS + channel]} "
                                   f"is not consumed exactly")
        if (item - start) & 3:
            spans.append(Span(item, item + (-(item - start) & 3), PAD, item))

    for stream in streams:
        length, payload = stream.length, stream.payload
        if (stream.used + 7) // 8 != length:
            raise AssetFormatError(f"HVQ2 {stream.name} stream at {payload:#x} is not consumed to its last byte")
        if stream.used % 8 and original[payload + length - 1] & (0xFF >> stream.used % 8):
            raise AssetFormatError(f"HVQ2 {stream.name} stream at {payload:#x} has unread nonzero bits")
        for word in range(payload, payload + length, 4):
            stop = min(word + 4, payload + length)
            spans.append(Span(word, stop, "hvq2_bits.word", word))
            if stop < word + 4:
                if any(original[stop:word + 4]):
                    raise AssetFormatError(f"HVQ2 {stream.name} stream at {payload:#x} word fill is not zero")
                spans.append(Span(stop, word + 4, "hvq2_bits.fill", word))
    return spans


def _hvq2_still(original: bytes, ranges: tuple[Interval, ...], params: Mapping[str, Any]) -> list[Span]:
    """Each closed range is one whole HVQ2 still picture (`_hvq2_picture`); no parameters."""
    if set(params) != {"context"} or params["context"]:
        raise AssetFormatError("HVQ2 still pictures take no parameters or context")
    return [span for start, end in ranges for span in _hvq2_picture(original, start, end)]


# ---------------------------------------------------------------------------- RLE bit masks

MASK_HEADER = 16
MASK_CAPACITY = 0x2580  # func_80069A68 passes this capacity (li s2, 9600) to func_8005EFF0


def _rle_mask(original: bytes, ranges: tuple[Interval, ...], params: Mapping[str, Any]) -> list[Span]:
    """Packed 1-bit masks: header {u32 pixels_per_byte 8, u32 data_offset 16, u16 width, u16 height,
    u32 rle_bytes}, then func_8005EFF0 codes: c < 0 copies -c literal bytes, c > 0 repeats the next
    byte c + 1 times, 0 ends. The codes must decode exactly width * height / 8 = 0x2580 bytes in
    rle_bytes bytes; the range is the stream end rounded up to 16 with zero fill.
    """
    if set(params) != {"context"} or params["context"]:
        raise AssetFormatError("RLE masks take no parameters or context")
    spans: list[Span] = []
    for start, end in ranges:
        if end - start < MASK_HEADER + 1:
            raise AssetFormatError(f"RLE mask at {start:#x} is shorter than its header")
        per_byte, data, width, height, rle_bytes = struct.unpack_from(">IIHHI", original, start)
        if per_byte != 8 or data != MASK_HEADER or width * height != per_byte * MASK_CAPACITY:
            raise AssetFormatError(f"RLE mask at {start:#x} header does not describe a {MASK_CAPACITY:#x}-byte mask")
        for offset, length, member in ((0, 4, "pixels_per_byte"), (4, 4, "data_offset"), (8, 2, "width"),
                                       (10, 2, "height"), (12, 4, "rle_bytes")):
            spans.append(Span(start + offset, start + offset + length, f"rlemask.{member}", start))
        pos, total = start + data, 0
        while True:
            if pos >= end:
                raise AssetFormatError(f"RLE mask at {start:#x} has no end code")
            code = original[pos]
            if code == 0:
                spans.append(Span(pos, pos + 1, "rle_code.end", pos))
                pos += 1
                break
            if code & 0x80:
                count = 256 - code
                if pos + 1 + count > end:
                    raise AssetFormatError(f"RLE mask literal at {pos:#x} runs past its range")
                spans.append(Span(pos, pos + 1, "rle_code.literal_count", pos))
                spans.extend(Span(pos + 1 + i, pos + 2 + i, "rle_literal.byte", pos + 1 + i) for i in range(count))
                pos += 1 + count
            else:
                count = code + 1
                if pos + 2 > end:
                    raise AssetFormatError(f"RLE mask run at {pos:#x} runs past its range")
                spans.append(Span(pos, pos + 1, "rle_code.run_length", pos))
                spans.append(Span(pos + 1, pos + 2, "rle_run.value", pos + 1))
                pos += 2
            total += count
            if total > MASK_CAPACITY:
                raise AssetFormatError(f"RLE mask at {start:#x} decodes past {MASK_CAPACITY:#x} bytes")
        if total != MASK_CAPACITY or pos - start - data != rle_bytes or (pos - start + 15) & ~15 != end - start:
            raise AssetFormatError(f"RLE mask at {start:#x} does not decode {MASK_CAPACITY:#x} bytes in rle_bytes "
                                   f"and fill its range")
        if pos < end:
            spans.append(Span(pos, end, PAD, pos))
    return spans


PARSERS: dict[str, Parser] = {
    "libmus_bank": _libmus_bank,
    "hvq2_still": _hvq2_still,
    "rle_mask": _rle_mask,
}


# ---------------------------------------------------------------------------- libmus songs and fx banks
#
# Author: opus-08 (Anthropic, claude-opus-5-5, xhigh). Layered on the assetformat framework:
# expects AssetFormatError, Span and PARSERS from the enclosing module.
#
# Grammar source (all matched C in src/units/main_14400; the binary consumers are pinned in
# config/rom-evidence/asset-formats.json):
#   song header / relocation  func_8012C4B0, func_8012C300 (+0x0C..+0x24 file offsets, +0x28 flag)
#   note events               func_8012AF30 (note, optional velocity, optional var-len ticks)
#   commands 0x80..0xAC       D_801487D0 handlers func_801291C0 .. func_80129C20
#   volume / pitch streams    func_8012BA20 / func_8012BADC (no terminator; value [wait [wait]])
#   envelope / keymap records func_80129240 (7 bytes) / func_8012AF30 keyMap[note] (6 bytes)
#   fx bank                   func_8012A910, func_8012C3E0, func_8012C330, func_8012AF30 (+0x14)
#   loop save/restore         func_801296E4 / func_80129750 (four saved-state slots, level cursors)
#   level read schedule       func_8012ACC0, func_8012AF30, func_8012BA20/BADC, func_80129384,
#                             func_8012C004, func_8012C138, func_8012C4B0, func_80129C2C
#
# r2 (Sol review of r1): keymap notes persist their instrument (func_8012AF30 writes +0xAE), loop
# depth is bounded by the four slots, and the fx remainder byte is framework pad.
# wave 2: a level stream is accepted when the exact integer read schedule of the consumers (frame
# by frame, both master slot orders, to the periodic steady state) reads every one of its events
# and no level read lands on a byte of a non-level region.
# wave 2 r2: the schedule covers both song starts of the game: func_80129EE0 (all voices at time 0)
# and func_80129F10 (each voice first seeks to its 0xAB marker), for every marker id the song holds.
# wave 2 layer B (optional): a level read that runs from the last level region into the master
# stream is accepted only when the witness declares it exactly as `dual_read` (voice, boundary,
# every start/order in which it happens with its reach, every byte and the level event it is
# decoded as, and its cause); those bytes keep their master-stream typing and no region boundary
# depends on the over-read.
# wave 2 r3 (SolReviewSongsWave2-r2): the seek uses func_80129F10's own note state (an explicit
# duration clears readTicks +0xB7 even when defaultTicks is 0; a mapped note leaves +0xAE), a
# selected marker whose duration is the 0x7FFF stop value refuses the start, and every populated
# voice (level streams or not) must return from its seek; executed bytes keep their decoded roles.

_MUS_VAR, _MUS_MARKER = -1, -2
# Operand bytes after each opcode, as consumed by its D_801487D0 handler. _MUS_VAR is the
# handlers' one- or two-byte index (high bit set: ((b & 0x7F) << 8) | next).
_MUS_OPERANDS = {
    0x80: 0, 0x81: _MUS_VAR, 0x82: 1, 0x83: 0, 0x84: 7, 0x85: 1, 0x86: 2, 0x87: 1, 0x88: 3, 0x89: 3,
    0x8A: 0, 0x8B: _MUS_VAR, 0x8C: 0, 0x8D: 1, 0x8E: 0, 0x8F: 1, 0x90: _MUS_VAR, 0x91: 0, 0x92: 0,
    0x93: 0, 0x94: 0, 0x95: 1, 0x96: 0, 0x97: 3, 0x98: 0, 0x99: 0, 0x9A: 0, 0x9B: 1, 0x9C: 1, 0x9D: 2,
    0x9E: _MUS_VAR, 0x9F: 0, 0xA0: 1, 0xA1: 6, 0xA2: 1, 0xA3: 2, 0xA4: 2, 0xA5: 2, 0xA6: 1,
    0xA7: _MUS_VAR, 0xA8: 1, 0xA9: 1, 0xAA: 1, 0xAB: _MUS_MARKER, 0xAC: 0,
}
_MUS_REST = 0x60          # func_8012AF30 NOTE_REST
_MUS_VERSION = 0x215
_MUS_SONG_HEADER = 0x38
_MUS_FX_HEADER = 0x18
_MUS_ENVELOPE = ("rate", "level0", "length1", "level1", "length2", "level2", "length3")  # func_80129240
_MUS_LEVEL_NAME = {"volumes": "volume", "pitches": "pitch"}
_MUS_LOOP_SLOTS = 4       # func_801296E4 stackPtr[4] .. stack70[4], indexed by depth unchecked


def _mus_u32(data: bytes, at: int) -> int:
    return struct.unpack_from(">I", data, at)[0]


def _mus_u16(data: bytes, at: int) -> int:
    return struct.unpack_from(">H", data, at)[0]


class _MusStream:
    """One note/command stream: linear decode for byte roles, then execution for cross-check.

    `song` selects the voice context: song voices start with readVelocity set (func_8012C4B0
    sets +0xD2) and have an owner (+0x74) but no fx definition (+0x78); fx voices start with
    readVelocity clear (func_8012C330 after func_8012C004) and have a definition but no owner.
    """

    def __init__(self, data: bytes, start: int, limit: int, song: bool, label: str, keymap=None) -> None:
        self.data, self.start, self.limit, self.song, self.label = data, start, limit, song, label
        # keymap(index) -> the record's instrument (u16 at +0), or None when the song has no keymaps.
        self.keymap = keymap

    def _fail(self, message: str) -> None:
        raise AssetFormatError(f"{self.label} @{self.start:#x}: {message}")

    def _byte(self, at: int) -> int:
        if not self.start <= at < self.limit:
            self._fail(f"read at {at:#x} leaves the stream region")
        return self.data[at]

    def _step(self, pc: int, st: dict, refs: dict | None) -> tuple[list[tuple[int, str]], int | None, str]:
        """Decode one instruction at pc under state st (mutated). Returns (roles, next_pc, kind)."""
        op = self._byte(pc)
        roles = [(pc, "op" if op & 0x80 else "note")]
        at = pc + 1
        if op & 0x80:
            count = _MUS_OPERANDS.get(op)
            if count is None:
                self._fail(f"opcode {op:#x} has no D_801487D0 handler")
            if op == 0xA1:
                self._fail("jump 0xA1 (func_801298C4) is outside this witness")
            if self.song and op == 0xA7:
                self._fail("0xA7 needs an fx definition (+0x78), which song voices lack")
            if not self.song and op in (0x90, 0x9E):
                self._fail(f"{op:#x} reads owner +0x74, which fx voices lack")
            value = None
            if count == _MUS_VAR:
                high = self._byte(at)
                roles.append((at, f"{op:02x}.index"))
                at += 1
                value = high
                if high & 0x80:
                    value = ((high & 0x7F) << 8) | self._byte(at)
                    roles.append((at, f"{op:02x}.index_low"))
                    at += 1
            elif count == _MUS_MARKER:          # func_80129BB0
                roles += [(at, "ab.value"), (at + 1, "ab.marker")]
                marker = self._byte(at + 1)
                self._byte(at)
                at += 2
                if marker & 0x80:
                    self._byte(at)
                    roles.append((at, "ab.extra"))
                    at += 1
            else:
                for k in range(count):
                    self._byte(at + k)
                    roles.append((at + k, f"{op:02x}.{k}"))
                at += count
            if op == 0x80:
                return roles, None, "end"
            if op == 0x8B:
                st["ticks"] = value
            elif op == 0xAC:
                st["ticks"] = 0
            elif op == 0x8C:
                st["read_ticks"] = 1
            elif op == 0x99:
                st["read_velocity"] = 1
            elif op in (0x9A, 0x9B):
                st["read_velocity"] = 0
            elif op == 0x81:
                st["instrument"] = value
            elif op == 0x9E:
                st["keymap"] = value
            elif op == 0x9F:
                st["keymap"] = None
            if refs is not None:
                if op == 0x90:
                    refs["envelope"].add(value)
                elif op == 0xA7:
                    refs["fx"].add(value)
            if op == 0x95:
                return roles, at, "push"
            if op == 0x96:
                return roles, at, "pop"
            return roles, at, "op"
        if st["read_velocity"]:
            velocity = self._byte(at)
            roles.append((at, "note.velocity"))
            at += 1
            if velocity & 0x80:
                st["read_velocity"] = 0
        if not st["ticks"] or st["read_ticks"]:
            if st["ticks"]:
                st["read_ticks"] = 0
            high = self._byte(at)
            roles.append((at, "note.ticks"))
            at += 1
            st["note_ticks"] = high
            if high & 0x80:
                st["note_ticks"] = ((high & 0x7F) << 8) + self._byte(at)
                roles.append((at, "note.ticks_low"))
                at += 1
        else:
            st["note_ticks"] = st["ticks"]
        # func_8012AF30: with no keymap a song voice (bank +0x74 set) reads instruments[+0xAE] for
        # every event, rests included; an fx voice (alt bank) only for notes. A mapped note first
        # stores keyMap[note].instrument into +0xAE, which persists (0x9F clears only the keymap).
        if op != _MUS_REST and st["keymap"] is not None:
            index = st["keymap"] + op
            if self.keymap is None:
                self._fail(f"mapped note at {pc:#x} without keymap records")
            st["instrument"] = self.keymap(index)
            if refs is not None:
                refs["keymap"].add(index)
                refs["instrument"].add(st["instrument"])
        elif st["keymap"] is None and (op != _MUS_REST or self.song) and refs is not None:
            refs["instrument"].add(st["instrument"])
        return roles, at, "note"

    @staticmethod
    def _state() -> dict:
        return {"read_velocity": 0, "ticks": 0, "read_ticks": 0, "instrument": 0, "keymap": None, "note_ticks": 0}

    def decode(self) -> int:
        """Linear decode to the stream's last instruction; returns its exclusive end.

        Every byte gets a role (op, note or a named operand); execute() checks those roles.
        The stream is typed as two spans: its first event byte, which func_8012AF30 reads with
        lb 0x0(cursor) right after func_8012C4B0/func_8012C330 point the voice cursor at the
        stream start, and the remaining decoded events.
        """
        st = self._state()
        st["read_velocity"] = 1 if self.song else 0
        self.roles: dict[int, str] = {}
        loops: list[int] = []
        pc = self.start
        while True:
            roles, nxt, kind = self._step(pc, st, None)
            self.roles.update(roles)
            if kind == "end":
                return self._typed(roles[-1][0] + 1)
            if kind == "push":
                if len(loops) == _MUS_LOOP_SLOTS:
                    self._fail(f"loop start at {pc:#x} needs a fifth func_801296E4 saved-state slot")
                loops.append(self.data[pc + 1])
            elif kind == "pop":
                if not loops:
                    self._fail(f"loop end at {pc:#x} without a loop start")
                if loops.pop() == 0xFF:      # func_80129750: 0xFF never expires
                    if nxt == self.limit - 1 and self.data[nxt] == 0x80:
                        self.roles[nxt] = "op"      # tool-written terminator after the endless loop
                        self.unreachable_end = nxt
                        return self._typed(nxt + 1)
                    return self._typed(nxt)
            pc = nxt

    def _typed(self, end: int) -> int:
        a = self.start
        self.spans = [Span(a, a + 1, "libmus_stream.first_event", a)]
        if end > a + 1:
            self.spans.append(Span(a + 1, end, "libmus_stream.events", a))
        return end

    def execute(self) -> dict:
        """Run the stream as func_8012AF30 does, following loops; cross-check every byte role."""
        self.unreachable_end = getattr(self, "unreachable_end", None)
        refs = {"envelope": set(), "keymap": set(), "instrument": set(), "fx": set()}
        st = self._state()
        st["read_velocity"] = 1 if self.song else 0
        stack: list[list[int]] = []
        seen: set = set()
        reached: set[int] = set()
        pc = self.start
        while pc is not None:
            key = (pc, st["read_velocity"], st["ticks"], st["read_ticks"], st["instrument"], st["keymap"],
                   tuple((a, c) for a, c in stack))
            if key in seen:
                break
            seen.add(key)
            roles, nxt, kind = self._step(pc, st, refs)
            for at, role in roles:
                if self.roles.get(at) != role:
                    self._fail(f"executed role {role} at {at:#x} differs from the linear decode")
                reached.add(at)
            if kind == "push":                               # func_801296E4
                if len(stack) == _MUS_LOOP_SLOTS:
                    self._fail(f"loop start at {pc:#x} executed with all four saved-state slots in use")
                stack.append([nxt, self.data[pc + 1]])
            elif kind == "pop":                              # func_80129750
                if not stack:
                    self._fail(f"loop end at {pc:#x} executed with an empty loop stack")
                top = stack[-1]
                if top[1] != 0xFF:
                    top[1] = (top[1] - 1) & 0xFF
                    if top[1] == 0:
                        stack.pop()
                        pc = nxt
                        continue
                pc = top[0]
                continue
            pc = nxt
        missing = set(self.roles) - reached - {self.unreachable_end}
        if missing:
            self._fail(f"{len(missing)} decoded bytes are never executed (first {min(missing):#x})")
        return refs


def _mus_level_stream(data: bytes, start: int, end: int, label: str, spans: list) -> tuple[int, int]:
    """Volume/pitch stream (func_8012BA20/func_8012BADC); must end exactly at `end`.

    Returns (W, steps): the events' total ticks and the level step that reads each event
    (func_8012BA20 reads event i at step 1 + the waits before it; the wait starts at 1).

    Typed as its first value byte (func_8012BA20/func_8012BADC lbu 0x0(cursor) once the voice
    cursor is set to the stream start by func_8012C4B0) and the remaining whole events.
    """
    at, ticks, steps = start, 0, []
    while at < end:
        value = data[at]
        at += 1
        if value & 0x80:
            if at >= end:
                raise AssetFormatError(f"{label} @{start:#x}: value without its wait")
            wait = data[at]
            at += 1
            if wait & 0x80:
                if at >= end:
                    raise AssetFormatError(f"{label} @{start:#x}: truncated two-byte wait")
                wait = ((wait & 0x7F) << 8) + data[at] + 2
                at += 1
            else:
                wait += 2
        else:
            wait = 1
        steps.append(ticks + 1)
        ticks += wait
    if at != end:
        raise AssetFormatError(f"{label} @{start:#x}: last event crosses the next region")
    spans.append(Span(start, start + 1, "libmus_level.first_value", start))
    if end > start + 1:
        spans.append(Span(start + 1, end, "libmus_level.events", start))
    return ticks, steps


# ---- exact level read schedule (wave 2) -------------------------------------------------------
_MUS_RATE_HZ = 60          # func_80129C2C: D_801CA6E4 = 60 when osTvType != 0 (NTSC/MPAL, this image)
_MUS_TEMPO_SCALE = 0x80    # voice +0x98 as func_8012C004 leaves it; func_8012A654 (song volume
                           # call) can rescale +0x98/+0x9C later, which this default-scale model excludes
_MUS_MAX_PASSES = 4096     # endless-loop passes examined before the phase must repeat


def _mus_tempo_step(tempo: int) -> int:
    """func_80129384: step9C = (T * 0x6000 / 120) / D_801CA6E4 * field_98 >> 7."""
    return ((tempo * 0x6000 // 120) // _MUS_RATE_HZ) * _MUS_TEMPO_SCALE >> 7


class _MusNoStart(Exception):
    """A func_80129F10 marker start that cannot return: some voice's seek runs an endless loop that
    never reaches the marker, so the call never returns and the song never plays with that marker."""


_MUS_STOP_TICKS = 0x7FFF   # func_8012ACC0: +0x9A == 0x7FFF skips func_8012AF30 for good


def _mus_start_label(marker: int | None) -> str:
    return "func_80129EE0 start" if marker is None else f"func_80129F10 marker {marker} start"


def _mus_timeline(stream: "_MusStream", marker: int | None = None, refs: dict | None = None):
    """Execute a song voice, yielding (note clock, event, arg) for 'tempo' (0x85 operand), 'started'
    (the selected marker's duration), 'push' (depth), 'pop' (restores?), 'pass' (decode state at an
    endless pop) and 'end'. Commands run when func_8012ACC0 calls func_8012AF30, i.e. at the clock
    of the next note.

    func_80129EE0 start (`marker` None): func_8012AF30 throughout. func_80129F10 start: the voice
    first seeks as func_80129F10 does (C 23-56): commands run through their D_801487D0 handlers;
    notes read their velocity and duration and only advance time, with func_80129F10's own state
    rule (an explicit duration clears readTicks +0xB7 whatever defaultTicks is, where func_8012AF30
    clears it only when defaultTicks is nonzero; the keymap note start, which stores +0xAE, is not
    run; a 0x7FFF duration is harmless because func_8012ACC0 is not involved). At the first 0xAB
    whose id is the marker (not executed: func_80129F10 skips its operands) it yields 'started' and
    continues with func_8012AF30. func_80129F10 writes that duration to +0x9A; 0x7FFF there makes
    func_8012ACC0 skip func_8012AF30 permanently while time and level updates go on, so such a start
    is refused, as a 0x7FFF note duration in playback is. Every executed byte must carry the role
    the linear decode gave it; `refs` collects the table references of playback (not the seek)."""
    data, st = stream.data, stream._state()
    st["read_velocity"] = 1
    pc, clock, stack, seeking = stream.start, 0, [], marker is not None
    label = _mus_start_label(marker)
    while True:
        op = data[pc]
        selected = seeking and op == 0xAB and data[pc + 1] == marker
        explicit, instrument = not st["ticks"] or st["read_ticks"], st["instrument"]
        roles, nxt, kind = stream._step(pc, st, None if seeking else refs)
        for at, role in roles:
            if stream.roles.get(at) != role:
                stream._fail(f"executed role {role} at {at:#x} differs from the linear decode ({label})")
        if selected:                                  # func_80129F10 C 57-70: cursor[2..] is the duration
            duration = data[pc + 2]
            if duration & 0x80:
                duration = ((duration & 0x7F) << 8) | data[pc + 3]
            if duration == _MUS_STOP_TICKS:
                stream._fail(f"the selected marker at {pc:#x} loads the 0x7FFF stop value into +0x9A: "
                             f"func_8012ACC0 never runs func_8012AF30 again while its level reads continue ({label})")
            yield clock, "started", duration
            seeking = False
            pc = nxt
            continue
        if kind == "end":
            yield clock, "end", None
            return
        if kind == "note":
            if seeking:
                if explicit:                          # func_80129F10 C 44-53: field_b7 = 0
                    st["read_ticks"] = 0
                st["instrument"] = instrument         # no note start during the seek
            elif st["note_ticks"] == _MUS_STOP_TICKS:  # func_8012ACC0 stops note processing
                stream._fail(f"note at {pc:#x} of 0x7FFF ticks ({label})")
            clock += st["note_ticks"] << 8
        elif op == 0x85:
            yield clock, "tempo", data[pc + 1]
        elif kind == "push":
            if len(stack) == _MUS_LOOP_SLOTS:
                stream._fail(f"loop start at {pc:#x} needs a fifth saved-state slot")
            stack.append([nxt, data[pc + 1]])
            yield clock, "push", len(stack) - 1
        elif kind == "pop":
            top = stack[-1]
            if top[1] != 0xFF:
                top[1] = (top[1] - 1) & 0xFF
                if top[1] == 0:
                    stack.pop()
                    yield clock, "pop", False
                    pc = nxt
                    continue
            if top[1] == 0xFF:
                yield clock, "pass", (tuple(map(tuple, stack)), st["read_velocity"], st["ticks"], st["read_ticks"],
                                      st["instrument"], st["keymap"])
            yield clock, "pop", True
            pc = top[0]
            continue
        pc = nxt


class _MusFrames:
    """Channel-voice frame times t_f (t_0 = 0) under the master's 0x85 commands.

    func_8012C004 starts every voice at time 0 with step9C = 0x6000 / D_801CA6E4; func_8012C690
    unpauses the whole song in one frame. The master (func_8012C138 slot 0..3 when free) runs before
    every channel voice of the frame, so its tempo reaches channels in the same frame
    (`same_frame`); a master stolen into a later slot reaches them one frame later. A tempo command
    setting the step already in effect is a no-op; a real change inside the master's endless loop
    would recur and is refused."""

    def __init__(self, master: "_MusStream", same_frame: bool, marker: int | None = None) -> None:
        period = 0x6000 // _MUS_RATE_HZ
        changes, current, looping, passes = [], period, False, 0
        timeline, origin = _mus_timeline(master, marker), 0
        if marker is not None:
            # func_80129F10: the master seeks to its marker first; 0x85 commands on the way set every
            # voice's step before the first frame. A master without the marker ends silently.
            origin, seen = None, set()
            for trigger, event, arg in timeline:
                if event == "tempo":
                    current = _mus_tempo_step(arg)
                elif event == "started":
                    origin = trigger - (arg << 8)
                    break
                elif event == "end":
                    timeline = iter(())
                    break
                elif event == "pass":
                    if arg in seen:
                        raise _MusNoStart(f"master seek never reaches marker {marker}")
                    seen.add(arg)
            origin = origin or 0
        start_step = current
        for trigger, event, arg in timeline:
            if event == "tempo":
                step = _mus_tempo_step(arg)
                if step != current:
                    if looping:
                        master._fail("tempo changes inside the master's endless loop")
                    changes.append((trigger, step))
                    current = step
            elif event == "pass":
                looping, passes = True, passes + 1
                if passes == 3:
                    break
            elif event == "end":
                break
        self.t, master_t, master_step, step = [0], origin, start_step, start_step
        while changes or len(self.t) < 2:
            master_t += master_step
            applied = None
            while changes and changes[0][0] < master_t:
                applied = master_step = changes.pop(0)[1]
            if same_frame and applied is not None:
                step = applied
            self.t.append(self.t[-1] + step)
            if not same_frame and applied is not None:
                step = applied
        self.step, self.last = step, len(self.t) - 1

    def time(self, f: int) -> int:
        return self.t[f] if f <= self.last else self.t[self.last] + (f - self.last) * self.step

    def frame_after(self, trigger: int) -> int:
        """First frame f >= 1 with t_f > trigger (func_8012ACC0: next3C - time0C < 0)."""
        if trigger >= self.t[self.last]:
            return self.last + (trigger - self.t[self.last]) // self.step + 1
        lo, hi = 1, self.last
        while lo < hi:
            mid = (lo + hi) // 2
            if self.t[mid] > trigger:
                hi = mid
            else:
                lo = mid + 1
        return lo

    def ticks(self, f: int, origin: int = 0, seeked: bool = False) -> int:
        """Level ticks run through frame f by a voice whose time starts at `origin` (a multiple of
        0x100): func_8012ACC0 calls func_8012BA20 only while due14 < time0C, leaving due14 =
        0x100 * ceil(time / 0x100) and never below its start 0. After a func_80129F10 seek
        (`seeked`) the updater was called once unconditionally, and its do-while ran at least one
        tick, so the count is at least 1."""
        return max(1 if seeked else 0, (origin >> 8) + (-(-self.time(f) // 256) if f > 0 else 0))


def _mus_level_segments(stream: "_MusStream", frames: _MusFrames, marker: int | None = None) -> list[tuple[int, int]]:
    """Stream positions read by one channel voice: [(pos0, last)] meaning positions pos0+1..last.

    Positions are level ticks of the voice's own stream. func_801296E4 saves the position reached
    when the push runs (the level update of that frame comes after the notes); func_80129750 restores
    it, starting a new segment; 0x80 ends the voice before that frame's level update. After the
    last tempo change frames are periodic, so the endless loop is followed until the phase of its
    pop on the frame grid repeats (the decode state repeating too).

    With a func_80129F10 `marker` start the voice first seeks: commands and notes run without level
    reads until the first 0xAB whose id is the marker (clock M, duration d); its time starts at
    (M - d) << 8 and one func_8012BA20/func_8012BADC call reads level positions 1..max(1, M-d)
    before the first frame (the updaters are do-while loops). The seek uses func_80129F10's note
    state (_mus_timeline). Loops pushed during the seek saved the untouched level cursor (position
    0). A voice whose seek reaches 0x80 is silent ([]); one that can never reach the marker raises
    _MusNoStart."""
    segments: list[tuple[int, int]] = []
    pos0, tick_base, saved, seen, origin = 0, 0, [], {}, 0
    timeline = _mus_timeline(stream, marker)
    if marker is not None:
        passes, found = set(), False
        for trigger, event, arg in timeline:
            if event == "tempo":
                stream._fail("0x85 in a channel stream changes the song tempo from a channel voice")
            if event == "started":
                origin, found = trigger - (arg << 8), True
                break
            if event == "push":
                del saved[arg:]
                saved.append(0)
            elif event == "pop" and not arg:
                saved.pop()
            elif event == "end":
                return []
            elif event == "pass":
                if arg in passes:
                    raise _MusNoStart(f"voice seek never reaches marker {marker}")
                passes.add(arg)
        if not found:
            stream._fail("voice timeline ended without 0x80")

    seeked = marker is not None

    def position(f: int) -> int:                   # position at the note phase of frame f
        return pos0 + frames.ticks(f - 1, origin, seeked) - tick_base

    def apply(trigger: int, event: str, arg) -> bool:
        nonlocal pos0, tick_base
        f = frames.frame_after(trigger - origin)
        if event == "push":
            del saved[arg:]
            saved.append(position(f))
        elif event == "pop":
            if arg:
                segments.append((pos0, position(f)))
                pos0, tick_base = saved[-1], frames.ticks(f - 1, origin, seeked)
            else:
                saved.pop()
        elif event == "end":
            segments.append((pos0, position(f)))
            return True
        elif event == "pass" and f > frames.last:
            key = (arg, (trigger - origin - frames.t[frames.last]) % frames.step)
            if key in seen:
                return True
            seen[key] = True
        return False

    body, last_pass = [], None
    for trigger, event, arg in timeline:
        if event == "tempo":
            stream._fail("0x85 in a channel stream changes the song tempo from a channel voice")
        if event == "pass":
            if last_pass is not None and last_pass[1] == arg:
                period = trigger - last_pass[0]
                rel = [(t - last_pass[0], e, a) for t, e, a in body]
                if apply(trigger, event, arg):
                    return segments
                base = trigger
                for _ in range(_MUS_MAX_PASSES):
                    for offset, e, a in rel:
                        if apply(base + offset, e, a):
                            return segments
                    base += period
                    if apply(base, "pass", arg):
                        return segments
                stream._fail("endless loop phase does not repeat within the pass budget")
            last_pass, body = (trigger, arg), []
            if apply(trigger, event, arg):
                return segments
            continue
        if last_pass is not None:
            body.append((trigger, event, arg))
        if apply(trigger, event, arg):
            return segments
    stream._fail("voice timeline ended without 0x80")


def _mus_drain(stream: "_MusStream", marker: int | None, is_master: bool) -> tuple[bool, set]:
    """Run one populated voice through a start to 0x80, or to the repetition of its endless-loop
    state, with every executed byte's role checked (_mus_timeline). func_80129F10 seeks every voice
    func_8012C4B0 started, the master and each channel with a note stream whether or not it has level
    streams, before it queues the unpause (C 23-56, 78), so each must reach its marker or 0x80.
    Returns (plays, the instrument indices playback reads); plays is False when the seek reached 0x80.
    Raises _MusNoStart when the seek can never reach the marker; a channel's 0x85 is refused (it would
    change every voice's step outside the master's schedule)."""
    refs = {"envelope": set(), "keymap": set(), "instrument": set(), "fx": set()}
    seen, playing = set(), marker is None
    for _, event, arg in _mus_timeline(stream, marker, refs):
        if event == "tempo" and not is_master:
            stream._fail("0x85 in a channel stream changes the song tempo from a channel voice")
        if event == "started":
            playing, seen = True, set()
        elif event == "end":
            return playing, refs["instrument"]
        elif event == "pass":
            if arg in seen:
                if not playing:
                    raise _MusNoStart(f"the seek of the voice at {stream.start:#x} never reaches marker {marker}")
                return True, refs["instrument"]
            seen.add(arg)
    stream._fail("voice timeline ended without 0x80")


_MUS_DUAL_READ_KEYS = {"channel", "level", "level_region", "boundary", "level_ticks", "voice_span_ticks", "reads",
                       "cause", "events"}
_MUS_DUAL_EVENT_KEYS = {"offset", "bytes", "value", "wait", "read_step"}
# Why a voice reads past its level data:
#   loop_restores_after_level_data  its endless loop restores only after the level data runs out
#                                   (voice_span_ticks > level_ticks), in every start;
#   restore_keeps_level_clock       the spans are equal, but func_80129750 restores the level cursor
#                                   and wait and not due14/due18, so under func_80129EE0 a later pass
#                                   runs on by less than one frame step;
#   seek_update_runs_one_tick       the spans are equal and only func_80129F10 starts over-read, by
#                                   one tick: its unconditional func_8012BA20/func_8012BADC call is a
#                                   do-while that runs a tick even when due >= time, and since
#                                   restores keep due14/due18 that head start carries to a later
#                                   pass that reaches one tick past the level data.
_MUS_OVERRUN_CAUSES = ("loop_restores_after_level_data", "restore_keeps_level_clock", "seek_update_runs_one_tick")


def _mus_dual_read_declaration(value) -> dict:
    """Validated `dual_read` witness parameter: {(channel, level): record}."""
    if value is None:
        return {}
    if not isinstance(value, (list, tuple)) or not value:
        raise AssetFormatError("libmus_song dual_read must be a non-empty list")
    out = {}
    for record in value:
        if not isinstance(record, dict) or set(record) != _MUS_DUAL_READ_KEYS or record["level"] not in ("volume", "pitch") \
                or not isinstance(record["events"], (list, tuple)) or not record["events"] \
                or any(not isinstance(ev, dict) or set(ev) != _MUS_DUAL_EVENT_KEYS for ev in record["events"]) \
                or not isinstance(record["reads"], dict) or not record["reads"] or record["cause"] not in _MUS_OVERRUN_CAUSES:
            raise AssetFormatError("libmus_song dual_read record does not match the schema")
        key = (record["channel"], record["level"])
        if key in out:
            raise AssetFormatError(f"libmus_song dual_read declares channel {key[0]} {key[1]} twice")
        out[key] = record
    return out


def _mus_overrun_events(data: bytes, s: int, boundary: int, stop: int, base: int, merged: list[list[int]],
                        reach: int) -> list[dict]:
    """Level events a voice reads beyond its streams: the bytes from `boundary` (song offset) decoded
    with the func_8012BA20/func_8012BADC grammar, read at level positions base + step, up to `reach`."""
    events, at, step = [], s + boundary, 1
    while base + step <= reach:
        start = at
        if at >= s + stop:
            raise AssetFormatError(f"song {s:#x}: the over-read leaves the region at {boundary:#x}")
        value = data[at]
        at += 1
        wait = 1
        if value & 0x80:
            wait = data[at]
            at += 1
            if wait & 0x80:
                wait = ((wait & 0x7F) << 8) + data[at] + 2
                at += 1
            else:
                wait += 2
        if at > s + stop:
            raise AssetFormatError(f"song {s:#x}: an over-read event crosses the end of the region at {boundary:#x}")
        if _mus_covered(merged, base + step):
            events.append({"offset": start - s, "bytes": data[start:at].hex(), "value": value & 0x7F, "wait": wait,
                           "read_step": base + step})
        step += wait
    return events


def _mus_voice_span(stream: "_MusStream") -> int:
    """Note ticks until the voice first restores its endless loop or ends (the length of the level
    data its first pass needs)."""
    for trigger, event, _ in _mus_timeline(stream):
        if event in ("pass", "end"):
            return trigger >> 8
    stream._fail("voice timeline ended without 0x80")


# Per-song schedule summary of the last parse (starts evaluated, impossible marker starts, silent
# voices); read by the layer builder for witness notes, never by replay decisions.
_MUS_SCHEDULE_REPORT: dict[int, dict] = {}


def _mus_covered(merged: list[list[int]], position: int) -> bool:
    """Is `position` in one of the sorted disjoint half-open-below segments (low, high]?"""
    lo, hi = 0, len(merged)
    while lo < hi:
        mid = (lo + hi) // 2
        if merged[mid][1] < position:
            lo = mid + 1
        else:
            hi = mid
    return lo < len(merged) and merged[lo][0] < position <= merged[lo][1]


def _mus_one_interval(intervals) -> tuple[int, int]:
    if len(intervals) != 1:
        raise AssetFormatError("libmus witnesses type exactly one map interval")
    return intervals[0]


def parse_libmus_song(original: bytes, intervals, params) -> list:
    """libmus song file, as loaded whole by func_80052914 and started by func_8012C4B0."""
    if set(params) - {"context", "dual_read"} or params.get("context"):
        raise AssetFormatError("libmus_song takes only a dual_read declaration")
    declared = _mus_dual_read_declaration(params.get("dual_read"))
    s, e = _mus_one_interval(intervals)
    size = e - s
    if size < _MUS_SONG_HEADER:
        raise AssetFormatError("song shorter than its header")
    word = [_mus_u32(original, s + 4 * i) for i in range(14)]
    version, channels, instruments = word[0], word[1], word[2]
    table_ch, table_vol, table_pitch, envelopes, keymaps, table_ins, master = word[3:10]
    if version != _MUS_VERSION or not 0 < channels <= 0x40 or not 0 < instruments:
        raise AssetFormatError(f"song header {version:#x}/{channels}/{instruments} is not libmus 0x215")
    if word[10] != 0 or any(word[11:14]):
        raise AssetFormatError("relocation flag +0x28 or reserved +0x2C..+0x37 is nonzero")
    if table_ch != _MUS_SONG_HEADER:
        raise AssetFormatError("channel table does not follow the 0x38-byte header")
    spans = [Span(s + 4 * i, s + 4 * i + 4, f"libmus_song.{name}", s) for i, name in enumerate((
        "version", "channel_count", "instrument_count", "channels", "volumes", "pitches", "envelopes",
        "keymaps", "instruments", "master", "relocated", "reserved_2c", "reserved_30", "reserved_34"))]
    tables = {"channels": table_ch, "volumes": table_vol, "pitches": table_pitch}
    pointers = {name: [_mus_u32(original, s + off + 4 * i) for i in range(channels)] for name, off in tables.items()}
    regions: dict[int, tuple[str, int]] = {}

    def claim(offset: int, kind: str, index: int = -1) -> None:
        if not _MUS_SONG_HEADER <= offset < size:
            raise AssetFormatError(f"{kind} offset {offset:#x} is outside the song")
        if offset in regions and regions[offset][0] != kind:
            raise AssetFormatError(f"{kind} and {regions[offset][0]} share offset {offset:#x}")
        regions.setdefault(offset, (kind, index))

    for name, offset in tables.items():
        claim(offset, name)
    claim(table_ins, "instruments")
    if keymaps != envelopes:          # equal offsets: the song has no keymap records
        claim(keymaps, "keymaps")
    claim(envelopes, "envelopes")
    claim(master, "master")
    for i in range(channels):
        for name, kind in (("channels", "channel"), ("volumes", "volume"), ("pitches", "pitch")):
            if pointers[name][i]:
                claim(pointers[name][i], kind, i)
    if not any(pointers["channels"]):
        raise AssetFormatError("song has no channel stream")
    order = sorted(regions)
    refs = {"envelope": set(), "keymap": set(), "instrument": set(), "fx": set()}
    levels: dict[int, tuple[int, int]] = {}
    streams: dict[int, _MusStream] = {}
    keymap_end = min((o for o in order if o > keymaps), default=size) if keymaps != envelopes else keymaps

    def keymap(index: int) -> int:
        record = keymaps + 6 * index
        if index < 0 or record + 6 > keymap_end:
            raise AssetFormatError(f"song {s:#x}: keymap note {index} is outside the keymap region")
        return _mus_u16(original, s + record)

    counts = {}
    tail = None
    for j, offset in enumerate(order):
        kind, _ = regions[offset]
        nxt = order[j + 1] if j + 1 < len(order) else size
        a, b = s + offset, s + nxt
        if kind in tables:
            if nxt - offset != 4 * channels:
                raise AssetFormatError(f"{kind} table is not channel_count words")
            entry = {"channels": "channel", "volumes": "volume", "pitches": "pitch"}[kind]
            spans += [Span(a + 4 * i, a + 4 * i + 4, f"libmus_ptr.{entry}", a + 4 * i) for i in range(channels)]
        elif kind == "instruments":
            used = 2 * instruments
            fill = (-(offset + used)) % 4
            if nxt - offset != used + fill:
                raise AssetFormatError("instrument table is not instrument_count halfwords plus word alignment")
            spans += [Span(a + 2 * i, a + 2 * i + 2, "libmus_instrument.sample", a + 2 * i) for i in range(instruments)]
            if fill:
                spans.append(Span(a + used, b, "pad", a + used))
        elif kind == "keymaps":
            if (nxt - offset) % 6:
                raise AssetFormatError("keymap region is not whole 6-byte records")
            counts["keymaps"] = (nxt - offset) // 6
            for r in range(a, b, 6):
                spans += [Span(r, r + 2, "libmus_keymap.instrument", r), Span(r + 2, r + 4, "libmus_keymap.envelope", r),
                          Span(r + 4, r + 5, "libmus_keymap.tuning", r), Span(r + 5, r + 6, "libmus_keymap.note", r)]
        elif kind == "envelopes":
            if (nxt - offset) % 7:
                raise AssetFormatError("envelope region is not whole 7-byte records")
            counts["envelopes"] = (nxt - offset) // 7
            for r in range(a, b, 7):
                spans += [Span(r + k, r + k + 1, f"libmus_envelope.{name}", r) for k, name in enumerate(_MUS_ENVELOPE)]
        elif kind in ("volume", "pitch"):
            levels[offset] = _mus_level_stream(original, a, b, f"song {s:#x} {kind}", spans)
        else:                                                  # master or channel note stream
            stream = _MusStream(original, a, b, True, f"song {s:#x} {kind}", keymap if keymaps != envelopes else None)
            streams[offset] = stream
            end = stream.decode()
            got = stream.execute()
            for key in refs:
                refs[key] |= got[key]
            spans += stream.spans
            if end != b:
                if j + 1 != len(order):
                    raise AssetFormatError(f"{kind} stream at {offset:#x} ends before the next region")
                tail = end
    if tail is not None:
        if e != s + ((tail - s + 15) & ~15):
            raise AssetFormatError("song tail is not alignment fill to 16")
        spans.append(Span(tail, e, "pad", tail))
    # Table extents: every referenced index is in range and every record is referenced.
    keymap_count = counts.get("keymaps", 0)
    if not refs["keymap"] <= set(range(keymap_count)):
        raise AssetFormatError("keymap reference outside the keymap region")
    envelope_refs = set(refs["envelope"])
    instrument_refs = set(refs["instrument"])
    for r in refs["keymap"]:                 # instruments were collected when each note stored them
        envelope_refs.add(_mus_u16(original, s + keymaps + 6 * r + 2))
    if envelope_refs != set(range(counts.get("envelopes", 0))):
        raise AssetFormatError("envelope records are not exactly the referenced set")
    if keymap_count and refs["keymap"] != set(range(keymap_count)):
        raise AssetFormatError("keymap records are not exactly the referenced set")
    if instrument_refs != set(range(instruments)):
        raise AssetFormatError("instrument table is not exactly the referenced set")
    # Level read schedule: every event of every volume/pitch region must be read by the exact
    # consumer schedule (_mus_level_segments) in both master slot orders and in every start the
    # game can give the song: func_80129EE0 (time 0), and func_80129F10 seeking to each 0xAB marker
    # id the song holds (func_80052944 starts T6 records 9/49/57 with marker 0, 36/38 through
    # func_80052AD8 with a runtime marker; func_80051D94 starts T6_alias records 2/3 that way). The
    # parser does not know which start a song's owner record selects, so it requires every one: a
    # marker start in which some populated voice (master or channel, with level streams or not)
    # can never return from its seek, or that leaves a level-bearing voice silent (its seek reaches
    # 0x80), or in which playback reads an instrument entry outside the table, refuses the song. A voice reading past its region's last event continues
    # into the next region in address order; that is consistent only while the next region is
    # another level region (whose events start on that boundary).
    following = {o: (order[j + 1] if j + 1 < len(order) else None) for j, o in enumerate(order)}
    master_stream = streams[master]
    marker_ids = sorted({original[at] for st in streams.values() for at, role in st.roles.items() if role == "ab.value"})
    self_check = {"starts": 0, "markers": marker_ids}
    observed: dict[tuple[int, str], dict] = {}
    populated = [master] + sorted({p for p in pointers["channels"] if p})
    for start in [None] + marker_ids:
        try:
            for offset in populated:
                plays, used = _mus_drain(streams[offset], start, offset == master)
                if not used <= set(range(instruments)):
                    raise AssetFormatError(f"song {s:#x}: the voice at {offset:#x} reads instrument entries "
                                           f"{sorted(used - set(range(instruments)))} past the table ({_mus_start_label(start)})")
        except _MusNoStart as error:
            raise AssetFormatError(f"song {s:#x}: {error} ({_mus_start_label(start)})") from None
        for same_frame in (True, False):
            label = (f"{'same' if same_frame else 'next'}-frame master order, "
                     f"{'func_80129EE0 start' if start is None else f'func_80129F10 marker {start} start'}")
            try:
                frames = _MusFrames(master_stream, same_frame, start)
                voice_segments = {i: _mus_level_segments(streams[pointers["channels"][i]], frames, start)
                                  for i in range(channels) if pointers["channels"][i]
                                  and (pointers["volumes"][i] or pointers["pitches"][i])}
            except _MusNoStart as error:
                raise AssetFormatError(f"song {s:#x}: {error} ({label})") from None
            self_check["starts"] += 1
            read_key = f"{'ee0' if start is None else f'marker_{start}'}/{'same_frame' if same_frame else 'next_frame'}"
            reads = {o: set() for o in levels}
            required = set()
            for i in range(channels):
                mine = [(_MUS_LEVEL_NAME[kind], pointers[kind][i]) for kind in ("volumes", "pitches") if pointers[kind][i]]
                if not mine:
                    continue
                if not pointers["channels"][i]:
                    raise AssetFormatError(f"song {s:#x} channel {i}: level stream without a voice (func_8012C4B0 starts none)")
                segments = sorted(voice_segments[i])
                if not segments:
                    raise AssetFormatError(f"song {s:#x} channel {i}: the voice of a level stream is silent ({label})")
                required.update(level for _, level in mine)
                merged: list[list[int]] = []
                for low, high in segments:
                    if high <= low:
                        continue
                    if merged and low <= merged[-1][1]:
                        merged[-1][1] = max(merged[-1][1], high)
                    else:
                        merged.append([low, high])
                reach = merged[-1][1] if merged else 0
                for level_name, level in mine:
                    base, region = 0, level
                    while True:
                        total, steps = levels[region]
                        for step in steps:
                            position = base + step
                            if _mus_covered(merged, position):
                                reads[region].add(step)
                        if reach <= base + total:
                            break
                        base, nxt = base + total, following[region]
                        if nxt not in levels:
                            where = "the song end" if nxt is None else f"{regions[nxt][0]} region {nxt:#x}"
                            if nxt != master or (i, level_name) not in declared:
                                raise AssetFormatError(
                                    f"song {s:#x} channel {i}: the level schedule reads past its streams into {where} "
                                    f"({reach - base} ticks, {label})")
                            # Declared over-read into the master stream: model it exactly; the bytes keep
                            # their master-stream spans, which must cover every one of them.
                            events = _mus_overrun_events(original, s, nxt, master_stream.spans[-1].end - s, base,
                                                         merged, reach)
                            record = observed.setdefault((i, level_name), {
                                "channel": i, "level": level_name, "level_region": level, "boundary": nxt,
                                "level_ticks": base, "voice_span_ticks": _mus_voice_span(streams[pointers["channels"][i]]),
                                "reads": {}, "events": events, "_frame_ticks": set()})
                            if record["events"] != events or record["level_ticks"] != base:
                                raise AssetFormatError(f"song {s:#x} channel {i}: the over-read differs between starts")
                            record["reads"][read_key] = reach
                            record["_frame_ticks"].add(-(-frames.step // 256))
                            break
                        region = nxt
            for level in sorted(required):
                total, steps = levels[level]
                if len(reads[level]) != len(steps):
                    raise AssetFormatError(
                        f"song {s:#x}: {len(steps) - len(reads[level])} events of the level region {level:#x} are never "
                        f"read ({label})")
    for key, record in observed.items():
        span, base = record["voice_span_ticks"], record["level_ticks"]
        overs = {k: reach - base for k, reach in record["reads"].items()}
        if span > base:
            cause = _MUS_OVERRUN_CAUSES[0]
        elif span == base and any(k.startswith("ee0/") for k in overs) and max(overs.values()) <= max(record["_frame_ticks"]):
            cause = _MUS_OVERRUN_CAUSES[1]
        elif span == base and not any(k.startswith("ee0/") for k in overs) and set(overs.values()) == {1}:
            cause = _MUS_OVERRUN_CAUSES[2]
        else:
            raise AssetFormatError(f"song {s:#x} channel {key[0]}: an over-read of {max(overs.values())} ticks with a "
                                   f"{span}-tick voice span over {base} level ticks is unexplained")
        record["cause"] = cause
        del record["_frame_ticks"]
    for key, record in declared.items():
        expected = {k: (dict(v) if isinstance(v, dict) else v) for k, v in record.items()}
        expected["events"] = [dict(ev) for ev in record["events"]]
        if observed.get(key) != expected:
            raise AssetFormatError(f"song {s:#x} channel {key[0]} {key[1]}: the declared dual_read is not the "
                                   f"over-read the consumer schedule performs")
    if set(observed) != set(declared):
        raise AssetFormatError(f"song {s:#x}: dual_read declares voices that do not over-read")
    _MUS_SCHEDULE_REPORT[s] = self_check
    return spans


def parse_libmus_fxbank(original: bytes, intervals, params) -> list:
    """libmus fx bank, installed by func_8012A910 and played by func_8012C3E0/func_8012C330."""
    if set(params) - {"context"} or params.get("context"):
        raise AssetFormatError("libmus_fxbank takes no parameters")
    s, e = _mus_one_interval(intervals)
    size = e - s
    count, effects, instruments, flags, bank, table_ins = (_mus_u32(original, s + 4 * i) for i in range(6))
    if not 0 < effects <= count or not 0 < instruments or flags or bank:
        raise AssetFormatError("fx header counts/flags/ptr-bank slot are not an unrelocated bank")
    entries_end = _MUS_FX_HEADER + 8 * count
    spans = [Span(s + 4 * i, s + 4 * i + 4, f"libmus_fxbank.{name}", s) for i, name in enumerate((
        "count", "effect_count", "instrument_count", "flags", "ptr_bank", "instruments"))]
    offsets = []
    for i in range(count):
        view = s + 8 * i                      # consumers index the bank by 8*i, then add 0x18/0x1C
        offsets.append(_mus_u32(original, view + 0x18))
        spans += [Span(view + 0x18, view + 0x1C, "libmus_fxbank_entry.data", view),
                  Span(view + 0x1C, view + 0x20, "libmus_fxbank_entry.priority", view)]
    if offsets[0] != entries_end or offsets != sorted(set(offsets)) or not offsets[-1] < table_ins < size:
        raise AssetFormatError("fx streams do not follow the entry table in entry order")
    refs = {"envelope": set(), "keymap": set(), "instrument": set(), "fx": set()}
    bounds = offsets + [table_ins]
    for i in range(count):
        a, b = s + bounds[i], s + bounds[i + 1]
        stream = _MusStream(original, a, b, False, f"fx {s:#x} entry {i}")
        end = stream.decode()
        got = stream.execute()
        for key in refs:
            refs[key] |= got[key]
        spans += stream.spans
        if end != b:
            # Only the last stream may stop short, by the one byte that aligns the u16 table.
            if i + 1 != count or b - end != 1 or (end - s) % 2 != 1 or table_ins % 2:
                raise AssetFormatError(f"fx entry {i} stream ends before the next stream")
            spans.append(Span(end, b, PAD, end))     # the framework requires it to be zero
    used = 2 * instruments
    a = s + table_ins
    spans += [Span(a + 2 * i, a + 2 * i + 2, "libmus_instrument.sample", a + 2 * i) for i in range(instruments)]
    if e != s + ((a + used - s + 15) & ~15):
        raise AssetFormatError("fx bank tail is not alignment fill to 16")
    if a + used < e:
        spans.append(Span(a + used, e, "pad", a + used))
    if refs["fx"] != set(range(effects, count)):
        raise AssetFormatError("0xA7 sub-effect calls are not exactly entries effect_count..count-1")
    if refs["instrument"] != set(range(instruments)):
        raise AssetFormatError("fx instrument table is not exactly the referenced set")
    return spans


REQUIRED_ZERO = REQUIRED_ZERO | {"libmus_song.relocated", "libmus_song.reserved_2c", "libmus_song.reserved_30",
                                 "libmus_song.reserved_34", "libmus_fxbank.flags", "libmus_fxbank.ptr_bank"}
PARSERS["libmus_song"] = parse_libmus_song
PARSERS["libmus_fxbank"] = parse_libmus_fxbank


# ---------------------------------------------------------------------------- code-based banks
#
# Author: opus-05 (Anthropic, claude-opus-5-5, xhigh). Layered on the assetformat framework after
# opus-08's libmus block: expects AssetFormatError, Span, PAD and PARSERS from the enclosing module.
# These are the located `asset_raw_code_based` rows whose ROM base a catalogued function
# materializes and passes to the PI reader (config/rom_regions.json code_bases, plus the segment-6
# palette window). None is CPU code. The matched readers that fix each layout:
#   glyph4bpp            func_8005DCEC/func_8005E134 (B_157b80, stride 0x72), func_8005DD80
#                        (B_175150, stride 130), func_8005DE08 (B_177150, stride 0x82): header u16 at
#                        +0 (width = high byte, height = byte +1), 4bpp pixel rows from +2
#   message_bank         func_80048480: id < 0x5DC1 -> D_8014AA94 range table -> D_8014D430 u32
#                        offset; func_8005DFE8/func_8005DE90 decode 1-byte codes, 0xFx lead + trail
#                        pairs and the NUL terminator
#   floor_event_tables   func_801E58A0_1377C0 (index and key lists), func_801E5A3C_13795C (record
#                        windows), func_801E5BE4_137B04 (paged record copy) and the record members
#                        its callers read: func_801F1A00_143920 (0x16-byte event record, stack copy
#                        at sp+0x10) with func_801F1510_143430 and func_801F1B58_143A78 (through the
#                        record pointer), func_801F1FF4_143F14 (0xC-byte placement record, sp+0x18)
#   tlut16               func_80071CCC -> func_8007248C transfers exactly 0x20 bytes; func_80070398
#                        loads it with G_LOADTLUT 0x0703C000 (16 entries)
#   script_bank          func_801E52EC_13720C (u32 script offset table), func_801EF140_141060 (opcode
#                        dispatch) and its handler cases (inline argument fetches through
#                        func_801E53A4_1372C4); see _SCRIPT_ARGS


def _cb_one(intervals) -> tuple[int, int]:
    if len(intervals) != 1:
        raise AssetFormatError("code-based witnesses type exactly one map interval")
    return intervals[0]


def _cb_word(data: bytes, site: object, word: object, label: str) -> int:
    """A pinned original instruction word (cross-check of a constant the parser relies on)."""
    if type(site) is not int or site % 4 or not 0 <= site <= len(data) - 4 or not isinstance(word, str) or len(word) != 8:
        raise AssetFormatError(f"{label} must pin an aligned site and an 8-digit hex word")
    value = struct.unpack_from(">I", data, site)[0]
    if value != int(word, 16):
        raise AssetFormatError(f"{label} {site:#x} is not the pinned instruction")
    return value


def _cb_tail(data: bytes, cursor: int, end: int, spans: list, label: str) -> None:
    """Short all-zero fill from cursor to end (the framework re-checks every pad)."""
    if cursor > end:
        raise AssetFormatError(f"{label} runs past the end of its range")
    if cursor < end:
        if end - cursor >= 16 or any(data[cursor:end]):
            raise AssetFormatError(f"{label} leaves {end - cursor} untyped bytes")
        spans.append(Span(cursor, end, PAD, cursor))


def parse_glyph4bpp(original: bytes, intervals, params) -> list:
    """Fixed-stride glyph/icon records: u8 width, u8 height, then rows of 8 bytes (16 4bpp pixels).

    Every pixel outside the declared width (plus `slack` shadow columns) or height is zero, the
    record count is exact and only short zero fill remains."""
    start, end = _cb_one(intervals)
    stride, rows = params["stride"], params["rows"]
    max_width, heights, slack = params["max_width"], set(params["heights"]), params["slack"]
    if stride - 2 != rows * 8 or not 0 < max_width <= 16 or slack not in (0, 1):
        raise AssetFormatError("glyph4bpp geometry is inconsistent")
    count, tail = divmod(end - start, stride)
    if count != params["count"] or tail >= 16:
        raise AssetFormatError("glyph4bpp record count disagrees with the range")
    spans = []
    for index in range(count):
        p = start + index * stride
        width, height = original[p], original[p + 1]
        if width > max_width or height not in heights or height > rows:
            raise AssetFormatError(f"glyph {index} header {width}x{height} is out of range")
        for row in range(rows):
            line = original[p + 2 + 8 * row:p + 10 + 8 * row]
            for column in range(16):
                pixel = (line[column // 2] >> (4 * (1 - column % 2))) & 15
                if pixel and (row >= height or column >= width + slack):
                    raise AssetFormatError(f"glyph {index} has a pixel outside its {width}x{height} box")
        spans += [Span(p, p + 1, "glyph.width", p), Span(p + 1, p + 2, "glyph.height", p),
                  Span(p + 2, p + stride, "glyph.pixels", p)]
    _cb_tail(original, start + count * stride, end, spans, "glyph4bpp")
    return spans


def parse_message_bank(original: bytes, intervals, params) -> list:
    """NUL-terminated messages addressed by the CPU offset table D_8014D430.

    The id range table D_8014AA94 ({u16 first id, u16 first offset index}) ends the bank's ids at
    func_80048480's limit 0x5DC1; that entry's index is the number of offsets. Every offset starts a
    message, every message is referenced, and every 0xFx lead byte has a nonzero trail byte (the
    decoders read it unconditionally)."""
    start, end = _cb_one(intervals)
    (context,) = params["context"]
    limit_word = _cb_word(original, params["limit_site"], params["limit_word"], "message limit")
    limit = params["id_limit"]
    if limit_word >> 26 != 0x0B or limit_word & 0xFFFF != limit:  # sltiu rX, rY, limit
        raise AssetFormatError("message limit instruction is not sltiu with the pinned id limit")
    ranges_at, range_count, table_at = params["range_table"], params["range_count"], params["offset_table"]
    if not (context[0] <= ranges_at and ranges_at + 4 * range_count <= context[1]):
        raise AssetFormatError("message range table lies outside its context row")
    entries = [struct.unpack_from(">HH", original, ranges_at + 4 * i) for i in range(range_count)]
    if entries[0] != (1, 0) or entries[-1] != (0xFFFF, 0xFFFF):
        raise AssetFormatError("message range table bounds changed")
    if any(not (a[0] < b[0] and a[1] <= b[1]) for a, b in zip(entries, entries[1:])):
        raise AssetFormatError("message range table is not increasing")
    count = next((index for first, index in entries if first == limit), None)
    if count != params["count"]:
        raise AssetFormatError("message offset count disagrees with the id limit entry")
    if not (context[0] <= table_at and table_at + 4 * count <= context[1]):
        raise AssetFormatError("message offset table lies outside its context row")
    offsets = {struct.unpack_from(">I", original, table_at + 4 * i)[0] for i in range(count)}
    spans, starts, p, last = [], set(), start, end
    while last > start and original[last - 1] == 0:
        last -= 1
    while p < last:
        starts.add(p - start)
        while True:
            byte = original[p]
            if byte == 0:
                spans.append(Span(p, p + 1, "text_end.nul", p))
                p += 1
                break
            if byte & 0xF0 == 0xF0:
                if p + 1 >= end or original[p + 1] == 0:
                    raise AssetFormatError(f"message lead byte at {p:#x} has no trail byte")
                spans += [Span(p, p + 1, "text_lead.code", p), Span(p + 1, p + 2, "text_trail.code", p + 1)]
                p += 2
            else:
                spans.append(Span(p, p + 1, "text_char.code", p))
                p += 1
            if p >= end:
                raise AssetFormatError("message runs past the end of the bank")
    if starts != offsets:
        raise AssetFormatError("message starts and the offset table disagree")
    _cb_tail(original, p, end, spans, "message_bank")
    return spans


# Event-script bytecode (B_1c6cb0). func_801E52EC_13720C reads the u32 offset of script `id`; the
# script stream is fetched one byte at a time by func_801E53A4_1372C4 at the frame cursor, which
# every fetch site post-increments. func_801EF140_141060 fetches the opcode and dispatches on its
# high nibble: 0x00 func_801EBCA8_13DBC8, 0x10 func_801ED008_13EF28, 0x20 func_801ED720_13F640
# (0x28-0x2F via func_801ED5A0_13F4C0), 0x30/0x40 func_801EDFBC_13FEDC (0x30-0x3A via
# func_801EDBFC_13FB1C), 0x50 func_801EB728_13D648 commands else func_801EB954_13D874 operand
# loads, 0x60/0x70 func_801F45F4_146514 (func_801F3664_145584, func_801F3B5C_145A7C), and 0x80 and
# above one text code (func_801EE7B8_1406D8 fetches a trail byte after a 0xFx lead). Inline
# argument bytes each handler case fetches (all other opcodes below 0x80 fetch none):
_SCRIPT_ARGS = {
    0x08: 4, 0x09: 1,                                         # func_801EBCA8_13DBC8 cases 8, 9
    0x2C: 3,                                                  # func_801EC3D0_13E2F0
    0x3A: 1,                                                  # func_801EDBFC_13FB1C
    0x3D: 1, 0x3E: 1, 0x40: 2, 0x41: 1, 0x47: 1, 0x49: 1,    # func_801EDFBC_13FEDC
    0x4E: 2,                                                  # func_801EB53C_13D45C
    0x57: 1, 0x5E: 1,                                         # func_801EB728_13D648
    0x50: 1, 0x51: 2, 0x54: 1, 0x55: 2, 0x56: 3, 0x58: 1, 0x59: 2, 0x5C: 1, 0x5D: 2,  # func_801EB954_13D874
    0x64: 1,                                                  # func_801F45F4_146514
    0x65: 2, 0x66: 1, 0x74: 3,                                # func_801F3664_145584
    0x6C: 4, 0x6D: 1, 0x72: 1, 0x77: 6, 0x78: 1, 0x7B: 5,     # func_801F3B5C_145A7C
}
# func_801EB954_13D874 loads (code & 3) + 1 big-endian bytes into operand slot (code >> 2) & 3 and
# flags it signed when its top bit is set; func_801EB728_13D648 claims 0x52 0x53 0x57 0x5A 0x5B
# 0x5E 0x5F first, so no 4-byte load is reachable.
_SCRIPT_LOADS = frozenset({0x50, 0x51, 0x54, 0x55, 0x56, 0x58, 0x59, 0x5C, 0x5D})
_SCRIPT_RETURN = 0x03   # func_801EBCA8_13DBC8 case 3: pop the call frame, or end the script
_SCRIPT_NAME = 0x1D     # func_801ED008_13EF28 case 0x1D: func_801EF7F0_141710(slot 0)
# func_801EBCA8_13DBC8 opcodes that can reach its tail `cursor += slot3 - 1` (cases 2, 4-7, 10-15
# and the switch default 0, 1): a jump to opcode + slot 3. func_801ED720_13F640 0x21/0x22 jump to
# opcode + the chosen option's word, set by 0x23 (always 1), 0x24 and func_801EBCA8_13DBC8 case 9
# (slot 3).
_SCRIPT_CONDS = frozenset({0x00, 0x01, 0x02, 0x04, 0x05, 0x06, 0x07, *range(0x0A, 0x10)})
_SCRIPT_FILL = 4        # zero fill after the final return pads the next script to 4 bytes
_SCRIPT_FILL_END = 16   # and the bank to its end


def _script_value(raw: bytes) -> int:
    value = int.from_bytes(raw, "big")
    return value - (1 << 8 * len(raw)) if raw[0] & 0x80 else value


def _script_name(original: bytes, pos: int, end: int, spans: list) -> int:
    """func_801EB208_13D128: up to 20 code slots, a 0xFx lead and its trail taking two, ended by
    0x11; each code is its own instance."""
    used = 0
    while used < 20:
        if pos >= end or (original[pos] & 0xF0 == 0xF0 and pos + 2 > end):
            raise AssetFormatError(f"script name codes at {pos:#x} run past the body")
        code = original[pos]
        if code & 0xF0 == 0xF0:
            spans += [Span(pos, pos + 1, "script_name_kanji.lead", pos), Span(pos + 1, pos + 2, "script_name_kanji.trail", pos)]
            pos, used = pos + 2, used + 2
        elif code == 0x11:
            spans.append(Span(pos, pos + 1, "script_name_end.code", pos))
            return pos + 1
        else:
            spans.append(Span(pos, pos + 1, "script_name_char.code", pos))
            pos, used = pos + 1, used + 1
    return pos


def _script_body(original: bytes, a: int, b: int, admitted: frozenset, name_function: int, fill: int) -> list:
    """One script: instructions from its directory offset to the return opcode 3 after which only
    zero fill shorter than `fill` remains; every jump whose offset is loaded by the instruction just
    before it lands on an instruction of the body, at the latest the final return."""
    spans, starts, jumps, options, bindings = [], set(), [], [], []
    run: list[tuple[int, int, bytes]] = []   # (position, slot, raw value) of the loads just before
    pos = a
    while True:
        if pos >= b:
            raise AssetFormatError(f"script body {a:#x} ends without return opcode 3 and zero fill")
        op = original[pos]
        starts.add(pos)
        fresh = run[-1][2] if run and run[-1][1] == 3 else None
        if op >= 0xF0:
            if pos + 2 > b:
                raise AssetFormatError(f"script text code at {pos:#x} runs past body {a:#x}")
            spans += [Span(pos, pos + 1, "script_kanji.lead", pos), Span(pos + 1, pos + 2, "script_kanji.trail", pos)]
            run, pos = [], pos + 2
            continue
        if op >= 0x80:
            spans.append(Span(pos, pos + 1, "script_char.code", pos))
            run, pos = [], pos + 1
            continue
        if op not in admitted:
            raise AssetFormatError(f"script opcode {op:#04x} at {pos:#x} has no pinned handler reads")
        nxt = pos + 1 + _SCRIPT_ARGS.get(op, 0)
        if nxt > b:
            raise AssetFormatError(f"script instruction at {pos:#x} runs past body {a:#x}")
        struct_name = f"script_op_{op:02x}"
        spans.append(Span(pos, pos + 1, f"{struct_name}.opcode", pos))
        spans += [Span(at, at + 1, f"{struct_name}.arg{at - pos}", pos) for at in range(pos + 1, nxt)]
        if op == _SCRIPT_NAME:
            load = next((entry for entry in reversed(run) if entry[1] == 0), None)
            if load is None:
                raise AssetFormatError(f"script opcode 0x1d at {pos:#x} has no function number loaded just before it")
            bindings.append((load[0], pos))
            if int.from_bytes(load[2], "big") == name_function:
                nxt = _script_name(original, nxt, b, spans)
        if op in _SCRIPT_CONDS and fresh is not None:
            jumps.append((pos, pos + _script_value(fresh)))
        elif op in (0x09, 0x24) and fresh is not None:
            options.append(_script_value(fresh))
        elif op == 0x23:
            options.append(1)
        elif op in (0x21, 0x22):
            jumps += [(pos, pos + w) for w in options]
            options = []
        run = run + [(pos, (op >> 2) & 3, original[pos + 1:nxt])] if op in _SCRIPT_LOADS else []
        if op == _SCRIPT_RETURN and b - nxt < fill and not any(original[nxt:b]):
            if nxt < b:
                spans.append(Span(nxt, b, PAD, nxt))
            break
        pos = nxt
    for source, to in jumps:
        if to not in starts or not a <= to <= pos:
            raise AssetFormatError(f"script jump at {source:#x} to {to:#x} is not an instruction of body {a:#x}")
    for load, at in bindings:
        if any(load < to <= at for _, to in jumps):
            raise AssetFormatError(f"a script jump enters the 0x1d function binding at {at:#x}")
    return spans


def parse_script_bank(original: bytes, intervals, params) -> list:
    """u32 script offset table (its size is the first offset), then the scripts it addresses, each
    walked opcode by opcode (_script_body) with the handler argument counts of _SCRIPT_ARGS.

    params: `count`; `opcodes`, the admitted opcodes below 0x80 (each has pinned dispatch and
    argument fetches in the format); `fetch` {jal_word, text, sites}, every fetcher call in the
    interpreter's text with its attribution, which must be complete; `name_call` {function,
    jtbl_site, jtbl_word, call_site, call_word}, the func_801EF7F0_141710 function whose case calls
    func_801EB208_13D128, the only one that fetches name codes."""
    start, end = _cb_one(intervals)
    first = struct.unpack_from(">I", original, start)[0]
    count = first // 4
    if first % 4 or count != params["count"]:
        raise AssetFormatError("script table size disagrees with its first offset")
    offsets = [struct.unpack_from(">I", original, start + 4 * i)[0] for i in range(count)]
    if any(a >= b for a, b in zip(offsets, offsets[1:])) or offsets[-1] >= end - start or any(o % 4 for o in offsets):
        raise AssetFormatError("script offsets are not increasing 4-byte aligned offsets inside the range")
    fetch = params["fetch"]
    jal, (lo, hi) = _hex_word(fetch["jal_word"]), fetch["text"]
    found = {at for at in range(lo, hi, 4) if struct.unpack_from(">I", original, at)[0] == jal}
    listed = [site for site, label in fetch["sites"] if isinstance(label, str) and label]
    if jal >> 26 != _JAL or lo % 4 or len(set(listed)) != len(fetch["sites"]) or set(listed) != found:
        raise AssetFormatError("script fetch attribution does not list every fetcher call exactly once")
    call = params["name_call"]
    _cb_word(original, call["jtbl_site"], call["jtbl_word"], "script name jump-table entry")
    if _cb_word(original, call["call_site"], call["call_word"], "script name call") >> 26 != _JAL:
        raise AssetFormatError("script name call is not a jal")
    admitted = frozenset(params["opcodes"])
    if any(type(op) is not int or not 0 <= op < 0x80 for op in admitted) or _SCRIPT_RETURN not in admitted:
        raise AssetFormatError("script admitted opcodes are invalid")
    spans = [Span(start + 4 * i, start + 4 * i + 4, "script_index.offset", start + 4 * i) for i in range(count)]
    bounds = offsets + [end - start]
    for i, (a, b) in enumerate(zip(bounds, bounds[1:])):
        spans += _script_body(original, start + a, start + b, admitted, call["function"],
                              _SCRIPT_FILL_END if i == count - 1 else _SCRIPT_FILL)
    return spans


# Floor record layouts (member, width), in record order. Every member except the alignment
# members is read by a pinned consumer access; the alignment members (before a halfword, or to
# the even record size) must be zero:
#   floor_event (Event801F1A00 / PartSpec): func_801F1A00_143920 loads its stack copy: +0 id,
#     +1 floor (lb), +2..+3 y/x (lhu), +5..+0xA two {id, low, high} requirement triples;
#     func_801F1510_143430 loads +0, +2, +3, +4 and func_801F1B58_143A78 loads +4, +0xC and +0xE
#     (lhu), +0x10..+0x14 through the record pointer. Alignment: +0xB, +0x15.
#   floor_placement (Placement801F1FF4): func_801F1FF4_143F14 loads its stack copy: +0 y, +1 x,
#     +2, +3..+5 requirement triple, +6 floor (lb), +8 (lhu), +0xA. Alignment: +7, +0xB.
_FLOOR_LAYOUTS = {
    "floor_event": (("id", 1), ("floor", 1), ("y", 1), ("x", 1), ("f04", 1), ("req0_id", 1), ("req0_low", 1),
                    ("req0_high", 1), ("req1_id", 1), ("req1_low", 1), ("req1_high", 1), ("pad_0b", 1),
                    ("f0c", 2), ("f0e", 2), ("f10", 1), ("f11", 1), ("f12", 1), ("f13", 1), ("f14", 1),
                    ("pad_15", 1)),
    "floor_placement": (("y", 1), ("x", 1), ("field_02", 1), ("req_id", 1), ("req_low", 1), ("req_high", 1),
                        ("floor", 1), ("pad_07", 1), ("field_08", 2), ("field_0a", 1), ("pad_0b", 1)),
}
# Members the floor parser requires to be zero (the key record's second byte is never read).
_FLOOR_ZERO = frozenset({"floor_key.zero", "floor_event.pad_0b", "floor_event.pad_15",
                         "floor_placement.pad_07", "floor_placement.pad_0b"})


def parse_floor_event_tables(original: bytes, intervals, params) -> list:
    """Index of {u32 key-list offset, u32 record-block offset}, then per entry a key list
    ({u8 key, u8 0, u16 first record}, ended by key 0 whose value is the record count) and its
    record block (count records in the _FLOOR_LAYOUTS layout of its struct, then short zero
    fill), laid out back to back. Key padding and record alignment members must be zero."""
    start, end = _cb_one(intervals)
    sizes, names = params["record_sizes"], params["record_structs"]
    if len(sizes) != params["entries"] or len(names) != len(sizes):
        raise AssetFormatError("floor table parameters are inconsistent")
    for size, name in zip(sizes, names):
        layout = _FLOOR_LAYOUTS.get(name)
        if layout is None or sum(width for _, width in layout) != size:
            raise AssetFormatError(f"floor record struct {name!r} has no {size}-byte layout")
    spans, cursor = [], start + 8 * len(sizes)
    for i in range(len(sizes)):
        p = start + 8 * i
        spans += [Span(p, p + 4, "floor_index.keys", p), Span(p + 4, p + 8, "floor_index.records", p)]
    for i, (size, name) in enumerate(zip(sizes, names)):
        keys, records = struct.unpack_from(">II", original, start + 8 * i)
        if start + keys != cursor:
            raise AssetFormatError(f"floor key list {i} does not follow the previous block")
        p, seen, previous = cursor, set(), 0
        while True:
            key, zero, first = original[p], original[p + 1], struct.unpack_from(">H", original, p + 2)[0]
            if zero:
                raise AssetFormatError(f"floor key record at {p:#x} has a nonzero second byte")
            spans += [Span(p, p + 1, "floor_key.key", p), Span(p + 1, p + 2, "floor_key.zero", p),
                      Span(p + 2, p + 4, "floor_key.first", p)]
            p += 4
            if first < previous:
                raise AssetFormatError(f"floor key list {i} record indices decrease")
            previous = first
            if key == 0:
                break
            if key in seen:
                raise AssetFormatError(f"floor key list {i} repeats key {key}")
            seen.add(key)
            if p >= end:
                raise AssetFormatError("floor key list runs past the range")
        if start + records != p:
            raise AssetFormatError(f"floor record block {i} does not follow its key list")
        q = p + first * size
        for r in range(p, q, size):
            at = r
            for member, width in _FLOOR_LAYOUTS[name]:
                field = f"{name}.{member}"
                if field in _FLOOR_ZERO and any(original[at:at + width]):
                    raise AssetFormatError(f"floor record alignment {field} at {at:#x} is not zero")
                spans.append(Span(at, at + width, field, r))
                at += width
        if i + 1 < len(sizes):
            next_keys = start + struct.unpack_from(">I", original, start + 8 * (i + 1))[0]
            _cb_tail(original, q, next_keys, spans, f"floor record block {i}")
            cursor = next_keys
        else:
            _cb_tail(original, q, end, spans, f"floor record block {i}")
    return spans


def parse_tlut16(original: bytes, intervals, params) -> list:
    """One 16-entry RGBA16 TLUT; the entry count is that of the consumer's G_LOADTLUT word."""
    start, end = _cb_one(intervals)
    hi = _cb_word(original, params["loadtlut_lui_site"], params["loadtlut_lui_word"], "LoadTLUT lui")
    lo = _cb_word(original, params["loadtlut_ori_site"], params["loadtlut_ori_word"], "LoadTLUT ori")
    if hi >> 26 != 0x0F or lo >> 26 != 0x0D or (lo >> 21) & 31 != (hi >> 16) & 31:
        raise AssetFormatError("LoadTLUT constant is not a lui/ori pair")
    command = (hi & 0xFFFF) << 16 | lo & 0xFFFF
    if command >> 24 != 0x07 or ((command >> 14) & 0x3FF) + 1 != params["entries"]:
        raise AssetFormatError("LoadTLUT entry count disagrees")
    if end - start != 2 * params["entries"]:
        raise AssetFormatError("TLUT size disagrees with its entry count")
    return [Span(p, p + 2, "tlut16.color", start) for p in range(start, end, 2)]


PARSERS["glyph4bpp"] = parse_glyph4bpp
PARSERS["message_bank"] = parse_message_bank
PARSERS["script_bank"] = parse_script_bank
PARSERS["floor_event_tables"] = parse_floor_event_tables
PARSERS["tlut16"] = parse_tlut16
REQUIRED_ZERO = REQUIRED_ZERO | _FLOOR_ZERO


# ---------------------------------------------------------------------------- T2 scene records
#
# Author: opus-08 (Anthropic, claude-opus-5-5, xhigh). Layered on the assetformat framework:
# expects AssetFormatError, Span, PAD and PARSERS from the enclosing module.
#
# A T2/T2_alias record (D_8013BAD4[28], D_8013C084[16]; 0x34 bytes) names three ROM windows:
# +0x18/+0x1C a segment-5 map file, +0x20/+0x24 a Yay0 model stream (already asset_yay0) and
# +0x28/+0x2C a segment-6 sky file. Grammar source (matched C in src/units/main_14400; the binary
# accesses are pinned in config/rom-evidence/asset-formats.json):
#   map file count / sections  func_800649F8, func_80066A28, func_80066AD0 (4-byte count DMA),
#                              func_80064A70 (12-byte section record at +4 + 12 * index; DMA of
#                              start..end to D_801DFF7C), func_80064D5C (rebases section +0 / +4)
#   section objects            func_80064D5C (cell table, cells, menu block, map info, rooms, group
#                              and patch lists), func_80066B2C, func_800673D8, func_80066950,
#                              func_800669FC, func_80042C20, func_80042D84, func_80042EC0,
#                              func_800430C4, func_80043228, func_80062554 (56 x 34 float grid)
#   sky file                   func_80064A70 (DMA rom_28..rom_2C), func_80064378 (segment-6
#                              rebase of +0, sky +0, backdrop +0/+4), func_80064444, func_80060048
#                              (format 0x15: CI, 8-bit texels, 256-entry TLUT)

_SCENE_SEGMENT_MAP = 5     # func_80064A70 func_80062CCC(section.start, data_18, 5)
_SCENE_SEGMENT_MODEL = 6   # func_80064D5C func_80062CCC(type->models, D_801D2550, 6)
_SCENE_SEGMENT_SKY = 6     # func_80064A70 func_80064378(D_8016DB74, 6)
_SCENE_SECTION = 12        # func_80064A70 DMA size and index stride
_SCENE_SECTION_GAP = 8     # zero bytes after every section's DMA end
_SCENE_CELL = 0x2C         # func_80064D5C addiu $t3, $t3, 0x2C; func_800673D8 type * 0x2C
_SCENE_ROOM = 0x20         # func_80064D5C / func_80042D84 room_count << 5
_SCENE_GROUP = 0x14        # func_80064D5C / func_80066B2C count * 0x14
_SCENE_PATCH = 0x18        # func_80064D5C / func_800673D8 count * 0x18
_SCENE_MAP_W, _SCENE_MAP_H = 56, 34   # D_80143450[54][76] canvas = (h + 20) x (w + 20); grid 0x38 x 34
_SCENE_GRID = 0x1DC0       # func_80064A70 room check; 56 * 34 floats
_SCENE_SETTINGS = 0x16     # bytes +0..+5 and class_values_06[16]
_SCENE_SKY_TLUT_ENTRIES = 256  # format 0x15: func_80060048 loads a 256-entry RGBA16 TLUT
_SCENE_BACKDROP = 0x16
_SCENE_SKY = 0x14


def _scene_u32(data: bytes, at: int) -> int:
    return struct.unpack_from(">I", data, at)[0]


def _scene_s32(data: bytes, at: int) -> int:
    return struct.unpack_from(">i", data, at)[0]


def _scene_one(ranges: tuple[Interval, ...], params: Mapping[str, Any], label: str) -> Interval:
    if len(ranges) != 1 or params.get("context") or set(params) != {"context"}:
        raise AssetFormatError(f"{label} takes exactly one range, no context and no parameters")
    return ranges[0]


class _SceneObjects:
    """Collects the typed objects of one region and checks that they tile it with alignment fill."""

    def __init__(self, data: bytes, origin: int, low: int, high: int, label: str) -> None:
        self.data, self.origin, self.low, self.high, self.label = data, origin, low, high, label
        self.objects: list[tuple[int, int, list[Span]]] = []

    def fail(self, message: str) -> None:
        raise AssetFormatError(f"{self.label}: {message}")

    def add(self, start: int, size: int, spans: list[Span], align: int) -> None:
        if size <= 0 or (start - self.origin) % align or not self.low <= start or start + size > self.high:
            self.fail(f"object {start:#x}+{size:#x} is misaligned or leaves {self.low:#x}..{self.high:#x}")
        if sum(s.end - s.start for s in spans) != size or min(s.start for s in spans) != start:
            self.fail(f"object {start:#x} spans do not cover it")
        self.objects.append((start, start + size, spans))

    def tile(self, end_align: int) -> list[Span]:
        """Objects in address order; fill between them is zero and shorter than the next alignment."""
        out: list[Span] = []
        cursor = self.low
        for start, end, spans in sorted(self.objects, key=lambda o: o[0]):
            if start < cursor:
                self.fail(f"objects overlap at {start:#x}")
            if start > cursor:
                if start - cursor >= 8 or any(self.data[cursor:start]):
                    self.fail(f"{start - cursor} fill bytes before {start:#x} are not zero alignment")
                out.append(Span(cursor, start, PAD, cursor))
            out += spans
            cursor = end
        if cursor > self.high or (self.high - cursor) >= end_align or (self.high - self.origin) % end_align \
                or any(self.data[cursor:self.high]):
            self.fail(f"tail {cursor:#x}..{self.high:#x} is not zero fill to {end_align}-byte alignment")
        if cursor < self.high:
            out.append(Span(cursor, self.high, PAD, cursor))
        return out


def _scene_fields(base: int, layout) -> list[Span]:
    spans, at = [], base
    for name, size in layout:
        spans.append(Span(at, at + size, name, base))
        at += size
    return spans


def _scene_array(base: int, size: int, struct_name: str, element: int) -> list[Span]:
    """A homogeneous array: its first element (the pinned access) and the remaining elements."""
    spans = [Span(base, base + element, f"{struct_name}.element", base)]
    if size > element:
        spans.append(Span(base + element, base + size, f"{struct_name}.elements", base))
    return spans


_SCENE_CELL_LAYOUT = (("scene_cell.flags", 2), (PAD, 2), ("scene_cell.tx", 4), ("scene_cell.ty", 4),
                      ("scene_cell.tz", 4), ("scene_cell.rx", 4), ("scene_cell.ry", 4), ("scene_cell.rz", 4),
                      ("scene_cell.sx", 4), ("scene_cell.sy", 4), ("scene_cell.sz", 4), ("scene_cell.models", 4))
_SCENE_ROOM_LAYOUT = (("scene_room.x0", 4), ("scene_room.y0", 4), ("scene_room.x1", 4), ("scene_room.y1", 4)) + \
    tuple(item for k in range(4) for item in ((f"scene_room.side{k}_high", 3), (f"scene_room.side{k}", 1)))
_SCENE_GROUP_LAYOUT = (("scene_group.field_00", 2), ("scene_group.field_02", 2), ("scene_group.position_count", 4),
                       ("scene_group.positions", 4), ("scene_group.mark_count", 4), ("scene_group.marks", 4))
_SCENE_PATCH_LAYOUT = (("scene_patch.id", 2), (PAD, 2), ("scene_patch.x", 4), ("scene_patch.y", 4),
                       ("scene_patch.cols", 4), ("scene_patch.rows", 4), ("scene_patch.tiles", 4))
_SCENE_SETTINGS_LAYOUT = (("scene_settings.field_00", 1), ("scene_settings.bordered", 1),
                          ("scene_settings.field_02", 1), ("scene_settings.field_03", 1),
                          ("scene_settings.field_04", 1), ("scene_settings.field_05", 1)) + \
    tuple((f"scene_settings.class_value_{k:02d}", 1) for k in range(16))


def _scene_pad_fields(spans: list[Span]) -> list[Span]:
    """PAD members of a struct layout become framework pad spans based at themselves."""
    return [Span(s.start, s.end, PAD, s.start) if s.field == PAD else s for s in spans]


def _scene_section(data: bytes, window: int, start: int, menu: int, end: int, label: str) -> list[Span]:
    objs = _SceneObjects(data, window, start, end, label)

    def ptr(value: int, what: str, null_ok: bool = False) -> int | None:
        if value == 0 and null_ok:
            return None
        if value >> 24 != _SCENE_SEGMENT_MAP:
            objs.fail(f"{what} {value:#010x} is not a segment-5 address")
        return window + (value & 0xFFFFFF)

    def struct_at(at: int, layout, align: int = 4) -> list[Span]:
        spans = _scene_pad_fields(_scene_fields(at, layout))
        for s in spans:
            if s.field == PAD and any(data[s.start:s.end]):
                objs.fail(f"alignment member {s.start:#x} is not zero")
        objs.add(at, sum(size for _, size in layout), spans, align)
        return spans

    def zero(at: int, size: int, what: str) -> None:
        if any(data[at:at + size]):
            objs.fail(f"{what} at {at:#x} is not zero")

    # Cell table and cells: D_801DE97C is the load buffer itself (func_800612D4), so the table is
    # the section's first object.
    cells, count = _scene_u32(data, start), _scene_u32(data, start + 4)
    struct_at(start, (("scene_cell_table.cells", 4), ("scene_cell_table.count", 4)))
    if count:
        base = ptr(cells, "cell array")
        for i in range(count):
            at = base + i * _SCENE_CELL
            if _scene_u32(data, at + 0x28) >> 24 != _SCENE_SEGMENT_MODEL:
                objs.fail(f"cell {i} model list is not a segment-6 address")
        # The first cell carries the pinned member accesses; the others are delimited records.
        spans = _scene_pad_fields(_scene_fields(base, _SCENE_CELL_LAYOUT))
        spans += [Span(base + i * _SCENE_CELL, base + (i + 1) * _SCENE_CELL, "scene_cell_record.cell",
                       base + i * _SCENE_CELL) for i in range(1, count)]
        if any(data[base + i * _SCENE_CELL + 2] | data[base + i * _SCENE_CELL + 3] for i in range(count)):
            objs.fail("cell alignment member is not zero")
        objs.add(base, count * _SCENE_CELL, spans, 4)
    elif cells:
        objs.fail("empty cell table has a cell pointer")

    # Menu block (func_80064D5C rebases all five words; func_80066B2C / func_80042EC0 read them).
    m = ptr(menu, "menu block")
    words = struct.unpack_from(">5I", data, m)
    struct_at(m, (("scene_menu.map", 4), ("scene_menu.groups", 4), ("scene_menu.patches", 4),
                  ("scene_menu.grid", 4), ("scene_menu.settings", 4)))
    info, groups, patches = ptr(words[0], "map info"), ptr(words[1], "group list"), ptr(words[2], "patch list")
    grid, settings = ptr(words[3], "grid", True), ptr(words[4], "settings")

    width, height, tiles, room_count, rooms = struct.unpack_from(">iiIiI", data, info)
    if (width, height) != (_SCENE_MAP_W, _SCENE_MAP_H) or not 0 <= room_count < 0x100:
        objs.fail(f"map {width}x{height} with {room_count} rooms is outside the consumers' bounds")
    struct_at(info, (("scene_map_info.width", 4), ("scene_map_info.height", 4), ("scene_map_info.tiles", 4),
                     ("scene_map_info.room_count_high", 3), ("scene_map_info.room_count", 1),
                     ("scene_map_info.rooms", 4)))
    t = ptr(tiles, "tile map")
    objs.add(t, 2 * width * height, _scene_array(t, 2 * width * height, "scene_tiles", 2), 2)
    if room_count:
        r = ptr(rooms, "room array")
        spans = []
        for i in range(room_count):
            at = r + i * _SCENE_ROOM
            x0, y0, x1, y1 = struct.unpack_from(">4i", data, at)
            if not (0 <= x0 <= x1 < width and 0 <= y0 <= y1 < height):
                objs.fail(f"room {i} ({x0},{y0})..({x1},{y1}) leaves the {width}x{height} map")
            for k in range(4):
                zero(at + 0x10 + 4 * k, 3, f"room {i} side {k} high bytes")
            spans += _scene_fields(at, _SCENE_ROOM_LAYOUT)
        objs.add(r, room_count * _SCENE_ROOM, spans, 4)
    elif rooms:
        objs.fail("empty room list has a room pointer")

    group_count, group_array = struct.unpack_from(">iI", data, groups)
    struct_at(groups, (("scene_group_list.count", 4), ("scene_group_list.groups", 4)))
    if group_count < 0 or (group_count == 0) != (group_array == 0):
        objs.fail("group list count and pointer disagree")
    if group_count:
        g = ptr(group_array, "group array")
        spans = []
        for i in range(group_count):
            at = g + i * _SCENE_GROUP
            zero(at, 2, f"group {i} field_00")
            _, positions_n, positions, marks_n, marks = struct.unpack_from(">iiIiI", data, at)
            spans += _scene_fields(at, _SCENE_GROUP_LAYOUT)
            if positions_n < 0 or marks_n < 0 or (positions_n == 0) != (positions == 0) \
                    or (marks_n == 0) != (marks == 0):
                objs.fail(f"group {i} counts and pointers disagree")
            if positions_n:
                p = ptr(positions, f"group {i} positions")
                for j in range(positions_n):
                    x, y = struct.unpack_from(">ii", data, p + 8 * j)
                    if not (0 <= x < width and 0 <= y < height):
                        objs.fail(f"group {i} position {j} ({x},{y}) leaves the map")
                objs.add(p, 8 * positions_n, [s for j in range(positions_n) for s in _scene_fields(
                    p + 8 * j, (("scene_position.x", 4), ("scene_position.y", 4)))], 4)
            if marks_n:
                k = ptr(marks, f"group {i} marks")
                mark_spans = []
                for j in range(marks_n):
                    zero(k + 4 * j + 3, 1, f"group {i} mark {j} alignment member")
                    mark_spans += _scene_pad_fields(_scene_fields(k + 4 * j, (
                        ("scene_mark.id", 2), ("scene_mark.flags", 1), (PAD, 1))))
                objs.add(k, 4 * marks_n, mark_spans, 2)
        objs.add(g, group_count * _SCENE_GROUP, spans, 4)

    patch_count, patch_array = struct.unpack_from(">iI", data, patches)
    struct_at(patches, (("scene_patch_list.count", 4), ("scene_patch_list.patches", 4)))
    if patch_count < 0 or (patch_count == 0) != (patch_array == 0):
        objs.fail("patch list count and pointer disagree")
    if patch_count:
        q = ptr(patch_array, "patch array")
        spans = []
        for i in range(patch_count):
            at = q + i * _SCENE_PATCH
            zero(at + 2, 2, f"patch {i} alignment member")
            _, x, y, cols, rows, ptiles = struct.unpack_from(">IiiiiI", data, at)
            spans += _scene_pad_fields(_scene_fields(at, _SCENE_PATCH_LAYOUT))
            if cols < 0 or rows < 0 or (cols * rows == 0) != (ptiles == 0):
                objs.fail(f"patch {i} extent and tile pointer disagree")
            if not (0 <= x and x + cols <= width and 0 <= y and y + rows <= height):
                objs.fail(f"patch {i} ({x},{y}) {cols}x{rows} leaves the map")
            if cols * rows:
                pt = ptr(ptiles, f"patch {i} tiles")
                objs.add(pt, 2 * cols * rows, _scene_array(pt, 2 * cols * rows, "scene_patch_tiles", 2), 2)
        objs.add(q, patch_count * _SCENE_PATCH, spans, 4)

    if grid is not None:
        objs.add(grid, _SCENE_GRID, _scene_array(grid, _SCENE_GRID, "scene_grid", 4), 4)
    objs.add(settings, _SCENE_SETTINGS, _scene_fields(settings, _SCENE_SETTINGS_LAYOUT), 1)
    return objs.tile(8)


def parse_scene_map(original: bytes, ranges: tuple[Interval, ...], params: Mapping[str, Any]) -> list[Span]:
    """T2 map file (+0x18/+0x1C): u32 section count, count x {start, menu, end} segment-5 records,
    then each section [start, end) followed by 8 zero bytes; zero fill to 16 at the end."""
    low, high = _scene_one(ranges, params, "scene_map")
    count = _scene_u32(original, low)
    table = low + 4 + _SCENE_SECTION * count
    if not 0 < count <= (high - low) // 64:
        raise AssetFormatError(f"scene_map {low:#x}: section count {count} is implausible")
    spans = [Span(low, low + 4, "scene_map_file.section_count", low)]
    cursor = table
    for i in range(count):
        at = low + 4 + _SCENE_SECTION * i
        start, menu, end = struct.unpack_from(">3I", original, at)
        if any(v >> 24 != _SCENE_SEGMENT_MAP for v in (start, menu, end)):
            raise AssetFormatError(f"scene_map {low:#x}: section {i} is not segment-5")
        s, e = low + (start & 0xFFFFFF), low + (end & 0xFFFFFF)
        if s != cursor or not s < low + (menu & 0xFFFFFF) < e or e + _SCENE_SECTION_GAP > high:
            raise AssetFormatError(f"scene_map {low:#x}: section {i} {s:#x}..{e:#x} is out of sequence")
        spans += _scene_fields(at, (("scene_section.start", 4), ("scene_section.menu", 4), ("scene_section.end", 4)))
        spans += _scene_section(original, low, s, menu, e, f"scene_map {low:#x} section {i}")
        if any(original[e:e + _SCENE_SECTION_GAP]):
            raise AssetFormatError(f"scene_map {low:#x}: bytes after section {i} are not zero")
        spans.append(Span(e, e + _SCENE_SECTION_GAP, PAD, e))
        cursor = e + _SCENE_SECTION_GAP
    if not 0 <= high - cursor < 16 or (high - low) % 16 or any(original[cursor:high]):
        raise AssetFormatError(f"scene_map {low:#x}: tail {cursor:#x}..{high:#x} is not zero fill to 16")
    if cursor < high:
        spans.append(Span(cursor, high, PAD, cursor))
    return spans


def parse_scene_sky(original: bytes, ranges: tuple[Interval, ...], params: Mapping[str, Any]) -> list[Span]:
    """T2 sky file (+0x28/+0x2C): segment-6 sky pointer padded to 8, then the optional CI8 backdrop
    (texels, a TLUT of at most 256 RGBA16 entries that every texel indexes, the 0x16-byte record
    padded to 0x18) and the 0x14-byte sky record; zero fill to 16 at the end."""
    low, high = _scene_one(ranges, params, "scene_sky")

    def ptr(value: int, what: str) -> int:
        if value >> 24 != _SCENE_SEGMENT_SKY:
            raise AssetFormatError(f"scene_sky {low:#x}: {what} {value:#010x} is not a segment-6 address")
        return low + (value & 0xFFFFFF)

    sky = ptr(_scene_u32(original, low), "sky pointer")
    if any(original[low + 4:low + 8]):
        raise AssetFormatError(f"scene_sky {low:#x}: header fill is not zero")
    spans = [Span(low, low + 4, "scene_sky_file.sky", low), Span(low + 4, low + 8, PAD, low + 4)]
    cursor = low + 8
    backdrop = _scene_u32(original, sky)
    if backdrop:
        b = ptr(backdrop, "backdrop")
        pixels, palette = ptr(_scene_u32(original, b), "texels"), ptr(_scene_u32(original, b + 4), "palette")
        width, height = struct.unpack_from(">HH", original, b + 8)
        entries = (b - palette) // 2
        if pixels != cursor or palette != pixels + width * height or (palette - low) % 8 or (b - palette) % 8 \
                or not 0 < entries <= _SCENE_SKY_TLUT_ENTRIES or sky != b + 0x18 or any(original[b + 0x16:b + 0x18]):
            raise AssetFormatError(f"scene_sky {low:#x}: backdrop layout does not chain texels, TLUT, record")
        if max(original[pixels:palette]) >= entries:
            raise AssetFormatError(f"scene_sky {low:#x}: a CI8 texel indexes past the {entries}-entry TLUT")
        if any(original[b + 0xC:b + 0xE]):
            raise AssetFormatError(f"scene_sky {low:#x}: backdrop +0xC holds {original[b + 0xC:b + 0xE].hex()}; "
                                   "no consumer reads it, so only a zero reserve is accepted")
        spans += [Span(pixels, palette, "scene_sky_texels.ci8", pixels),
                  Span(palette, b, "scene_sky_tlut.rgba16", palette)]
        spans += _scene_fields(b, (("scene_sky_backdrop.texels", 4), ("scene_sky_backdrop.tlut", 4),
                                   ("scene_sky_backdrop.width", 2), ("scene_sky_backdrop.height", 2),
                                   ("scene_sky_backdrop.reserved_0c", 2), ("scene_sky_backdrop.x", 2),
                                   ("scene_sky_backdrop.y", 2), ("scene_sky_backdrop.w", 2),
                                   ("scene_sky_backdrop.h", 2)))
        spans.append(Span(b + _SCENE_BACKDROP, b + 0x18, PAD, b + _SCENE_BACKDROP))
        cursor = sky
    if sky != cursor or sky + _SCENE_SKY > high or original[sky + 9] or original[sky + 0xF]:
        raise AssetFormatError(f"scene_sky {low:#x}: sky record {sky:#x} is misplaced or its band fill is set")
    spans += _scene_fields(sky, (("scene_sky.backdrop", 4), ("scene_sky.upper_y", 2), ("scene_sky.upper_r", 1),
                                 ("scene_sky.upper_g", 1), ("scene_sky.upper_b", 1)))
    spans.append(Span(sky + 9, sky + 10, PAD, sky + 9))
    spans += [Span(sky + 10, sky + 12, "scene_sky.lower_y", sky), Span(sky + 12, sky + 13, "scene_sky.lower_r", sky),
              Span(sky + 13, sky + 14, "scene_sky.lower_g", sky), Span(sky + 14, sky + 15, "scene_sky.lower_b", sky),
              Span(sky + 15, sky + 16, PAD, sky + 15), Span(sky + 16, sky + 20, "scene_sky.flags", sky)]
    end = sky + _SCENE_SKY
    if not 0 <= high - end < 16 or (high - low) % 16 or any(original[end:high]):
        raise AssetFormatError(f"scene_sky {low:#x}: tail {end:#x}..{high:#x} is not zero fill to 16")
    if end < high:
        spans.append(Span(end, high, PAD, end))
    return spans


# Fields the parsers require to be zero (high bytes of byte/halfword-read words, Group +0, and
# the backdrop member +0xC that lies between consumer-read height and x but is never read).
REQUIRED_ZERO = REQUIRED_ZERO | {"scene_map_info.room_count_high", "scene_group.field_00",
                                 "scene_sky_backdrop.reserved_0c"} | \
    {f"scene_room.side{k}_high" for k in range(4)}
PARSERS["scene_map"] = parse_scene_map
PARSERS["scene_sky"] = parse_scene_sky

# ---------------------------------------------------------------------------- EFAT chunk payloads (opus-06)

IFF_FORM, IFF_ESFC = 0x464F524D, 0x45534643
IFF_SHRF, IFF_EGRF = 0x53485246, 0x45475246
IFF_EGRP, IFF_TIMG = 0x45475250, 0x54494D47
IFF_MAX_BASE, IFF_MAX_GRAPHS = 3, 8   # func_8008E65C: at most three base chunks and eight EGRF chunks


def _iff_egrp(original: bytes, data: int, size: int, graphs: Sequence[int], spans: list[Span]) -> None:
    """EGRP body (func_8008D9A0 with func_8008D7E0/func_8008D8C0), all words read by func_8008DF04.

    {u32 count (the consumer keeps the low byte); count x group {graph_offset, list1_count,
    list1_count words, list2_count, list2_count words}}; the words consumed equal the chunk size.
    A group's graph_offset is added to the first EGRF chunk's address (Layout.graph_offset) and
    func_8008EA34 requires an EGRF chunk header there, so it must be the offset of one of this
    FORM's EGRF chunks. When the FORM has a single EGRF chunk the only such offset is 0, and the
    field is named egrp_group.sole_graph_offset (required zero).
    """
    end = data + size

    def word(at: int, name: str, base: int) -> int:
        if at + 4 > end:
            raise AssetFormatError(f"EGRP {name} at {at:#x} escapes its chunk")
        spans.append(Span(at, at + 4, name, base))
        return _u32(original, at)

    offsets = {g - graphs[0] for g in graphs}
    count = word(data, "egrp.count", data) & 0xFF
    cursor = data + 4
    for _ in range(count):
        group = cursor
        name = "egrp_group.sole_graph_offset" if len(graphs) == 1 else "egrp_group.graph_offset"
        if word(group, name, group) not in offsets:
            raise AssetFormatError(f"EGRP group at {group:#x} names no EGRF chunk of its FORM")
        cursor = group + 4
        for list_name in ("list1", "list2"):
            items = word(cursor, f"egrp_group.{list_name}_count", group)
            cursor += 4
            if items > (end - cursor) // 4:
                raise AssetFormatError(f"EGRP {list_name} at {cursor:#x} escapes its chunk")
            for _ in range(items):
                word(cursor, f"egrp_{list_name}.word", cursor)
                cursor += 4
    if cursor != end:
        raise AssetFormatError(f"EGRP at {data:#x} consumes {cursor - data} of {size} bytes")


def _iff_timg(original: bytes, data: int, size: int, spans: list[Span]) -> None:
    """TIMG body (func_8008DC30, func_8008DB48): {f32 x, f32 y, u32 count; count x {f32 value
    (func_8008E178), u8 attribute (a one-byte func_8008E0C4 copy)}}; 12 + 5*count equals the size."""
    if size < 12:
        raise AssetFormatError(f"TIMG at {data:#x} is shorter than its record")
    for offset, name in ((0, "timg.x"), (4, "timg.y"), (8, "timg.count")):
        spans.append(Span(data + offset, data + offset + 4, name, data))
    count = _u32(original, data + 8)
    if count > size or 12 + 5 * count != size:
        raise AssetFormatError(f"TIMG at {data:#x}: {count} keys do not fill {size} bytes")
    for k in range(count):
        key = data + 12 + 5 * k
        spans.append(Span(key, key + 4, "timg_key.value", key))
        spans.append(Span(key + 4, key + 5, "timg_attr.value", key + 4))


def _iff_form_payloads(original: bytes, start: int, end: int, spans: list[Span]) -> list[Interval]:
    """Walk one directory-bound ESFC FORM; return its payload intervals, appending their spans.

    Chunks follow the 12-byte FORM header back to back as {u32 tag, u32 size, data} and end
    exactly at the FORM end (func_8008E65C). Base chunks (EGRP, TIMG; at most three, all before
    any SHRF or EGRF) are typed whole. SHRF (at most one) and EGRF (at most eight) data are a u32
    unpacked size followed by a Yay0 stream filling the chunk (func_8008E8E4, func_8008EA34 read
    the size with func_8008DF04 and decompress from data + 4); the stream itself is the separate
    asset_yay0 row, so each payload interval ends after the unpacked size and the next one
    starts at the end of that chunk.
    """
    if _u32(original, start) != IFF_FORM or _u32(original, start + 8) != IFF_ESFC:
        raise AssetFormatError(f"EFAT FORM at {start:#x} is not a compressed ESFC FORM")
    walk: list[tuple[int, int, int]] = []
    cursor = start + 12
    while cursor < end:
        if cursor + 8 > end:
            raise AssetFormatError(f"EFAT chunk header at {cursor:#x} escapes its FORM")
        tag, size = _u32(original, cursor), _u32(original, cursor + 4)
        if size > end - cursor - 8:
            raise AssetFormatError(f"EFAT chunk at {cursor:#x} escapes its FORM")
        walk.append((cursor, tag, size))
        cursor += 8 + size
    graphs = [c for c, tag, _ in walk if tag == IFF_EGRF]
    shared = [c for c, tag, _ in walk if tag == IFF_SHRF]
    base = [i for i, (_, tag, _) in enumerate(walk) if tag not in (IFF_SHRF, IFF_EGRF)]
    if (not graphs or len(graphs) > IFF_MAX_GRAPHS or len(shared) > 1 or len(base) > IFF_MAX_BASE
            or base != list(range(len(base)))):
        raise AssetFormatError(f"EFAT FORM at {start:#x} chunk order or counts are not func_8008E65C's")
    payloads: list[Interval] = []
    first = start + 12
    for chunk, tag, size in walk:
        spans.append(Span(chunk, chunk + 4, "chunk.tag", chunk))
        spans.append(Span(chunk + 4, chunk + 8, "chunk.size", chunk))
        data = chunk + 8
        if tag == IFF_EGRP:
            _iff_egrp(original, data, size, graphs, spans)
        elif tag == IFF_TIMG:
            _iff_timg(original, data, size, spans)
        elif tag in (IFF_SHRF, IFF_EGRF):
            if (size < 4 + 16 or original[data + 4:data + 8] != b"Yay0"
                    or _u32(original, data + 8) != _u32(original, data)):
                raise AssetFormatError(f"EFAT chunk at {chunk:#x} is not an unpacked size and its Yay0 stream")
            spans.append(Span(data, data + 4, "packed.unpacked_size", data))
            payloads.append((first, data + 4))
            first = data + size
        else:
            raise AssetFormatError(f"EFAT base chunk at {chunk:#x} has no derived grammar")
    if first != end:
        raise AssetFormatError(f"EFAT FORM at {start:#x} ends with uncompressed data")
    return payloads


def parse_iff_efat_payload(original: bytes, ranges: tuple[Interval, ...], params: Mapping[str, Any]) -> list[Span]:
    """Non-Yay0 chunk payloads of the EFAT archive's ESFC FORMs.

    The single context interval is the EFAT directory row {"EFAT", u32 count, count x (u32
    offset, u32 size)} ending at the first FORM; entry (offset, size) binds FORM [first + offset,
    first + offset + 8 + size) or is (0, 0) (func_8008E354, func_8008E3B0). Every FORM is walked
    (`_iff_form_payloads`); every closed range must be one of the walked payload intervals, and only
    closed ranges are typed (a payload interval may stay open).
    """
    if set(params) != {"context"} or len(params["context"]) != 1:
        raise AssetFormatError("EFAT payloads take the EFAT directory row as their only context")
    directory, first = params["context"][0]
    count = _u32(original, directory + 4)
    if original[directory:directory + 4] != b"EFAT" or first != directory + 8 + 8 * count:
        raise AssetFormatError("EFAT directory header does not end at the first FORM")
    closed = set(ranges)
    if len(closed) != len(ranges):
        raise AssetFormatError("EFAT payload ranges repeat")
    spans: list[Span] = []
    found: set[Interval] = set()
    for index in range(count):
        offset, size = struct.unpack_from(">II", original, directory + 8 + 8 * index)
        if not offset and not size:
            continue
        form: list[Span] = []
        payloads = _iff_form_payloads(original, first + offset, first + offset + 8 + size, form)
        keep = [p for p in payloads if p in closed]
        found |= set(keep)
        spans.extend(s for s in form if any(a <= s.start < b for a, b in keep))
    if found != closed:
        raise AssetFormatError("EFAT payload ranges are not walked chunk payload intervals")
    return spans


REQUIRED_ZERO = REQUIRED_ZERO | {"egrp_group.sole_graph_offset"}
PARSERS["iff_efat_payload"] = parse_iff_efat_payload


# ------------------------------------------------------------ sprite bank and palettes, T3 model groups, GBI segments
#
# Author: opus-07 (Anthropic, claude-opus-5-5, xhigh). Layered on the assetformat framework:
# expects AssetFormatError, Span, PAD, PARSERS, REQUIRED_ZERO, _cb_one, _cb_word and parse_tlut16
# from the enclosing module. Grammar sources (matched C in src/units/main_14400; the binary accesses
# are pinned in config/rom-evidence/asset-formats.json):
#   sprite_bank    N_e53db0_block, the segment-5 sprite directory at 0xE53DB0. func_80071CCC bounds the
#                  id with sltiu 0x195 and transfers 16-byte headers {u8 flags (palette count = flags & 0x1F),
#                  3 fill, u32 segment-5 palette words, u8 frame count, 3 fill, u32 segment-5 frames}; it also
#                  transfers 12-byte frames (addiu a2, zero, 0xC) and 4-byte palette words (addiu a2, zero, 4).
#                  func_80074500 copies the same header (0x10) and the whole frame table (count * 12) for unit
#                  poses. Frame fields and their readers:
#                    u32 segment-6 Yay0 address +0, u16 compressed size +4, u8 width +6, u8 height +7
#                      (func_800724DC, which decodes (width * height) >> 1 bytes);
#                    s8 origin x +8 and s8 origin y +9 (func_80071420 / func_80070398, lb);
#                    s8 step +0xA (func_8007A680, lb: Pose.step_0A advances frame_3F);
#                    +0xB, which nothing reads and which must be zero.
#                  func_80072440 adds 0xE7BE70 to segment-6 words.
#   sprite_tlut16  S_sprite_palette: the 0x20-byte segment-6 TLUTs that func_80071CCC transfers for the
#                  palette words of the sprite directory (owner_word anchors of rule S_sprite_palette).
#   model_groups   T3 raw windows. func_80067628 DMAs the record's +0x14..+0x18 window to the scene buffer
#                  and decodes the record's +0x1C Yay0 stream to D_801D2C08, which func_800612D4 places
#                  0x880 bytes after the window (addiu v1, v0, 0x880). func_80067860 rebases model.groups
#                  (segment 5), reads model.count and rebases every nonzero group (segment 6), then walks
#                  each group's NULL-terminated node list.
#   gbi_segment    I_raw "GFX static" (segment 1) and "WINDOW static" (segment 5): F3DEX2 display lists that
#                  the CPU enters through segmented addresses (lui/addiu pairs in func_8005F210,
#                  func_8005C790, func_8007F350), including G_DL calls within the segment and the viewport
#                  that a G_MOVEMEM (index G_MV_VIEWPORT) loads from the segment.

_MODEL_HEADER = 12
_SPRITE_HEADER = 16
_SPRITE_SEGMENT_BANK = 5
_SPRITE_SEGMENT_DATA = 6


def _pinned_address(data: bytes, pair: Mapping[str, Any], label: str) -> int:
    """Value of a pinned `lui rX, hi` + `addiu/ori rX, rX, lo` pair (both original words)."""
    hi = _cb_word(data, pair["lui_site"], pair["lui_word"], f"{label} lui")
    lo = _cb_word(data, pair["low_site"], pair["low_word"], f"{label} low")
    if hi >> 26 != 0x0F or lo >> 26 not in (0x09, 0x0D) or (lo >> 21) & 31 != (hi >> 16) & 31:
        raise AssetFormatError(f"{label} is not a lui/addiu address pair")
    low = lo & 0xFFFF
    if lo >> 26 == 0x09 and low & 0x8000:
        low -= 0x10000
    return ((hi & 0xFFFF) << 16) + low & 0xFFFFFFFF


def _pinned_size(data: bytes, pin: Mapping[str, Any], label: str) -> int:
    """Immediate of a pinned `addiu rX, rY, size` word (a size argument or buffer offset)."""
    word = _cb_word(data, pin["site"], pin["word"], label)
    if word >> 26 != 0x09 or word & 0x8000:
        raise AssetFormatError(f"{label} is not a nonnegative addiu immediate")
    return word & 0xFFFF


def parse_sprite_bank(original: bytes, intervals, params) -> list:
    """The segment-5 sprite directory: `count` headers, then each sprite's frame and palette arrays.

    Header 0 is the all-zero null sprite (func_80071CCC treats id 0 as empty). Every other header
    has nonzero frame and palette counts and segment-5 pointers. The arrays tile the rest of the
    bank in address order; between them and at the end only zero fill shorter than 16 bytes that
    ends on a 16-byte boundary is allowed. Every frame addresses a segment-6 Yay0 stream whose
    declared size is (width * height) >> 1 and whose header offsets lie inside its compressed size.
    Its +0xB byte, which no consumer reads, is zero. Every palette word is a segment-6 address of a
    32-byte TLUT inside the ROM."""
    start, end = _cb_one(intervals)
    count = params["count"]
    bound = _cb_word(original, params["id_bound"]["site"], params["id_bound"]["word"], "sprite id bound")
    if bound >> 26 != 0x0B or bound & 0xFFFF != count:  # sltiu rX, rY, count
        raise AssetFormatError("sprite id bound is not sltiu with the pinned count")
    mask = _cb_word(original, params["palette_mask"]["site"], params["palette_mask"]["word"], "palette mask")
    if mask >> 26 != 0x0C or mask & 0xFFFF != 0x1F:  # andi rX, rY, 0x1F
        raise AssetFormatError("palette count mask is not andi 0x1F")
    frame_size = _pinned_size(original, params["frame_size"], "frame transfer size")
    word_size = _pinned_size(original, params["palette_word_size"], "palette word transfer size")
    tlut_size = _pinned_size(original, params["tlut_size"], "palette transfer size")
    if (frame_size, word_size, tlut_size) != (12, 4, 32):
        raise AssetFormatError("sprite record sizes disagree with their consumer transfers")
    data_base = _pinned_address(original, params["segment6"], "segment-6 base")
    size = end - start
    if count * _SPRITE_HEADER > size:
        raise AssetFormatError("sprite headers run past the bank")
    u32 = lambda at: struct.unpack_from(">I", original, at)[0]  # noqa: E731
    spans: list[Span] = []
    arrays: list[tuple[int, int, int]] = []  # (bank offset, record size, record count)
    for index in range(count):
        h = start + _SPRITE_HEADER * index
        flags, palettes, frames_count, frames = original[h], u32(h + 4), original[h + 8], u32(h + 12)
        if any(original[h + 1:h + 4]) or any(original[h + 9:h + 12]):
            raise AssetFormatError(f"sprite header {index} fill bytes are nonzero")
        spans += [Span(h, h + 1, "sprite_header.flags", h), Span(h + 1, h + 4, "sprite_header.fill_1", h),
                  Span(h + 4, h + 8, "sprite_header.palettes", h), Span(h + 8, h + 9, "sprite_header.frame_count", h),
                  Span(h + 9, h + 12, "sprite_header.fill_9", h), Span(h + 12, h + 16, "sprite_header.frames", h)]
        if index == 0:
            if any(original[h:h + _SPRITE_HEADER]):
                raise AssetFormatError("sprite 0 is not the null descriptor")
            continue
        if not frames_count or not flags & 0x1F:
            raise AssetFormatError(f"sprite {index} has no frame or no palette")
        for pointer, record, number in ((frames, frame_size, frames_count), (palettes, word_size, flags & 0x1F)):
            if pointer >> 24 != _SPRITE_SEGMENT_BANK:
                raise AssetFormatError(f"sprite {index} array pointer {pointer:#x} is not segment 5")
            arrays.append((pointer & 0xFFFFFF, record, number))
    cursor = count * _SPRITE_HEADER
    for offset, record, number in sorted(arrays):
        if offset != cursor:
            if not (cursor < offset < cursor + 16 and offset % 16 == 0) or any(original[start + cursor:start + offset]):
                raise AssetFormatError(f"sprite array at +{offset:#x} does not follow the previous one")
            spans.append(Span(start + cursor, start + offset, PAD, start + cursor))
        stop = offset + record * number
        if stop > size:
            raise AssetFormatError(f"sprite array at +{offset:#x} runs past the bank")
        for p in range(start + offset, start + stop, record):
            if record == word_size:
                word = u32(p)
                if word >> 24 != _SPRITE_SEGMENT_DATA or data_base + (word & 0xFFFFFF) + tlut_size > len(original):
                    raise AssetFormatError(f"palette word at {p:#x} is not a segment-6 TLUT address")
                spans.append(Span(p, p + 4, "sprite_palette_ref.word", p))
                continue
            address, (packed,) = u32(p), struct.unpack_from(">H", original, p + 4)
            width, height = original[p + 6], original[p + 7]
            if address >> 24 != _SPRITE_SEGMENT_DATA or not width or not height:
                raise AssetFormatError(f"sprite frame at {p:#x} is not a segment-6 picture")
            stream = data_base + (address & 0xFFFFFF)
            if stream + packed > len(original) or original[stream:stream + 4] != b"Yay0":
                raise AssetFormatError(f"sprite frame at {p:#x} does not address a Yay0 stream")
            decoded, links, literals = struct.unpack_from(">III", original, stream + 4)
            if decoded != (width * height) >> 1 or not 16 <= links <= literals <= packed:
                raise AssetFormatError(f"sprite frame at {p:#x} disagrees with its Yay0 header")
            if original[p + 11]:
                raise AssetFormatError(f"sprite frame at {p:#x} has a nonzero unread byte +0xB")
            spans += [Span(p, p + 4, "sprite_frame.data", p), Span(p + 4, p + 6, "sprite_frame.size", p),
                      Span(p + 6, p + 7, "sprite_frame.width", p), Span(p + 7, p + 8, "sprite_frame.height", p),
                      Span(p + 8, p + 9, "sprite_frame.origin_x", p), Span(p + 9, p + 10, "sprite_frame.origin_y", p),
                      Span(p + 10, p + 11, "sprite_frame.step", p), Span(p + 11, p + 12, "sprite_frame.pad_0b", p)]
        cursor = stop
    if cursor != size:
        if size % 16 or size - cursor >= 16 or any(original[start + cursor:end]):
            raise AssetFormatError("sprite bank tail is not short zero fill to a 16-byte boundary")
        spans.append(Span(start + cursor, end, PAD, start + cursor))
    return spans


def parse_model_groups(original: bytes, intervals, params) -> list:
    """T3 model-group windows: {segment-5 groups pointer to +12, u32 count, u32 0} and `count`
    segment-6 group-list pointers, padded with zero to the 0x880-byte window that precedes the
    decoded model. Each range is paired, through the original T3 record words, with the Yay0 model
    stream of the same record (context); every nonzero group addresses a NULL-terminated list of
    4-aligned segment-6 node pointers inside the decoded model."""
    from assets import AssetError, decode_yay0

    window = _pinned_size(original, params["decoded_offset"], "decoded model offset")
    groups_segment = _pinned_size(original, params["groups_segment"], "groups segment")
    list_segment = _pinned_size(original, params["list_segment"], "group list segment")
    table, stride, records = params["table"], params["record_size"], params["records"]
    bound = _cb_word(original, params["record_bound"]["site"], params["record_bound"]["word"], "T3 record bound")
    if bound >> 26 != 0x0B or bound & 0xFFFF != records:  # sltiu rX, rY, records
        raise AssetFormatError("T3 record bound is not sltiu with the pinned record count")
    context = params["context"]
    if len(intervals) != records or len(context) != records or stride < 0x24:
        raise AssetFormatError("model groups need one window and one model stream per T3 record")
    u32 = lambda data, at: struct.unpack_from(">I", data, at)[0]  # noqa: E731
    spans: list[Span] = []
    for index, ((start, end), (model_start, model_end)) in enumerate(zip(intervals, context)):
        record = table + stride * index
        stored_end = u32(original, record + 0x20)
        if (u32(original, record + 0x14), u32(original, record + 0x18)) != (start, end) or \
                u32(original, record + 0x1C) != model_start or not model_end <= stored_end < model_end + 16:
            raise AssetFormatError(f"T3 record {index} does not pair this window with this model stream")
        if end - start != window:
            raise AssetFormatError(f"model window {index} is not the {window:#x}-byte buffer before the model")
        groups, count, reserved = u32(original, start), u32(original, start + 4), u32(original, start + 8)
        if groups != (groups_segment << 24 | _MODEL_HEADER) or reserved:
            raise AssetFormatError(f"model window {index} header is not {{segment-5 groups at +12, count, 0}}")
        table_end = _MODEL_HEADER + 4 * count
        if not table_end <= window < table_end + 16 or any(original[start + table_end:end]):
            raise AssetFormatError(f"model window {index} group count does not fill the window")
        try:
            model, _ = decode_yay0(original[model_start:model_end])
        except AssetError as error:
            raise AssetFormatError(f"model stream {index} does not decode: {error}") from error
        spans += [Span(start, start + 4, "model.groups", start), Span(start + 4, start + 8, "model.count", start),
                  Span(start + 8, start + 12, "model.word_08", start)]
        for p in range(start + _MODEL_HEADER, start + table_end, 4):
            pointer = u32(original, p)
            if pointer:
                at = pointer & 0xFFFFFF
                if pointer >> 24 != list_segment or at % 4:
                    raise AssetFormatError(f"group pointer at {p:#x} is not an aligned segment-6 list")
                while True:
                    if at + 4 > len(model):
                        raise AssetFormatError(f"group list at {p:#x} runs past the decoded model")
                    node = u32(model, at)
                    if not node:
                        break
                    if node >> 24 != list_segment or node % 4 or (node & 0xFFFFFF) >= len(model):
                        raise AssetFormatError(f"group list at {p:#x} holds a non-node word {node:#x}")
                    at += 4
            spans.append(Span(p, p + 4, "model_group.list", p))
        if table_end != window:
            spans.append(Span(start + table_end, end, PAD, start + table_end))
    return spans


# F3DEX2 GBI opcodes that occur in these segments, and the commands whose operands are all zero.
_GBI_OPCODES = {0xD7: "texture", 0xD9: "geometrymode", 0xDC: "movemem", 0xDE: "dl", 0xDF: "enddl",
                0xE2: "setothermode_l", 0xE3: "setothermode_h", 0xE6: "loadsync", 0xE7: "pipesync",
                0xE8: "tilesync", 0xEA: "setkeygb", 0xEB: "setkeyr", 0xEC: "setconvert", 0xEE: "setprimdepth",
                0xF0: "loadtlut", 0xF2: "settilesize", 0xF3: "loadblock", 0xF5: "settile", 0xF7: "setfillcolor",
                0xF8: "setfogcolor", 0xF9: "setblendcolor", 0xFA: "setprimcolor", 0xFB: "setenvcolor",
                0xFC: "setcombine", 0xFD: "settimg"}
_GBI_ZERO_OPERANDS = frozenset({0xDF, 0xE6, 0xE7, 0xE8})
_GBI_MV_VIEWPORT = 8
_GBI_VIEWPORT = 16


def parse_gbi_segment(original: bytes, intervals, params) -> list:
    """A display-list segment: every byte is a command of a display list entered from a pinned CPU
    segment address (or called from such a list with G_DL) or the viewport a G_MOVEMEM loads from the
    segment; lists end at G_ENDDL, commands are 8-byte F3DEX2 words of known opcodes, sync and end
    commands carry zero operands, a viewport's vscale[3]/vtrans[3] padding is zero, and nothing else
    may remain."""
    start, end = _cb_one(intervals)
    segment, struct_name = params["segment"], params["struct"]
    if not isinstance(struct_name, str) or not struct_name.isidentifier() or segment not in range(1, 16):
        raise AssetFormatError("GBI segment parameters are invalid")
    size = end - start
    commands: dict[int, int] = {}
    data: dict[int, int] = {}
    pending: list[int] = []
    for entry in params["entries"]:
        address = _pinned_address(original, entry, "display-list entry")
        if address >> 24 != segment or address & 0xFFFFFF != entry["offset"]:
            raise AssetFormatError(f"display-list entry {address:#x} is not segment {segment} +{entry['offset']:#x}")
        pending.append(entry["offset"])
    if not pending:
        raise AssetFormatError("GBI segment has no pinned entry")
    while pending:
        offset = pending.pop()
        while offset not in commands:
            if offset % 8 or not 0 <= offset <= size - 8:
                raise AssetFormatError(f"display list leaves the segment at +{offset:#x}")
            w0, w1 = struct.unpack_from(">II", original, start + offset)
            op = w0 >> 24
            if op not in _GBI_OPCODES:
                raise AssetFormatError(f"unknown GBI opcode {op:#x} at +{offset:#x}")
            if op in _GBI_ZERO_OPERANDS and (w0 & 0xFFFFFF or w1):
                raise AssetFormatError(f"{_GBI_OPCODES[op]} at +{offset:#x} has nonzero operands")
            commands[offset] = op
            if op == 0xDF:
                break
            if op == 0xDE:
                if (w0 >> 16) & 0xFF not in (0, 1) or w0 & 0xFFFF or (w1 >> 24) & 0xF != segment:
                    raise AssetFormatError(f"G_DL at +{offset:#x} does not target this segment")
                if (w0 >> 16) & 0xFF:  # branch: this list continues at the target
                    offset = w1 & 0xFFFFFF
                    continue
                pending.append(w1 & 0xFFFFFF)
            elif op == 0xDC and (w1 >> 24) & 0xF == segment:
                length = ((w0 >> 19) & 0x1F) * 8 + 8
                if w0 & 0xFF != _GBI_MV_VIEWPORT or length != _GBI_VIEWPORT or (w0 >> 8) & 0xFF:
                    raise AssetFormatError(f"G_MOVEMEM at +{offset:#x} is not a viewport load")
                data[w1 & 0xFFFFFF] = length
            offset += 8
    spans: list[Span] = []
    for offset, op in commands.items():
        spans.append(Span(start + offset, start + offset + 8, f"{struct_name}.dl_{_GBI_OPCODES[op]}", start))
    for offset, length in data.items():
        if offset % 8 or offset + length > size or any(o in commands for o in range(offset, offset + length, 8)):
            raise AssetFormatError(f"viewport at +{offset:#x} overlaps commands or leaves the segment")
        vp = start + offset
        if original[vp + 6:vp + 8] != b"\0\0" or original[vp + 14:vp + 16] != b"\0\0":
            raise AssetFormatError(f"viewport at +{offset:#x} has nonzero Vp padding (vscale[3]/vtrans[3])")
        spans += [Span(start + offset, start + offset + 8, f"{struct_name}.viewport_scale", start),
                  Span(start + offset + 8, start + offset + 16, f"{struct_name}.viewport_trans", start)]
    return spans


def parse_sprite_tlut16(original: bytes, intervals, params) -> list:
    """The segment-6 sprite palettes (S_sprite_palette): every interval is one 16-entry RGBA16 TLUT
    typed as `tlut16` (entry count from the consumer's G_LOADTLUT word). Each interval's start is
    bound by an owner_word anchor to the sprite-bank palette word that addresses it."""
    if not intervals:
        raise AssetFormatError("sprite palettes need at least one TLUT")
    spans: list[Span] = []
    for interval in intervals:
        spans += parse_tlut16(original, (interval,), params)
    return spans


REQUIRED_ZERO = REQUIRED_ZERO | {"model.word_08", "sprite_header.fill_1", "sprite_header.fill_9", "sprite_frame.pad_0b"}
PARSERS["sprite_bank"] = parse_sprite_bank
PARSERS["sprite_tlut16"] = parse_sprite_tlut16
PARSERS["model_groups"] = parse_model_groups
PARSERS["gbi_segment"] = parse_gbi_segment


# ---------------------------------------------------------------------------- segment-2 game data bank
#
# Author: opus-10 (Anthropic, claude-opus-5-5, xhigh). Layered on the assetformat framework after
# opus-08's scene block: expects AssetFormatError, Span, PAD, PARSERS, REQUIRED_ZERO and _cb_word
# from the enclosing module. B_194fc0 is the bank that func_8006AC30 reads: it adds a record
# offset `stride * first` (odd strides rounded up to even) to the bank ROM base plus the segment
# offset of a segment-2 address (`segAddr & 0xFFFFFF`) and DMAs `stride * count` bytes. Each
# witness range is a reviewed sub-range of the one located map row (params["within"]); a segment
# offset `o` is ROM `parent start + o`. The matched consumers that fix each layout:
#   seg2_level           func_80044C60 (seg 0x0, 0x318), func_80044DCC (0x630), func_80044D74
#                        (0x948), func_80044D1C (0xC60), func_80044E24 (0xF78): 8-byte records,
#                        index level-1 (u32 +0, then four bytes; +6, +7 read by the callers)
#   seg2_special         func_80044E7C: the single 8-byte record at 0x15A8 (+6 u16)
#   seg2_floor_header    func_800ABB50: 0xC-byte records of the resource's header table, index
#                        floor; +4 is the row that func_800ABBA0 uses for the four floor tables
#   seg2_floor_record    func_800ABBA0: 0x26-byte records (u16 at +0xE, other members bytes)
#   seg2_*list_ref       func_800ABBA0: 4-byte segment pointers to a list (tables a/b/c);
#                        func_800ABEE4/func_800ABF64/func_800ABFE4 read seg2_ilist_ref tables
#   seg2_mlist_entry     func_800A9C8C/func_800AA7CC: {u8 id, u8 level, u16 weight} until id 0
#                        and weight 0 (26 entries are transferred)
#   seg2_ilist_entry     func_800AB1C8: {u8 id, u8 weight, u16 0, u32 wlist} until id 0 and
#                        wlist 0 (21 entries)
#   seg2_tlist_entry     func_800A9C8C/func_800AAFC8: {u8 id, u8 weight} until id 0 (25 entries)
#   seg2_wlist_entry     func_800AB2B0 via func_800AC064/func_800ABEA4: {u8 id, u8 weight}
#                        until id 0 (62 entries)
#   seg2_kind_range(_b)  func_80044ECC/func_8004505C (58 kinds from 29) and func_80045134 (8
#                        kinds from 0x57): {u16 first, u8 count, u8}; the counts partition the
#                        per-level record tables seg2_kind_stats/seg2_kind_attrs (func_80044FDC,
#                        func_80044F5C) and seg2_kind_b_stats (func_800451A8)
#   seg2_weights14/10/11 func_800ABD24/func_800ABDFC/func_800ABE50 -> func_800AA3D8: one byte per
#                        weight, records indexed by floor record +0x00/+0x09/+0x20
#   seg2_values6         func_800ABD94: 6-byte records indexed by floor record +0x18; it returns
#                        byte (kind - 0x18) for kinds 0x18..0x1B
# The rest of the bank stays located: the 19-entry directory of the table-a reference tables at
# 0x37F4 and two identical 21-record blocks at 0x25F18 and 0x2AEA8 have no consumer.


def _s2_pair(data: bytes, pin: object, label: str) -> int:
    """Value of a pinned lui + addiu/lw pair (the second uses the first's register)."""
    if not isinstance(pin, list) or len(pin) != 2 or any(not isinstance(p, list) or len(p) != 2 for p in pin):
        raise AssetFormatError(f"{label} must pin [[lui site, word], [low site, word]]")
    hi = _cb_word(data, pin[0][0], pin[0][1], f"{label} lui")
    lo = _cb_word(data, pin[1][0], pin[1][1], f"{label} low half")
    if hi >> 26 != 0x0F or lo >> 26 not in (0x09, 0x23) or (lo >> 21) & 31 != (hi >> 16) & 31:
        raise AssetFormatError(f"{label} is not a lui + addiu/lw pair")
    return ((hi & 0xFFFF) << 16) + (lo & 0xFFFF) - ((lo & 0x8000) << 1) & 0xFFFFFFFF


def _s2_imm(data: bytes, pin: object, opcode: int, label: str, from_zero: bool = True) -> int:
    """Immediate of a pinned instruction with the given opcode (sltiu, or addiu from zero or,
    with from_zero False, from another register)."""
    if not isinstance(pin, list) or len(pin) != 2:
        raise AssetFormatError(f"{label} must pin [site, word]")
    word = _cb_word(data, pin[0], pin[1], label)
    if word >> 26 != opcode or (opcode == 0x09 and ((word >> 21) & 31 == 0) != from_zero):
        raise AssetFormatError(f"{label} is not the expected instruction")
    return (word & 0xFFFF) - ((word & 0x8000) << 1)


# Record layouts (member, width), in record order.
_S2_LAYOUTS = {
    "seg2_level": (("field_00", 4), ("field_04", 1), ("field_05", 1), ("field_06", 1), ("field_07", 1)),
    "seg2_special": tuple((f"field_{i:02x}", 1) for i in range(6)) + (("field_06", 2),),
    "seg2_floor_header": (("kind", 1), ("variant", 1), ("field_02", 1), ("flags", 1), ("row", 1),
                          ("field_05", 1), ("field_06", 2), ("mode", 1), ("coord_x", 1), ("coord_y", 1),
                          ("status", 1)),
    "seg2_floor_record": tuple((f"field_{i:02x}", 1) for i in range(0xE)) + (("field_0e", 2),)
    + tuple((f"field_{i:02x}", 1) for i in range(0x10, 0x26)),
    "seg2_mlist_ref": (("list", 4),),
    "seg2_ilist_ref": (("list", 4),),
    "seg2_tlist_ref": (("list", 4),),
    "seg2_mlist_entry": (("id", 1), ("level", 1), ("weight", 2)),
    "seg2_ilist_entry": (("id", 1), ("weight", 1), ("pad_02", 2), ("wlist", 4)),
    "seg2_tlist_entry": (("id", 1), ("weight", 1)),
    "seg2_wlist_entry": (("id", 1), ("weight", 1)),
    "seg2_kind_range": (("first", 2), ("count", 1), ("field_03", 1)),
    "seg2_kind_range_b": (("first", 2), ("count", 1), ("field_03", 1)),
    "seg2_kind_stats": (("field_00", 2), ("field_02", 2), ("field_04", 2), ("field_06", 1), ("level_07", 1))
    + tuple((f"field_{i:02x}", 1) for i in range(8, 0x18)),
    "seg2_kind_attrs": (("field_00", 2), ("field_02", 2), ("field_04", 2), ("flags_06", 1), ("field_07", 1)),
    "seg2_kind_b_stats": (("field_00", 2), ("field_02", 2), ("field_04", 2), ("field_06", 1), ("field_07", 1),
                          ("field_08", 1), ("field_09", 1), ("field_0a", 2))
    + tuple((f"field_{i:02x}", 1) for i in range(0xC, 0x18)),
    "seg2_weights14": tuple((f"weight_{i:02x}", 1) for i in range(14)),
    "seg2_weights10": tuple((f"weight_{i:02x}", 1) for i in range(10)),
    "seg2_weights11": tuple((f"weight_{i:02x}", 1) for i in range(11)),
    "seg2_weights14": tuple((f"weight_{i:02x}", 1) for i in range(14)),
    "seg2_values6": tuple((f"value_{i:02x}", 1) for i in range(6)),
}
# List grammars: entry struct, entry size, transferred entry count, terminator test.
_S2_LISTS = {"mlist": ("seg2_mlist_entry", 4, 0x1A), "ilist": ("seg2_ilist_entry", 8, 0x15),
             "tlist": ("seg2_tlist_entry", 2, 0x19), "wlist": ("seg2_wlist_entry", 2, 0x3E)}
_S2_REF_LIST = {"seg2_mlist_ref": "mlist", "seg2_ilist_ref": "ilist", "seg2_tlist_ref": "tlist"}
# The ilist entry's halfword between weight and wlist pointer is never read and must be zero.
_S2_ZERO = frozenset({"seg2_ilist_entry.pad_02"})


def _s2_record(spans: list, at: int, name: str) -> None:
    cursor = at
    for member, width in _S2_LAYOUTS[name]:
        spans.append(Span(cursor, cursor + width, f"{name}.{member}", at))
        cursor += width


def _s2_seg(word: int, size: int, label: str) -> int:
    if word >> 24 != 2 or (word & 0xFFFFFF) >= size:
        raise AssetFormatError(f"{label} {word:#010x} is not a segment-2 address inside the bank")
    return word & 0xFFFFFF


def parse_seg2_game_data(original: bytes, intervals, params) -> list:
    """Typed sub-ranges of the segment-2 game data bank (see the block comment).

    Fixed tables come from consumer-pinned bases, strides and counts; the dungeon tables from
    the 21 resource records of the CPU table D_80142B1C (context row): header table (count =
    metadata[0]), and, indexed by header +4, the floor record and list-reference tables whose
    reviewed extents (`extents`) cover at least every referenced row. Lists are parsed from
    every reference entry; weight indices of every floor record must lie inside their tables.
    Gaps between objects are short zero fill ending at 4- or 16-byte alignment."""
    within = params.get("within")
    if not isinstance(within, tuple) or len(within) != len(intervals) or not intervals:
        raise AssetFormatError("seg2 game data witnesses type reviewed sub-ranges of one row")
    parents = {tuple(w) if w is not None else None for w in within}
    if len(parents) != 1 or None in parents:
        raise AssetFormatError("seg2 game data sub-ranges must share one parent row")
    (base, limit), = parents
    size = limit - base
    ranges = sorted(intervals)
    if any(not base <= a < b <= limit for a, b in ranges):
        raise AssetFormatError("seg2 game data sub-range lies outside its parent")
    spans: list = []
    objects: dict[int, tuple[int, str]] = {}  # segment offset -> (end, kind)

    def claim(start: int, end: int, kind: str) -> None:
        if not 0 <= start < end <= size:
            raise AssetFormatError(f"seg2 {kind} {start:#x}..{end:#x} runs outside the bank")
        if start in objects:
            raise AssetFormatError(f"seg2 {kind} at {start:#x} is claimed twice")
        objects[start] = (end, kind)

    # Fixed tables.
    tables = params["tables"]
    for t in tables:
        name, seg, count, stride = t["struct"], t["seg"], t["count"], t["stride"]
        layout = _S2_LAYOUTS.get(name)
        if layout is None or sum(w for _, w in layout) != stride or type(count) is not int or count <= 0:
            raise AssetFormatError(f"seg2 table {seg:#x}: struct {name!r} has no {stride}-byte layout")
        pins = t["pins"]
        if pins is None:
            if not t.get("layout_only"):
                raise AssetFormatError(f"seg2 table {seg:#x} has no pinned consumer base")
        else:
            if _s2_pair(original, pins["base"], f"seg2 table {seg:#x} base") != 0x02000000 | seg:
                raise AssetFormatError(f"seg2 table {seg:#x}: pinned base disagrees")
            if _s2_imm(original, pins["stride"], 0x09, f"seg2 table {seg:#x} stride") != stride:
                raise AssetFormatError(f"seg2 table {seg:#x}: pinned stride disagrees")
            if "count" in pins and _s2_imm(original, pins["count"], 0x0B, f"seg2 table {seg:#x} count") != count:
                raise AssetFormatError(f"seg2 table {seg:#x}: pinned count disagrees")
        step = stride + (stride & 1)
        claim(seg, seg + count * step, name)
        for i in range(count):
            at = base + seg + i * step
            _s2_record(spans, at, name)
            if step != stride:
                if original[at + stride]:
                    raise AssetFormatError(f"seg2 record {at:#x} rounding byte is not zero")
                spans.append(Span(at + stride, at + step, PAD, at + stride))
    by_seg = {t["seg"]: t for t in tables}

    # Kind ranges partition their per-level record tables.
    for ranges_seg, records in params["kind_partitions"]:
        table, total = by_seg[ranges_seg], 0
        for i in range(table["count"]):
            first, count = struct.unpack_from(">HB", original, base + ranges_seg + 4 * i)
            if first != total or not count:
                raise AssetFormatError(f"seg2 kind range {ranges_seg:#x}[{i}] does not continue the partition")
            total += count
        for seg in records:
            if by_seg[seg]["count"] != total:
                raise AssetFormatError(f"seg2 record table {seg:#x} does not have the {total} partitioned records")

    # Dungeon resources (CPU context table) and their floor tables.
    res = params["resources"]
    (context,) = params["context"]
    vram = _s2_pair(original, res["vram"], "seg2 resource table")
    rom = vram - res["vram_delta"]
    if rom != res["rom"] or not (context[0] <= rom and rom + 0x18 * res["count"] <= context[1]):
        raise AssetFormatError("seg2 resource table lies outside its context row")
    reach: dict[str, dict[int, int]] = {k: {} for k in ("seg2_floor_record", "seg2_mlist_ref",
                                                        "seg2_ilist_ref", "seg2_tlist_ref")}
    headers = set()
    for d in range(res["count"]):
        record = struct.unpack_from(">5I4B", original, rom + 0x18 * d)
        header = _s2_seg(record[0], size, f"seg2 resource {d} header")
        floors = record[5]
        if not floors or header in headers:
            raise AssetFormatError(f"seg2 resource {d} header table is empty or shared")
        headers.add(header)
        claim(header, header + 0xC * floors, "seg2_floor_header")
        rows = set()
        for i in range(floors):
            _s2_record(spans, base + header + 0xC * i, "seg2_floor_header")
            rows.add(original[base + header + 0xC * i + 4])
        for kind, word in zip(reach, record[1:5]):
            seg = _s2_seg(word, size, f"seg2 resource {d} {kind}")
            reach[kind][seg] = max(reach[kind].get(seg, 0), max(rows) + 1)
    lists: dict[int, str] = {}
    extents = params["extents"]
    if set(extents) != set(reach):
        raise AssetFormatError("seg2 extents do not name the four floor table kinds")
    for kind, rows in extents.items():
        given = {seg: count for seg, count in rows}
        if len(given) != len(rows) or set(given) != set(reach[kind]):
            raise AssetFormatError(f"seg2 {kind} extents do not name exactly the referenced tables")
        for seg, count in given.items():
            if type(count) is not int or count < reach[kind][seg]:
                raise AssetFormatError(f"seg2 {kind} table {seg:#x} is shorter than its referenced rows")
            stride = sum(w for _, w in _S2_LAYOUTS[kind])
            claim(seg, seg + count * stride, kind)
            for i in range(count):
                _s2_record(spans, base + seg + i * stride, kind)
                if kind != "seg2_floor_record":
                    word = struct.unpack_from(">I", original, base + seg + 4 * i)[0]
                    _s2_list(lists, _s2_seg(word, size, f"seg2 {kind} {seg:#x}[{i}]"), _S2_REF_LIST[kind])
    for t in tables:
        if t["struct"] == "seg2_ilist_ref":
            for i in range(t["count"]):
                word = struct.unpack_from(">I", original, base + t["seg"] + 4 * i)[0]
                _s2_list(lists, _s2_seg(word, size, f"seg2 ilist ref {t['seg']:#x}[{i}]"), "ilist")

    # Floor record weight indices stay inside their weight tables.
    for member, table_seg in params["weight_indices"]:
        offset = int(member.split("_")[1], 16)
        limit_count = by_seg[table_seg]["count"]
        for seg, count in extents["seg2_floor_record"]:
            for i in range(count):
                if original[base + seg + 0x26 * i + offset] >= limit_count:
                    raise AssetFormatError(f"seg2 floor record {seg:#x}[{i}] {member} exceeds table {table_seg:#x}")

    # An mlist entry {id, level} names a kind of the kind range table (id - bias, the bias of
    # the pinned kind check) and a level inside that kind's record range.
    kinds = params["mlist_kinds"]
    kind_table = by_seg[kinds["table"]]
    bias = -_s2_imm(original, kinds["bias"], 0x09, "seg2 kind bias", from_zero=False)
    kind_counts = [original[base + kind_table["seg"] + 4 * i + 2] for i in range(kind_table["count"])]

    # Lists (a list reached from several entries is parsed once; an ilist adds its wlists).
    pending = sorted(lists.items())
    while pending:
        start, kind = pending.pop()
        name, entry, limit_count = _S2_LISTS[kind]
        at, n = start, 0
        while True:
            p = base + at
            ident = original[p]
            if kind == "mlist":
                weight = struct.unpack_from(">H", original, p + 2)[0]
                end = ident == 0
                if end != (weight == 0):
                    raise AssetFormatError(f"seg2 mlist entry {p:#x} ends one consumer walk only")
                if not end and not (0 <= ident - bias < len(kind_counts)
                                    and 1 <= original[p + 1] <= kind_counts[ident - bias]):
                    raise AssetFormatError(f"seg2 mlist entry {p:#x} names no kind level")
            elif kind == "ilist":
                pad, target = struct.unpack_from(">HI", original, p + 2)
                end = ident == 0
                if pad or end != (target == 0):
                    raise AssetFormatError(f"seg2 ilist entry {p:#x} has fill or ends one consumer walk only")
                if not end:
                    seg = _s2_seg(target, size, f"seg2 ilist entry {p:#x}")
                    _s2_list(lists, seg, "wlist")
                    # A shared wlist is parsed once; an already claimed start must be that wlist, never a
                    # table or another object (r2, SolReviewSeg2Bank F1).
                    if seg in objects:
                        if objects[seg][1] != "seg2_wlist_entry":
                            raise AssetFormatError(f"seg2 ilist entry {p:#x} wlist {seg:#x} is a {objects[seg][1]} object")
                    elif (seg, "wlist") not in pending:
                        pending.append((seg, "wlist"))
            else:
                end = ident == 0
            _s2_record(spans, p, name)
            at += entry
            n += 1
            if end:
                break
            if n == limit_count:
                raise AssetFormatError(f"seg2 {kind} at {start:#x} has no terminator in {limit_count} entries")
        claim(start, at, name)

    # Every object lies inside one sub-range; the rest of each sub-range is short zero fill
    # ending at 4- or 16-byte alignment. Nothing is typed in the parent's remainders.
    order = sorted((start, end, kind) for start, (end, kind) in objects.items())
    cursor_range = 0
    for low, high in ranges:
        low, high = low - base, high - base
        cursor = low
        while cursor_range < len(order) and order[cursor_range][0] < high:
            start, end, kind = order[cursor_range]
            if start < cursor or end > high:
                raise AssetFormatError(f"seg2 {kind} at {start:#x} overlaps or leaves its sub-range")
            if start > cursor:
                _s2_fill(original, base, cursor, start, spans)
            cursor = end
            cursor_range += 1
        if cursor < high:
            _s2_fill(original, base, cursor, high, spans)
    if cursor_range != len(order) or (order and order[0][0] < ranges[0][0] - base):
        raise AssetFormatError("seg2 object lies outside the reviewed sub-ranges")
    return spans


def _s2_fill(original: bytes, base: int, start: int, following: int, spans: list) -> None:
    gap = following - start
    if any(original[base + start:base + following]) or not (
            (following % 4 == 0 and gap < 4) or (following % 16 == 0 and gap < 16)):
        raise AssetFormatError(f"seg2 bytes {start:#x}..{following:#x} are not alignment fill")
    spans.append(Span(base + start, base + following, PAD, base + start))


def _s2_list(lists: dict, seg: int, kind: str) -> None:
    """Record that a list of `kind` starts at seg; a start reached as two kinds is an error."""
    if lists.setdefault(seg, kind) != kind:
        raise AssetFormatError(f"seg2 list at {seg:#x} is reached as {lists[seg]} and {kind}")


PARSERS["seg2_game_data"] = parse_seg2_game_data
REQUIRED_ZERO = REQUIRED_ZERO | _S2_ZERO

# ---------------------------------------------------------------------------- T5 save streams (opus-06)

T5_SIZE = 0x2800             # D_801541E4[2] (ROM 0x12722C), the kind-2 record capacity (func_80044308)
T5_POSITIONAL = 0x27F8       # func_800CB0C0 / func_800CB050: value and check words at 0x27F8 / 0x27FC
T5_VRAM = 0x8002CFC0         # main_14400: VRAM - ROM
T5_CHECKSUM_SEED = 0x4368758A  # func_800CA088
T5_POOL_CAPACITY = (200, 35, 60, 10, 3, 3, 3)   # func_800B06E0: func_800AF890(pool, items, bits, capacity)
T5_PARTNER_BACKUP = 0x80159250  # D_80159250: five {data, size, aux_data, aux_size} (func_800EF868)
T5_COMMAND_SIZES = 0x80151A3C   # D_80151A3C: 63 command sizes (0xFF = length byte + payload)
T5_COMMANDS_WITHOUT_CLASS = frozenset({0x36, 0x3D})  # func_80093E8C default: error
T5_ITEM_KIND_BASES = 0x8015374C  # D_8015374C, searched by func_800AC1AC
T5_ITEM_FACTORIES = 0x80153860   # D_80153860[kind][variant] (func_800AC244)
T5_ITEM_SPECIAL = {3: 0x8010EA20, 4: 0x8010CC70, 6: 0x80113520, 10: 0x8010DE30, 15: 0x8010DD70}
# Item id -> (factory or special constructor, code site forming the class vtable, vtable, load method).
# func_800B0164 calls the object's load at vtable +0x2C; the parser checks the vtable word and the
# code immediate in the original. Only ids the six recordings contain are listed: any other id fails.
T5_ITEMS = {
    1: (0x80117318, 0xEA33C, 0x8015DB80, "item"), 2: (0x801174C8, 0xEA4EC, 0x8015DBD8, "item"),
    7: (0x80117EA8, 0xEAECC, 0x8015DD90, "item"), 14: (0x80118718, 0xEB73C, 0x8015DFF8, "item"),
    15: (0x80118808, 0xEB82C, 0x8015E050, "item"), 17: (0x80118C08, 0xEBC2C, 0x8015E100, "item"),
    19: (0x80118DE8, 0xEBE0C, 0x8015E1B0, "item"), 21: (0x80119648, 0xEC66C, 0x8015E360, "item"),
    25: (0x8011A208, 0xED22C, 0x8015E4B0, "scroll"), 31: (0x8011AD48, 0xEDD6C, 0x8015E690, "scroll"),
    34: (0x8011B598, 0xEE5BC, 0x8015E7F0, "scroll"), 38: (0x8011BE58, 0xEEE7C, 0x8015E930, "scroll"),
    40: (0x8011C178, 0xEF19C, 0x8015E9D0, "scroll"), 44: (0x8011C7C8, 0xEF7EC, 0x8015EB10, "scroll"),
    46: (0x8011CC08, 0xEFC2C, 0x8015EBB0, "scroll"), 50: (0x8010EA20, 0xE1A84, 0x8015D518, "weapon"),
    51: (0x8010EA20, 0xE1A84, 0x8015D518, "weapon"), 52: (0x8010EA20, 0xE1A84, 0x8015D518, "weapon"),
    82: (0x8010EA20, 0xE1A84, 0x8015D518, "weapon"),
    91: (0x8010CC70, 0xDFCD4, 0x8015D1E0, "equipment"),
    92: (0x8010CC70, 0xDFCD4, 0x8015D1E0, "equipment"),
    94: (0x8010CC70, 0xDFCD4, 0x8015D1E0, "equipment"),
    118: (0x8011D378, 0xF039C, 0x80148F90, "arrow"), 119: (0x8011D3D8, 0xF03FC, 0x80148FD0, "arrow"),
    120: (0x8011D438, 0xF045C, 0x80149010, "arrow"), 161: (0x8011FE68, 0xF2E8C, 0x8015F578, "item"),
    162: (0x8011FF28, 0xF2F4C, 0x8015F5C8, "item"), 163: (0x8011FFE8, 0xF300C, 0x8015F618, "item"),
    166: (0x80120468, 0xF348C, 0x8015F708, "pot"), 168: (0x80120948, 0xF396C, 0x8015F7A8, "pot"),
    169: (0x80120A28, 0xF3A4C, 0x8015F7F8, "pot"), 201: (0x80123468, 0xF648C, 0x8015FBC0, "lot"),
    202: (0x80123630, 0xF664C, 0x8015FC10, "lot"), 233: (0x80126CC0, 0xF9D2C, 0x80160520, "material"),
    235: (0x80127220, 0xFA28C, 0x801605B0, "material"),
    236: (0x80127460, 0xFA4CC, 0x801605F8, "material"),
    237: (0x80127810, 0xFA87C, 0x80160640, "material"),
}
T5_LOADERS = {   # load method -> (VRAM, base load, section, reads)
    "item": (0x800AF174, None, b"Item", ((1, "data"),)),
    "equipment": (0x8010C420, "item", b"Equipment", ((0x14, "record"),)),
    "weapon": (0x8010EFC4, "equipment", b"Weapon", ((2, "record"),)),
    "scroll": (0x8011340C, "item", b"Scroll", ((1, "value"),)),
    "lot": (0x8010CBD4, "item", b"Lot", ((2, "value"),)),
    "arrow": (0x80112710, "lot", b"Arrow", ((8, "record"),)),
    "material": (0x80112C44, "item", b"Material", ((4, "value"),)),
    "pot": (0x80115370, "item", b"Pot", ()),
    "creature": (0x800A7CAC, None, b"Creature", ((0xC, "record"),)),
    "life": (0x800E032C, "creature", b"Life", ((0x1E, "record"),)),
    "adventurer": (0x800E9614, "life", b"Adventurer", ((8, "record"),)),
    "player": (0x800ECBC4, "adventurer", b"Player", ((0x34, "block84"),)),
    "partner": (0x800EE9C0, "adventurer", b"ES_Creature", ((0x18, "save"),)),
    "partner_items": (0x8010A1C4, "partner", None, ()),
}
# CrturTbl kinds 0x18..0x1C: D_8015CC4C[kind - 0x18](flags) (func_800A85A0); vtable at object +0x24.
T5_CREATURES = {0x1A: (0x8010A07C, 0xDCFD0, 0x8015C8A0, "partner_items"),
                0x1B: (0x8010AC28, 0xDD848, 0x8015C9F0, "partner")}
T5_PLAYER = (0x800EB120, 0xBE17C, 0x80159008, "player")   # D_801476B8: func_800EB120 installs D_80159008


def _t5_crc(name: bytes) -> int:
    """func_800CA584 with func_800CA560: the 16-bit section tag."""
    def step(value: int) -> int:
        return ((value << 7) | (((value >> 6) ^ (value >> 13)) & 0x7F)) & 0xFFFF
    value = 9
    for byte in name:
        value = step((value + byte) & 0xFFFF)
    for _ in range(3):
        value = step(value)
    return value


def _t5_mix(value: int) -> int:
    """func_800CA0EC / func_800C55B0 step."""
    return ((value << 8) & 0xFFFFFFFF) | (((value >> 10) ^ (value >> 23)) & 0xFF)


def _t5_immediate(original: bytes, site: int) -> int:
    """`addiu rX, rY, lo` at site with its reaching `lui rY` (as rommap.immediate_at)."""
    use = _u32(original, site)
    base, low = (use >> 21) & 31, use & 0xFFFF
    if use >> 26 != 0x09:
        raise AssetFormatError(f"T5 class site {site:#x} is not addiu")
    for back in range(site - 4, max(site - 36, 0), -4):
        word = _u32(original, back)
        if word >> 26 == 0x0F and (word >> 16) & 31 == base:
            return ((word & 0xFFFF) << 16) + (low - 0x10000 if low & 0x8000 else low) & 0xFFFFFFFF
    raise AssetFormatError(f"T5 class site {site:#x} has no reaching lui")


class _T5Stream:
    """Sequential reader over [8, end) with the ROM stream's bound (func_800444A4 refuses reads past end)."""

    def __init__(self, original: bytes, start: int, end: int, spans: list[Span]) -> None:
        self.original, self.start, self.pos, self.end, self.spans = original, start, start + 8, start + end, spans

    def read(self, size: int, field: str, base: int | None = None) -> bytes:
        if size < 0 or self.pos + size > self.end:
            raise AssetFormatError(f"T5 read of {size} at {self.pos:#x} passes the logical end")
        at = self.pos
        if size:
            self.spans.append(Span(at, at + size, field, at if base is None else base))
        self.pos += size
        return self.original[at:at + size]

    def section(self, name: bytes, struct_name: str) -> int:
        """func_800CA4E8: a u16 tag that must equal the section name's checksum."""
        at = self.pos
        if self.pos + 2 > self.end or (self.original[at] << 8 | self.original[at + 1]) != _t5_crc(name):
            raise AssetFormatError(f"T5 section {name.decode()} expected at {at:#x}")
        self.read(2, f"{struct_name}.tag", at)
        return at


def _t5_load(stream: _T5Stream, loader: str) -> None:
    """A class load method: its base load, then its own section and reads (sources in T5_LOADERS)."""
    vram, base, section, reads = T5_LOADERS[loader]
    if loader == "partner_items":                       # func_8010A1C4: partner load, then the ItemSet
        _t5_load(stream, "partner")
        _t5_itemset(stream)
        return
    if loader == "pot":                                 # func_80115370
        _t5_load(stream, "item")
        stream.section(b"Pot", "pot")
        tag = stream.section(b"PotContents", "potcontents")   # func_800D00E0
        _t5_itemset(stream)
        stream.read(1, "potcontents.owner_id", tag)
        stream.read(1, "pot.field_28", tag)
        stream.read(1, "pot.field_29", tag)
        return
    if base is not None:
        _t5_load(stream, base)
    tag = stream.section(section, loader)
    for size, member in reads:
        stream.read(size, f"{loader}.{member}", tag)
    if loader == "player":                              # func_800ECBC4 ends with its ItemSet (+0xCC)
        _t5_itemset(stream)


def _t5_itemset(stream: _T5Stream) -> None:
    """func_800CE9C4 "ItemSet": count, two bytes, then `count` index bytes."""
    tag = stream.section(b"ItemSet", "itemset")
    count = stream.read(1, "itemset.count", tag)[0]
    stream.read(1, "itemset.field_0d", tag)
    stream.read(1, "itemset.field_0e", tag)
    stream.read(count, "itemset.indices", tag)


def _t5_class(original: bytes, key: int, table: Mapping[int, tuple], label: str, slot_table: int | None) -> str:
    """Resolve and check one class: factory slot (if tabled), the vtable-forming immediate and the
    vtable's +0x2C load method word."""
    if key not in table:
        raise AssetFormatError(f"T5 {label} {key:#x} has no derived class")
    factory, site, vtable, loader = table[key]
    if slot_table is not None and _u32(original, slot_table) != factory:
        raise AssetFormatError(f"T5 {label} {key:#x} factory slot changed")
    if _t5_immediate(original, site) != vtable or _u32(original, vtable - T5_VRAM + 0x2C) != T5_LOADERS[loader][0]:
        raise AssetFormatError(f"T5 {label} {key:#x} class vtable or load method changed")
    return loader


def _t5_item(original: bytes, stream: _T5Stream) -> None:
    item = stream.pos
    item_id = stream.read(1, "item_slot.id", item)[0]
    bases = original[T5_ITEM_KIND_BASES - T5_VRAM:T5_ITEM_KIND_BASES - T5_VRAM + 22]
    lo, hi = 1, 0x15                                   # func_800AC1AC
    while hi - lo >= 2:
        mid = (hi + lo) // 2
        lo, hi = (lo, mid) if item_id < bases[mid] else (mid, hi)
    kind = lo
    if kind in T5_ITEM_SPECIAL:
        slot = None
        if item_id in T5_ITEMS and T5_ITEMS[item_id][0] != T5_ITEM_SPECIAL[kind]:
            raise AssetFormatError(f"T5 item {item_id} is not its kind's constructor")
    else:
        slot = _u32(original, T5_ITEM_FACTORIES - T5_VRAM + 4 * kind) - T5_VRAM + 4 * (item_id - bases[kind])
    _t5_load(stream, _t5_class(original, item_id, T5_ITEMS, "item", slot))


def _t5_pool(original: bytes, stream: _T5Stream, capacity: int) -> None:
    """func_800B0164 "ItemTbl": occupancy bits ((capacity + 7) / 8 bytes), then each live item."""
    tag = stream.section(b"ItemTbl", "itemtbl")
    bits = stream.read((capacity + 7) // 8, "itemtbl.occupancy", tag)
    for index in range(capacity):
        if bits[index >> 3] & (1 << (index & 7)):
            _t5_item(original, stream)


def _t5_state(original: bytes, stream: _T5Stream) -> None:
    """RecStat (func_800CADFC) and All (func_800C993C) with world (func_800C9AF8) and moment
    (func_800C9CC4); returns at the first command record."""
    tag = stream.section(b"RecStat", "recstat")
    record = stream.read(0x18, "recstat.record", tag)
    tag = stream.section(b"All", "all")
    resumed = stream.read(1, "all.resumed", tag)[0]          # D_801476C3, tested by func_800C9810
    stream.read(12, "all.rng_state", tag)                   # func_800C5BE8: D_80147620 size 12 (func_800C5DBC)
    world = stream.section(b"world", "world")
    tag = stream.section(b"Castle", "castle")                # func_800D1CC4
    for size, member in ((0x14, "field_00"), (0x14, "field_14"), (0xC, "field_bb")):
        stream.read(size, f"castle.{member}", tag)
    for capacity in T5_POOL_CAPACITY[1:]:                   # func_800B02F4: pools 1..6
        _t5_pool(original, stream, capacity)
    tag = stream.section(b"EventFlg", "eventflg")            # func_801EF5F8_141518
    for size, member in ((0x40, "flags_40"), (0x20, "flags_20"), (0x10, "flags_10"), (4, "word_a"), (4, "word_b")):
        stream.read(size, f"eventflg.{member}", tag)
    tag = stream.section(b"Read", "read")                    # func_80112E30
    stream.read(4, "read.flags", tag)
    tag = stream.section(b"NamTbl", "namtbl")                # func_800B0DCC
    for size, member in ((5, "field_5"), (0xA0, "names"), (1, "count"), (0x28, "field_28")):
        stream.read(size, f"namtbl.{member}", tag)
    tag = stream.section(b"Zoo", "zoo")                      # func_800D83F0
    stream.read(0x8E, "zoo.packed", tag)
    stream.read(9, "zoo.field_9", tag)
    tag = stream.section(b"Memento", "memento")              # func_80122820
    stream.read(9, "memento.record", tag)
    tag = stream.section(b"Museum", "museum")                # func_800A17E0
    stream.read(5, "museum.head", tag)
    stream.read(4, "museum.tail", tag)
    stream.read(4, "world.value_80142930", world)           # func_800A1A34
    stream.read(1, "world.flag_80138c40", world)            # func_8004645C
    if not (record[7] & 2):                                 # func_800C993C: moment also needs D_80140160[6]
        raise AssetFormatError("T5 record flag leaves the moment section to runtime state")
    stream.section(b"moment", "moment")
    tag = stream.section(b"Floor", "floor")                  # func_800A9608
    stream.read(11, "floor.selection_save", tag)
    for member in ("mode", "x", "y", "flags", "status"):
        stream.read(1, f"floor.selection_{member}", tag)
    tag = stream.section(b"Ident", "ident")                  # func_800AEE58
    for size, member in ((0xC, "rng_record"), (1, "byte_8014303c"), (0x20, "table_80143064"), (0x20, "table_80143044")):
        stream.read(size, f"ident.{member}", tag)
    tag = stream.section(b"ItemName", "itemname")            # func_800AF030
    for size in (0x16, 0x1B, 0x13, 0x13, 0xD, 0x15):
        stream.read(size, f"itemname.block_{size:02x}", tag)
    _t5_pool(original, stream, T5_POOL_CAPACITY[0])          # func_800B0164(&D_80143094)
    _t5_load(stream, _t5_class(original, 0, {0: T5_PLAYER}, "player", None))   # D_801476B8 load
    tag = stream.section(b"CrturTbl", "crturtbl")            # func_800A8E40
    bits = stream.read(4, "crturtbl.used", tag)
    for index in range(29):
        if bits[index >> 3] & (1 << (index & 7)):
            entry = stream.pos
            kind = stream.read(1, "crturtbl_entry.kind", entry)[0]
            stream.read(1, "crturtbl_entry.flags", entry)
            if not 0x18 <= kind < 0x1D:
                raise AssetFormatError(f"T5 creature kind {kind:#x} has no derived class")
            slot = 0x8015CC4C - T5_VRAM + 4 * (kind - 0x18)          # func_800A83D0 -> func_800A85A0
            _t5_load(stream, _t5_class(original, kind, T5_CREATURES, "creature", slot))
    tag = stream.section(b"EventFlg", "eventflg2")           # func_801F2A70_144990
    stream.read(2, "eventflg2.word", tag)
    tag = stream.section(b"ItemTbl", "itemrefs")             # func_800B6434
    count = struct.unpack(">h", stream.read(2, "itemrefs.count", tag))[0]
    for _ in range(max(count, 0)):
        entry = stream.pos
        stream.read(8, "itemrefs_entry.ref", entry)
        stream.read(1, "itemrefs_entry.value", entry)
    tag = stream.section(b"PtnerBkup", "ptnerbkup")          # func_800EF868 with D_80159250
    for index in range(5):
        _, size, _, aux = struct.unpack_from(">IiIi", original, T5_PARTNER_BACKUP - T5_VRAM + 16 * index)
        stream.read(size, f"ptnerbkup.data_{index}", tag)
        stream.read(aux, f"ptnerbkup.aux_{index}", tag)
    tag = stream.section(b"PStatus", "pstatus")              # func_800B77B4
    stream.read(1, "pstatus.value", tag)
    if resumed:                                             # func_800C9810: Eye and Audio follow
        raise AssetFormatError("T5 resumed records (Eye/Audio sections) have no derived grammar here")


def _t5_commands(original: bytes, stream: _T5Stream) -> int:
    """func_80093E8C: records until the stream end; a byte with bit 7 repeats the previous command
    (count in the low bits), otherwise an opcode below 0x3F with D_80151A3C[opcode] - 1 payload bytes
    (or 0xFF: a length byte and that many bytes)."""
    sizes = original[T5_COMMAND_SIZES - T5_VRAM:T5_COMMAND_SIZES - T5_VRAM + 0x3F]
    count, previous = 0, None
    while stream.pos < stream.end:
        record = stream.pos
        opcode = stream.read(1, "command.opcode", record)[0]
        count += 1
        if opcode & 0x80:
            if previous is None:
                raise AssetFormatError(f"T5 repeat at {record:#x} has no previous command")
            continue
        if opcode >= 0x3F or opcode in T5_COMMANDS_WITHOUT_CLASS:
            raise AssetFormatError(f"T5 command {opcode:#x} at {record:#x} has no class")
        size = sizes[opcode]
        if size == 0xFF:
            length = stream.read(1, "command.length", record)[0]
            stream.read(length, "command.payload", record)
        elif size >= 2:
            stream.read(size - 1, "command.payload", record)
        previous = opcode
    return count


def _t5_walk(original: bytes, start: int) -> tuple[list[Span], Interval]:
    """Type the whole stream of the T5 buffer at `start` and its value pair; return the spans and the
    PtnerBkup section interval (tag through the last partner-backup record)."""
    logical, checksum = _u32(original, start), _u32(original, start + 4)
    if not 8 < logical <= T5_POSITIONAL:
        raise AssetFormatError(f"T5 buffer at {start:#x} end {logical:#x} is outside the stream area")
    value = T5_CHECKSUM_SEED
    for byte in original[start + 8:start + logical]:
        value = _t5_mix(value) ^ byte
    if value != checksum:
        raise AssetFormatError(f"T5 buffer at {start:#x} checksum differs from func_800CA118")
    spans = [Span(start, start + 4, "rec_end.value", start),
             Span(start + 4, start + 8, "rec_checksum.value", start + 4)]
    stream = _T5Stream(original, start, logical, spans)
    _t5_state(original, stream)
    _t5_commands(original, stream)
    end = start + T5_SIZE
    gold, check = _u32(original, end - 8), _u32(original, end - 4)
    mixed = gold
    for _ in range(4):
        mixed = _t5_mix(mixed)
    if mixed != check:
        raise AssetFormatError(f"T5 buffer at {start:#x} value check differs from func_800CB02C")
    spans.append(Span(end - 8, end - 4, "rec_value.value", end - 8))
    spans.append(Span(end - 4, end, "rec_value_check.value", end - 4))
    backup = (next(s.start for s in spans if s.field == "ptnerbkup.tag"),
              next(s.start for s in spans if s.field == "pstatus.tag"))
    return spans, backup


def t5_pieces(original: bytes, start: int) -> tuple[Interval, Interval, Interval]:
    """The closable parts of the T5 buffer at `start`: header..PtnerBkup, PStatus..logical end and the
    value pair. Open: the PtnerBkup section and the tail [end, 0x27F8)."""
    _, (backup, status) = _t5_walk(original, start)
    return (start, backup), (status, start + _u32(original, start)), (start + T5_POSITIONAL, start + T5_SIZE)


def parse_t5_save_stream(original: bytes, ranges: tuple[Interval, ...], params: Mapping[str, Any]) -> list[Span]:
    """T5 demo records: one kind-2 record stream per 0x2800-byte buffer (func_80044308, func_800CA76C).

    The stream (`_t5_walk`): u32 end and u32 checksum (read by func_80044580 at offsets 0 and 4; the
    checksum is func_800CA118 over [8, end)), the record and game state (`_t5_state`) and command records
    to `end` (`_t5_commands`); the value/check pair at 0x27F8 (func_800CB0C0 reads, func_800CB050
    writes; check = func_800CB02C(value)).

    Each buffer is a located map row (params["within"]) and closes exactly its three `t5_pieces`.
    Two parts stay located:
    - [end, 0x27F8): no consumer reads it (func_800444A4 refuses sequential reads past `end`; the
      positional reads are 0, 4, 0x27F8 and 0x27FC), but no content writer is pinned for its bytes
      (func_800CA2C0 only rewrites the end and checksum words; func_80043900 copies whole slots);
    - the PtnerBkup section (tag and the five D_80159250 records, 1,474 bytes): typed, but its records
      are empty partner backups whose zero bytes the format does not require (MIN_EVIDENCE_SHARE).
    """
    within = params.get("within")
    if (set(params) != {"context", "within"} or params["context"] or not isinstance(within, tuple)
            or len(within) != len(ranges) or None in within):
        raise AssetFormatError("T5 save streams close only sub-ranges of their buffer rows (no context)")
    parents: dict[Interval, list[Interval]] = {}
    for interval, parent in zip(ranges, within):
        parents.setdefault(tuple(parent), []).append(interval)
    spans: list[Span] = []
    for (start, end), pieces in parents.items():
        if end - start != T5_SIZE:
            raise AssetFormatError(f"T5 buffer at {start:#x} is not one record capacity")
        typed, (backup, status) = _t5_walk(original, start)
        expected = ((start, backup), (status, start + _u32(original, start)), (start + T5_POSITIONAL, end))
        if sorted(pieces) != list(expected):
            raise AssetFormatError(f"T5 buffer at {start:#x} closes other than its three typed pieces (the tail "
                                   "is unread with no pinned content writer; PtnerBkup is zero-dominated)")
        spans.extend(s for s in typed if not backup <= s.start < status)
    return spans


PARSERS["t5_save_stream"] = parse_t5_save_stream
