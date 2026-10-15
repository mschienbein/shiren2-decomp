#!/usr/bin/env python3
"""Bounded n_aspMain binary classification; never CPU C or editable-source credit.

The witness is the unmodified ELF member of the pinned public SDK repack. ELF
section sizes and global end symbols establish stored extents, not DMA capacity.
OS2.0L describes the containing distribution, not the game's unique SDK version.
"""
from __future__ import annotations

import argparse
import hashlib
import json
import struct
from pathlib import Path

from spimdisasm.elf32 import Elf32File

from images import load_inventory
from rom import CANONICAL_ROM, PROJECT, attest

OBJECT_PATH = "config/rom-evidence/n_aspMain.o"
OBJECT_SHA256 = "93b92bdcc86d779cca5bf23ff0806bb3b2d3f886df5c612b1cfe023a829e20e1"
ARCHIVE_URL = "https://ultra64.ca/files/software/other/sdks/n64sdk.7z"
ARCHIVE_SHA256 = "9e3969af9b14e6310ea785fa5b2a2e0c71021b1baa1ad122ff931521659e93fb"
ARCHIVE_MEMBER = "ultra/usr/lib/PR/n_aspMain.o"
# Whole original windows, including task constructor and submission delay slots.
LOADER_BINDINGS = (
    ("audio_job_producer", 0x1006C8, 0x1007C0, "944a9a1ef21f7a00589da352c16cc1f080f94f50b9085c0e5fddd4d0168f58ca"),
    ("audio_task_constructor", 0x1004B4, 0x10056C, "58cd551c346aa4b1e54c2c28babc9e626ac74faa4dc523eae6db8a2169a78d0b"),
    ("resident_sp_loader", 0xDE14, 0xDE88, "3d50af50874e12c4db73d3845bb3b79a6949eca901254dd900ffedd31abba334"),
    ("rspboot", 0x109A50, 0x109B20, "5759e9bb21f2e504bfbb3e5b75173cb81aa50c60b19e77bcee1d0f6fc34e8fa4"),
)


def _sha(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def _word(original: bytes, offset: int) -> int:
    return struct.unpack_from(">I", original, offset)[0]


def _address_pair(original: bytes, site: int) -> int:
    """Decode a contiguous lui/addiu pair, not a pointer inferred from a label."""
    upper, lower = _word(original, site), _word(original, site + 4)
    register = (upper >> 16) & 31
    if (upper >> 26 != 15 or (upper >> 21) & 31 or lower >> 26 != 9
            or (lower >> 21) & 31 != register or (lower >> 16) & 31 != register):
        raise ValueError("Audio address is not a lui/addiu pair")
    immediate = lower & 0xFFFF
    if immediate & 0x8000:
        immediate -= 0x10000
    return (((upper & 0xFFFF) << 16) + immediate) & 0xFFFFFFFF


def verify(original: bytes, source_root: Path = PROJECT) -> dict:
    """Replay bounded instructions and parse the exact external ELF witness.

    Caller attests the whole ROM (the CLI and rommap both do). No map flags,
    asserted extents, expected-byte edits, ROM extraction or source credit.
    """
    loader = []
    for name, start, end, expected in LOADER_BINDINGS:
        data = original[start:end]
        if _sha(data) != expected:
            raise ValueError(f"Audio loader identity changed: {name}")
        loader.append({"id": name, "rom_start": start, "rom_end": end,
                       "sha256": expected, "words": [f"{w:08x}" for w in struct.unpack(f">{len(data) // 4}I", data)]})
    initial_callbacks = struct.unpack_from(">4I", original, 0x11BAE0)
    if initial_callbacks != (0x8012D3B0, 0x8012D414, 0x8012D474, 0x80148AA0):
        raise ValueError("Audio initial callback table/slot changed")
    inventory = load_inventory(source_root / "config/images.json")
    delta = inventory.images["main_14400"].vram_rom_delta
    text_ram = _address_pair(original, 0x1006EC)
    data_ram = _address_pair(original, 0x1006F8)
    text_start, data_start = text_ram - delta, data_ram - delta
    boot_start = _address_pair(original, 0x1004FC)
    boot_size = _address_pair(original, 0x100504) - boot_start
    descriptor_text = _word(original, 0x10052C) & 0xFFFF
    descriptor_data = _word(original, 0x100534) & 0xFFFF
    text_request = (_word(original, 0x109A5C) & 0xFFFF) + 1

    obj = (source_root / OBJECT_PATH).read_bytes()
    if _sha(obj) != OBJECT_SHA256:
        raise ValueError("Audio SDK object identity changed")
    # Pin before parsing: a substituted/truncated ELF is never parser input.
    elf = Elf32File(obj)
    header = elf.header
    if (obj[:7] != b"\x7fELF\x01\x02\x01" or header.type != 1 or header.machine != 8
            or header.flags != 0x10000000 or header.phnum != 0):
        raise ValueError("Audio SDK container must be ELF32 big-endian MIPS ET_REL")
    if elf.symtab is None or elf.strtab is None:
        raise ValueError("Audio SDK object lacks end symbols")
    symbols = {elf.strtab[symbol.name]: symbol for symbol in elf.symtab if symbol.stBind == 1}
    sections = []
    bindings = ((".text", "Text", text_start, "rsp_audio_text", "rsp_ucode_text", 6, 2),
                (".data", "Data", data_start, "rsp_audio_data", "rsp_ucode_data", 3, 1))
    for name, suffix, start, component, cls, flags, symbol_type in bindings:
        selected = [(i, s) for i, s in enumerate(elf.sectionHeaders) if elf.shstrtab[s.name] == name]
        if len(selected) != 1:
            raise ValueError(f"Audio SDK section is not unique: {name}")
        index, section = selected[0]
        first, last = (symbols.get(f"n_aspMain{suffix}{edge}") for edge in ("Start", "End"))
        if (section.type != 1 or section.flags != flags or section.addr != 0 or section.addralign != 16
                or first is None or last is None or first.shndx != index or last.shndx != index
                or first.value != 0 or first.size != section.size or last.value != section.size or last.size != 0
                or first.stType != symbol_type or last.stType != symbol_type):
            raise ValueError(f"Audio SDK section/end-symbol contract changed: {name}")
        if any(s.type in {4, 9} and s.info == index for s in elf.sectionHeaders):
            raise ValueError(f"Audio SDK section requires relocation: {name}")
        payload = obj[section.offset:section.offset + section.size]
        end = start + last.value
        if len(payload) != section.size or not 0 <= start < end <= len(original) or original[start:end] != payload:
            raise ValueError(f"Audio SDK section differs from ROM: {name}")
        sections.append({"component_id": component, "section": name, "class": cls,
                         "rom_start": start, "rom_end": end, "size": len(payload),
                         "storage_vram_start": start + delta, "storage_vram_end": end + delta,
                         "elf_section_index": index, "elf_file_offset": section.offset,
                         "elf_alignment": section.addralign, "start_symbol": f"n_aspMain{suffix}Start",
                         "end_symbol": f"n_aspMain{suffix}End", "sha256": _sha(payload),
                         "md5": hashlib.md5(payload).hexdigest(), "status": "classified_binary"})
    return {"status": "PASS_BINARY_IDENTITY_ONLY", "object_sha256": OBJECT_SHA256,
            "object_size": len(obj), "archive_url": ARCHIVE_URL, "archive_sha256": ARCHIVE_SHA256,
            "archive_member": ARCHIVE_MEMBER, "sdk_distribution": "OS2.0L-labelled public repack; not unique game SDK provenance",
            "machine": {"elf_class": 32, "byte_order": "big", "e_type": header.type,
                        "e_machine": header.machine, "e_flags": header.flags, "execution": "RSP, not VR4300 CPU"},
            "sections": sections, "loader_windows": loader,
            "task": {"type": 2, "bootstrap_storage_vram": boot_start, "bootstrap_size": boot_size,
                     "descriptor_text_size": descriptor_text, "descriptor_data_size": descriptor_data,
                     "initial_callback_table_rom": 0x11BAE0, "initial_callbacks_and_slot": list(initial_callbacks),
                     "callback_limit": "initial table is mutable; no exhaustive runtime ordering claim",
                     "text_request": {"rom_start": text_start, "rom_end": text_start + text_request,
                                      "sp_start": 0x04001080, "size": text_request},
                     "data_request": {"rom_start": data_start, "rom_end": data_start + descriptor_data,
                                      "sp_start": 0x04000000, "size": descriptor_data},
                     "transfer_limits": "normal fresh-bootstrap programmed requests, not observed DMA completion or stored extents"},
            "matched_c_bytes": 0, "matched_cpp_bytes": 0, "editable_source_bytes": 0,
            "accepted": False, "whole_denominator_closed": False}


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--root", type=Path, default=PROJECT)
    parser.add_argument("--rom", type=Path, default=CANONICAL_ROM)
    args = parser.parse_args()
    try:
        original = args.rom.read_bytes()
        target = attest(original)
        result = verify(original, args.root)
        result["target"] = target
        print(json.dumps(result, indent=2))
    except (ValueError, OSError) as error:
        parser.exit(1, f"{error}\n")


if __name__ == "__main__":
    main()
