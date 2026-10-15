#!/usr/bin/env python3
"""Account for accepted C and C++ instruction bytes and the complete-game ROM map.

Publication belongs to verify.py and the integrator. This tool reads their
accepted pointer and hash-bound independent validation records. It does not
recompile objects on display or promote a receipt. Matched C/C++ is counted against
an immutable provisional CPU function catalogue (frozen separately from changing
source/TU splits). The complete-game section counts every ROM byte by class and status
from the receipt's `rom_map` section, which verify.py rechecks; it never adds C credit.
"""
from __future__ import annotations

import argparse
import fcntl
import hashlib
import json
import os
import re
import tempfile
from dataclasses import dataclass
from datetime import datetime, timezone
from pathlib import Path, PurePosixPath
from typing import Any, Mapping

from certification import (REQUIRED_ARTIFACTS, REQUIRED_RECEIPT_FIELDS, build_graph, c_attributions,
                           elf_code, image_evidence, json_digest, linked_code_section, read_json, rom_map_report, sha256)
from evidence import COMPILED_KINDS, compiled_kind, require_inventory, validate_attributions, validate_matches
from images import ImageInventory, policy_records
from packet import FunctionReference, enumerate_functions
from rom import CANONICAL_ROM, EXPECTED_SHA256, PROJECT, attest
from workspace import input_manifest, verify_manifest
import rommap
import toolchain_library
import generated_symbols

_SHA = re.compile(r"[0-9a-f]{64}\Z")
_SCRATCH = "scratch/progress-accounting-2026-10-05"
FAMILY_LABELS = {"c": "C", "cpp": "C++", "c_and_cpp": "C+C++", "toolchain_library": toolchain_library.CATEGORY}
_REVIEW_FAMILIES = {"gpt": "gpt", "GPT (Astra)": "gpt", "Astra (GPT)": "gpt", "astra": "gpt",
                    "claude": "claude", "anthropic-claude (Opus)": "claude", "opus": "claude"}
_HANDWRITTEN_EVIDENCE = PurePosixPath("docs/evidence/handwritten")


def _digest(value: object) -> str:
    if not isinstance(value, str) or _SHA.fullmatch(value) is None:
        raise ValueError("Expected a lowercase SHA256 digest")
    return value


def _integer(value: object) -> int:
    if type(value) is not int or value < 0:
        raise ValueError("Expected a nonnegative integer")
    return value


def _path(root: Path, relative: str) -> Path:
    name = PurePosixPath(relative)
    if (not relative or name.is_absolute() or ".." in name.parts or "\\" in relative
            or name.as_posix() != relative):
        raise ValueError("Evidence path must be canonical and project-relative")
    path = root
    for component in name.parts:
        path /= component
        if path.is_symlink():
            raise ValueError("Symlinks are not progress evidence")
    if not path.is_file():
        raise ValueError(f"Missing regular evidence file: {relative}")
    return path


def _reference(root: Path, path: Path) -> dict[str, str]:
    relative = path.absolute().relative_to(root.absolute()).as_posix()
    return {"path": relative, "sha256": sha256(_path(root, relative))}


def _read_reference(root: Path, record: dict[str, str]) -> Path:
    path = _path(root, record["path"])
    if sha256(path) != _digest(record["sha256"]):
        raise ValueError(f"Changed progress evidence: {record['path']}")
    return path


def _handwritten_evidence(root: Path, ref: dict[str, str],
                          cache: dict[tuple[str, str], Any]) -> Any:
    if not PurePosixPath(ref["path"]).is_relative_to(_HANDWRITTEN_EVIDENCE):
        raise ValueError("Handwritten evidence must be under docs/evidence/handwritten")
    key = ref["path"], ref["sha256"]
    if key not in cache:
        cache[key] = read_json(_read_reference(root, ref))
        _read_reference(root, ref)
    return cache[key]


def validate_handwritten_schema(document: dict[str, Any]) -> None:
    """Strict v1 schema; unknown fields, booleans as integers and empty evidence fail closed."""
    require_inventory(document, {"schema_version", "target_sha256", "functions"},
                      "handwritten record")
    if type(document["schema_version"]) is not int or document["schema_version"] != 1:
        raise ValueError("Unsupported handwritten record schema")
    _digest(document["target_sha256"])
    if not isinstance(document["functions"], list):
        raise ValueError("Handwritten functions must be a list")
    for entry in document["functions"]:
        require_inventory(entry, {"image_id", "symbol", "rom_start", "size", "expected_sha256",
                                  "sdk_routines", "sdk_source", "evidence", "classifier_family",
                                  "classification_source", "supporting_evidence", "reviews"},
                          "handwritten entry")
        for key in ("image_id", "symbol", "classifier_family"):
            if not isinstance(entry[key], str) or not entry[key].strip():
                raise ValueError(f"Invalid handwritten {key}")
        if entry["classifier_family"] not in {"gpt", "claude"}:
            raise ValueError("Unknown handwritten classifier family")
        start, size = _integer(entry["rom_start"]), _integer(entry["size"])
        if start % 4 or not size or size % 4:
            raise ValueError("Invalid handwritten instruction interval")
        _digest(entry["expected_sha256"])
        if not isinstance(entry["supporting_evidence"], list):
            raise ValueError("Handwritten supporting evidence must be a list")
        for ref in [entry["classification_source"], *entry["supporting_evidence"]]:
            require_inventory(ref, {"path", "sha256"}, "handwritten evidence reference")
            if not isinstance(ref["path"], str) or not ref["path"].strip():
                raise ValueError("Invalid handwritten evidence path")
            _digest(ref["sha256"])
        for key in ("sdk_routines", "evidence"):
            if (not isinstance(entry[key], list) or not entry[key]
                    or any(not isinstance(s, str) or not s.strip() for s in entry[key])):
                raise ValueError(f"Nonempty handwritten {key} required")
        source = entry["sdk_source"]
        if source is not None and (not isinstance(source, str) or not source.strip()):
            raise ValueError("Handwritten SDK source must be text or null for unknown provenance")
        if not isinstance(entry["reviews"], list) or not entry["reviews"]:
            raise ValueError("Nonempty handwritten reviews required")
        seen = set()
        for review in entry["reviews"]:
            require_inventory(review, {"path", "sha256", "family", "items"}, "handwritten review")
            for key in ("path", "family", "items"):
                if not isinstance(review[key], str) or not review[key].strip():
                    raise ValueError(f"Invalid handwritten review {key}")
            _digest(review["sha256"])
            if review["family"] not in {"gpt", "claude"} or review["items"] not in {"functions", "item_verdicts"}:
                raise ValueError("Unknown handwritten review family or item collection")
            if review["path"] in seen:
                raise ValueError("Duplicate handwritten review")
            seen.add(review["path"])


def validate_handwritten(accepted: AcceptedReceipt, denominator: dict[str, Any],
                         document: dict[str, Any]) -> dict[str, dict[str, int]]:
    """Bind catalogue intervals, source classifications and confirming cross-family review items."""
    validate_handwritten_schema(document)
    identity = denominator["identity"]
    if (document["target_sha256"] != EXPECTED_SHA256 or identity["target_sha256"] != EXPECTED_SHA256
            or denominator["denominator_id"] != json_digest(identity)):
        raise ValueError("Handwritten record disagrees with immutable catalogue identity")
    catalogue = {(f["image_id"], f["symbol"]): f for f in identity["functions"]}
    counts = {i["image_id"]: {"functions": 0, "instruction_bytes": 0} for i in identity["images"]}
    seen: set[tuple[str, str]] = set()
    intervals: list[tuple[int, int]] = []
    evidence: dict[tuple[str, str], Any] = {}
    for entry in document["functions"]:
        key = entry["image_id"], entry["symbol"]
        f = catalogue.get(key)
        if f is None:
            raise ValueError(
                f"Handwritten catalogue function missing: {key} in denominator "
                f"{denominator['denominator_id']}; review the classification after a rename, move, split or merge")
        changed = [field for field, catalogue_field in (
            ("rom_start", "rom_start"), ("size", "size"), ("expected_sha256", "sha256"))
            if entry[field] != f[catalogue_field]]
        if changed:
            raise ValueError(
                f"Handwritten entry disagrees with catalogue function: {key} in denominator "
                f"{denominator['denominator_id']} (changed: {', '.join(changed)}); "
                "review the classification after a move, split or merge")
        start, end = entry["rom_start"], entry["rom_start"] + entry["size"]
        if key in seen or any(start < last and first < end for first, last in intervals):
            raise ValueError(f"Duplicate or overlapping handwritten entry: {key}")
        seen.add(key)
        intervals.append((start, end))
        body = accepted.original[start:end]
        if len(body) != entry["size"] or hashlib.sha256(body).hexdigest() != entry["expected_sha256"]:
            raise ValueError(f"Handwritten instruction bytes disagree with original: {key}")
        for match in accepted.document["c_matches"]:
            if (key == (match["image_id"], match["symbol"])
                    or start < match["rom_start"] + match["size"] and match["rom_start"] < end):
                raise ValueError(f"Handwritten entry overlaps accepted C/C++: {key}")
        source_ref = entry["classification_source"]
        source = _handwritten_evidence(accepted.root, source_ref, evidence)
        source_families = set()
        if isinstance(source, dict):
            source_family = _REVIEW_FAMILIES.get(str(source.get("family")))
            if source_family is None:
                raise ValueError(f"Unknown handwritten classification source family: {key}")
            source_families.add(source_family)
            rows = source.get("results")
        else:
            rows = source
        if (not isinstance(rows, list) or any(not isinstance(row, dict)
                or not isinstance(row.get("functions"), list)
                or any(not isinstance(symbol, str) for symbol in row["functions"]) for row in rows)):
            raise ValueError(f"Invalid handwritten classification source rows: {key}")
        classified = [row for row in rows if entry["symbol"] in row["functions"]]
        if (len(classified) != 1 or classified[0].get("status") != "handwritten"
                or classified[0].get("image", entry["image_id"]) != entry["image_id"]):
            raise ValueError(f"Source does not classify handwritten function: {key}")
        for ref in entry["supporting_evidence"]:
            _handwritten_evidence(accepted.root, ref, evidence)
        # Archives omit the ignored leading scratch/ directory. R1's list has no
        # family field; its hashed review binds authorship to the original path.
        origin = "scratch/" + PurePosixPath(source_ref["path"]).relative_to(_HANDWRITTEN_EVIDENCE).as_posix()
        families = set()
        for ref in entry["reviews"]:
            review = _handwritten_evidence(accepted.root, ref, evidence)
            if not isinstance(review, dict) or _REVIEW_FAMILIES.get(str(review.get("reviewer_family"))) != ref["family"]:
                raise ValueError(f"Handwritten reviewer family disagrees: {ref['path']}")
            items = review.get(ref["items"])
            if not isinstance(items, list) or any(not isinstance(item, dict) for item in items):
                raise ValueError(f"Missing handwritten review items: {ref['path']}")
            confirmed = [item for item in items if item.get("symbol") == entry["symbol"]]
            if (len(confirmed) != 1 or confirmed[0].get("verdict") != "confirmed"
                    or (ref["items"] == "item_verdicts" and confirmed[0].get("classification") != "handwritten")):
                raise ValueError(f"Review does not confirm handwritten function: {key}")
            families.add(ref["family"])
            if review.get("records") == origin and "author_reviewed" in review:
                source_families.add(_REVIEW_FAMILIES.get(str(review["author_reviewed"])))
        if source_families != {entry["classifier_family"]}:
            raise ValueError(f"Handwritten classifier family disagrees with source evidence: {key}")
        if not families - source_families:
            raise ValueError(f"Handwritten function lacks cross-family review: {key}")
        counts[entry["image_id"]]["functions"] += 1
        counts[entry["image_id"]]["instruction_bytes"] += entry["size"]
    return counts


def handwritten_accounting(accepted: AcceptedReceipt, denominator: dict[str, Any],
                          coverage: dict[str, Any]) -> dict[str, Any]:
    """Live classification accounting is separate from immutable accepted C/C++ history."""
    reference = _reference(accepted.root, accepted.root / "config/handwritten.json")
    document = read_json(_read_reference(accepted.root, reference))
    classified = validate_handwritten(accepted, denominator, document)
    _read_reference(accepted.root, reference)
    images = {}
    for image_id, row in coverage["images"].items():
        c, cpp = row["matched_c_instruction_bytes"], row.get("matched_cpp_instruction_bytes", 0)
        handwritten = classified[image_id]["instruction_bytes"]
        library = row.get("matched_toolchain_library_instruction_bytes", 0)
        total = row["provisional_catalogued_instruction_bytes"]
        accounted = c + cpp + handwritten + library
        images[image_id] = {"matched_c_instruction_bytes": c, "matched_cpp_instruction_bytes": cpp,
            "classified_handwritten_functions": classified[image_id]["functions"],
            "classified_handwritten_instruction_bytes": handwritten, "accounted_instruction_bytes": accounted,
            "provisional_catalogued_instruction_bytes": total,
            "mapped_catalogue_accounted_percent": 100 * accounted / total if total else None}
        if "matched_toolchain_library_functions" in coverage:
            images[image_id].update(matched_toolchain_library_functions=row["matched_toolchain_library_functions"],
                                   matched_toolchain_library_instruction_bytes=library)
    result = {key: sum(row[key] for row in images.values()) for key in (
        "matched_c_instruction_bytes", "matched_cpp_instruction_bytes", "classified_handwritten_functions",
        "classified_handwritten_instruction_bytes", "accounted_instruction_bytes")}
    if "matched_toolchain_library_functions" in coverage:
        for key in ("matched_toolchain_library_functions", "matched_toolchain_library_instruction_bytes"):
            result[key] = sum(row[key] for row in images.values())
    total = coverage["provisional_catalogued_instruction_bytes"]
    result.update(validated_denominator_id=denominator["denominator_id"],
                  provisional_catalogued_instruction_bytes=total,
                  mapped_catalogue_accounted_percent=100 * result["accounted_instruction_bytes"] / total if total else None,
                  images=images)
    return {"handwritten_classification": reference, "accounting": result}


@dataclass(frozen=True)
class AcceptedReceipt:
    root: Path
    reference: Path
    path: Path
    digest: str
    document: dict[str, Any]
    inventory: ImageInventory
    original: bytes
    pointer_digest: str | None

    @property
    def identity(self) -> dict[str, str]:
        return {"path": self.path.relative_to(self.root).as_posix(), "sha256": self.digest}


def load_receipt(root: Path, reference: Path, identity: dict[str, str],
                 pointer_digest: str | None = None) -> AcceptedReceipt:
    """Recheck inputs used in accounting; full object reproduction is publication's proof."""
    path = _read_reference(root, identity)
    if not path.is_relative_to(root / "build") or path.name != "receipt.json":
        raise ValueError("Only canonical build receipts can be accounted")
    receipt = read_json(path)
    # Receipts accepted before the complete-game map stay accountable for catalogue progress only.
    require_inventory(receipt, REQUIRED_RECEIPT_FIELDS if "rom_map" in receipt else REQUIRED_RECEIPT_FIELDS - {"rom_map"},
                      "receipt fields")
    if (type(receipt["schema_version"]) is not int or receipt["schema_version"] != 3
            or receipt["status"] != "byte-identical" or receipt["target_sha256"] != EXPECTED_SHA256):
        raise ValueError("Only certified v3 target receipts are supported")
    original = reference.read_bytes()
    attest(original)
    directory = path.parent
    require_inventory(receipt["artifacts"], REQUIRED_ARTIFACTS, "artifacts")
    for name, expected in receipt["artifacts"].items():
        if sha256(_path(directory, name)) != _digest(expected):
            raise ValueError(f"Changed accepted artifact: {name}")
    manifest = read_json(directory / "input-manifest.json")
    if manifest != receipt["input_manifest"]:
        raise ValueError("Frozen input manifest disagrees with receipt")
    verify_manifest(directory / "source", manifest)
    if (directory / "shiren2.z64").read_bytes() != original:
        raise ValueError("Accepted ROM differs from fixed original")
    graph = build_graph(directory)
    if graph != receipt["graph"]:
        raise ValueError("Frozen source/linker graph disagrees with receipt")
    inventory, report = image_evidence(directory / "source", original)
    for key, value in report.items():
        if receipt["image_inventory"].get(key) != value:
            raise ValueError("Frozen original image evidence disagrees")
    matches = read_json(directory / "source/config/matches.json")["functions"]
    if matches != receipt["c_matches"]:
        raise ValueError("Frozen accepted matches disagree with receipt")
    counts = validate_matches(matches, policy_records(inventory))
    if set(receipt["coverage"]) - {"description"} != set(counts):
        raise ValueError("Accepted coverage counter inventory disagrees with source languages")
    for key, value in counts.items():
        if type(receipt["coverage"].get(key)) is not int or receipt["coverage"].get(key) != value:
            raise ValueError("Accepted C coverage disagrees with unique instruction ranges")
    validate_attributions(matches, receipt["c_attributions"], graph)
    identities = generated_symbols.load(directory / "source", original)
    generated_units = generated_symbols.active_units(identities, matches)
    if generated_units:
        selected_matches = [row for row in matches if row["source"] in generated_units]
        selected_graph = {name: item for name, item in graph.items() if item["source"] in generated_units}
        observed = c_attributions(directory, selected_matches, selected_graph, receipt["profile"], original,
                                  reproduction=receipt["reproduction"])
        if observed != [row for row in receipt["c_attributions"] if row["source"] in generated_units]:
            raise ValueError("Generated attribution disagrees with emitted object/link evidence")
    attributes = {(a["image_id"], a["symbol"]): a for a in receipt["c_attributions"]}
    sources = {item["source"] for item in graph.values() if item["source_kind"] in COMPILED_KINDS}
    if set(receipt["profile"]["tu_profiles"]) != sources:
        raise ValueError("Accepted per-TU profile assignments disagree")
    toolchain_library.validate_bindings(receipt["profile"])
    for match in matches:
        if toolchain_library.match_family(match) == toolchain_library.FAMILY:
            toolchain_library.validate_source(directory / "source", match["source"])
        attribution = attributes[match["image_id"], match["symbol"]]
        name = attribution["object"]
        profile = receipt["profile"]["tu_profiles"][match["source"]]
        reproduced = receipt["reproduction"]["c_objects"][name]
        if (attribution["profile_id"] != profile or reproduced["profile_id"] != profile
                or reproduced["source"] != match["source"]
                or reproduced["object_sha256"] != receipt["objects"][name]):
            raise ValueError("Accepted C attribution/reproduction identity disagrees")
        expected = original[match["rom_start"]:match["rom_start"] + match["size"]]
        expected_digest = hashlib.sha256(expected).hexdigest()
        linked, _ = elf_code(directory / "shiren2.elf", match["vram_start"], match["size"],
                             section_name=linked_code_section(match["image_id"]))
        if (linked != expected or attribution["reference_sha256"] != expected_digest
                or attribution["linked_sha256"] != expected_digest):
            raise ValueError("Accepted linked C instructions disagree with original")
    if sha256(path) != identity["sha256"]:
        raise ValueError("Accepted receipt changed while reading")
    return AcceptedReceipt(root, reference, path, identity["sha256"], receipt, inventory, original, pointer_digest)


def live_receipt(root: Path = PROJECT, reference: Path = CANONICAL_ROM) -> AcceptedReceipt:
    pointer = _path(root, "build/latest.json")
    digest = sha256(pointer)
    value = read_json(pointer)
    if (set(value) != {"schema_version", "receipt", "receipt_sha256"}
            or type(value["schema_version"]) is not int or value["schema_version"] != 3):
        raise ValueError("A live accepted v3 pointer is required; trials are not progress")
    receipt = load_receipt(root, reference, {"path": value["receipt"], "sha256": value["receipt_sha256"]}, digest)
    if sha256(pointer) != digest:
        raise ValueError("Accepted pointer changed during accounting")
    return receipt


def validate_acceptance(accepted: AcceptedReceipt, reference: dict[str, str],
                        *, require_pointer: bool) -> dict[str, Any]:
    """Root's accepted checkpoint binds publication, independent review and test evidence."""
    root, receipt = accepted.root, accepted.document
    document = read_json(_read_reference(root, reference))
    if (type(document.get("schema_version")) is not int or document.get("schema_version") != 1
            or document.get("kind") != "accepted-v3-integration-checkpoint"
            or document.get("status") != "accepted" or document.get("target_sha256") != EXPECTED_SHA256
            or document.get("accepted_receipt") != accepted.identity
            or document["frozen_manifest"]["sha256"] != receipt["input_manifest"]["sha256"]):
        raise ValueError("Validation record does not certify this accepted receipt")
    promotion = document["promotion"]
    if (promotion.get("serial") is not True or promotion.get("exit_code") != 0
            or promotion.get("source_current_at_acceptance") is not True
            or (require_pointer and promotion.get("latest_pointer_sha256") != accepted.pointer_digest)):
        raise ValueError("Missing successful serial current-source publication evidence")
    coverage = document["coverage"]
    if (type(coverage.get("matched_c_functions")) is not int
            or coverage["matched_c_functions"] != receipt["coverage"]["matched_c_functions"]
            or type(coverage.get("matched_c_instruction_bytes")) is not int
            or coverage["matched_c_instruction_bytes"] != receipt["coverage"]["matched_c_bytes"]):
        raise ValueError("Validation coverage disagrees with receipt")
    for family in ("cpp", "c_and_cpp", toolchain_library.FAMILY):
        if f"matched_{family}_functions" in receipt["coverage"]:
            if (type(coverage.get(f"matched_{family}_functions")) is not int
                    or coverage[f"matched_{family}_functions"] != receipt["coverage"][f"matched_{family}_functions"]
                    or type(coverage.get(f"matched_{family}_instruction_bytes")) is not int
                    or coverage[f"matched_{family}_instruction_bytes"] != receipt["coverage"][f"matched_{family}_bytes"]):
                raise ValueError(f"Validation {FAMILY_LABELS[family]} coverage disagrees with receipt")
    review_ref = document["independent_whole_rom_review"]
    review = read_json(_read_reference(root, review_ref))
    if (not str(review.get("status", "")).startswith("PASS")
            or review.get("receipt_hashes", {}).get(accepted.path.parent.name) != accepted.digest
            or review.get("current_frozen_manifest_sha256") != receipt["input_manifest"]["sha256"]
            or review.get("coverage") != receipt["coverage"]):
        raise ValueError("Independent full-receipt review does not bind accepted evidence")
    suite = document["full_suite"]
    if suite.get("passed") is not True or _integer(suite.get("tests")) == 0:
        raise ValueError("Missing passing acceptance suite")
    _read_reference(root, {"path": suite["log"], "sha256": suite["log_sha256"]})
    for record in document["independent_new_function_reviews"].values():
        semantic = read_json(_read_reference(root, record))
        if not str(semantic.get("status", "")).startswith("PASS"):
            raise ValueError("A referenced independent function review has not passed")
    return document


def catalogue_identity(inventory: ImageInventory,
                       functions: Mapping[tuple[str, str], FunctionReference]) -> dict[str, Any]:
    """One identity schema for receipt-derived and in-flight build catalogues."""
    return {"target_sha256": EXPECTED_SHA256, "scope": "PROVISIONAL mapped CPU function instruction catalogue",
            "complete_game_denominator_bytes": None,
            "images": [{"image_id": image.image_id, "rom_start": image.rom_start, "rom_end": image.rom_end,
                        "vram_start": image.vram_start, "split_status": image.split_status,
                        "provisional_cpu_ranges": [{"rom_start": r.rom_start, "rom_end": r.rom_end}
                                                   for r in image.provisional_cpu_ranges]}
                       for image in sorted(inventory.images.values(), key=lambda i: i.image_id)],
            "functions": [{"image_id": f.image_id, "symbol": f.symbol, "rom_start": f.rom_start,
                           "vram_start": f.vram_start, "size": f.size, "sha256": f.expected_sha256,
                           "handwritten_annotation": f.handwritten_annotation,
                           "internal_aliases": list(f.internal_aliases)}
                          for f in sorted(functions.values(), key=lambda f: (f.image_id, f.rom_start))]}


def audit_catalogue(identity: dict[str, Any], original: bytes) -> dict[str, Any]:
    if (identity["target_sha256"] != EXPECTED_SHA256 or identity["complete_game_denominator_bytes"] is not None
            or identity["scope"] != "PROVISIONAL mapped CPU function instruction catalogue"):
        raise ValueError("A provisional fixed-original catalogue is required")
    images = {image["image_id"]: image for image in identity["images"]}
    if len(images) != len(identity["images"]):
        raise ValueError("Duplicate catalogue image")
    for image in images.values():
        start, end, vram = (_integer(image[key]) for key in ("rom_start", "rom_end", "vram_start"))
        if not 0 <= start < end <= len(original) or start % 4 or end % 4 or vram % 4:
            raise ValueError("Invalid immutable original image interval")
        previous = start
        for region in image["provisional_cpu_ranges"]:
            first, last = _integer(region["rom_start"]), _integer(region["rom_end"])
            if not previous <= first < last <= end or first % 4 or last % 4:
                raise ValueError("Invalid or overlapping provisional CPU ranges")
            previous = last
    names: set[tuple[str, str]] = set()
    ends: dict[str, int] = {}
    statistics: dict[str, dict[str, int]] = {}
    for f in identity["functions"]:
        image = images[f["image_id"]]
        start, size, vram = _integer(f["rom_start"]), _integer(f["size"]), _integer(f["vram_start"])
        if (not size or size % 4 or start % 4 or start < ends.get(f["image_id"], 0)
                or not image["rom_start"] <= start < start + size <= image["rom_end"]
                or vram != start + image["vram_start"] - image["rom_start"]
                or not any(r["rom_start"] <= start < start + size <= r["rom_end"]
                           for r in image["provisional_cpu_ranges"])):
            raise ValueError("Overlapping, unordered or unmapped catalogue function interval")
        for name in [f["symbol"], *f["internal_aliases"]]:
            key = f["image_id"], name
            if key in names:
                raise ValueError("Duplicate catalogue function or alias")
            names.add(key)
        ends[f["image_id"]] = start + size
        body = original[start:start + size]
        if len(body) != size or hashlib.sha256(body).hexdigest() != _digest(f["sha256"]):
            raise ValueError("Catalogue instruction bytes disagree with fixed original")
        if type(f["handwritten_annotation"]) is not bool:
            raise ValueError("Invalid provisional handwritten annotation")
        stats = statistics.setdefault(f["image_id"], {"functions": 0, "instruction_bytes": 0,
                        "zero_words_inside_functions": 0, "all_zero_functions": 0,
                        "handwritten_annotated_functions": 0, "handwritten_annotated_bytes": 0,
                        "internal_aliases": 0, "gap_bytes": 0, "nonzero_gap_bytes": 0})
        stats["functions"] += 1
        stats["instruction_bytes"] += size
        stats["zero_words_inside_functions"] += sum(body[i:i + 4] == b"\0" * 4 for i in range(0, size, 4))
        stats["all_zero_functions"] += not any(body)
        stats["handwritten_annotated_functions"] += f["handwritten_annotation"]
        stats["handwritten_annotated_bytes"] += size if f["handwritten_annotation"] else 0
        stats["internal_aliases"] += len(f["internal_aliases"])
    for image_id, stats in statistics.items():
        functions = [f for f in identity["functions"] if f["image_id"] == image_id]
        for r in images[image_id]["provisional_cpu_ranges"]:
            cursor = r["rom_start"]
            for f in functions:
                if not r["rom_start"] <= f["rom_start"] < f["rom_start"] + f["size"] <= r["rom_end"]:
                    continue
                gap = original[cursor:f["rom_start"]]
                stats["gap_bytes"] += len(gap)
                stats["nonzero_gap_bytes"] += sum(byte != 0 for byte in gap)
                cursor = f["rom_start"] + f["size"]
            gap = original[cursor:r["rom_end"]]
            stats["gap_bytes"] += len(gap)
            stats["nonzero_gap_bytes"] += sum(byte != 0 for byte in gap)
    return {"images": statistics, "functions": sum(s["functions"] for s in statistics.values()),
            "instruction_bytes": sum(s["instruction_bytes"] for s in statistics.values())}


def freeze_catalogue(accepted: AcceptedReceipt, validation: dict[str, str], *,
                     previous: dict[str, str] | None = None,
                     revise_images: tuple[str, ...] = ()) -> dict[str, Any]:
    if bool(previous) != bool(revise_images):
        raise ValueError("An image-scoped revision needs both a previous denominator and selected images")
    catalogue = enumerate_functions(accepted.path, reference=accepted.reference)
    if catalogue.baseline.receipt_sha256 != accepted.digest:
        raise ValueError("Accepted catalogue baseline changed")
    identity = catalogue_identity(catalogue.inventory, catalogue)
    scope = set(revise_images)
    if previous is not None:
        old = read_json(_read_reference(accepted.root, previous))
        if old.get("schema_version") != 1 or old["denominator_id"] != json_digest(old["identity"]):
            raise ValueError("Changed previous denominator identity")
        if old["audit"] != audit_catalogue(old["identity"], accepted.original):
            raise ValueError("Previous catalogue audit disagrees with original rows")
        old_images = {image["image_id"]: image for image in old["identity"]["images"]}
        new_images = {image["image_id"]: image for image in identity["images"]}
        if not scope <= new_images.keys() or old_images.keys() != new_images.keys():
            raise ValueError("Image-scoped revision requires known images and an unchanged image inventory")
        # Existing rows remain authoritative outside the explicitly reviewed images.
        # Re-splatting can fold old entries or expose new labels; that is a separate revision.
        identity["images"] = [new_images[name] if name in scope else old_images[name] for name in sorted(new_images)]
        identity["functions"] = sorted(
            [row for row in old["identity"]["functions"] if row["image_id"] not in scope]
            + [row for row in identity["functions"] if row["image_id"] in scope],
            key=lambda row: (row["image_id"], row["rom_start"]))
    return {"schema_version": 1, "denominator_id": json_digest(identity), "identity": identity,
            "audit": audit_catalogue(identity, accepted.original),
            "origin": {"accepted_receipt": accepted.identity,
                       "source_manifest_sha256": accepted.document["input_manifest"]["sha256"],
                       "validation": validation,
                       "method": "packet.FunctionCatalogue from fully verified frozen original references",
                       **({"previous_denominator": previous, "revised_images": sorted(scope)} if previous else {})}}


def calculate(accepted: AcceptedReceipt, denominator: dict[str, Any]) -> dict[str, Any]:
    """Matched C (and, when the receipt has any, C++ and C+C++) functions/bytes per image and in total.

    A C-only receipt yields exactly the C counters, so its recorded progress is unchanged.
    """
    if type(denominator.get("schema_version")) is not int or denominator.get("schema_version") != 1:
        raise ValueError("Unsupported immutable denominator schema")
    identity = denominator["identity"]
    if denominator["denominator_id"] != json_digest(identity):
        raise ValueError("Changed immutable denominator identity")
    _read_reference(accepted.root, denominator["origin"]["accepted_receipt"])
    _read_reference(accepted.root, denominator["origin"]["validation"])
    if "previous_denominator" in denominator["origin"]:
        _read_reference(accepted.root, denominator["origin"]["previous_denominator"])
    audit = audit_catalogue(identity, accepted.original)
    if denominator["audit"] != audit:
        raise ValueError("Stored catalogue audit disagrees with original rows")
    if accepted.document.get("rom_map") is not None:
        frozen_map = rommap.load(accepted.path.parent / "source", accepted.original)
        rommap.check_catalogue(frozen_map, denominator)
    for image in identity["images"]:
        actual = accepted.inventory.images.get(image["image_id"])
        if actual is None or (actual.rom_start, actual.rom_end, actual.vram_start) != (
                image["rom_start"], image["rom_end"], image["vram_start"]):
            raise ValueError("Immutable denominator image mapping disagrees with accepted evidence")
    by_slot = {(f["image_id"], f["rom_start"]): f for f in identity["functions"]}
    coverage = accepted.document["coverage"]
    # Counter families: `c` alone for a C-only receipt; `c`, `cpp` and their sum once C++ is accepted.
    families = ("c", "cpp", "c_and_cpp") if "matched_cpp_functions" in coverage else ("c",)
    if "matched_toolchain_library_functions" in coverage:
        families += (toolchain_library.FAMILY,)
    images: dict[str, dict[str, Any]] = {}
    for image in identity["images"]:
        count = audit["images"].get(image["image_id"], {})
        row: dict[str, Any] = {"catalogued_functions": count.get("functions", 0),
                               "provisional_catalogued_instruction_bytes": count.get("instruction_bytes", 0) or None}
        for family in families:
            row.update({f"matched_{family}_functions": 0, f"matched_{family}_instruction_bytes": 0,
                        f"mapped_catalogue_{family}_percent": None})
        images[image["image_id"]] = row
    for match in accepted.document["c_matches"]:
        f = by_slot.get((match["image_id"], match["rom_start"]))
        if f is None or (f["vram_start"], f["size"]) != (match["vram_start"], match["size"]):
            raise ValueError("Accepted C coverage is outside the immutable denominator; review an explicit revision")
        a = next(a for a in accepted.document["c_attributions"] if (a["image_id"], a["symbol"]) == (match["image_id"], match["symbol"]))
        if a["reference_sha256"] != f["sha256"]:
            raise ValueError("Accepted C reference differs from original denominator")
        kind = toolchain_library.match_family(match)
        if kind not in families:
            raise ValueError("Accepted C++ match without C++ receipt coverage")
        row = images[match["image_id"]]
        credited_families = {kind, "c_and_cpp"} if kind in {"c", "cpp"} else {kind}
        for family in credited_families & set(families):
            row[f"matched_{family}_functions"] += 1
            row[f"matched_{family}_instruction_bytes"] += match["size"]
    for row in images.values():
        total = row["provisional_catalogued_instruction_bytes"]
        if total:
            for family in families:
                row[f"mapped_catalogue_{family}_percent"] = 100 * row[f"matched_{family}_instruction_bytes"] / total
    total = audit["instruction_bytes"]
    result: dict[str, Any] = {}
    for family in families:
        matched = sum(row[f"matched_{family}_instruction_bytes"] for row in images.values())
        functions = sum(row[f"matched_{family}_functions"] for row in images.values())
        if (matched, functions) != (coverage[f"matched_{family}_bytes"], coverage[f"matched_{family}_functions"]):
            raise ValueError(f"Duplicate or inconsistent accepted {FAMILY_LABELS[family]} numerator")
        result.update({f"matched_{family}_functions": functions, f"matched_{family}_instruction_bytes": matched,
                       f"mapped_catalogue_{family}_percent": 100 * matched / total if total else None})
    generated = [{key: match[key] for key in ("image_id", "symbol", "source", "generated", "size")}
                 for match in accepted.document["c_matches"] if "generated" in match]
    if generated:
        functions, size = len(generated), sum(row["size"] for row in generated)
        if (functions, size) != (coverage.get("matched_generated_cpp_functions"), coverage.get("matched_generated_cpp_bytes")):
            raise ValueError("Generated C++ subset disagrees with receipt")
        result.update({"matched_generated_cpp_functions": functions, "matched_generated_cpp_instruction_bytes": size,
                       "generated_functions": generated})
        for image_id, row in images.items():
            subset = [entry for entry in generated if entry["image_id"] == image_id]
            row.update({"matched_generated_cpp_functions": len(subset),
                        "matched_generated_cpp_instruction_bytes": sum(entry["size"] for entry in subset)})
    result.update({"provisional_catalogued_instruction_bytes": total or None,
                   "complete_game_denominator_bytes": None, "complete_game_percent": None, "images": images})
    return result


def complete_game(accepted: AcceptedReceipt) -> dict[str, Any] | None:
    """Separate research classification and accepted frozen-source coverage.

    The accepted receipt supplies the exact source/pack/replay evidence. A
    research classification, stored padding or preserved binary supplies none.
    """
    section = accepted.document.get("rom_map")
    if section is None:
        return None
    receipt = accepted.document
    if rom_map_report(accepted.path.parent, accepted.original, receipt["c_matches"],
                      commands=receipt["commands"], reproduction=receipt["reproduction"],
                      graph=receipt["graph"], profile=receipt["profile"]) != section:
        raise ValueError("Accepted complete-game ROM map accounting disagrees with frozen evidence")
    cpu = section["cpu_code"]
    total = cpu["instruction_bytes"]
    matched_c, matched_cpp = cpu.get("matched_c_bytes", 0), cpu.get("matched_cpp_bytes", 0)

    def percent(value: int) -> float | None:
        return 100 * value / total if total else None

    return {"map": {"path": section["path"], "sha256": section["sha256"], "map_id": section["map_id"],
                    "partition_sha256": section["partition_sha256"],
                    "evidence_policy_sha256": section["evidence_policy_sha256"],
                    "rules_sha256": section["rules_sha256"], "cpu_catalogue_id": section["cpu_catalogue_id"]},
            "rom_size": section["rom_size"], "classes": section["classes"], "status_bytes": section["status_bytes"],
            "cpu_code": cpu, "asset_compressed": section["asset_compressed"],
            "whole_denominator_closed": False,
            "research_classified_bytes": section["research_classified_bytes"],
            "research_open_bytes": section["research_open_bytes"],
            "research_classified_percent": 100 * section["research_classified_bytes"] / section["rom_size"],
            "accepted_build_source_bytes": section["build_source_bytes"],
            "accepted_build_source_percent": 100 * section["build_source_bytes"] / section["rom_size"],
            "accepted_source_receipt": accepted.identity,
            "matched_c_percent_of_cpu_code": percent(matched_c),
            "matched_c_and_cpp_percent_of_cpu_code": percent(matched_c + matched_cpp)}


def _complete_game_markdown(complete: dict[str, Any] | None) -> list[str]:
    if complete is None:
        return ["**Complete-game denominator: not recorded; this accepted receipt predates the ROM map.**", ""]
    cpu, packed = complete["cpu_code"], complete["asset_compressed"]
    text = [f"**Complete-game denominator: OPEN ({complete['rom_size']:,} ROM bytes).**",
            f"Research-classified: {complete['research_classified_bytes']:,} bytes "
            f"({complete['research_classified_percent']:.4f}%); {complete['research_open_bytes']:,} research-open or located.",
            f"Accepted editable build sources: {complete['accepted_build_source_bytes']:,} bytes "
            f"({complete['accepted_build_source_percent']:.4f}%). Research classification never adds source credit.",
            f"**Matched C over CPU code: {complete['matched_c_percent_of_cpu_code']:.6f}% of {cpu['instruction_bytes']:,} instruction bytes "
            f"(C + C++ {complete['matched_c_and_cpp_percent_of_cpu_code']:.6f}%).**",
            f"Yay0: {packed['roundtrip_streams']:,} of {packed['streams']:,} streams ({packed['stream_bytes']:,} bytes, "
            f"{packed['decoded_bytes']:,} decoded) re-encode byte-identically from frozen editable sources.", "",
            "| Group | Status | Bytes | % of ROM |", "| --- | --- | ---: | ---: |"]
    for key, value in complete["status_bytes"].items():
        group, status = key.split(":")
        text.append(f"| {group} | {status} | {value:,} | {100 * value / complete['rom_size']:.4f}% |")
    text += ["", f"ROM map `{complete['map']['map_id']}` (`{complete['map']['path']}` in the accepted snapshot). "
             "Only exact `cpu_text_function` ranges carry C/C++ credit. Canonical-ROM passthrough, research binaries, padding and unaccepted handwritten proposals earn no editable-source credit.", ""]
    return text


def _history(path: Path) -> list[dict[str, Any]]:
    if not path.exists():
        return []
    events = []
    previous = None
    for line in path.read_text().splitlines():
        if not line.strip():
            raise ValueError("Blank or partial progress-history record")
        value = json.loads(line, object_pairs_hook=_unique_pairs)
        digest = value.pop("event_sha256")
        if value.get("previous_event_sha256") != previous or json_digest(value) != digest:
            raise ValueError("Changed progress-history chain")
        value["event_sha256"] = digest
        previous = digest
        events.append(value)
    return events


def _unique_pairs(pairs: list[tuple[str, Any]]) -> dict[str, Any]:
    result: dict[str, Any] = {}
    for key, value in pairs:
        if key in result:
            raise ValueError(f"Duplicate JSON key: {key}")
        result[key] = value
    return result


def _atomic(root: Path, path: Path, content: str) -> None:
    if path.exists() and path.read_text() == content:
        return
    temporary_root = root / _SCRATCH
    temporary_root.mkdir(parents=True, exist_ok=True)
    with tempfile.NamedTemporaryFile(mode="w", encoding="utf-8", dir=temporary_root, delete=False) as f:
        temporary = Path(f.name)
        f.write(content)
        f.flush()
        os.fsync(f.fileno())
    try:
        os.replace(temporary, path)
    finally:
        temporary.unlink(missing_ok=True)


def _freshness(accepted: AcceptedReceipt) -> dict[str, Any]:
    frozen = accepted.document["input_manifest"]
    current = input_manifest(accepted.root)
    old, new = frozen["files"], current["files"]
    return {"frozen_accepted_inputs_valid": True, "current_tree_matches_accepted_snapshot": frozen == current,
            "accepted_manifest_sha256": frozen["sha256"], "current_manifest_sha256": current["sha256"],
            "changed": sorted(k for k in old.keys() & new.keys() if old[k] != new[k]),
            "added": sorted(new.keys() - old.keys()), "missing": sorted(old.keys() - new.keys())}


def markdown(report: dict[str, Any]) -> str:
    c, freshness = report["coverage"], report["freshness"]
    accounting = report["accounting"]
    split = "matched_cpp_functions" in c
    percent = c["mapped_catalogue_c_percent"]
    text = ["# Matching C/C++ progress" if split else "# Matching C progress", "",
            f"**Accepted C: {c['matched_c_instruction_bytes']:,} instruction bytes across {c['matched_c_functions']} functions.**"]
    if split:
        both = c["mapped_catalogue_c_and_cpp_percent"]
        text += [f"**Accepted C++: {c['matched_cpp_instruction_bytes']:,} instruction bytes across {c['matched_cpp_functions']} functions.**",
                 f"**Accepted C + C++: {c['matched_c_and_cpp_instruction_bytes']:,} instruction bytes across {c['matched_c_and_cpp_functions']} functions"
                 + (f" ({both:.6f}% of the PROVISIONAL mapped CPU catalogue).**" if both is not None else ".**")]
    if "matched_toolchain_library_functions" in c:
        text += [f"**Accepted {toolchain_library.CATEGORY}: {c['matched_toolchain_library_instruction_bytes']:,} instruction bytes across {c['matched_toolchain_library_functions']} functions (zero C/C++ credit).**"]
    if c.get("generated_functions"):
        text += [f"**Generated C++ subset: {c['matched_generated_cpp_instruction_bytes']:,} bytes across {c['matched_generated_cpp_functions']} functions (included above, not extra credit).**",
                 "", "| Classification | Catalogue function | Compiler symbol | Emitting TU | Bytes |",
                 "| --- | --- | --- | --- | ---: |"]
        for row in c["generated_functions"]:
            text.append(f"| generated | `{row['symbol']}` | `{row['generated']}` | `{row['source']}` | {row['size']} |")
        text.append("")
    text += [f"**PROVISIONAL mapped CPU catalogue: {percent:.6f}%{' (C only)' if split else ''}.**" if percent is not None else "**Mapped CPU catalogue percentage: unknown.**",
             "", *_complete_game_markdown(report.get("complete_game")),
             "The catalogue contains original CPU instruction extents, including classified handwritten code. Classification never adds matched C/C++ credit.", ""]
    accounted_percent = accounting["mapped_catalogue_accounted_percent"]
    text += [f"**Classified handwritten: {accounting['classified_handwritten_instruction_bytes']:,} instruction bytes across {accounting['classified_handwritten_functions']} functions (zero C/C++ credit).**",
             f"**Accounted (C + C++ + toolchain library + classified handwritten): {accounting['accounted_instruction_bytes']:,} instruction bytes"
             + (f" ({accounted_percent:.6f}% of the PROVISIONAL catalogue).**" if accounted_percent is not None else "; percentage unknown.**"), "",
             "| Image | Accepted C bytes | Accepted C++ bytes | Toolchain library bytes | Classified handwritten bytes | Accounted bytes | Catalogued instruction bytes (provisional) | C / catalogue | C + C++ / catalogue | Accounted / catalogue |",
             "| --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: |"]
    for image, row in accounting["images"].items():
        total = row["provisional_catalogued_instruction_bytes"]
        counts = f"| {image} | {row['matched_c_instruction_bytes']:,} | {row['matched_cpp_instruction_bytes']:,} | {row.get('matched_toolchain_library_instruction_bytes', 0):,} | {row['classified_handwritten_instruction_bytes']:,} | {row['accounted_instruction_bytes']:,} |"
        compiled = c["images"][image]
        both = compiled.get("mapped_catalogue_c_and_cpp_percent", compiled["mapped_catalogue_c_percent"])
        text.append(f"{counts} {total:,} | {compiled['mapped_catalogue_c_percent']:.6f}% | {both:.6f}% | {row['mapped_catalogue_accounted_percent']:.6f}% |"
                    if total else f"{counts} unknown | unknown | unknown | unknown |")
    text += ["", "Preserved ASM/binary inputs, initialized data and inter-function padding earn zero matching C instruction credit. Zero-word NOPs inside function bodies remain instruction words. Rebuilding the exact 32 MiB ROM does not establish 100% matching C.", "",
             f"Accepted receipt: `{report['accepted_receipt']['path']}` (`{report['accepted_receipt']['sha256']}`).",
             f"Denominator identity: `{report['denominator']['id']}`; immutable catalogue: [{Path(report['denominator']['path']).name}](progress-catalogues/{Path(report['denominator']['path']).name}).",
             f"Validation: [{Path(report['validation']['path']).name}]({Path(report['validation']['path']).name}) (`{report['validation']['sha256']}`).", "",
             f"Reviewed handwritten record: `{report['handwritten_classification']['path']}` (`{report['handwritten_classification']['sha256']}`). Validated against denominator `{accounting['validated_denominator_id']}`. Live classification accounting is revalidated separately; accepted C/C++ history is unchanged.", "",
             "Frozen accepted evidence remains valid. " + ("Current source matches that snapshot." if freshness['current_tree_matches_accepted_snapshot'] else "**Current source differs from the accepted snapshot; working changes receive no additional credit.**"),
             "Display rechecks receipt/hash, frozen source, original/linked instructions and stored review/test evidence. Full object rebuilding belongs to publication verification.", "",
             f"Catalogue audit: {report['catalogue_audit']['functions']:,} functions, " + f"{sum(r['gap_bytes'] for r in report['catalogue_audit']['images'].values()):,} excluded gap bytes, " + f"{sum(r['zero_words_inside_functions'] for r in report['catalogue_audit']['images'].values()):,} zero words inside functions. " + f"{sum(r['handwritten_annotated_bytes'] for r in report['catalogue_audit']['images'].values()):,} bytes carry provisional handwritten annotations; these annotations do not add C credit.", "",
             "Record: `uv run --frozen python tools/progress.py record --validation docs/<accepted-validation>.json`.",
             "Display: `uv run --frozen python tools/progress.py show` (`--json` for structured output).",
             "A denominator revision requires `record --revise-denominator --reason '<reviewed original-boundary change>'`; original catalogue files and prior history are retained.", ""]
    return "\n".join(text)


def _output(root: Path, directory: Path | None, *, create: bool = False) -> Path:
    destination = directory.absolute() if directory else root / "docs"
    if ".." in destination.parts or not destination.is_relative_to(root):
        raise ValueError("Progress output must stay inside its project")
    if destination != root / "docs" and not destination.is_relative_to(root / _SCRATCH):
        raise ValueError("Progress output must be docs or the private accounting evidence root")
    path = root
    for part in destination.relative_to(root).parts:
        path /= part
        if path.is_symlink():
            raise ValueError("Progress outputs cannot use symlinks")
    if create:
        destination.mkdir(parents=True, exist_ok=True)
    return destination


def record(validation_path: Path, *, root: Path = PROJECT, reference: Path = CANONICAL_ROM,
           revise_denominator: bool = False, reason: str | None = None,
           output_dir: Path | None = None, revise_images: tuple[str, ...] = ()) -> dict[str, Any]:
    if revise_images and not revise_denominator:
        raise ValueError("Selected images require an explicit denominator revision")
    scratch = root / _SCRATCH
    scratch.mkdir(parents=True, exist_ok=True)
    with (scratch / "record.lock").open("a") as lock:
        fcntl.flock(lock, fcntl.LOCK_EX)
        accepted = live_receipt(root, reference)
        validation = _reference(root, validation_path)
        validate_acceptance(accepted, validation, require_pointer=True)
        output = _output(root, output_dir, create=True)
        for name in ["progress.json", "progress-history.jsonl", "PROGRESS.md"]:
            if (output / name).is_symlink():
                raise ValueError("Progress outputs cannot use symlinks")
        history_path = output / "progress-history.jsonl"
        events = _history(history_path)
        old = events[-1] if events else None
        if old is None or revise_denominator:
            if revise_denominator and (not reason or not reason.strip()):
                raise ValueError("An explicit denominator revision needs its reviewed reason")
            if revise_images and old is None:
                raise ValueError("An image-scoped revision needs existing denominator history")
            denominator = freeze_catalogue(accepted, validation,
                previous=old["denominator"] if revise_images else None, revise_images=revise_images)
            catalogue_path = output / "progress-catalogues" / f"{denominator['denominator_id']}.json"
            catalogue_path.parent.mkdir(parents=True, exist_ok=True)
            if catalogue_path.exists():
                existing = read_json(catalogue_path)
                if existing["identity"] != denominator["identity"]:
                    raise ValueError("An immutable catalogue file cannot be replaced")
                denominator = existing
            else:
                with catalogue_path.open("x", encoding="utf-8") as f:
                    f.write(json.dumps(denominator, indent=2, sort_keys=True) + "\n")
            denominator_ref = _reference(root, catalogue_path)
        else:
            denominator_ref = old["denominator"]
            denominator = read_json(_read_reference(root, denominator_ref))
        counts = calculate(accepted, denominator)
        complete = complete_game(accepted)
        previous_map = next((e["complete_game"]["map"]["map_id"] for e in reversed(events) if e.get("complete_game")), None)
        map_changed = complete is not None and previous_map is not None and complete["map"]["map_id"] != previous_map
        if map_changed and not revise_denominator:
            raise ValueError("The complete-game ROM map changed; record an explicit --revise-denominator with a reviewed reason")
        accounting = handwritten_accounting(accepted, denominator, counts)
        existing = next((event for event in events if event["accepted_receipt"] == accepted.identity
                         and event["denominator"] == denominator_ref), None)
        if existing is None:
            event: dict[str, Any] = {"schema_version": 3, "kind": "accepted-matching-progress",
                "recorded_at": datetime.now(timezone.utc).isoformat(),
                "accepted_receipt": accepted.identity, "validation": validation,
                "denominator": denominator_ref, "denominator_id": denominator["denominator_id"],
                "coverage": counts, "complete_game": complete, "previous_event_sha256": old["event_sha256"] if old else None,
                "matched_c_byte_delta": counts["matched_c_instruction_bytes"] - (old["coverage"]["matched_c_instruction_bytes"] if old else 0),
                "denominator_change": {"previous_id": old["denominator_id"] if old else None,
                                       "new_id": denominator["denominator_id"], "reason": reason if old else "Initial accepted original catalogue"}}
            if map_changed:
                event["rom_map_change"] = {"previous_map_id": previous_map, "new_map_id": complete["map"]["map_id"], "reason": reason}
            if "matched_cpp_instruction_bytes" in counts:
                event["matched_cpp_byte_delta"] = counts["matched_cpp_instruction_bytes"] - (old["coverage"].get("matched_cpp_instruction_bytes", 0) if old else 0)
            event["event_sha256"] = json_digest(event)
        else:
            if existing["coverage"] != counts or existing.get("complete_game") != complete:
                raise ValueError("Stored progress for the same accepted evidence changed")
            event = existing
        if sha256(root / "build/latest.json") != accepted.pointer_digest:
            raise ValueError("Accepted pointer changed before progress recording")
        validate_acceptance(accepted, validation, require_pointer=True)
        if existing is None:
            with history_path.open("a", encoding="utf-8") as f:
                f.write(json.dumps(event, sort_keys=True, separators=(",", ":")) + "\n")
                f.flush()
                os.fsync(f.fileno())
            events.append(event)
        report = {"schema_version": 3, "target_sha256": EXPECTED_SHA256, "accepted_receipt": accepted.identity,
            "validation": validation, "denominator": {"id": denominator["denominator_id"], **denominator_ref},
            "catalogue_audit": denominator["audit"], "coverage": counts, "complete_game": complete,
            "freshness": _freshness(accepted), "history_event_sha256": event["event_sha256"],
            "history_head_sha256": events[-1]["event_sha256"], "recorded_at": event["recorded_at"]}
        report.update(accounting)
        _atomic(root, output / "progress.json", json.dumps(report, indent=2, sort_keys=True) + "\n")
        _atomic(root, output / "PROGRESS.md", markdown(report))
        return {"appended": existing is None, "report": report}


def show(*, root: Path = PROJECT, reference: Path = CANONICAL_ROM,
         output_dir: Path | None = None) -> dict[str, Any]:
    output = _output(root, output_dir)
    report = read_json(_path(root, (output / "progress.json").relative_to(root).as_posix()))
    accepted = live_receipt(root, reference)
    if report["accepted_receipt"] != accepted.identity:
        raise ValueError("Accepted pointer advanced; record its independently accepted validation first")
    validate_acceptance(accepted, report["validation"], require_pointer=True)
    denominator = read_json(_read_reference(root, report["denominator"]))
    counts = calculate(accepted, denominator)
    complete = complete_game(accepted)
    if (report["coverage"] != counts or report.get("complete_game") != complete
            or report["denominator"]["id"] != denominator["denominator_id"]):
        raise ValueError("Stored progress disagrees with original denominator/accepted C/ROM map")
    events = _history(_path(root, (output / "progress-history.jsonl").relative_to(root).as_posix()))
    if not events or events[-1]["event_sha256"] != report["history_head_sha256"]:
        raise ValueError("Progress/history head disagree; record again to recover")
    event = next((e for e in events if e["event_sha256"] == report["history_event_sha256"]), None)
    if (event is None or event["accepted_receipt"] != accepted.identity or event["coverage"] != counts
            or event.get("complete_game") != complete):
        raise ValueError("Progress lacks a matching immutable history event")
    report["freshness"] = _freshness(accepted)
    report.update(handwritten_accounting(accepted, denominator, counts))
    return report


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    sub = parser.add_subparsers(dest="command", required=True)
    update = sub.add_parser("record", help="Integrator: record the live independently accepted pointer")
    update.add_argument("--validation", required=True, type=Path)
    update.add_argument("--revise-denominator", action="store_true")
    update.add_argument("--revise-image", action="append", default=[], metavar="IMAGE",
                        help="With --revise-denominator, revise only these images and retain all other rows")
    update.add_argument("--reason")
    update.add_argument("--output-dir", type=Path, help="Private preview under scratch/progress-accounting-2026-10-05")
    display = sub.add_parser("show", help="Read accepted progress; never rebuild objects")
    display.add_argument("--json", action="store_true")
    display.add_argument("--output-dir", type=Path)
    for command in (update, display):
        command.add_argument("--root", type=Path, default=PROJECT)
        command.add_argument("--rom", type=Path, default=CANONICAL_ROM)
    args = parser.parse_args()
    try:
        if args.command == "record":
            result = record(args.validation.absolute(), root=args.root.resolve(), reference=args.rom,
                            revise_denominator=args.revise_denominator, reason=args.reason, output_dir=args.output_dir, revise_images=tuple(args.revise_image))
            report = result["report"]
            print("Recorded accepted progress." if result["appended"] else "Unchanged accepted progress; history not appended.")
        else:
            report = show(root=args.root.resolve(), reference=args.rom, output_dir=args.output_dir)
        print(json.dumps(report, indent=2, sort_keys=True) if getattr(args, "json", False) else markdown(report))
    except (ValueError, OSError, KeyError, TypeError) as error:
        parser.exit(1, f"{error}\n")


if __name__ == "__main__":
    main()
