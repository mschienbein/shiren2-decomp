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

from certification import REQUIRED_ARTIFACTS, build_graph, c_attributions, check_elf, compiler_environment, expected_commands, file_inventory, object_commands, python_command, read_json, rom_map_report, sha256, split_commands, splat_options, tool_profile, runtime_fingerprint, image_evidence, image_matches, reproduce_c_objects_and_link, linked_image_records, validate_asset_recipe
from certification import archive_orchestration, orchestration_sources, validate_orchestration
from evidence import COMPILED_KINDS, validate_attributions, validate_matches
from certification import generated_table_discards
from rom import CANONICAL_ROM, EXPECTED_SHA256, PROJECT, attest
from workspace import create_snapshot, verify_manifest
import toolchain_library


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
    toolchain_library.validate_dependencies(source_root, source or "", result)
    return sorted(result)


def execute_snapshot(directory: Path, tool_root: Path, reference: Path, *, preflight_only: bool = False) -> Path | None:
    """Build frozen game inputs using the independently selected trusted tooling."""
    directory = directory.resolve()
    entrypoint = Path(__file__).resolve()
    if entrypoint != (tool_root.resolve() / "tools/build.py"):
        raise ValueError("Build child must execute the operator-selected trusted tool_root/tools/build.py")
    recipe = split_commands()
    validate_asset_recipe(directory / "source", recipe)
    manifest = read_json(directory / "input-manifest.json")
    verify_manifest(directory / "source", manifest)
    if preflight_only:
        print(json.dumps({"status": "TRUSTED_CHILD_PREFLIGHT_ONLY_NOT_BUILT", "accepted": False, "build_source_bytes": 0,
                          "entrypoint": str(entrypoint), "entrypoint_sha256": sha256(entrypoint),
                          "input_manifest_sha256": manifest["sha256"], "recipe": recipe}))
        return None
    orchestration = archive_orchestration(directory, entrypoint)
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

        for operation in recipe:
            run(operation)
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
        generated_table_discards(directory, graph, profile, reference.read_bytes())
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
    inventory_report["linked_images"] = linked_image_records(directory, inventory, reference.read_bytes())
    reproduction = reproduce_c_objects_and_link(directory, graph, profile, reference)
    attribution = c_attributions(directory, matches, graph, profile, reference.read_bytes(), reproduction=reproduction)
    validate_attributions(matches, attribution, graph)
    check_elf(directory)
    if commands != expected_commands(graph, profile):
        raise ValueError("Executed commands disagree with the declared graph")
    complete_game = rom_map_report(directory, reference.read_bytes(), matches, commands=commands,
                                   reproduction=reproduction, graph=graph, profile=profile)
    if orchestration_sources(entrypoint.parent) != orchestration["source_files"]:
        raise ValueError("Trusted orchestration sources changed while building")
    validate_orchestration(directory, orchestration)
    receipt = {
        "schema_version": 3, "status": "byte-identical", "created_at": datetime.now(timezone.utc).isoformat(),
        "target_sha256": EXPECTED_SHA256, "rom": report, "input_manifest": manifest,
        "profile": profile, "runtime": {**runtime, "orchestration": orchestration}, "compiler_environment": environment,
        "coverage": {**coverage, "description": "Reviewed C/C++ functions, including explicitly listed generated C++ subsets (not double-counted); pinned upstream toolchain library counted separately, never in C/C++; complete-game accounting is the rom_map section"},
        "c_matches": matches, "c_attributions": attribution, "graph": graph, "dependencies": dependencies,
        "artifacts": {name: sha256(directory / name) for name in sorted(REQUIRED_ARTIFACTS)},
        "generated": generated_before, "compiled": file_inventory(directory / "compiled"),
        "objects": {name: sha256(directory / name) for name in graph}, "commands": commands,
        "reproduction": reproduction, "image_inventory": inventory_report, "rom_map": complete_game,
        "limitations": ["Image inventory is partial; both overlays link as same-VRAM splat code parents whose data/rodata cuts stay provisional until their owning C is matched, and CPU/data/ucode section closure is unfinished", "Scoped C profiles and MIPS26/HI16/LO16 text relocations only; whole-game compiler provenance remains unproved", "Python standard-library/shared-library fingerprint closure pending", "Human review required for semantics and disguised raw-byte misuse"],
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
    # Keep the verifier outside the tested snapshot; only the bound asset packer
    # executes snapshot Python. Game/header/config inputs remain frozen.
    subprocess.run([python_command(), "-B", str(tool_root.resolve() / "tools/build.py"), "--execute-snapshot", str(directory), "--tool-root", str(tool_root.resolve()), "--rom", str(reference.resolve())], check=True)
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
    parser.add_argument("--preflight-only", action="store_true",
                        help="With --execute-snapshot only: check trusted child entry and frozen inputs; never build or issue a receipt")
    args = parser.parse_args()
    if args.preflight_only and not args.execute_snapshot:
        parser.error("--preflight-only requires --execute-snapshot")
    try:
        if args.execute_snapshot:
            execute_snapshot(args.execute_snapshot, args.tool_root, args.rom, preflight_only=args.preflight_only)
        else:
            build(args.name, args.promote, args.tool_root, args.rom, args.source_root, args.build_root)
    except (ValueError, OSError, KeyError, subprocess.CalledProcessError) as error:
        parser.exit(1, f"{error}\nSee the exclusive run's build.log when present.\n")


if __name__ == "__main__":
    main()
