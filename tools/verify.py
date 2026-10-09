#!/usr/bin/env python3
"""Verify complete pilot evidence; promotion additionally requires current source."""
from __future__ import annotations

import argparse
import fcntl
import json
import os
import subprocess
import tempfile
from pathlib import Path
from typing import Any

import yaml

from certification import REQUIRED_ARTIFACTS, REQUIRED_RECEIPT_FIELDS, build_graph, c_attributions, check_elf, check_elf_rom, checked_path, compiler_environment, expected_commands, file_inventory, intermediate_keys, object_commands, read_json, sha256, splat_options, tool_profile, runtime_fingerprint, image_evidence, image_matches, reproduce_c_objects_and_link, linked_image_records, retained_trial
from evidence import COMPILED_KINDS, require_inventory, validate_attributions, validate_matches
from rom import CANONICAL_ROM, EXPECTED_SHA256, PROJECT, attest
from workspace import verify_manifest


def verify_receipt(path: Path, document: dict[str, Any] | None = None, *, require_current: bool = True, tool_root: Path = PROJECT, reference: Path = CANONICAL_ROM) -> dict[str, Any]:
    before_digest = sha256(path) if document is None else None
    receipt = document if document is not None else read_json(path)
    require_inventory(receipt, REQUIRED_RECEIPT_FIELDS, "receipt fields")
    if type(receipt.get("schema_version")) is not int or receipt.get("schema_version") != 3 or receipt.get("status") != "byte-identical" or receipt.get("target_sha256") != EXPECTED_SHA256:
        raise ValueError("Receipt does not attest the expected target with current v3 evidence")
    directory = path.parent.resolve()
    require_inventory(receipt["artifacts"], REQUIRED_ARTIFACTS, "artifacts")
    for name, expected in receipt["artifacts"].items():
        if sha256(checked_path(directory, name)) != expected:
            raise ValueError(f"Changed artifact: {name}")
    if yaml.safe_load((directory / "splat.override.yaml").read_text()) != splat_options(directory, reference):
        raise ValueError("Splat paths/options disagree with the private build contract")
    manifest = read_json(directory / "input-manifest.json")
    if receipt["input_manifest"] != manifest:
        raise ValueError("Receipt input manifest differs from frozen evidence")
    verify_manifest(directory / "source", manifest)
    if require_current:
        verify_manifest(PROJECT, manifest)
    report = attest(checked_path(directory, "shiren2.z64").read_bytes())
    if receipt["rom"] != report or attest(reference.read_bytes()) != report:
        raise ValueError("ROM metadata disagrees with the original target")
    profile = tool_profile(tool_root, directory / "source")
    if receipt["profile"] != profile:
        raise ValueError("Changed compiler/tool profile")
    if receipt["runtime"] != runtime_fingerprint():
        raise ValueError("Changed Python/package runtime")
    if receipt["compiler_environment"] != compiler_environment(directory, profile):
        raise ValueError("Compiler environment disagrees with profile")
    graph = build_graph(directory)
    if receipt["graph"] != graph:
        raise ValueError("Receipt graph disagrees with linker/source declarations")
    matches = read_json(directory / "source/config/matches.json")["functions"]
    inventory, inventory_report = image_evidence(directory / "source", reference.read_bytes())
    inventory_report["linked_images"] = linked_image_records(directory, inventory, reference.read_bytes(), graph=graph)
    if receipt["image_inventory"] != inventory_report:
        raise ValueError("Original image inventory disagrees with frozen source/ROM evidence")
    counts = image_matches(matches, inventory)
    generated_keys, compiled_keys = intermediate_keys(graph, matches, directory / "source", observed=receipt["generated"])
    require_inventory(receipt["generated"], generated_keys, "generated inputs")
    require_inventory(receipt["compiled"], compiled_keys, "compiler intermediates")
    require_inventory(receipt["objects"], set(graph), "objects")
    for name, expected in receipt["objects"].items():
        if sha256(checked_path(directory, name)) != expected:
            raise ValueError(f"Stale object: {name}")
    if receipt["objects"] != {f"obj/{name}": digest for name, digest in file_inventory(directory / "obj").items()}:
        raise ValueError("Changed or omitted object evidence inventory")
    if receipt["generated"] != file_inventory(directory / "generated"):
        raise ValueError("Changed or omitted generated input inventory")
    if receipt["compiled"] != file_inventory(directory / "compiled"):
        raise ValueError("Changed or omitted compiled evidence inventory")
    c_sources = {item["source"] for item in graph.values() if item["source_kind"] in COMPILED_KINDS}
    require_inventory(receipt["dependencies"], c_sources, "C dependencies")
    from build import dependency_inputs
    for source in c_sources:
        dependency = directory / "compiled" / Path(source).with_suffix(".d")
        recorded = dependency_inputs(directory, dependency, manifest, source)
        name = next(name for name, item in graph.items() if item["source_kind"] in COMPILED_KINDS and item["source"] == source)
        with retained_trial(directory, "dependencies") as temporary:
            regenerated = Path(temporary) / "source.d"
            operation = object_commands(name, graph[name], profile)[0]
            call = {"argv": operation, "cwd": str(directory), "environment": compiler_environment(directory, profile),
                "driver_children": "Internal GCC children untraced; flags unchanged"}
            (Path(temporary) / "command.json").write_text(json.dumps(call, indent=2) + "\n")
            with regenerated.open("w", encoding="utf-8") as output:
                completed = subprocess.run(operation, cwd=directory, env=compiler_environment(directory, profile), stdout=output, stderr=subprocess.PIPE)
            (Path(temporary) / "stderr.txt").write_bytes(completed.stderr)
            call["returncode"] = completed.returncode
            (Path(temporary) / "command.json").write_text(json.dumps(call, indent=2) + "\n")
            completed.check_returncode()
            reconstructed = dependency_inputs(directory, regenerated, manifest, source)
        if receipt["dependencies"][source] != recorded or recorded != reconstructed:
            raise ValueError("C dependency closure disagrees with compiler evidence")
    if receipt["commands"] != expected_commands(graph, profile):
        raise ValueError("Compiler/build commands disagree with declared graph")
    # Exact counter keys: C-only manifests carry only the C counters; C++ counters cannot be forged.
    if (receipt["c_matches"] != matches or set(receipt["coverage"]) - {"description"} != set(counts)
            or any(type(receipt["coverage"].get(key)) is not int or receipt["coverage"].get(key) != value for key, value in counts.items())):
        raise ValueError("C coverage disagrees with the accepted match manifest")
    reproduction = reproduce_c_objects_and_link(directory, graph, profile, reference)
    if receipt["reproduction"] != reproduction:
        raise ValueError("C object/full-link reproduction disagrees with receipt")
    attribution = c_attributions(directory, matches, graph, profile, reference.read_bytes(), reproduction=reproduction)
    validate_attributions(matches, receipt["c_attributions"], graph)
    if receipt["c_attributions"] != attribution:
        raise ValueError("C attribution disagrees with actual objects/map/bytes")
    check_elf(directory)
    check_elf_rom(directory, profile)
    if tool_profile(tool_root, directory / "source") != profile or receipt["runtime"] != runtime_fingerprint():
        raise ValueError("External tools/runtime changed during verification")
    verify_manifest(directory / "source", manifest)
    if require_current:
        verify_manifest(PROJECT, manifest)
    if before_digest is not None and sha256(path) != before_digest:
        raise ValueError("Receipt changed during verification")
    return receipt


def promote_receipt(path: Path, *, tool_root: Path = PROJECT, reference: Path = CANONICAL_ROM) -> None:
    path = path.resolve()
    build_root = (PROJECT / "build").resolve()
    if not path.is_relative_to(build_root) or path.name != "receipt.json":
        raise ValueError("Only a canonical project build can be promoted")
    # Cooperating integrations are serialized. Canonical source writes belong
    # to the one integrator; filesystem tools are not an OS access sandbox.
    with (build_root / ".integration.lock").open("a") as lock:
        fcntl.flock(lock, fcntl.LOCK_EX)
        before_digest = sha256(path)
        receipt = verify_receipt(path, require_current=True, tool_root=tool_root, reference=reference)
        pointer = {"schema_version": 3, "receipt": path.relative_to(PROJECT).as_posix(), "receipt_sha256": before_digest}
        with tempfile.NamedTemporaryFile(mode="w", encoding="utf-8", dir=build_root, prefix=".latest-", suffix=".json", delete=False) as temporary:
            temporary.write(json.dumps(pointer, indent=2) + "\n")
            temporary.flush()
            os.fsync(temporary.fileno())
            temporary_path = Path(temporary.name)
        try:
            verify_manifest(PROJECT, receipt["input_manifest"])
            verify_manifest(path.parent / "source", receipt["input_manifest"])
            if sha256(path) != before_digest:
                raise ValueError("Receipt changed before publication")
            os.replace(temporary_path, build_root / "latest.json")
        finally:
            temporary_path.unlink(missing_ok=True)


def latest_receipt() -> Path:
    pointer = read_json(PROJECT / "build/latest.json")
    path = checked_path(PROJECT, pointer["receipt"])
    if pointer.get("schema_version") != 3 or pointer.get("receipt_sha256") != sha256(path):
        raise ValueError("Accepted pointer is legacy or changed; supply an explicit current v3 receipt")
    return path


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("receipt", type=Path, nargs="?")
    parser.add_argument("--snapshot-only", action="store_true", help="Verify frozen private trial; do not assert current-tree freshness")
    parser.add_argument("--promote", action="store_true", help="Integrator: atomically publish a current-source receipt")
    parser.add_argument("--tool-root", type=Path, default=PROJECT)
    parser.add_argument("--rom", type=Path, default=CANONICAL_ROM)
    args = parser.parse_args()
    try:
        if args.snapshot_only and args.promote:
            raise ValueError("Snapshot-only verification cannot promote")
        path = args.receipt.resolve() if args.receipt else latest_receipt()
        receipt = verify_receipt(path, require_current=not args.snapshot_only, tool_root=args.tool_root, reference=args.rom)
        if args.promote:
            promote_receipt(path, tool_root=args.tool_root, reference=args.rom)
    except (ValueError, OSError, KeyError, subprocess.CalledProcessError) as error:
        parser.exit(1, f"{error}\n")
    counts = receipt["coverage"]
    scope = "frozen snapshot" if args.snapshot_only else "current source"
    cpp = (f"; {counts['matched_cpp_functions']} C++ function(s), {counts['matched_cpp_bytes']} C++ bytes"
           if "matched_cpp_functions" in counts else "")
    print(f"PASS: exact ROM, image ownership, fresh C objects and full linker reproduction ({scope}); {counts['matched_c_functions']} C function(s), {counts['matched_c_bytes']} C bytes{cpp}")


if __name__ == "__main__":
    main()
