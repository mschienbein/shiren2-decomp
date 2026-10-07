#!/usr/bin/env python3
"""Import and verify the exact Japanese ROM, preserving the user's original."""
from __future__ import annotations

import argparse
import hashlib
import json
import struct
import subprocess
import zlib
from datetime import datetime, timezone
from pathlib import Path

PROJECT = Path(__file__).resolve().parents[1]
REPOSITORY = PROJECT.parents[1]
CANONICAL_ROM = REPOSITORY / "inputs/shiren2/baserom.jp.z64"
EXPECTED_SIZE = 0x2000000
EXPECTED_SHA256 = "4073a9f6516ef5d15cdd8b259a952a7d07633a2815f2a738fb5d98415d30a458"
EXPECTED_SHA1 = "f97b070a1fa6ab3d190b36d6b68a2ef182d35233"


def normalize(data: bytes) -> tuple[bytes, str]:
    if len(data) < 0x40 or len(data) % 4:
        raise ValueError("ROM must contain a complete header and a whole number of words")
    magic = data[:4]
    if magic == bytes.fromhex("80371240"):
        return data, "big-endian"
    if magic == bytes.fromhex("37804012"):
        result = bytearray(data)
        result[::2], result[1::2] = data[1::2], data[::2]
        return bytes(result), "byte-swapped"
    if magic == bytes.fromhex("40123780"):
        result = bytearray(len(data))
        for index in range(4):
            result[index::4] = data[3 - index::4]
        return bytes(result), "little-endian words"
    raise ValueError(f"Unsupported ROM magic: {magic.hex()}")


def attest(data: bytes) -> dict[str, object]:
    digest = hashlib.sha256(data).hexdigest()
    if len(data) != EXPECTED_SIZE or digest != EXPECTED_SHA256:
        raise ValueError(f"Wrong target: size={len(data)}, sha256={digest}")
    return {
        "size": len(data),
        "sha256": digest,
        "sha1": hashlib.sha1(data).hexdigest(),
        "crc32": f"{zlib.crc32(data):08x}",
        "magic": data[:4].hex(),
        "title": data[0x20:0x34].decode("shift_jis").rstrip(),
        "entrypoint": f"0x{struct.unpack_from('>I', data, 8)[0]:08X}",
        "header_crc1": f"0x{struct.unpack_from('>I', data, 0x10)[0]:08X}",
        "header_crc2": f"0x{struct.unpack_from('>I', data, 0x14)[0]:08X}",
        "game_id": data[0x3B:0x3F].decode("ascii"),
        "revision": data[0x3F],
    }


def import_rom(source: Path) -> dict[str, object]:
    raw = source.read_bytes()
    canonical, order = normalize(raw)
    report = attest(canonical)
    relative = CANONICAL_ROM.relative_to(REPOSITORY)
    subprocess.run(
        ["git", "check-ignore", "--quiet", str(relative)], cwd=REPOSITORY, check=True
    )
    if CANONICAL_ROM.exists() and CANONICAL_ROM.read_bytes() != canonical:
        raise ValueError("Existing canonical ROM differs; refusing to overwrite it")
    CANONICAL_ROM.parent.mkdir(parents=True, exist_ok=True)
    if not CANONICAL_ROM.exists():
        CANONICAL_ROM.write_bytes(canonical)
    report.update({
        "source_path": str(source.resolve()),
        "source_byte_order": order,
        "source_sha256": hashlib.sha256(raw).hexdigest(),
        "canonical_path": str(relative),
        "imported_at": datetime.now(timezone.utc).isoformat(),
    })
    (CANONICAL_ROM.parent / "import.json").write_text(
        json.dumps(report, indent=2, ensure_ascii=False) + "\n", encoding="utf-8"
    )
    return report


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    commands = parser.add_subparsers(dest="command", required=True)
    importer = commands.add_parser("import")
    importer.add_argument("source", type=Path)
    verifier = commands.add_parser("verify")
    verifier.add_argument("rom", type=Path, nargs="?", default=CANONICAL_ROM)
    args = parser.parse_args()
    try:
        report = import_rom(args.source) if args.command == "import" else attest(args.rom.read_bytes())
    except (ValueError, OSError, subprocess.CalledProcessError) as error:
        parser.exit(1, f"{error}\n")
    print(json.dumps(report, indent=2, ensure_ascii=False))


if __name__ == "__main__":
    main()
