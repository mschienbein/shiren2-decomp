"""Compiler-generated g++ 2.x vtables in CPU data: classified, never credited.

User ruling `scratch/omp/rulings/VTABLES-CLASSIFY-NO-CREDIT-20261014.md`: docs/OMP_WORKFLOW.md's
rule stands (compiler-generated vtables earn no C/C++ source and no owned-data credit). A vtable
closes as `compiler_generated_vtable` (research status classified_binary) only when its pinned row
in `config/rom-evidence/gxx-vtables.json` replays here against the original, the frozen CPU
catalogue and the base partition's cpu_data rows (the unsplit gaps; rommap's witness split then
carves the replayed intervals out of them):

* Layout, parsed positively per entry. The pinned gxx281pm cc1plus emits vtables without thunks:
  an 8-aligned `.rodata` array of 8-byte entries {s16 delta; s16 index (always 0); u32 pfn}. Entry 0
  is the -fno-rtti header {-(subobject offset), 0, 0}; a slot's this-adjustment lies between the
  header delta and 0 and is a word multiple.
* Every slot pfn, `__pure_virtual` included, is the exact start of a catalogued CPU function that
  is co-resident with the table: its own image or an always-resident image (overlay pointers never
  resolve into the other overlay).
* Start: a proven vptr install into a proven object member. Paths and register values come only
  from the shared engine `tools/mipsflow.py` (opus-09); a function it cannot model exactly
  (FlowUnknown: an indirect jump that is not a bounded GCC switch, `jalr` with rd = 0, a transfer
  in a delay slot, ...) proves nothing. An install is an entry-reachable `sw rt, off(base)` node
  (an instruction run as an entered delay slot is its own node) in a function that names the table,
  where every definition of rt reaching the store computes the table's address (mipsflow.value:
  lui/addiu/ori through moves on every path; a callee's clobber is a RETURN definition, never a
  constant), and the destination is a proven member:
    - receiver: base is, on every path, a0 as the function received it plus a constant
      (mipsflow.source), i.e. the g++ `this`; the member is that constant plus off, >= 0, a word
      multiple;
    - bound: any other base (a static address, a stack slot, a pointer argument, a call's result)
      only through a call of the same function whose receiver is that same object: a jal to a
      co-resident catalogued function H, entry-reachable, whose a0 as H receives it plus a member N
      equals base plus off (a static base: both constant, inside KSEG0 RAM; otherwise
      mipsflow.same, with the call and the store on one path, one dominating the other), where H
      itself has a proven receiver install at member N of a pinned table. H's own install writing a
      vtable at member N is the member identity; a register origin alone never is.
  Every proven install [store, context, function, member, origin, binding] is pinned and replayed
  exactly; a table without one stays open.
* Extent: the start is 8-aligned. The end is exactly the first of the next referenced address
  (code materialization or aligned data word), the end of the enclosing cpu_data gap, and the first
  8-byte entry that fails the slot rules; there is at least one slot and no interior header.

Zero fills between tables, unreferenced tables and every byte this parser does not type stay
cpu_data (open). Zero-only or geometry-only intervals never qualify; nothing here grants credit.
Which slot targets are matched C/C++ or classified handwritten is a generator-side gate recorded
in the evidence (pinned config/matches.json and config/handwritten.json digests), not replayed.
"""
from __future__ import annotations

import hashlib
import struct
from typing import Any, Iterable, Mapping, Sequence

import mipsflow

INPUT = "gxx-vtables.json"
SCHEMA = "shiren2-gxx-vtable-evidence-v3"
CLASS = "compiler_generated_vtable"
ENTRY = 8
RESIDENT = ("resident", "main_14400")
BOUNDS = frozenset({"reference", "gap_end", "entry"})
_GPR_IMM_WRITERS = frozenset({0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x18, 0x19, 0x1A, 0x1B,
                              0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x30, 0x34, 0x37, 0x38, 0x3C})
_ABSOLUTE = frozenset({0x09, 0x0D, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A,
                       0x2B, 0x2E, 0x31, 0x35, 0x37, 0x39, 0x3D, 0x3F})
_NO_RD = frozenset({0x08, 0x0C, 0x0D, 0x0F, 0x11, 0x13, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
                    0x30, 0x31, 0x32, 0x33, 0x34, 0x36})


class VtableError(ValueError):
    """A pinned vtable witness does not replay against the original."""


def table_id(image_id: str, start: int) -> str:
    return f"VT_{image_id}_{start:x}"


def _images(images: Iterable[Mapping[str, Any]]) -> dict[str, dict[str, int]]:
    result = {}
    for image in images:
        size = image["rom_end"] - image["rom_start"]
        result[image["image_id"]] = {"rom_start": image["rom_start"], "rom_end": image["rom_end"],
                                     "vram_start": image["vram_start"], "vram_end": image["vram_start"] + size,
                                     "delta": image["vram_start"] - image["rom_start"]}
    return result


def visible(image_id: str) -> tuple[str, ...]:
    """Images whose addresses a pointer stored in `image_id` can denote."""
    return RESIDENT if image_id in RESIDENT else (image_id, *RESIDENT)


def _owners(images: Mapping[str, Mapping[str, int]], value: int, holder: str) -> list[str]:
    """Images a value held in `holder` can address; a resident holder may name either overlay."""
    return [name for name, image in images.items()
            if image["vram_start"] <= value < image["vram_end"]
            and (name in RESIDENT or name == holder or holder in RESIDENT)]


def references(original: bytes, functions: Sequence[Mapping[str, Any]], images: Mapping[str, Mapping[str, int]],
               data_gaps: Iterable[tuple[int, int, str]]) -> dict[str, dict[int, list[tuple[str, int, str]]]]:
    """Every absolute address the original names, per image: {image: {vram: [(kind, rom site, holder)]}}.

    Code: a linear scan of each catalogued function tracking `lui` values into addiu/ori and
    loads/stores, and through an addu/daddu of a pure %hi base with an index (gcc -G 0 indexed
    access). Data: every aligned word of the cpu_data gaps whose value lies in a co-resident image.
    Missing a reference only weakens nothing: references shorten extents; they never extend them.
    """
    result: dict[str, dict[int, list[tuple[str, int, str]]]] = {name: {} for name in images}
    for function in functions:
        holder, regs = function["image_id"], {}
        for k in range(function["size"] // 4):
            site = function["rom_start"] + 4 * k
            word = struct.unpack_from(">I", original, site)[0]
            op, rs, rt, imm = word >> 26, (word >> 21) & 31, (word >> 16) & 31, word & 0xFFFF
            if op == 0x0F:
                regs[rt] = imm << 16
                continue
            if op in _ABSOLUTE and rs in regs:
                value = regs[rs] | imm if op == 0x0D else (regs[rs] + (imm - 0x10000 if imm & 0x8000 else imm)) & 0xFFFFFFFF
                for name in _owners(images, value, holder):
                    result[name].setdefault(value, []).append(("code", site, function["symbol"]))
            if op in _GPR_IMM_WRITERS:
                regs.pop(rt, None)
            elif op == 0x03:
                regs.pop(31, None)
            elif op == 0x11 and rs in (0, 1, 2):  # mfc1/dmfc1/cfc1
                regs.pop(rt, None)
            elif op == 0 and (word & 63) not in _NO_RD:
                rd, funct = (word >> 11) & 31, word & 63
                hi = [regs[r] for r in (rs, rt) if r in regs and not regs[r] & 0xFFFF]
                if funct in (0x21, 0x2D) and len(hi) == 1 and not (rs in regs and rt in regs):
                    regs[rd] = hi[0]
                else:
                    regs.pop(rd, None)
    for start, end, holder in data_gaps:
        for site in range(start + -start % 4, end - 3, 4):
            value = struct.unpack_from(">I", original, site)[0]
            for name in _owners(images, value, holder):
                result[name].setdefault(value, []).append(("data", site, holder))
    return result


def _function_starts(functions: Sequence[Mapping[str, Any]]) -> dict[str, dict[int, str]]:
    result: dict[str, dict[int, str]] = {}
    for function in functions:
        result.setdefault(function["image_id"], {})[function["vram_start"]] = function["symbol"]
    return result


# -- vptr installs (all paths and values from mipsflow, the shared engine) --------------------------
_SW, _JAL, _A0 = 0x2B, 0x03, 4
_ARGUMENTS = frozenset({5, 6, 7})
_KSEG0 = (0x80000000, 0x80800000)  # cached RAM (8 MiB with the expansion pak)
_CONTEXT = {mipsflow.ORDINARY: "ordinary", mipsflow.SLOT: "slot"}


def _u32(original: bytes, at: int) -> int:
    return struct.unpack_from(">I", original, at)[0]


def _signed(value: int) -> int:
    return value - (1 << 32) if value & 0x80000000 else value


class _Function:
    """One catalogued function's mipsflow graph (flow None: the engine refused it) and its calls."""

    def __init__(self, original: bytes, function: Mapping[str, Any], starts: Mapping[str, Mapping[int, str]]) -> None:
        self.original, self.symbol = original, function["symbol"]
        self.low, self.high = function["rom_start"], function["rom_start"] + function["size"]
        base = function["vram_start"] - self.low
        try:
            self.flow: mipsflow.Flow | None = mipsflow.Flow(original, self.low, self.high, base)
        except mipsflow.FlowUnknown:
            self.flow = None
        # (call pc, callee) of every entry-reachable jal to the start of a co-resident catalogued function
        self.calls: list[tuple[int, str]] = []
        for pc in range(self.low, self.high, 4) if self.flow else ():
            word = _u32(original, pc)
            if word >> 26 != _JAL or (pc, mipsflow.RETURN) not in self.flow.reachable:
                continue
            vram = ((pc + base + 4) & 0xF0000000) | (word & 0x3FFFFFF) << 2
            symbols = [s for s in (starts.get(name, {}).get(vram) for name in visible(function["image_id"])) if s]
            if len(symbols) == 1:
                self.calls.append((pc, symbols[0]))

    def stores(self, value: int) -> list[tuple[Any, int, int]]:
        """(node, base register, offset) of every entry-reachable `sw` node whose stored register
        holds `value` on every path (each reaching definition computes it)."""
        result = []
        for pc in range(self.low, self.high, 4) if self.flow else ():
            word = _u32(self.original, pc)
            if word >> 26 != _SW:
                continue
            for node in self.flow.at(pc):
                if self.flow.value((word >> 16) & 31, node) == value:
                    result.append((node, (word >> 21) & 31, ((word & 0xFFFF) ^ 0x8000) - 0x8000))
        return result

    def receiver(self, base: int, node: Any, offset: int) -> int | None:
        """The member written when `base` is, on every path, the incoming a0 (`this`) plus a constant."""
        reg, root, k = self.flow.source(base, node)
        if reg != _A0 or self.flow.reaching(_A0, root) != {mipsflow.ENTRY}:
            return None
        member = _signed((k + offset) & 0xFFFFFFFF)
        return member if member >= 0 and not member % 4 else None

    def origin(self, base: int, node: Any) -> str:
        """What a non-receiver base is (a report label; the binding is the proof)."""
        if self.flow.value(base, node) is not None:
            return "static"
        reg, root, _ = self.flow.source(base, node)
        defs = self.flow.reaching(reg, root)
        if defs == {mipsflow.ENTRY}:
            return "stack" if reg == mipsflow.SP else "argument" if reg in _ARGUMENTS else "value"
        if len(defs) > 1:
            return "merged"  # e.g. an argument or, when it is null, a fresh allocation
        return "call_result" if next(iter(defs))[1] == mipsflow.RETURN else "value"

    def bind(self, base: int, node: Any, offset: int, members: Mapping[str, Iterable[int]]) -> list[Any] | None:
        """[call, callee, member]: the first call (pc order, then member) whose callee receives the
        object the store writes into, with the store at its member N, and has a proven receiver
        install at N."""
        flow = self.flow
        target = flow.value(base, node)
        for pc, callee in self.calls:
            ret = (pc, mipsflow.RETURN)
            for member in sorted(members.get(callee, ())):
                if target is not None:
                    receiver = flow.value(_A0, ret)
                    bound = (receiver is not None and _KSEG0[0] <= receiver and receiver + member < _KSEG0[1]
                             and (target + offset) & 0xFFFFFFFF == receiver + member)
                else:
                    bound = (flow.same(base, node, _A0, ret, offset, member)
                             and (flow.dominates(ret, node) or flow.dominates(node, ret)))
                if bound:
                    return [pc, callee, member]
        return None


class _Prover:
    """Install proofs for tables, flow graphs cached per function."""

    def __init__(self, original: bytes, functions: Sequence[Mapping[str, Any]],
                 starts: Mapping[str, Mapping[int, str]]) -> None:
        self.original, self.starts = original, starts
        self.functions = {f["symbol"]: f for f in functions}
        self.cache: dict[str, _Function] = {}

    def function(self, symbol: str) -> _Function:
        if symbol not in self.cache:
            self.cache[symbol] = _Function(self.original, self.functions[symbol], self.starts)
        return self.cache[symbol]

    def installs(self, value: int, code: Sequence[Sequence[Any]],
                 members: Mapping[str, Iterable[int]] | None) -> list[list[Any]]:
        """Sorted proven installs of the table at VRAM `value` in the functions of its start
        references; receiver installs only when `members` is None."""
        result = []
        for symbol in sorted({symbol for _, symbol in code}):
            function = self.function(symbol)
            for node, base, offset in function.stores(value):
                member = function.receiver(base, node, offset)
                if member is not None:
                    result.append([node[0], _CONTEXT[node[1]], symbol, member, "this", None])
                    continue
                binding = None if members is None else function.bind(base, node, offset, members)
                if binding:
                    result.append([node[0], _CONTEXT[node[1]], symbol, binding[2], function.origin(base, node),
                                   binding[:2]])
        return sorted(result, key=lambda r: (r[0], r[1]))

    def prove(self, rows: Sequence[Mapping[str, Any]], vram: Mapping[str, int]) -> list[dict[str, Any]]:
        """`rows` with their installs; rows without one are dropped. Bindings may cite the receiver
        installs of any of `rows` (a dropped row has none, so one pass is the fixed point)."""
        receivers = {row["id"]: self.installs(vram[row["id"]], row["start_refs"], None) for row in rows}
        members: dict[str, set[int]] = {}
        for found in receivers.values():
            for install in found:
                members.setdefault(install[2], set()).add(install[3])
        result = []
        for row in rows:
            found = self.installs(vram[row["id"]], row["start_refs"], members)
            if found:
                result.append({**row, "installs": found})
        return result


def parse(original: bytes, image_id: str, start: int, gap_end: int, delta: int,
          starts: Mapping[str, Mapping[int, str]], next_reference: int) -> dict[str, Any]:
    """Parse one table from its header; the result is the unique maximal extent."""
    if (start + delta) % ENTRY or start + ENTRY > gap_end:
        raise VtableError(f"vtable {start:#x} is not an 8-aligned entry array inside its cpu_data gap")
    header, index, pfn = struct.unpack_from(">hhI", original, start)
    if index or pfn or header > 0 or header % 4:
        raise VtableError(f"vtable {start:#x} header is not the -fno-rtti entry {{-offset, 0, 0}}")
    limit = min(gap_end, next_reference)
    cursor, slots, bound = start + ENTRY, [], None
    while bound is None:
        if cursor + ENTRY > limit:
            bound = "gap_end" if limit == gap_end and next_reference >= gap_end else "reference"
            break
        adjust, index, pfn = struct.unpack_from(">hhI", original, cursor)
        symbols = [starts.get(name, {}).get(pfn) for name in visible(image_id)]
        symbols = [s for s in symbols if s]
        if index or not header <= adjust <= 0 or adjust % 4 or len(symbols) != 1:
            bound = "entry"
            break
        slots.append([adjust, symbols[0]])
        cursor += ENTRY
    if not slots:
        raise VtableError(f"vtable {start:#x} has no slot that resolves to a catalogued function")
    return {"end": cursor, "header_delta": header, "slots": slots, "bound": bound}


def _gap_of(gaps: Sequence[tuple[int, int, str]], image_id: str, start: int) -> tuple[int, int]:
    for a, b, name in gaps:
        if name == image_id and a <= start < b:
            return a, b
    raise VtableError(f"vtable {start:#x} is not inside a cpu_data gap of {image_id}")


def candidates(original: bytes, catalogue: Mapping[str, Any], base_rows: Iterable[Sequence[Any]]) -> list[dict[str, Any]]:
    """Every table this parser types at a code-materialized label, without install proofs."""
    functions = catalogue["functions"]
    images = _images(catalogue["images"])
    gaps = data_gaps(base_rows, images)
    refs = references(original, functions, images, gaps)
    starts = _function_starts(functions)
    rows = []
    for gap_start, gap_end, image_id in gaps:
        delta = images[image_id]["delta"]
        labels = sorted(v - delta for v in refs[image_id] if gap_start <= v - delta < gap_end)
        for i, start in enumerate(labels):
            code = sorted([site, symbol] for kind, site, symbol in refs[image_id][start + delta] if kind == "code")
            if not code:
                continue
            following = labels[i + 1] if i + 1 < len(labels) else gap_end
            try:
                parsed = parse(original, image_id, start, gap_end, delta, starts, following)
            except VtableError:
                continue
            rows.append({"id": table_id(image_id, start), "image_id": image_id, "start": start, **parsed,
                         "start_refs": code, "sha256": hashlib.sha256(original[start:parsed["end"]]).hexdigest()})
    return rows


def prove(original: bytes, catalogue: Mapping[str, Any], rows: Sequence[Mapping[str, Any]]) -> list[dict[str, Any]]:
    """`rows` (candidates the generator admits) with their proven installs; rows without one are
    dropped (evidence generation; replay never trusts it)."""
    functions = catalogue["functions"]
    images = _images(catalogue["images"])
    vram = {row["id"]: row["start"] + images[row["image_id"]]["delta"] for row in rows}
    proven = _Prover(original, functions, _function_starts(functions)).prove(rows, vram)
    fields = ("id", "image_id", "start", "end", "header_delta", "slots", "bound", "start_refs", "installs", "sha256")
    return [{key: row[key] for key in fields} for row in proven]


def scan(original: bytes, catalogue: Mapping[str, Any], base_rows: Iterable[Sequence[Any]]) -> list[dict[str, Any]]:
    """Every typed table whose start a proven install anchors (no generator policy applied)."""
    return prove(original, catalogue, candidates(original, catalogue, base_rows))


def data_gaps(base_rows: Iterable[Sequence[Any]], images: Mapping[str, Any]) -> list[tuple[int, int, str]]:
    """The base partition's cpu_data rows (ref = owning image), before any witness split."""
    gaps = [(row[0], row[1], row[3]) for row in base_rows if row[2] == "cpu_data"]
    if any(ref not in images for _, _, ref in gaps):
        raise VtableError("a cpu_data base row does not name its image")
    return gaps


def replay(original: bytes, document: object, catalogue: Mapping[str, Any], base_rows: Iterable[Sequence[Any]],
           target_sha256: str) -> dict[tuple[int, int], str]:
    """Replay every pinned table against the base rows; {exact (start, end): table id}. Fails closed."""
    if (not isinstance(document, dict) or document.get("schema") != SCHEMA
            or document.get("target_sha256") != target_sha256 or not isinstance(document.get("vtables"), list)):
        raise VtableError("vtable evidence does not describe the fixed target")
    functions = catalogue["functions"]
    images = _images(catalogue["images"])
    gaps = data_gaps(base_rows, images)
    refs = references(original, functions, images, gaps)
    starts = _function_starts(functions)
    sites = {f["symbol"]: (f["rom_start"], f["rom_start"] + f["size"]) for f in functions}
    result: dict[tuple[int, int], str] = {}
    previous_end = {name: -1 for name in images}
    fields = {"id", "image_id", "start", "end", "header_delta", "slots", "bound", "start_refs", "installs", "sha256"}
    checked = []
    for row in document["vtables"]:
        if not isinstance(row, dict) or set(row) != fields or row["image_id"] not in images:
            raise VtableError("vtable evidence row does not match the schema")
        image_id, start, end = row["image_id"], row["start"], row["end"]
        if type(start) is not int or type(end) is not int or row["id"] != table_id(image_id, start):
            raise VtableError(f"vtable evidence identity is invalid: {row.get('id')!r}")
        if start < previous_end[image_id]:
            raise VtableError(f"vtable evidence rows overlap or are unordered at {start:#x}")
        _, gap_end = _gap_of(gaps, image_id, start)
        delta = images[image_id]["delta"]
        named = refs[image_id]
        code = sorted([site, symbol] for kind, site, symbol in named.get(start + delta, ()) if kind == "code")
        if not code or code != row["start_refs"]:
            raise VtableError(f"vtable {start:#x} start is not the pinned code-materialized address")
        for site, symbol in code:
            low, high = sites[symbol]
            if not low <= site < high:
                raise VtableError(f"vtable {start:#x} reference site lies outside {symbol}")
        following = min((v - delta for v in named if start < v - delta < gap_end), default=gap_end)
        parsed = parse(original, image_id, start, gap_end, delta, starts, following)
        if (parsed["end"] != end or parsed["header_delta"] != row["header_delta"] or parsed["slots"] != row["slots"]
                or parsed["bound"] != row["bound"] or row["bound"] not in BOUNDS):
            raise VtableError(f"vtable {start:#x} extent/layout disagrees with the original "
                              f"(parsed end {parsed['end']:#x}, pinned {end:#x})")
        if any(start < v - delta < end for v in named):
            raise VtableError(f"vtable {start:#x} overruns a neighbouring labelled object")
        if hashlib.sha256(original[start:end]).hexdigest() != row["sha256"]:
            raise VtableError(f"vtable {start:#x} bytes changed")
        checked.append(row)
        previous_end[image_id] = end
    # Installs last: a binding may cite any pinned table's receiver install.
    vram = {row["id"]: row["start"] + images[row["image_id"]]["delta"] for row in checked}
    proven = {row["id"]: row["installs"] for row in _Prover(original, functions, starts).prove(checked, vram)}
    for row in checked:
        if not row["installs"] or proven.get(row["id"]) != row["installs"]:
            raise VtableError(f"vtable {row['start']:#x} start is not anchored by its pinned proven vptr installs")
        result[(row["start"], row["end"])] = row["id"]
    return result
