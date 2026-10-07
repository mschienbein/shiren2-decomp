#!/usr/bin/env python3
"""Author loop: compile one candidate C file under an exact TU profile, link it at
the original address and compare its complete natural allocation with the ROM.

    uv run --frozen python tools/match.py func_800624F0 cand.c
    uv run --frozen python tools/match.py func_8002E810,func_8002E8A4 unit.c --profile gcc272-kmc26-O2-signed
    uv run --frozen python tools/match.py func_800624F0 cand.c --all-profiles
    uv run --frozen python tools/match.py func_800624F0 --asm        # original disassembly

The candidate is compiled exactly like a canonical TU (same driver, flags,
assembler and environment as `certification.object_commands`), with
`-I source/include` resolving to the canonical tree (or `--source-root`). Extra
`-I` directories come first, so a private header can shadow a canonical one.

Gates checked here (the full-ROM build and `verify.py` remain the acceptance):
- the preprocessed translation unit contains no inline assembly, and no
  participating project file uses `##` or escaping includes;
- `.text` is the only executable section and defines exactly the requested
  functions, at their original offsets with their original sizes;
- relocations are limited to R_MIPS_26/HI16/LO16 in text (R_MIPS_32 in data);
- the linker-resolved `.text` equals the ROM over the object's whole text size,
  which must fit before the next catalogued function (the natural slot);
- other allocated sections (rodata/data/bss/COMMON) are reported with the
  original address inferred from HI16/LO16 pairs and compared when initialized.
  Main-image C cannot own data until the main data carrier is split, so such a
  result is `match-needs-data`, not `match`.

Exit status: 0 match, 1 mismatch/overflow/needs-data, 2 policy violation or error.
Nothing here edits canonical source/config or grants credit.
"""
from __future__ import annotations

import argparse
import difflib
import hashlib
import json
import re
import shutil
import struct
import subprocess
import os
import sys
import time
from dataclasses import asdict, dataclass, field
from pathlib import Path
from typing import Any

import yaml

from certification import compiler_environment, profile_toolchain, read_json, source_file_violations, tool_profile
from images import load_inventory
from rom import CANONICAL_ROM, PROJECT

DEFAULT_PROFILES = {"resident": "gcc272-kmc26-O2-signed", "main_14400": "gcc272-modern237-O2-unsigned"}
R_MIPS_32, R_MIPS_26, R_MIPS_HI16, R_MIPS_LO16 = 2, 4, 5, 6
TEXT_RELOCATIONS = {R_MIPS_26: "R_MIPS_26", R_MIPS_HI16: "R_MIPS_HI16", R_MIPS_LO16: "R_MIPS_LO16"}
DATA_RELOCATIONS = {R_MIPS_32: "R_MIPS_32", **TEXT_RELOCATIONS}
SHF_WRITE, SHF_ALLOC, SHF_EXECINSTR = 1, 2, 4
SHT_PROGBITS, SHT_SYMTAB, SHT_NOBITS, SHT_REL = 1, 2, 8, 9
SHN_UNDEF, SHN_ABS, SHN_COMMON = 0, 0xFFF1, 0xFFF2
STT_FUNC, STT_SECTION, STT_FILE = 2, 3, 4
# Sections the assembler always emits that never reach the ROM.
IGNORED_SECTIONS = {".reginfo", ".MIPS.abiflags", ".pdr", ".rel.pdr", ".mdebug", ".comment", ".note",
                    ".gnu.attributes", ".symtab", ".strtab", ".shstrtab", ""}
DATA_SECTION_NAMES = {".data", ".rodata", ".rdata", ".sdata", ".bss", ".sbss"}
ALIAS = re.compile(r"PROVIDE\(\s*([A-Za-z_.$][\w.$]*)\s*=\s*([A-Za-z_.$][\w.$]*|0x[0-9A-Fa-f]+)\s*\)\s*;")
COMMENT = re.compile(r"/\*.*?\*/", re.DOTALL)
ASM_TOKEN = re.compile(r"\b(?:asm|__asm|__asm__|INCLUDE_ASM|GLOBAL_ASM|INCLUDE_RODATA)\b")
LITERAL = re.compile(r'"(?:\\.|[^"\\\n])*"|\'(?:\\.|[^\'\\\n])*\'')
LINEMARKER = re.compile(r'^# \d+ "([^"]+)"', re.MULTILINE)


# ---------------------------------------------------------------- ELF reading

@dataclass(frozen=True)
class Section:
    index: int
    name: str
    type: int
    flags: int
    addr: int
    offset: int
    size: int
    link: int
    info: int
    addralign: int


@dataclass(frozen=True)
class Symbol:
    name: str
    value: int
    size: int
    bind: int
    type: int
    shndx: int


class Elf:
    """Minimal ELF32 big-endian reader: sections, symbols and REL records."""

    def __init__(self, data: bytes) -> None:
        if data[:6] != b"\x7fELF\x01\x02" or struct.unpack_from(">H", data, 18)[0] != 8:
            raise ValueError("Expected an ELF32 big-endian MIPS file")
        self.data = data
        shoff, = struct.unpack_from(">I", data, 0x20)
        shentsize, shnum, shstrndx = struct.unpack_from(">HHH", data, 0x2E)
        raw = [struct.unpack_from(">10I", data, shoff + i * shentsize) for i in range(shnum)]
        names = raw[shstrndx]
        self.sections = [Section(i, self._string(names[4], r[0]), r[1], r[2], r[3], r[4], r[5], r[6], r[7], r[8])
                         for i, r in enumerate(raw)]
        self.symbols: list[Symbol] = []
        for section in self.sections:
            if section.type == SHT_SYMTAB:
                strings = self.sections[section.link].offset
                for i in range(section.size // 16):
                    name, value, size, info, _other, shndx = struct.unpack_from(">IIIBBH", data, section.offset + i * 16)
                    self.symbols.append(Symbol(self._string(strings, name), value, size, info >> 4, info & 15, shndx))

    def _string(self, table: int, offset: int) -> str:
        end = self.data.index(b"\0", table + offset)
        return self.data[table + offset:end].decode("latin-1")

    def section(self, name: str) -> Section | None:
        found = [s for s in self.sections if s.name == name]
        if len(found) > 1:
            raise ValueError(f"Duplicate section {name}")
        return found[0] if found else None

    def contents(self, section: Section) -> bytes:
        return b"\0" * section.size if section.type == SHT_NOBITS else self.data[section.offset:section.offset + section.size]

    def relocations(self, target: Section) -> list[tuple[int, int, Symbol]]:
        result = []
        for section in self.sections:
            if section.type == SHT_REL and section.info == target.index:
                for i in range(section.size // 8):
                    offset, info = struct.unpack_from(">II", self.data, section.offset + i * 8)
                    result.append((offset, info & 0xFF, self.symbols[info >> 8]))
        return sorted(result, key=lambda item: item[0])


# ------------------------------------------------------------ project lookups

@dataclass(frozen=True)
class Target:
    image_id: str
    symbol: str
    rom_start: int
    vram_start: int
    size: int
    slot_end: int  # ROM offset of the next catalogued function (or CPU range end)


def catalogue_functions() -> list[dict[str, Any]]:
    pointer = read_json(PROJECT / "docs/progress.json")["denominator"]
    return read_json(PROJECT / pointer["path"])["identity"]["functions"]


def resolve_targets(symbols: list[str], image_id: str | None = None) -> list[Target]:
    """Catalogue entries for one function or a contiguous run of functions."""
    inventory = load_inventory(PROJECT / "config/images.json")
    functions = sorted(catalogue_functions(), key=lambda f: (f["image_id"], f["rom_start"]))
    found: list[int] = []
    for name in symbols:
        hits = [i for i, f in enumerate(functions) if f["symbol"] == name and (image_id is None or f["image_id"] == image_id)]
        if len(hits) != 1:
            raise ValueError(f"{name}: {'ambiguous; pass --image' if hits else 'not in the function catalogue'}")
        found.append(hits[0])
    if len({functions[i]["image_id"] for i in found}) != 1 or found != list(range(found[0], found[0] + len(found))):
        raise ValueError("A multi-function unit must list catalogue-adjacent functions of one image in ROM order")
    targets = []
    for i in found:
        f = functions[i]
        image = inventory.images[f["image_id"]]
        if i + 1 < len(functions) and functions[i + 1]["image_id"] == f["image_id"]:
            end = functions[i + 1]["rom_start"]
        else:
            end = next(r.rom_end for r in image.provisional_cpu_ranges if r.rom_start <= f["rom_start"] < r.rom_end)
        targets.append(Target(f["image_id"], f["symbol"], f["rom_start"], f["vram_start"], f["size"], end))
    return targets


def segment_subalign(rom: int) -> int | None:
    config = yaml.safe_load((PROJECT / "config/shiren2.jp.yaml").read_text())
    segments = [s for s in config["segments"] if isinstance(s, dict) and "start" in s]
    owner = None
    for segment in sorted(segments, key=lambda s: s["start"]):
        if segment["start"] <= rom:
            owner = segment
    return owner.get("subalign") if owner else None

SUBSEGMENT = re.compile(r"^(?P<indent>\s*)- (?:\[0x(?P<list_start>[0-9A-Fa-f]+), (?P<list_kind>[.\w]+), (?P<list_name>[^\]]+)\]"
                        r"|\{start: 0x(?P<dict_start>[0-9A-Fa-f]+), type: (?P<dict_kind>[.\w]+), name: (?P<dict_name>[^,}]+).*\})\s*$")


def yaml_subsegments(lines: list[str]) -> list[tuple[int, int, str, str]]:
    """(line index, ROM start, kind, name) for every list/dict ROM subsegment line of the splat YAML."""
    result = []
    for index, line in enumerate(lines):
        found = SUBSEGMENT.match(line)
        if found:
            start = found["list_start"] or found["dict_start"]
            result.append((index, int(start, 16), found["list_kind"] or found["dict_kind"], (found["list_name"] or found["dict_name"]).strip()))
    return result


def data_owner(rom: int) -> tuple[str, str] | None:
    """The YAML data subsegment owning ROM `rom`: an asm `data`/`rodata` range, or a C unit's
    own `.data`/`.rodata` carve (the state after `tools/adopt.py` has adopted it)."""
    parts = yaml_subsegments((PROJECT / "config/shiren2.jp.yaml").read_text().splitlines())
    owner = max((p for p in parts if p[1] <= rom), key=lambda p: (p[1], p[0]), default=None)
    return (owner[2], owner[3]) if owner and owner[2] in {"data", "rodata", ".data", ".rodata"} else None



def accepted_build() -> Path:
    pointer = read_json(PROJECT / "build/latest.json")
    return (PROJECT / pointer["receipt"]).parent


def symbol_addresses(elf_path: Path) -> dict[str, int]:
    """Defined symbols of the accepted link (asm labels, linker-script D_ symbols, C)."""
    elf = Elf(elf_path.read_bytes())
    result: dict[str, int] = {}
    for symbol in sorted(elf.symbols, key=lambda s: s.bind != 1):  # globals win
        if symbol.name and symbol.shndx != SHN_UNDEF and symbol.type not in {STT_SECTION, STT_FILE}:
            result.setdefault(symbol.name, symbol.value)
    return result


def original_asm(symbol: str, build: Path) -> str:
    # A catalogued function start can be a jump-table target (`jlabel`) once rodata is split.
    pattern = re.compile(rf"^\s*[gj]label {re.escape(symbol)}\s*$(.*?)^\s*endlabel {re.escape(symbol)}\s*$", re.MULTILINE | re.DOTALL)
    for path in sorted((build / "generated/asm").rglob("*.s")):
        text = path.read_text(errors="replace")
        if f"label {symbol}\n" in text:
            found = pattern.search(text)
            if found:
                return f"# {path.relative_to(build)}\nglabel {symbol}{found[1]}endlabel {symbol}\n"
    raise ValueError(f"No generated assembly for {symbol} in {build}")


# ----------------------------------------------------------- source policy

def policy_violations(preprocessed: str, participating: list[Path]) -> list[str]:
    """Inline assembly anywhere in the expanded TU, plus the canonical per-file source policy."""
    problems = []
    code = "\n".join(line for line in LITERAL.sub(" ", preprocessed).splitlines() if not line.startswith("#"))
    for token in sorted(set(ASM_TOKEN.findall(code))):
        problems.append(f"preprocessed translation unit contains '{token}'")
    for path in participating:
        problems.extend(source_file_violations(path, str(path)))
    return problems


# ---------------------------------------------------------------- matching

@dataclass
class Result:
    status: str
    profile_id: str
    image_id: str
    symbols: list[str]
    rom_start: int
    vram_start: int
    instruction_bytes: int
    slot_bytes: int
    text_bytes: int | None = None
    leftover_gap_bytes: int | None = None
    differing_words: int | None = None
    first_difference: int | None = None
    source: dict[str, str] = field(default_factory=dict)
    participating_files: list[dict[str, str]] = field(default_factory=list)
    relocations: dict[str, int] = field(default_factory=dict)
    data_sections: list[dict[str, Any]] = field(default_factory=list)
    problems: list[str] = field(default_factory=list)
    out_dir: str = ""
    elapsed_seconds: float = 0.0


def _sha256(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def _sext16(value: int) -> int:
    return value - 0x10000 if value & 0x8000 else value


def _infer_section_bases(elf: Elf, text: Section, original: bytes) -> dict[int, list[int]]:
    """Original base address of each local data section, from HI16/LO16 pairs.

    REL addends live in the instruction fields; the original ROM instructions at
    the same offsets give the address the pair must produce.
    """
    code = elf.contents(text)
    def word(data: bytes, offset: int) -> int:
        return struct.unpack_from(">I", data, offset)[0]

    candidates: dict[int, list[int]] = {}
    last_hi: dict[int, int] = {}
    for offset, kind, symbol in elf.relocations(text):
        if symbol.shndx in {SHN_UNDEF, SHN_ABS, SHN_COMMON} or symbol.shndx == text.index:
            continue
        if kind == R_MIPS_HI16:
            last_hi[symbol.shndx] = offset
        elif kind == R_MIPS_LO16 and symbol.shndx in last_hi and offset + 4 <= len(original):
            hi = last_hi[symbol.shndx]
            addend = ((word(code, hi) & 0xFFFF) << 16) + _sext16(word(code, offset) & 0xFFFF)
            target = ((word(original, hi) & 0xFFFF) << 16) + _sext16(word(original, offset) & 0xFFFF)
            candidates.setdefault(symbol.shndx, []).append((target - symbol.value - addend) & 0xFFFFFFFF)
    return candidates


def _disassemble(objdump: str, data: bytes, vram: int, work: Path, name: str) -> list[tuple[str, str]]:
    path = work / f"{name}.bin"
    path.write_bytes(data)
    output = subprocess.run([objdump, "-D", "-b", "binary", "-m", "mips:4300", "-EB", "-M", "reg-names=32",
                             f"--adjust-vma=0x{vram:X}", str(path)], capture_output=True, text=True, check=True).stdout
    lines = []
    for line in output.splitlines():
        found = re.match(r"\s*([0-9a-f]+):\s+([0-9a-f]{8})\s+(.*)$", line)
        if found:
            lines.append((found[2], re.sub(r"\s+", " ", found[3]).strip()))
    return lines


def render_diff(original: list[tuple[str, str]], candidate: list[tuple[str, str]], vram: int) -> str:
    """Side-by-side listing aligned on mnemonics; '*' marks a differing word."""
    def mnemonic(item: tuple[str, str]) -> str:
        return item[1].split(" ")[0]

    rows = []
    matcher = difflib.SequenceMatcher(None, [mnemonic(i) for i in original], [mnemonic(i) for i in candidate], autojunk=False)
    for _tag, a0, a1, b0, b1 in matcher.get_opcodes():
        for k in range(max(a1 - a0, b1 - b0)):
            left = original[a0 + k] if a0 + k < a1 else None
            right = candidate[b0 + k] if b0 + k < b1 else None
            mark = " " if left and right and left[0] == right[0] else ("r" if left and right and mnemonic(left) == mnemonic(right) else "*")
            address = f"{vram + 4 * (a0 + k):08X}" if left else " " * 8
            rows.append(f"{mark} {address}  {(left[0] + ' ' + left[1]) if left else '':<44} | {(right[0] + ' ' + right[1]) if right else ''}")
    return "\n".join(rows) + "\n"


def check(targets: list[Target], source: Path, profile_id: str, out_dir: Path, *, includes: list[Path] = (),
          source_root: Path = PROJECT, build: Path | None = None, rom: bytes | None = None,
          diagnostic: dict[str, Any] | None = None) -> tuple[Result, str]:
    """Compile, link and compare one candidate. Returns the result and a diff listing.

    `diagnostic` may override `gcc` (driver path), `extra_cflags`, `assembler`
    (path) and `asflags` for toolchain research. Such results are labelled
    `<profile>+diagnostic` and are never adoptable.
    """
    started = time.monotonic()
    first, last = targets[0], targets[-1]
    unit_rom, unit_vram = first.rom_start, first.vram_start
    slot = last.slot_end - unit_rom
    result = Result("error", profile_id, first.image_id, [t.symbol for t in targets], unit_rom, unit_vram,
                    sum(t.size for t in targets), slot, out_dir=str(out_dir))
    out_dir.mkdir(parents=True, exist_ok=True)
    rom = rom if rom is not None else CANONICAL_ROM.read_bytes()
    original = rom[unit_rom:last.slot_end]
    build = build or accepted_build()
    source = source.resolve()
    shutil.copyfile(source, out_dir / "candidate.c")
    result.source = {"path": str(source), "sha256": _sha256(source)}
    link = out_dir / "source"
    if link.is_symlink() or link.exists():
        link.unlink()
    link.symlink_to(source_root.resolve(), target_is_directory=True)

    profile = tool_profile(PROJECT, source_root)
    if profile_id not in profile["profiles"]:
        raise ValueError(f"Unknown profile {profile_id}; choose from {', '.join(profile['profiles'])}")
    assigned = profile["profiles"][profile_id]
    gcc, assembler = profile_toolchain(profile, profile_id)
    asflags = assigned["assembler_flags"]
    flags = [item for path in includes for item in ("-I", str(path.resolve()))] + assigned["compiler_flags"]
    (out_dir / "tmp").mkdir(exist_ok=True)
    environment = compiler_environment(out_dir, profile)
    if diagnostic:
        result.profile_id = f"{profile_id}+diagnostic"
        if diagnostic.get("gcc"):
            gcc = [str(Path(diagnostic["gcc"]).resolve())]
            environment["COMPILER_PATH"] = str(Path(gcc[0]).parent)
        flags += diagnostic.get("extra_cflags", [])
        assembler = str(Path(diagnostic["assembler"]).resolve()) if diagnostic.get("assembler") else assembler
        asflags = diagnostic["asflags"] if diagnostic.get("asflags") is not None else asflags
        (out_dir / "diagnostic.json").write_text(json.dumps(diagnostic, indent=2) + "\n")
    diff = ""

    def run(argv: list[str], log: str) -> subprocess.CompletedProcess[str]:
        completed = subprocess.run(argv, cwd=out_dir, env=environment, capture_output=True, text=True)
        (out_dir / f"{log}.log").write_text(json.dumps(argv) + f"\nexit {completed.returncode}\n" + completed.stdout + completed.stderr)
        if completed.returncode != 0:
            raise ValueError(f"{log} failed (exit {completed.returncode}):\n{completed.stderr.strip()}")
        return completed

    try:
        preprocessed = run([*gcc, *flags, "-E", str(source)], "preprocess").stdout
        (out_dir / "candidate.i").write_text(preprocessed)
        participating = sorted({Path(p).resolve() for p in LINEMARKER.findall(preprocessed)
                                if not p.startswith("<") and Path(p).is_file()})
        result.participating_files = [{"path": str(p), "sha256": _sha256(p)} for p in participating]
        result.problems += policy_violations(preprocessed, participating)
        run([*gcc, *flags, "-S", str(source), "-o", "candidate.s"], "compile")
        run([assembler, *asflags, "-o", "candidate.o", "candidate.s"], "assemble")

        elf = Elf((out_dir / "candidate.o").read_bytes())
        text = elf.section(".text")
        if text is None or text.type != SHT_PROGBITS or not text.flags & SHF_EXECINSTR:
            raise ValueError("Object has no executable .text")
        result.text_bytes = text.size
        # KMC as 2.6 emits macro-generated `.rodata` (li.d constants) without SHF_ALLOC; it still links.
        allocated = [s for s in elf.sections if s.size and s.name not in IGNORED_SECTIONS
                     and (s.flags & SHF_ALLOC or s.name in DATA_SECTION_NAMES)]
        for section in allocated:
            if section.flags & SHF_EXECINSTR and section.index != text.index:
                result.problems.append(f"extra executable section {section.name}")
        functions = {s.name: s for s in elf.symbols if s.type == STT_FUNC or (s.shndx == text.index and s.type == 0 and s.bind == 1)}
        if set(functions) != set(result.symbols):
            result.problems.append(f"object defines functions {sorted(functions)}, expected exactly {result.symbols}")
        for target in targets:
            symbol = functions.get(target.symbol)
            if symbol and (symbol.shndx != text.index or symbol.value != target.rom_start - unit_rom or symbol.size != target.size):
                result.problems.append(f"{target.symbol}: object offset/size {symbol.value:#x}/{symbol.size} != original {target.rom_start - unit_rom:#x}/{target.size}")
        for section in [text, *allocated]:
            allowed = TEXT_RELOCATIONS if section.index == text.index else DATA_RELOCATIONS
            for _offset, kind, _symbol in elf.relocations(section):
                name = allowed.get(kind, f"type{kind}")
                result.relocations[name] = result.relocations.get(name, 0) + 1
                if kind not in allowed:
                    result.problems.append(f"unsupported relocation {name} in {section.name}")
        commons = [s for s in elf.symbols if s.shndx == SHN_COMMON]

        # Link at the original address with the accepted build's symbol values.
        bases = _infer_section_bases(elf, text, original)
        known = symbol_addresses(build / "shiren2.elf")
        aliases = source_root / "config/symbol_aliases.ld"
        if aliases.is_file():
            for alias, target in ALIAS.findall(COMMENT.sub(" ", aliases.read_text())):
                if target.startswith("0x"):
                    known.setdefault(alias, int(target, 16))
                elif target in known:
                    known.setdefault(alias, known[target])
        lines = ["SECTIONS {", f"  .text 0x{unit_vram:X} : {{ candidate.o(.text) }}"]
        placed: dict[str, int] = {}
        for k, section in enumerate(s for s in allocated if s.index != text.index):
            estimates = sorted(set(bases.get(section.index, [])))
            address = estimates[0] if len(estimates) == 1 else 0x90000000 + k * 0x100000
            placed[section.name] = address
            noload = " (NOLOAD)" if section.type == SHT_NOBITS else ""
            lines.append(f"  {section.name} 0x{address:X}{noload} : {{ candidate.o({section.name}) }}")
            result.data_sections.append({"section": section.name, "size": section.size, "addralign": section.addralign,
                                         "vram": address if len(estimates) == 1 else None,
                                         "address_estimates": [hex(e) for e in estimates]})
        if commons:
            # Tentative definitions own BSS; place them where the accepted link has them.
            missing = [s.name for s in commons if s.name not in known]
            if missing:
                raise ValueError(f"COMMON symbols absent from the accepted build: {missing}")
            common_base = min(known[s.name] for s in commons)
            lines.append(f"  .common 0x{common_base:X} (NOLOAD) : {{ candidate.o(COMMON) }}")
        lines += ["  /DISCARD/ : { *(*) }", "}"]
        unresolved = []
        for name in sorted({s.name for s in elf.symbols if s.shndx == SHN_UNDEF and s.name}):
            if name in known:
                lines.append(f"{name} = 0x{known[name]:08X};")
            else:
                unresolved.append(name)
        if unresolved:
            raise ValueError(f"Symbols not defined by the accepted build: {unresolved}")
        (out_dir / "link.ld").write_text("\n".join(lines) + "\n")
        run([profile["tools"]["ld"]["path"], "-m", profile["ld_emulation"], "-T", "link.ld", "-e", "0",
             "-o", "candidate.elf", "candidate.o"], "link")
        linked = Elf((out_dir / "candidate.elf").read_bytes())
        code = linked.contents(linked.section(".text"))

        image = load_inventory(PROJECT / "config/images.json").images[first.image_id]
        for record in result.data_sections:
            section = linked.section(record["section"])
            if record["vram"] is None or section.type == SHT_NOBITS:
                # Placement of uninitialized storage is proven by the text comparison.
                record["matches_original"] = None if record["vram"] is None else True
                continue
            offset = record["vram"] - image.vram_rom_delta
            record["rom_start"] = offset
            record["matches_original"] = image.rom_start <= offset and offset + section.size <= image.rom_end and rom[offset:offset + section.size] == linked.contents(section)
        if commons:
            placed_commons = {s.name: s.value for s in linked.symbols if s.name in {c.name for c in commons}}
            result.data_sections.append({"section": "COMMON", "size": sum(s.size for s in commons),
                                         "vram": min(placed_commons.values()) if placed_commons else None,
                                         "symbols": {s.name: hex(known[s.name]) for s in commons},
                                         "matches_original": all(placed_commons.get(s.name) == known[s.name] for s in commons)})

        subalign = segment_subalign(unit_rom)
        if subalign is None and unit_vram % max(text.addralign, 1):
            result.problems.append(f".text alignment {text.addralign} would move vram 0x{unit_vram:X}")
        compared = original[:len(code)]
        result.differing_words = sum(1 for i in range(0, min(len(code), len(compared)), 4) if code[i:i + 4] != compared[i:i + 4])
        result.first_difference = next((i for i in range(0, min(len(code), len(compared)), 4) if code[i:i + 4] != compared[i:i + 4]), None)
        result.leftover_gap_bytes = slot - len(code)
        if result.problems:
            result.status = "policy"
        elif len(code) > slot:
            result.status = "overflow"
        elif code != compared:
            result.status = "mismatch"
        elif not all(r["matches_original"] for r in result.data_sections):
            result.status = "mismatch-data"
        elif any(r["section"] in {"COMMON", ".bss", ".sbss"} or r.get("vram") is None or r["vram"] % max(r.get("addralign", 1), 1)
                 for r in result.data_sections):
            result.status = "match-needs-data"
        elif result.data_sections:
            owners = [data_owner(r["rom_start"]) for r in result.data_sections if "rom_start" in r]
            result.status = "match-with-data" if all(owners) else "match-needs-data"
        else:
            result.status = "match"
        if result.status != "match":
            objdump = profile["tools"]["objdump"]["path"]
            span = max(len(code), min(slot, len(original)))
            diff = render_diff(_disassemble(objdump, original[:span], unit_vram, out_dir, "original"),
                               _disassemble(objdump, code, unit_vram, out_dir, "candidate"), unit_vram)
            (out_dir / "diff.txt").write_text(diff)
    except ValueError as error:
        result.problems.append(str(error))
        result.status = "policy" if result.status == "policy" else "error"
    result.elapsed_seconds = round(time.monotonic() - started, 3)
    (out_dir / "result.json").write_text(json.dumps(asdict(result), indent=2) + "\n")
    return result, diff


def summary(result: Result) -> str:
    head = (f"{result.status.upper()}: {','.join(result.symbols)} [{result.image_id} ROM 0x{result.rom_start:X} "
            f"VRAM 0x{result.vram_start:X}] profile {result.profile_id}")
    body = [f"  instruction bytes {result.instruction_bytes}, natural slot {result.slot_bytes}, object text {result.text_bytes}"
            + (f", leftover gap {result.leftover_gap_bytes}" if result.leftover_gap_bytes is not None else "")]
    if result.differing_words:
        body.append(f"  {result.differing_words} differing word(s); first at +0x{result.first_difference:X}")
    for record in result.data_sections:
        body.append(f"  data {record['section']} {record['size']} bytes at {hex(record['vram']) if record['vram'] is not None else 'unknown'}"
                    f" (matches original: {record.get('matches_original')})")
    body += [f"  problem: {problem}" for problem in result.problems]
    body.append(f"  evidence: {result.out_dir}/result.json ({result.elapsed_seconds}s)")
    return "\n".join([head, *body])


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("functions", help="symbol, or comma-separated catalogue-adjacent symbols for a joint unit")
    parser.add_argument("source", type=Path, nargs="?", help="candidate C file")
    parser.add_argument("--image", choices=sorted(DEFAULT_PROFILES))
    parser.add_argument("--profile", help="profile id from config/compiler_profiles.json (default per image)")
    parser.add_argument("--all-profiles", action="store_true", help="try every declared profile")
    parser.add_argument("-I", dest="includes", type=Path, action="append", default=[], help="extra include directory (searched first)")
    parser.add_argument("--out", type=Path, help="evidence directory (default scratch/omp/match/<symbol>/<time>)")
    parser.add_argument("--source-root", type=Path, default=PROJECT, help="project tree providing source/include and profiles")
    parser.add_argument("--asm", action="store_true", help="print the original generated assembly and exit")
    parser.add_argument("--m2c", action="store_true", help="print an unverified m2c starting point and exit")
    parser.add_argument("--json", action="store_true", help="print result JSON instead of the summary")
    parser.add_argument("--no-diff", action="store_true", help="do not print the diff listing")
    research = parser.add_argument_group("toolchain research (results are never adoptable)")
    research.add_argument("--diag-gcc", help="alternate gcc driver path")
    research.add_argument("--diag-cflags", default="", help="extra compiler flags, space separated")
    research.add_argument("--diag-as", help="alternate assembler path")
    research.add_argument("--diag-asflags", help="replacement assembler flags, space separated")
    args = parser.parse_args()
    # Author runs yield CPU to the integrator's builds and verification (inherited by compilers).
    try:
        os.nice(10)
    except OSError:  # already at a high niceness (macOS refuses to exceed the limit)
        pass
    try:
        targets = resolve_targets(args.functions.split(","), args.image)
        if args.asm or args.m2c:
            build = accepted_build()
            for target in targets:
                text = original_asm(target.symbol, build)
                if args.asm:
                    print(text)
                    continue
                path = PROJECT / "scratch/omp/cache" / f"{target.symbol}.s"
                path.parent.mkdir(parents=True, exist_ok=True)
                path.write_text(".text\n" + re.sub(r"^\s*jlabel (\S+)\s*$", r"\1:", text, flags=re.MULTILINE))
                completed = subprocess.run([str(Path(sys.prefix) / "bin/m2c"), "--target", "mips-gcc-c", "-f", target.symbol, str(path)],
                                           capture_output=True, text=True)
                print(completed.stdout + completed.stderr)
            return
        if args.source is None:
            parser.error("a candidate source is required unless --asm/--m2c is given")
        matched = {m["symbol"] for m in read_json(PROJECT / "config/matches.json")["functions"]}
        if matched & {t.symbol for t in targets}:
            print(f"note: already matched in config/matches.json: {sorted(matched & {t.symbol for t in targets})}", file=sys.stderr)
        profiles = sorted(read_json(args.source_root / "config/compiler_profiles.json")["profiles"]) if args.all_profiles \
            else [args.profile or DEFAULT_PROFILES[targets[0].image_id]]
        base = args.out or PROJECT / "scratch/omp/match" / targets[0].symbol / time.strftime("%Y%m%dT%H%M%S")
        rom = CANONICAL_ROM.read_bytes()
        statuses = []
        for profile_id in profiles:
            out_dir = base / profile_id if len(profiles) > 1 else base
            diagnostic = {"gcc": args.diag_gcc, "extra_cflags": args.diag_cflags.split(), "assembler": args.diag_as,
                          "asflags": args.diag_asflags.split() if args.diag_asflags is not None else None}
            diagnostic = diagnostic if any(diagnostic.values()) else None
            result, diff = check(targets, args.source, profile_id, out_dir, includes=args.includes,
                                 source_root=args.source_root, rom=rom, diagnostic=diagnostic)
            statuses.append(result.status)
            print(json.dumps(asdict(result), indent=2) if args.json else summary(result))
            if diff and not args.no_diff and not args.json:
                print(diff)
    except (ValueError, OSError, KeyError) as error:
        parser.exit(2, f"error: {error}\n")
    adoptable = {"match", "match-with-data"}
    sys.exit(0 if adoptable & set(statuses) else 2 if all(s in {"policy", "error"} for s in statuses) else 1)


if __name__ == "__main__":
    main()
