#!/usr/bin/env python3
"""Build frozen source with private outputs; publish only through verification."""
from __future__ import annotations

import argparse
import json
import re
import subprocess
import sys
from datetime import datetime, timezone
from pathlib import Path
from typing import Any

import yaml

from certification import REQUIRED_ARTIFACTS, build_graph, c_attributions, check_elf, compiler_environment, expected_commands, file_inventory, object_commands, python_command, read_json, sha256, splat_options, tool_profile, runtime_fingerprint, image_evidence, image_matches, reproduce_c_objects_and_link, linked_image_records
from evidence import COMPILED_KINDS, validate_attributions, validate_matches
from rom import CANONICAL_ROM, EXPECTED_SHA256, PROJECT, attest
from workspace import create_snapshot, verify_manifest


def compare_rom(path: Path, reference: Path = CANONICAL_ROM) -> dict[str, object]:
    expected = reference.read_bytes()
    attest(expected)
    actual = path.read_bytes()
    if actual != expected:
        offset = next((i for i, (a, b) in enumerate(zip(actual, expected)) if a != b), min(len(actual), len(expected)))
        raise ValueError(f"ROM mismatch at offset 0x{offset:X}; actual size={len(actual)}, expected size={len(expected)}")
    return attest(actual)


def write_json(path: Path, value: object) -> None:
    path.write_text(json.dumps(value, indent=2, sort_keys=True) + "\n", encoding="utf-8")


def dependency_inputs(directory: Path, dependency: Path, manifest: dict[str, Any], source: str | None = None) -> list[str]:
    text = dependency.read_text().replace("\\\n", " ")
    if ":" not in text:
        raise ValueError("Compiler returned no dependency target")
    names = text.split(":", 1)[1].split()
    result = set()
    for name in names:
        path = (directory / name).resolve()
        source_root = (directory / "source").resolve()
        if not path.is_relative_to(source_root):
            raise ValueError(f"Compiler dependency outside frozen source: {name}")
        relative = path.relative_to(source_root).as_posix()
        if relative not in manifest["files"] or sha256(path) != manifest["files"][relative]["sha256"]:
            raise ValueError(f"Compiler dependency missing from frozen manifest: {relative}")
        result.add(relative)
    if not result:
        raise ValueError("Empty compiler dependency closure")
    if source is not None and source not in result:
        raise ValueError("Compiler dependency closure omits its declared C source")
    return sorted(result)


def execute_snapshot(directory: Path, tool_root: Path, reference: Path) -> Path:
    """Called in a child interpreter executing the copied build tooling."""
    directory = directory.resolve()
    manifest = read_json(directory / "input-manifest.json")
    verify_manifest(directory / "source", manifest)
    reference_report = attest(reference.read_bytes())
    profile = tool_profile(tool_root, directory / "source")
    inventory, inventory_report = image_evidence(directory / "source", reference.read_bytes())
    runtime = runtime_fingerprint()
    (directory / "tmp").mkdir()
    overrides = splat_options(directory, reference)
    (directory / "splat.override.yaml").write_text(yaml.safe_dump(overrides), encoding="utf-8")
    commands: list[list[str]] = []
    dependencies: dict[str, list[str]] = {}
    environment = compiler_environment(directory, profile)
    with (directory / "build.log").open("w", encoding="utf-8") as log:
        def run(command: list[str], output: Path | None = None) -> None:
            commands.append(command)
            log.write(json.dumps(command) + "\n")
            log.flush()
            if output is None:
                subprocess.run(command, cwd=directory, env=environment, stdout=log, stderr=subprocess.STDOUT, check=True)
            else:
                with output.open("w", encoding="utf-8") as stream:
                    subprocess.run(command, cwd=directory, env=environment, stdout=stream, stderr=log, check=True)

        run([python_command(), "-m", "splat", "split", "source/config/shiren2.jp.yaml", "splat.override.yaml", "--disassemble-all"])
        generated_before = file_inventory(directory / "generated")
        graph = build_graph(directory)
        for name, item in graph.items():
            (directory / name).parent.mkdir(parents=True, exist_ok=True)
            operations = object_commands(name, item, profile)
            if item["source_kind"] in COMPILED_KINDS:
                compiled = directory / "compiled" / Path(item["source"]).with_suffix(".s")
                compiled.parent.mkdir(parents=True, exist_ok=True)
                dependency = compiled.with_suffix(".d")
                run(operations[0], dependency)
                dependencies[item["source"]] = dependency_inputs(directory, dependency, manifest, item["source"])
                for operation in operations[1:]:
                    run(operation)
            else:
                for operation in operations:
                    run(operation)
        for operation in expected_commands(graph, profile)[-2:]:
            run(operation)

    report = compare_rom(directory / "shiren2.z64", reference)
    verify_manifest(directory / "source", manifest)
    if file_inventory(directory / "generated") != generated_before:
        raise ValueError("Generated inputs changed while compiling")
    if tool_profile(tool_root, directory / "source") != profile or runtime_fingerprint() != runtime or attest(reference.read_bytes()) != reference_report:
        raise ValueError("External tool/runtime/ROM inputs changed while building")
    matches = read_json(directory / "source/config/matches.json")["functions"]
    coverage = image_matches(matches, inventory)
    inventory_report["linked_images"] = linked_image_records(directory, inventory, reference.read_bytes(), graph=graph)
    reproduction = reproduce_c_objects_and_link(directory, graph, profile, reference)
    attribution = c_attributions(directory, matches, graph, profile, reference.read_bytes(), reproduction=reproduction)
    validate_attributions(matches, attribution, graph)
    check_elf(directory)
    if commands != expected_commands(graph, profile):
        raise ValueError("Executed commands disagree with the declared graph")
    receipt = {
        "schema_version": 3, "status": "byte-identical", "created_at": datetime.now(timezone.utc).isoformat(),
        "target_sha256": EXPECTED_SHA256, "rom": report, "input_manifest": manifest,
        "profile": profile, "runtime": runtime, "compiler_environment": environment,
        "coverage": {**coverage, "description": f"Reviewed {'C and C++' if 'matched_cpp_functions' in coverage else 'C'} functions only; whole-game executable denominator is unknown"},
        "c_matches": matches, "c_attributions": attribution, "graph": graph, "dependencies": dependencies,
        "artifacts": {name: sha256(directory / name) for name in sorted(REQUIRED_ARTIFACTS)},
        "generated": generated_before, "compiled": file_inventory(directory / "compiled"),
        "objects": {name: sha256(directory / name) for name in graph}, "commands": commands,
        "reproduction": reproduction, "image_inventory": inventory_report,
        "limitations": ["Image inventory is partial; overlay B supports binary storage only with zero new C, overlay A remains opaque, and CPU/data/ucode section closure is unfinished", "Scoped C profiles and MIPS26/HI16/LO16 text relocations only; whole-game compiler provenance remains unproved", "Python standard-library/shared-library fingerprint closure pending", "Human review required for semantics and disguised raw-byte misuse"],
    }
    receipt_path = directory / "receipt.json"
    write_json(receipt_path, receipt)
    print(f"PASS: {directory.name}/shiren2.z64 is byte-identical ({EXPECTED_SHA256})")
    print(f"Receipt: {receipt_path}")
    return receipt_path


def build(name: str, promote: bool = False, tool_root: Path = PROJECT, reference: Path = CANONICAL_ROM,
          source_root: Path = PROJECT, build_root: Path | None = None) -> Path:
    """Snapshot `source_root` into `build_root/name` and build it.

    A non-canonical source or build root is a private trial: it can be verified
    with `verify.py <receipt> --snapshot-only` but never promoted.
    """
    if re.fullmatch(r"[a-zA-Z0-9][a-zA-Z0-9_-]*", name) is None:
        raise ValueError("Build name must contain only letters, digits, underscores and hyphens")
    canonical_root = (PROJECT / "build").resolve()
    build_root = (build_root or canonical_root).resolve()
    if promote and (source_root.resolve() != PROJECT.resolve() or build_root != canonical_root):
        raise ValueError("Only canonical project source built into build/ can be promoted")
    attest(reference.read_bytes())
    build_root.mkdir(parents=True, exist_ok=True)
    directory = build_root / name
    directory.mkdir(exist_ok=False)
    manifest = create_snapshot(source_root.resolve(), directory / "source")
    write_json(directory / "input-manifest.json", manifest)
    # Execute copied build tooling as well as copied game source.
    subprocess.run([python_command(), str(directory / "source/tools/build.py"), "--execute-snapshot", str(directory), "--tool-root", str(tool_root.resolve()), "--rom", str(reference.resolve())], check=True)
    receipt_path = directory / "receipt.json"
    from verify import promote_receipt, verify_receipt
    verify_receipt(receipt_path, require_current=False, tool_root=tool_root, reference=reference)
    if promote:
        promote_receipt(receipt_path, tool_root=tool_root, reference=reference)
    return receipt_path


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--name", default=datetime.now(timezone.utc).strftime("run-%Y%m%dT%H%M%S%fZ"))
    publication = parser.add_mutually_exclusive_group()
    publication.add_argument("--promote", action="store_true", help="Integrator: verify current source and publish accepted receipt")
    publication.add_argument("--no-promote", action="store_true", help="Private trial; default")
    parser.add_argument("--tool-root", type=Path, default=PROJECT)
    parser.add_argument("--rom", type=Path, default=CANONICAL_ROM)
    parser.add_argument("--source-root", type=Path, default=PROJECT,
                        help="Private trial: snapshot this project copy instead of canonical source")
    parser.add_argument("--build-root", type=Path, default=None,
                        help="Private trial: create the run directory here instead of build/")
    parser.add_argument("--execute-snapshot", type=Path, help=argparse.SUPPRESS)
    args = parser.parse_args()
    try:
        if args.execute_snapshot:
            execute_snapshot(args.execute_snapshot, args.tool_root, args.rom)
        else:
            build(args.name, args.promote, args.tool_root, args.rom, args.source_root, args.build_root)
    except (ValueError, OSError, KeyError, subprocess.CalledProcessError) as error:
        parser.exit(1, f"{error}\nSee the exclusive run's build.log when present.\n")


if __name__ == "__main__":
    main()
