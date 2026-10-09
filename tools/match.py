#!/usr/bin/env python3
"""Author loop: compile one candidate C or C++ file under an exact TU profile, link it at
the original address and compare its complete natural allocation with the ROM.

    uv run --frozen python tools/match.py func_800624F0 cand.c
    uv run --frozen python tools/match.py func_8002E810,func_8002E8A4 unit.c --profile gcc272-kmc26-O2-signed
    uv run --frozen python tools/match.py func_800FCE80 cand.cpp      # C++: gxx281pm-gnu291-O2-unsigned
    uv run --frozen python tools/match.py func_800624F0 cand.c --all-profiles
    uv run --frozen python tools/match.py func_800624F0 --asm        # original disassembly

The candidate is compiled exactly like a canonical TU (same driver, flags,
assembler and environment as `certification.object_commands`), with
`-I source/include` resolving to the canonical tree (or `--source-root`). Extra
`-I` directories come first, so a private header can shadow a canonical one.

The suffix selects the language: `.c` compiles with a C profile, `.cpp` with a C++ profile
(`language: c++`, cc1plus through the same 2.8.1 driver; the default for a `.cpp` main-image
candidate is `gxx281pm-gnu291-O2-unsigned`). A C++ candidate must define and reference only
extern "C" names (no g++ mangled symbols, vtables or typeinfo).

Gates checked here (the full-ROM build and `verify.py` remain the acceptance):
- the preprocessed translation unit contains no inline assembly, and no
  participating project file uses `##` or escaping includes; a C++ TU also uses no
  exception/RTTI construct, emits no `.eh_frame`/`.gcc_except_table`/`.gnu.linkonce.*`
  section, no C++-linkage symbol and no `.text` bytes outside its function symbols;
- `.text` is the only executable section and defines exactly the requested
  functions, at their original offsets with their original sizes;
- relocations are limited to R_MIPS_26/HI16/LO16 in text (R_MIPS_32 in data);
- the linker-resolved `.text` equals the ROM over the object's whole text size,
  which must fit before the next catalogued function (the natural slot);
- other allocated sections (rodata/data/bss/COMMON) are reported with the
  original address inferred from HI16/LO16 pairs and compared when initialized.
  Initialized data is adoptable (`match-with-data`) when it lies inside a YAML
  data/rodata subsegment or an adopt-made original-byte `bin` bridge of that
  section order. Uninitialized storage is linked exactly as the build
  links a C unit's BSS (`<obj>(.bss COMMON .scommon)` under the segment's
  SUBALIGN), at the inferred `.bss` address or, for COMMON only, at the accepted
  address of the tentative definition; the text comparison proves that
  placement. It is adoptable when the whole extent lies inside a loader-backed
  BSS range of `config/images.json`, inside one asm `bss` YAML subsegment (or
  the unit's own `.bss` carve). Global BSS definitions must retain their accepted
  addresses; labels referenced outside the unit or by its own externs must have
  such a definition. The plan holds only under the current parent SUBALIGN, so a
  unit whose initialized data is off that grid (its adoption changes the parent)
  is blocked too. Otherwise the result is `match-needs-data`, with the reasons
  in `bss_carve.blockers`.

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
from dataclasses import asdict, dataclass, field, replace
from pathlib import Path
from typing import Any

import yaml

from certification import CXX_FORBIDDEN_TOKEN, GXX_MANGLED, compiler_environment, profile_kind, profile_toolchain, read_json, source_file_violations, tool_profile
from evidence import COMPILED_SUFFIXES, compiled_kind
from images import load_inventory
from rom import CANONICAL_ROM, PROJECT

DEFAULT_PROFILES = {"resident": "gcc272-kmc26-O2-signed", "main_14400": "gcc272-modern237-O2-unsigned"}
# `.cpp` candidates: C++ is calibrated for the main image only.
CXX_DEFAULT_PROFILES = {"main_14400": "gxx281pm-gnu291-O2-unsigned"}
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
UNINITIALIZED = {".bss", ".sbss", "COMMON"}
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


def _yaml_segment(rom: int) -> tuple[dict[str, Any] | None, dict[str, Any]]:
    """The splat code segment containing ROM `rom`, and the global splat options."""
    config = yaml.safe_load((PROJECT / "config/shiren2.jp.yaml").read_text())
    segments = [s for s in config["segments"] if isinstance(s, dict) and "start" in s]
    owner = None
    for segment in sorted(segments, key=lambda s: s["start"]):
        if segment["start"] <= rom:
            owner = segment
    return owner, config.get("options", {})


def segment_subalign(rom: int) -> int | None:
    owner, _options = _yaml_segment(rom)
    return owner.get("subalign") if owner else None


def splat_subalign(segment: dict[str, Any], options: dict[str, Any]) -> int:
    """The SUBALIGN splat emits for a top-level segment's output sections (text, data and
    `.<segment>_bss`): its own `subalign`, else the global option (default 16)."""
    return segment.get("subalign", options.get("subalign", 16))


def bss_link_rules(rom: int) -> tuple[str, int, bool]:
    """(segment name, BSS SUBALIGN, bss_contains_common) of the splat segment holding text ROM `rom`.
    Splat emits `.<segment>_bss (NOLOAD) : SUBALIGN(n)` with n = `splat_subalign`."""
    owner, options = _yaml_segment(rom)
    if owner is None:
        raise ValueError(f"No splat code segment contains ROM 0x{rom:X}")
    return owner["name"], splat_subalign(owner, options), bool(owner.get("bss_contains_common", options.get("ld_bss_contains_common", False)))


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


def node_line_span(node: yaml.Node) -> tuple[int, int]:
    """Source lines [first, end) of one complete YAML subsegment node.

    A flow collection or scalar ends at its own end mark (a flow mapping's closing
    brace, possibly on a later line). A block collection's own end mark is the start of
    the next sibling, so its last child bounds it instead."""
    last = node
    while isinstance(last, (yaml.MappingNode, yaml.SequenceNode)) and not last.flow_style and last.value:
        last = last.value[-1][1] if isinstance(last, yaml.MappingNode) else last.value[-1]
    mark = last.end_mark
    return node.start_mark.line, mark.line + bool(mark.column)


@dataclass(frozen=True)
class DataPart:
    start: int
    end: int
    kind: str
    name: str
    preserved: bool
    segment: dict[str, Any]
    segment_end: int
    segment_index: int
    part_index: int


def yaml_data_parts(text: str) -> list[DataPart]:
    """Initialized data ownership, bounded by its own parent and the next subsegment.

    Only adopt's explicitly section-ordered original-byte bins are carveable;
    opaque/RSP bins and text padding are not initialized C data owners.
    """
    config = yaml.safe_load(text)
    segments = config["segments"]
    result = []
    for segment_index, segment in enumerate(segments[:-1]):
        if not isinstance(segment, dict) or segment.get("type") != "code":
            continue
        following = segments[segment_index + 1]
        segment_end = following[0] if isinstance(following, list) else following["start"]
        delta = segment["vram"] - segment["start"]
        parts = segment["subsegments"]
        starts = [part[0] if isinstance(part, list) else part.get("start", part.get("vram", segment_end + delta) - delta)
                  for part in parts]
        for index, part in enumerate(parts):
            if isinstance(part, list):
                if len(part) < 3:
                    continue
                start, kind, name = part[:3]
            else:
                start, kind, name = starts[index], part.get("type"), part.get("name")
            preserved = kind == "bin" and isinstance(part, dict)
            if preserved:
                section = part.get("linker_section_order")
                if (section not in {".data", ".rodata"}
                        or set(part) != {"start", "type", "name", "linker_section_order"}
                        or name != f"{segment['name']}_{section[1:]}_bytes_{start:x}"):
                    continue
                kind = section[1:]
            if kind not in {"data", "rodata", ".data", ".rodata"}:
                continue
            end = starts[index + 1] if index + 1 < len(starts) else segment_end
            if type(start) is not int or type(end) is not int or not segment["start"] <= start < end <= segment_end:
                raise ValueError(f"Invalid initialized data interval for {name}")
            result.append(DataPart(start, end, kind, name, preserved, segment, segment_end, segment_index, index))
    return result


def data_owner(rom: int) -> tuple[str, str] | None:
    """An initialized asm/C data owner or an explicit original-byte bridge."""
    parts = yaml_data_parts((PROJECT / "config/shiren2.jp.yaml").read_text())
    owner = next((part for part in parts if part.start <= rom < part.end), None)
    return (owner.kind, owner.name) if owner else None


@dataclass(frozen=True)
class BssPart:
    line: int
    end_line: int  # exclusive source line for replacing a complete mapping
    indent: str
    vram: int
    kind: str  # `bss` (asm storage) or `.bss` (a C unit's carve)
    name: str
    segment: str
    next_vram: int | None  # next BSS subsegment of the same segment; None = runs to the segment's BSS end


def yaml_bss_subsegments(lines: list[str]) -> list[BssPart]:
    """Parsed BSS boundaries and complete source spans, independent of key order/comments.

    Only direct, explicit dict-form BSS entries are editable. Unsupported layouts
    raise rather than disappearing from the ownership map and exposing their storage.
    """
    text = "\n".join(line.rstrip("\r\n") for line in lines) + "\n"
    loader = yaml.SafeLoader(text)

    def scalar(node: yaml.Node | None) -> Any:
        if not isinstance(node, yaml.ScalarNode):
            raise ValueError("BSS layout requires explicit scalar fields")
        return loader.construct_object(node)

    def mapping(node: yaml.Node | None) -> dict[str, yaml.Node]:
        if not isinstance(node, yaml.MappingNode):
            raise ValueError("BSS layout requires mapping declarations")
        fields = {}
        for key, value in node.value:
            if not isinstance(key, yaml.ScalarNode) or key.tag != "tag:yaml.org,2002:str":
                raise ValueError("BSS layout does not support non-string or merged mapping keys")
            if key.value in fields:
                raise ValueError(f"Duplicate YAML key {key.value!r} in BSS layout")
            fields[key.value] = value
        return fields

    def is_bss_list(node: yaml.SequenceNode) -> bool:
        return len(node.value) > 1 and scalar(node.value[1]) in {"bss", ".bss", "sbss", ".sbss"}

    try:
        root = loader.get_single_node()
        # Aliases reuse nodes and source marks. Editing one occurrence would also
        # change another owner (or remove its anchor), so no such layout is editable.
        pending, seen = [root], set()
        while pending:
            node = pending.pop()
            if id(node) in seen:
                raise ValueError("YAML aliases are not supported for BSS ownership edits")
            seen.add(id(node))
            if isinstance(node, yaml.MappingNode):
                pending.extend(item for pair in node.value for item in pair)
            elif isinstance(node, yaml.SequenceNode):
                pending.extend(node.value)
        segments = mapping(root).get("segments")
        if not isinstance(segments, yaml.SequenceNode):
            raise ValueError("BSS layout requires a segments list")
        result: list[BssPart] = []
        segment_names = set()
        for segment_node in segments.value:
            if isinstance(segment_node, yaml.SequenceNode):
                if is_bss_list(segment_node):
                    raise ValueError("Top-level/list-form BSS boundaries are not supported")
                continue
            segment = mapping(segment_node)
            if "type" in segment and scalar(segment["type"]) in {"bss", ".bss", "sbss", ".sbss"}:
                raise ValueError("BSS boundaries must be direct code subsegments")
            if "subsegments" not in segment:
                continue
            name = scalar(segment.get("name"))
            if not isinstance(name, str) or not name or name in segment_names:
                raise ValueError("BSS layout requires unique named parent segments")
            segment_names.add(name)
            subsegments = segment["subsegments"]
            if not isinstance(subsegments, yaml.SequenceNode):
                raise ValueError(f"Segment {name} requires an explicit subsegments list")
            parts: list[BssPart] = []
            for node in subsegments.value:
                if isinstance(node, yaml.SequenceNode):
                    if is_bss_list(node):
                        raise ValueError("List-form BSS boundaries require explicit vram mappings")
                    continue
                fields = mapping(node)
                if "subsegments" in fields:
                    raise ValueError("Nested subsegments are not supported for BSS ownership")
                kind = scalar(fields.get("type"))
                if kind in {"sbss", ".sbss"}:
                    raise ValueError(".sbss boundaries are not supported by the C-unit BSS rule")
                if kind not in {"bss", ".bss"}:
                    continue
                if set(fields) != {"type", "vram", "name"}:
                    raise ValueError("BSS boundaries require only explicit type, vram and name fields")
                vram, part_name = scalar(fields["vram"]), scalar(fields["name"])
                if type(vram) is not int or vram < 0 or not isinstance(part_name, str) or not part_name or "\n" in part_name:
                    raise ValueError("BSS boundaries require an integer vram and a nonempty single-line name")
                # The complete mapping: through a flow mapping's closing brace, or a block mapping's last field.
                line, end_line = node_line_span(node)
                prefix = lines[line][:node.start_mark.column]
                indent = re.fullmatch(r"(?P<indent> *)- +", prefix)
                if indent is None:
                    raise ValueError("BSS boundaries require their own block-sequence entry")
                if parts and vram <= parts[-1].vram:
                    raise ValueError(f"BSS boundaries in segment {name} are not strictly increasing")
                parts.append(BssPart(line, end_line, indent["indent"], vram, kind, part_name, name, None))
            result.extend(replace(part, next_vram=parts[i + 1].vram if i + 1 < len(parts) else None)
                          for i, part in enumerate(parts))
        return result
    except yaml.YAMLError as error:
        raise ValueError(f"Cannot parse BSS layout: {error}") from error
    finally:
        loader.dispose()


def bss_owner(lines: list[str], vram: int) -> BssPart | None:
    return max((p for p in yaml_bss_subsegments(lines) if p.vram <= vram), key=lambda p: (p.vram, p.line), default=None)


def bss_carve_blockers(lines: list[str], ranges: list[tuple[int, int]], start: int, tail: int, unit: str | None) -> tuple[list[str], BssPart | None]:
    """Why [start, tail) cannot be carved out of the YAML BSS layout (empty when it can).

    `ranges` are the image's loader-backed BSS ranges; `unit` is the unit's YAML name
    (`units/<image>/<stem>`), whose own `.bss` carve is an acceptable owner."""
    region = next(((low, high) for low, high in ranges if low <= start and tail <= high), None)
    if region is None:
        return [f"BSS 0x{start:X}-0x{tail:X} is not inside a loader-backed BSS range of the image"], None
    try:
        owner = bss_owner(lines, start)
    except ValueError as error:
        return [f"Cannot determine BSS ownership: {error}"], None
    if owner is None or owner.vram < region[0] or not (owner.kind == "bss" or (owner.kind == ".bss" and owner.name == unit and owner.vram == start)):
        return [f"BSS 0x{start:X} is not inside an asm bss subsegment (owner: {owner and (owner.kind, owner.name)})"], owner
    window_end = min(owner.next_vram, region[1]) if owner.next_vram is not None else region[1]
    if tail > window_end:
        return [f"BSS 0x{start:X}-0x{tail:X} crosses the next BSS subsegment at 0x{window_end:X}"], owner
    return [], owner


def external_bss_references(build: Path, source_root: Path, names: set[str], functions: list[str], source: Path) -> dict[str, list[str]]:
    """Files outside the unit mentioning `names`: the accepted build's generated asm (minus the
    unit's own function bodies, C-unit matchings and BSS label definitions), project C/C++ sources
    and headers, and `config/symbol_aliases.ld` alias targets."""
    if not names:
        return {}
    pattern = re.compile(r"(?<![\w$.])(" + "|".join(map(re.escape, sorted(names))) + r")(?![\w$])")
    bodies = [re.compile(rf"^\s*[gj]label {re.escape(f)}\s*$.*?^\s*endlabel {re.escape(f)}\s*$", re.MULTILINE | re.DOTALL) for f in functions]
    asm = build / "generated/asm"
    paths = [p for p in asm.rglob("*.s") if not p.name.endswith(".bss.s") and "matchings" not in p.relative_to(asm).parts]
    suffixes = {".h", *COMPILED_SUFFIXES.values()}
    paths += [p for root in (source_root / "src", source_root / "include") for p in root.rglob("*") if p.suffix in suffixes]
    found: dict[str, list[str]] = {}
    for path in paths:
        if path.resolve() == source.resolve():
            continue
        text = path.read_text(errors="replace")
        if not pattern.search(text):
            continue
        for body in bodies:
            text = body.sub("", text)
        for name in sorted(set(pattern.findall(text))):
            found.setdefault(name, []).append(str(path))
    aliases = source_root / "config/symbol_aliases.ld"
    if aliases.is_file():
        for _alias, target in ALIAS.findall(COMMENT.sub(" ", aliases.read_text())):
            if target in names:
                found.setdefault(target, []).append(str(aliases))
    return found


def bss_label_names(build: Path, start: int, end: int, known: dict[str, int]) -> set[str]:
    """Labels the accepted build's generated BSS assembly defines inside [start, end)."""
    names = set()
    for path in (build / "generated/asm/data").rglob("*.bss.s"):
        for name in re.findall(r"^\s*dlabel (\S+)\s*$", path.read_text(errors="replace"), re.MULTILINE):
            if start <= known.get(name, -1) < end:
                names.add(name)
    return names


def foreign_commons(build: Path, names: set[str], source: Path) -> dict[str, list[str]]:
    """Accepted-build C objects that also carry a COMMON definition of one of `names`.
    GNU ld allocates a common symbol in the first object that defines it, not in this unit."""
    if not names:
        return {}
    stem = None
    try:
        stem = source.resolve().relative_to((PROJECT / "src").resolve()).with_suffix("").as_posix()
    except ValueError:
        pass
    found: dict[str, list[str]] = {}
    root = build / "obj/source/src"
    for path in sorted(root.rglob("*.o")) if root.is_dir() else []:
        if stem and path.relative_to(root).with_suffix("").as_posix() == stem:
            continue
        for symbol in Elf(path.read_bytes()).symbols:
            if symbol.shndx == SHN_COMMON and symbol.name in names:
                found.setdefault(symbol.name, []).append(str(path.relative_to(build)))
    return found



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

def policy_violations(preprocessed: str, participating: list[Path], *, cxx: bool = False) -> list[str]:
    """Inline assembly anywhere in the expanded TU (and, for C++, exception/RTTI constructs),
    plus the canonical per-file source policy."""
    problems = []
    code = "\n".join(line for line in LITERAL.sub(" ", preprocessed).splitlines() if not line.startswith("#"))
    for token in sorted(set(ASM_TOKEN.findall(code))):
        problems.append(f"preprocessed translation unit contains '{token}'")
    if cxx:
        for token in sorted(set(CXX_FORBIDDEN_TOKEN.findall(code))):
            problems.append(f"preprocessed C++ translation unit contains exception/RTTI construct '{token}'")
    for path in participating:
        problems.extend(source_file_violations(path, str(path)))
    return problems


def cxx_object_problems(elf: Elf, text: Section) -> list[str]:
    """C++ object policy: extern "C" names only, no exception/vague-linkage sections, and no
    `.text` bytes or data symbols outside the function symbols (no raw blobs standing in for code)."""
    problems = []
    for section in elf.sections:
        if section.name in {".eh_frame", ".gcc_except_table"} or section.name.startswith(".gnu.linkonce."):
            problems.append(f"C++ section {section.name} (exception tables need -fno-exceptions; an out-of-line "
                            "inline function or vtable copy needs a non-inline key method or an inline definition before use)")
    mangled = sorted({s.name for s in elf.symbols if s.bind in {1, 2} and s.name and GXX_MANGLED.search(s.name)})
    if mangled:
        problems.append(f"C++-linkage symbols {mangled}: define and reference only extern \"C\" names")
    code = elf.contents(text)
    covered = bytearray(len(code))
    for symbol in elf.symbols:
        if symbol.shndx != text.index:
            continue
        if symbol.type == STT_FUNC:
            covered[symbol.value:symbol.value + symbol.size] = b"\1" * len(covered[symbol.value:symbol.value + symbol.size])
        elif symbol.type not in {0, STT_SECTION, STT_FILE}:
            problems.append(f"data symbol {symbol.name} in .text")
    if any(byte and not mark for byte, mark in zip(code, covered)):
        problems.append(".text contains nonzero bytes outside its function symbols")
    return problems


def default_profile(image_id: str, source: Path) -> str:
    """The image's measured default for the candidate's language (by suffix)."""
    if source.suffix == COMPILED_SUFFIXES["cpp"]:
        if image_id not in CXX_DEFAULT_PROFILES:
            raise ValueError(f"No calibrated C++ profile for {image_id}; pass --profile")
        return CXX_DEFAULT_PROFILES[image_id]
    return DEFAULT_PROFILES[image_id]


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
    bss_carve: dict[str, Any] | None = None
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


def _plan_bss_carve(result: Result, linked: Elf, image: Any, build: Path, source: Path, known: dict[str, int], commons: list[Symbol],
                    has_bss: bool, bss_start: int | None, segment: str, subalign: int, contains_common: bool,
                    source_root: Path) -> dict[str, Any]:
    """The YAML carve the unit's uninitialized storage needs, and every reason it cannot be adopted."""
    blockers = []
    if any(r["section"] == ".sbss" for r in result.data_sections):
        blockers.append(".sbss is not linked by splat's C-unit BSS rule (.bss COMMON .scommon)")
    if has_bss and bss_start is None:
        blockers.append(".bss address is not determined by the text's HI16/LO16 pairs")
    if len(commons) > 1:
        blockers.append("several COMMON symbols: GNU ld allocates them in link-hash-table order, which this link cannot "
                        "reproduce for the full ROM link; keep at most one tentative definition (extern/static the rest)")
    foreign = foreign_commons(build, {c.name for c in commons}, source)
    if foreign:
        blockers.append(f"accepted objects also tentatively define {foreign}; ld would allocate the storage there")
    # Initialized data off the parent's SUBALIGN grid is adopted only by changing that parent
    # (main 4 -> 1, see adopt.split_data_yaml); this plan assumes the current parent and would be stale.
    offgrid = [f"{r['section']} 0x{r['rom_start']:X}-0x{r['rom_start'] + r['size']:X}" for r in result.data_sections
               if "rom_start" in r and (r["rom_start"] % subalign or (r["rom_start"] + r["size"]) % subalign)]
    if offgrid:
        blockers.append(f"initialized data {offgrid} is not SUBALIGN({subalign})-aligned: adopting it changes the parent "
                        "alignment this BSS plan assumes; adopt the byte-granular data layout first, then rematch")
    carve: dict[str, Any] = {"segment": segment, "subalign": subalign, "vram": None, "end": None, "tail": None, "range_end": None,
                             "owner": None, "add_bss_contains_common": bool(commons) and not contains_common,
                             "global_symbols": [], "replaced_labels": [], "blockers": blockers}
    out = linked.section(".bss")
    if out is None or bss_start is None:
        return carve
    start, end = out.addr, out.addr + out.size
    tail = -(-end // subalign) * subalign  # the next input section of the image BSS is SUBALIGN-ed
    if start % subalign:
        blockers.append(f"BSS 0x{start:X} is not {subalign}-aligned; the segment SUBALIGN would move it")
    unit = None
    try:
        unit = source.resolve().relative_to((PROJECT / "src").resolve()).with_suffix("").as_posix()
    except ValueError:
        pass
    yaml_lines = (PROJECT / "config/shiren2.jp.yaml").read_text().splitlines()
    ranges = [(r.vram_start, r.vram_end) for r in image.bss_ranges]
    more, owner = bss_carve_blockers(yaml_lines, ranges, start, tail, unit)
    blockers += more
    if owner is not None and owner.segment != segment:
        blockers.append(f"BSS owner belongs to segment {owner.segment}, not the unit's segment {segment}")
    defined = {s.name: s.value for s in linked.symbols if s.name and s.bind == 1 and s.shndx == out.index}
    replacements = set()
    for name, address in sorted(defined.items()):
        if name not in known:
            blockers.append(f"global BSS symbol {name} has no accepted address (linked at 0x{address:X})")
        elif address != known[name]:
            blockers.append(f"global BSS symbol {name} is at 0x{address:X}, not its accepted address 0x{known[name]:X}")
        else:
            replacements.add(name)
    labels = bss_label_names(build, start, tail, known)
    external = external_bss_references(build, source_root, labels - replacements, result.symbols, source)
    if external:
        blockers.append(f"labels inside the carve are referenced outside the unit without global definitions at their accepted addresses: {external}")
    # The unit's own extern references resolve here through link.ld assignments (absolute symbols);
    # in the ROM link the carve's labels no longer exist, so they would be undefined.
    own = sorted((labels - replacements) & {s.name for s in linked.symbols if s.shndx == SHN_ABS})
    if own:
        blockers.append(f"the unit references labels inside its own carve without defining them: {own}")
    carve.update({"vram": start, "end": end, "tail": tail, "range_end": next((high for low, high in ranges if low <= start < high), None),
                  "owner": owner and {"kind": owner.kind, "name": owner.name, "vram": owner.vram},
                  "global_symbols": sorted(defined), "replaced_labels": sorted(labels)})
    return carve


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
    shutil.copyfile(source, out_dir / f"candidate{source.suffix}")
    result.source = {"path": str(source), "sha256": _sha256(source)}
    link = out_dir / "source"
    if link.is_symlink() or link.exists():
        link.unlink()
    link.symlink_to(source_root.resolve(), target_is_directory=True)

    profile = tool_profile(PROJECT, source_root)
    if profile_id not in profile["profiles"]:
        raise ValueError(f"Unknown profile {profile_id}; choose from {', '.join(profile['profiles'])}")
    assigned = profile["profiles"][profile_id]
    kind = compiled_kind(source.name)  # the driver selects the C or C++ front end by suffix
    if profile_kind(profile, profile_id) != kind and not diagnostic:
        raise ValueError(f"Profile {profile_id} does not compile {source.suffix} sources; use a {'C++' if kind == 'cpp' else 'C'} profile")
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
        result.problems += policy_violations(preprocessed, participating, cxx=kind == "cpp")
        run([*gcc, *flags, "-S", str(source), "-o", "candidate.s"], "compile")
        run([assembler, *asflags, "-o", "candidate.o", "candidate.s"], "assemble")

        elf = Elf((out_dir / "candidate.o").read_bytes())
        text = elf.section(".text")
        if text is None or text.type != SHT_PROGBITS or not text.flags & SHF_EXECINSTR:
            raise ValueError("Object has no executable .text")
        result.text_bytes = text.size
        if kind == "cpp":
            result.problems += cxx_object_problems(elf, text)
            if result.problems:
                result.status = "policy"  # a later unresolved mangled reference stays a policy result
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
        segment_name, bss_subalign, contains_common = bss_link_rules(unit_rom)
        bss = elf.section(".bss")
        bss = bss if bss is not None and bss.size else None
        for k, section in enumerate(s for s in allocated if s.index != text.index and s is not bss):
            estimates = sorted(set(bases.get(section.index, [])))
            address = estimates[0] if len(estimates) == 1 else 0x90000000 + k * 0x100000
            placed[section.name] = address
            noload = " (NOLOAD)" if section.type == SHT_NOBITS else ""
            lines.append(f"  {section.name} 0x{address:X}{noload} : {{ candidate.o({section.name}) }}")
            result.data_sections.append({"section": section.name, "size": section.size, "addralign": section.addralign,
                                         "vram": address if len(estimates) == 1 else None,
                                         "address_estimates": [hex(e) for e in estimates]})
        bss_start: int | None = None
        if bss is not None or commons:
            missing = [s.name for s in commons if s.name not in known]
            if missing:
                raise ValueError(f"COMMON symbols absent from the accepted build: {missing}")
            if bss is not None:
                estimates = sorted(set(bases.get(bss.index, [])))
                bss_start = estimates[0] if len(estimates) == 1 else None
                result.data_sections.append({"section": ".bss", "size": bss.size, "addralign": bss.addralign, "vram": bss_start,
                                             "address_estimates": [hex(e) for e in estimates]})
            else:
                # Tentative definitions alone: the unit's BSS starts where the accepted link has them.
                bss_start = min(known[s.name] for s in commons)
            # Exactly the build's C-unit BSS rule: `<obj>(.bss COMMON .scommon)` under the segment SUBALIGN.
            address = bss_start if bss_start is not None else 0x9F000000
            lines.append(f"  .bss 0x{address:X} (NOLOAD) : SUBALIGN({bss_subalign}) {{ candidate.o(.bss COMMON .scommon) }}")
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
            names = {c.name for c in commons}
            placed_commons = {s.name: s.value for s in linked.symbols if s.name in names and s.shndx != SHN_UNDEF}
            result.data_sections.append({"section": "COMMON", "size": sum(s.size for s in commons),
                                         "addralign": max(s.value for s in commons),  # st_value of a COMMON symbol is its alignment
                                         "vram": min(placed_commons.values()) if placed_commons else None,
                                         "symbols": {s.name: hex(known[s.name]) for s in commons},
                                         "linked": {name: hex(value) for name, value in sorted(placed_commons.items())},
                                         "matches_original": all(placed_commons.get(s.name) == known[s.name] for s in commons)})
        if any(r["section"] in UNINITIALIZED for r in result.data_sections):
            result.bss_carve = _plan_bss_carve(result, linked, image, build, source, known, commons, bss is not None, bss_start,
                                               segment_name, bss_subalign, contains_common, source_root)

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
        elif any(r["section"] not in UNINITIALIZED and (r.get("vram") is None or r["vram"] % max(r.get("addralign", 1), 1))
                 for r in result.data_sections):
            result.status = "match-needs-data"
        elif not all(data_owner(r["rom_start"]) for r in result.data_sections if "rom_start" in r):
            result.status = "match-needs-data"
        elif result.bss_carve is not None and result.bss_carve["blockers"]:
            result.status = "match-needs-data"
        elif result.data_sections:
            result.status = "match-with-data"
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
    carve = result.bss_carve
    if carve:
        if carve["vram"] is not None:
            owner = carve["owner"]
            body.append(f"  bss carve 0x{carve['vram']:X}-0x{carve['end']:X} (tail 0x{carve['tail']:X}, SUBALIGN {carve['subalign']}) "
                        f"in {owner['kind'] + ' ' + owner['name'] if owner else 'no owner'}"
                        + (", adds bss_contains_common" if carve["add_bss_contains_common"] else ""))
        body += [f"  bss blocker: {blocker}" for blocker in carve["blockers"]]
    body += [f"  problem: {problem}" for problem in result.problems]
    body.append(f"  evidence: {result.out_dir}/result.json ({result.elapsed_seconds}s)")
    return "\n".join([head, *body])


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("functions", help="symbol, or comma-separated catalogue-adjacent symbols for a joint unit")
    parser.add_argument("source", type=Path, nargs="?", help="candidate C (.c) or C++ (.cpp) file")
    parser.add_argument("--image", choices=sorted(DEFAULT_PROFILES))
    parser.add_argument("--profile", help="profile id from config/compiler_profiles.json (default per image and language)")
    parser.add_argument("--all-profiles", action="store_true", help="try every declared profile of the candidate's language")
    parser.add_argument("-I", dest="includes", type=Path, action="append", default=[], help="extra include directory (searched first)")
    parser.add_argument("--out", type=Path, help="evidence directory (default scratch/omp/match/<symbol>/<time>)")
    parser.add_argument("--source-root", type=Path, default=PROJECT, help="project tree providing source/include and profiles")
    parser.add_argument("--asm", action="store_true", help="print the original generated assembly and exit")
    parser.add_argument("--m2c", action="store_true", help="print an unverified m2c starting point and exit")
    parser.add_argument("--json", action="store_true", help="print result JSON instead of the summary")
    parser.add_argument("--no-diff", action="store_true", help="do not print the diff listing")
    parser.add_argument("--explain", action="store_true", help="diagnose register/scheduling differences from isolated GCC RTL dumps; never receipt evidence")
    research = parser.add_argument_group("toolchain research (results are never adoptable)")
    research.add_argument("--diag-gcc", help="alternate gcc driver path")
    research.add_argument("--diag-cflags", default="", help="extra compiler flags, space separated")
    research.add_argument("--diag-as", help="alternate assembler path")
    research.add_argument("--diag-asflags", help="replacement assembler flags, space separated")
    args = parser.parse_args()
    if args.explain and (args.asm or args.m2c or args.diag_gcc or args.diag_cflags or args.diag_as or args.diag_asflags is not None):
        parser.error("--explain requires a normal pinned-profile candidate comparison, not --asm/--m2c/--diag-*")
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
        if args.all_profiles:
            declared = read_json(args.source_root / "config/compiler_profiles.json")["profiles"]
            language = "c++" if args.source.suffix == COMPILED_SUFFIXES["cpp"] else "c"
            profiles = sorted(key for key, item in declared.items() if item.get("language", "c") == language)
        else:
            profiles = [args.profile or default_profile(targets[0].image_id, args.source)]
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
            payload = asdict(result)
            explanation = None
            if args.explain:
                from explain import collect, render
                explanation = collect(result, targets, args.source, profile_id, out_dir, includes=args.includes,
                                      source_root=args.source_root, rom=rom)
                payload["explanation"] = explanation
            print(json.dumps(payload, indent=2) if args.json else summary(result))
            if explanation is not None and not args.json:
                print(render(explanation))
            if diff and not args.no_diff and not args.json:
                print(diff)
    except (ValueError, OSError, KeyError) as error:
        parser.exit(2, f"error: {error}\n")
    adoptable = {"match", "match-with-data"}
    sys.exit(0 if adoptable & set(statuses) else 2 if all(s in {"policy", "error"} for s in statuses) else 1)


if __name__ == "__main__":
    main()
