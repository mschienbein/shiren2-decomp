#!/usr/bin/env python3
"""Rank unmatched catalogued CPU functions by matchability for parallel authors.

Reads the fixed function catalogue, ``config/matches.json``, ``config/images.json``
and the accepted build's generated assembly (no compilation, no receipt replay).
Catalogue boundaries are authoritative: instruction words are sliced from the
build's assembly by catalogue ROM range and checked against catalogue VRAM and
SHA-256 (any disagreement aborts); build-vs-catalogue label differences are
reported under ``boundary_differences``. %hi/%lo symbol names are checked
against the instruction immediates.

Blocked reasons:
  ``main-rodata``  main_14400 function whose code needs compiler-owned rodata that
                   lives in the opaque ``main_rsp_and_data`` carrier: a jump table
                   located in the carrier (``lw %lo(table)`` or a hoisted
                   ``addiu %lo(table)`` base), or a float/double literal: a direct
                   (non-indexed) lwc1/ldc1 of a carrier symbol that no instruction
                   anywhere stores to or takes the address of, located at/above the
                   lowest carrier jump table (``rodata_floor``; .rodata follows .data).
  ``handwritten``  catalogue/assembly handwritten annotation, or cop0 (incl. eret,
                   tlb*), cache, sync, syscall, or break other than the GCC
                   division traps ``break 6``/``break 7``.
Matched functions are excluded from the output (counted in the summary).

Difficulty score (lower is easier), in instruction-equivalents:
  n_instructions + 10*loops + 3*jal_sites + 5*jalr + 12*jump_tables
  + 3*(mult + div) + 15*uses_fpu + 10*uses_double
"""
from __future__ import annotations

import bisect
import argparse
import hashlib
import json
import re
from dataclasses import dataclass
from pathlib import Path
from typing import Any

from packet import Instruction, _parse_functions
from rom import PROJECT

CATALOGUE = PROJECT / "docs/progress-catalogues/2daad37d88dc919dacfba86ee939069d34bd213776d16f06f941bd8e87a03088.json"
DEFAULT_JSON = PROJECT / "scratch/omp/triage/triage.json"
SIZE_BUCKETS = (32, 64, 128, 256, 512)
CARRIER_REGION = "main_14400:data"

_RELOC = re.compile(r"%(hi|lo)\(([A-Za-z_][A-Za-z0-9_]*)\)")
_HEX_SYMBOL = re.compile(r"[A-Za-z]+_([0-9A-Fa-f]+)\Z")
_LO_BASE = re.compile(r"%lo\([A-Za-z_][A-Za-z0-9_]*\)\((\$\w+)\)")
_FIRST_OPERAND = re.compile(r"\S+\s+(\$\w+)")
_FPR_LOADS = {"lwc1", "ldc1"}
_STORES = {"sb", "sh", "sw", "sd", "swl", "swr", "sdl", "sdr", "swc1", "sdc1"}
_ADDRESS = {"addiu", "daddiu"}
_ARGUMENT_REGISTERS = {"$a0", "$a1", "$a2", "$a3"}
_BRANCH_OPCODES = {4, 5, 6, 7, 20, 21, 22, 23}
_REGIMM_BRANCHES = {0, 1, 2, 3, 16, 17, 18, 19}


@dataclass(frozen=True)
class Region:
    name: str
    vram_start: int
    vram_end: int


def _signed16(value: int) -> int:
    return value - 0x10000 if value & 0x8000 else value


def symbol_address(name: str) -> int | None:
    match = _HEX_SYMBOL.fullmatch(name)
    return None if match is None else int(match.group(1), 16)


def load_regions(images_path: Path) -> tuple[list[Region], dict[str, list[tuple[int, int]]], dict[str, int]]:
    """VRAM regions (components first), CPU ROM ranges and ROM->VRAM deltas per image."""
    document = json.loads(images_path.read_text(encoding="utf-8"))
    regions: list[Region] = []
    cpu: dict[str, list[tuple[int, int]]] = {}
    deltas: dict[str, int] = {}
    for component in document.get("components", []):
        regions.append(Region(f"{component['parent_image_id']}:{component['component_id']}", component["storage_vram_start"], component["storage_vram_end"]))
    for image in document["images"]:
        image_id = image["image_id"]
        delta = image["vram_start"] - image["rom_start"]
        deltas[image_id] = delta
        ranges = [(item["rom_start"], item["rom_end"]) for item in image.get("provisional_cpu_ranges", [])]
        cpu[image_id] = ranges
        if not ranges:
            regions.append(Region("overlay", image["vram_start"], image["vram_end"]))
            continue
        cursor = image["rom_start"]
        for start, end in sorted(ranges):
            if start > cursor:
                regions.append(Region(f"{image_id}:data", cursor + delta, start + delta))
            regions.append(Region(f"{image_id}:text", start + delta, end + delta))
            cursor = end
        if cursor < image["rom_end"]:
            regions.append(Region(f"{image_id}:data", cursor + delta, image["rom_end"] + delta))
        for bss in image.get("bss_ranges", []):
            regions.append(Region(f"{image_id}:bss", bss["vram_start"], bss["vram_end"]))
    return regions, cpu, deltas


def region_of(address: int | None, regions: list[Region]) -> str:
    if address is None:
        return "symbolic"
    for region in regions:
        if region.vram_start <= address < region.vram_end:
            return region.name
    return "other"


def _operands(assembly: str) -> list[str]:
    parts = assembly.split(None, 1)
    return [item.strip() for item in parts[1].split(",")] if len(parts) > 1 else []


def _jump_table(index: int, register: str, words: tuple[Instruction, ...], mnemonics: list[str],
                accesses: list[dict[str, Any]]) -> tuple[dict[str, Any] | None, str | None]:
    """Find the table feeding ``jr register`` at ``index``.

    direct:  ``lw register, %lo(table)(base)`` (GCC's usual lui/addu/lw switch).
    hoisted: ``lw register, 0(base)`` where ``base`` (or a non-index operand of the
             addu forming it) was set by ``addiu %lo(table)`` earlier in the function;
             accepted only with that index addu or a spimdisasm ``jtbl_`` name, so a
             function pointer loaded from a global struct stays an indirect jump.
    """
    load = next((j for j in range(index - 1, max(-1, index - 9), -1)
                 if mnemonics[j] in ("lw", "ld") and _operands(words[j].assembly)[:1] == [register]), None)
    if load is None:
        return None, None
    direct = next((item for item in accesses if item["index"] == load), None)
    if direct is not None:
        return direct, "direct"
    found = re.search(r"\((\$\w+)\)$", words[load].assembly.strip())
    if found is None:
        return None, None
    base = found.group(1)
    candidates = {base}
    indexed = False
    for j in range(load - 1, max(-1, load - 9), -1):
        operands = _operands(words[j].assembly)
        if mnemonics[j] in ("addu", "daddu") and operands[0] == base:
            candidates.update(operand for operand in operands[1:] if operand != register)
            indexed = True
            break
    candidates.discard(register)
    table = next((item for item in reversed(accesses)
                  if item["index"] < index and item["mnemonic"] in _ADDRESS and item["destination"] in candidates), None)
    if table is None or not (indexed or table["symbol"].startswith("jtbl_")):
        return None, None
    return table, "hoisted"


def is_literal(load: dict[str, Any], written: set[str], rodata_floor: dict[str, str]) -> bool:
    """Direct FPU load of an initialized symbol never stored/address-taken, at/above .rodata floor."""
    return (load["region"].endswith(":data") and not load["indexed"] and load["symbol"] not in written
            and load["address"] is not None and load["address"] >= rodata_floor.get(load["region"], "00000000"))


def analyse(symbol: str, words: tuple[Instruction, ...], regions: list[Region]) -> dict[str, Any]:
    """Word-level and relocation-level signals for one function (no global context)."""
    start, end = words[0].vram, words[-1].vram + 4
    mnemonics = [word.assembly.split(None, 1)[0] for word in words]
    jal_targets: list[int] = []
    tail_targets: list[int] = []
    indirect_jr: list[tuple[int, int]] = []
    loops = jalr = mult = div = 0
    system: list[str] = []
    uses_fpu = uses_double = False
    frame = 0
    for index, word in enumerate(words):
        w, vram = word.word, word.vram
        op, rs, rt, funct = w >> 26, (w >> 21) & 31, (w >> 16) & 31, w & 63
        if op == 3:
            jal_targets.append(((vram + 4) & 0xF0000000) | ((w & 0x03FFFFFF) << 2))
        elif op == 2:
            target = ((vram + 4) & 0xF0000000) | ((w & 0x03FFFFFF) << 2)
            if start <= target < end:
                loops += target <= vram
            else:
                tail_targets.append(target)
        elif op == 0:
            if funct == 8 and rs != 31:
                indirect_jr.append((index, rs))
            elif funct == 9:
                jalr += 1
            elif funct in (24, 25, 28, 29):
                mult += 1
            elif funct in (26, 27, 30, 31):
                div += 1
            elif funct in (12, 15):
                system.append(mnemonics[index])
            elif funct == 13 and not ((w >> 16) & 0x3FF in (6, 7) and (w >> 6) & 0x3FF == 0):
                system.append(word.assembly.strip())
        elif op in (16, 47):
            system.append(mnemonics[index])
        if op in _BRANCH_OPCODES or (op == 1 and rt in _REGIMM_BRANCHES) or (op == 17 and rs == 8):
            loops += vram + 4 + (_signed16(w & 0xFFFF) << 2) <= vram
        if op == 17 or op in (49, 53, 57, 61):
            uses_fpu = True
            if op in (53, 61) or (op == 17 and (rs in (1, 5, 17, 21) or (rs in (16, 20) and funct == 33))):
                uses_double = True
        if not frame and op in (9, 25) and rs == 29 and rt == 29 and w & 0x8000:
            frame = -_signed16(w & 0xFFFF)

    # Relocations, cross-checked against the instruction words.
    accesses: list[dict[str, Any]] = []
    hi_index: dict[str, list[int]] = {}
    for index, word in enumerate(words):
        for kind, name in _RELOC.findall(word.assembly):
            address = symbol_address(name)
            low = word.word & 0xFFFF
            if address is not None:
                expected = ((address + 0x8000) >> 16) & 0xFFFF if kind == "hi" else address & 0xFFFF
                if low != expected:
                    raise ValueError(f"{symbol}: %{kind}({name}) disagrees with word {word.word:08X} at {word.vram:08X}")
            if kind == "hi":
                hi_index.setdefault(name, []).append(index)
                continue
            mnemonic = mnemonics[index]
            if mnemonic in _ADDRESS:
                operands = [item.strip() for item in word.assembly.split(None, 1)[1].split(",")]
                destination, base = operands[0], operands[1]
            else:
                found = _LO_BASE.search(word.assembly)
                first = _FIRST_OPERAND.match(word.assembly)
                base = found.group(1) if found else None
                destination = first.group(1) if first else None
            indexed = False
            previous = [item for item in hi_index.get(name, []) if item < index]
            if previous and base is not None:
                pattern = re.compile(rf"d?addu\s+\{base},")
                indexed = any(pattern.match(words[between].assembly.strip()) for between in range(previous[-1] + 1, index))
            accesses.append({
                "index": index, "vram": word.vram, "symbol": name, "address": address,
                "region": region_of(address, regions), "mnemonic": mnemonic,
                "destination": destination, "indexed": indexed,
            })

    jump_tables: list[dict[str, Any]] = []
    indirect_jumps: list[str] = []
    for index, register in indirect_jr:
        name = f"${_GPR_NAMES[register]}"
        table, method = _jump_table(index, name, words, mnemonics, accesses)
        if table is None:
            indirect_jumps.append(f"{words[index].vram:08X}:jr {name}")
        else:
            jump_tables.append({"jr_vram": f"{words[index].vram:08X}", "register": name, "table": table["symbol"], "method": method,
                                "address": None if table["address"] is None else f"{table['address']:08X}", "region": table["region"]})

    float_loads = [{"vram": f"{item['vram']:08X}", "symbol": item["symbol"], "region": item["region"],
                    "address": None if item["address"] is None else f"{item['address']:08X}",
                    "double": item["mnemonic"] == "ldc1", "indexed": item["indexed"]}
                   for item in accesses if item["mnemonic"] in _FPR_LOADS]
    data_symbols = sorted({item["symbol"] for item in accesses if not item["symbol"].startswith(("func_", "jtbl_"))}
                          | {name for name in hi_index if not name.startswith(("func_", "jtbl_"))})
    refs_by_region: dict[str, int] = {}
    for name in data_symbols:
        region = region_of(symbol_address(name), regions)
        refs_by_region[region] = refs_by_region.get(region, 0) + 1
    carrier_args = sorted({item["symbol"] for item in accesses
                           if item["mnemonic"] in _ADDRESS and item["region"] == CARRIER_REGION and item["destination"] in _ARGUMENT_REGISTERS})
    return {
        "n_instructions": len(words),
        "jal_targets": jal_targets,
        "tail_targets": tail_targets,
        "jalr": jalr,
        "loops": loops,
        "uses_fpu": uses_fpu,
        "uses_double": uses_double,
        "mult": mult,
        "div": div,
        "system_instructions": system,
        "stack_frame": frame,
        "data_symbols": data_symbols,
        "refs_by_region": dict(sorted(refs_by_region.items())),
        "jump_tables": jump_tables,
        "indirect_jumps": indirect_jumps,
        "float_loads": float_loads,
        "carrier_address_args": carrier_args,
        "_accesses": accesses,
    }


_GPR_NAMES = ("zero", "at", "v0", "v1", "a0", "a1", "a2", "a3", "t0", "t1", "t2", "t3", "t4", "t5", "t6", "t7",
              "s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7", "t8", "t9", "k0", "k1", "gp", "sp", "fp", "ra")


def difficulty(record: dict[str, Any]) -> int:
    return (record["n_instructions"] + 10 * record["loops"] + 3 * record["jal_sites"] + 5 * record["jalr"]
            + 12 * len(record["jump_tables"]) + 3 * (record["mult"] + record["div"])
            + 15 * record["uses_fpu"] + 10 * record["uses_double"])


def default_build() -> Path:
    pointer = json.loads((PROJECT / "build/latest.json").read_text(encoding="utf-8"))
    return (PROJECT / pointer["receipt"]).parent


def triage(build: Path, catalogue_path: Path = CATALOGUE) -> dict[str, Any]:
    catalogue = json.loads(catalogue_path.read_text(encoding="utf-8"))["identity"]["functions"]
    matches = json.loads((PROJECT / "config/matches.json").read_text(encoding="utf-8"))["functions"]
    matched = {(item.get("image_id", "resident"), item["symbol"]) for item in matches}
    regions, cpu, deltas = load_regions(PROJECT / "config/images.json")

    def image_of(rom: int) -> str:
        owners = [image for image, ranges in cpu.items() if any(start <= rom < end for start, end in ranges)]
        if len(owners) != 1:
            raise ValueError(f"ROM {rom:X} is not inside exactly one CPU range")
        return owners[0]

    # The catalogue is the authority for function boundaries; the build's asm may
    # split/merge differently (aliases, padding), so slice its words by ROM range.
    words_by_rom: dict[int, Instruction] = {}
    build_functions: dict[tuple[str, str], tuple[int, int, bool]] = {}
    for path in sorted((build / "generated/asm").rglob("*.s")):
        if "data" in path.relative_to(build / "generated/asm").parts[:1]:
            continue
        for symbol, _assembly, words, handwritten in _parse_functions(path.read_text(encoding="utf-8")):
            key = (image_of(words[0].rom_offset), symbol)
            if key in build_functions:
                raise ValueError(f"Duplicate parsed function {key}")
            build_functions[key] = (words[0].rom_offset, words[-1].rom_offset + 4, handwritten)
            for word in words:
                if words_by_rom.setdefault(word.rom_offset, word) is not word:
                    raise ValueError(f"Duplicate parsed instruction at ROM {word.rom_offset:X}")
    build_spans = sorted(build_functions.values())
    build_starts = [span[0] for span in build_spans]

    entries = {(item["image_id"], item["symbol"]): item for item in catalogue}
    parsed: dict[tuple[str, str], tuple[tuple[Instruction, ...], bool]] = {}
    for key, item in entries.items():
        try:
            words = tuple(words_by_rom[item["rom_start"] + offset] for offset in range(0, item["size"], 4))
        except KeyError as error:
            raise ValueError(f"Catalogue function {key} is not covered by generated assembly") from error
        body = b"".join(word.word.to_bytes(4, "big") for word in words)
        if (image_of(item["rom_start"]), words[0].vram, hashlib.sha256(body).hexdigest()) != (key[0], item["vram_start"], item["sha256"]):
            raise ValueError(f"Assembly disagrees with catalogue for {key}")
        if words[0].vram - words[0].rom_offset != deltas[key[0]]:
            raise ValueError(f"VRAM delta disagrees with images.json for {key}")
        span = build_spans[bisect.bisect_right(build_starts, item["rom_start"]) - 1]
        parsed[key] = (words, span[2])
    if not matched <= set(entries):
        raise ValueError(f"Matched functions absent from catalogue: {sorted(matched - set(entries))}")
    boundary_differences = {
        "catalogue_only": [f"{key[0]}:{key[1]}" for key, item in sorted(entries.items())
                           if build_functions.get(key, (None, None))[:2] != (item["rom_start"], item["rom_start"] + item["size"])],
        "build_only": [f"{key[0]}:{key[1]}@0x{span[0]:X}+{span[1] - span[0]}" for key, span in sorted(build_functions.items()) if key not in entries],
    }

    by_vram = {item["vram_start"]: item["symbol"] for item in catalogue}
    for item in catalogue:
        for alias in item["internal_aliases"]:
            address = symbol_address(alias)
            if address is not None:
                by_vram.setdefault(address, f"{item['symbol']}:{alias}")
    analyses = {key: analyse(key[1], parsed[key][0], regions) for key in entries}

    # Global relocation facts: stored/address-taken symbols and jal callers.
    written: set[str] = set()
    callers: dict[int, set[str]] = {}
    for key, result in analyses.items():
        for access in result["_accesses"]:
            if access["mnemonic"] in _STORES or access["mnemonic"] in _ADDRESS:
                written.add(access["symbol"])
        for target in result["jal_targets"]:
            callers.setdefault(target, set()).add(key[1])
    # .rodata follows .data in each image; the lowest jump table bounds it from below.
    rodata_floor: dict[str, str] = {}
    for result in analyses.values():
        for table in result["jump_tables"]:
            if table["address"] and table["region"].endswith(":data"):
                rodata_floor[table["region"]] = min(rodata_floor.get(table["region"], table["address"]), table["address"])
    jtbl_addresses = sorted(table["address"] for result in analyses.values() for table in result["jump_tables"]
                            if table["region"] == CARRIER_REGION and table["address"])

    starts: dict[str, list[int]] = {}
    for item in catalogue:
        starts.setdefault(item["image_id"], []).append(item["rom_start"])
    for values in starts.values():
        values.sort()

    records: list[dict[str, Any]] = []
    for key, item in sorted(entries.items(), key=lambda pair: (pair[0][0], pair[1]["rom_start"])):
        if key in matched:
            continue
        image_id, symbol = key
        result = analyses[key]
        image_starts = starts[image_id]
        position = image_starts.index(item["rom_start"])
        range_end = next(end for start, end in cpu[image_id] if start <= item["rom_start"] < end)
        slot_end = image_starts[position + 1] if position + 1 < len(image_starts) and image_starts[position + 1] < range_end else range_end
        for load in result["float_loads"]:
            load["literal"] = is_literal(load, written, rodata_floor)
        rodata_kinds: list[str] = []
        if image_id == "main_14400":
            if any(table["region"] == CARRIER_REGION for table in result["jump_tables"]):
                rodata_kinds.append("jump_table")
            if any(load["literal"] and load["region"] == CARRIER_REGION for load in result["float_loads"]):
                rodata_kinds.append("float_literal")
        handwritten = bool(item["handwritten_annotation"] or parsed[key][1] or result["system_instructions"])
        blocked = (["main-rodata"] if rodata_kinds else []) + (["handwritten"] if handwritten else [])
        record = {
            "image_id": image_id,
            "symbol": symbol,
            "rom_start": f"0x{item['rom_start']:X}",
            "vram_start": f"0x{item['vram_start']:08X}",
            "size": item["size"],
            "slot_end_rom": f"0x{slot_end:X}",
            "gap_bytes": slot_end - item["rom_start"] - item["size"],
            "handwritten_annotation": bool(item["handwritten_annotation"]),
            "n_instructions": result["n_instructions"],
            "leaf": not result["jal_targets"] and not result["jalr"],
            "callees": sorted({by_vram.get(target, f"0x{target:08X}") for target in result["jal_targets"]}),
            "jal_sites": len(result["jal_targets"]),
            "tail_calls": sorted({by_vram.get(target, f"0x{target:08X}") for target in result["tail_targets"]}),
            "caller_count": len(callers.get(item["vram_start"], set()) - {symbol}),
            "jalr": result["jalr"],
            "loops": result["loops"],
            "uses_fpu": result["uses_fpu"],
            "uses_double": result["uses_double"],
            "mult": result["mult"],
            "div": result["div"],
            "system_instructions": result["system_instructions"],
            "stack_frame": result["stack_frame"],
            "data_symbols": result["data_symbols"],
            "refs_by_region": result["refs_by_region"],
            "jump_tables": result["jump_tables"],
            "indirect_jumps": result["indirect_jumps"],
            "float_loads": result["float_loads"],
            "carrier_address_args": result["carrier_address_args"],
            "main_rodata_kinds": rodata_kinds,
            "blocked": blocked,
            "ready": not blocked,
        }
        record["difficulty"] = difficulty(record)
        records.append(record)
    records.sort(key=lambda record: (not record["ready"], record["difficulty"], record["image_id"], int(record["rom_start"], 16)))
    return {
        "schema_version": 1,
        "build": str(build.relative_to(PROJECT)) if build.is_relative_to(PROJECT) else str(build),
        "catalogue": str(catalogue_path.relative_to(PROJECT)) if catalogue_path.is_relative_to(PROJECT) else str(catalogue_path),
        "catalogued": len(entries),
        "matched": len(matched),
        "carrier_jump_table_range": [jtbl_addresses[0], jtbl_addresses[-1]] if jtbl_addresses else None,
        "rodata_floor": rodata_floor,
        "boundary_differences": boundary_differences,
        "functions": records,
    }


def _bucket(size: int) -> str:
    return next((f"<={limit}" for limit in SIZE_BUCKETS if size <= limit), f">{SIZE_BUCKETS[-1]}")


def summarise(records: list[dict[str, Any]]) -> dict[str, Any]:
    def tally(items: list[dict[str, Any]]) -> dict[str, int]:
        return {"count": len(items), "bytes": sum(item["size"] for item in items)}

    reasons = sorted({reason for record in records for reason in record["blocked"]})
    kinds = sorted({kind for record in records for kind in record["main_rodata_kinds"]})
    histogram = {}
    for label in [f"<={limit}" for limit in SIZE_BUCKETS] + [f">{SIZE_BUCKETS[-1]}"]:
        bucket = [record for record in records if _bucket(record["size"]) == label]
        histogram[label] = {"all": tally(bucket), "ready": tally([record for record in bucket if record["ready"]])}
    return {
        "unmatched": tally(records),
        "ready": tally([record for record in records if record["ready"]]),
        "blocked": tally([record for record in records if record["blocked"]]),
        "blocked_by_reason": {reason: tally([record for record in records if reason in record["blocked"]]) for reason in reasons},
        "blocked_only_by_reason": {reason: tally([record for record in records if record["blocked"] == [reason]]) for reason in reasons},
        "main_rodata_kinds": {kind: tally([record for record in records if kind in record["main_rodata_kinds"]]) for kind in kinds},
        "size_histogram": histogram,
    }


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--image", choices=("resident", "main_14400"))
    parser.add_argument("--max-size", type=int)
    parser.add_argument("--ready-only", action="store_true")
    parser.add_argument("--top", type=int, default=40, help="Table rows per image (default 40)")
    parser.add_argument("--json", type=Path, default=DEFAULT_JSON)
    parser.add_argument("--build", type=Path, help="Accepted build directory (default: build/latest.json receipt parent)")
    parser.add_argument("--catalogue", type=Path, default=CATALOGUE)
    args = parser.parse_args()
    report = triage((args.build or default_build()).resolve(), args.catalogue.resolve())
    records = [record for record in report["functions"]
               if (args.image is None or record["image_id"] == args.image)
               and (args.max_size is None or record["size"] <= args.max_size)
               and (not args.ready_only or record["ready"])]
    report["filters"] = {"image": args.image, "max_size": args.max_size, "ready_only": args.ready_only}
    report["summary"] = summarise(records)
    report["functions"] = records
    args.json.parent.mkdir(parents=True, exist_ok=True)
    args.json.write_text(json.dumps(report, indent=1) + "\n", encoding="utf-8")

    summary = report["summary"]
    print(f"build {report['build']}  catalogued {report['catalogued']}  matched {report['matched']}  filters {report['filters']}")
    print(f"unmatched {summary['unmatched']['count']} ({summary['unmatched']['bytes']} B)  ready {summary['ready']['count']} ({summary['ready']['bytes']} B)"
          f"  blocked {summary['blocked']['count']} ({summary['blocked']['bytes']} B)")
    for reason, value in summary["blocked_by_reason"].items():
        print(f"  blocked {reason:<12} {value['count']:>5} ({value['bytes']} B)")
    for kind, value in summary["main_rodata_kinds"].items():
        print(f"  main-rodata/{kind:<14} {value['count']:>5} ({value['bytes']} B)")
    print("  size      all(count/B)        ready(count/B)")
    for label, value in summary["size_histogram"].items():
        print(f"  {label:<6} {value['all']['count']:>5} {value['all']['bytes']:>9}   {value['ready']['count']:>5} {value['ready']['bytes']:>9}")
    for image_id in ("resident", "main_14400"):
        rows = [record for record in records if record["image_id"] == image_id and record["ready"]][:args.top]
        if not rows:
            continue
        print(f"\n{image_id}: top {len(rows)} ready (by difficulty)")
        print(f"{'#':>3} {'symbol':<16} {'size':>5} {'score':>5} leaf loops fpu jtbl {'callers':>7}  callees")
        for rank, record in enumerate(rows, 1):
            callees = ",".join(record["callees"][:3]) + (f",+{len(record['callees']) - 3}" if len(record["callees"]) > 3 else "")
            print(f"{rank:>3} {record['symbol']:<16} {record['size']:>5} {record['difficulty']:>5} {'Y' if record['leaf'] else '-':>4}"
                  f" {record['loops']:>5} {'D' if record['uses_double'] else 'F' if record['uses_fpu'] else '-':>3}"
                  f" {len(record['jump_tables']):>4} {record['caller_count']:>7}  {callees}")
    print(f"\njson: {args.json}")


if __name__ == "__main__":
    main()
