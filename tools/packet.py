#!/usr/bin/env python3
"""Prepare image-qualified function references from a verified frozen build."""
from __future__ import annotations

import argparse
import hashlib
import json
import re
import stat
import subprocess
import sys
from collections.abc import Iterator, Mapping
from dataclasses import asdict, dataclass, replace
from datetime import datetime, timezone
from importlib.metadata import PackageNotFoundError, version
from pathlib import Path, PurePosixPath
from types import MappingProxyType
from typing import Any

from certification import build_graph, file_inventory, image_evidence, json_digest, linked_image_records, read_json, runtime_fingerprint, sha256, tool_profile
from evidence import COMPILED_KINDS, COMPILED_SUFFIXES
from images import ImageInventory, ImageRecord, require_cpu_range
from rom import CANONICAL_ROM, EXPECTED_SHA256, PROJECT, attest
from verify import latest_receipt, verify_receipt
from workspace import verify_manifest

FunctionKey = tuple[str, str]
_IDENTIFIER = re.compile(r"[A-Za-z_][A-Za-z0-9_]*\Z")
# A jump-table target at a catalogued function start is emitted as `jlabel func_X`.
_LABEL = re.compile(r"^\s*(glabel|jlabel(?=\s+[A-Za-z_])|endlabel)\s+(\S+)\s*$", re.MULTILINE)
# Jump-table case targets inside a function (rodata subsegments make them global).
_CASE_LABEL = re.compile(r"\s*jlabel\s+\.L[0-9A-F]{8}\s*")
_ALIAS = re.compile(r"^\s*alabel\s+([A-Za-z_][A-Za-z0-9_]*)\s*$", re.MULTILINE)
_WORD = re.compile(r"^\s*/\*\s+([0-9A-Fa-f]+)\s+([0-9A-Fa-f]+)\s+([0-9A-Fa-f]{8})\s+\*/\s+(.+)$")


@dataclass(frozen=True)
class Instruction:
    rom_offset: int
    vram: int
    word: int
    assembly: str


@dataclass(frozen=True)
class DirectCall:
    instruction_vram: int
    target_vram: int
    operand: str
    possible_targets: tuple[FunctionKey, ...] = ()


@dataclass(frozen=True)
class FunctionReference:
    image_id: str
    symbol: str
    reference_source: str
    reference_source_sha256: str
    object_path: str
    unit: str
    rom_start: int
    vram_start: int
    instruction_bytes: bytes
    assembly: str
    instructions: tuple[Instruction, ...]
    already_accepted: bool
    accepted_source: str | None
    profile_id: str | None
    profile_candidates: tuple[str, ...]
    handwritten_annotation: bool
    direct_calls: tuple[DirectCall, ...]
    indirect_calls: tuple[int, ...]
    data_references: tuple[str, ...]
    internal_aliases: tuple[str, ...]
    callers: tuple[FunctionKey, ...] = ()

    @property
    def key(self) -> FunctionKey:
        return self.image_id, self.symbol

    @property
    def size(self) -> int:
        return len(self.instruction_bytes)

    @property
    def expected_sha256(self) -> str:
        return hashlib.sha256(self.instruction_bytes).hexdigest()


@dataclass(frozen=True)
class BaselineIdentity:
    receipt_path: Path
    receipt_sha256: str
    source_manifest_sha256: str
    reference_path: Path
    reference_sha256: str
    manifest_json: str
    profile_json: str
    runtime_json: str
    generated_json: str
    linker_script_sha256: str
    image_inventory_sha256: str

    @property
    def directory(self) -> Path:
        return self.receipt_path.parent


@dataclass(frozen=True)
class FunctionCatalogue(Mapping[FunctionKey, FunctionReference]):
    baseline: BaselineIdentity
    inventory: ImageInventory
    functions: Mapping[FunctionKey, FunctionReference]

    def __getitem__(self, key: FunctionKey) -> FunctionReference:
        return self.functions[key]

    def __iter__(self) -> Iterator[FunctionKey]:
        return iter(self.functions)

    def __len__(self) -> int:
        return len(self.functions)


def _json(value: object) -> str:
    return json.dumps(value, sort_keys=True, separators=(",", ":"))


def _regular_path(root: Path, relative: str) -> Path:
    parsed = PurePosixPath(relative)
    if not relative or parsed.is_absolute() or ".." in parsed.parts or "\\" in relative or parsed.as_posix() != relative:
        raise ValueError(f"Reference path escapes its frozen root: {relative!r}")
    path = root
    for index, component in enumerate(parsed.parts):
        path /= component
        mode = path.lstat().st_mode
        check = stat.S_ISREG if index == len(parsed.parts) - 1 else stat.S_ISDIR
        if not check(mode):
            raise ValueError(f"Reference path contains a symlink or nonregular input: {relative}")
    return path


def _check_baseline(baseline: BaselineIdentity) -> None:
    """Identity checks without recompiling the attested build per task."""
    if sha256(baseline.receipt_path) != baseline.receipt_sha256:
        raise ValueError("Frozen baseline receipt changed")
    manifest = json.loads(baseline.manifest_json)
    if read_json(baseline.directory / "input-manifest.json") != manifest:
        raise ValueError("Frozen baseline manifest changed")
    verify_manifest(baseline.directory / "source", manifest)
    if file_inventory(baseline.directory / "generated") != json.loads(baseline.generated_json):
        raise ValueError("Frozen generated reference inventory changed")
    if sha256(_regular_path(baseline.directory, "shiren2.ld")) != baseline.linker_script_sha256:
        raise ValueError("Frozen image/source graph changed")
    if sha256(baseline.reference_path) != baseline.reference_sha256:
        raise ValueError("Original reference ROM changed")
    if _json(tool_profile(PROJECT, baseline.directory / "source")) != baseline.profile_json:
        raise ValueError("Frozen tool/profile identity changed")
    if _json(runtime_fingerprint()) != baseline.runtime_json:
        raise ValueError("Frozen Python/package runtime changed")


def _parse_functions(text: str) -> list[tuple[str, str, tuple[Instruction, ...], bool]]:
    """Parse paired global labels; emitted body lines need original words."""
    result: list[tuple[str, str, tuple[Instruction, ...], bool]] = []
    start: re.Match[str] | None = None
    previous_end = 0
    seen: set[str] = set()
    for label in _LABEL.finditer(text):
        kind, symbol = label.groups()
        if _IDENTIFIER.fullmatch(symbol) is None:
            raise ValueError(f"Unsupported or ambiguous assembly label: {symbol}")
        if kind in {"glabel", "jlabel"}:
            if start is not None or symbol in seen:
                raise ValueError(f"Duplicate or nested function label: {symbol}")
            start = label
            seen.add(symbol)
            continue
        if start is None or symbol != start.group(2):
            raise ValueError(f"Mismatched function end label: {symbol}")
        body = text[start.end():label.start()]
        words: list[Instruction] = []
        for line in body.splitlines():
            instruction = _WORD.fullmatch(line)
            alias = _ALIAS.fullmatch(line)
            if instruction:
                offset, address, word, assembly = instruction.groups()
                if assembly.lstrip().startswith("."):
                    raise ValueError(f"Noninstruction directive inside function: {symbol}")
                words.append(Instruction(int(offset, 16), int(address, 16), int(word, 16), assembly))
            elif alias:
                name = alias.group(1)
                if name in seen:
                    raise ValueError(f"Duplicate or ambiguous internal assembly alias: {name}")
                seen.add(name)
            elif line.strip().startswith("alabel"):
                raise ValueError(f"Malformed internal assembly alias: {line.strip()}")
            elif line.strip() and not line.lstrip().startswith(("/*", "#")) and not re.fullmatch(r"\s*[.$A-Za-z_][.$A-Za-z_0-9]*:\s*", line) and not _CASE_LABEL.fullmatch(line):
                raise ValueError(f"Unannotated or malformed instruction inside function: {symbol}")
        if not words:
            raise ValueError(f"Function label has no original instruction words: {symbol}")
        prefix = text[previous_end:start.start()]
        result.append((symbol, text[start.start():label.end()], tuple(words), "Handwritten function" in prefix))
        previous_end = label.end()
        start = None
    if start is not None:
        raise ValueError(f"Function label lacks an end label: {start.group(2)}")
    return result


def _function_reference(
    image_id: str, symbol: str, assembly: str, words: tuple[Instruction, ...], handwritten: bool,
    source: str, source_digest: str, object_path: str, unit: str,
    accepted: dict[str, Any] | None, profile: dict[str, Any], inventory: ImageInventory, reference: bytes,
) -> FunctionReference:
    first = words[0]
    body = b"".join(word.word.to_bytes(4, "big") for word in words)
    image = require_cpu_range(image_id, first.rom_offset, len(body), inventory)
    for index, word in enumerate(words):
        if word.rom_offset != first.rom_offset + index * 4 or word.vram != first.vram + index * 4:
            raise ValueError(f"Noncontiguous ROM/VRAM instruction reference: {image_id}:{symbol}")
        if word.vram != word.rom_offset + image.vram_rom_delta:
            raise ValueError(f"Instruction mapping disagrees with original image: {image_id}:{symbol}")
    if body != reference[first.rom_offset:first.rom_offset + len(body)]:
        raise ValueError(f"Instruction words disagree with original ROM: {image_id}:{symbol}")
    if accepted is not None and (accepted["rom_start"], accepted["vram_start"], accepted["size"]) != (first.rom_offset, first.vram, len(body)):
        raise ValueError(f"Accepted C reference disagrees with frozen match interval: {image_id}:{symbol}")
    calls: list[DirectCall] = []
    indirect: list[int] = []
    for word in words:
        if word.word >> 26 == 3:
            address = ((word.vram + 4) & 0xF0000000) | ((word.word & 0x03FFFFFF) << 2)
            calls.append(DirectCall(word.vram, address, word.assembly.split(None, 1)[-1]))
        elif word.word >> 26 == 0 and word.word & 0x3F == 9:
            indirect.append(word.vram)
    data = tuple(sorted(set(re.findall(r"%(?:hi|lo)\(([A-Za-z_][A-Za-z_0-9]*)\)", assembly))))
    accepted_source = None if accepted is None else accepted["source"]
    return FunctionReference(
        image_id, symbol, source, source_digest, object_path, unit,
        first.rom_offset, first.vram, body, assembly, words, accepted is not None,
        accepted_source, None if accepted_source is None else profile["tu_profiles"][accepted_source],
        tuple(sorted(profile["profiles"])), handwritten, tuple(calls), tuple(indirect), data, tuple(_ALIAS.findall(assembly)),
    )


def enumerate_functions(receipt_path: Path | None = None, reference: Path = CANONICAL_ROM) -> FunctionCatalogue:
    """Attest one frozen baseline, then enumerate image-qualified CPU references.

    Handwritten annotations, compiler provenance and Splat boundaries remain
    diagnostic/provisional. Queue filtering and independent review are separate.
    """
    path = receipt_path if receipt_path is not None else latest_receipt()
    if path.is_symlink() or not path.is_file():
        raise ValueError("A regular frozen receipt is required")
    path = path.resolve()
    digest = sha256(path)
    receipt = verify_receipt(path, require_current=False, reference=reference)
    original = reference.read_bytes()
    attest(original)
    if receipt["target_sha256"] != EXPECTED_SHA256:
        raise ValueError("Frozen baseline has the wrong target")
    directory = path.parent
    graph = build_graph(directory)
    inventory, inventory_report = image_evidence(directory / "source", original)
    inventory_report["linked_images"] = linked_image_records(directory, inventory, original)
    if graph != receipt["graph"] or inventory_report != receipt["image_inventory"]:
        raise ValueError("Rederived graph/image identity disagrees with frozen receipt")
    baseline = BaselineIdentity(
        path, digest, receipt["input_manifest"]["sha256"], reference.resolve(),
        hashlib.sha256(original).hexdigest(), _json(receipt["input_manifest"]),
        _json(receipt["profile"]), _json(receipt["runtime"]), _json(receipt["generated"]),
        receipt["artifacts"]["shiren2.ld"], inventory_report["config_sha256"],
    )
    _check_baseline(baseline)
    matched = {(match.get("image_id", "resident"), match["symbol"]): match for match in receipt["c_matches"]}
    sources: list[tuple[str, str, str, str, dict[str, Any] | None]] = []
    for object_path, item in graph.items():
        if item["source_kind"] == "asm":
            if item["image_id"] not in inventory.images:
                raise ValueError("Frozen assembly lacks a declared original image")
            source = item["input"]
            if not source.startswith("generated/asm/") or not source.endswith(".s"):
                raise ValueError("Assembly graph input is outside frozen generated assembly")
            text = _regular_path(directory, source).read_text(encoding="utf-8")
            if re.search(r"^\s*(?:\.text\s*$|\.section\s+\.text\b)", text, re.MULTILINE):
                unit = PurePosixPath(source).relative_to("generated/asm").with_suffix("").as_posix()
                sources.append((source, item["image_id"], object_path, unit, None))
    for key, match in matched.items():
        owners = [(name, item) for name, item in graph.items() if item["source_kind"] in COMPILED_KINDS and (item["image_id"], item["source"]) == (key[0], match["source"])]
        if len(owners) != 1:
            raise ValueError("Accepted function lacks a unique frozen C/C++ graph source")
        unit = PurePosixPath(match["source"]).relative_to("src").with_suffix("").as_posix()
        source = f"generated/asm/matchings/{unit}/{match['symbol']}.s"
        sources.append((source, key[0], owners[0][0], unit, match))
    functions: dict[FunctionKey, FunctionReference] = {}
    for source, image_id, object_path, unit, accepted in sorted(sources, key=lambda item: item[0]):
        name = PurePosixPath(source).relative_to("generated").as_posix()
        if name not in receipt["generated"]:
            raise ValueError(f"Frozen assembly is absent from required generated inventory: {source}")
        source_path = _regular_path(directory, source)
        if sha256(source_path) != receipt["generated"][name]:
            raise ValueError(f"Stale frozen assembly reference: {source}")
        parsed = _parse_functions(source_path.read_text(encoding="utf-8"))
        if accepted is not None and [entry[0] for entry in parsed] != [accepted["symbol"]]:
            raise ValueError("Accepted matching reference contains ambiguous function labels")
        for symbol, assembly, words, handwritten in parsed:
            function = _function_reference(image_id, symbol, assembly, words, handwritten, source, receipt["generated"][name], object_path, unit, accepted, receipt["profile"], inventory, original)
            if function.key in functions:
                raise ValueError(f"Duplicate image/function identity: {function.key}")
            functions[function.key] = function
    for image_id in inventory.images:
        end = -1
        labels: set[str] = set()
        for function in sorted((item for item in functions.values() if item.image_id == image_id), key=lambda item: item.rom_start):
            current = {function.symbol, *function.internal_aliases}
            if current & labels:
                raise ValueError(f"Duplicate or ambiguous image-qualified alias: {image_id}/{sorted(current & labels)}")
            labels.update(current)
            if function.rom_start < end:
                raise ValueError(f"Overlapping function intervals in original image: {image_id}")
            end = function.rom_start + function.size
    addresses: dict[int, list[FunctionKey]] = {}
    for key, function in functions.items():
        addresses.setdefault(function.vram_start, []).append(key)
    callers: dict[FunctionKey, set[FunctionKey]] = {key: set() for key in functions}
    for key, function in list(functions.items()):
        resolved: list[DirectCall] = []
        for call in function.direct_calls:
            targets = tuple(sorted(addresses.get(call.target_vram, [])))
            resolved.append(replace(call, possible_targets=targets))
            for target in targets:
                callers[target].add(key)
        functions[key] = replace(function, direct_calls=tuple(resolved))
    for key in functions:
        functions[key] = replace(functions[key], callers=tuple(sorted(callers[key])))
    _check_baseline(baseline)
    return FunctionCatalogue(baseline, inventory, MappingProxyType(dict(sorted(functions.items()))))


def _packet_destination(destination: Path, *, exclusive: bool) -> Path:
    if ".." in destination.parts:
        raise ValueError("Packet output must not contain parent traversal")
    project = PROJECT.resolve()
    original_project = PROJECT.absolute()
    destination = destination.absolute()
    if destination.is_relative_to(original_project):
        destination = project / destination.relative_to(original_project)
    if not destination.is_relative_to(project / "scratch") or destination == project / "scratch":
        raise ValueError("Packet output must be a private directory below PROJECT/scratch")
    relative = destination.relative_to(project)
    path = project
    for index, part in enumerate(relative.parts):
        path /= part
        try:
            mode = path.lstat().st_mode
        except FileNotFoundError:
            continue
        if not stat.S_ISDIR(mode):
            raise ValueError(f"Packet output uses a symlink or non-directory: {path}")
        if exclusive and index == len(relative.parts) - 1:
            raise FileExistsError(f"Packet output already exists: {path}")
    return destination


def _context_inputs(baseline: BaselineIdentity) -> dict[str, dict[str, Any]]:
    manifest = json.loads(baseline.manifest_json)
    selected = {"config/images.json", "config/compiler_profiles.json", "config/symbol_addrs.txt", "config/reloc_addrs.txt"}
    return {
        name: entry for name, entry in manifest["files"].items()
        if name.startswith("include/") or name in selected
    }


def _write_exclusive(path: Path, content: bytes) -> None:
    _packet_destination(path.parent, exclusive=False)
    path.parent.mkdir(parents=True, exist_ok=True)
    _packet_destination(path.parent, exclusive=False)
    with path.open("xb") as stream:
        stream.write(content)


def _editable_files(values: tuple[str, ...]) -> list[str]:
    result: list[str] = []
    for value in values:
        path = PurePosixPath(value)
        if (
            not value or "\\" in value or path.is_absolute() or ".." in path.parts
            or path.as_posix() != value or len(path.parts) < 2 or path.parts[0] != "src"
            or path.suffix not in COMPILED_SUFFIXES.values() or value in result
        ):
            raise ValueError("Worker source ownership must contain distinct canonical src/*.c or src/*.cpp paths")
        result.append(value)
    return result


def _m2c_metadata() -> dict[str, Any]:
    executable = Path(sys.prefix) / "bin/m2c"
    try:
        package_version: str | None = version("m2c")
    except PackageNotFoundError:
        package_version = None
    return {
        "executable": str(executable),
        "executable_sha256": sha256(executable) if executable.is_file() and not executable.is_symlink() else None,
        "package_version": package_version,
    }


def _packet_metadata(catalogue: FunctionCatalogue, function: FunctionReference, destination: Path, editable: list[str]) -> dict[str, Any]:
    baseline = catalogue.baseline
    image: ImageRecord = catalogue.inventory.images[function.image_id]
    runtime = json.loads(baseline.runtime_json)
    profile = json.loads(baseline.profile_json)
    return {
        "schema_version": 1,
        "kind": "shiren2-frozen-function-packet",
        "status": "preparing",
        "created_at": datetime.now(timezone.utc).isoformat(),
        "function_key": {"image_id": function.image_id, "symbol": function.symbol},
        "target_sha256": EXPECTED_SHA256,
        "baseline": {
            "receipt": str(baseline.receipt_path), "receipt_sha256": baseline.receipt_sha256,
            "source_manifest_sha256": baseline.source_manifest_sha256,
            "image_inventory_sha256": baseline.image_inventory_sha256,
            "image_inventory_digest_scope": "frozen config/images.json; linked image evidence is bound by the receipt SHA-256",
            "profile_sha256": json_digest(profile), "profile": profile,
            "runtime_sha256": json_digest(runtime), "runtime_evidence": "frozen receipt.runtime",
            "runtime_scope": runtime.get("scope", "scope from frozen receipt"),
            "require_current_tree": False,
        },
        "image": asdict(image),
        "instruction_extent": {
            "rom_start": function.rom_start, "rom_end": function.rom_start + function.size,
            "vram_start": function.vram_start, "vram_end": function.vram_start + function.size,
            "size": function.size, "instruction_count": len(function.instructions),
            "sha256": function.expected_sha256, "private_bytes": "expected.bin",
        },
        "boundary": {
            "start": "frozen Splat glabel", "end": "frozen Splat endlabel; following padding excluded",
            "confidence": "provisional", "compiler_produced": "unverified",
            "internal_aliases": list(function.internal_aliases),
            "cpu_range_policy": "original image mapping and confirmed non-CPU exclusions enforced; not a text/classification proof",
            "handwritten_annotation": function.handwritten_annotation,
        },
        "unit": {
            "name": function.unit, "split_source": function.reference_source,
            "split_source_sha256": function.reference_source_sha256, "object": function.object_path,
            "accepted_source": function.accepted_source, "already_accepted": function.already_accepted,
            "source_context": "frozen baseline/source; worker TU regrouping requires coordinator review",
        },
        "toolchain": {
            "assigned_profile_id": function.profile_id, "candidate_profile_ids": list(function.profile_candidates),
            "uncertainty": "Only documented scoped calibration cases are proven; candidate profiles are not a universal compiler/ABI identification",
        },
        "references": {
            "direct_calls": [asdict(call) for call in function.direct_calls],
            "indirect_call_instruction_vrams": list(function.indirect_calls),
            "data_symbols": list(function.data_references),
            "possible_callers": [list(key) for key in function.callers],
            "confidence": "direct call addresses decoded from original words; prototypes, indirect targets and overlay lifetimes unresolved",
        },
        "ownership": {
            "packet_root": str(destination), "allowed_roots": [str(destination)],
            "editable_packet_files": ["candidate.c", "attempt.json", "candidate-notes.md"],
            "worker_source_files": editable,
            "source_files_scope": "coordinator must assign these in a separate private workspace before dispatch; no canonical write permission",
            "shared_authority": "coordinator owns headers, config, profiles, image boundaries, matches, integration and latest",
        },
        "candidate": {"status": "unverified", "c_credit_bytes": 0},
        "acceptance": {
            "required": [
                "review types, ABI, effects and function boundaries against original evidence",
                "exact original function bytes with declared source/object/profile and relocation proof",
                "independent reviewer approval; no inline assembly or embedded-byte C credit",
                "coordinator's fresh combined full-ROM build and complete current receipt verification",
            ],
            "similarity_or_m2c_exit_is_acceptance": False,
        },
    }


def create_packet(
    catalogue: FunctionCatalogue, image_id: str, symbol: str, destination: Path,
    *, run_m2c: bool = True, editable_files: tuple[str, ...] = (),
) -> Path:
    """Write one exclusive private packet, never a lease or an accepted match."""
    key = image_id, symbol
    if key not in catalogue:
        raise ValueError(f"Unknown image-qualified function: {key}")
    function = catalogue[key]
    editable = _editable_files(editable_files)
    destination = _packet_destination(destination, exclusive=True)
    _check_baseline(catalogue.baseline)
    destination.mkdir(parents=True, exist_ok=False)
    metadata = _packet_metadata(catalogue, function, destination, editable)
    try:
        _packet_destination(destination, exclusive=False)
        expected_outputs: dict[str, str] = {}
        _write_exclusive(destination / "expected.bin", function.instruction_bytes)
        expected_outputs["expected.bin"] = function.expected_sha256
        target_bytes = (".text\n" + function.assembly + "\n").encode("utf-8")
        _write_exclusive(destination / "target.s", target_bytes)
        expected_outputs["target.s"] = hashlib.sha256(target_bytes).hexdigest()
        context: dict[str, dict[str, Any]] = {}
        for relative, expected in _context_inputs(catalogue.baseline).items():
            source = _regular_path(catalogue.baseline.directory / "source", relative)
            data = source.read_bytes()
            if len(data) != expected["size"] or hashlib.sha256(data).hexdigest() != expected["sha256"]:
                raise ValueError(f"Frozen type context changed: {relative}")
            _packet_destination(destination, exclusive=False)
            _write_exclusive(destination / "context" / relative, data)
            context[relative] = {**expected, "packet_path": f"context/{relative}"}
            expected_outputs[f"context/{relative}"] = expected["sha256"]
        metadata["context"] = {"files": context, "sha256": json_digest(context), "interpretation": "frozen available headers/type and symbol records; relevant type semantics still require review"}
        m2c = _m2c_metadata()
        command = [m2c["executable"], "--target", "mips-gcc-c", "--stop-on-error", "-f", symbol, str(destination / "target.s")]
        if run_m2c:
            if m2c["executable_sha256"] is None:
                raise ValueError("m2c executable is unavailable in the locked Python environment")
            _check_baseline(catalogue.baseline)
            _packet_destination(destination, exclusive=False)
            result = subprocess.run(command, cwd=destination, text=True, capture_output=True, check=False)
            _packet_destination(destination, exclusive=False)
            stdout, stderr, returncode = result.stdout, result.stderr, result.returncode
            if sha256(Path(m2c["executable"])) != m2c["executable_sha256"] or _m2c_metadata() != m2c:
                raise ValueError("m2c executable/package identity changed during use")
        else:
            stdout, stderr, returncode = "", "m2c was explicitly skipped; no candidate generated.\n", None
        _packet_destination(destination, exclusive=False)
        _write_exclusive(destination / "candidate.c", stdout.encode("utf-8"))
        _write_exclusive(destination / "m2c.log", stderr.encode("utf-8"))
        expected_outputs["candidate.c"] = hashlib.sha256(stdout.encode("utf-8")).hexdigest()
        expected_outputs["m2c.log"] = hashlib.sha256(stderr.encode("utf-8")).hexdigest()
        metadata["m2c"] = {**m2c, "command": command, "invoked": run_m2c, "returncode": returncode, "status": "unverified-output" if run_m2c else "not-run"}
        metadata["candidate"]["contains_unknown_type_marker"] = bool(re.search(r"\?(?:\s|$)", stdout))
        _check_baseline(catalogue.baseline)
        _packet_destination(destination, exclusive=False)
        outputs = file_inventory(destination)
        if outputs != expected_outputs:
            raise ValueError("Packet required output inventory changed during creation")
        if outputs["expected.bin"] != function.expected_sha256:
            raise ValueError("Packet original bytes changed during creation")
        for entry in context.values():
            if outputs[entry["packet_path"]] != entry["sha256"]:
                raise ValueError("Packet frozen context changed during creation")
        metadata["outputs"] = outputs
        metadata["status"] = "prepared-unverified"
        task_bytes = (json.dumps(metadata, indent=2) + "\n").encode("utf-8")
        _write_exclusive(destination / "task.json", task_bytes)
        _check_baseline(catalogue.baseline)
        _packet_destination(destination, exclusive=False)
        if file_inventory(destination) != {**outputs, "task.json": hashlib.sha256(task_bytes).hexdigest()}:
            raise ValueError("Packet output inventory changed after metadata write")
    except (ValueError, OSError, subprocess.SubprocessError) as error:
        try:
            _packet_destination(destination, exclusive=False)
            failure = {"status": "invalidated", "reason": str(error), "function_key": list(key), "baseline_receipt_sha256": catalogue.baseline.receipt_sha256, "c_credit_bytes": 0}
            (destination / "failure.json").write_text(json.dumps(failure, indent=2) + "\n", encoding="utf-8")
            task = destination / "task.json"
            if task.exists() and not task.is_symlink():
                metadata["status"] = "invalidated"
                metadata["failure"] = str(error)
                task.write_text(json.dumps(metadata, indent=2) + "\n", encoding="utf-8")
        except (ValueError, OSError):
            pass  # Never follow an unsafe output parent merely to record failure.
        raise
    return destination


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("symbol", nargs="?")
    parser.add_argument("--image", help="Explicit original image identity")
    parser.add_argument("--receipt", type=Path, help="Frozen verified baseline; default accepted latest")
    parser.add_argument("--output", type=Path, help="Exclusive directory below project scratch")
    parser.add_argument("--no-m2c", action="store_true")
    args = parser.parse_args()
    try:
        catalogue = enumerate_functions(args.receipt)
        if args.image is not None and args.image not in catalogue.inventory.images:
            raise ValueError(f"Unknown image: {args.image}")
        if args.symbol is None:
            if args.output is not None:
                raise ValueError("--output requires a function symbol")
            for function in catalogue.values():
                if args.image is None or function.image_id == args.image:
                    accepted = " accepted-C" if function.already_accepted else ""
                    print(f"{function.image_id}:{function.symbol:24} ROM 0x{function.rom_start:06X} VRAM 0x{function.vram_start:08X} {function.size:6} bytes{accepted}")
            return
        choices = [key for key in catalogue if key[1] == args.symbol and (args.image is None or key[0] == args.image)]
        if len(choices) != 1:
            raise ValueError(f"Function requires a unique image-qualified identity: {args.symbol}; choices={choices}")
        image_id, symbol = choices[0]
        destination = args.output or PROJECT / "scratch/packets" / f"{image_id}-{symbol}-{datetime.now(timezone.utc).strftime('%Y%m%dT%H%M%S%fZ')}"
        print(create_packet(catalogue, image_id, symbol, destination, run_m2c=not args.no_m2c))
    except (ValueError, OSError, KeyError, subprocess.SubprocessError) as error:
        parser.exit(1, f"{error}\n")


if __name__ == "__main__":
    main()
