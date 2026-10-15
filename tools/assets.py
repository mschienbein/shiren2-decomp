"""Stored-tail assets: decoded Yay0 sidecars re-encoded into the exact original ROM tail.

One-time `assets.py extract --root <source tree>` creates private editable source files at
`assets/tail/yay0/<rom start>.bin`. Workspace snapshots include those files and their hashes.
The build runs `assets.py pack splat.override.yaml` after splat: it reads only the frozen
decoded sources for Yay0 content, re-encodes with pinned crunch64, and copies non-Yay0 ranges
from the original. It never regenerates decoded inputs or rescues an inexact stream.

The packed tail must equal the original and replaces `generated/assets/remaining.bin`.
`source-manifest.json` freezes paths, hashes, import evidence, map identities and the actual
encoder implementation. `import-manifest` explicitly migrates hash-pinned r1 files without
decoding or recompression. Neither pack nor validation creates missing inputs.

Every non-Yay0 range is explicit reference passthrough with zero editable-source credit.
Manifest validation is not acceptance: only the receipt-bound approved recipe and independent
replay can establish roundtrip. A one-time ROM decode never implies independent authorship.

`decode_yay0` is the bounded reference decoder (big-endian header; links at +8 and
literals/counts at +12; 12-bit distance plus one; lengths plus two, extended plus 18).
"""
from __future__ import annotations

import argparse
import hashlib
import importlib.metadata
import json
import struct
import sys
from dataclasses import dataclass
from pathlib import Path
from typing import Any

import yaml

import rom
import rommap


MAX_INPUT = 1 << 20
MAX_OUTPUT = 4 << 20
MAX_TOKENS = 1 << 20
TAIL_SEGMENT = "remaining"
TAIL_OUTPUT = "generated/assets/remaining.bin"
SIDECAR_ROOT = "generated/assets/tail"
MANIFEST_KIND = "shiren2-tail-asset-pack"
ENCODER = {"package": "crunch64", "version": "0.6.2", "codec": "yay0"}
SOURCE_MANIFEST = "assets/tail/source-manifest.json"
SOURCE_KIND = "shiren2-frozen-tail-sources"
PROVENANCE_ROOT = "assets/tail/provenance"
# Historical import authority only. This receipt is never r2 acceptance.
R1_RECEIPT_SHA256 = "0b6ea54955fe6503e72e1468991684a17c43058b0936303e43f3b6ce59f1a6a7"
R1_PACK_SHA256 = "f394de1f1057f73536ef97015d5a195ec9dcded393f54dadfb85a8399ef1fbcc"


class AssetError(ValueError):
    """Input, policy, identity or output validation failed."""


@dataclass(frozen=True)
class DecodeEvidence:
    decoded_size: int
    link_offset: int
    literal_offset: int
    mask_end: int
    minimum_flag_byte_end: int
    link_end: int
    literal_end: int
    consumed_end: int
    tokens: int
    literals: int
    links: int
    extended_lengths: int
    overlapping_links: int
    maximum_distance: int
    unused_mask_bits: int
    unused_mask_value: int


def sha256(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def decode_yay0(data: bytes, *, max_input: int = MAX_INPUT,
                max_output: int = MAX_OUTPUT, max_tokens: int = MAX_TOKENS
                ) -> tuple[bytes, DecodeEvidence]:
    for name, limit in (("input", max_input), ("output", max_output), ("token", max_tokens)):
        if type(limit) is not int or limit <= 0:
            raise AssetError(f"invalid {name} limit")
    if len(data) > max_input:
        raise AssetError("input limit exceeded")
    if len(data) < 16:
        raise AssetError("truncated Yay0 header")
    magic, size, link_start, literal_start = struct.unpack_from(">4sIII", data)
    if magic != b"Yay0":
        raise AssetError("invalid Yay0 magic")
    if not 0 < size <= max_output:
        raise AssetError("decoded size exceeds nonempty output limit")
    if not 16 <= link_start <= literal_start <= len(data):
        raise AssetError("unordered or out-of-window stream offsets")
    if (link_start - 16) % 4 or (literal_start - link_start) % 2:
        raise AssetError("unaligned mask-word or link-halfword region")
    mask_cursor, link_cursor, literal_cursor = 16, link_start, literal_start
    flags = bits_left = tokens = literals = links = extended = overlap = farthest = 0
    output = bytearray()
    while len(output) < size:
        if tokens >= max_tokens:
            raise AssetError("token work limit exceeded")
        if bits_left == 0:
            if mask_cursor + 4 > link_start:
                raise AssetError("mask stream read crosses link region")
            flags = struct.unpack_from(">I", data, mask_cursor)[0]
            mask_cursor += 4
            bits_left = 32
        literal = bool(flags & (1 << (bits_left - 1)))
        bits_left -= 1
        if literal:
            if literal_cursor >= len(data):
                raise AssetError("truncated literal stream")
            output.append(data[literal_cursor])
            literal_cursor += 1
            literals += 1
        else:
            if link_cursor + 2 > literal_start:
                raise AssetError("link stream read crosses literal region")
            link = struct.unpack_from(">H", data, link_cursor)[0]
            link_cursor += 2
            distance = (link & 0xfff) + 1
            if distance > len(output):
                raise AssetError("backreference precedes output")
            length = link >> 12
            if length == 0:
                if literal_cursor >= len(data):
                    raise AssetError("truncated extended length")
                length = data[literal_cursor] + 18
                literal_cursor += 1
                extended += 1
            else:
                length += 2
            if length > size - len(output):
                raise AssetError("backreference run exceeds declared output")
            overlap += int(length > distance)
            farthest = max(farthest, distance)
            # Source advances through newly produced bytes for overlapping runs.
            for _ in range(length):
                output.append(output[-distance])
            links += 1
        tokens += 1
    evidence = DecodeEvidence(
        size, link_start, literal_start, mask_cursor, 16 + (tokens + 7) // 8,
        link_cursor, literal_cursor, max(mask_cursor, link_cursor, literal_cursor),
        tokens, literals, links, extended, overlap, farthest, bits_left,
        flags & ((1 << bits_left) - 1),
    )
    return bytes(output), evidence


def _tail_segment(source_root: Path) -> tuple[int, int] | None:
    segments = yaml.safe_load((source_root / "config/shiren2.jp.yaml").read_text())["segments"]
    for index, segment in enumerate(segments[:-1]):
        if isinstance(segment, dict) and segment.get("name") == TAIL_SEGMENT and segment.get("type") == "bin":
            following = segments[index + 1]
            return segment["start"], following[0] if isinstance(following, list) else following["start"]
    return None


def tail_interval(source_root: Path) -> tuple[int, int]:
    """ROM interval of the splat `remaining` storage segment in frozen YAML."""
    interval = _tail_segment(source_root)
    if interval is None:
        raise AssetError("Frozen YAML has no stored-tail storage segment")
    return interval


def sidecar_key(start: int) -> str:
    """Generated-inventory key of one decoded stream sidecar."""
    return f"assets/tail/yay0/{start:08x}.bin"


def tail_streams(source_root: Path) -> list[tuple[int, int]]:
    """Yay0 ranges of the frozen map inside the stored tail (unvalidated listing)."""
    start, end = tail_interval(source_root)
    document = json.loads((source_root / rommap.MAP_PATH).read_text())
    return [(row[0], row[1]) for row in document["ranges"] if row[2] == "asset_yay0" and start <= row[0] < end]


def generated_keys(source_root: Path) -> set[str]:
    """Generated files `pack` adds beside splat's split output; none without a stored tail."""
    if _tail_segment(source_root) is None:
        return set()
    return {"assets/tail/manifest.json", *(sidecar_key(start) for start, _ in tail_streams(source_root))}


def _crunch64():
    import crunch64
    if crunch64.__version__ != ENCODER["version"]:
        raise AssetError(f"Yay0 packing requires crunch64 {ENCODER['version']}")
    return crunch64


def _decode(stream: bytes) -> bytes:
    """Pinned crunch64 decoder; Rust panics (BaseException) become asset errors."""
    try:
        decoded = _crunch64().yay0.decompress(stream)
    except (KeyboardInterrupt, SystemExit):
        raise
    except BaseException as error:  # noqa: BLE001 - pyo3 panics derive from BaseException
        raise AssetError(f"Yay0 stream does not decode: {error}") from None
    if len(decoded) != struct.unpack_from(">I", stream, 4)[0]:
        raise AssetError("Yay0 stream decodes to a different declared size")
    return decoded


def _json_bytes(value: Any) -> bytes:
    return (json.dumps(value, sort_keys=True, separators=(",", ":")) + "\n").encode()


def _identity(rom_map: rommap.RomMap) -> dict[str, str]:
    return {key: getattr(rom_map, key) for key in
            ("map_id", "partition_sha256", "evidence_policy_sha256")}


def encoder_fingerprint() -> dict[str, Any]:
    """Measure installed implementation bytes, using the receipt package hash convention."""
    encoder = _crunch64()
    distribution = importlib.metadata.distribution("crunch64")
    if distribution.files is None or distribution.version != ENCODER["version"]:
        raise AssetError("Missing encoder implementation inventory")
    files = {}
    for relative in distribution.files:
        if "__pycache__" in relative.parts or relative.suffix in {".pyc", ".pyo"}:
            continue
        path = Path(distribution.locate_file(relative))
        if not path.is_file():
            raise AssetError(f"Missing encoder implementation: {relative}")
        files[str(relative)] = sha256(path.read_bytes())
    loaded = Path(encoder.__file__).resolve()
    if loaded != Path(distribution.locate_file("crunch64/__init__.py")).resolve():
        raise AssetError("Loaded encoder is outside the measured distribution")
    digest = sha256(json.dumps(files, sort_keys=True, separators=(",", ":")).encode())
    return {**ENCODER, "files_sha256": digest, "file_count": len(files),
            "python_binary_sha256": sha256(Path(sys.executable).read_bytes()),
            "python_version": sys.version}


def source_encoder_identity() -> dict[str, Any]:
    """Source identity measures encoder distribution bytes, not the host Python."""
    return {key: value for key, value in encoder_fingerprint().items()
            if key not in {"python_binary_sha256", "python_version"}}


def _file(root: Path, relative: str) -> Path:
    """Require a lexical in-root path with no symlink at any component."""
    if not isinstance(relative, str):
        raise AssetError("Asset path must be a relative string")
    path = Path(relative)
    if path.is_absolute() or not path.parts or any(p in {".", ".."} for p in relative.split("/")):
        raise AssetError(f"Unsafe asset path: {relative}")
    current = root
    if root.is_symlink():
        raise AssetError(f"Symlink asset root: {root}")
    for part in path.parts:
        current = current / part
        if current.is_symlink():
            raise AssetError(f"Symlink asset path: {relative}")
    if not current.is_file():
        raise AssetError(f"Missing asset file: {relative}")
    return current


def _inventory(root: Path, relative: str) -> set[str]:
    directory = root / relative
    if directory.is_symlink() or (root / "assets").is_symlink():
        raise AssetError(f"Symlink asset inventory: {directory}")
    found = set()
    for path in directory.rglob("*"):
        if path.is_symlink():
            raise AssetError(f"Symlink asset inventory: {path}")
        if path.is_file():
            found.add(path.relative_to(root).as_posix())
    return found


def _ranges(source: Path, rom_map: rommap.RomMap) -> list[tuple]:
    start, end = tail_interval(source)
    ranges = [row for row in rom_map.ranges if start <= row[0] < end]
    cursor = start
    for first, last, *_ in ranges:
        if first != cursor or not first < last <= end:
            raise AssetError("Frozen map ranges do not tile the stored-tail segment")
        cursor = last
    if cursor != end:
        raise AssetError("Frozen map ranges do not align with the stored-tail segment")
    return ranges


def _source_rows(source: Path, original: bytes, ranges: list[tuple]) -> list[dict]:
    rows = []
    for first, last, cls, _ in ranges:
        if cls != "asset_yay0":
            continue
        path = sidecar_key(first)
        decoded = _file(source, path).read_bytes()
        stored = original[first:last]
        if stored[:4] != b"Yay0" or len(stored) < 16 or len(decoded) != struct.unpack_from(">I", stored, 4)[0]:
            raise AssetError(f"Frozen source size disagrees with stored header: {first:#x}")
        rows.append({"rom_start": first, "rom_end": last, "stored_sha256": sha256(stored),
                     "path": path, "size": len(decoded), "sha256": sha256(decoded)})
    return rows


def _evidence(source: Path, name: str, data: bytes) -> dict:
    path = f"{PROVENANCE_ROOT}/{name}"
    destination = source / path
    destination.parent.mkdir(parents=True, exist_ok=True)
    with destination.open("xb") as stream:
        stream.write(data)
    return {"path": path, "sha256": sha256(data), "size": len(data)}


def _read_evidence(source: Path, declaration: Any, name: str) -> bytes:
    if not isinstance(declaration, dict) or set(declaration) != {"path", "sha256", "size"}:
        raise AssetError("Invalid source provenance evidence declaration")
    if declaration["path"] != f"{PROVENANCE_ROOT}/{name}":
        raise AssetError("Unexpected source provenance evidence path")
    data = _file(source, declaration["path"]).read_bytes()
    if declaration != {"path": declaration["path"], "sha256": sha256(data), "size": len(data)}:
        raise AssetError("Changed source provenance evidence")
    return data


def _historical(source: Path, evidence: dict, rows: list[dict]) -> None:
    if set(evidence) != {"receipt", "inputs", "pack", "extractor"}:
        raise AssetError("Invalid historical import evidence")
    receipt_bytes = _read_evidence(source, evidence["receipt"], "r1-receipt.json")
    pack_bytes = _read_evidence(source, evidence["pack"], "r1-pack.json")
    if sha256(receipt_bytes) != R1_RECEIPT_SHA256 or sha256(pack_bytes) != R1_PACK_SHA256:
        raise AssetError("Untrusted historical receipt or pack evidence")
    receipt = json.loads(receipt_bytes)
    inputs = json.loads(_read_evidence(source, evidence["inputs"], "r1-input-manifest.json"))
    extractor = _read_evidence(source, evidence["extractor"], "r1-assets.py")
    historical = json.loads(pack_bytes)
    if (receipt["input_manifest"] != inputs
            or receipt["generated"].get("assets/tail/manifest.json") != sha256(pack_bytes)
            or receipt["target_sha256"] != rom.EXPECTED_SHA256
            or inputs["files"].get("tools/assets.py") != {"sha256": sha256(extractor), "size": len(extractor)}
            or historical.get("schema_version") != 1 or historical.get("kind") != MANIFEST_KIND):
        raise AssetError("Historical import evidence binding mismatch")
    expected = [[row["rom_start"], row["rom_end"], row["sha256"], True] for row in rows]
    if historical.get("streams") != expected or historical.get("exact_streams") != len(rows):
        raise AssetError("Historical stream records do not bind these frozen sources")
    declared = {row["path"]: {"sha256": row["sha256"], "size": row["size"]} for row in rows}
    frozen = {key: value for key, value in inputs["files"].items() if key.startswith("assets/tail/")}
    if frozen != declared:
        raise AssetError("Frozen sources differ from historical input inventory")


def _manifest(source: Path, original: bytes, rom_map: rommap.RomMap,
              rows: list[dict], provenance: dict) -> dict:
    return {"schema_version": 3, "kind": SOURCE_KIND, "target_sha256": sha256(original),
            **_identity(rom_map), "encoder": source_encoder_identity(),
            "provenance": provenance, "streams": rows, "history": []}


def extract(source_root: Path, reference: Path) -> dict[str, Any]:
    """Explicit import before snapshot; existing sources are never overwritten."""
    original = reference.read_bytes()
    rom.attest(original)
    rom_map = rommap.load(source_root, original)
    ranges = _ranges(source_root, rom_map)
    if source_root.is_symlink() or (source_root / "assets").is_symlink():
        raise AssetError("Symlink source destination")
    destination = source_root / "assets/tail"
    destination.mkdir(parents=True, exist_ok=False)
    (destination / "yay0").mkdir()
    for first, last, cls, _ in ranges:
        if cls == "asset_yay0":
            with (source_root / sidecar_key(first)).open("xb") as stream:
                stream.write(_decode(original[first:last]))
    rows = _source_rows(source_root, original, ranges)
    provenance = {"mode": "one-time-rom-decode", "phase": "before-snapshot",
                  "independent_authorship": False,
                  "tool": _evidence(source_root, "extract-assets.py", Path(__file__).read_bytes()),
                  "imported_sources": _evidence(source_root, "imported-sources.json", _json_bytes(rows)),
                  "decoder": encoder_fingerprint()}
    manifest = _manifest(source_root, original, rom_map, rows, provenance)
    with (source_root / SOURCE_MANIFEST).open("xb") as stream:
        stream.write(_json_bytes(manifest))
    return {"status": "EXTRACTED", "sources": len(rows), "source_manifest": SOURCE_MANIFEST}


def import_manifest(source_root: Path, reference: Path, historical_directory: Path) -> dict:
    """Explicitly migrate hash-pinned r1 sources; no decoding or recompression."""
    if (source_root / SOURCE_MANIFEST).exists() or (source_root / PROVENANCE_ROOT).exists():
        raise AssetError("Source provenance already exists; refusing to overwrite")
    original = reference.read_bytes()
    rom.attest(original)
    rom_map = rommap.load(source_root, original)
    rows = _source_rows(source_root, original, _ranges(source_root, rom_map))
    if _inventory(source_root, "assets/tail") != {row["path"] for row in rows}:
        raise AssetError("Missing or extra existing asset sources")
    names = {"receipt": ("r1-receipt.json", "receipt.json"),
             "inputs": ("r1-input-manifest.json", "input-manifest.json"),
             "pack": ("r1-pack.json", f"{SIDECAR_ROOT}/manifest.json"),
             "extractor": ("r1-assets.py", "source/tools/assets.py")}
    # Validate before writing even provenance files. Existing edited sources stay untouched.
    blobs = {key: _file(historical_directory, old).read_bytes() for key, (_, old) in names.items()}
    if sha256(blobs["receipt"]) != R1_RECEIPT_SHA256 or sha256(blobs["pack"]) != R1_PACK_SHA256:
        raise AssetError("Untrusted historical import")
    receipt = json.loads(blobs["receipt"])
    inputs = json.loads(blobs["inputs"])
    historical = json.loads(blobs["pack"])
    if (receipt["input_manifest"] != inputs
            or historical.get("streams") != [[r["rom_start"], r["rom_end"], r["sha256"], True] for r in rows]
            or {k: v for k, v in inputs["files"].items() if k.startswith("assets/tail/")} !=
               {r["path"]: {"sha256": r["sha256"], "size": r["size"]} for r in rows}
            or inputs["files"].get("tools/assets.py") !=
               {"sha256": sha256(blobs["extractor"]), "size": len(blobs["extractor"])}):
        raise AssetError("Historical evidence does not match existing sources")
    evidence = {key: _evidence(source_root, names[key][0], blob) for key, blob in blobs.items()}
    _historical(source_root, evidence, rows)
    provenance = {"mode": "hash-bound-r1-import", "phase": "before-snapshot",
                  "independent_authorship": False, "evidence": evidence}
    manifest = _manifest(source_root, original, rom_map, rows, provenance)
    with (source_root / SOURCE_MANIFEST).open("xb") as stream:
        stream.write(_json_bytes(manifest))
    return {"status": "IMPORTED", "sources": len(rows), "source_manifest": SOURCE_MANIFEST,
            "historical_only": True, "decodes": 0, "recompressions": 0}


def read_source_manifest(source: Path, rom_map: rommap.RomMap, original: bytes) -> dict:
    """Check frozen inputs; declared decode provenance is not proof of editability."""
    return _read_source_manifest(source, rom_map, original, rebinding=False)


def _read_source_manifest(source: Path, rom_map: rommap.RomMap, original: bytes,
                          *, rebinding: bool) -> dict:
    manifest = json.loads(_file(source, SOURCE_MANIFEST).read_bytes())
    version = manifest.get("schema_version") if isinstance(manifest, dict) else None
    extra = {"history"} if version == 3 else set()
    if not isinstance(manifest, dict) or set(manifest) != {
            "schema_version", "kind", "target_sha256", "map_id", "partition_sha256",
            "evidence_policy_sha256", "encoder", "provenance", "streams"} | extra:
        raise AssetError("Invalid frozen source manifest fields")
    if (version not in ({2, 3} if rebinding else {3}) or manifest["kind"] != SOURCE_KIND
            or manifest["target_sha256"] != sha256(original)):
        raise AssetError("Unsupported frozen source manifest; explicit rebind required")
    if not rebinding and any(manifest[key] != value for key, value in _identity(rom_map).items()):
        raise AssetError("Frozen source manifest does not belong to this ROM map")
    if not rebinding and manifest["encoder"] != source_encoder_identity():
        raise AssetError("Frozen source encoder implementation changed; explicit rebind required")
    rows = _source_rows(source, original, _ranges(source, rom_map))
    if manifest["streams"] != rows:
        raise AssetError("Frozen source declaration, path, size or hash changed")
    provenance = manifest["provenance"]
    if not isinstance(provenance, dict) or provenance.get("phase") != "before-snapshot" or provenance.get("independent_authorship") is not False:
        raise AssetError("Invalid source provenance: no decode-at-build or independent-ROM-copy claim")
    mode = provenance.get("mode")
    if mode == "hash-bound-r1-import":
        if set(provenance) != {"mode", "phase", "independent_authorship", "evidence"}:
            raise AssetError("Invalid historical provenance fields")
        _historical(source, provenance["evidence"], rows)
        evidence_paths = {item["path"] for item in provenance["evidence"].values()}
    elif mode == "one-time-rom-decode":
        if set(provenance) != {"mode", "phase", "independent_authorship", "tool", "imported_sources", "decoder"}:
            raise AssetError("Invalid one-time import provenance fields")
        tool = _read_evidence(source, provenance["tool"], "extract-assets.py")
        imported = _read_evidence(source, provenance["imported_sources"], "imported-sources.json")
        # These are declarations, not a trusted extractor execution attestation.
        # Exact recompression remains mandatory for rebind, pack and replay.
        if not tool or json.loads(imported) != rows:
            raise AssetError("Unbound declared extraction tool or imported sources")
        evidence_paths = {provenance["tool"]["path"], provenance["imported_sources"]["path"]}
    else:
        raise AssetError("Unsupported source provenance; decode-at-build and canonical-copy are forbidden")
    history = manifest.get("history", [])
    if not isinstance(history, list):
        raise AssetError("Invalid source manifest history")
    for item in history:
        name = f"manifest-{item['sha256']}.json"
        previous = json.loads(_read_evidence(source, item, name))
        if previous.get("streams") != rows or previous.get("provenance") != provenance:
            raise AssetError("Archived source manifest changed source or provenance")
        evidence_paths.add(item["path"])
    expected = {SOURCE_MANIFEST, *(row["path"] for row in rows), *evidence_paths}
    if _inventory(source, "assets/tail") != expected:
        raise AssetError("Missing or extra frozen asset files")
    return manifest


def rebind(source: Path, reference: Path) -> dict:
    """Explicit full-corpus migration; preserve prior manifest, never decode."""
    original = reference.read_bytes()
    rom.attest(original)
    rom_map = rommap.load(source, original)
    path = _file(source, SOURCE_MANIFEST)
    before = path.read_bytes()
    old = _read_source_manifest(source, rom_map, original, rebinding=True)
    encoder = _crunch64()
    for row in old["streams"]:
        decoded = _file(source, row["path"]).read_bytes()
        if encoder.yay0.compress(decoded) != original[row["rom_start"]:row["rom_end"]]:
            raise AssetError(f"Rebind refuses nonexact frozen stream: {row['rom_start']:#x}")
    if path.read_bytes() != before or _read_source_manifest(
            source, rom_map, original, rebinding=True) != old:
        raise AssetError("Source changed during rebind")
    manifest = _manifest(source, original, rom_map, old["streams"], old["provenance"])
    manifest["history"] = list(old.get("history", []))
    if all(old.get(key) == manifest[key] for key in manifest if key != "history"):
        raise AssetError("Manifest already bound; no migration needed")
    archive = _evidence(source, f"manifest-{sha256(before)}.json", before)
    manifest["history"].append(archive)
    temporary = path.with_name(path.name + ".rebind")
    with temporary.open("xb") as stream:
        stream.write(_json_bytes(manifest))
    temporary.replace(path)
    return {"status": "REBOUND_EXACT_STREAMS_ONLY", "accepted": False,
            "accepted_credit": 0, "streams": len(old["streams"]), "decodes": 0,
            "old_manifest": archive, "source_manifest_sha256": sha256(path.read_bytes()),
            "runtime_encoder": encoder_fingerprint()}


def _build_inputs(directory: Path) -> tuple[Path, bytes, rommap.RomMap]:
    options = yaml.safe_load(_file(directory, "splat.override.yaml").read_text())["options"]
    if Path(options["base_path"]).resolve() != directory.resolve():
        raise AssetError("Pack must run in the build directory named by its split options")
    original = Path(options["target_path"]).read_bytes()
    rom.attest(original)
    source = directory / "source"
    return source, original, rommap.load(source, original)


def _pack_record(source: Path, original: bytes, rom_map: rommap.RomMap,
                 frozen: dict, packed: bytes) -> dict:
    start, end = tail_interval(source)
    sources = {row["rom_start"]: row for row in frozen["streams"]}
    ranges = []
    for first, last, cls, ref in _ranges(source, rom_map):
        encoded = packed[first - start:last - start]
        if encoded != original[first:last]:
            raise AssetError(f"Packed stored interval changed: {first:#x}")
        record = {"rom_start": first, "rom_end": last, "class": cls, "ref": ref,
                  "sha256": sha256(encoded), "mode": "passthrough", "editable_bytes": 0}
        if cls == "asset_yay0":
            record.update(mode="encoded-frozen-source", editable_bytes=last - first,
                          source=sources[first])
        ranges.append(record)
    return {"schema_version": 2, "kind": MANIFEST_KIND, **_identity(rom_map),
            "target_sha256": sha256(original),
            "source_manifest_sha256": sha256(_file(source, SOURCE_MANIFEST).read_bytes()),
            "encoder": frozen["encoder"], "runtime_encoder": encoder_fingerprint(),
            "packer_sha256": sha256(Path(__file__).read_bytes()),
            "recipe": "frozen-yay0-encode-and-explicit-reference-passthrough",
            "tail": {"rom_start": start, "rom_end": end, "sha256": sha256(packed), "size": len(packed)},
            "ranges": ranges, "streams": frozen["streams"], "exact_streams": len(sources)}


def pack(directory: Path) -> dict[str, Any]:
    """Encode only validated frozen sources; never import or rescue from the ROM."""
    directory = directory.resolve()
    source, original, rom_map = _build_inputs(directory)
    frozen = read_source_manifest(source, rom_map, original)
    start, end = tail_interval(source)
    output = _file(directory, TAIL_OUTPUT)
    if output.read_bytes() != original[start:end]:
        raise AssetError("Split tail storage disagrees with the original before packing")
    sidecars = directory / SIDECAR_ROOT
    if sidecars.exists():
        raise AssetError("Generated tail sidecars already exist")
    encoder = _crunch64()
    parts = []
    for first, last, cls, _ in _ranges(source, rom_map):
        if cls != "asset_yay0":
            parts.append(original[first:last])  # Explicit reference passthrough, zero editable credit.
            continue
        decoded = _file(source, sidecar_key(first)).read_bytes()
        encoded = encoder.yay0.compress(decoded)
        if encoded != original[first:last]:
            raise AssetError(f"Frozen decoded source does not reproduce its Yay0 stream: {first:#x}")
        parts.append(encoded)
    packed = b"".join(parts)
    if packed != original[start:end]:
        raise AssetError("Packed tail differs from the original")
    # Recheck frozen sources before committing any generated evidence.
    if read_source_manifest(source, rom_map, original) != frozen:
        raise AssetError("Frozen source manifest changed during packing")
    manifest = _pack_record(source, original, rom_map, frozen, packed)
    (sidecars / "yay0").mkdir(parents=True, exist_ok=False)
    for row in frozen["streams"]:
        decoded = _file(source, row["path"]).read_bytes()
        if len(decoded) != row["size"] or sha256(decoded) != row["sha256"]:
            raise AssetError("Frozen source changed while copying generated sidecar")
        with (directory / "generated" / row["path"]).open("xb") as stream:
            stream.write(decoded)
    with (sidecars / "manifest.json").open("xb") as stream:
        stream.write(_json_bytes(manifest))
    temporary = output.with_name(output.name + ".packed")
    with temporary.open("xb") as stream:
        stream.write(packed)
    temporary.replace(output)
    return manifest


def read_manifest(directory: Path, rom_map: rommap.RomMap) -> dict[int, bool]:
    """Re-encode actual frozen sources before reporting any per-stream build-source proof."""
    source, original, actual_map = _build_inputs(directory)
    if _identity(actual_map) != _identity(rom_map):
        raise AssetError("Tail pack does not belong to this ROM map")
    frozen = read_source_manifest(source, rom_map, original)
    packed = _file(directory, TAIL_OUTPUT).read_bytes()
    start, end = tail_interval(source)
    if packed != original[start:end]:
        raise AssetError("Packed tail differs from attested reference storage")
    manifest = json.loads(_file(directory, f"{SIDECAR_ROOT}/manifest.json").read_bytes())
    if manifest != _pack_record(source, original, rom_map, frozen, packed):
        raise AssetError("Tail pack manifest does not match actual frozen source and stored evidence")
    encoder = _crunch64()
    for row in frozen["streams"]:
        sidecar = _file(directory, "generated/" + row["path"]).read_bytes()
        if len(sidecar) != row["size"] or sha256(sidecar) != row["sha256"]:
            raise AssetError("Decoded sidecar changed after packing")
        decoded = _file(source, row["path"]).read_bytes()
        if len(decoded) != row["size"] or sha256(decoded) != row["sha256"]:
            raise AssetError("Frozen source changed during roundtrip validation")
        encoded = encoder.yay0.compress(decoded)
        stored = packed[row["rom_start"] - start:row["rom_end"] - start]
        if encoded != stored:
            raise AssetError(f"Frozen source fails actual encoder roundtrip: {row['rom_start']:#x}")
    expected = {f"{SIDECAR_ROOT}/manifest.json", *("generated/" + row["path"] for row in frozen["streams"])}
    if _inventory(directory, SIDECAR_ROOT) != expected:
        raise AssetError("Missing or extra generated tail evidence")
    return {row["rom_start"]: True for row in frozen["streams"]}


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    commands = parser.add_subparsers(dest="command", required=True)
    command = commands.add_parser("pack", help="encode frozen sources and pack the stored tail")
    command.add_argument("override", type=Path)
    for name in ("extract", "import-manifest", "validate-sources", "rebind"):
        command = commands.add_parser(name, help="explicit pre-build source provenance operation")
        command.add_argument("--root", type=Path, required=True)
        command.add_argument("--rom", type=Path, required=True)
        if name == "import-manifest":
            command.add_argument("--historical-build", type=Path, required=True)
    args = parser.parse_args()
    try:
        if args.command == "extract":
            result = extract(args.root, args.rom)
        elif args.command == "import-manifest":
            result = import_manifest(args.root, args.rom, args.historical_build)
        elif args.command == "rebind":
            result = rebind(args.root, args.rom)
        elif args.command == "validate-sources":
            original = args.rom.read_bytes()
            rom.attest(original)
            manifest = read_source_manifest(args.root, rommap.load(args.root, original), original)
            result = {"status": "DECLARED_PROVENANCE_ONLY_NOT_EDITABILITY", "sources": len(manifest["streams"]),
                      "source_manifest_sha256": sha256((args.root / SOURCE_MANIFEST).read_bytes()),
                      "accepted_roundtrip": False}
        else:
            if args.override.resolve() != Path.cwd() / "splat.override.yaml":
                raise AssetError("Run pack from the build directory that owns the split override")
            manifest = pack(Path.cwd())
            result = {"status": "PACKED", "streams": len(manifest["streams"]),
                      "exact_streams": manifest["exact_streams"], "encoder": manifest["encoder"]}
    except (AssetError, rommap.MapError, ValueError, OSError, KeyError, TypeError) as error:
        parser.exit(1, f"{error}\n")
    print(json.dumps(result, sort_keys=True))


if __name__ == "__main__":
    main()
