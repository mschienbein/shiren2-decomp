"""Operand-checked MIPS III encoding evidence, not an assembler/disassembler.

Options describe individual words a source instruction can emit, not a sequence
proof. Unrecognized syntax fails closed. Only relocation-bearing fields may be
wildcards; all concrete operands and reserved fields remain checked.
"""
from __future__ import annotations

import re

Options = tuple[tuple[int, int], ...]
_FULL = 0xFFFFFFFF
_GPRS = dict(zip(
    "zero at v0 v1 a0 a1 a2 a3 t0 t1 t2 t3 t4 t5 t6 t7 s0 s1 s2 s3 s4 s5 s6 s7 t8 t9 k0 k1 gp sp fp ra".split(),
    range(32),
))
_GPRS["s8"] = 30
_SYMBOL = re.compile(r"(?:[A-Za-z_.][A-Za-z0-9_.$]*|[0-9]+[fb])(?:[+-](?:0[xX][0-9a-fA-F]+|[0-9]+))?\Z")
_R3 = {"add": 0x20, "addu": 0x21, "sub": 0x22, "subu": 0x23,
       "and": 0x24, "or": 0x25, "xor": 0x26, "nor": 0x27,
       "slt": 0x2A, "sltu": 0x2B, "dadd": 0x2C, "daddu": 0x2D,
       "dsub": 0x2E, "dsubu": 0x2F}
_IMM = {"addi": 8, "addiu": 9, "slti": 10, "sltiu": 11,
        "andi": 12, "ori": 13, "xori": 14, "daddi": 24, "daddiu": 25}
_SHIFT = {"sll": 0, "srl": 2, "sra": 3, "dsll": 0x38,
          "dsrl": 0x3A, "dsra": 0x3B, "dsll32": 0x3C,
          "dsrl32": 0x3E, "dsra32": 0x3F}
_VARIABLE = {"sllv": 4, "srlv": 6, "srav": 7,
             "dsllv": 0x14, "dsrlv": 0x16, "dsrav": 0x17}
_MEMORY = {"lb": 32, "lh": 33, "lwl": 34, "lw": 35, "lbu": 36,
           "lhu": 37, "lwr": 38, "lwu": 39, "sb": 40, "sh": 41,
           "swl": 42, "sw": 43, "sdl": 44, "sdr": 45, "swr": 46,
           "ll": 48, "lwc1": 49, "lld": 52, "ldc1": 53, "ld": 55,
           "sc": 56, "swc1": 57, "scd": 60, "sdc1": 61, "sd": 63,
           "ldl": 26, "ldr": 27}


def _reg(text: str, fp: bool = False) -> int:
    if fp:
        match = re.fullmatch(r"\$f([0-9]+)", text)
        if match and int(match[1]) < 32:
            return int(match[1])
    elif text.startswith("$"):
        name = text[1:]
        if name in _GPRS:
            return _GPRS[name]
        if name.isascii() and name.isdecimal() and int(name) < 32:
            return int(name)
    raise ValueError("Unsupported register")


def _number(text: str) -> int:
    if not re.fullmatch(r"[+-]?(?:0[xX][0-9a-fA-F]+|[0-9]+)", text):
        raise ValueError("Unsupported integer")
    # Never misread gas's leading-zero octal spelling as decimal. Python's
    # base-zero parser rejects those unsupported spellings instead.
    return int(text, 0)


def _immediate(text: str, unsigned: bool = False, relocation: str | None = None) -> int | None:
    if relocation:
        match = re.fullmatch(r"%" + relocation + r"\((.+)\)", text)
        if match and _SYMBOL.fullmatch(match[1]):
            return None
    value = _number(text)
    if not ((0 <= value <= 65535) if unsigned else (-32768 <= value <= 32767)):
        raise ValueError("Immediate out of range")
    return value & 0xFFFF


def _r(function: int, rd: int = 0, rs: int = 0, rt: int = 0, shift: int = 0) -> tuple[int, int]:
    return _FULL, rs << 21 | rt << 16 | rd << 11 | shift << 6 | function


def _i(opcode: int, rt: int, rs: int, immediate: int | None) -> tuple[int, int]:
    return (_FULL if immediate is not None else 0xFFFF0000,
            opcode << 26 | rs << 21 | rt << 16 | (immediate or 0))


def _li(rt: int, value: int) -> Options:
    if not -0x80000000 <= value <= 0xFFFFFFFF:
        raise ValueError("Only 32-bit li is supported")
    signed = value if value < 0x80000000 else value - 0x100000000
    choices = []
    if -32768 <= signed <= 32767:
        choices.append(_i(9, rt, 0, signed & 0xFFFF))
    if 0 <= value <= 65535:
        choices.append(_i(13, rt, 0, value))
    if choices:
        return tuple(choices)
    bits = value & _FULL
    choices.append(_i(15, rt, 0, bits >> 16))
    if bits & 65535:
        choices.append(_i(13, rt, rt, bits & 65535))
    return tuple(choices)


def _address_parts(text: str) -> tuple[str, int]:
    match = re.fullmatch(r"(.*)\((\$[^()]*)\)", text)
    if match:
        return match[1] or "0", _reg(match[2])
    return text, 0


def _memory(mnemonic: str, target: str, address: str) -> Options:
    fp = mnemonic.endswith("c1")
    rt = _reg(target, fp)
    displacement, base = _address_parts(address)
    opcode = _MEMORY[mnemonic]
    try:
        immediate = _immediate(displacement, relocation="lo")
        return (_i(opcode, rt, base, immediate),)
    except ValueError:
        pass
    symbolic = bool(_SYMBOL.fullmatch(displacement))
    if symbolic:
        high = low = None
    else:
        value = _number(displacement)
        if not -0x80000000 <= value <= 0x7FFFFFFF:
            raise ValueError("Unsupported address")
        high, low = ((value + 0x8000) >> 16) & 65535, value & 65535
    # Integer loads use their destination as the temporary unless it is also
    # the address base. Stores and FP transfers must preserve their source.
    load = mnemonic in {"lb", "lh", "lw", "lbu", "lhu", "lwu", "ld", "ll", "lld"}
    scratch = rt if load and rt not in (0, base) else 1
    if scratch == base or (not load and not fp and rt == scratch):
        raise ValueError("Macro would clobber a live operand")
    choices = [_i(15, scratch, 0, high)]
    if base:
        choices.append(_r(0x21, scratch, scratch, base))
    choices.append(_i(opcode, rt, scratch, low))
    return tuple(choices)


def encoding_options(assembly: str) -> Options | None:
    """Return mask/value alternatives for one line, or None when unsupported.

    Symbolic jump/branch targets are relocations; numeric control-transfer
    targets need a PC and are deliberately unsupported. Complex assembler
    expressions, PIC/GOT macros, trap-generating division macros, and 64-bit
    constant synthesis are likewise not evidence this parser can prove.
    """
    text = assembly.split("#", 1)[0].strip()
    parts = text.split(None, 1)
    if not parts:
        return None
    mnemonic = parts[0]
    args = [part.strip() for part in parts[1].split(",")] if len(parts) == 2 else []
    if any(not arg for arg in args):
        return None
    try:
        return _encoding(mnemonic, args)
    except (ValueError, KeyError):
        return None


def _encoding(m: str, a: list[str]) -> Options | None:
    if m == "nop" and not a:
        return ((_FULL, 0),)
    if m in _R3 and len(a) == 3:
        rd, rs = _reg(a[0]), _reg(a[1])
        try:
            rt = _reg(a[2])
        except ValueError:
            if m in {"sub", "subu", "dsub", "dsubu"}:
                immediate = -_number(a[2])
                if not -32768 <= immediate <= 32767:
                    return None
                opcode = {"sub": 8, "subu": 9, "dsub": 24, "dsubu": 25}[m]
                return (_i(opcode, rd, rs, immediate & 0xFFFF),)
            macro = {"add": "addi", "addu": "addiu", "dadd": "daddi", "daddu": "daddiu",
                     "and": "andi", "or": "ori", "xor": "xori", "slt": "slti", "sltu": "sltiu"}
            im = macro[m]
            return (_i(_IMM[im], rd, rs, _immediate(a[2], im in {"andi", "ori", "xori"})),)
        return (_r(_R3[m], rd, rs, rt),)
    if m in _SHIFT and len(a) == 3:
        shift = _number(a[2])
        if not 0 <= shift <= 31:
            return None
        return (_r(_SHIFT[m], _reg(a[0]), 0, _reg(a[1]), shift),)
    if m in _VARIABLE and len(a) == 3:
        return (_r(_VARIABLE[m], _reg(a[0]), _reg(a[2]), _reg(a[1])),)
    if m == "move" and len(a) == 2:
        return tuple(_r(fn, _reg(a[0]), _reg(a[1])) for fn in (0x21, 0x25, 0x2D))
    if m in {"neg", "negu", "dneg", "dnegu", "not"} and len(a) == 2:
        fn = {"neg": 0x22, "negu": 0x23, "dneg": 0x2E, "dnegu": 0x2F, "not": 0x27}[m]
        return (_r(fn, _reg(a[0]), _reg(a[1]) if m == "not" else 0,
                   0 if m == "not" else _reg(a[1])),)
    if m in _IMM and len(a) == 3:
        return (_i(_IMM[m], _reg(a[0]), _reg(a[1]),
                   _immediate(a[2], m in {"andi", "ori", "xori"}, "lo")),)
    if m == "lui" and len(a) == 2:
        return (_i(15, _reg(a[0]), 0, _immediate(a[1], True, "hi")),)
    if m == "li" and len(a) == 2:
        return _li(_reg(a[0]), _number(a[1]))
    if m == "la" and len(a) == 2:
        rt = _reg(a[0])
        offset, base = _address_parts(a[1])
        if _SYMBOL.fullmatch(offset):
            if base:
                return None
            return (_i(15, rt, 0, None), _i(9, rt, rt, None))
        if base:
            return (_i(9, rt, base, _immediate(offset)),)
        return _li(rt, _number(offset))
    if m in _MEMORY and len(a) == 2:
        return _memory(m, a[0], a[1])
    if m in {"j", "jr", "jal", "jalr"}:
        if len(a) == 1:
            if a[0].startswith("$"):
                return (_r(9 if m in {"jal", "jalr"} else 8,
                           31 if m in {"jal", "jalr"} else 0, _reg(a[0])),)
            if m in {"j", "jal"} and _SYMBOL.fullmatch(a[0]):
                return ((0xFC000000, (3 if m == "jal" else 2) << 26),)
        if m == "jalr" and len(a) == 2:
            return (_r(9, _reg(a[0]), _reg(a[1])),)
        return None
    branches = {"beq": 4, "bne": 5, "beql": 20, "bnel": 21}
    if m in branches and len(a) == 3 and _SYMBOL.fullmatch(a[2]):
        return (_i(branches[m], _reg(a[1]), _reg(a[0]), None),)
    if m in {"beqz", "bnez", "beqzl", "bnezl"} and len(a) == 2:
        return _encoding({"beqz": "beq", "bnez": "bne", "beqzl": "beql", "bnezl": "bnel"}[m], [a[0], "$0", a[1]])
    if m in {"b", "bal"} and len(a) == 1 and _SYMBOL.fullmatch(a[0]):
        return (_i(4 if m == "b" else 1, 0 if m == "b" else 17, 0, None),)
    regimm = {"bltz": 0, "bgez": 1, "bltzl": 2, "bgezl": 3,
              "bltzal": 16, "bgezal": 17, "bltzall": 18, "bgezall": 19}
    if m in regimm and len(a) == 2 and _SYMBOL.fullmatch(a[1]):
        return (_i(1, regimm[m], _reg(a[0]), None),)
    zero_branch = {"blez": 6, "bgtz": 7, "blezl": 22, "bgtzl": 23}
    if m in zero_branch and len(a) == 2 and _SYMBOL.fullmatch(a[1]):
        return (_i(zero_branch[m], 0, _reg(a[0]), None),)
    if m in {"mfhi", "mflo", "mthi", "mtlo"} and len(a) == 1:
        fn = {"mfhi": 16, "mthi": 17, "mflo": 18, "mtlo": 19}[m]
        reg = _reg(a[0])
        return (_r(fn, reg if m.startswith("mf") else 0, reg if m.startswith("mt") else 0),)
    muldiv = {"mult": 24, "multu": 25, "div": 26, "divu": 27,
              "dmult": 28, "dmultu": 29, "ddiv": 30, "ddivu": 31}
    if m in muldiv:
        if "div" in m:
            if len(a) != 3 or _reg(a[0]) != 0:
                return None
            a = a[1:]
        if len(a) == 2:
            return (_r(muldiv[m], 0, _reg(a[0]), _reg(a[1])),)
        return None
    transfers = {"mfc0": (16, 0), "dmfc0": (16, 1), "mtc0": (16, 4), "dmtc0": (16, 5),
                 "mfc1": (17, 0), "dmfc1": (17, 1), "cfc1": (17, 2),
                 "mtc1": (17, 4), "dmtc1": (17, 5), "ctc1": (17, 6)}
    if m in transfers and len(a) == 2:
        opcode, rs = transfers[m]
        # Control registers are numbered, not FPRs or GPR aliases.
        if m.endswith("0") or m in {"cfc1", "ctc1"}:
            if not re.fullmatch(r"\$[0-9]+", a[1]):
                return None
            rd = _reg(a[1])
        else:
            rd = _reg(a[1], True)
        return ((_FULL, opcode << 26 | rs << 21 | _reg(a[0]) << 16 | rd << 11),)
    if m in {"bc1f", "bc1t", "bc1fl", "bc1tl"} and len(a) == 1 and _SYMBOL.fullmatch(a[0]):
        return (_i(17, {"bc1f": 0, "bc1t": 1, "bc1fl": 2, "bc1tl": 3}[m], 8, None),)
    return _floating(m, a)


def _floating(m: str, a: list[str]) -> Options | None:
    parts = m.split(".")
    if len(parts) < 2 or parts[-1] not in {"s", "d", "w", "l"}:
        return None
    fmt = {"s": 16, "d": 17, "w": 20, "l": 21}[parts[-1]]
    prefix = 17 << 26 | fmt << 21
    binary = {"add": 0, "sub": 1, "mul": 2, "div": 3}
    unary = {"sqrt": 4, "abs": 5, "mov": 6, "neg": 7}
    if len(parts) == 2 and fmt in {16, 17}:
        if parts[0] in binary and len(a) == 3:
            return ((_FULL, prefix | _reg(a[2], True) << 16 | _reg(a[1], True) << 11 |
                     _reg(a[0], True) << 6 | binary[parts[0]]),)
        if parts[0] in unary and len(a) == 2:
            return ((_FULL, prefix | _reg(a[1], True) << 11 | _reg(a[0], True) << 6 | unary[parts[0]]),)
    if len(parts) == 3 and parts[0] == "c" and fmt in {16, 17} and len(a) == 2:
        conditions = "f un eq ueq olt ult ole ule sf ngle seq ngl lt nge le ngt".split()
        if parts[1] in conditions:
            return ((_FULL, prefix | _reg(a[1], True) << 16 | _reg(a[0], True) << 11 |
                     0x30 | conditions.index(parts[1])),)
    if len(parts) == 3 and len(a) == 2:
        op, dest, source = parts
        fn = None
        if op == "cvt" and dest in {"s", "d", "w", "l"} and dest != source:
            if source in {"s", "d"} or dest in {"s", "d"}:
                fn = {"s": 32, "d": 33, "w": 36, "l": 37}[dest]
        if op in {"round", "trunc", "ceil", "floor"} and dest in {"w", "l"} and source in {"s", "d"}:
            fn = (12 if dest == "w" else 8) + {"round": 0, "trunc": 1, "ceil": 2, "floor": 3}[op]
        if fn is not None:
            return ((_FULL, prefix | _reg(a[1], True) << 11 | _reg(a[0], True) << 6 | fn),)
    return None
