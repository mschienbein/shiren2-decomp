"""Compare a frozen C inventory with original o32 ABI evidence.

Reuse the canonical interfaces classifier without preprocessing mutable sources.
The frozen inventory must supply by-value aggregate layouts explicitly.
"""
from __future__ import annotations

import argparse
from collections import Counter, defaultdict
import json
from pathlib import Path
import re
import sys
import time
from typing import Any

from interfaces import basic_kind


class UnknownLayout(ValueError):
    """The frozen inventory does not establish an ABI layout."""


def _split_params(text: str) -> list[str]:
    depth = 0
    start = 0
    result = []
    for index, char in enumerate(text):
        if char in "([{" :
            depth += 1
        elif char in ")]}":
            depth -= 1
        elif char == "," and depth == 0:
            result.append(text[start:index].strip())
            start = index + 1
    result.append(text[start:].strip())
    return result


def _return_type(record: dict) -> Any:
    if "return_type" in record:
        return record["return_type"]
    signature = record.get("signature")
    if isinstance(signature, dict):
        for key in ("return_type", "returns"):
            if key in signature:
                return signature[key]
    declaration = record.get("declaration", "")
    match = re.search(r"\b" + re.escape(record["symbol"]) + r"\s*\(", declaration)
    return declaration[:match.start()].strip() if match else ""


def _signature(record: dict) -> tuple[list, bool]:
    signature = record.get("signature")
    if isinstance(signature, dict) and "params" in signature:
        if signature.get("prototyped") is False or signature["params"] is None:
            raise UnknownLayout("unspecified parameter list")
        params = signature["params"]
        if not isinstance(params, list):
            raise UnknownLayout("signature.params must be a list")
        return [p for p in params if p != "..." and p != "void"], bool(signature.get("variadic", False) or "..." in params)
    declaration = record.get("declaration", "")
    match = re.search(r"\b" + re.escape(record["symbol"]) + r"\s*\(", declaration)
    if not match:
        raise UnknownLayout("no signature or parseable frozen declaration")
    start = match.end()
    depth = 1
    end = start
    while end < len(declaration) and depth:
        if declaration[end] == "(":
            depth += 1
        elif declaration[end] == ")":
            depth -= 1
        end += 1
    if depth:
        raise UnknownLayout("unbalanced parameter declaration")
    body = declaration[start:end - 1].strip()
    if not body:
        raise UnknownLayout("unspecified parameter list ()")
    params = [] if body == "void" else _split_params(body)
    return [p for p in params if p != "..."], "..." in params


_SCALARS = {
    "char": (1, 1), "short": (2, 2), "short int": (2, 2),
    "int": (4, 4), "long": (4, 4), "long int": (4, 4),
    "long long": (8, 8), "long long int": (8, 8),
    "float": (4, 4), "double": (8, 8),
    "s8": (1, 1), "u8": (1, 1), "s16": (2, 2), "u16": (2, 2),
    "s32": (4, 4), "u32": (4, 4), "s64": (8, 8), "u64": (8, 8),
    "f32": (4, 4), "f64": (8, 8), "size_t": (4, 4), "ptrdiff_t": (4, 4),
    "int8_t": (1, 1), "uint8_t": (1, 1), "int16_t": (2, 2), "uint16_t": (2, 2),
    "int32_t": (4, 4), "uint32_t": (4, 4), "int64_t": (8, 8), "uint64_t": (8, 8),
    "pointer": (4, 4), "_Bool": (1, 1),
}


def _type_text(value: Any) -> str:
    if isinstance(value, dict):
        return str(value.get("type", value.get("type_name", value.get("declaration", ""))))
    return str(value)


def _clean_type(text: str) -> str:
    text = re.sub(r"\b(const|volatile|restrict|__restrict|register|extern|static|inline|__inline__)\b", "", text)
    return " ".join(text.split())


def _layout(value: Any, types: dict, seen: frozenset = frozenset()) -> tuple[int, int, bool]:
    text = _clean_type(_type_text(value))
    if not text:
        raise UnknownLayout("missing type")
    # Function and array parameters decay; the pointee's completeness is irrelevant.
    if "*" in text or "[" in text or "(" in text or (isinstance(value, dict) and value.get("kind") in {"pointer", "array", "function"}):
        return 4, 4, False
    metadata = value if isinstance(value, dict) else {}
    if text in seen:
        raise UnknownLayout(f"cyclic type alias: {text}")
    if text in types:
        target = types[text]
        if isinstance(target, str):
            return _layout(target, types, seen | {text})
        metadata = {**target, **metadata}
        if "alias" in metadata:
            return _layout(metadata["alias"], types, seen | {text})
    size = metadata.get("size", metadata.get("size_bytes"))
    alignment = metadata.get("alignment", metadata.get("align"))
    aggregate = metadata.get("kind") in {"struct", "union", "aggregate"} or text.startswith(("struct ", "union ")) or text == "aggregate"
    if size is not None and alignment is not None:
        if not isinstance(size, int) or not isinstance(alignment, int) or size <= 0 or alignment not in {1, 2, 4, 8}:
            raise UnknownLayout(f"unsupported layout for {text}")
        return size, alignment, aggregate
    scalar = " ".join(w for w in text.split() if w not in {"signed", "unsigned"}) or "int"
    if set(text.split()) <= {"char", "short", "int", "long", "float", "double", "signed", "unsigned"}:
        kind = basic_kind(text.split())
        return kind.width, kind.width, False
    if scalar in _SCALARS:
        return *_SCALARS[scalar], False
    if re.fullmatch(r"enum\s+\w+", text):
        return 4, 4, False
    # A trailing identifier is a parameter name only if its prefix is a known type.
    prefix, _, name = text.rpartition(" ")
    if prefix and re.fullmatch(r"[A-Za-z_]\w*", name) and name not in _SCALARS and name not in {"signed", "unsigned", "void"}:
        try:
            return _layout(prefix, types, seen | {text})
        except UnknownLayout:
            pass
    raise UnknownLayout(f"unknown by-value size/alignment: {text}")


def _return_hidden(return_type: Any, types: dict, seen: frozenset = frozenset()) -> int:
    text = _clean_type(_type_text(return_type))
    if text == "void" or "*" in text:
        return 0
    if text in seen:
        raise UnknownLayout(f"cyclic return type alias: {text}")
    metadata = return_type if isinstance(return_type, dict) else types.get(text, {})
    if isinstance(metadata, str):
        return _return_hidden(metadata, types, seen | {text})
    if metadata.get("kind") == "pointer":
        return 0
    if "alias" in metadata:
        return _return_hidden(metadata["alias"], types, seen | {text})
    if text.startswith(("struct ", "union ")) or text == "aggregate" or metadata.get("kind") in {"struct", "union", "aggregate"}:
        return 1
    return int(_layout(return_type, types)[2])


def _slots(params: list, return_type: Any, types: dict) -> tuple[int, int, set[int]]:
    hidden = _return_hidden(return_type, types)
    slot = hidden
    occupied = set(range(hidden))
    for param in params:
        size, alignment, _ = _layout(param, types)
        if alignment == 8:
            slot = (slot + 1) & ~1
        end = slot + (size + 3) // 4
        occupied.update(range(slot, end))
        slot = end
    return slot, hidden, occupied


def _input_slots(fact: dict, limit: int, *, conservative: bool = False) -> set[int]:
    # The high-water mark proves the last slot, not occupancy of alignment holes.
    slots = {limit - 1} if limit else set()
    registers = "conservative_live_in_registers" if conservative else "live_in_registers"
    slots.update(int(register[1:]) for register in fact.get(registers, []))
    slots.update(item["argument_index"] for item in fact.get("live_in_stack", []))
    slots.update(item["argument_index"] for item in fact.get("live_in_evidence", [])
                 if conservative or item.get("certainty", "proven") == "proven")
    return {slot for slot in slots if 0 <= slot < limit}


def _canonical(source: str, root: Path | None) -> bool:
    path = Path(source)
    if path.is_absolute():
        if root is None:
            return False
        try:
            path = path.relative_to(root.resolve())
        except ValueError:
            return False
    return bool(path.parts) and path.parts[0] == "src" and ".." not in path.parts


def _addresses(fact: dict) -> list[str]:
    addresses = {a for item in fact.get("live_in_evidence", []) for a in item.get("addresses", [])}
    addresses.update(a for item in fact.get("live_in_stack", []) for a in item.get("addresses", []))
    addresses.update(a for item in fact.get("float_evidence", []) for a in item.get("addresses", []))
    return sorted(addresses or {fact["vram_start"]})


def check_inventory(inventory: dict, facts: dict, root: Path | None = None) -> dict:
    """Check frozen records only; never consult mutable source files for type layouts.

    Optional inventory.types maps names to aliases or {kind,size,alignment} layouts.
    Findings coalesce identical contracts, retaining every participating source.
    Uncertain layouts suppress slot findings, not independent return/varargs checks.
    """
    records = []
    for key in ("definitions", "declarations", "records"):
        for record in inventory.get(key, []):
            records.append({**record, "record_kind": key})
    if not records and "functions" in inventory:
        records = [{**r, "record_kind": "functions"} for r in inventory["functions"]]
    types = inventory.get("types", {})
    index = defaultdict(list)
    definitions = defaultdict(set)
    for fact in facts.get("functions", []):
        index[fact["symbol"]].append(fact)
    for record in records:
        if record["record_kind"] in {"definitions", "functions"}:
            definitions[record["symbol"]].add(record["source"])
    findings = []
    uncertainties = []
    unmatched = set()
    checked = 0
    for record in records:
        symbol, source = record["symbol"], record["source"]
        candidates = index[symbol]
        if record.get("image_id"):
            candidates = [f for f in candidates if f.get("image_id") == record["image_id"]]
        if not candidates:
            unmatched.add(symbol)
            continue
        if len(candidates) != 1:
            uncertainties.append({"symbol": symbol, "source": source, "reason": "ambiguous function image", "evidence_addresses": sorted(f["vram_start"] for f in candidates)})
            continue
        fact = candidates[0]
        checked += 1
        base = {"symbol": symbol, "image_id": fact.get("image_id"), "sources": [source], "declaration": record.get("declaration"), "signature": record.get("signature"), "return_type": record.get("return_type"), "evidence_addresses": _addresses(fact)}

        def add(kind: str, message: str, addresses: list | None = None, sources: set | None = None, **details: Any) -> None:
            findings.append({**base, "kind": kind, "severity": "info" if kind == "extra-argument" or kind.startswith("possible-") else "error", "message": message, "evidence_addresses": sorted(set(addresses or base["evidence_addresses"])), "sources": sorted(sources or {source}), **details})

        prologue = fact.get("varargs_prologue")
        unused_varargs_compatible = bool(fact.get("spill_only_varargs"))
        return_type = _return_type(record)
        try:
            params, variadic = _signature(record)
        except UnknownLayout as error:
            uncertainties.append({"symbol": symbol, "source": source, "reason": str(error), "evidence_addresses": _addresses(fact)})
            params, variadic = None, None
        if variadic is not None and variadic != bool(prologue) and not (variadic and unused_varargs_compatible):
            add("variadic-mismatch", "declaration and original varargs prologue disagree", (prologue or {}).get("evidence_addresses"), declared_variadic=variadic, observed_variadic=bool(prologue))
        if params is not None:
            try:
                slots, hidden, occupied = _slots(params, return_type, types)
            except UnknownLayout as error:
                uncertainties.append({"symbol": symbol, "source": source, "reason": str(error), "evidence_addresses": _addresses(fact)})
            else:
                # va_start spills and accesses to variadic arguments are not fixed parameters.
                required = int(prologue.get("first_variadic_slot", prologue.get("fixed_parameter_count", 0))) if prologue else int(fact.get("proven_live_in", fact.get("live_in", 0)))
                conservative = required if prologue else int(fact.get("conservative_live_in", required))
                missing = _input_slots(fact, required) - occupied
                if missing:
                    add("lost-argument", "declared fixed ABI slots do not cover original fixed inputs", declared_slots=slots, required_slots=required, hidden_return_slots=hidden, missing_slots=sorted(missing))
                elif slots > required:
                    add("extra-argument", "declared fixed ABI slots exceed observed fixed inputs; unused parameters are legitimate", declared_slots=slots, required_slots=required, hidden_return_slots=hidden)
                possible_missing = _input_slots(fact, conservative, conservative=True) - occupied
                if not missing and possible_missing:
                    add("possible-lost-argument (indirect)", "indirect-call assumptions may require additional argument slots", declared_slots=slots, required_slots=conservative, missing_slots=sorted(possible_missing))
        if _clean_type(_type_text(return_type)) == "void":
            for kind, consumers in (
                ("void-consumed", [c for c in fact.get("result_consumers", []) if c.get("proven_consumed", c.get("consumed", False))]),
                ("possible-void-consumed (indirect)", [c for c in fact.get("result_consumers", []) if c.get("conservative_consumed", False) and not c.get("proven_consumed", c.get("consumed", False))]),
            ):
                if not consumers:
                    continue
                sources = {source}
                addresses = []
                for consumer in consumers:
                    caller_sources = definitions.get(consumer["caller"], set())
                    sources.update(caller_sources or {consumer.get("source") or f"<original:{consumer['caller']}>"})
                    addresses.extend(consumer.get("evidence_addresses", []))
                    addresses.append(consumer["call_address"])
                add(kind, "original caller consumes a result declared void" if kind == "void-consumed" else "indirect-call assumptions may consume a result declared void", addresses, sources, consumers=consumers)
    grouped = {}
    for finding in findings:
        key = json.dumps({k: v for k, v in finding.items() if k != "sources"}, sort_keys=True)
        if key not in grouped:
            grouped[key] = finding
        else:
            grouped[key]["sources"] = sorted(set(grouped[key]["sources"]) | set(finding["sources"]))
    findings = sorted(grouped.values(), key=lambda f: (f["symbol"], f.get("image_id") or "", f["kind"], json.dumps(f, sort_keys=True)))
    for finding in findings:
        finding["category"] = "canonical-only" if all(_canonical(s, root) for s in finding["sources"]) else "private"
    uncertainties = sorted({json.dumps(u, sort_keys=True): u for u in uncertainties}.values(), key=lambda u: (u["symbol"], u["source"], u["reason"]))
    summary = {"checked_records": checked, "unmatched_symbols": sorted(unmatched), "errors": sum(f["severity"] == "error" for f in findings), "informational": sum(f["severity"] == "info" for f in findings), "uncertainties": len(uncertainties), "by_kind": dict(sorted(Counter(f["kind"] for f in findings).items())), "by_category": {category: dict(sorted(Counter(f["kind"] for f in findings if f["category"] == category).items())) for category in ("canonical-only", "private")}}
    return {"schema_version": 1, "findings": findings, "uncertainties": uncertainties, "summary": summary,
            "inventory_policy": "frozen declarations only; unknown by-value layouts are explicit uncertainties, never guessed",
            "scalar_classifier": "interfaces.basic_kind with fixed o32 typedef widths"}


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--inventory", required=True, type=Path)
    parser.add_argument("--facts", type=Path)
    parser.add_argument("--json", type=Path, required=True)
    parser.add_argument("--canonical-json", type=Path, required=True)
    parser.add_argument("--root", type=Path, default=Path(__file__).resolve().parents[1])
    args = parser.parse_args(argv)
    start = time.perf_counter()
    try:
        inventory = json.loads(args.inventory.read_text())
        if args.facts:
            facts = json.loads(args.facts.read_text())
        else:
            from abi_facts import analyze_project
            facts = analyze_project(args.root)
        report = check_inventory(inventory, facts, args.root)
        canonical = {**report, "findings": [f for f in report["findings"] if f["category"] == "canonical-only"]}
        canonical["summary"] = {"errors": sum(f["severity"] == "error" for f in canonical["findings"]), "informational": sum(f["severity"] == "info" for f in canonical["findings"]), "by_kind": dict(sorted(Counter(f["kind"] for f in canonical["findings"]).items()))}
        for path, value in ((args.json, report), (args.canonical_json, canonical)):
            path.parent.mkdir(parents=True, exist_ok=True)
            path.write_text(json.dumps(value, indent=2, sort_keys=True) + "\n")
    except (OSError, ValueError, KeyError, TypeError) as error:
        print(f"abi_check: {error}", file=sys.stderr)
        return 2
    finally:
        print(f"abi_check runtime_seconds={time.perf_counter() - start:.6f}", file=sys.stderr)
    return int(report["summary"]["errors"] > 0)


if __name__ == "__main__":
    raise SystemExit(main())
