#!/usr/bin/env python3
"""Read-only, whole-catalogue MIPS o32 ABI evidence from original splat assembly.

No compiler, generated C, or accepted-build mutation is involved. ``live_in`` is
an alias for ``proven_live_in``; ``conservative_live_in`` additionally assumes
unknown indirect callees consume all four integer argument registers. This
certainty split deliberately does not turn a function-pointer call into proof of
an otherwise unused parameter. All argument indexes in evidence are zero based;
the live-in numbers are slot counts (highest incoming slot read plus one).
"""
from __future__ import annotations

import argparse
import bisect
import json
import re
import sys
import time
from collections import defaultdict, deque
from dataclasses import dataclass, field
from pathlib import Path
from typing import Any

GPRS = ("zero at v0 v1 a0 a1 a2 a3 t0 t1 t2 t3 t4 t5 t6 t7 "
        "s0 s1 s2 s3 s4 s5 s6 s7 t8 t9 k0 k1 gp sp fp ra").split()
REG_NUM = {name: i for i, name in enumerate(GPRS)}
REG_NUM.update({f"f{i}": 32 + i for i in range(32)})
FP_ALIASES = {"fv0": 0, "fv1": 2, "ft0": 4, "ft1": 6, "ft2": 8,
              "ft3": 10, "fa0": 12, "fa1": 14, "ft4": 16, "ft5": 18,
              "fs0": 20, "fs1": 22, "fs2": 24, "fs3": 26, "fs4": 28, "fs5": 30}
for _name, _num in list(FP_ALIASES.items()):
    FP_ALIASES[_name + "f"] = _num + 1
ARG_MASK = sum(1 << i for i in range(4, 8))
FLOAT_MASK = sum(1 << i for i in (44, 45, 46, 47))
RESULT_MASK = (1 << 2) | (1 << 32) | (1 << 33)
CLOBBERS = sum(1 << REG_NUM[r] for r in
               ("at v0 v1 a0 a1 a2 a3 t0 t1 t2 t3 t4 t5 t6 t7 t8 t9 ra").split())
CLOBBERS |= sum(1 << i for i in range(32, 52))
ROW = re.compile(r"^\s*/\*\s*([0-9A-Fa-f]+)\s+([0-9A-Fa-f]+)\s+([0-9A-Fa-f]{8})\s*\*/\s*(.*?)\s*$")
LABEL = re.compile(r"^\s*(?:([gdj]label)\s+([\w.$]+)|([\w.$]+):)\s*$")
REG = re.compile(r"\$([A-Za-z0-9]+)")
SYMBOL = re.compile(r"%(?:hi|lo)\(([\w.$]+)\)")
MEMORY = re.compile(r"^(.*?)\(\$([A-Za-z0-9]+)\)$")
LOADS = set("lb lbu lh lhu lw lwu ld lwl lwr ldl ldr ll lld lwc1 ldc1 lwc2 ldc2".split())
STORES = set("sb sh sw sd swl swr sdl sdr sc scd swc1 sdc1 swc2 sdc2".split())
BRANCHES = set("b beq bne beqz bnez bgez bgtz blez bltz beql bnel beqzl bnezl bgezl bgtzl blezl bltzl bc1t bc1f bc1tl bc1fl bc0t bc0f bc0tl bc0fl bgezal bltzal bgezall bltzall bal".split())
LIKELY = {m for m in BRANCHES if m.endswith("l")} - {"bal", "bgezal", "bltzal"}
LINK_BRANCHES = {"bal", "bgezal", "bltzal", "bgezall", "bltzall"}
ARITHMETIC = set("add addu dadd daddu sub subu dsub dsubu and or xor nor slt sltu sll srl sra sllv srlv srav dsll dsrl dsra dsllv dsrlv dsrav dsll32 dsrl32 dsra32 addi addiu daddi daddiu andi ori xori slti sltiu lui li la move neg negu dneg dnegu not abs".split())


def hx(value: int) -> str:
    return f"0x{value:08X}"


def reg_name(name: str) -> str:
    name = name.lstrip("$").lower()
    if name in FP_ALIASES:
        return f"f{FP_ALIASES[name]}"
    if name == "s8":
        return "fp"
    if name.isdecimal() and int(name) < 32:
        return GPRS[int(name)]
    return name


def reg_bit(name: str) -> int:
    number = REG_NUM.get(reg_name(name))
    return 0 if number is None or number == 0 else 1 << number


def reg_bits(operand: str) -> int:
    result = 0
    for name in REG.findall(operand):
        result |= reg_bit(name)
    return result


def immediate(value: str) -> int | None:
    try:
        return int(value.strip(), 0)
    except ValueError:
        return None


@dataclass(slots=True)
class Instruction:
    rom: int
    address: int
    word: int
    op: str
    operands: tuple[str, ...]
    source: str
    reads: int = 0
    writes: int = 0
    unknown: bool = False


def instruction_effects(ins: Instruction) -> None:
    op, args = ins.op, ins.operands
    bits = [reg_bits(arg) for arg in args]
    reads = writes = 0
    if op in {"nop", "sync", "break", "syscall", "eret", "rfe", "tlbp", "tlbr", "tlbwi", "tlbwr"}:
        pass
    elif op in LOADS:
        writes = bits[0]
        reads = sum(bits[1:])
        if op in {"lwl", "lwr", "ldl", "ldr"}:
            reads |= bits[0]  # the unaligned load merges with the old register
    elif op in STORES:
        reads = 0
        for bit in bits:
            reads |= bit
        if op in {"sc", "scd"}:
            writes = bits[0]
    elif op in BRANCHES:
        for bit in bits[:-1]:
            reads |= bit
        if op in LINK_BRANCHES:
            writes = reg_bit("ra")
    elif op in {"jal", "j"}:
        writes = reg_bit("ra") if op == "jal" else 0
    elif op in {"jr", "jalr"}:
        reads = bits[-1]
        if op == "jalr":
            writes = bits[0] if len(bits) == 2 else reg_bit("ra")
    elif op in {"mtc0", "dmtc0", "ctc0", "ctc1", "mtc2", "dmtc2"}:
        reads = bits[0]
    elif op in {"mfc0", "dmfc0", "cfc0", "cfc1", "mfc2", "dmfc2"}:
        writes = bits[0]
    elif op in {"mtc1", "dmtc1"}:
        reads, writes = bits[0], bits[1]
    elif op in {"mfc1", "dmfc1"}:
        reads, writes = bits[1], bits[0]
    elif op in {"mult", "multu", "dmult", "dmultu", "div", "divu", "ddiv", "ddivu", "mthi", "mtlo"}:
        for bit in bits:
            reads |= bit
    elif op in {"mfhi", "mflo"}:
        writes = bits[0]
    elif op.startswith("c.") or op in {"cache", "pref", "teq", "tne", "tge", "tgeu", "tlt", "tltu", "teqi", "tnei", "tgei", "tgeiu", "tlti", "tltiu"}:
        for bit in bits:
            reads |= bit
    elif op in {"movn", "movz"} or op.startswith(("movt.", "movf.", "movn.", "movz.")):
        for bit in bits:
            reads |= bit  # a conditional move does not definitely kill its destination
    elif op in ARITHMETIC or re.fullmatch(r"(?:add|sub|mul|div|sqrt|abs|mov|neg|cvt\.[sdwl]|trunc\.[wl]|round\.[wl]|ceil\.[wl]|floor\.[wl])\.[sdwl]", op):
        writes = bits[0] if bits else 0
        for bit in bits[1:]:
            reads |= bit
    else:
        ins.unknown = True
        for bit in bits:
            reads |= bit  # visible operands, but never invent a definite kill
    even_fp = sum(1 << i for i in range(32, 64, 2))
    formats = op.split(".")
    conversion = len(formats) == 3 and formats[0] in {"cvt", "trunc", "round", "ceil", "floor"}
    if op in {"sdc1", "dmfc1"} or formats[-1] in {"d", "l"}:
        reads |= (reads & even_fp) << 1
    if op in {"ldc1", "dmtc1"} or (conversion and formats[1] in {"d", "l"}) or (not conversion and formats[-1] == "d"):
        writes |= (writes & even_fp) << 1
    ins.reads, ins.writes = reads, writes


@dataclass
class Assembly:
    instructions: dict[int, Instruction] = field(default_factory=dict)
    labels: dict[str, int] = field(default_factory=dict)
    tables: dict[str, tuple[str, ...]] = field(default_factory=dict)
    functions: list[dict[str, Any]] = field(default_factory=list)


def parse_assembly(text: str, source: str = "<memory>", image_id: str = "main_14400") -> Assembly:
    """Parse instructions, aliases, and rodata tables without treating data as code."""
    result = Assembly()
    pending: list[str] = []
    current: tuple[str, list[Instruction]] | None = None
    data_label: str | None = None
    table_words: dict[str, list[str]] = defaultdict(list)
    for line in text.splitlines():
        label = LABEL.match(line)
        if label:
            kind, name, plain = label.groups()
            name = name or plain
            pending.append(name)
            if kind == "glabel" and not name.startswith("jtbl") and current is None:
                current = (name, [])
            if kind == "dlabel" or name.startswith("jtbl"):
                data_label = name
            continue
        if line.strip().startswith("endlabel "):
            name = line.split()[1]
            if current and current[0] == name:
                symbol, words = current
                if words:
                    result.functions.append({"symbol": symbol, "image_id": image_id,
                                             "rom_start": words[0].rom, "vram_start": words[0].address,
                                             "size": words[-1].address + 4 - words[0].address})
                current = None
            continue
        if line.strip().startswith("enddlabel "):
            data_label = None
            pending.clear()
            continue
        row = ROW.match(line)
        body = row[4] if row else line.strip()
        body = re.sub(r"/\*.*?\*/|#.*$", "", body).strip()
        if row:
            address = int(row[2], 16)
            for name in pending:
                result.labels[name] = address
            pending.clear()
        if body.startswith(".word"):
            if data_label:
                table_words[data_label].extend(v.strip() for v in body[5:].split(","))
            continue
        if not row or not body or body.startswith("."):
            continue
        pieces = body.split(None, 1)
        operands = tuple(part.strip() for part in pieces[1].split(",")) if len(pieces) == 2 else ()
        ins = Instruction(int(row[1], 16), address, int(row[3], 16), pieces[0], operands, source)
        instruction_effects(ins)
        result.instructions[address] = ins
        if current:
            current[1].append(ins)
    for name, words in table_words.items():
        if name.startswith("jtbl") or any(w.startswith(".L") for w in words):
            result.tables[name] = tuple(words)
    return result


@dataclass(slots=True)
class Node:
    ins: Instruction | None
    kind: str = "instruction"
    address: int = 0
    target: str | None = None
    tail: bool = False
    indirect: bool = False
    successors: list[int] = field(default_factory=list)
    reads: int = 0
    writes: int = 0


@dataclass
class Function:
    row: dict[str, Any]
    instructions: list[Instruction]
    nodes: list[Node] = field(default_factory=list)
    values: list[dict[str, tuple[str, Any]] | None] = field(default_factory=list)
    available: list[int | None] = field(default_factory=list)
    proven: int = 0
    conservative: int = 0
    varargs: dict[str, Any] | None = None
    dead_home_spills: list[dict[str, Any]] = field(default_factory=list)
    spill_only_varargs: dict[str, Any] | None = None
    indirect_origins: dict[int, set[int]] = field(default_factory=dict)
    warnings: set[str] = field(default_factory=set)
    jump_targets: dict[int, tuple[int, ...]] = field(default_factory=dict)
    callsites: list[dict[str, Any]] = field(default_factory=list)


def target_address(target: str, labels: dict[str, int]) -> int | None:
    if target in labels:
        return labels[target]
    value = immediate(target)
    if value is not None:
        return value
    match = re.search(r"(?:_|\.L)([0-9A-Fa-f]{8})$", target)
    return int(match[1], 16) if match else None


def branch_truth(ins: Instruction) -> bool | None:
    args = ins.operands
    if ins.op in {"b", "bal"}:
        return True
    if len(args) >= 3 and args[0] == args[1]:
        if ins.op in {"beq", "beql"}:
            return True
        if ins.op in {"bne", "bnel"}:
            return False
    if args and reg_name(args[0]) == "zero":
        if ins.op in {"beqz", "beqzl", "bgez", "bgezl", "blez", "blezl", "bgezal", "bgezall"}:
            return True
        if ins.op in {"bnez", "bnezl", "bltz", "bltzl", "bgtz", "bgtzl", "bltzal", "bltzall"}:
            return False
    return None


def build_cfg(fn: Function, labels: dict[str, int], symbols: dict[int, str]) -> None:
    words = fn.instructions
    indexes = {ins.address: i for i, ins in enumerate(words)}
    nodes = [Node(ins, address=ins.address, reads=ins.reads, writes=ins.writes) for ins in words]

    def add(kind: str, ins: Instruction, **kwargs: Any) -> int:
        nodes.append(Node(None, kind, ins.address, **kwargs))
        return len(nodes) - 1

    def destination(ins: Instruction, address: int | None, target: str) -> int:
        if address in indexes:
            return indexes[address]
        return add("call", ins, target=symbols.get(address, target), tail=True,
                   writes=CLOBBERS)

    for i, ins in enumerate(words):
        op = ins.op
        next_index = indexes.get(ins.address + 4)
        if op not in BRANCHES | {"j", "jal", "jr", "jalr"}:
            if next_index is not None and op not in {"eret", "break", "syscall"}:
                nodes[i].successors = [next_index]
            continue
        slot = indexes.get(ins.address + 4)
        if slot is None:
            raise ValueError(f"{fn.row['symbol']}: missing delay slot at {hx(ins.address)}")
        delay = words[slot]
        if delay.op in BRANCHES | {"j", "jal", "jr", "jalr"}:
            fn.warnings.add(f"control instruction in delay slot at {hx(delay.address)}")
        nodes.append(Node(delay, address=delay.address, reads=delay.reads, writes=delay.writes))
        delayed = len(nodes) - 1
        continuation = indexes.get(ins.address + 8)
        nodes[i].successors = [delayed]
        if op == "jr" and reg_name(ins.operands[0]) == "ra":
            nodes[delayed].successors = [add("return", ins)]
        elif op == "jr" and ins.address in fn.jump_targets:
            nodes[delayed].successors = [destination(ins, target, symbols.get(target, hx(target)))
                                           for target in fn.jump_targets[ins.address]]
        elif op in {"jal", "jalr"} or op == "jr":
            indirect = op != "jal"
            target = None if indirect else symbols.get(target_address(ins.operands[-1], labels), ins.operands[-1])
            tail = op == "jr" or (op == "jalr" and len(ins.operands) == 2 and reg_name(ins.operands[0]) == "zero")
            event = add("call", ins, target=target, tail=tail, indirect=indirect, writes=CLOBBERS)
            nodes[delayed].successors = [event]
            if not tail and continuation is not None:
                nodes[event].successors = [continuation]
        elif op == "j":
            nodes[delayed].successors = [destination(ins, target_address(ins.operands[0], labels), ins.operands[0])]
        else:
            truth = branch_truth(ins)
            target = target_address(ins.operands[-1], labels)
            if op in LINK_BRANCHES:
                event = add("call", ins, target=symbols.get(target, ins.operands[-1]), writes=CLOBBERS)
                if continuation is not None:
                    nodes[event].successors = [continuation]
                taken = event
            else:
                taken = destination(ins, target, ins.operands[-1])
            if op in LIKELY:
                nodes[delayed].successors = [taken]
                nodes[i].successors = ([delayed] if truth is not False else [])
                if truth is not True and continuation is not None:
                    nodes[i].successors.append(continuation)
            else:
                nodes[delayed].successors = [taken] if truth is not False else []
                if truth is not True and continuation is not None:
                    nodes[delayed].successors.append(continuation)
    fn.nodes = nodes


def transfer_values(node: Node, incoming: dict[str, tuple[str, Any]], tables: dict[str, tuple[str, ...]]) -> dict[str, tuple[str, Any]]:
    values = dict(incoming)
    for reg in list(values):
        if reg_bit(reg) & node.writes:
            del values[reg]
    ins = node.ins
    if ins is None or not ins.operands:
        return values
    args, op = ins.operands, ins.op
    dest = reg_name(args[0])
    if not args[0].startswith("$") or not (reg_bit(dest) & ins.writes):
        return values
    symbols = [name for operand in args for name in SYMBOL.findall(operand) if name in tables]
    value: tuple[str, Any] | None = None
    src = lambda operand: ("int", 0) if reg_name(operand) == "zero" else incoming.get(reg_name(operand))
    if op in LOADS:
        memory = MEMORY.match(args[-1])
        base = incoming.get(reg_name(memory[2])) if memory else None
        if symbols or (base and base[0] == "table"):
            value = ("targets", symbols[0] if symbols else base[1])
    elif symbols:
        value = ("table", symbols[0])
    elif op in {"li", "lui"}:
        number = immediate(args[-1])
        if number is not None:
            value = ("int", number << 16 if op == "lui" else number)
    elif op in {"move", "daddu", "addu", "dadd", "add", "or"}:
        sources = [src(arg) for arg in args[1:]]
        if len(sources) == 1:
            value = sources[0]
        elif any(v and v[0] == "table" for v in sources):
            value = next(v for v in sources if v and v[0] == "table")
        elif len(sources) == 2:
            left, right = sources
            if right == ("int", 0):
                value = left
            elif left == ("int", 0):
                value = right
            elif left and right and op != "or":
                if right[0] == "int" and left[0] in {"sp", "int"}:
                    value = (left[0], left[1] + right[1])
                elif left[0] == "int" and right[0] == "sp":
                    value = ("sp", left[1] + right[1])
    elif op in {"addi", "addiu", "daddi", "daddiu", "ori", "andi"}:
        base, number = src(args[1]), immediate(args[2])
        if base and number is not None:
            if op in {"ori", "andi"} and base[0] == "int":
                value = ("int", base[1] | number if op == "ori" else base[1] & number)
            elif op not in {"ori", "andi"} and base[0] in {"sp", "int"}:
                value = (base[0], base[1] + number)
    elif op == "and" and len(args) == 3:
        left, right = src(args[1]), src(args[2])
        if left and right:
            if left[0] == "sp" and right == ("int", -4):
                value = ("sp", left[1] & -4)  # entry sp is at least eight-byte aligned
            elif left[0] == right[0] == "int":
                value = ("int", left[1] & right[1])
    if value is not None:
        values[dest] = value
    return values


def symbolic_values(fn: Function, tables: dict[str, tuple[str, ...]]) -> None:
    states: list[dict[str, tuple[str, Any]] | None] = [None] * len(fn.nodes)
    states[0] = {"sp": ("sp", 0)}
    queue, queued = deque([0]), {0}
    while queue:
        index = queue.popleft()
        queued.discard(index)
        outgoing = transfer_values(fn.nodes[index], states[index], tables)
        for target in fn.nodes[index].successors:
            old = states[target]
            merged = outgoing if old is None else {k: v for k, v in old.items() if outgoing.get(k) == v}
            if old != merged:
                states[target] = merged
                if target not in queued:
                    queue.append(target)
                    queued.add(target)
    fn.values = states


def memory_offset(ins: Instruction, values: dict[str, tuple[str, Any]]) -> int | None:
    if ins.op not in LOADS | STORES or not ins.operands:
        return None
    memory = MEMORY.match(ins.operands[-1])
    if not memory:
        return None
    number, base = immediate(memory[1]), values.get(reg_name(memory[2]))
    if number is None or base is None or base[0] != "sp":
        return None
    return base[1] + number


def memory_width(op: str) -> int:
    if op in {"lb", "lbu", "sb"}:
        return 1
    if op in {"lh", "lhu", "sh"}:
        return 2
    return 8 if op in {"ld", "sd", "ldc1", "sdc1", "lld", "scd", "ldl", "ldr", "sdl", "sdr", "ldc2", "sdc2"} else 4


def memory_bytes(op: str, offset: int) -> range:
    width = memory_width(op)
    if op in {"lwl", "lwr", "ldl", "ldr", "swl", "swr", "sdl", "sdr"}:
        aligned = offset & -width
        if op.endswith("l"):
            return range(offset, aligned + width)
        return range(aligned, offset + 1)
    return range(offset, offset + width)


def prepare_function(fn: Function, asm: Assembly, symbols: dict[int, str]) -> None:
    while True:
        build_cfg(fn, asm.labels, symbols)
        symbolic_values(fn, asm.tables)
        added = False
        for index, ins in enumerate(fn.instructions):
            if ins.op != "jr" or reg_name(ins.operands[0]) == "ra" or fn.values[index] is None:
                continue
            value = fn.values[index].get(reg_name(ins.operands[0]))
            if value and value[0] == "targets":
                targets = tuple(sorted({address for name in asm.tables[value[1]]
                                        if (address := target_address(name, asm.labels)) not in {None, 0}}))
                if targets and fn.jump_targets.get(ins.address) != targets:
                    fn.jump_targets[ins.address] = targets
                    added = True
        if not added:
            break
    # A consecutive complementary unaligned-load pair constructs a whole word.
    # Its first merge's discarded old bytes are not incoming ABI arguments.
    # Only fold when the CFG guarantees the second load follows (not a taken
    # branch's delay-slot load). Entry directly into the second remains a read.
    for node in fn.nodes:
        ins = node.ins
        if ins is None or ins.op not in {"lwl", "lwr", "ldl", "ldr"} or len(node.successors) != 1:
            continue
        following = fn.nodes[node.successors[0]].ins
        if following is None or following.address != ins.address + 4 or not following.operands or following.operands[0] != ins.operands[0]:
            continue
        complement = {"lwl": ("lwr", 3), "lwr": ("lwl", -3), "ldl": ("ldr", 7), "ldr": ("ldl", -7)}
        expected, delta = complement[ins.op]
        first_memory, second_memory = MEMORY.match(ins.operands[-1]), MEMORY.match(following.operands[-1])
        if following.op != expected or not first_memory or not second_memory or first_memory[2] != second_memory[2]:
            continue
        left, right = immediate(first_memory[1]), immediate(second_memory[1])
        if left is not None and right == left + delta and reg_name(first_memory[2]) != reg_name(ins.operands[0]):
            node.reads &= ~reg_bits(ins.operands[0])
    for index, node in enumerate(fn.nodes):
        if node.ins and node.ins.unknown:
            fn.warnings.add(f"unmodelled instruction {node.ins.op} at {hx(node.address)}")
        state = fn.values[index]
        if state is None or node.ins is None:
            continue
        offset = memory_offset(node.ins, state)
        if offset is None:
            continue
        for byte in memory_bytes(node.ins.op, offset):
            if byte >= 16:
                bit = 1 << (64 + byte // 4)
                if node.ins.op in LOADS:
                    node.reads |= bit
                elif offset % 4 == 0 and memory_width(node.ins.op) >= 4 and node.ins.op not in {"swl", "swr", "sdl", "sdr"}:
                    node.writes |= bit


def entry_availability(fn: Function, input_mask: int) -> None:
    states: list[int | None] = [None] * len(fn.nodes)
    states[0] = input_mask | RESULT_MASK
    queue = deque([0])
    while queue:
        index = queue.popleft()
        outgoing = states[index] & ~fn.nodes[index].writes
        for target in fn.nodes[index].successors:
            old = states[target]
            merged = outgoing if old is None else old | outgoing
            if old != merged:
                states[target] = merged
                queue.append(target)
    fn.available = states


def callee_inputs(fn: Function, index: int, mask: int, callee: Function | None = None) -> int:
    """Translate stack slots; optional variadic inputs are not caller live-ins.

    Passing a callee selects the required input contract. Omitting it preserves raw
    instruction uses for the separate result-dependency analysis.
    """
    if callee and (callee.varargs or callee.spill_only_varargs):
        limit = callee.varargs["first_variadic_slot"] if callee.varargs else 1
        mask = sum(1 << bit for bit in iter_bits(mask)
                   if (slot := argument_index(bit)) is None or slot < limit)
    result = mask & ((1 << 64) - 1)
    state = fn.values[index]
    sp = state.get("sp") if state else None
    if sp and sp[0] == "sp":
        for bit in iter_bits(mask >> 64):
            offset = sp[1] + bit * 4
            if offset >= 16 and offset % 4 == 0:
                result |= 1 << (64 + offset // 4)
    return result


def iter_bits(mask: int):
    while mask:
        low = mask & -mask
        yield low.bit_length() - 1
        mask ^= low


def detect_varargs(fn: Function) -> dict[str, Any] | None:
    saves: dict[str, int] = {}
    for index, node in enumerate(fn.nodes):
        ins, state, available = node.ins, fn.values[index], fn.available[index]
        if ins is None or state is None or available is None or ins.op != "sw":
            continue
        reg = reg_name(ins.operands[0])
        if reg not in {"a0", "a1", "a2", "a3"} or not (available & reg_bit(reg)):
            continue
        if memory_offset(ins, state) == 4 * (REG_NUM[reg] - 4):
            saves[reg] = min(saves.get(reg, ins.address), ins.address)
    if "a3" not in saves:
        return None
    home_states: list[int | None] = [None] * len(fn.nodes)
    home_states[0] = 0
    queue = deque([0])
    while queue:
        index = queue.popleft()
        node, state = fn.nodes[index], fn.values[index]
        outgoing = home_states[index]
        if node.ins is not None and state is not None and node.ins.op in STORES:
            offset = memory_offset(node.ins, state)
            if offset is not None:
                for byte in range(offset, offset + memory_width(node.ins.op)):
                    if 0 <= byte < 16:
                        outgoing &= ~(1 << (byte // 4))
                reg = reg_name(node.ins.operands[0])
                if node.ins.op == "sw" and reg in saves and offset == 4 * (REG_NUM[reg] - 4) and fn.available[index] & reg_bit(reg):
                    outgoing |= 1 << (REG_NUM[reg] - 4)
        for target in node.successors:
            old = home_states[target]
            merged = outgoing if old is None else old & outgoing
            if old != merged:
                home_states[target] = merged
                queue.append(target)
    candidates = []
    for index, node in enumerate(fn.nodes):
        ins, state = node.ins, fn.values[index]
        if ins is None or state is None or ins.op not in {"addiu", "addi", "daddiu", "daddi"}:
            continue
        if reg_name(ins.operands[0]) == "sp":
            continue
        value = transfer_values(node, state, {}).get(reg_name(ins.operands[0]))
        if value is None or value[0] != "sp" or not 4 <= value[1] < 20:
            continue
        first = value[1] // 4
        required = {f"a{i}" for i in range(first, 4)}
        if not required or not required <= saves.keys() or ins.address <= max(saves[r] for r in required):
            continue
        required_mask = sum(1 << i for i in range(first, 4))
        if home_states[index] is None or home_states[index] & required_mask != required_mask:
            continue
        candidates.append((ins.address, first, value[1]))
    if not candidates:
        return None
    address, fixed, offset = min(candidates)
    return {"fixed_parameter_count": fixed, "first_variadic_slot": fixed,
            "saved_registers": sorted(saves), "evidence_addresses": sorted({hx(v) for v in saves.values()} | {hx(address)}),
            "address_taken_offset": offset,
            "inference": "contiguous argument-home spills followed by entry-SP-relative va_start address; count is o32 slots"}


def drop_unused_home_spills(fn: Function) -> None:
    """Discard only dead incoming-register homing, retaining reloads and escapes."""
    saves: dict[int, list[int]] = defaultdict(list)
    used_slots: set[int] = set()
    other_reads = 0
    has_calls = False

    def escaped(value: tuple[str, Any] | None) -> None:
        if value and value[0] == "sp" and 0 <= value[1] < 16:
            # An escaped pointer can advance through the remaining home area.
            used_slots.update(range(value[1] // 4, 4))

    for index, node in enumerate(fn.nodes):
        available, state, ins = fn.available[index], fn.values[index], node.ins
        if available is None or state is None:
            continue
        reads = node.reads & ARG_MASK & available
        outgoing = transfer_values(node, state, {}) if node.writes else state
        if ins is not None:
            offset = memory_offset(ins, state)
            if ins.op == "sw":
                reg = reg_name(ins.operands[0])
                if reg in {"a0", "a1", "a2", "a3"}:
                    slot = REG_NUM[reg] - 4
                    if offset == 4 * slot and available & reg_bit(reg):
                        saves[slot].append(index)
                        reads &= ~reg_bit(reg)
            if ins.op in LOADS and offset is not None:
                used_slots.update(byte // 4 for byte in memory_bytes(ins.op, offset)
                                  if 0 <= byte < 16)
            if ins.op in STORES:
                escaped(state.get(reg_name(ins.operands[0])))
            elif ins.writes:
                for reg, value in state.items():
                    if ins.reads & reg_bit(reg) and value[0] == "sp":
                        # Preserve unknown pointer arithmetic/bit manipulation as
                        # an escape rather than claiming a home is unobservable.
                        if not any(v[0] == "sp" and ins.writes & reg_bit(r)
                                   for r, v in outgoing.items()):
                            if ins.op not in LOADS:
                                escaped(value)
        for successor in node.successors:
            merged = fn.values[successor]
            if merged is not None:
                for reg, value in outgoing.items():
                    if reg != "sp" and merged.get(reg) != value:
                        escaped(value)
        other_reads |= reads
        if node.kind == "call":
            has_calls = True
            for reg in ("a0", "a1", "a2", "a3"):
                escaped(state.get(reg))
        elif node.kind == "return":
            escaped(state.get("v0"))
    dead = set()
    for slot, indexes in sorted(saves.items()):
        bit = reg_bit(f"a{slot}")
        if slot in used_slots or other_reads & bit:
            continue
        dead.add(slot)
        for index in indexes:
            fn.nodes[index].reads &= ~bit
        fn.dead_home_spills.append({
            "argument_index": slot, "register": f"a{slot}",
            "addresses": sorted({hx(fn.nodes[index].address) for index in indexes}),
        })
    if {1, 2, 3} <= dead and not has_calls:
        fn.spill_only_varargs = {
            "compatible": True,
            "minimum_fixed_parameter_count": 1,
            "evidence_addresses": sorted({address for spill in fn.dead_home_spills
                                          for address in spill["addresses"]}),
            "inference": "unused incoming a1-a3 home spills with no reload, escaping home address, or call; compatible with unused varargs, not proof of a variadic contract",
        }


def infer_inputs(functions: list[Function], by_symbol: dict[str, Function], input_mask: int) -> int:
    for fn in functions:
        entry_availability(fn, input_mask)
        fn.varargs = detect_varargs(fn)
        drop_unused_home_spills(fn)
        for index, node in enumerate(fn.nodes):
            available = fn.available[index]
            if available is not None:
                fn.proven |= node.reads & available & input_mask
        fn.conservative = fn.proven
    rounds = 0
    while True:
        rounds += 1
        changed = False
        for fn in functions:
            proven, conservative = fn.proven, fn.conservative
            for index, node in enumerate(fn.nodes):
                available = fn.available[index]
                if node.kind != "call" or available is None:
                    continue
                callee = by_symbol.get(node.target)
                if callee is None:
                    conservative |= ARG_MASK & available
                    if not node.indirect:
                        fn.warnings.add(f"uncatalogued direct callee {node.target} at {hx(node.address)}; argument use is conservative")
                else:
                    proven |= callee_inputs(fn, index, callee.proven, callee) & available
                    conservative |= callee_inputs(fn, index, callee.conservative, callee) & available
            if (proven, conservative) != (fn.proven, fn.conservative):
                fn.proven, fn.conservative = proven, conservative
                changed = True
        if not changed:
            break
    # Preserve the original indirect call addresses through chains and recursion.
    changed = True
    while changed:
        changed = False
        for fn in functions:
            for index, node in enumerate(fn.nodes):
                available = fn.available[index]
                if node.kind != "call" or available is None:
                    continue
                callee = by_symbol.get(node.target)
                if callee is None:
                    contributions = {bit: {node.address} for bit in iter_bits(ARG_MASK & available)}
                else:
                    contributions = {}
                    for bit, origins in callee.indirect_origins.items():
                        for mapped in iter_bits(callee_inputs(fn, index, 1 << bit, callee) & available):
                            contributions.setdefault(mapped, set()).update(origins)
                for bit, origins in contributions.items():
                    old = fn.indirect_origins.setdefault(bit, set())
                    if not origins <= old:
                        old.update(origins)
                        changed = True
    return rounds


def transparent_move(ins: Instruction | None) -> tuple[int, int] | None:
    """Return source/destination masks for copies, not computations or sinks."""
    if ins is None or not ins.operands:
        return None
    args, op = ins.operands, ins.op
    source = None
    if op in {"move", "mov.s", "mov.d", "mtc1", "mfc1", "dmtc1", "dmfc1"}:
        source = args[0] if op in {"mtc1", "dmtc1"} else args[1]
    elif op in {"addu", "daddu", "or", "add", "dadd"} and len(args) == 3:
        if reg_name(args[1]) == "zero":
            source = args[2]
        elif reg_name(args[2]) == "zero":
            source = args[1]
    elif op in {"addi", "addiu", "daddi", "daddiu", "sll", "srl", "dsll", "dsrl"} and len(args) == 3 and immediate(args[2]) == 0:
        source = args[1]
    if source is None:
        return None
    # Reads/writes already include FP register pairs where appropriate.
    return ins.reads, ins.writes


def result_paths(fn: Function, start: list[int], by_symbol: dict[str, Function]) -> tuple[dict, dict, dict]:
    proven = {"v0": set(), "f0": set()}
    conservative = {"v0": set(), "f0": set()}
    forwards = {origin: {"v0": set(), "f0": set()} for origin in ("v0", "f0")}
    # Track transparent copies through every register, including callee-saved
    # registers across intervening calls. Computations are concrete consumers;
    # copies are consumers only when their downstream use is established.
    for origin, initial in (("v0", 1 << 2), ("f0", (1 << 32) | (1 << 33))):
        seen: dict[int, int] = {}
        queue = deque((index, initial) for index in start)
        while queue:
            index, alive = queue.popleft()
            alive &= ~seen.get(index, 0)
            if not alive:
                continue
            seen[index] = seen.get(index, 0) | alive
            node = fn.nodes[index]
            move = transparent_move(node.ins)
            if node.kind == "call":
                callee = by_symbol.get(node.target)
                direct_inputs = callee_inputs(fn, index, callee.proven) if callee else 0
                all_inputs = callee_inputs(fn, index, callee.conservative) if callee else ARG_MASK
                if direct_inputs & alive:
                    proven[origin].add(node.address)
                if all_inputs & alive:
                    conservative[origin].add(node.address)
                    if callee:
                        for bit, addresses in callee.indirect_origins.items():
                            if callee_inputs(fn, index, 1 << bit) & alive:
                                conservative[origin].update(addresses)
            elif node.kind == "return":
                if alive & (1 << 2):
                    forwards[origin]["v0"].add(node.address)
                if alive & ((1 << 32) | (1 << 33)):
                    forwards[origin]["f0"].add(node.address)
            elif move is None and node.reads & alive:
                proven[origin].add(node.address)
            outgoing = alive & ~node.writes
            if move and move[0] & alive:
                outgoing |= move[1]
            if outgoing:
                queue.extend((target, outgoing) for target in node.successors)
        conservative[origin].update(proven[origin])
    return proven, conservative, forwards


def infer_consumers(functions: list[Function], by_symbol: dict[str, Function]) -> dict[str, list[dict[str, Any]]]:
    demand = {certainty: {name: {"v0": set(), "f0": set()} for name in by_symbol}
              for certainty in ("proven", "conservative")}
    for fn in functions:
        for index, node in enumerate(fn.nodes):
            if node.kind != "call" or fn.available[index] is None or node.target not in by_symbol:
                continue
            if node.tail:
                proven = {"v0": set(), "f0": set()}
                conservative = {"v0": set(), "f0": set()}
                forwards = {"v0": {"v0": {node.address}, "f0": set()},
                            "f0": {"v0": set(), "f0": {node.address}}}
            else:
                proven, conservative, forwards = result_paths(fn, node.successors, by_symbol)
            record = {"caller": fn.row["symbol"], "caller_image": fn.row["image_id"],
                      "call_address": hx(node.address), "tail": node.tail,
                      "callee": node.target, "proven": proven,
                      "conservative": conservative, "forwards": forwards}
            fn.callsites.append(record)
            for certainty in demand:
                for reg, addresses in record[certainty].items():
                    demand[certainty][node.target][reg].update(addresses)
    changed = True
    while changed:
        changed = False
        for fn in functions:
            for site in fn.callsites:
                for certainty in demand:
                    for origin, outputs in site["forwards"].items():
                        for output, returns in outputs.items():
                            if not returns:
                                continue
                            new = demand[certainty][site["caller"]][output]
                            old = demand[certainty][site["callee"]][origin]
                            if not new <= old:
                                old.update(new)
                                changed = True
    consumers: dict[str, list[dict[str, Any]]] = defaultdict(list)
    for fn in functions:
        for site in fn.callsites:
            registers = {"proven": [], "conservative": []}
            evidence = {"proven": set(), "conservative": set()}
            propagation = set()
            for certainty in demand:
                for origin in ("v0", "f0"):
                    addresses = set(site[certainty][origin])
                    for output, returns in site["forwards"][origin].items():
                        downstream = demand[certainty][site["caller"]][output] if returns else set()
                        if downstream:
                            addresses.update(returns)
                            addresses.update(downstream)
                            propagation.add(f"{site['caller']}:{output}:{certainty}")
                    if addresses:
                        registers[certainty].append(origin)
                        evidence[certainty].update(addresses)
            consumers[site["callee"]].append({
                key: site[key] for key in ("caller", "caller_image", "call_address", "tail")
            } | {"consumed": bool(registers["proven"]), "proven_consumed": bool(registers["proven"]),
                 "conservative_consumed": bool(registers["conservative"]),
                 "registers": registers["proven"], "conservative_registers": registers["conservative"],
                 "evidence_addresses": [hx(v) for v in sorted(evidence["proven"] | evidence["conservative"])],
                 "proven_evidence_addresses": [hx(v) for v in sorted(evidence["proven"])],
                 "conservative_evidence_addresses": [hx(v) for v in sorted(evidence["conservative"])],
                 "propagation": sorted(propagation)})
    return consumers


def argument_index(bit: int) -> int | None:
    return bit - 4 if 4 <= bit <= 7 else bit - 64 if bit >= 68 else None


def slot_count(mask: int) -> int:
    return max((index + 1 for bit in iter_bits(mask) if (index := argument_index(bit)) is not None), default=0)


def function_facts(fn: Function, by_symbol: dict[str, Function], consumers: dict[str, list[dict[str, Any]]]) -> dict[str, Any]:
    evidence: dict[tuple[int, str, str, str | None], set[int]] = defaultdict(set)
    float_evidence: dict[str, set[int]] = defaultdict(set)
    for index, node in enumerate(fn.nodes):
        available = fn.available[index]
        if available is None:
            continue
        contributions = [(node.reads & available, "instruction", "proven", None)]
        if node.kind == "call":
            callee = by_symbol.get(node.target)
            if callee:
                proven = callee_inputs(fn, index, callee.proven, callee) & available
                conservative = callee_inputs(fn, index, callee.conservative, callee) & available
                contributions += [(proven, "direct-call", "proven", node.target),
                                  (conservative & ~proven, "direct-call", "indirect-conservative", node.target)]
            else:
                contributions.append((ARG_MASK & available, "indirect-conservative" if node.indirect else "unknown-callee-conservative", "indirect-conservative", node.target))
        for mask, reason, certainty, callee_name in contributions:
            for bit in iter_bits(mask):
                arg = argument_index(bit)
                if arg is not None:
                    evidence[arg, reason, certainty, callee_name].add(node.address)
                elif 44 <= bit <= 47:
                    float_evidence["f12" if bit < 46 else "f14"].add(node.address)
    live_evidence = []
    for (arg, reason, certainty, callee), addresses in sorted(evidence.items(), key=lambda item: (item[0][0], item[0][1], item[0][2], item[0][3] or "")):
        bit = arg + 4 if arg < 4 else arg + 64
        item = {"argument_index": arg, "register": f"a{arg}" if arg < 4 else None,
                "addresses": [hx(v) for v in sorted(addresses)], "reason": reason,
                "certainty": certainty, "callee": callee}
        if certainty != "proven":
            item["indirect_evidence_addresses"] = [hx(v) for v in sorted(fn.indirect_origins.get(bit, set()))]
        live_evidence.append(item)
    stack = []
    for bit in iter_bits(fn.proven):
        if bit >= 68:
            slot = bit - 64
            stack.append({"argument_index": slot, "offset": slot * 4,
                          "addresses": sorted({a for item in live_evidence if item["argument_index"] == slot and item["certainty"] == "proven" for a in item["addresses"]})})
    returns = [(index, node) for index, node in enumerate(fn.nodes)
               if node.kind == "return" and fn.available[index] is not None]
    float_args = [reg for reg, bits in (("f12", (1 << 44) | (1 << 45)), ("f14", (1 << 46) | (1 << 47))) if fn.proven & bits]
    return {"symbol": fn.row["symbol"], "image_id": fn.row["image_id"],
            "vram_start": hx(fn.row["vram_start"]), "rom_start": hx(fn.row["rom_start"]),
            "size": fn.row["size"], "source": fn.instructions[0].source,
            "live_in": slot_count(fn.proven), "proven_live_in": slot_count(fn.proven),
            "conservative_live_in": slot_count(fn.conservative),
            "live_in_registers": [f"a{i}" for i in range(4) if fn.proven & (1 << (i + 4))],
            "conservative_live_in_registers": [f"a{i}" for i in range(4) if fn.conservative & (1 << (i + 4))],
            "live_in_stack": stack, "live_in_evidence": live_evidence,
            "float_args": float_args, "float_evidence": [{"register": reg, "addresses": [hx(v) for v in sorted(addresses)]} for reg, addresses in sorted(float_evidence.items())],
            "varargs_prologue": fn.varargs,
            "dead_home_spills": fn.dead_home_spills,
            "spill_only_varargs": fn.spill_only_varargs,
            "result_consumers": sorted(consumers.get(fn.row["symbol"], []), key=lambda row: (row["caller_image"], row["call_address"], row["caller"])),
            "returns_value_paths": {"has_returns": bool(returns),
                "v0_written_on_all_returns": bool(returns) and all(not (fn.available[index] & (1 << 2)) for index, _ in returns),
                "f0_written_on_all_returns": bool(returns) and all(not (fn.available[index] & (1 << 32)) for index, _ in returns),
                "return_addresses": sorted({hx(node.address) for _, node in returns}),
                "includes_call_clobbers": True},
            "indirect_conservative": bool(fn.indirect_origins) or any(
                node.indirect and fn.available[index] is not None for index, node in enumerate(fn.nodes)),
            "jump_tables": [{"jump_address": hx(address), "targets": [hx(v) for v in targets]} for address, targets in sorted(fn.jump_targets.items())],
            "warnings": sorted(fn.warnings)}


def analyze_assembly(asm: Assembly, catalogue: list[dict[str, Any]]) -> dict[str, Any]:
    rows = sorted((row for row in catalogue if row["image_id"] in {"main_14400", "resident"}), key=lambda row: (row["image_id"], row["vram_start"]))
    addresses = sorted(asm.instructions)
    functions = []
    for row in rows:
        start = bisect.bisect_left(addresses, row["vram_start"])
        end = bisect.bisect_left(addresses, row["vram_start"] + row["size"])
        words = [asm.instructions[address] for address in addresses[start:end]]
        if len(words) * 4 != row["size"] or not words or words[0].rom != row["rom_start"]:
            raise ValueError(f"Incomplete original instructions for {row['symbol']}: {len(words) * 4}/{row['size']} bytes")
        functions.append(Function(row, words))
    symbols = {row["vram_start"]: row["symbol"] for row in rows}
    by_symbol = {fn.row["symbol"]: fn for fn in functions}
    if len(by_symbol) != len(functions):
        raise ValueError("Ambiguous symbols across CPU images")
    input_mask = ARG_MASK | FLOAT_MASK
    for fn in functions:
        prepare_function(fn, asm, symbols)
        for node in fn.nodes:
            input_mask |= node.reads & ~((1 << 64) - 1)
    if input_mask.bit_length() > 68:
        input_mask |= ((1 << input_mask.bit_length()) - 1) & ~((1 << 68) - 1)
    rounds = infer_inputs(functions, by_symbol, input_mask)
    consumers = infer_consumers(functions, by_symbol)
    facts = [function_facts(fn, by_symbol, consumers) for fn in functions]
    return {"schema_version": 1, "scope": "original main_14400 and resident CPU instructions; overlays excluded",
            "certainty_policy": "live_in aliases proven_live_in; indirect-only argument assumptions are conservative, never hard C-contract proof",
            "argument_home_policy": {
                "dead_home_spills": "exclude a register's sole instruction uses that store it into its own incoming sp+4*k home, provided the slot is never reloaded and its address cannot escape; unknown pointer transformations and merges retain the spill",
                "spill_only_varargs": "unused a1-a3 home spills without calls are compatible with an unused variadic declaration, not proof of variadicity",
                "caller_live_ins": "direct and tail callees with a va_start prologue contribute only slots before first_variadic_slot; optional register and stack varargs never become required caller live-ins",
                "result_dependencies": "retain separately observed callee instruction uses, including optional argument uses, for result-consumption analysis",
            },
            "result_consumption_policy": {
                "consumed": "alias of proven_consumed",
                "transparent_moves": "propagate dependencies through copies; a copy alone is not consumption",
                "direct_and_tail_arguments": "proven only when the callee's proven input-register set includes the forwarded register",
                "indirect_arguments": "conservative only unless another concrete consumer is reached",
                "concrete_consumers": "arithmetic, comparison/branch, memory value or address, and indirect call target",
                "wrapper_returns": "propagate downstream proven and conservative demand separately, including cross-register copies",
            },
            "functions": facts,
            "summary": {"functions": len(facts), "instructions": sum(len(fn.instructions) for fn in functions),
                        "images": {image: sum(f["image_id"] == image for f in facts) for image in sorted({f["image_id"] for f in facts})},
                        "input_fixed_point_rounds": rounds,
                        "varargs_prologues": sum(f["varargs_prologue"] is not None for f in facts),
                        "functions_with_dead_home_spills": sum(bool(f["dead_home_spills"]) for f in facts),
                        "spill_only_varargs_compatible": sum(f["spill_only_varargs"] is not None for f in facts),
                        "jump_table_jumps": sum(len(f["jump_tables"]) for f in facts),
                        "indirect_conservative_functions": sum(f["indirect_conservative"] for f in facts),
                        "functions_with_warnings": sum(bool(f["warnings"]) for f in facts)}}


def analyze_text(text: str, image_id: str = "main_14400") -> dict[str, Any]:
    asm = parse_assembly(text, image_id=image_id)
    return analyze_assembly(asm, asm.functions)


def analyze_project(root: Path, build: Path | None = None) -> dict[str, Any]:
    root = Path(root).resolve()
    # These are exactly the pointers used by tools.match.catalogue_functions and
    # accepted_build; reading them directly avoids importing compiler/build tools.
    progress = json.loads((root / "docs/progress.json").read_text())
    catalogue = json.loads((root / progress["denominator"]["path"]).read_text())["identity"]["functions"]
    if build is None:
        build = (root / json.loads((root / "build/latest.json").read_text())["receipt"]).parent
    elif not build.is_absolute():
        build = root / build
    asm = Assembly()
    for path in sorted((build / "generated/asm").rglob("*.s")):
        parsed = parse_assembly(path.read_text(), str(path.relative_to(root)))
        for address, ins in parsed.instructions.items():
            old = asm.instructions.get(address)
            if old is not None and (old.rom, old.word) != (ins.rom, ins.word):
                raise ValueError(f"Conflicting original instructions at {hx(address)}")
            asm.instructions.setdefault(address, ins)
        asm.labels.update(parsed.labels)
        asm.tables.update(parsed.tables)
    report = analyze_assembly(asm, catalogue)
    report["accepted_build"] = str(build.relative_to(root))
    report["catalogue"] = progress["denominator"]["path"]
    return report


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--root", type=Path, default=Path(__file__).resolve().parents[1])
    parser.add_argument("--build", type=Path)
    parser.add_argument("--symbol", nargs="+", action="extend", default=[])
    parser.add_argument("--json", type=Path)
    args = parser.parse_args(argv)
    start = time.perf_counter()
    report = analyze_project(args.root, args.build)
    elapsed = time.perf_counter() - start
    print(f"ABI facts: {report['summary']['functions']} functions, {report['summary']['instructions']} instructions in {elapsed:.6f} seconds", file=sys.stderr)
    if args.json:
        args.json.parent.mkdir(parents=True, exist_ok=True)
        args.json.write_text(json.dumps(report, indent=2, sort_keys=True) + "\n")
    if args.symbol:
        wanted = {symbol for group in args.symbol for symbol in group.split(",")}
        found = [row for row in report["functions"] if row["symbol"] in wanted]
        missing = wanted - {row["symbol"] for row in found}
        if missing:
            parser.error("uncatalogued symbols: " + ", ".join(sorted(missing)))
        print(json.dumps(found, indent=2, sort_keys=True))
    elif not args.json:
        print(json.dumps(report, indent=2, sort_keys=True))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
