#!/usr/bin/env python3
"""Integrator: adopt matched candidates into canonical source and configuration.

    uv run --frozen python tools/adopt.py func_800624F0 cand.c [--profile ID] [-I dir] [--dry-run]
    uv run --frozen python tools/adopt.py --batch adopt.json [--dry-run]

Each unit is re-checked with `tools/match.py` and must be an exact text match
(`match`). Before anything is copied, `tools/interfaces.py` audits the function
interfaces of every canonical TU together with the batch candidates; a unit is
refused if its source introduces a finding over the canonical tree plus the units
adopted before it (`--dry-run` reports it the same way). Adoption then:
- copies the C file to `src/units/<image>/<unit>.c`; candidate-local headers go
  beside it, headers found through `-I` go to `include/` (replacing an existing
  canonical header requires `--replace-header include/<name>`);
- appends `config/matches.json` rows, the TU profile and the image binding;
- splits the owning `asm` subsegment in `config/shiren2.jp.yaml` into
  `c` + (zero padding `bin` if the object leaves a gap) + `asm` tail;
- re-checks the canonical copy without extra include paths, and restores every
  touched file if anything fails.

A batch file is a JSON list of objects with keys `functions` (list or comma
string), `source`, optional `profile`, `includes`, `replace_headers`, `name`,
`evidence`. Adoption grants no credit: build, verify, review and promote next.
"""
from __future__ import annotations

import argparse
import difflib
import json
import re
import shutil
import sys
import time
from dataclasses import dataclass, field
from pathlib import Path

from interfaces import Gate, collect
from match import DEFAULT_PROFILES, SUBSEGMENT, Elf, accepted_build, check, resolve_targets, symbol_addresses, yaml_subsegments
from rom import CANONICAL_ROM, PROJECT

CONFIG_FILES = ("config/matches.json", "config/compiler_profiles.json", "config/images.json", "config/shiren2.jp.yaml",
                "config/symbol_aliases.ld")
TAIL_PREFIX = {"resident": "resident_tail", "main_14400": "main_14400_tail"}
DATA_TAIL_PREFIX = {("resident", "data"): "resident_tail", ("resident", "rodata"): "resident_tail",
                    ("main_14400", "data"): "main_data", ("main_14400", "rodata"): "main_rodata"}
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


def _owner(lines: list[str], rom: int) -> tuple[int, int, str, str] | None:
    return max((p for p in yaml_subsegments(lines) if p[1] <= rom), key=lambda p: (p[1], p[0]), default=None)


def _carve(text: str, rom: int, end: int, owner_kinds: set[str], new: list[str], tail_prefix: str) -> str:
    """Insert `new` subsegment lines for [rom, end) into the owning subsegment, plus a tail if bytes remain."""
    lines = text.splitlines(keepends=True)
    owner = _owner(lines, rom)
    if owner is None or owner[2] not in owner_kinds:
        raise ValueError(f"ROM 0x{rom:X} is not inside a {'/'.join(sorted(owner_kinds))} subsegment (owner: {owner})")
    following = [p for p in yaml_subsegments(lines) if p[0] > owner[0]]
    if not following or following[0][1] < end:
        raise ValueError(f"Next subsegment after {owner[3]} starts before the unit's natural end 0x{end:X}")
    indent = SUBSEGMENT.match(lines[owner[0]])["indent"]
    rendered = [f"{indent}- {line}\n" for line in new]
    if end < following[0][1]:
        rendered.append(f"{indent}- [0x{end:X}, {owner[2]}, {tail_prefix}_{end:x}]\n")
    if owner[1] == rom:
        lines[owner[0]:owner[0] + 1] = rendered
    else:
        lines[owner[0] + 1:owner[0] + 1] = rendered
    return "".join(lines)


def split_yaml(text: str, image_id: str, unit: str, rom: int, text_size: int, slot_end: int) -> str:
    """Carve the text unit [rom, slot_end) out of the asm subsegment that currently owns it."""
    new = [f"[0x{rom:X}, c, units/{image_id}/{unit}]"]
    if rom + text_size < slot_end:
        new.append(f"{{start: 0x{rom + text_size:X}, type: bin, name: {unit}_padding, linker_section_order: .text}}")
    return _carve(text, rom, slot_end, {"asm"}, new, TAIL_PREFIX[image_id])


def split_data_yaml(text: str, image_id: str, unit: str, section: str, rom: int, size: int) -> str:
    """Carve a C unit's `.data`/`.rodata` [rom, rom+size) out of the owning subsegment of the same kind.

    splat emits every data-kind piece of a segment before every rodata-kind piece, so a
    `.data` carve inside a `rodata` range (or the reverse) would reorder bytes.
    """
    kind = section.lstrip(".")
    lines = text.splitlines(keepends=True)
    owner = _owner(lines, rom)
    if owner is None or owner[2] != kind:
        raise ValueError(f"{section} at ROM 0x{rom:X} must lie inside a `{kind}` subsegment (owner: {owner}); "
                         "declare read-only tables `const` or fix the data/rodata boundary")
    # The asm piece left after the carve is assembled as its own section: doubles inside it are
    # aligned relative to its start, and resident KMC objects' (empty) .rodata entries are 16-aligned.
    following = next((p for p in yaml_subsegments(lines) if p[0] > owner[0]), None)
    end = rom + size
    if following and end < following[1] and end % TAIL_ALIGNMENT[(image_id, kind)]:
        raise ValueError(f"{section} carve ends at ROM 0x{end:X}, leaving an asm tail not aligned to "
                         f"{TAIL_ALIGNMENT[(image_id, kind)]} bytes (needs a section-alignment fix)")
    return _carve(text, rom, end, {kind}, [f"[0x{rom:X}, .{kind}, units/{image_id}/{unit}]"],
                  DATA_TAIL_PREFIX[(image_id, kind)])


def interior_aliases(result, unit: str) -> str:
    """PROVIDE lines for accepted-build labels inside a newly owned data range.

    splat labels every address other code references, so a C table can cover labels
    such as `D_80139B58` (an interior element) that remaining assembly still uses.
    The C object defines only its own names; the rest keep their original addresses.
    """
    if not result.data_sections:
        return ""
    known = symbol_addresses(accepted_build() / "shiren2.elf")
    defined = {s.name for s in Elf((Path(result.out_dir) / "candidate.o").read_bytes()).symbols if s.shndx}
    lines = []
    for record in result.data_sections:
        if record.get("vram") is None:
            continue
        start, end = record["vram"], record["vram"] + record["size"]
        for name, address in sorted(known.items(), key=lambda item: item[1]):
            if start <= address < end and name not in defined and "." not in name:
                lines.append(f"PROVIDE({name} = 0x{address:08X}); /* inside {unit} {record['section']} */\n")
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
        profile = unit.profile or DEFAULT_PROFILES[resolve_targets(unit.functions)[0].image_id]
    except (ValueError, KeyError, IndexError):
        profile = unit.profile  # adopt() reports the unresolvable unit itself
    return {"source": str(unit.source), "profile": profile, "includes": [str(path) for path in unit.includes]}


def adopt(unit: Unit, *, rom: bytes, dry_run: bool, log: list[str], gate: Gate, index: int) -> dict[str, int]:
    targets = resolve_targets(unit.functions)
    image_id = targets[0].image_id
    name = unit.name or targets[0].symbol
    profile = unit.profile or DEFAULT_PROFILES[image_id]
    source_rel = f"src/units/{image_id}/{name}.c"
    destination = PROJECT / source_rel
    matches = json.loads((PROJECT / "config/matches.json").read_text())
    taken = {(m.get("image_id", "resident"), m["symbol"]) for m in matches["functions"]}
    if any((image_id, t.symbol) in taken for t in targets):
        raise ValueError(f"Already matched: {[t.symbol for t in targets if (image_id, t.symbol) in taken]}")
    if destination.exists():
        raise ValueError(f"Canonical source already exists: {source_rel}")
    stamp = time.strftime("%Y%m%dT%H%M%S")
    work = PROJECT / "scratch/omp/adopt" / f"{name}-{stamp}"
    result, _ = check(targets, unit.source, profile, work / "candidate", includes=unit.includes, rom=rom)
    if result.status not in {"match", "match-with-data"}:
        raise ValueError(f"{name}: tools/match.py status {result.status}: {result.problems or result.out_dir}")
    introduced = gate.check(index)
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
    updated = dict(originals)
    rows = json.loads(originals["config/matches.json"])
    evidence = unit.evidence or (f"omp tools/match.py exact natural {result.text_bytes}-byte {profile} text"
                                 f" (source sha256 {result.source['sha256'][:16]}); full-ROM acceptance required")
    for target in targets:
        rows["functions"].append({"image_id": image_id, "symbol": target.symbol, "source": source_rel,
                                  "rom_start": target.rom_start, "vram_start": target.vram_start,
                                  "size": target.size, "evidence": evidence})
    updated["config/matches.json"] = json.dumps(rows, indent=2, ensure_ascii=False) + "\n"
    profiles = json.loads(originals["config/compiler_profiles.json"])
    profiles["tu_profiles"][source_rel] = profile
    updated["config/compiler_profiles.json"] = json.dumps(profiles, indent=2, ensure_ascii=False) + "\n"
    images = json.loads(originals["config/images.json"])
    images["source_bindings"][source_rel] = image_id
    updated["config/images.json"] = json.dumps(images, indent=2, ensure_ascii=False) + "\n"
    updated["config/shiren2.jp.yaml"] = split_yaml(originals["config/shiren2.jp.yaml"], image_id, name,
                                                   targets[0].rom_start, result.text_bytes, slot_end)
    for record in result.data_sections:
        updated["config/shiren2.jp.yaml"] = split_data_yaml(updated["config/shiren2.jp.yaml"], image_id, name,
                                                            record["section"], record["rom_start"], record["size"])
    updated["config/symbol_aliases.ld"] += interior_aliases(result, name)
    copies = {candidate: destination, **headers}
    log.append(f"== {name}: {len(targets)} function(s), {result.instruction_bytes} instruction bytes, "
               f"text {result.text_bytes}/{result.slot_bytes}, profile {profile}")
    for origin, target in copies.items():
        log.append(f"   copy {origin} -> {target.relative_to(PROJECT)}")
    if dry_run:
        for path in CONFIG_FILES:
            log.extend(line.rstrip("\n") for line in difflib.unified_diff(
                originals[path].splitlines(keepends=True), updated[path].splitlines(keepends=True), f"a/{path}", f"b/{path}"))
        gate.admit(index)
        return {"functions": len(targets), "instruction_bytes": result.instruction_bytes}

    replaced = {target: target.read_bytes() for target in copies.values() if target.exists()}
    created: list[Path] = []
    try:
        for origin, target in copies.items():
            target.parent.mkdir(parents=True, exist_ok=True)
            if not target.exists():
                created.append(target)
            shutil.copyfile(origin, target)
        for path in CONFIG_FILES:
            (PROJECT / path).write_text(updated[path], encoding="utf-8")
        # A relocated header can resolve differently from canonical paths; with no moved
        # headers the canonical compile has identical inputs, and the full build re-proves it.
        if headers or result.data_sections:
            final, _ = check(targets, destination, profile, work / "canonical", rom=rom)
            if final.status != result.status:
                raise ValueError(f"{name}: canonical copy no longer matches ({final.status}): {final.problems or final.out_dir}")
    except BaseException:
        for path in CONFIG_FILES:
            (PROJECT / path).write_text(originals[path], encoding="utf-8")
        for target in created:
            target.unlink(missing_ok=True)
        for target, content in replaced.items():
            target.write_bytes(content)
        raise
    gate.admit(index)
    return {"functions": len(targets), "instruction_bytes": result.instruction_bytes}


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
    parser.add_argument("--dry-run", action="store_true")
    args = parser.parse_args()
    if args.batch:
        units = [Unit(functions=item["functions"].split(",") if isinstance(item["functions"], str) else list(item["functions"]),
                      source=Path(item["source"]), profile=item.get("profile"),
                      includes=[Path(p) for p in item.get("includes", [])], replace_headers=item.get("replace_headers", []),
                      name=item.get("name"), evidence=item.get("evidence"))
                 for item in json.loads(args.batch.read_text())]
    elif args.functions and args.source:
        units = [Unit(args.functions.split(","), args.source, args.profile, args.includes, args.replace_header, args.name, args.evidence)]
    else:
        parser.error("give FUNCTIONS SOURCE or --batch FILE")
    rom = CANONICAL_ROM.read_bytes()
    gate = Gate(collect(PROJECT, [interface_candidate(unit) for unit in units]))
    log: list[str] = []
    totals = {"functions": 0, "instruction_bytes": 0}
    failures = []
    for index, unit in enumerate(units):
        try:
            counts = adopt(unit, rom=rom, dry_run=args.dry_run, log=log, gate=gate, index=index)
            totals = {key: totals[key] + counts[key] for key in totals}
        except (ValueError, OSError, KeyError) as error:
            failures.append(f"{','.join(unit.functions)}: {error}")
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
