"""Opt-in fixed-window Yay0 sidecars; original compressed ROM storage is unchanged.

Private proposal for tools/assets.py. Format semantics follow the sealed triage
and pinned Orthrus 0.2.2 implementation, not its swapped header-comment labels.
Nonempty, aligned, ordered-stream and work limits are this bounded policy.
"""
from __future__ import annotations

import argparse
import hashlib
import json
import re
import struct
from dataclasses import asdict, dataclass
from pathlib import Path
from typing import Any

import rom


MAX_INPUT = 1 << 20
MAX_OUTPUT = 4 << 20
MAX_TOKENS = 1 << 20
REVIEWED_WINDOWS = (
    (0x240C20, 0x241B60), (0x241B60, 0x242A20),
    (0x242A20, 0x2435F0), (0x2435F0, 0x253040),
)
TRIAGE_HANDOFF = "fcfe35cd7b1c742a24c8352df5fc863b294a0bed755a2ea1054a6b092d2a1f53"
TRIAGE_MANIFEST = "66dd508d112ce56cd224f523e744389c27883d5a9a042360536149eb611c603d"
ROM_AUTHORITY_SHA256 = hashlib.sha256(Path(rom.__file__).read_bytes()).hexdigest()


class AssetError(ValueError):
    """Input, policy, identity or output-directory validation failed."""


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


@dataclass(frozen=True)
class Region:
    name: str
    start: int
    end: int
    window_sha256: str
    consumed_end: int
    consumed_sha256: str
    gap_sha256: str
    decoded_size: int
    decoded_sha256: str
    cursors: tuple[int, int, int]


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


def integer(value: object, field: str) -> int:
    if type(value) is not int:
        raise AssetError(f"{field} must be an integer")
    return value


def hash_value(value: object, field: str) -> str:
    if not isinstance(value, str) or not re.fullmatch(r"[0-9a-f]{64}", value):
        raise AssetError(f"{field} must be a lowercase SHA256")
    return value


def validate_regions(document: Any) -> tuple[Region, ...]:
    keys = {"schema_version", "kind", "target_sha256", "target_size", "triage_handoff_sha256",
            "triage_manifest_sha256", "limits", "regions"}
    if not isinstance(document, dict) or set(document) != keys:
        raise AssetError("asset configuration fields do not match schema")
    if (integer(document["schema_version"], "schema_version") != 1
            or document["kind"] != "shiren2-four-reviewed-yay0-sidecars"
            or document["target_sha256"] != rom.EXPECTED_SHA256
            or integer(document["target_size"], "target_size") != rom.EXPECTED_SIZE
            or document["triage_handoff_sha256"] != TRIAGE_HANDOFF
            or document["triage_manifest_sha256"] != TRIAGE_MANIFEST):
        raise AssetError("asset configuration target or triage identity changed")
    limits = document["limits"]
    if (not isinstance(limits, dict) or set(limits) != {"max_input", "max_output", "max_tokens"}
            or any(type(v) is not int for v in limits.values())
            or limits != {"max_input": MAX_INPUT, "max_output": MAX_OUTPUT, "max_tokens": MAX_TOKENS}):
        raise AssetError("asset configuration limits changed")
    rows = document["regions"]
    if not isinstance(rows, list) or len(rows) != 4:
        raise AssetError("exactly four reviewed windows are required")
    regions = []
    row_keys = {"id", "rom_start", "rom_end", "window_sha256", "consumed_rom_end",
                "consumed_sha256", "gap_sha256", "decoded_size", "decoded_sha256", "expected_cursors"}
    for row, (start, end) in zip(rows, REVIEWED_WINDOWS, strict=True):
        if not isinstance(row, dict) or set(row) != row_keys:
            raise AssetError("asset region fields do not match schema")
        if (integer(row["rom_start"], "rom_start"), integer(row["rom_end"], "rom_end")) != (start, end):
            raise AssetError("reviewed window geometry changed")
        name = f"rom-{start:08x}"
        if row["id"] != name:
            raise AssetError("region identifier must equal its fixed ROM name")
        consumed = integer(row["consumed_rom_end"], "consumed_rom_end")
        size = integer(row["decoded_size"], "decoded_size")
        cursors = row["expected_cursors"]
        if (not start + 16 <= consumed <= end or not 0 < size <= MAX_OUTPUT
                or not isinstance(cursors, dict) or set(cursors) != {"mask_end", "link_end", "literal_end"}):
            raise AssetError("invalid consumed endpoint, size or cursors")
        regions.append(Region(name, start, end, hash_value(row["window_sha256"], "window_sha256"), consumed,
                              hash_value(row["consumed_sha256"], "consumed_sha256"),
                              hash_value(row["gap_sha256"], "gap_sha256"), size,
                              hash_value(row["decoded_sha256"], "decoded_sha256"),
                              tuple(integer(cursors[k], k) for k in ("mask_end", "link_end", "literal_end"))))
    return tuple(regions)


def output_destination(output: Path, allowed_root: Path) -> Path:
    if ".." in output.parts or output.name in ("", ".", ".."):
        raise AssetError("unsafe output path")
    destination = output.absolute()
    root = allowed_root.resolve(strict=True)
    if not destination.is_relative_to(root) or destination == root:
        raise AssetError("output must be a new directory below the private output root")
    # Reject symlink traversal, including dangling output links, before resolving.
    for component in (destination, *destination.parents):
        if component.is_symlink():
            raise AssetError("output path contains a symlink")
        if component == root:
            break
    if destination.exists():
        raise AssetError("output collision: destination already exists")
    if not destination.parent.is_dir() or not destination.parent.resolve(strict=True).is_relative_to(root):
        raise AssetError("output parent must already exist inside the private output root")
    return destination


def extract_sidecars(rom_path: Path, config_path: Path, output: Path,
                     *, allowed_root: Path | None = None) -> dict[str, object]:
    allowed = allowed_root if allowed_root is not None else rom.PROJECT / "scratch"
    destination = output_destination(output, allowed)
    if rom_path.stat().st_size != rom.EXPECTED_SIZE:
        raise AssetError("original ROM size changed")
    original = rom_path.read_bytes()
    rom_identity = rom.attest(original)  # Existing fixed-target authority; never normalize or import.
    authority_path = Path(rom.__file__)
    if sha256(authority_path.read_bytes()) != ROM_AUTHORITY_SHA256:
        raise AssetError("ROM authority source changed after import")
    if config_path.stat().st_size > 65536:
        raise AssetError("asset configuration is too large")
    config_bytes = config_path.read_bytes()
    regions = validate_regions(json.loads(config_bytes))
    artifacts: list[tuple[str, bytes]] = []
    records: list[dict[str, object]] = []
    for region in regions:
        window = original[region.start:region.end]
        if sha256(window) != region.window_sha256:
            raise AssetError(f"{region.name}: stored-window identity changed")
        decoded, evidence = decode_yay0(window)
        consumed = region.start + evidence.consumed_end
        gap = window[evidence.consumed_end:]
        if ((evidence.mask_end, evidence.link_end, evidence.literal_end) != region.cursors
                or consumed != region.consumed_end
                or sha256(window[:evidence.consumed_end]) != region.consumed_sha256
                or sha256(gap) != region.gap_sha256 or any(gap)
                or len(decoded) != region.decoded_size or sha256(decoded) != region.decoded_sha256):
            raise AssetError(f"{region.name}: consumed stream, cursors, gap or output identity changed")
        filename = f"{region.name}.decoded.bin"  # No configurable filename or traversal surface.
        artifacts.append((filename, decoded))
        intervals = {"header": (0, 16), "mask_words": (16, evidence.mask_end),
                     "links": (evidence.link_offset, evidence.link_end),
                     "literals_and_lengths": (evidence.literal_offset, evidence.literal_end)}
        records.append({
            "id": region.name,
            "stored_window": {"rom_start": region.start, "rom_end": region.end, "sha256": sha256(window)},
            "consumed_stream": {"rom_start": region.start, "rom_end": consumed, "sha256": sha256(window[:evidence.consumed_end])},
            "consulted_streams": {name: {"rom_start": region.start + a, "rom_end": region.start + b,
                                         "sha256": sha256(window[a:b])} for name, (a, b) in intervals.items()},
            "cursors_and_tokens": asdict(evidence),
            "gap": {"rom_start": consumed, "rom_end": region.end, "size": len(gap),
                    "sha256": sha256(gap), "hex": gap.hex(), "all_zero": True},
            "decoded_sidecar": {"path": filename, "size": len(decoded), "sha256": sha256(decoded)},
        })
    manifest: dict[str, object] = {
        "schema_version": 1, "kind": "shiren2-opt-in-yay0-sidecar-extraction",
        "original_rom": {**rom_identity, "source_path": str(rom_path.resolve())},
        "configuration_sha256": sha256(config_bytes),
        "decoder": {"id": "bounded-yay0-sidecars-v1", "source_sha256": sha256(Path(__file__).read_bytes()),
                    "rom_authority_source_sha256": ROM_AUTHORITY_SHA256,
                    "limits": {"max_input": MAX_INPUT, "max_output": MAX_OUTPUT, "max_tokens": MAX_TOKENS}},
        "triage": {"handoff_sha256": TRIAGE_HANDOFF, "manifest_sha256": TRIAGE_MANIFEST},
        "regions": records, "original_compressed_windows_and_gaps_unchanged": True,
        "sidecars_only": True, "recompression": False, "accepted_c_credit": 0,
    }
    # Validate and decode everything before the exclusive directory reservation.
    # Each file is also exclusive. An I/O failure can leave an incomplete private
    # directory; only a successfully written manifest represents complete output.
    destination.mkdir(mode=0o700, exist_ok=False)
    for name, data in artifacts:
        with (destination / name).open("xb") as stream:
            stream.write(data)
    with (destination / "manifest.json").open("x", encoding="utf-8") as stream:
        stream.write(json.dumps(manifest, sort_keys=True, indent=2, ensure_ascii=False) + "\n")
    return manifest


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    commands = parser.add_subparsers(dest="command", required=True)
    extract = commands.add_parser("extract", help="explicitly produce four private decoded sidecars")
    extract.add_argument("--rom", type=Path, default=rom.CANONICAL_ROM)
    extract.add_argument("--config", type=Path, default=Path(__file__).resolve().parents[1] / "config/asset_regions.json")
    extract.add_argument("--output", type=Path, required=True)
    args = parser.parse_args()
    try:
        manifest = extract_sidecars(args.rom, args.config, args.output)
    except (AssetError, ValueError, OSError) as error:
        parser.exit(1, f"{error}\n")
    print(json.dumps({"status": "EXTRACTED_PRIVATE_SIDECARS", "assets": len(manifest["regions"]),
                      "output": str(args.output), "accepted_c_credit": 0}, indent=2))


if __name__ == "__main__":
    main()
