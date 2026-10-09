#!/usr/bin/env python3
"""Integrator: adopt matched candidates into canonical source and configuration.

    uv run --frozen python tools/adopt.py func_800624F0 cand.c [--profile ID] [-I dir] [--dry-run]
    uv run --frozen python tools/adopt.py func_800FCE80 cand.cpp    # C++ TU, gxx281pm-gnu291-O2-unsigned
    uv run --frozen python tools/adopt.py func_80056D60,func_80056D70 joint.c --absorb func_80056D60
    uv run --frozen python tools/adopt.py --batch adopt.json [--dry-run]

Each unit is re-checked with `tools/match.py` and must be an exact text match
(`match`). The candidate's suffix selects its language: a `.cpp` candidate becomes a
C++ TU (`src/units/<image>/<unit>.cpp`, a `cpp` splat subsegment, a C++ profile; the
default is the image's C++ profile), a `.c` candidate a C TU. Before anything is copied,
`tools/interfaces.py` audits the function interfaces of every canonical TU (C and C++)
together with the batch candidates; a unit is
refused if its source introduces a finding over the canonical tree plus the units
adopted before it (`--dry-run` reports it the same way). Adoption then:
- copies the source to `src/units/<image>/<unit>.c` (or `.cpp`); candidate-local headers go
  beside it, headers found through `-I` go to `include/` (replacing an existing
  canonical header requires `--replace-header include/<name>`);
- appends `config/matches.json` rows, the TU profile and the image binding;
- splits the owning `asm` subsegment in `config/shiren2.jp.yaml` into
  `c`/`cpp` + (zero padding `bin` if the object leaves a gap) + `asm` tail; refused
  when the parent SUBALIGN would move the unit, its padding or the tail (each must
  start on a SUBALIGN multiple; resident `main` and main_14400 are SUBALIGN(1));
- carves owned storage (re-check status `match-with-data`): `.data`/`.rodata` at its
  exact natural extent out of an unowned data/rodata subsegment or an adopt-generated
  original-byte `bin` bridge (short bridges keep neighbouring bytes; main_14400 may switch
  its parent `subalign: 4` to `1` for byte placement; resident `main` is already
  `subalign: 1` by reviewed configuration), `.bss`/COMMON as `{type: .bss, vram: ...,
  name: units/<image>/<unit>}` (+ a `bss` tail) out of the owning asm `bss` subsegment,
  enabling `bss_contains_common` on the segment when the unit has COMMON. A BSS plan
  is used only under the parent SUBALIGN it was computed with; a unit whose data carve
  changes that parent is refused (adopt the byte-granular data layout first, then
  rematch);
- re-checks the canonical copy without extra include paths (including its BSS plan),
  and restores every touched file if anything fails.

`--absorb SYMBOL` (repeatable; batch key `absorbs`, a list of symbols) lets a candidate
replace already adopted units, e.g. a joint TU of adjacent functions sharing one
initialized datum. Each symbol selects its whole adopted unit (its TU), which must lie
entirely inside the candidate: every function of it listed by the candidate and its
ROM range inside the candidate's span; any other adopted function in the span is still
refused. Within the same rollback, the absorbed units' `config/matches.json` rows, TU
profiles and image bindings are removed; their layout returns to the neighbouring
form before the normal split/carve (the `c`/`cpp` subsegment with its padding `bin`
and own asm tail, each `.data`/`.rodata` carve with its own original-byte bridges and
data tail, the `.bss` carve with its own `bss` tail: merged into a preceding unowned
subsegment of that kind, else one named like an adopt tail, `<prefix>_<address>`); their
interior PROVIDE aliases are dropped (the new carve regenerates what it needs); their
sources are removed unless the candidate reuses the path; the interface audit omits
their TUs. Adopting A and then a joint with `absorbs: [A]` therefore yields the same
configuration and sources as adopting the joint without A. Not reverted: a parent
`subalign: 1` transition, `bss_contains_common`, and candidate-local headers. A data
carve whose bridges another unit has since reused is refused; the BSS plan still comes
from `tools/match.py` against the current layout, so a joint re-owning an absorbed
unit's BSS is refused as `match-needs-data`.

`--dry-run` executes the same sequential adoption and canonical-copy rechecks in a
temporary source snapshot, then discards it. Later candidates see earlier successful
source/header/configuration changes, including bridges released by a re-carve with
`absorbs`. The printed diff is cumulative (including source/header additions and
removals), not a concatenation of intermediate layouts. Original source inputs are
not written. Match receipts/logs are retained under `scratch/omp/adopt/` before the
snapshot is discarded, and reported evidence paths point to those retained copies.
Failures roll back their own unit in the snapshot just as in a real batch;
successful earlier units remain visible to later candidates.

A batch file is a JSON list of objects with keys `functions` (list or comma
string), `source`, optional `profile`, `includes`, `replace_headers`, `name`,
`evidence`, `absorbs` (list or comma string). Adoption grants no credit: build, verify,
review and promote next.
"""
from __future__ import annotations

import argparse
import difflib
import json
import os
import re
import shutil
import subprocess
import sys
import tempfile
import time
from dataclasses import asdict, dataclass, field
from pathlib import Path
from typing import Any

import yaml

from evidence import COMPILED_SUFFIXES
from interfaces import Gate, analyse, collect
from match import (SHN_COMMON, SHT_NOBITS, SUBSEGMENT, UNINITIALIZED, DataPart, Elf, accepted_build, bss_owner, check,
                   default_profile, node_line_span, resolve_targets, splat_subalign, symbol_addresses, yaml_bss_subsegments,
                   yaml_data_parts, yaml_subsegments)
from rom import CANONICAL_ROM, PROJECT
from workspace import SOURCE_DIRECTORIES, create_snapshot, input_manifest, verify_manifest

CONFIG_FILES = ("config/matches.json", "config/compiler_profiles.json", "config/images.json", "config/shiren2.jp.yaml",
                "config/symbol_aliases.ld")
TAIL_PREFIX = {"resident": "resident_tail", "main_14400": "main_14400_tail"}
DATA_TAIL_PREFIX = {("resident", "data"): "resident_tail", ("resident", "rodata"): "resident_tail", ("resident", "bss"): "resident_tail",
                    ("main_14400", "data"): "main_data", ("main_14400", "rodata"): "main_rodata", ("main_14400", "bss"): "main_bss"}
# Section alignment of the asm piece left after a data carve (measured on the generated objects).
TAIL_ALIGNMENT = {("resident", "data"): 16, ("resident", "rodata"): 16, ("main_14400", "data"): 4, ("main_14400", "rodata"): 8}


@dataclass
class Unit:
    functions: list[str]
    source: Path
    profile: str | None = None
    includes: list[Path] = field(default_factory=list)
    replace_headers: list[str] = field(default_factory=list)
    name: str | None = None
    evidence: str | None = None
    absorbs: list[str] = field(default_factory=list)


def _owner(lines: list[str], rom: int) -> tuple[int, int, str, str] | None:
    return max((p for p in yaml_subsegments(lines) if p[1] <= rom), key=lambda p: (p[1], p[0]), default=None)


def _carve(text: str, start: int, end: int, owner_kinds: set[str], new: list[str], tail_prefix: str,
           bss_end: int | None = None) -> str:
    """Insert `new` subsegment lines for [start, end) into the owning subsegment, plus a tail if space remains.

    Owners are the ROM-addressed subsegments or, given `bss_end` (the loader-backed BSS end of
    the segment), the vram-addressed `{type: bss, vram: ..., name: ...}` subsegments; the last
    BSS subsegment of a segment runs to `bss_end`. BSS tails are named after the vram's low
    24 bits (`resident_tail_412d0`). Parsed YAML source spans preserve or replace
    whole BSS mappings, including multiline declarations."""
    lines = text.splitlines(keepends=True)
    part = None
    if bss_end is None:
        owner = _owner(lines, start)
        limit = next((p[1] for p in yaml_subsegments(lines) if owner and p[0] > owner[0]), None)
    else:
        part = bss_owner(lines, start)
        owner = part and (part.line, part.vram, part.kind, part.name)
        limit = part and (bss_end if part.next_vram is None else part.next_vram)
    if owner is None or owner[2] not in owner_kinds:
        raise ValueError(f"{'ROM' if bss_end is None else 'BSS'} 0x{start:X} is not inside a {'/'.join(sorted(owner_kinds))} "
                         f"subsegment (owner: {owner})")
    if limit is None or limit < end:
        raise ValueError(f"Next subsegment after {owner[3]} starts before the unit's natural end 0x{end:X}")
    indent = part.indent if part is not None else SUBSEGMENT.match(lines[owner[0]])["indent"]
    owner_end = part.end_line if part is not None else owner[0] + 1
    rendered = [f"{indent}- {line}\n" for line in new]
    if end < limit:
        tail = f"{tail_prefix}_{(end if bss_end is None else end & 0xFFFFFF):x}"
        if re.search(rf"(?<![\w/]){re.escape(tail)}(?!\w)", text):
            raise ValueError(f"Tail subsegment name {tail} already exists")
        rendered.append(f"{indent}- [0x{end:X}, {owner[2]}, {tail}]\n" if bss_end is None
                        else f"{indent}- {{type: {owner[2]}, vram: 0x{end:X}, name: {tail}}}\n")
    if owner[1] == start:
        lines[owner[0]:owner_end] = rendered
    else:
        lines[owner_end:owner_end] = rendered
    return "".join(lines)


def split_yaml(text: str, image_id: str, unit: str, rom: int, text_size: int, slot_end: int, kind: str = "c") -> str:
    """Carve the text unit [rom, slot_end) out of the asm subsegment that currently owns it.

    `kind` is the compiled subsegment type: `c`, or `cpp` for a C++ translation unit."""
    if kind not in COMPILED_SUFFIXES:
        raise ValueError(f"Unsupported compiled subsegment kind {kind}")
    new = [f"[0x{rom:X}, {kind}, units/{image_id}/{unit}]"]
    if rom + text_size < slot_end:
        new.append(f"{{start: 0x{rom + text_size:X}, type: bin, name: {unit}_padding, linker_section_order: .text}}")
    return _carve(text, rom, slot_end, {"asm"}, new, TAIL_PREFIX[image_id])


def assert_text_placement(text: str, rom: int, text_size: int, slot_end: int) -> None:
    """Refuse a text carve whose inputs the parent SUBALIGN would move.

    ld places every input of a segment's output section at the next multiple of the
    parent's SUBALIGN (splat `subalign`, default 16), whatever the object's own alignment.
    The C/C++ unit starts at `rom`, its padding `bin` (if any) at `rom + text_size`, and the
    following input at `slot_end`; each must already lie on that grid."""
    config = yaml.safe_load(text)
    starts = [(item.get("start") if isinstance(item, dict) else item[0], item) for item in config["segments"]]
    owner = max(((start, index) for index, (start, _item) in enumerate(starts) if type(start) is int and start <= rom), default=None)
    segment = starts[owner[1]][1] if owner else None
    if not isinstance(segment, dict) or "vram" not in segment or owner[1] + 1 == len(starts):
        raise ValueError(f"No splat code segment contains ROM 0x{rom:X}")
    subalign = splat_subalign(segment, config.get("options") or {})
    delta = segment["vram"] - segment["start"]
    moved = [start for start in dict.fromkeys((rom, rom + text_size, slot_end)) if (start + delta) % subalign]
    if moved:
        raise ValueError(f"Segment {segment['name']} links inputs at SUBALIGN({subalign}): the input at ROM 0x{moved[0]:X} "
                         "would move; byte placement there needs a reviewed `subalign: 1` layout")


def _yaml_field(node: yaml.MappingNode, name: str) -> yaml.Node:
    return next(value for key, value in node.value if key.value == name)


def _assert_main_bss_unchanged(owner: DataPart) -> None:
    """Do not lower SUBALIGN underneath an existing C BSS/COMMON layout."""
    segment = owner.segment
    delta = segment["vram"] - segment["start"]
    bss = [part for part in segment["subsegments"]
           if isinstance(part, dict) and part.get("type") in {"bss", ".bss", "sbss", ".sbss"}]
    if bss != [{"type": "bss", "vram": owner.segment_end + delta, "name": segment["name"]}]:
        raise ValueError("Byte-granular data layout must precede C BSS adoption; rerun BSS plans under SUBALIGN(1)")
    # A missing YAML .bss owner alone is insufficient: implicit COMMON and orphan
    # NOBITS inputs would also be affected. Check the accepted object inventory.
    build = accepted_build()
    graph = json.loads((build / "receipt.json").read_text())["graph"]
    expected = f"generated/asm/data/{segment['name']}.bss.s"
    raw_bss = 0
    for path, item in graph.items():
        if item.get("image_id") != "main_14400":
            continue
        elf = Elf((build / path).read_bytes())
        if any(symbol.shndx == SHN_COMMON for symbol in elf.symbols):
            raise ValueError(f"SUBALIGN transition would affect accepted COMMON: {path}")
        for section in elf.sections:
            if section.type == SHT_NOBITS and section.size:
                if (item["source_kind"] != "asm" or item["input"] != expected
                        or section.name != ".bss" or section.size != segment["bss_size"]):
                    raise ValueError(f"SUBALIGN transition would affect accepted BSS: {path} {section.name}")
                raw_bss += 1
    if raw_bss != 1:
        raise ValueError("SUBALIGN transition requires the one complete accepted main BSS input")


def _byte_subalign_line(owner: DataPart, parent_node: yaml.MappingNode, lines: list[str]) -> int:
    """Enable supported parent SUBALIGN(1), without altering text intervals.

    Returns the source line of the parent's single `subalign` field, which must be a
    one-line block-mapping entry so it can be rewritten in place."""
    segment = owner.segment
    if segment.get("name") != "main_14400" or segment.get("subalign") != 4:
        raise ValueError(f"Byte-granular initialized data requires a reviewed SUBALIGN transition of segment "
                         f"{segment.get('name')} (only main_14400 `subalign: 4` -> 1 is automatic)")
    parts = segment["subsegments"]
    for index, part in enumerate(parts):
        kind = part[1] if isinstance(part, list) and len(part) > 1 else part.get("type") if isinstance(part, dict) else None
        is_text = kind in {"asm", "hasm", *COMPILED_SUFFIXES} or (kind == "bin" and isinstance(part, dict)
                                                                  and part.get("linker_section_order") == ".text")
        if not is_text:
            continue
        start = part[0] if isinstance(part, list) else part["start"]
        following = parts[index + 1] if index + 1 < len(parts) else None
        end = (following[0] if isinstance(following, list) else following.get("start")) if following else None
        if type(start) is not int or type(end) is not int or start % 4 or end % 4:
            raise ValueError("SUBALIGN transition requires unchanged word-aligned text intervals")
    _assert_main_bss_unchanged(owner)
    fields = [(key, value) for key, value in parent_node.value if key.value == "subalign"]
    key, value = fields[0] if len(fields) == 1 else (None, None)
    if (key is None or parent_node.flow_style or value.end_mark.line != key.start_mark.line
            or lines[key.start_mark.line][:key.start_mark.column].strip()):
        raise ValueError("Parent subalign must be one single-line block-mapping field to change it")
    return key.start_mark.line


def split_data_yaml(text: str, image_id: str, unit: str, section: str, rom: int, size: int,
                    *, preserved: list[tuple[int, int]] | None = None) -> str:
    """Carve exact C data; preserve short boundary fragments as original-byte bins.

    Splat's data disassembler truncates partial words and aligns doubles relative
    to each new rodata file. Bins bridge to the existing safe asm alignment; they
    have .data input sections but retain the original .data/.rodata link order.
    Neither bin bytes nor their linker aliases are C ownership or credit.
    """
    if section not in {".data", ".rodata"} or size <= 0:
        raise ValueError("An initialized data carve requires .data/.rodata and a positive size")
    kind = section[1:]
    owner = next((part for part in yaml_data_parts(text) if part.start <= rom < part.end), None)
    if owner is None or owner.kind != kind:
        raise ValueError(f"{section} at ROM 0x{rom:X} must lie inside an unowned `{kind}` subsegment")
    spec = owner.segment["subsegments"][owner.part_index]
    if (isinstance(spec, dict) and not owner.preserved and set(spec) != {"start", "type", "name"}
            or isinstance(spec, list) and len(spec) != 3):
        raise ValueError("Data subsegment options require an explicit layout review before carving")
    end = rom + size
    if end > owner.end:
        raise ValueError(f"Next subsegment after {owner.name} starts before the unit's natural end 0x{end:X}")
    alignment = TAIL_ALIGNMENT[(image_id, kind)]
    prefix = owner.start if owner.preserved else max(owner.start, rom // alignment * alignment)
    tail = owner.end if owner.preserved else min(owner.end, (end + alignment - 1) // alignment * alignment)
    bridges = [(start, stop) for start, stop in ((prefix, rom), (end, tail)) if start < stop]
    # Pinned splat rejects subsegment SUBALIGN. Parent SUBALIGN(4) also rounds
    # binary inputs, so byte-started C/bridges require the guarded parent change.
    tree = yaml.compose(text)
    parent = _yaml_field(tree, "segments").value[owner.segment_index]
    node = _yaml_field(parent, "subsegments").value[owner.part_index]
    lines = text.splitlines(keepends=True)
    subalign = splat_subalign(owner.segment, yaml.safe_load(text).get("options") or {})
    if any(address % subalign for address in [rom, end, *[start for start, _stop in bridges]]):
        line = _byte_subalign_line(owner, parent, lines)
        indent = lines[line][:len(lines[line]) - len(lines[line].lstrip())]
        lines[line] = f"{indent}subalign: 1  # exact C data and preserved-byte bridges; text intervals unchanged\n"
    new = []
    if prefix > owner.start:
        new.append(f"[0x{owner.start:X}, {kind}, {owner.name}]")
    if prefix < rom:
        new.append(f"{{start: 0x{prefix:X}, type: bin, name: {owner.segment['name']}_{kind}_bytes_{prefix:x}, linker_section_order: .{kind}}}")
    new.append(f"[0x{rom:X}, .{kind}, units/{image_id}/{unit}]")
    if end < tail:
        new.append(f"{{start: 0x{end:X}, type: bin, name: {owner.segment['name']}_{kind}_bytes_{end:x}, linker_section_order: .{kind}}}")
    if tail < owner.end:
        new.append(f"[0x{tail:X}, {kind}, {DATA_TAIL_PREFIX[(image_id, kind)]}_{tail:x}]")
    # The complete owner node: a flow mapping/sequence through its closing delimiter
    # (possibly on a later line), a block mapping through its last field.
    first, last = node_line_span(node)
    entry = re.fullmatch(r"(?P<indent> *)- +", lines[first][:node.start_mark.column])
    if entry is None:
        raise ValueError(f"Data owner {owner.name} must be its own block-sequence entry to be carved")
    lines[first:last] = [f"{entry['indent']}- {line}\n" for line in new]
    if preserved is not None:
        delta = owner.segment["vram"] - owner.segment["start"]
        preserved.extend((start + delta, stop + delta) for start, stop in bridges)
    return "".join(lines)


def split_bss_yaml(text: str, image_id: str, unit: str, vram: int, end: int, bss_end: int) -> str:
    """Carve a C unit's `.bss`+COMMON [vram, end) out of the owning asm `bss` subsegment.

    `end` is the SUBALIGN-ed end of the unit's linked `.bss`+COMMON (`Result.bss_carve["tail"]`);
    `bss_end` is the loader-backed BSS end that bounds the segment's last BSS subsegment."""
    return _carve(text, vram, end, {"bss"}, [f"{{type: .bss, vram: 0x{vram:X}, name: units/{image_id}/{unit}}}"],
                  DATA_TAIL_PREFIX[(image_id, "bss")], bss_end)


def _segment_config(text: str, segment: str) -> tuple[dict[str, Any], dict[str, Any]]:
    """The parsed splat segment named `segment` (exactly one) and the global options."""
    config = yaml.safe_load(text)
    found = [item for item in config["segments"] if isinstance(item, dict) and item.get("name") == segment]
    if len(found) != 1:
        raise ValueError(f"No unique splat segment named {segment}")
    return found[0], config.get("options") or {}


def enable_bss_common(text: str, segment: str) -> str:
    """Set `bss_contains_common: true` on splat segment `segment`, so each C unit's COMMON links
    with its own `.bss` (`<obj>(.bss COMMON .scommon)`) instead of becoming an orphan section.

    The whole layout is parsed first (aliases, merge keys and duplicate keys refuse), so an
    existing field is found in any position, style or comment form: `true` leaves the text
    unchanged, `false` (or a non-boolean) is refused. Otherwise one field is inserted before
    the parent's `subsegments` key, and the result must reparse with exactly that one field."""
    lines = text.splitlines(keepends=True)
    yaml_bss_subsegments(lines)
    config, _options = _segment_config(text, segment)
    if "bss_contains_common" in config:
        if config["bss_contains_common"] is True:
            return text
        if config["bss_contains_common"] is False:
            raise ValueError(f"Segment {segment} explicitly disables bss_contains_common")
        raise ValueError(f"Segment {segment} has a non-boolean bss_contains_common: {config['bss_contains_common']!r}")
    parent = next((node for node in _yaml_field(yaml.compose(text), "segments").value
                   if isinstance(node, yaml.MappingNode) and any(
                       key.value == "name" and isinstance(value, yaml.ScalarNode) and value.value == segment
                       for key, value in node.value)), None)
    key = parent and next((key for key, _value in parent.value if key.value == "subsegments"), None)
    if key is None or parent.flow_style or lines[key.start_mark.line][:key.start_mark.column].strip():
        raise ValueError(f"Segment {segment} needs a block mapping whose subsegments key starts its own line")
    lines.insert(key.start_mark.line, f"{lines[key.start_mark.line][:key.start_mark.column]}bss_contains_common: true\n")
    result = "".join(lines)
    yaml_bss_subsegments(result.splitlines(keepends=True))
    if _segment_config(result, segment)[0].get("bss_contains_common") is not True:
        raise ValueError(f"Segment {segment}: bss_contains_common insertion did not take effect")
    return result


def carve_bss_plan(text: str, image_id: str, unit: str, carve: dict[str, Any]) -> str:
    """Apply `match.py`'s BSS plan (`Result.bss_carve`), only under the parent SUBALIGN it was computed with.

    The carve's start, tail and COMMON placement depend on that SUBALIGN. A data carve in
    the same adoption may have changed the parent (main 4 -> 1); such a plan is stale."""
    parent, options = _segment_config(text, carve["segment"])
    effective = splat_subalign(parent, options)
    if effective != carve["subalign"]:
        raise ValueError(f"BSS plan for segment {carve['segment']} was computed under SUBALIGN({carve['subalign']}), "
                         f"but this adoption's layout uses SUBALIGN({effective}). Adopt the byte-granular data layout "
                         "first (a unit without BSS/COMMON), then rematch this unit under the effective parent")
    text = split_bss_yaml(text, image_id, unit, carve["vram"], carve["tail"], carve["range_end"])
    if carve["add_bss_contains_common"]:
        text = enable_bss_common(text, carve["segment"])
    return text


def _bss_plan_key(carve: dict[str, Any] | None) -> tuple[Any, ...] | None:
    """The placement a BSS plan fixes; `owner` and the COMMON flag legitimately change once carved."""
    return carve and tuple(carve[name] for name in ("segment", "subalign", "vram", "end", "tail"))


def interior_aliases(result, unit: str, *, preserved: list[tuple[int, int]] | None = None) -> str:
    """PROVIDE lines for accepted-build labels inside a newly owned data range.

    splat labels every address other code references, so a C table can cover labels
    such as `D_80139B58` (an interior element) that remaining assembly still uses.
    The C object defines only its own names; the rest keep their original addresses.
    Labels inside `preserved` original-byte bridges keep theirs too (never C).
    """
    if not result.data_sections:
        return ""
    known = symbol_addresses(accepted_build() / "shiren2.elf")
    defined = {s.name for s in Elf((Path(result.out_dir) / "candidate.o").read_bytes()).symbols if s.shndx}
    lines = []
    for record in result.data_sections:
        # match.py blocks a BSS carve whose labels are referenced outside the unit, so BSS needs none.
        if record.get("vram") is None or record["section"] in UNINITIALIZED:
            continue
        start, end = record["vram"], record["vram"] + record["size"]
        for name, address in sorted(known.items(), key=lambda item: item[1]):
            if start <= address < end and name not in defined and "." not in name:
                lines.append(f"PROVIDE({name} = 0x{address:08X}); /* inside {unit} {record['section']} */\n")
    for start, end in preserved or ():
        for name, address in sorted(known.items(), key=lambda item: item[1]):
            if start <= address < end and "." not in name:
                if name in defined:
                    raise ValueError(f"C object redefines neighboring preserved-data label {name}")
                lines.append(f"PROVIDE({name} = 0x{address:08X}); /* preserved bytes beside {unit}, not C */\n")
    return "".join(lines)


def _header_destinations(result_files: list[dict[str, str]], candidate: Path, includes: list[Path], image_id: str) -> dict[Path, Path]:
    """Map non-canonical participating headers to their canonical locations."""
    canonical = [(PROJECT / "src").resolve(), (PROJECT / "include").resolve()]
    unit_dir = PROJECT / "src/units" / image_id
    mapping: dict[Path, Path] = {}
    for record in result_files:
        path = Path(record["path"])
        if path == candidate or any(path.is_relative_to(root) for root in canonical):
            continue
        if path.is_relative_to(candidate.parent):
            mapping[path] = unit_dir / path.relative_to(candidate.parent)
            continue
        roots = [root.resolve() for root in includes if path.is_relative_to(root.resolve())]
        if not roots:
            raise ValueError(f"Participating file outside the candidate directory and -I paths: {path}")
        mapping[path] = PROJECT / "include" / path.relative_to(roots[0])
    return mapping


def interface_candidate(unit: Unit) -> dict[str, object]:
    """The unit's source as a `tools/interfaces.py` candidate, preprocessed with the profile adopt() uses."""
    try:
        profile = unit.profile or default_profile(resolve_targets(unit.functions)[0].image_id, unit.source)
    except (ValueError, KeyError, IndexError):
        profile = unit.profile  # adopt() reports the unresolvable unit itself
    return {"source": str(unit.source), "profile": profile, "includes": [str(path) for path in unit.includes]}


@dataclass(frozen=True)
class Absorbed:
    """An adopted unit (one TU with its `config/matches.json` rows) that a candidate replaces."""
    image_id: str
    source: str  # project-relative TU source, e.g. `src/units/main_14400/func_80056D60.c`
    rows: tuple[dict[str, Any], ...]  # its matches rows, in ROM order

    @property
    def layout_name(self) -> str:
        """Splat name of its `c`/`cpp`, `.data`/`.rodata` and `.bss` subsegments: the source below src/, no suffix."""
        return Path(self.source).relative_to("src").with_suffix("").as_posix()

    @property
    def stem(self) -> str:
        """The unit name adopt used for its padding `bin` and in its alias comments."""
        return Path(self.source).stem


def absorbed_units(rows: list[dict[str, Any]], symbols: list[str], image_id: str, targets: list[Any]) -> list[Absorbed]:
    """The adopted units selected by `symbols` (any function of each), in ROM order.

    Refused unless each lies entirely inside the candidate: every function of the unit is a
    candidate function and its ROM range lies inside the candidate's span. Any other adopted
    function of the image that the candidate lists or whose range overlaps the span is refused."""
    start, end = targets[0].rom_start, targets[-1].slot_end
    listed = {target.symbol for target in targets}
    image_rows = [row for row in rows if row.get("image_id", "resident") == image_id]
    units: dict[str, Absorbed] = {}
    for symbol in symbols:
        hits = [row for row in image_rows if row["symbol"] == symbol]
        if len(hits) != 1:
            raise ValueError(f"Cannot absorb {symbol}: {len(hits)} adopted {image_id} rows of that name")
        source = hits[0]["source"]
        parts = Path(source).parts
        if parts[0] != "src" or ".." in parts or Path(source).suffix not in COMPILED_SUFFIXES.values():
            raise ValueError(f"Cannot absorb {symbol}: unsupported unit source {source}")
        members = tuple(sorted((row for row in image_rows if row["source"] == source), key=lambda row: row["rom_start"]))
        outside = [row["symbol"] for row in members if row["symbol"] not in listed]
        if outside:
            raise ValueError(f"Absorbed unit {source} also owns {outside}; the candidate must list every function it absorbs")
        escaped = [row["symbol"] for row in members if not start <= row["rom_start"] <= row["rom_start"] + row["size"] <= end]
        if escaped:
            raise ValueError(f"Absorbed unit {source}: {escaped} lie outside the candidate span 0x{start:X}-0x{end:X}")
        units[source] = Absorbed(image_id, source, members)
    owned = {row["symbol"] for unit in units.values() for row in unit.rows}
    overlapping = [row["symbol"] for row in image_rows if row["symbol"] not in owned
                   and (row["symbol"] in listed or (row["rom_start"] < end and start < row["rom_start"] + row["size"]))]
    if overlapping:
        raise ValueError(f"Already matched: {overlapping} (to replace whole adopted units, absorb one symbol of each with --absorb)")
    return sorted(units.values(), key=lambda unit: unit.rows[0]["rom_start"])


def _layout_entries(text: str) -> dict[int, list[tuple[Any, int, int, str | None]]]:
    """Per parent segment index: each subsegment's parsed value, its source lines [first, end) and entry indent."""
    config = yaml.safe_load(text)
    tree = yaml.compose(text)
    lines = text.splitlines(keepends=True)
    entries = {}
    for index, (segment, node) in enumerate(zip(config["segments"], _yaml_field(tree, "segments").value)):
        if not isinstance(segment, dict) or not isinstance(segment.get("subsegments"), list):
            continue
        parts = []
        for value, child in zip(segment["subsegments"], _yaml_field(node, "subsegments").value):
            first, end = node_line_span(child)
            entry = re.fullmatch(r"(?P<indent> *)- +", lines[first][:child.start_mark.column])
            parts.append((value, first, end, entry["indent"] if entry else None))
        entries[index] = parts
    return entries


def _rom_part(value: Any) -> tuple[int, str, str] | None:
    """(start, kind, name) of a plain ROM subsegment: `[start, kind, name]` or `{start: .., type: .., name: ..}`."""
    if isinstance(value, list) and len(value) == 3:
        start, kind, name = value
    elif isinstance(value, dict) and set(value) == {"start", "type", "name"}:
        start, kind, name = value["start"], value["type"], value["name"]
    else:
        return None
    return (start, kind, name) if type(start) is int and isinstance(kind, str) and isinstance(name, str) else None


def _fresh_name(text: str, name: str) -> str:
    if re.search(rf"(?<![\w/]){re.escape(name)}(?!\w)", text):
        raise ValueError(f"Restored subsegment name {name} already exists")
    return name


def restore_text_yaml(text: str, unit: Absorbed) -> str:
    """Return an absorbed unit's `c`/`cpp` subsegment, its padding `bin` and its own asm tail to the neighbouring asm.

    The inverse of `split_yaml`: merged into a preceding `asm` subsegment, else replaced by one
    `asm` subsegment named like an adopt tail. Only an asm tail named after its own start
    (`<prefix>_<start>`, what `split_yaml` emits after a unit) joins; other boundaries stay."""
    if unit.image_id not in TAIL_PREFIX:
        raise ValueError(f"No text layout convention for image {unit.image_id}")
    prefix = TAIL_PREFIX[unit.image_id]
    found = [(parts, index) for parts in _layout_entries(text).values() for index, (value, *_span) in enumerate(parts)
             if (part := _rom_part(value)) and part[1] in COMPILED_SUFFIXES and part[2] == unit.layout_name]
    if len(found) != 1:
        raise ValueError(f"Absorbed unit {unit.source} needs exactly one c/cpp subsegment {unit.layout_name} (found {len(found)})")
    parts, index = found[0]
    rom = _rom_part(parts[index][0])[0]
    if rom != unit.rows[0]["rom_start"]:
        raise ValueError(f"{unit.layout_name} text starts at 0x{rom:X}, not at its first function 0x{unit.rows[0]['rom_start']:X}")
    if parts[index][3] is None:
        raise ValueError(f"{unit.layout_name} must be its own block-sequence entry to be restored")
    last = index
    padding = parts[last + 1][0] if last + 1 < len(parts) else None
    if (isinstance(padding, dict) and set(padding) == {"start", "type", "name", "linker_section_order"}
            and (padding["type"], padding["name"], padding["linker_section_order"]) == ("bin", f"{unit.stem}_padding", ".text")):
        last += 1
    tail = _rom_part(parts[last + 1][0]) if last + 1 < len(parts) else None
    if tail and tail[1] == "asm" and tail[2] == f"{prefix}_{tail[0]:x}":
        last += 1
    previous = _rom_part(parts[index - 1][0]) if index else None
    replacement = [] if previous and previous[1] == "asm" else [
        f"{parts[index][3]}- [0x{rom:X}, asm, {_fresh_name(text, f'{prefix}_{rom:x}')}]\n"]
    lines = text.splitlines(keepends=True)
    lines[parts[index][1]:parts[last][2]] = replacement
    return "".join(lines)


def restore_data_yaml(text: str, unit: Absorbed) -> str:
    """Return the first `.data`/`.rodata` carve of an absorbed unit to the neighbouring data form.

    The inverse of `split_data_yaml` for a carve in adopt's own form: the carve, its own
    original-byte bridges (inside the grid units around an off-grid start or end) and its data
    tail (`<prefix>_<grid end>`) merge into a preceding unowned data subsegment of that kind,
    else become one named like an adopt data tail. A carve bordering initialized data that is
    not its own bridge (e.g. one carved out of another unit's bridge) is refused for review."""
    parts = yaml_data_parts(text)
    carve = next((part for part in parts if part.kind in {".data", ".rodata"} and part.name == unit.layout_name), None)
    if carve is None:
        return text
    kind = carve.kind[1:]
    if (unit.image_id, kind) not in TAIL_ALIGNMENT:
        raise ValueError(f"No {carve.kind} layout convention for image {unit.image_id}")
    alignment, prefix = TAIL_ALIGNMENT[(unit.image_id, kind)], DATA_TAIL_PREFIX[(unit.image_id, kind)]
    near = {part.part_index: part for part in parts if part.segment_index == carve.segment_index}
    floor, ceil = carve.start // alignment * alignment, -(-carve.end // alignment) * alignment
    first = last = carve.part_index
    foreign = (f"{unit.layout_name} {carve.kind} 0x{carve.start:X}-0x{carve.end:X} borders initialized data that is not "
               "its own bridge; absorbing it needs a layout review")
    if carve.start % alignment:
        before = near.get(first - 1)
        if before is not None and before.preserved and before.kind == kind and (before.start == floor or near.get(first - 2) is None):
            first -= 1
        elif before is not None:
            raise ValueError(foreign)
    if carve.end % alignment:
        after = near.get(last + 1)
        if after is not None and after.preserved and after.kind == kind and (after.end == ceil or near.get(last + 2) is None):
            last += 1
        elif after is not None:
            raise ValueError(foreign)
    tail = near.get(last + 1)
    if tail is not None and not tail.preserved and tail.kind == kind and tail.start == ceil and tail.name == f"{prefix}_{ceil:x}":
        last += 1
    entries = _layout_entries(text)[carve.segment_index]
    if entries[first][3] is None:
        raise ValueError(f"{unit.layout_name} {carve.kind} must be its own block-sequence entry to be restored")
    start, before = near[first].start, near.get(first - 1)
    merged = before is not None and not before.preserved and before.kind == kind
    replacement = [] if merged else [f"{entries[first][3]}- [0x{start:X}, {kind}, {_fresh_name(text, f'{prefix}_{start:x}')}]\n"]
    lines = text.splitlines(keepends=True)
    lines[entries[first][1]:entries[last][2]] = replacement
    return "".join(lines)


def restore_bss_yaml(text: str, unit: Absorbed) -> str:
    """Return an absorbed unit's `.bss` carve and its own `bss` tail to the neighbouring asm `bss`.

    The inverse of `split_bss_yaml`: merged into the preceding asm `bss` subsegment of the
    segment, else replaced by one named like an adopt BSS tail (`<prefix>_<low 24 vram bits>`)."""
    lines = text.splitlines(keepends=True)
    parts = yaml_bss_subsegments(lines)
    owned = [part for part in parts if part.kind == ".bss" and part.name == unit.layout_name]
    if not owned:
        return text
    if len(owned) != 1 or (unit.image_id, "bss") not in DATA_TAIL_PREFIX:
        raise ValueError(f"Cannot restore the BSS layout of {unit.layout_name}")
    part, prefix = owned[0], DATA_TAIL_PREFIX[(unit.image_id, "bss")]
    same = [other for other in parts if other.segment == part.segment]
    index = same.index(part)
    edits = []
    following = same[index + 1] if index + 1 < len(same) else None
    if following is not None and following.kind == "bss" and following.name == f"{prefix}_{following.vram & 0xFFFFFF:x}":
        edits.append((following.line, following.end_line, []))
    previous = same[index - 1] if index else None
    name = None if previous is not None and previous.kind == "bss" else _fresh_name(text, f"{prefix}_{part.vram & 0xFFFFFF:x}")
    edits.append((part.line, part.end_line, [f"{part.indent}- {{type: bss, vram: 0x{part.vram:X}, name: {name}}}\n"] if name else []))
    for first, end, replacement in sorted(edits, reverse=True):
        lines[first:end] = replacement
    return "".join(lines)


def restore_layout(text: str, unit: Absorbed) -> str:
    """`config/shiren2.jp.yaml` with every subsegment of an absorbed unit returned to the neighbouring form."""
    text = restore_text_yaml(text, unit)
    while (restored := restore_data_yaml(text, unit)) != text:
        text = restored
    text = restore_bss_yaml(text, unit)
    left = [value for parts in _layout_entries(text).values() for value, *_span in parts
            if (value[2] if isinstance(value, list) and len(value) > 2 else value.get("name") if isinstance(value, dict) else None)
            == unit.layout_name]
    if left:
        raise ValueError(f"{unit.layout_name} still owns subsegments after restoring its layout: {left}")
    return text


def retire_aliases(text: str, unit: Absorbed) -> str:
    """Drop the PROVIDE lines `interior_aliases` generated for an absorbed unit (interior and bridge labels)."""
    stem = re.escape(unit.stem)
    return re.sub(rf"^PROVIDE\([A-Za-z_$][\w$]* = 0x[0-9A-F]{{8}}\); "
                  rf"/\* (?:inside {stem} \.\w+|preserved bytes beside {stem}, not C) \*/\n", "", text, flags=re.MULTILINE)


def retire_units(originals: dict[str, str], absorbed: list[Absorbed]) -> dict[str, str]:
    """Configuration without the absorbed units: matches rows, TU profiles, image bindings and
    interior aliases removed, layout restored to the neighbouring asm/data/bss form."""
    updated = dict(originals)
    if not absorbed:
        return updated
    retired = {(unit.image_id, unit.source) for unit in absorbed}
    rows = json.loads(originals["config/matches.json"])
    rows["functions"] = [row for row in rows["functions"] if (row.get("image_id", "resident"), row["source"]) not in retired]
    profiles = json.loads(originals["config/compiler_profiles.json"])
    images = json.loads(originals["config/images.json"])
    layout, aliases = originals["config/shiren2.jp.yaml"], originals["config/symbol_aliases.ld"]
    for unit in absorbed:
        if unit.source not in profiles["tu_profiles"] or images["source_bindings"].get(unit.source) != unit.image_id:
            raise ValueError(f"Absorbed unit {unit.source} lacks its TU profile or {unit.image_id} image binding")
        del profiles["tu_profiles"][unit.source]
        del images["source_bindings"][unit.source]
        layout = restore_layout(layout, unit)
        aliases = retire_aliases(aliases, unit)
    updated["config/matches.json"] = json.dumps(rows, indent=2, ensure_ascii=False) + "\n"
    updated["config/compiler_profiles.json"] = json.dumps(profiles, indent=2, ensure_ascii=False) + "\n"
    updated["config/images.json"] = json.dumps(images, indent=2, ensure_ascii=False) + "\n"
    updated["config/shiren2.jp.yaml"] = layout
    updated["config/symbol_aliases.ld"] = aliases
    return updated


def gate_findings(gate: Gate, index: int, retired: set[str]) -> list[Any]:
    """Findings candidate `index` introduces over the gate's admitted TUs without the `retired` canonical TUs."""
    if not retired:
        return gate.check(index)
    missing = retired - {result.tu for result in gate.admitted if result.candidate is None}
    if missing:
        raise ValueError(f"Absorbed {sorted(missing)} are not canonical TUs of this interface audit "
                         "(adopted earlier in this run?); absorb them in a separate run")
    kept = [result for result in gate.admitted if result.tu not in retired]
    previous = {finding.key for finding in analyse(kept).findings}
    return [finding for finding in analyse([*kept, gate.candidates[index]]).findings if finding.key not in previous]


def gate_admit(gate: Gate, index: int, retired: set[str]) -> None:
    """Admit candidate `index`; the `retired` canonical TUs leave the audit for later candidates."""
    if retired:
        gate.admitted = [result for result in gate.admitted if result.tu not in retired]
    gate.admit(index)


def adopt(unit: Unit, *, rom: bytes, log: list[str], gate: Gate, index: int) -> dict[str, int]:
    targets = resolve_targets(unit.functions)
    image_id = targets[0].image_id
    name = unit.name or targets[0].symbol
    profile = unit.profile or default_profile(image_id, unit.source)
    kind = next((kind for kind, suffix in COMPILED_SUFFIXES.items() if unit.source.suffix == suffix), None)
    if kind is None:
        raise ValueError(f"Candidate source must be .c or .cpp: {unit.source}")
    source_rel = f"src/units/{image_id}/{name}{COMPILED_SUFFIXES[kind]}"
    destination = PROJECT / source_rel
    matches = json.loads((PROJECT / "config/matches.json").read_text())
    absorbed = absorbed_units(matches["functions"], unit.absorbs, image_id, targets)
    retired = {item.source for item in absorbed}
    # A .c and a .cpp unit of one name would share the object obj/source/src/units/<image>/<name>.o.
    existing = [path for path in (destination.with_suffix(suffix) for suffix in COMPILED_SUFFIXES.values())
                if path.exists() and path.relative_to(PROJECT).as_posix() not in retired]
    if existing:
        raise ValueError(f"Canonical source already exists: {existing[0].relative_to(PROJECT)}")
    stamp = time.strftime("%Y%m%dT%H%M%S")
    work = PROJECT / "scratch/omp/adopt" / f"{name}-{stamp}"
    result, _ = check(targets, unit.source, profile, work / "candidate", includes=unit.includes, rom=rom)
    if result.status not in {"match", "match-with-data"}:
        raise ValueError(f"{name}: tools/match.py status {result.status}: {result.problems or result.out_dir}")
    introduced = gate_findings(gate, index, retired)
    if introduced:
        log.append(f"== {name}: refused, introduces {len(introduced)} interface finding(s) (tools/interfaces.py)")
        log.extend(f"   {finding.summary()}" for finding in introduced)
        raise ValueError(f"{name}: introduces {len(introduced)} interface finding(s); see tools/interfaces.py --prototype")
    candidate = unit.source.resolve()
    headers = _header_destinations(result.participating_files, candidate, unit.includes, image_id)
    for origin, target in headers.items():
        relative = target.relative_to(PROJECT).as_posix()
        if target.exists() and target.read_bytes() != origin.read_bytes() and relative not in unit.replace_headers:
            raise ValueError(f"{relative} exists with different content; pass --replace-header {relative} if intended")
    slot_end = targets[-1].slot_end
    if any(rom[targets[0].rom_start + result.text_bytes:slot_end]):
        raise ValueError("Leftover natural gap contains nonzero bytes")

    originals = {path: (PROJECT / path).read_text(encoding="utf-8") for path in CONFIG_FILES}
    base = retire_units(originals, absorbed)  # the configuration without the absorbed units
    updated = dict(base)
    rows = json.loads(base["config/matches.json"])
    evidence = unit.evidence or (f"omp tools/match.py exact natural {result.text_bytes}-byte {profile} text"
                                 f" (source sha256 {result.source['sha256'][:16]}); full-ROM acceptance required")
    for target in targets:
        rows["functions"].append({"image_id": image_id, "symbol": target.symbol, "source": source_rel,
                                  "rom_start": target.rom_start, "vram_start": target.vram_start,
                                  "size": target.size, "evidence": evidence})
    updated["config/matches.json"] = json.dumps(rows, indent=2, ensure_ascii=False) + "\n"
    profiles = json.loads(base["config/compiler_profiles.json"])
    profiles["tu_profiles"][source_rel] = profile
    updated["config/compiler_profiles.json"] = json.dumps(profiles, indent=2, ensure_ascii=False) + "\n"
    images = json.loads(base["config/images.json"])
    images["source_bindings"][source_rel] = image_id
    updated["config/images.json"] = json.dumps(images, indent=2, ensure_ascii=False) + "\n"
    assert_text_placement(base["config/shiren2.jp.yaml"], targets[0].rom_start, result.text_bytes, slot_end)
    updated["config/shiren2.jp.yaml"] = split_yaml(base["config/shiren2.jp.yaml"], image_id, name,
                                                   targets[0].rom_start, result.text_bytes, slot_end, kind)
    preserved_data: list[tuple[int, int]] = []
    for record in result.data_sections:
        if record["section"] in UNINITIALIZED:
            continue  # carved below as the unit's `.bss` subsegment
        updated["config/shiren2.jp.yaml"] = split_data_yaml(updated["config/shiren2.jp.yaml"], image_id, name,
                                                            record["section"], record["rom_start"], record["size"],
                                                            preserved=preserved_data)
    if result.bss_carve:
        # After the data carves: refused if they changed the parent SUBALIGN the plan assumed.
        updated["config/shiren2.jp.yaml"] = carve_bss_plan(updated["config/shiren2.jp.yaml"], image_id, name, result.bss_carve)
    updated["config/symbol_aliases.ld"] += interior_aliases(result, name, preserved=preserved_data)
    copies = {candidate: destination, **headers}
    log.append(f"== {name}: {len(targets)} function(s), {result.instruction_bytes} instruction bytes, "
               f"text {result.text_bytes}/{result.slot_bytes}, profile {profile}")
    for origin, target in copies.items():
        log.append(f"   copy {origin} -> {target.relative_to(PROJECT)}")
    removed_sources = [PROJECT / item.source for item in absorbed if PROJECT / item.source not in copies.values()]
    for item in absorbed:
        log.append(f"   absorb {item.source} ({', '.join(row['symbol'] for row in item.rows)}): rows, TU profile, image binding, "
                   f"layout and aliases retired; source {'removed' if PROJECT / item.source in removed_sources else 'replaced'}")

    replaced = {target: target.read_bytes() for target in copies.values() if target.exists()}
    removed = {path: path.read_bytes() for path in removed_sources}
    created: list[Path] = []
    try:
        for origin, target in copies.items():
            target.parent.mkdir(parents=True, exist_ok=True)
            if not target.exists():
                created.append(target)
            shutil.copyfile(origin, target)
        for path in removed:
            path.unlink()
        for path in CONFIG_FILES:
            (PROJECT / path).write_text(updated[path], encoding="utf-8")
        # A relocated header can resolve differently from canonical paths; with no moved
        # headers the canonical compile has identical inputs, and the full build re-proves it.
        if headers or result.data_sections:
            final, _ = check(targets, destination, profile, work / "canonical", rom=rom)
            if final.status != result.status:
                raise ValueError(f"{name}: canonical copy no longer matches ({final.status}): {final.problems or final.out_dir}")
            if _bss_plan_key(final.bss_carve) != _bss_plan_key(result.bss_carve):
                raise ValueError(f"{name}: canonical BSS plan {_bss_plan_key(final.bss_carve)} differs from the adopted "
                                 f"plan {_bss_plan_key(result.bss_carve)}")
    except BaseException:
        for path in CONFIG_FILES:
            (PROJECT / path).write_text(originals[path], encoding="utf-8")
        for target in created:
            target.unlink(missing_ok=True)
        for target, content in replaced.items():
            target.write_bytes(content)
        for path, content in removed.items():
            path.write_bytes(content)
        raise
    gate_admit(gate, index, retired)
    return {"functions": len(targets), "instruction_bytes": result.instruction_bytes}


def _symbols(value: str | list[str]) -> list[str]:
    """A batch symbol list: a JSON list or one comma-separated string."""
    return value.split(",") if isinstance(value, str) else list(value)


def _load_units(path: Path) -> list[Unit]:
    return [Unit(functions=_symbols(item["functions"]), source=Path(item["source"]), profile=item.get("profile"),
                 includes=[Path(p) for p in item.get("includes", [])], replace_headers=item.get("replace_headers", []),
                 name=item.get("name"), evidence=item.get("evidence"), absorbs=_symbols(item.get("absorbs", [])))
            for item in json.loads(path.read_text())]


def _adopt_batch(units: list[Unit], rom: bytes, log: list[str]) -> tuple[dict[str, int], list[str]]:
    gate = Gate(collect(PROJECT, [interface_candidate(unit) for unit in units]))
    totals = {"functions": 0, "instruction_bytes": 0}
    failures = []
    for index, unit in enumerate(units):
        try:
            counts = adopt(unit, rom=rom, log=log, gate=gate, index=index)
            totals = {key: totals[key] + counts[key] for key in totals}
        except (ValueError, OSError, KeyError) as error:
            failures.append(f"{','.join(unit.functions)}: {error}")
    return totals, failures


def _preview_batch(units: list[Unit], rom_path: Path, log: list[str]) -> tuple[dict[str, int], list[str]]:
    """Run real adoption in an isolated process/tree and report its cumulative source diff."""
    project = PROJECT.resolve()
    with tempfile.TemporaryDirectory(prefix="adopt-preview-") as temporary:
        directory = Path(temporary).resolve()
        staged = directory / "tree"
        before = create_snapshot(project, staged)
        # Matching only reads these resources; all writable source inputs are copied.
        for name in ("build", "generated", "docs", ".cache"):
            if (project / name).exists():
                (staged / name).symlink_to((project / name).resolve(), target_is_directory=True)

        def staged_path(path: Path) -> str:
            original = path.resolve()
            if original.is_relative_to(project):
                relative = original.relative_to(project)
                if relative.parts and (relative.parts[0] in SOURCE_DIRECTORIES or relative.as_posix() in before["files"]):
                    return str(staged / relative)
            return str(original)

        batch = directory / "batch.json"
        batch.write_text(json.dumps([{**asdict(unit), "source": staged_path(unit.source),
                                      "includes": [staged_path(path) for path in unit.includes]}
                                     for unit in units], indent=2) + "\n")
        # Import from the copied tools: rom.PROJECT and every imported/default root
        # must refer to the snapshot, without rebinding the live process's globals.
        worker = ("import json, sys; from pathlib import Path; "
                  "sys.path.insert(0, str(Path.cwd() / 'tools')); import adopt; "
                  "log = []; totals, failures = adopt._adopt_batch("
                  "adopt._load_units(Path(sys.argv[1])), Path(sys.argv[2]).read_bytes(), log); "
                  "print(json.dumps({'totals': totals, 'failures': failures, 'log': log}))")
        completed = subprocess.run([sys.executable, "-B", "-c", worker, str(batch), str(rom_path.resolve())],
                                   cwd=staged, env={**os.environ, "PYTHONDONTWRITEBYTECODE": "1"},
                                   capture_output=True, text=True)
        evidence = staged / "scratch/omp/adopt"
        retained_paths = {}
        if evidence.is_dir():
            retained = project / "scratch/omp/adopt"
            retained.mkdir(parents=True, exist_ok=True)
            for unit_evidence in sorted(evidence.iterdir()):
                destination = Path(tempfile.mkdtemp(prefix=f"{unit_evidence.name}-preview-", dir=retained))
                # Preserve evidence without overwriting earlier runs or following the
                # matcher's `source` symlink back into the temporary project.
                shutil.copytree(unit_evidence, destination, symlinks=True, dirs_exist_ok=True)
                retained_paths[str(unit_evidence)] = str(destination)

        def reported_path(text: str) -> str:
            for original, retained in retained_paths.items():
                text = text.replace(original, retained)
            return text.replace(str(staged), str(project))

        if completed.returncode:
            raise ValueError(f"Dry-run worker failed ({completed.returncode}): {reported_path(completed.stderr.strip())}")
        result = json.loads(completed.stdout)
        log.extend(reported_path(line) for line in result["log"])
        after = input_manifest(staged)
        verify_manifest(project, before)  # never diff against concurrently changed inputs
        for relative in sorted(before["files"].keys() | after["files"].keys()):
            if before["files"].get(relative) == after["files"].get(relative):
                continue
            old = (project / relative).read_text() if relative in before["files"] else ""
            new = (staged / relative).read_text() if relative in after["files"] else ""
            log.extend(line.rstrip("\n") for line in difflib.unified_diff(
                old.splitlines(keepends=True), new.splitlines(keepends=True),
                f"a/{relative}" if relative in before["files"] else "/dev/null",
                f"b/{relative}" if relative in after["files"] else "/dev/null"))
        return result["totals"], [reported_path(failure) for failure in result["failures"]]


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("functions", nargs="?", help="symbol or comma-separated adjacent symbols")
    parser.add_argument("source", type=Path, nargs="?")
    parser.add_argument("--batch", type=Path, help="JSON list of units")
    parser.add_argument("--profile")
    parser.add_argument("-I", dest="includes", type=Path, action="append", default=[])
    parser.add_argument("--replace-header", action="append", default=[], help="allow replacing include/<name> with the candidate's copy")
    parser.add_argument("--name", help="unit/file stem (default: first symbol)")
    parser.add_argument("--evidence", help="matches.json evidence text")
    parser.add_argument("--absorb", action="append", default=[], metavar="SYMBOL",
                        help="replace the whole adopted unit owning SYMBOL (repeatable); it must lie inside the candidate")
    parser.add_argument("--dry-run", action="store_true")
    args = parser.parse_args()
    if args.batch:
        units = _load_units(args.batch)
    elif args.functions and args.source:
        units = [Unit(args.functions.split(","), args.source, args.profile, args.includes, args.replace_header, args.name, args.evidence,
                      args.absorb)]
    else:
        parser.error("give FUNCTIONS SOURCE or --batch FILE")
    log: list[str] = []
    if args.dry_run:
        totals, failures = _preview_batch(units, CANONICAL_ROM, log)
    else:
        totals, failures = _adopt_batch(units, CANONICAL_ROM.read_bytes(), log)
    print("\n".join(log))
    verb = "would adopt" if args.dry_run else "adopted"
    print(f"{verb} {totals['functions']} function(s), {totals['instruction_bytes']} instruction bytes; {len(failures)} failure(s)")
    for failure in failures:
        print(f"FAILED {failure}", file=sys.stderr)
    if not args.dry_run and totals["functions"]:
        print("next: build twice (tools/build.py --name <x>-a/-b --no-promote), gmake test, review, verify --promote, progress record")
    sys.exit(1 if failures else 0)


if __name__ == "__main__":
    main()
