"""asm-differ settings. SHIREN2_BUILD chooses a verified build directory."""
from __future__ import annotations

import os
import json
from pathlib import Path

PROJECT = Path(__file__).resolve().parent


def apply(config: dict, args: object) -> None:
    name = os.environ.get("SHIREN2_BUILD")
    build = PROJECT / "build" / name if name else (PROJECT / json.loads((PROJECT / "build/latest.json").read_text())["receipt"]).parent
    config.update({
        "arch": "mips",
        "baseimg": str(PROJECT.parents[1] / "inputs/shiren2/baserom.jp.z64"),
        "myimg": str(build / "shiren2.z64"),
        "mapfile": str(build / "shiren2.map"),
        "build_dir": str(build / "obj"),
        "source_directories": ["src", "include", "config"],
        "objdump_executable": str(PROJECT / ".cache/binutils/bin/mips64-elf-objdump"),
        "objdump_flags": ["-M", "reg-names=32"],
    })
