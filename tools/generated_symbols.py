"""Declared compiler-generated function identities, never object rewriting or code rescue.

The unit key is its first catalogue function, not a candidate filename. This lets
match and adopt use the same declaration before and after copying the source.
Runtime identities are deliberately closed: adding a runtime requires a reviewed
code/evidence change, not just another configurable linker alias.
"""
from __future__ import annotations

import hashlib
import json
import re
from pathlib import Path
from typing import Any

CONFIG = "config/generated_symbols.json"
GENERATED = re.compile(r"(?:__tcf_[0-9]+|_GLOBAL_\.[ID]\.[A-Za-z_][A-Za-z0-9_.$]*)\Z")
WRITTEN_GENERATED = re.compile(r"\b(?:__tcf_[0-9]+|_GLOBAL_)\b")
EMPTY = {"schema_version": 1, "units": {}, "runtime_bindings": {}}
TABLE_DISCARD_RULE = "rom-absent-v1"
TABLE_PREFIXES = {".ctors": "_GLOBAL_.I.", ".dtors": "_GLOBAL_.D."}
REVIEWED_RUNTIME = {
    "atexit": {
        "image_id": "main_14400", "symbol": "func_800D8F30",
        "rom_start": 0xABF70, "vram_start": 0x800D8F30, "size": 8,
        "contract": "int atexit(void (*callback)(void)): ignores callback, returns zero; no-op console runtime",
        "catalogued_direct_callers": [{"symbol": "func_801EF7F0_141710", "vram": 0x801EF8C0, "rom": 0x1417E0}],
        "sha256": "adaad653172d31a2481c65220b1c56565bff36fb10341defe5b01cd6fb3850be",
        "evidence": "config/generated-runtime-evidence.json",
        "evidence_sha256": "4890499c5d68db06afddf668a86193d471033f0255b4cab317bf7bd78fc82388",
    }
}


def _read(path: Path) -> Any:
    def unique(pairs: list[tuple[str, Any]]) -> dict[str, Any]:
        result = {}
        for key, value in pairs:
            if key in result:
                raise ValueError(f"Duplicate generated-symbol key: {key}")
            result[key] = value
        return result
    return json.loads(path.read_text(), object_pairs_hook=unique)


def load(root: Path, reference: bytes | None = None) -> dict[str, Any]:
    path = root / CONFIG
    if not path.exists():
        return {**EMPTY, "units": {}, "runtime_bindings": {}}
    document = _read(path)
    if (not isinstance(document, dict) or set(document) != set(EMPTY)
            or type(document["schema_version"]) is not int or document["schema_version"] != 1
            or not isinstance(document["units"], dict) or not isinstance(document["runtime_bindings"], dict)):
        raise ValueError("Invalid generated-symbol declaration")
    catalogue = _read(root / "config/rom_cpu_catalogue.json")["identity"]["functions"]
    by_key = {(row["image_id"], row["symbol"]): row for row in catalogue}
    for name, binding in document["runtime_bindings"].items():
        if name not in REVIEWED_RUNTIME or binding != REVIEWED_RUNTIME[name]:
            raise ValueError(f"Unreviewed runtime binding or wrong address: {name}")
        row = by_key.get((binding["image_id"], binding["symbol"]))
        if row is None or any(row[key] != binding[key] for key in ("rom_start", "vram_start", "size", "sha256")):
            raise ValueError(f"Runtime binding disagrees with catalogue: {name}")
        evidence = root / binding["evidence"]
        if evidence.is_symlink() or hashlib.sha256(evidence.read_bytes()).hexdigest() != binding["evidence_sha256"]:
            raise ValueError(f"Runtime review evidence changed: {name}")
        if reference is not None:
            start, size = binding["rom_start"], binding["size"]
            if hashlib.sha256(reference[start:start + size]).hexdigest() != binding["sha256"]:
                raise ValueError(f"Runtime ROM evidence changed: {name}")
    occupied = set()
    for anchor, unit in document["units"].items():
        required = {"image_id", "functions", "generated", "runtime_names"}
        if (not isinstance(unit, dict) or not required <= set(unit) or set(unit) - required - {"discard_tables"}
                or not isinstance(unit["image_id"], str) or not isinstance(unit["functions"], list)
                or not unit["functions"] or any(not isinstance(s, str) for s in unit["functions"])
                or unit["functions"][0] != anchor or len(set(unit["functions"])) != len(unit["functions"])
                or not isinstance(unit["generated"], dict) or not isinstance(unit["runtime_names"], list)
                or any(not isinstance(s, str) for s in unit["runtime_names"])):
            raise ValueError(f"Invalid generated-symbol unit: {anchor}")
        if "discard_tables" in unit and (unit["discard_tables"] != TABLE_DISCARD_RULE or not unit["generated"]):
            raise ValueError(f"Unreviewed generated table discard rule: {anchor}")
        names = unit["functions"]
        image_rows = sorted((r for r in catalogue if r["image_id"] == unit["image_id"]), key=lambda r: r["rom_start"])
        image_names = [r["symbol"] for r in image_rows]
        if anchor not in image_names or image_names[image_names.index(anchor):image_names.index(anchor) + len(names)] != names:
            raise ValueError(f"Generated unit is not catalogue-adjacent in ROM order: {anchor}")
        keys = {(unit["image_id"], name) for name in names}
        if occupied & keys:
            raise ValueError(f"Overlapping generated-symbol units: {anchor}")
        occupied |= keys
        mapped = unit["generated"]
        if (any(not GENERATED.fullmatch(name) for name in mapped)
                or any(not isinstance(name, str) or name not in names for name in mapped.values())
                or len(set(mapped.values())) != len(mapped)):
            raise ValueError(f"Non-allowlisted or non-bijective generated map: {anchor}")
        runtime = unit["runtime_names"]
        if len(set(runtime)) != len(runtime) or set(runtime) - set(document["runtime_bindings"]):
            raise ValueError(f"Undeclared runtime name in unit: {anchor}")
    return document


def unit_for(document: dict[str, Any], rows: list[dict[str, Any]]) -> dict[str, Any]:
    ordered = sorted(rows, key=lambda row: row["rom_start"])
    keys = {(row.get("image_id", "resident"), row["symbol"]) for row in ordered}
    for unit in document["units"].values():
        declared = {(unit["image_id"], name) for name in unit["functions"]}
        if keys & declared:
            if keys != declared or [r["symbol"] for r in ordered] != unit["functions"]:
                raise ValueError("Generated-symbol unit must be matched/adopted as a complete TU")
            return unit
    return {"generated": {}, "runtime_names": []}


def active_units(document: dict[str, Any], matches: list[dict[str, Any]]) -> dict[str, dict[str, Any]]:
    grouped: dict[str, list[dict[str, Any]]] = {}
    for row in matches:
        grouped.setdefault(row["source"], []).append(row)
    active = {}
    for source, rows in grouped.items():
        keys = {(row.get("image_id", "resident"), row["symbol"]) for row in rows}
        declared = any(keys == {(unit["image_id"], name) for name in unit["functions"]}
                       for unit in document["units"].values())
        # Pending joint declarations do not relabel existing partial owners.
        # Adoption must absorb those owners; generated markers activate strict proof.
        unit = unit_for(document, rows) if declared or any("generated" in row for row in rows) else {"generated": {}, "runtime_names": []}
        reverse = {target: emitted for emitted, target in unit["generated"].items()}
        for row in rows:
            if row.get("generated") != reverse.get(row["symbol"]):
                raise ValueError(f"Manifest generated identity disagrees with declaration: {row['symbol']}")
        if unit["generated"] or unit["runtime_names"]:
            if not source.endswith(".cpp"):
                raise ValueError("Generated-symbol identities require a C++ TU")
            active[source] = {**unit, "rows": rows}
    return active


def validate_active(root: Path, units: dict[str, Any], reference: bytes | None = None) -> None:
    if not units:
        return
    catalogue = {(row["image_id"], row["symbol"]): row for row in
                 _read(root / "config/rom_cpu_catalogue.json")["identity"]["functions"]}
    for unit in units.values():
        for row in unit["rows"]:
            original = catalogue.get((row["image_id"], row["symbol"]))
            if original is None or any(original[key] != row[key] for key in ("rom_start", "vram_start", "size")):
                raise ValueError(f"Generated TU extent disagrees with catalogue: {row['symbol']}")
            if reference is not None:
                body = reference[row["rom_start"]:row["rom_start"] + row["size"]]
                if hashlib.sha256(body).hexdigest() != original["sha256"]:
                    raise ValueError(f"Generated TU catalogue bytes disagree: {row['symbol']}")


def profile_policy(root: Path) -> dict[str, Any]:
    document = load(root)
    units = active_units(document, _read(root / "config/matches.json")["functions"])
    validate_active(root, units)
    return {"declaration": document, "active_units": units}


def linker_flags(profile: dict[str, Any], graph: dict[str, Any]) -> list[str]:
    policy = profile.get("symbol_identities")
    if policy is None:
        return []
    sources = {item["source"] for item in graph.values() if item["source_kind"] in {"c", "cpp"}}
    flags = []
    runtimes = set()
    for source, unit in sorted(policy["active_units"].items()):
        if source not in sources:
            raise ValueError(f"Generated TU missing from link graph: {source}")
        mapped = set(unit["generated"].values())
        for row in unit["rows"]:
            if row["symbol"] in mapped:
                # Local __tcf_N names can repeat between objects. Never rename them;
                # expose only the ROM catalogue address, then prove the object origin.
                flags.append(f"--defsym={row['symbol']}=0x{row['vram_start']:08X}")
        runtimes.update(unit["runtime_names"])
    for name in sorted(runtimes):
        flags.append(f"--defsym={name}={policy['declaration']['runtime_bindings'][name]['symbol']}")
    return flags


def emitted_functions(elf: Any, text: Any) -> dict[str, Any]:
    result = {}
    for symbol in elf.symbols:
        if symbol.shndx == 0:
            continue
        if symbol.type == 2 or (symbol.shndx == text.index and symbol.type == 0 and symbol.bind == 1):
            if symbol.name in result:
                raise ValueError(f"Duplicate emitted function: {symbol.name}")
            result[symbol.name] = symbol
    return result


def object_problems(elf: Any, text: Any, rows: list[dict[str, Any]], unit: dict[str, Any], *, cxx: bool) -> list[str]:
    problems = []
    mapping = unit["generated"]
    if (mapping or unit["runtime_names"]) and not cxx:
        problems.append("Generated-symbol identities require a C++ TU")
    functions = emitted_functions(elf, text)
    collision = set(mapping.values()) & set(functions)
    if collision:
        problems.append(f"Hand-defined catalogue name collision: {sorted(collision)}")
    reverse = {target: emitted for emitted, target in mapping.items()}
    expected = {reverse.get(row["symbol"], row["symbol"]) for row in rows}
    if set(functions) != expected:
        problems.append(f"object defines functions {sorted(functions)}, expected exactly {sorted(expected)}; unmapped extras forbidden")
    base = min(row["rom_start"] for row in rows)
    for row in rows:
        emitted = reverse.get(row["symbol"], row["symbol"])
        symbol = functions.get(emitted)
        if symbol and (symbol.shndx != text.index or symbol.value != row["rom_start"] - base or symbol.size != row["size"]):
            problems.append(f"{row['symbol']} ({emitted}): object offset/size {symbol.value:#x}/{symbol.size} != original {row['rom_start'] - base:#x}/{row['size']}")
    undefined = {s.name for s in elf.symbols if s.shndx == 0 and s.name}
    defined = {s.name for s in elf.symbols if s.shndx != 0}
    if (undefined & set(REVIEWED_RUNTIME)) - set(unit["runtime_names"]):
        problems.append("Undeclared runtime name used by TU")
    if set(unit["runtime_names"]) - undefined or set(REVIEWED_RUNTIME) & defined:
        problems.append("Runtime binding must name an emitted undefined import, never a TU definition")
    return problems


def runtime_addresses(document: dict[str, Any], unit: dict[str, Any], known: dict[str, int]) -> dict[str, int]:
    result = {}
    for name in unit["runtime_names"]:
        binding = document["runtime_bindings"][name]
        address = binding["vram_start"]
        if known.get(binding["symbol"]) != address or (name in known and known[name] != address):
            raise ValueError(f"Runtime binding has wrong link address: {name}")
        result[name] = address
    return result


def generated_evidence(elf: Any, text: Any, rows: list[dict[str, Any]], unit: dict[str, Any], code: bytes, reference: bytes) -> list[dict[str, Any]]:
    functions = emitted_functions(elf, text)
    by_name = {row["symbol"]: row for row in rows}
    base = min(row["rom_start"] for row in rows)
    result = []
    for emitted, target in unit["generated"].items():
        row, symbol = by_name[target], functions.get(emitted)
        expected = reference[row["rom_start"]:row["rom_start"] + row["size"]]
        body = code[symbol.value:symbol.value + symbol.size] if symbol else b""
        exact = bool(symbol and symbol.shndx == text.index and symbol.value == row["rom_start"] - base
                     and symbol.size == row["size"] and len(expected) == row["size"] and body == expected)
        result.append({"symbol": target, "generated": emitted, "size": row["size"],
                       "rom_start": row["rom_start"], "vram_start": row["vram_start"],
                       "object_offset": symbol.value if symbol else None, "matches_original": exact,
                       "reference_sha256": hashlib.sha256(expected).hexdigest(),
                       "linked_sha256": hashlib.sha256(body).hexdigest()})
    return result


def table_discards(root: Path, elf: Any, unit: dict[str, Any], reference: bytes) -> list[dict[str, Any]]:
    """Prove a declared TU's compiler tables absent from the whole attested ROM.

    This authorizes only linker discard, never table placement or object editing.
    All bytes/relocations are re-derived; no candidate-supplied absence evidence.
    """
    tables = [s for s in elf.sections if s.size and any(s.name == name or s.name.startswith(name + ".") for name in TABLE_PREFIXES)]
    if not tables:
        return []
    if unit.get("discard_tables") != TABLE_DISCARD_RULE or not unit.get("generated"):
        raise ValueError("Undeclared .ctors/.dtors discard; no table placement path")
    from images import load_inventory
    inventory = load_inventory(root / "config/images.json")
    catalogue = _read(root / "config/rom_cpu_catalogue.json")["identity"]["functions"]
    rows = {(r["image_id"], r["symbol"]): r for r in catalogue}
    image = inventory.images[unit["image_id"]]
    scans = []
    for scanned in sorted(inventory.images.values(), key=lambda item: item.rom_start):
        body = reference[scanned.rom_start:scanned.rom_end]
        if len(body) != scanned.size or hashlib.sha256(body).hexdigest() != scanned.rom_sha256:
            raise ValueError(f"Table absence needs complete original image bytes: {scanned.image_id}")
        scans.append({"image_id": scanned.image_id, "rom_start": scanned.rom_start,
                      "rom_end": scanned.rom_end, "sha256": scanned.rom_sha256})
    from rom import EXPECTED_SIZE, attest
    if len(reference) != EXPECTED_SIZE:
        raise ValueError("Table absence needs the complete original 32 MiB ROM")
    rom_evidence = attest(reference)
    scanned_rom = {"rom_start": 0, "rom_end": EXPECTED_SIZE, "sha256": rom_evidence["sha256"]}
    text = elf.section(".text")
    if text is None:
        raise ValueError("Generated table has no text owner")
    functions = emitted_functions(elf, text)
    result = []
    for table in tables:
        if (table.name not in TABLE_PREFIXES or elf.section(table.name) != table or table.type != 1
                or table.flags != 3 or table.addr != 0 or table.addralign != 4 or table.size % 4):
            raise ValueError(f"Unsupported compiler table section: {table.name}")
        raw = elf.contents(table)
        rel_sections = [s for s in elf.sections if s.type in {4, 9} and s.info == table.index]
        if (len(raw) != table.size or len(rel_sections) != 1 or rel_sections[0].type != 9
                or rel_sections[0].size != 2 * table.size):
            raise ValueError(f"Compiler table must contain only R_MIPS_32 words: {table.name}")
        relocations = elf.relocations(table)
        if [(offset, kind) for offset, kind, _ in relocations] != [(offset, 2) for offset in range(0, table.size, 4)]:
            raise ValueError(f"Compiler table must contain only R_MIPS_32 words: {table.name}")
        if any(s.shndx == table.index and (s.type != 3 or s.bind != 0 or s.value != 0 or s.size != 0) for s in elf.symbols):
            raise ValueError("Compiler table has a named or exported storage owner")
        if any(symbol.shndx == table.index for section in elf.sections for _, _, symbol in elf.relocations(section)):
            raise ValueError("Compiler table has incoming references and cannot be discarded")
        entries = []
        seen = set()
        for offset, _, symbol in relocations:
            addend = int.from_bytes(raw[offset:offset + 4], "big")
            if symbol.shndx != text.index or symbol.type not in {2, 3}:
                raise ValueError("Compiler table targets foreign storage")
            value = symbol.value + addend
            candidates = [(name, target) for name, target in unit["generated"].items()
                          if name.startswith(TABLE_PREFIXES[table.name]) and name in functions
                          and functions[name].shndx == text.index and functions[name].value == value]
            if len(candidates) != 1:
                raise ValueError("Compiler table must target its mapped generated initializer/finalizer")
            emitted, target = candidates[0]
            row = rows[(unit["image_id"], target)]
            if (target in seen or target not in unit["functions"] or row["rom_start"] < image.rom_start
                    or row["rom_start"] + row["size"] > image.rom_end
                    or row["vram_start"] != row["rom_start"] + image.vram_rom_delta):
                raise ValueError("Compiler table target has duplicate or invalid catalogue ownership")
            seen.add(target)
            pointer = row["vram_start"].to_bytes(4, "big")
            # No excluded ranges: include header/IPL3, all images, the stored
            # tail and unaligned occurrences, even across image boundaries.
            hit = reference.find(pointer)
            if hit != -1:
                raise ValueError(f"Compiler table target word exists in ROM: {target} at {hit:#x}")
            entries.append({"offset": offset, "generated": emitted, "symbol": target, "vram": row["vram_start"]})
        result.append({"section": table.name, "size": table.size, "rule": TABLE_DISCARD_RULE,
                       "object_sha256": hashlib.sha256(raw).hexdigest(), "entries": entries,
                       "scanned_images": scans, "scanned_rom": scanned_rom,
                       "disposition": "discard", "source_credit_bytes": 0})
    return result


def prove_linker_discard(map_text: str, object_name: str, records: list[dict[str, Any]]) -> None:
    """Require each authorized table to be in ld's discarded-input inventory."""
    if not records:
        return
    before, marker, after = map_text.partition("Discarded input sections\n")
    discarded, end, rest = after.partition("Memory Configuration\n")
    if not marker or not end:
        raise ValueError("Missing linker discarded-input evidence")
    for record in records:
        pattern = re.compile(r"^\s+" + re.escape(record["section"])
                             + r"\s+(0x[0-9a-fA-F]+)\s+(0x[0-9a-fA-F]+)\s+"
                             + re.escape(object_name) + r"\s*$", re.MULTILINE)
        hits = pattern.findall(discarded)
        if (len(hits) != 1 or int(hits[0][0], 16) != 0 or int(hits[0][1], 16) != record["size"]
                or pattern.search(before) or pattern.search(rest)):
            raise ValueError(f"Compiler table was not exclusively discarded: {object_name} {record['section']}")
