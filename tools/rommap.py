#!/usr/bin/env python3
"""Frozen whole-ROM classification map: every original byte in exactly one classified range.

`config/rom_map.json` is generated from the fixed original ROM, `config/images.json`
(images and RSP components), the frozen CPU function catalogue and the reviewed rules in
`config/rom_regions.json`. It is frozen: matching work never changes it. Range classes say
what the bytes are; their matching status is derived per accepted receipt by `account`.
Only `cpu_text_function` ranges can carry C/C++ credit, and only through exact receipt matches.
A located raw asset row keeps its class and ref; it is research-classified (classified_binary)
only when a pinned witness in `config/rom-evidence/asset-formats.json` replays through
`tools/assetformat.py` (exact row, positive typing of every byte, consumer cross-checks).
A compiler-generated g++ vtable inside a cpu_data row is carved out as its own
`compiler_generated_vtable` row (classified_binary, never owned or matched credit) only when its
pinned witness in `config/rom-evidence/gxx-vtables.json` replays through `tools/gxxvtable.py`
against the base partition (`cpu_vtables`); every other cpu_data byte stays open (user ruling
scratch/omp/rulings/VTABLES-CLASSIFY-NO-CREDIT-20261014.md).

    rommap.py generate --catalogue docs/progress-catalogues/<id>.json   # writes config/rom_map.json
    rommap.py check [--full --catalogue ...]                           # validate (and regenerate)
"""
from __future__ import annotations

import argparse
import bisect
import hashlib
import json
import re
import struct
from dataclasses import dataclass, field, replace
from pathlib import Path
from typing import Any, Iterable, Mapping

import yaml
import assetformat
import gxxvtable
import mipsflow
import rspaudio
import toolchain_library

from evidence import COMPILED_SUFFIXES
from toolchain_library import FAMILY, match_family, validate_source
from images import ImageInventory, load_inventory
from rom import CANONICAL_ROM, EXPECTED_SHA256, EXPECTED_SIZE, PROJECT, attest

MAP_PATH = "config/rom_map.json"
RULES_PATH = "config/rom_regions.json"
HANDWRITTEN_PATH = "config/handwritten.json"
CATALOGUE_PATH = "config/rom_cpu_catalogue.json"
MAP_KIND = "shiren2-rom-map"
RULES_KIND = "shiren2-rom-region-rules"
EVIDENCE_PATH = "config/rom_evidence.json"
EVIDENCE_SHA256 = "1b35f4e4a105e0b1b22db1d531d4399ace046632ae681bc2235046693b0c4da0"

# Class -> accounting group. Groups have their own units in `account`.
CLASSES: Mapping[str, str] = {
    "rom_header": "boot", "ipl3_bootcode": "boot",
    "cpu_text_function": "cpu_code",
    "cpu_data": "cpu_data", "image_opaque": "cpu_image_opaque",
    "compiler_generated_vtable": "cpu_data",
    "cpu_text_unowned": "cpu_image_opaque", "cpu_text_unexplained": "cpu_image_opaque",
    "rsp_boot": "rsp_ucode", "rsp_ucode_text": "rsp_ucode", "rsp_ucode_data": "rsp_ucode",
    "asset_yay0": "asset_compressed",
    "asset_audio_bank_ptrtable": "asset_raw", "asset_audio_wavetable": "asset_raw",
    "asset_hvqm2_video": "asset_raw", "asset_demo_input": "asset_raw",
    "asset_raw_referenced": "asset_raw", "asset_raw_code_based": "asset_raw",
    "asset_iff_container_meta": "asset_raw", "asset_iff_payload": "asset_raw", "asset_raw_unattributed": "asset_raw",
    "asset_raw_unexplained": "asset_raw",
    "pad_align_zero": "padding", "pad_align_ff": "padding", "rom_padding_zero": "padding",
    "cpu_text_padding": "padding", "cpu_data_padding": "padding",
}
GROUP_UNITS: Mapping[str, str] = {
    "boot": "bytes", "cpu_code": "instruction bytes", "cpu_data": "bytes", "cpu_image_opaque": "bytes",
    "rsp_ucode": "bytes", "asset_compressed": "stream bytes", "asset_raw": "bytes", "padding": "bytes",
}
BUILD_STATUSES = frozenset({"matched_c", "matched_cpp", "matched_toolchain_library", "owned_c", "roundtrip"})
RESEARCH_STATUSES = frozenset({"classified_binary", "classified_handwritten", "closed", "closed_unexplained_residual"})
OPEN_STATUSES = frozenset({"open", "located"})
_ZERO_CLASSES = frozenset({"pad_align_zero", "rom_padding_zero", "cpu_text_padding", "cpu_data_padding"})
_RSP_CLASS = {"rsp_boot": "rsp_boot", "rsp_gfx_text": "rsp_ucode_text", "rsp_gfx_data": "rsp_ucode_data"}
_ID = re.compile(r"[A-Za-z][A-Za-z0-9_]*\Z")
_SHA = re.compile(r"[0-9a-f]{64}\Z")


class MapError(ValueError):
    """The map, its rules or its accounting inputs are invalid."""


def _int(value: object, label: str) -> int:
    if type(value) is not int or value < 0:
        raise MapError(f"{label} must be a nonnegative integer")
    return value


def _text(value: object, label: str) -> str:
    if not isinstance(value, str) or not value.strip():
        raise MapError(f"{label} must be nonempty text")
    return value


def _record(value: object, label: str, fields: set[str]) -> dict[str, Any]:
    if not isinstance(value, dict) or set(value) != fields:
        raise MapError(f"{label} fields do not match the schema")
    return value


def _identity(value: object, label: str, seen: set[str]) -> str:
    if not isinstance(value, str) or not _ID.fullmatch(value) or value in seen:
        raise MapError(f"{label} identity is invalid or duplicated")
    seen.add(value)
    return value


def sha256_bytes(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def json_digest(value: object) -> str:
    return sha256_bytes(json.dumps(value, sort_keys=True, separators=(",", ":")).encode())


def load_evidence(original: bytes, rules: Mapping[str, Any], source_root: Path = PROJECT) -> dict[str, Any]:
    """Replay hash-pinned research inputs, never user-supplied proof-state flags.

    Consumer bodies are the catalogue-bound original instructions, not mutable C
    prose. Retained base traces additionally bind exact call/delay-slot words.
    Historical Yay0 reports remain research lineage, not current build receipts.
    """
    raw = (source_root / EVIDENCE_PATH).read_bytes()
    if sha256_bytes(raw) != EVIDENCE_SHA256:
        raise MapError("Research evidence policy identity changed")
    policy = json.loads(raw)
    if rules != policy["rules"]:
        raise MapError("Region rules differ from reviewed loader/owner evidence")
    result = {}
    for name, digest in policy["inputs"].items():
        data = (source_root / "config/rom-evidence" / name).read_bytes()
        if sha256_bytes(data) != digest:
            raise MapError(f"Research evidence identity changed: {name}")
        result[name] = json.loads(data)
    for row in policy["consumers"]:
        start, end = row["rom_start"], row["rom_start"] + row["size"]
        if sha256_bytes(original[start:end]) != row["sha256"]:
            raise MapError(f"Loader consumer instruction bytes changed: {row['symbol']}")
    traces = result["consumer-traces.json"]
    for row in traces["retained_bases"] + traces["rejected_bases"]:
        for interval in row["instruction_evidence"]:
            _bound_slice(original, interval, "Loader instruction")
            for line in interval["words"]:
                fields = line.split()
                site, word = int(fields[1], 16), int(fields[4], 16)
                if struct.unpack_from(">I", original, site)[0] != word:
                    raise MapError("Loader instruction word changed")
    for row in result["audio-intervals.json"]["intervals"]:
        _bound_slice(original, row, "Audio interval")
    for binding in result["window-bindings.json"]["windows"].values():
        start, end = (int(binding["window"][k], 16) for k in ("start", "end"))
        if sha256_bytes(original[start:start + 16]) != binding["header16_sha256"]:
            raise MapError("Owner window header changed")
        for owner in binding["table_owners"]:
            site = int(owner["table_base"], 16) + owner["record"] * owner["stride"] + owner["field_offset"]
            if (site != int(owner["ref_rom"], 16)
                    or struct.unpack_from(">II", original, site) != (start, end)
                    or sha256_bytes(original[site:site + 8]) != owner["pair_sha256"]):
                raise MapError("Owner pair/alias evidence changed")
        capacity = binding.get("capacity_evidence")
        if capacity:
            site = int(capacity["source_rom"], 16)
            if (struct.unpack_from(">I", original, site)[0] != capacity["value"]
                    or sha256_bytes(original[site:site + 4]) != capacity["sha256"]
                    or end - start != capacity["value"]):
                raise MapError("Demo capacity is not a logical payload extent")
    return result


def _bound_slice(original: bytes, row: Mapping[str, Any], label: str) -> tuple[int, int]:
    start, end = (int(row[k], 16) for k in ("start", "end"))
    if not 0 <= start < end <= len(original) or end - start != row["size"]:
        raise MapError(f"{label} bounds changed")
    data = original[start:end]
    if sha256_bytes(data) != row["sha256"] or hashlib.md5(data).hexdigest() != row["md5"]:
        raise MapError(f"{label} hash/MD5 does not bind its exact extent")
    return start, end


def _sprite_palette_ranges(original: bytes, evidence: Mapping[str, Any]) -> set[tuple[int, int]]:
    """Bind static palette references; neither runtime use nor allocation is proved."""
    for consumer in evidence["consumers"]:
        _bound_slice(original, consumer, "Palette consumer instruction")
    metadata = immediate_at(original, 0x44134)
    bases = {5: immediate_at(original, 0x454BC), 6: immediate_at(original, 0x454AC)}
    count = struct.unpack_from(">I", original, 0x44254)[0] & 0xFFFF
    stride = struct.unpack_from(">I", original, 0x44144)[0] & 0xFFFF
    length = struct.unpack_from(">I", original, 0x45124)[0] & 0xFFFF
    descriptor_bounds = _bound_slice(original, evidence["descriptor_table"], "Palette descriptor table")
    if (stride != 16 or length != 32 or metadata != bases[5]
            or descriptor_bounds != (metadata, metadata + count * stride)):
        raise MapError("Palette descriptor geometry differs from original consumers")
    ranges = set()
    previous_end = 0
    for interval in evidence["whole_intervals"]:
        start, end = _bound_slice(original, interval, "Whole palette interval")
        if start < previous_end:
            raise MapError("Whole palette intervals overlap or repeat")
        cursor = start
        for payload in interval["payloads"]:
            low, high = _bound_slice(original, payload, "Palette payload")
            if low != cursor or high - low != length or not payload["references"]:
                raise MapError("Palette references must cover the whole interval without gaps")
            for ref in payload["references"]:
                descriptor = _int(ref["descriptor"], "Palette descriptor")
                member = _int(ref["member"], "Palette member")
                if descriptor >= count:
                    raise MapError("Palette descriptor exceeds the original index bound")
                site = metadata + descriptor * stride
                flags = original[site]
                members = flags & 31
                if not members or member >= members:
                    raise MapError("Palette reference requires a positive count and bounded member")
                table = metadata + (struct.unpack_from(">I", original, site + 4)[0] & 0xFFFFFF)
                pointer_site = table + member * 4
                if not metadata <= table < table + members * 4 <= bases[6]:
                    raise MapError("Palette pointer table escapes the metadata region")
                pointer_bounds = _bound_slice(original, ref["pointer_word"], "Palette pointer word")
                if pointer_bounds != (pointer_site, pointer_site + 4):
                    raise MapError("Palette pointer is not the selected descriptor member")
                pointer = struct.unpack_from(">I", original, pointer_site)[0]
                base = bases.get((pointer >> 24) & 15)
                if base is None or base + (pointer & 0xFFFFFF) != low:
                    raise MapError("Palette pointer does not address its complete payload")
                if ref["tlut_mode"] != ("IA16" if flags & 0x20 else "RGBA16"):
                    raise MapError("Palette TLUT mode differs from the original descriptor")
            cursor = high
        if cursor != end:
            raise MapError("Palette references do not cover the whole interval")
        ranges.add((start, end))
        previous_end = end
    return ranges


def efat_structure(original: bytes, rules: Mapping[str, Any], evidence: Mapping[str, Any]) -> tuple[set[tuple[int, int]], set[tuple[int, int]], list[tuple[int, int]]]:
    """Directory size equals FORM+4; stored extent adds eight, then even FF alignment."""
    start, base = _bound_slice(original, evidence["directory"], "EFAT directory")
    count = struct.unpack_from(">I", original, start + 4)[0]
    if (original[start:start + 4] != b"EFAT" or count != evidence["record_count"]
            or base != start + 8 + count * 8 or base != rules["iff_archive"]["first_form"]
            or start != immediate_at(original, rules["iff_archive"]["site"])):
        raise MapError("EFAT directory geometry changed")
    headers, gaps, forms = {(start, base)}, set(), []
    expected = {row["index"]: row for row in evidence["forms"]}
    empty = {row["index"] for row in evidence["empty_entries"]}
    cursor = base
    for index in range(count):
        site = start + 8 + index * 8
        offset, size = struct.unpack_from(">II", original, site)
        if index in empty:
            if offset or size:
                raise MapError("EFAT empty entry changed")
            continue
        row = expected.get(index)
        if row is None:
            raise MapError("EFAT unbound FORM")
        a, b = _bound_slice(original, row, "EFAT FORM")
        if (a != base + offset or b - a != size + 8 or size < 4
                or original[a:a + 4] != b"FORM" or struct.unpack_from(">I", original, a + 4)[0] != size
                or original[a + 8:a + 12].hex() != row["form_type_hex"]
                or sha256_bytes(original[site:site + 8]) != row["directory_sha256"]
                or sha256_bytes(original[a:a + 12]) != row["header_sha256"]):
            raise MapError("EFAT FORM length/header disagrees with directory")
        if a != cursor:
            if a != cursor + 1 or not cursor & 1 or a & 1 or original[cursor:a] != b"\xff":
                raise MapError("EFAT alignment gap is not structural FF")
            gaps.add((cursor, a))
        headers.add((a, a + 12))
        forms.append((a, b))
        cursor = b
    recorded = {_bound_slice(original, row, "EFAT FF gap") for row in evidence["ff_padding"]}
    if gaps != recorded or cursor != rules["content_end"]:
        raise MapError("EFAT alignment gaps/final extent changed")
    return headers, gaps, forms


CPU_PADDING_INPUT = "cpu-padding.json"
CPU_PADDING_DIR = "config/rom-evidence/cpu-padding"
RESIDUAL_INPUT = "unexplained-residuals.json"
RULINGS_DIR = "config/rom_rulings"
# User-ruled unexplained residuals: expected prior class -> residual class. The bytes are retained verbatim
# with no format, consumer, ownership, padding or code claim and no credit; a later witness supersedes them.
RESIDUAL_CLASSES: Mapping[str, str] = {"cpu_text_unowned": "cpu_text_unexplained",
                                       "asset_raw_unattributed": "asset_raw_unexplained"}
_RULED_LITERAL = re.compile(r"0x[0-9A-F]+-0x[0-9A-F]+")
CPU_DATA_PADDING_INPUT = "cpu-data-padding.json"
CPU_DATA_PADDING_DIR = "config/rom-evidence/cpu-data-padding"
CPU_DATA_PADDING_RULE = "pad_data_alignment"
_DATA_SECTIONS = frozenset({".data", ".rodata"})
CPU_PADDING_RULES = frozenset({"pad_object_rounding", "pad_sdk_section", "pad_composite"})
# Witness tiers; removing a tier reopens its ranges on the next regeneration. The witness_assembler
# tiers rest on the witness-only rtems variants of the pinned gas 2.9.1 and binutils 2.37 sources;
# cross-profile joints are ruled separately (see cpu-padding.json "tiers"); pending-peer joints are
# withheld until their peer candidates are adopted (cpu-padding.json "withheld").
CPU_PADDING_TIERS = frozenset({"canonical_reproduction", "original_sdk_object", "alternate_profile_reproduction",
                               "witness_assembler_reproduction", "witness_assembler_joint_reproduction",
                               "witness_assembler_cross_profile_joint"})
_REL_FIELD = {2: 0xFFFFFFFF, 4: 0x03FFFFFF, 5: 0x0000FFFF, 6: 0x0000FFFF}  # R_MIPS_32, _26, _HI16, _LO16
_SDK_MIN_CODE = 16  # selectivity floor when unreproduced original-object identity masks relocation fields


def _elf_text(data: bytes, label: str) -> dict[str, Any]:
    """The .text bytes, per-word relocation field masks and .text symbols of one big-endian ELF32 REL object."""
    if (len(data) < 0x34 or data[:4] != b"\x7fELF" or data[4] != 1 or data[5] != 2
            or struct.unpack_from(">H", data, 16)[0] != 1 or struct.unpack_from(">H", data, 18)[0] != 8):
        raise MapError(f"{label} is not a big-endian MIPS ELF32 relocatable object")
    shoff = struct.unpack_from(">I", data, 0x20)[0]
    entsize, count, names = struct.unpack_from(">HHH", data, 0x2E)
    sections = [struct.unpack_from(">IIIIIIIIII", data, shoff + i * entsize) for i in range(count)]
    base = sections[names][4]

    def name(row: tuple[int, ...]) -> bytes:
        return data[base + row[0]:data.index(b"\0", base + row[0])]

    texts = [i for i, row in enumerate(sections) if name(row) == b".text"]
    if len(texts) != 1 or sections[texts[0]][1] != 1:
        raise MapError(f"{label} must have exactly one PROGBITS .text")
    index = texts[0]
    _, _, _, _, offset, size, _, _, align, _ = sections[index]
    body = data[offset:offset + size]
    if size % 4 or align < 16 or align & (align - 1):
        raise MapError(f"{label} .text size/alignment is not a word-sized power-of-two section")
    masks, symbols, referenced = [0] * (size // 4), [], set()
    for row in sections:
        if row[1] in (4, 9):
            width = 12 if row[1] == 4 else 8
            for k in range(row[5] // width):
                where, info = struct.unpack_from(">II", data, row[4] + k * width)
                referenced.add(info >> 8)
                if row[7] != index:
                    continue
                if where % 4 or where >= size or info & 0xFF not in _REL_FIELD:
                    raise MapError(f"{label} has an unsupported .text relocation")
                masks[where // 4] |= _REL_FIELD[info & 0xFF]
        elif row[1] == 2:
            for k in range(row[5] // 16):
                _, value, length, info, _, shndx = struct.unpack_from(">IIIBBH", data, row[4] + 16 * k)
                if shndx == index and info & 15 != 3:
                    symbols.append((value, length, info & 15, info >> 4, k))
    return {"body": body, "align": align, "masks": masks, "symbols": symbols, "referenced": referenced}


def _pinned_object(source_root: Path, pin: Mapping[str, Any], label: str) -> dict[str, Any]:
    path = source_root / CPU_PADDING_DIR / f"{pin['sha256']}.o"
    if pin["path"] != f"{CPU_PADDING_DIR}/{pin['sha256']}.o":
        raise MapError(f"{label} witness object path is not content-addressed")
    data = path.read_bytes()
    if sha256_bytes(data) != pin["sha256"]:
        raise MapError(f"{label} witness object identity changed")
    return _elf_text(data, label)


def cpu_padding(original: bytes, catalogue: Mapping[str, Any], evidence: Mapping[str, Any],
                source_root: Path = PROJECT) -> dict[tuple[int, int], str]:
    """Replay every pinned CPU padding witness against the original; {(start, end): rule}.

    A witness is an object whose .text ends exactly where the zero interval ends:
    its last code byte is a catalogued function end, its section size is that end
    rounded up to the section alignment, the rounding bytes are zero with no
    relocation and no symbol in them (only an unreferenced local end-of-function
    label may sit exactly at the code end), and every non-relocation bit of its code equals the
    original. A composite interval may add only minimal linker alignment fill before
    an aligned object witnessed by another record. Zero bytes, geometry or an
    equally sized all-zero object never qualify; nothing here grants C/C++ credit.
    """
    document = evidence[CPU_PADDING_INPUT]
    if (document.get("schema") != "shiren2-cpu-padding-evidence-v1" or document.get("target_sha256") != EXPECTED_SHA256
            or not isinstance(document.get("ranges"), list)):
        raise MapError("CPU padding evidence does not describe the fixed target")
    images = {row["image_id"]: row for row in catalogue["images"]}
    starts: dict[str, dict[int, int]] = {}
    ends: dict[str, set[int]] = {}
    for f in catalogue["functions"]:
        starts.setdefault(f["image_id"], {})[f["rom_start"]] = f["rom_start"] + f["size"]
        ends.setdefault(f["image_id"], set()).add(f["rom_start"] + f["size"])
    result: dict[tuple[int, int], str] = {}
    verified: set[tuple[str, int]] = set()
    fills: list[tuple[str, int, int, int]] = []
    for record in document["ranges"]:
        start, end, image_id = _int(record["start"], "start"), _int(record["end"], "end"), record["image_id"]
        label = f"CPU padding {start:#x}..{end:#x}"
        image = images.get(image_id)
        parts = record["parts"]
        if (image is None or record["tier"] not in CPU_PADDING_TIERS or record["rule"] not in CPU_PADDING_RULES
                or not isinstance(parts, list) or not parts or (start, end) in result
                or record["rule"] != ("pad_composite" if len(parts) > 1 else
                                      {"object_rounding": "pad_object_rounding", "sdk_section": "pad_sdk_section"}.get(parts[0]["kind"]))):
            raise MapError(f"{label} has an unsupported rule, tier or image")
        if not start < end or start not in ends.get(image_id, set()) or not _zero(original, start, end):
            raise MapError(f"{label} is not a zero interval after a catalogued function end")
        cpu_end = next((r["rom_end"] for r in image["provisional_cpu_ranges"] if r["rom_start"] <= start < r["rom_end"]), None)
        if end not in starts[image_id] and end != cpu_end:
            raise MapError(f"{label} does not end at the next function or the reviewed CPU range end")
        if any(start < a < end for a in starts[image_id]):
            raise MapError(f"{label} overlaps a catalogued function")
        vram = image["vram_start"] - image["rom_start"]
        cursor = start
        for part in parts:
            a, b, kind = _int(part["start"], "part start"), _int(part["end"], "part end"), part["kind"]
            if a != cursor or not a < b <= end:
                raise MapError(f"{label} parts do not tile the interval")
            cursor = b
            if kind in ("object_rounding", "sdk_section"):
                obj = _pinned_object(source_root, part["object"], label)
                base = _int(part["object_rom_start"], "object_rom_start")
                code = a - base
                body, align, masks = obj["body"], obj["align"], obj["masks"]
                if (base + len(body) != b or code <= 0 or code % 4 or len(body) != -(-code // align) * align
                        or (base + vram) % align or any(body[code:]) or any(masks[code // 4:])
                        or any(value > code or (value == code and (typ or bind or number in obj["referenced"]))
                               for value, _, typ, bind, number in obj["symbols"])):
                    raise MapError(f"{label} object section does not end in its own rounding")
                cover, function = base, None
                while cover < a:
                    function = starts[image_id].get(cover)
                    if function is None:
                        raise MapError(f"{label} object code is not tiled by catalogued functions")
                    cover = function
                if cover != a:
                    raise MapError(f"{label} object code does not end at the interval start")
                for i in range(code // 4):
                    word, rom = struct.unpack_from(">I", body, 4 * i)[0], struct.unpack_from(">I", original, base + 4 * i)[0]
                    if (word ^ rom) & ~masks[i] & 0xFFFFFFFF or (part["identity"] == "raw" and (masks[i] or word != rom)):
                        raise MapError(f"{label} object code differs from the original")
                if part["identity"] not in ("raw", "nonreloc"):
                    raise MapError(f"{label} identity mode is unsupported")
                if kind == "object_rounding":
                    sized = [value + length for value, length, typ, _, _ in obj["symbols"] if typ == 2 and length]
                    if not sized or max(sized) != code:
                        raise MapError(f"{label} last sized function does not end at the interval start")
                elif part["identity"] == "nonreloc" and code < _SDK_MIN_CODE:
                    raise MapError(f"{label} original object code is below the identity floor")
                verified.add((part["object"]["sha256"], base))
            elif kind == "alignment_fill":
                align = _int(part["align"], "align")
                nxt = part["next_object"]
                if (b != end or align < 16 or align & (align - 1) or (b + vram) % align or not (a + vram) % align
                        or b - a >= align or _int(nxt["object_rom_start"], "next object") != b):
                    raise MapError(f"{label} is not minimal fill before an aligned input")
                fills.append((nxt["sha256"], b, align, start))
            else:
                raise MapError(f"{label} part kind is unsupported")
        if cursor != end:
            raise MapError(f"{label} parts do not reach the interval end")
        result[(start, end)] = record["rule"]
    for digest, base, align, start in fills:
        if (digest, base) not in verified:
            raise MapError(f"CPU padding {start:#x}: the aligned next object is not itself witnessed")
        pin = next(p["object"] for r in document["ranges"] for p in r["parts"]
                   if p.get("object", {}).get("sha256") == digest and p.get("object_rom_start") == base)
        if _pinned_object(source_root, pin, f"CPU padding {start:#x}")["align"] != align:
            raise MapError(f"CPU padding {start:#x}: fill alignment differs from the next object")
    return result


# ---------------------------------------------------------------------------- rules


@dataclass(frozen=True)
class Window:
    """A tail ROM window owned by CPU-side evidence."""
    start: int
    end: int
    ref: str
    explicit: bool  # both bounds come from original table words or code immediates


def immediate_at(original: bytes, site: int) -> int:
    """Value of `addiu/ori rX, rY, lo` at `site` combined with the nearest preceding `lui rY`."""
    if site % 4 or not 0 < site <= len(original) - 4:
        raise MapError("code immediate site is outside the ROM")
    use = struct.unpack_from(">I", original, site)[0]
    op, base, low = use >> 26, (use >> 21) & 31, use & 0xFFFF
    if op not in (0x09, 0x0D):
        raise MapError(f"code site {site:#x} is not addiu/ori")
    for back in range(site - 4, max(site - 36, -4), -4):
        word = struct.unpack_from(">I", original, back)[0]
        if word >> 26 == 0x0F and (word >> 16) & 31 == base:
            signed = low - 0x10000 if op == 0x09 and low & 0x8000 else low
            return ((word & 0xFFFF) << 16) + signed & 0xFFFFFFFF
        if (word >> 16) & 31 == base and word >> 26 not in (0x28, 0x29, 0x2B) and word != 0:
            break
    raise MapError(f"code site {site:#x} has no reaching lui")


def instruction_immediate(original: bytes, site: int, opcode: int, base: int | None = None) -> int:
    """Unsigned 16-bit immediate of the I-type word at `site`, after checking its opcode (and base)."""
    if site % 4 or not 0 < site <= len(original) - 4:
        raise MapError("code immediate site is outside the ROM")
    word = struct.unpack_from(">I", original, site)[0]
    if word >> 26 != opcode or (base is not None and (word >> 21) & 31 != base):
        raise MapError(f"code site {site:#x} is not the expected instruction")
    return word & 0xFFFF


def load_rules(document: object) -> dict[str, Any]:
    rules = _record(document, "Region rules", {
        "schema_version", "kind", "target_sha256", "target_size", "ipl3_md5", "tail_start", "content_end",
        "tables", "chains", "immediate_windows", "inferred_windows", "code_bases", "sprite_palettes", "iff_archive"})
    if (rules["schema_version"] != 1 or rules["kind"] != RULES_KIND or rules["target_sha256"] != EXPECTED_SHA256
            or rules["target_size"] != EXPECTED_SIZE):
        raise MapError("Region rules do not describe the fixed target")
    if not re.fullmatch(r"[0-9a-f]{32}", str(rules["ipl3_md5"])):
        raise MapError("IPL3 identity must be an md5")
    tail, end = _int(rules["tail_start"], "tail_start"), _int(rules["content_end"], "content_end")
    if not tail < end <= EXPECTED_SIZE:
        raise MapError("Invalid tail/content interval")
    seen: set[str] = set()
    for row in rules["tables"]:
        _record(row, "Table rule", {"id", "rom_start", "rom_end", "record_size", "pair_offsets", "note"})
        _identity(row["id"], "Table", seen)
        start, stop, size = (_int(row[k], k) for k in ("rom_start", "rom_end", "record_size"))
        offsets = row["pair_offsets"]
        if (not start < stop <= tail or size < 8 or size % 4 or (stop - start) % size or not offsets
                or any(type(o) is not int or o % 4 or not 0 <= o <= size - 8 for o in offsets)):
            raise MapError(f"Invalid table geometry: {row['id']}")
        _text(row["note"], "Table note")
    for row in rules["chains"]:
        _record(row, "Chain rule", {"id", "rom_start", "rom_end", "stride", "note"})
        _identity(row["id"], "Chain", seen)
        if (_int(row["rom_start"], "rom_start") % 4 or (_int(row["rom_end"], "rom_end") - row["rom_start"]) % 4
                or not row["rom_start"] < row["rom_end"] <= tail or _int(row["stride"], "stride") == 0):
            raise MapError(f"Invalid chain geometry: {row['id']}")
        _text(row["note"], "Chain note")
    for row in rules["immediate_windows"]:
        _record(row, "Immediate window", {"id", "start_site", "end_site", "note"})
        _identity(row["id"], "Immediate window", seen)
        _int(row["start_site"], "start_site"), _int(row["end_site"], "end_site"), _text(row["note"], "note")
    for row in rules["inferred_windows"]:
        _record(row, "Inferred window", {"id", "start_site", "end_rom", "end_basis", "note"})
        _identity(row["id"], "Inferred window", seen)
        _int(row["start_site"], "start_site"), _int(row["end_rom"], "end_rom")
        _text(row["end_basis"], "end_basis"), _text(row["note"], "note")
    for row in rules["code_bases"]:
        _record(row, "Code base", {"id", "site", "note"})
        _identity(row["id"], "Code base", seen)
        _int(row["site"], "site")
        _text(row["note"], "Code base consumer evidence")
    for row in rules["sprite_palettes"]:
        _record(row, "Sprite palette rule", {"id", "directory_site", "segment_site", "count_site", "size_site",
                                             "record_size", "note"})
        _identity(row["id"], "Sprite palette rule", seen)
        for key in ("directory_site", "segment_site", "count_site", "size_site"):
            _int(row[key], key)
        if _int(row["record_size"], "record_size") != 16:
            raise MapError(f"Invalid sprite directory geometry: {row['id']}")
        _text(row["note"], "Sprite palette consumer evidence")
    iff = _record(rules["iff_archive"], "IFF archive", {"id", "site", "first_form", "note"})
    _identity(iff["id"], "IFF archive", seen)
    _int(iff["site"], "site"), _int(iff["first_form"], "first_form"), _text(iff["note"], "note")
    return rules


def rule_windows(rules: Mapping[str, Any], original: bytes, source_root: Path = PROJECT) -> list[Window]:
    """Every owner window, re-derived from original table words and code immediates."""
    tail, end = rules["tail_start"], rules["content_end"]
    load_evidence(original, rules, source_root)
    windows: dict[tuple[int, int], Window] = {}

    def add(start: int, stop: int, ref: str, explicit: bool) -> None:
        if not tail <= start < stop <= end:
            raise MapError(f"{ref}: window {start:#x}-{stop:#x} is outside the stored tail")
        windows.setdefault((start, stop), Window(start, stop, ref, explicit))

    for row in rules["tables"]:
        for base in range(row["rom_start"], row["rom_end"], row["record_size"]):
            for offset in row["pair_offsets"]:
                pair = struct.unpack_from(">II", original, base + offset)
                if pair == (0, 0):
                    continue
                add(*pair, row["id"].split("_alias")[0], True)
    for row in rules["chains"]:
        for site in range(row["rom_start"], row["rom_end"], 4):
            start = struct.unpack_from(">I", original, site)[0]
            capacity = struct.unpack_from(">I", original, 0x12722C)[0]
            if capacity != row["stride"]:
                raise MapError("Demo consumer capacity changed")
            add(start, start + capacity, row["id"], False)
    for row in rules["immediate_windows"]:
        add(immediate_at(original, row["start_site"]), immediate_at(original, row["end_site"]), row["id"], True)
    starts = {w.start for w in windows.values()}
    for row in rules["inferred_windows"]:
        add(immediate_at(original, row["start_site"]), row["end_rom"], row["id"], False)
    for row in rules["inferred_windows"]:
        # The inferred end must be some other window start or a Yay0 stream (checked by the generator).
        if row["end_rom"] not in starts | {w.start for w in windows.values()} and original[row["end_rom"]:row["end_rom"] + 4] != b"Yay0":
            raise MapError(f"{row['id']}: inferred end is not a referenced start or stream")
    for row in rules["sprite_palettes"]:
        # Directory base, descriptor count (slti), segment-6 base and the 32-byte TLUT read are all
        # original instruction operands; every palette word is an original table word.
        directory = immediate_at(original, row["directory_site"])
        segment = immediate_at(original, row["segment_site"])
        count = instruction_immediate(original, row["count_site"], 0x0A)
        size = instruction_immediate(original, row["size_site"], 0x09, base=0)
        for record in range(directory, directory + count * row["record_size"], row["record_size"]):
            members = original[record] & 31
            if not members:
                continue
            pointer = struct.unpack_from(">I", original, record + 4)[0]
            if (pointer >> 24) & 15 != 5:
                raise MapError(f"{row['id']}: palette table pointer at {record + 4:#x} is not segment 5")
            table = directory + (pointer & 0xFFFFFF)
            for site in range(table, table + 4 * members, 4):
                word = struct.unpack_from(">I", original, site)[0]
                if (word >> 24) & 15 != 6:
                    raise MapError(f"{row['id']}: palette word at {site:#x} is not segment 6")
                start = segment + (word & 0xFFFFFF)
                add(start, start + size, row["id"], True)
    ordered = sorted(windows.values(), key=lambda w: (w.start, -w.end))
    for index, first in enumerate(ordered):  # windows nest or are disjoint; partial overlap is ambiguous
        for second in ordered[index + 1:]:
            if second.start >= first.end:
                break
            if second.end > first.end:
                raise MapError(f"Partially overlapping owner windows {first.ref}/{second.ref}")
    return ordered


# ---------------------------------------------------------------------------- Yay0


def tail_streams(original: bytes, start: int, end: int,
                 evidence: list[dict[str, Any]]) -> list[tuple[int, int, int]]:
    """Replay retained exact stream identities/header bounds; perform zero decodes."""
    streams = []
    cursor = start
    for row in evidence:
        offset, stop = row["offset"], row["end"]
        if not cursor <= offset < stop <= end or stop - offset < 16:
            raise MapError("Inherited Yay0 stream bounds overlap or escape the tail")
        size, link, chunk = struct.unpack_from(">III", original, offset + 4)
        if (original[offset:offset + 4] != b"Yay0" or not 0 < size <= 4 << 20
                or not 16 <= link <= chunk <= stop - offset
                or sha256_bytes(original[offset:stop]) != row["stream_sha256"]
                or row["crunch64_exact"] is not True or not _SHA.fullmatch(row["decoded_sha256"])):
            raise MapError("Inherited Yay0 stream identity/header disagrees")
        streams.append((offset, stop, size))
        cursor = stop
    return streams


# ---------------------------------------------------------------------------- generation


def _zero(original: bytes, start: int, end: int) -> bool:
    return not original[start:end].strip(b"\0")


def _ruled_scope(text: str) -> set[str]:
    """Range literals named in a ruling's Scope section, excluding its "Out of scope" paragraphs."""
    if "\n## Scope\n" not in text:
        raise MapError("Ruling copy has no Scope section")
    scope = text.split("\n## Scope\n", 1)[1].split("\n## ", 1)[0]
    return {literal for paragraph in scope.split("\n\n") if not paragraph.lstrip().startswith("Out of scope")
            for literal in _RULED_LITERAL.findall(paragraph)}


def unexplained_residuals(original: bytes, evidence: Mapping[str, Any],
                          source_root: Path = PROJECT) -> dict[tuple[int, int], tuple[str, str, str, str]]:
    """User-ruled unexplained residuals: {(start, end): (prior_class, prior_ref, class, id)}.

    Each record binds exact original bytes, its expected prior class/ref, its precise missing witness and a
    byte-pinned copy of the ruling under config/rom_rulings/. Every in-scope range literal of every pinned
    ruling must have exactly one record and every record must be in scope. Nothing here grants credit.
    """
    document = evidence[RESIDUAL_INPUT]
    if (document.get("schema") != "shiren2-unexplained-residuals-v1" or document.get("target_sha256") != EXPECTED_SHA256
            or document.get("status") != "closed_unexplained_residual"
            or not isinstance(document.get("rulings"), dict) or not isinstance(document.get("ranges"), list)):
        raise MapError("Unexplained residual evidence does not describe the fixed target")
    scopes: dict[str, set[str]] = {}
    for name, pin in document["rulings"].items():
        path = pin.get("path", "")
        data = None
        if path == f"{RULINGS_DIR}/{name}.md":
            try:
                data = (source_root / path).read_bytes()
            except OSError:
                data = None
        if data is None or sha256_bytes(data) != pin.get("sha256"):
            raise MapError(f"Ruling copy identity changed: {name}")
        # Parse the scope from the same verified buffer; never reopen the copy.
        scopes[name] = _ruled_scope(data.decode("utf-8"))
    result: dict[tuple[int, int], tuple[str, str, str, str]] = {}
    named: set[tuple[str, str]] = set()
    for record in document["ranges"]:
        label = f"Unexplained residual {record.get('id')!r}"
        start, end = _bound_slice(original, record, label)
        literal = f"{start:#X}-{end:#X}".replace("0X", "0x")
        if (record.get("ruling") not in scopes or literal not in scopes[record["ruling"]]
                or (record["ruling"], literal) in named):
            raise MapError(f"{label} is not a range in its ruling's scope")
        if (record.get("class") != RESIDUAL_CLASSES.get(record.get("prior_class"))
                or record.get("prior_ref") not in ("catalogue", "unattributed")
                or not isinstance(record.get("missing_witness"), str) or not record["missing_witness"].strip()
                or record.get("id") != f"residual_{start:08x}_{end:08x}"):
            raise MapError(f"{label} has an unsupported class, prior class, id or missing witness")
        if any(a < end and start < b for a, b in result):
            raise MapError(f"{label} overlaps another residual")
        named.add((record["ruling"], literal))
        result[(start, end)] = (record["prior_class"], record["prior_ref"], record["class"], record["id"])
    if named != {(name, literal) for name, literals in scopes.items() for literal in literals}:
        raise MapError("A ruled range has no unexplained-residual record")
    return result


def _cpu_text_pieces(start: int, end: int, functions: list[tuple[int, int]],
                     padding: Mapping[tuple[int, int], str] | None = None) -> Iterable[tuple[int, int, str, str]]:
    """Partition a CPU range without mistaking uncovered bytes for proved padding.

    Only an exact interval replayed by `cpu_padding` becomes `cpu_text_padding`.
    Zero content alone is not ownership evidence; every other gap and tail stays open
    unless `generate` closes that exact piece as a ruled unexplained residual.
    """
    padding = padding or {}

    def uncovered(a: int, b: int) -> tuple[int, int, str, str]:
        rule = padding.get((a, b))
        return (a, b, "cpu_text_padding", rule) if rule else (a, b, "cpu_text_unowned", "catalogue")

    cursor = start
    for f_start, f_end in sorted(functions):
        if not start <= f_start < f_end <= end:
            continue
        if f_start > cursor:
            yield uncovered(cursor, f_start)
        yield f_start, f_end, "cpu_text_function", "catalogue"
        cursor = f_end
    if cursor < end:
        yield uncovered(cursor, end)

def _rsp_components(original: bytes, inventory: ImageInventory, source_root: Path) -> dict[str, tuple[int, int, str, str]]:
    """Audio closes only after external ELF identity and stored end-symbol replay."""
    try:
        proof = rspaudio.verify(original, source_root)
    except (ValueError, OSError) as error:
        raise MapError(str(error)) from error
    audio = {row["component_id"]: row for row in proof["sections"]}
    if {component.component_id for component in inventory.components} != set(_RSP_CLASS) | set(audio):
        raise MapError("Current ROM-map classification requires the complete reviewed RSP components")
    result = {}
    for component in inventory.components:
        name = component.component_id
        if name in audio:
            row = audio[name]
            if (component.rom_start, component.rom_end, component.rom_sha256) != (
                    row["rom_start"], row["rom_end"], row["sha256"]):
                raise MapError(f"Audio component differs from parsed SDK section: {name}")
            cls = row["class"]
        elif name in _RSP_CLASS:
            cls = _RSP_CLASS[name]
        else:
            raise MapError(f"Unclassified RSP component: {name}")
        result[name] = (component.rom_start, component.rom_end, cls, name)
    return result



def generate(original: bytes, inventory: ImageInventory, catalogue: Mapping[str, Any],
             rules: Mapping[str, Any], *, catalogue_id: str, rules_sha256: str,
             source_root: Path = PROJECT) -> dict[str, Any]:
    return _generate(original, inventory, catalogue, rules, catalogue_id=catalogue_id,
                     rules_sha256=rules_sha256, source_root=source_root)[0]


def _generate(original: bytes, inventory: ImageInventory, catalogue: Mapping[str, Any],
              rules: Mapping[str, Any], *, catalogue_id: str, rules_sha256: str,
              source_root: Path = PROJECT) -> tuple[dict[str, Any], dict[tuple[int, int], str]]:
    """The map document and the located rows closed by replayed asset format witnesses."""
    attest(original)
    rules = load_rules(rules)
    rsp_components = _rsp_components(original, inventory, source_root)
    rows: list[list[Any]] = []
    evidence = load_evidence(original, rules, source_root)
    residuals = unexplained_residuals(original, evidence, source_root)
    closed: set[tuple[int, int]] = set()

    def add(start: int, end: int, cls: str, ref: str) -> None:
        if end <= start:
            raise MapError(f"Empty range at {start:#x}")
        # Catalogue completeness is separate from the ownership of zero gaps.
        if cls in ("cpu_text_unowned", "cpu_text_padding") and not _zero(original, start, end):
            raise MapError(f"Nonzero uncatalogued CPU bytes at {start:#x}..{end:#x}")
        ruled = residuals.get((start, end))
        if ruled is not None:
            if (cls, ref) != ruled[:2]:
                raise MapError(f"Ruled residual {ruled[3]} is {cls}/{ref}, not its expected prior class {ruled[0]}")
            cls, ref = ruled[2], ruled[3]
            closed.add((start, end))
        rows.append([start, end, cls, ref])

    if hashlib.md5(original[0x40:0x1000]).hexdigest() != rules["ipl3_md5"]:
        raise MapError("IPL3 identity changed")
    add(0, 0x40, "rom_header", "boot")
    add(0x40, 0x1000, "ipl3_bootcode", "boot")
    images = sorted(inventory.images.values(), key=lambda i: i.rom_start)
    if images[0].rom_start != 0x1000 or images[-1].rom_end != rules["tail_start"]:
        raise MapError("Images must span the boot end to the stored tail")
    cpu_ranges = {image["image_id"]: [(r["rom_start"], r["rom_end"]) for r in image["provisional_cpu_ranges"]]
                  for image in catalogue["images"]}
    functions: dict[str, list[tuple[int, int]]] = {}
    for f in catalogue["functions"]:
        functions.setdefault(f["image_id"], []).append((f["rom_start"], f["rom_start"] + f["size"]))
    padding = cpu_padding(original, catalogue, evidence, source_root)
    for image in images:
        position = image.rom_start
        pieces: list[tuple[int, int, str, str]] = []
        for start, end in cpu_ranges.get(image.image_id, []):
            pieces.extend(_cpu_text_pieces(start, end, functions.get(image.image_id, []), padding))
        for component in inventory.components:
            if component.parent_image_id == image.image_id:
                pieces.append(rsp_components[component.component_id])
        split = bool(cpu_ranges.get(image.image_id))
        for start, end, cls, ref in sorted(pieces):
            if start < position:
                raise MapError(f"Overlapping image pieces at {start:#x}")
            if start > position:
                gap_class = "cpu_data" if split else "image_opaque"
                add(position, start, gap_class, image.image_id)
            add(start, end, cls, ref)
            position = end
        if position < image.rom_end:
            add(position, image.rom_end, "cpu_data" if split else "image_opaque", image.image_id)
    if {(row[0], row[1]) for row in rows if row[2] == "cpu_text_padding"} != set(padding):
        raise MapError("CPU padding evidence names an interval that is not an uncovered CPU gap")
    _generate_tail(original, rules, add, source_root)
    if closed != set(residuals):
        missing = ", ".join(residuals[key][3] for key in sorted(set(residuals) - closed))
        raise MapError(f"Ruled residual range is missing or shifted: {missing}")
    rows.sort(key=lambda row: row[0])
    base_rows = rows
    replayed = cpu_vtables(original, catalogue, evidence, base_rows)
    vtables = [(start, end, gxxvtable.CLASS, table, table) for (start, end), table in sorted(replayed.items())]
    rows, splits = split_rows(base_rows, witness_subranges(evidence[assetformat.INPUT]) + vtables
                              + cpu_data_padding(original, catalogue, evidence, base_rows, replayed, source_root))
    witnessed = format_witnesses(original, catalogue, rows, rules, source_root, base_rows)
    unclosed = sorted({(piece[0], piece[1]) for split in splits for piece in split["pieces"]
                       if piece[2:4] == split["parent"][2:4]} - set(witnessed))
    if unclosed:
        raise MapError(f"Witness sub-range {unclosed[0][0]:#x}..{unclosed[0][1]:#x} is not closed by its witness")
    document = _document(original, rows, rules_sha256=rules_sha256, catalogue_id=catalogue_id, catalogue=catalogue,
                         splits=splits)
    return document, witnessed


def _generate_tail(original: bytes, rules: Mapping[str, Any], add, source_root: Path = PROJECT) -> None:
    tail, content_end = rules["tail_start"], rules["content_end"]
    if any(original[content_end:]):
        raise MapError("content_end must start the final all-zero fill")
    windows = rule_windows(rules, original, source_root)
    evidence = load_evidence(original, rules, source_root)
    palettes = _sprite_palette_ranges(original, evidence["sprite-palettes.json"])
    palette_rules = {row["id"] for row in rules["sprite_palettes"]}
    palette_rows: dict[int, int] = {}
    streams = tail_streams(original, tail, content_end, evidence["yay0-streams.json"])
    if not streams or streams[-1][1] != content_end:
        raise MapError("content_end must be the end of the last stored stream")
    by_start = {s[0]: s for s in streams}
    bases = {immediate_at(original, row["site"]): row["id"] for row in rules["code_bases"]}
    iff = rules["iff_archive"]
    iff_start = immediate_at(original, iff["site"])
    headers, ff_gaps, forms = efat_structure(original, rules, evidence["efat-evidence.json"])
    bounds = {tail, content_end, EXPECTED_SIZE, iff_start, iff["first_form"], *bases}
    for a, b in headers | ff_gaps | set(forms):
        bounds.update((a, b))
    for s in streams:
        bounds.update(s[:2])
    for w in windows:
        bounds.update((w.start, w.end))
    bounds = sorted(b for b in bounds if tail <= b <= EXPECTED_SIZE)
    starts = [w.start for w in windows]
    first_base = min(bases)
    last_base_end = min(s[0] for s in streams)  # code-based raw region ends at the first stream
    stream_ends = {s[1] for s in streams}

    def innermost(a: int, b: int) -> Window | None:
        best = None
        for w in windows[:bisect.bisect_right(starts, a)]:
            if w.start <= a and b <= w.end and (best is None or w.end - w.start < best.end - best.start):
                best = w
        return best

    for a, b in zip(bounds, bounds[1:]):
        stream = by_start.get(a)
        if stream is not None and stream[1] == b:
            window = innermost(a, b)
            add(a, b, "asset_yay0", window.ref if window else ("IFF_efat" if a >= iff_start else "archive"))
        elif a == content_end:
            add(a, b, "rom_padding_zero", "boot")
        elif (a, b) in headers:
            add(a, b, "asset_iff_container_meta", iff["id"])
        elif (a, b) in ff_gaps:
            add(a, b, "pad_align_ff", iff["id"])
        elif (a in stream_ends and _zero(original, a, b)
              and (b == (a + 3) & ~3 or b == (a + 15) & ~15) and b - a < 16):
            add(a, b, "pad_align_zero", "align")
        elif (window := innermost(a, b)) is not None:
            cls = _window_class(original, window)
            add(a, b, cls, window.ref)
            if window.ref in palette_rules and cls == "asset_raw_referenced":
                palette_rows[a] = b
        elif first_base <= a < last_base_end:
            base = max(c for c in bases if c <= a)
            add(a, b, "asset_raw_code_based", bases[base])
        elif any(start + 12 <= a < b <= end for start, end in forms):
            add(a, b, "asset_iff_payload", iff["id"])
        else:
            add(a, b, "asset_raw_unattributed", "unattributed")
    # The sprite-palette descriptor-walk windows classify every palette payload. sprite-palettes.json is an
    # independent binding of the same data: each of its whole intervals must be a maximal run of palette
    # window rows that tiles it exactly, or generation fails.
    palette_ends = set(palette_rows.values())
    for start, end in sorted(palettes):
        cursor = start
        while cursor < end and cursor in palette_rows:
            cursor = palette_rows[cursor]
        if cursor != end or start in palette_ends or end in palette_rows:
            raise MapError("Palette evidence must name whole intervals tiled exactly by sprite-palette windows")


def _window_class(original: bytes, window: Window) -> str:
    head = original[window.start:window.start + 16]
    if window.ref.startswith("N_palette"):
        return "asset_raw_code_based"
    if window.ref == "T5":
        return "asset_demo_input"
    if head.startswith(b"HVQ2 1.0"):
        return "asset_hvqm2_video"
    if head.startswith(b"N64 PtrTablesV2"):
        return "asset_audio_bank_ptrtable"
    if head.startswith(b"N64 WaveTables"):
        return "asset_audio_wavetable"
    return "asset_raw_referenced"


_EFAT_PACKED = (b"SHRF", b"EGRF")  # chunk data: u32 unpacked size, then one Yay0 stream to the chunk end


def owner_words(original: bytes, rules: Mapping[str, Any],
                evidence: Mapping[str, Any]) -> dict[str, dict[int, tuple[int, int]]]:
    """{rule id: {word site: (start, end)}} for asset format `owner_word` anchors.

    Each walk re-derives, from the validated rules and original words, windows that no code
    immediate or table pair forms; tests pin every result to the generated map rows:
    - sprite palette rules: every segment-6 palette word reached from the directory immediate, the
      descriptor count (slti) and each descriptor's segment-5 table pointer, as `generate` walks
      them; start = segment-6 base + the word's offset, end = start + the TLUT read size;
    - chain rules: every chain start word; end = start + the demo capacity word (= stride);
    - the IFF archive: in every validated EFAT FORM the chunks walked from FORM+12 by their u32
      lengths end exactly at the FORM end, and the tag word of each packed chunk (SHRF/EGRF, data
      a u32 size then Yay0) resolves to the payload before its stream: from FORM+12 or the end of
      the previous packed chunk up to tag + 12.
    """
    words: dict[str, dict[int, tuple[int, int]]] = {}
    for row in rules["sprite_palettes"]:
        directory = immediate_at(original, row["directory_site"])
        segment = immediate_at(original, row["segment_site"])
        count = instruction_immediate(original, row["count_site"], 0x0A)
        size = instruction_immediate(original, row["size_site"], 0x09, base=0)
        found = words.setdefault(row["id"], {})
        for record in range(directory, directory + count * row["record_size"], row["record_size"]):
            members = original[record] & 31
            if not members:
                continue
            pointer = struct.unpack_from(">I", original, record + 4)[0]
            if (pointer >> 24) & 15 != 5:
                raise MapError(f"{row['id']}: palette table pointer at {record + 4:#x} is not segment 5")
            table = directory + (pointer & 0xFFFFFF)
            for site in range(table, table + 4 * members, 4):
                word = struct.unpack_from(">I", original, site)[0]
                if (word >> 24) & 15 != 6:
                    raise MapError(f"{row['id']}: palette word at {site:#x} is not segment 6")
                start = segment + (word & 0xFFFFFF)
                found[site] = (start, start + size)
    for row in rules["chains"]:
        capacity = struct.unpack_from(">I", original, 0x12722C)[0]
        if capacity != row["stride"]:
            raise MapError("Demo consumer capacity changed")
        found = words.setdefault(row["id"], {})
        for site in range(row["rom_start"], row["rom_end"], 4):
            start = struct.unpack_from(">I", original, site)[0]
            found[site] = (start, start + capacity)
    _, _, forms = efat_structure(original, rules, evidence["efat-evidence.json"])
    found = words.setdefault(rules["iff_archive"]["id"], {})
    for form_start, form_end in forms:
        chunk = cursor = form_start + 12
        while chunk < form_end:
            if chunk + 8 > form_end:
                raise MapError(f"EFAT chunk header at {chunk:#x} runs past its FORM")
            after = chunk + 8 + struct.unpack_from(">I", original, chunk + 4)[0]
            if after > form_end:
                raise MapError(f"EFAT chunk at {chunk:#x} runs past its FORM")
            if original[chunk:chunk + 4] in _EFAT_PACKED:
                if after < chunk + 28 or original[chunk + 12:chunk + 16] != b"Yay0":
                    raise MapError(f"EFAT packed chunk at {chunk:#x} does not hold a size and a Yay0 stream")
                found[chunk] = (cursor, chunk + 12)
                cursor = after
            chunk = after
        if chunk != form_end:
            raise MapError(f"EFAT FORM at {form_start:#x} chunks do not end at the FORM end")
    return words


# Witness-driven sub-range splits. A reviewed witness may close part of one generated row: the row
# is split exactly at the witness edges, every covered piece takes the witness's class and ref, and
# every uncovered remainder keeps the row's class, ref and status. A piece keeps its parent's class
# and ref (a located asset sub-range that a format witness closes) unless SPLIT_RECLASS allows its
# class for the parent's class: a pinned g++ vtable (gxxvtable, user ruling
# VTABLES-CLASSIFY-NO-CREDIT-20261014) becomes its own compiler_generated_vtable piece of a cpu_data row,
# and a replayed minimal alignment fill (`cpu_data_padding`) its own padding piece.
SPLIT_RECLASS: dict[str, frozenset[str]] = {"cpu_data": frozenset({gxxvtable.CLASS, "cpu_data_padding"})}


def _elf_data_section(data: bytes, wanted: str, label: str) -> dict[str, Any]:
    """One named PROGBITS data section of a big-endian ELF32 REL object: bytes, relocated-bit mask, alignment."""
    if (len(data) < 0x34 or data[:4] != b"\x7fELF" or data[4] != 1 or data[5] != 2
            or struct.unpack_from(">H", data, 16)[0] != 1 or struct.unpack_from(">H", data, 18)[0] != 8):
        raise MapError(f"{label} is not a big-endian MIPS ELF32 relocatable object")
    shoff = struct.unpack_from(">I", data, 0x20)[0]
    entsize, count, names = struct.unpack_from(">HHH", data, 0x2E)
    sections = [struct.unpack_from(">IIIIIIIIII", data, shoff + i * entsize) for i in range(count)]
    base = sections[names][4]
    named = [i for i, row in enumerate(sections) if data[base + row[0]:data.index(b"\0", base + row[0])] == wanted.encode()]
    if len(named) != 1 or sections[named[0]][1] != 1:
        raise MapError(f"{label} object must have exactly one PROGBITS {wanted}")
    index = named[0]
    _, _, _, _, offset, size, _, _, align, _ = sections[index]
    if not size or align < 1 or align & (align - 1):
        raise MapError(f"{label} {wanted} is empty or not power-of-two aligned")
    mask = bytearray(size)
    for row in sections:
        if row[1] in (4, 9) and row[7] == index:
            width = 12 if row[1] == 4 else 8
            for k in range(row[5] // width):
                where, info = struct.unpack_from(">II", data, row[4] + k * width)
                field = _REL_FIELD.get(info & 0xFF)
                if field is None or where % 4 or where + 4 > size:
                    raise MapError(f"{label} has an unsupported {wanted} relocation")
                for j in range(4):
                    mask[where + j] |= (field >> (24 - 8 * j)) & 0xFF
    return {"body": data[offset:offset + size], "mask": bytes(mask), "align": align}


def _padding_file(source_root: Path, pin: object, suffix: str, label: str) -> bytes:
    """A content-addressed CPU data padding witness file (object or compiler assembly)."""
    if not isinstance(pin, dict) or pin.get("path") != f"{CPU_DATA_PADDING_DIR}/{pin.get('sha256')}{suffix}":
        raise MapError(f"{label} is not content-addressed")
    data = (source_root / pin["path"]).read_bytes()
    if sha256_bytes(data) != pin["sha256"]:
        raise MapError(f"{label} identity changed")
    return data


def _first_object_align(asm: str, section: str) -> int | None:
    """The alignment the compiler requested for the first object it emits into `section`: the largest of
    the consecutive `.align` directives right after the first switch into that section (1 if none)."""
    lines = [line.split("#", 1)[0].strip() for line in asm.splitlines()]
    for index, line in enumerate(lines):
        if line == ".data" if section == ".data" else (line.split() == [".section", ".rodata"] or line == ".rdata"):
            power = 0
            for follow in lines[index + 1:]:
                if not follow:
                    continue
                parts = follow.split()
                if parts[0] == ".align" and len(parts) == 2 and parts[1].isdigit():
                    power = max(power, int(parts[1]))
                    continue
                return 1 << power
            return None
    return None


def cpu_data_padding(original: bytes, catalogue: Mapping[str, Any], evidence: Mapping[str, Any],
                     base_rows: list[list[Any]], vtables: Mapping[tuple[int, int], str],
                     source_root: Path = PROJECT) -> list[tuple[Any, ...]]:
    """Replay every cpu_data alignment-padding record; split_rows pieces for the closed runs.

    A zero run [start, end) inside one cpu_data row closes only as the minimal fill before an object
    that requests alignment N: the previous object is a canonical YAML carve [carve_start, start) whose
    pinned compiled data section is exactly that long and equals the original on every non-relocation
    bit; the next object is either the carve [end, carve_end) with the same proof, whose pinned compiled
    .s requests N for the section's first object (equal to the section's ELF alignment), or a replayed
    g++ vtable starting at end, for which the vtable layer's pinned cc1plus probe requests N. Then
    (end + vram) % N == 0 and end - start < N. Zero bytes, geometry or a section alignment the compiler
    did not request for the first object never qualify; nothing here grants C/C++ or owned-data credit.
    """
    document = evidence[CPU_DATA_PADDING_INPUT]
    if (document.get("schema") != "shiren2-cpu-data-padding-evidence-v2" or document.get("target_sha256") != EXPECTED_SHA256
            or document.get("rule") != CPU_DATA_PADDING_RULE or not isinstance(document.get("runs"), list)):
        raise MapError("CPU data padding evidence does not describe the fixed target")
    images = {row["image_id"]: row for row in catalogue["images"]}
    carves = {(row[0], row[1]): (row[2], row[3]) for row in splat_rows(source_root)}
    starts = [row[0] for row in base_rows]
    tables = {start: (end, table) for (start, end), table in vtables.items()}
    probe = evidence[gxxvtable.INPUT]["layout"]["probe_output_sha256"]
    pieces: list[tuple[Any, ...]] = []

    def carve(side: Mapping[str, Any], a: int, b: int, which: str, label: str) -> dict[str, Any]:
        section, unit = side.get("section"), side.get("unit")
        if side.get("kind") != "carve" or section not in _DATA_SECTIONS or carves.get((a, b)) != (section, unit):
            raise MapError(f"{label}: the {which} object is not the canonical {section} carve {a:#x}..{b:#x}")
        data = _padding_file(source_root, side.get("object"), ".o", f"{label} {which} object")
        obj = _elf_data_section(data, section, f"{label} {which}")
        body, mask = obj["body"], obj["mask"]
        if len(body) != b - a or any((x ^ y) & ~m & 0xFF for x, y, m in zip(body, original[a:b], mask)):
            raise MapError(f"{label}: the {which} object section is not exactly the original carve")
        return obj

    for record in document["runs"]:
        start, end = _int(record.get("start"), "start"), _int(record.get("end"), "end")
        align, image_id = _int(record.get("align"), "align"), record.get("image_id")
        label = f"CPU data padding {start:#x}..{end:#x}"
        image = images.get(image_id)
        if image is None or record.get("id") != f"cdp_{start:08x}_{end:08x}" or not 0 < start < end:
            raise MapError(f"{label} has an unsupported id or image")
        vram = image["vram_start"] - image["rom_start"]
        if align < 1 or align & (align - 1) or end - start >= align:
            raise MapError(f"{label} is not shorter than its alignment")
        if (end + vram) % align:
            raise MapError(f"{label}: the next object start is not aligned to {align}")
        if not _zero(original, start, end):
            raise MapError(f"{label} is not a zero run")
        previous, following = record.get("previous") or {}, record.get("next") or {}
        low = _int(previous.get("carve_start"), "carve_start")
        if following.get("kind") == "vtable":
            end_table = tables.get(end)
            if end_table is None or end_table[1] != following.get("table"):
                raise MapError(f"{label}: the next object is not the replayed vtable starting at {end:#x}")
            high = end_table[0]
            pin = following.get("compiler_probe")
            asm = _padding_file(source_root, pin, ".s", f"{label} next compiler probe")
            if pin["sha256"] != probe:
                raise MapError(f"{label}: the next compiler probe is not the vtable layer's pinned cc1plus probe")
            requested = _first_object_align(asm.decode("utf-8"), ".rodata")
        else:
            high = _int(following.get("carve_end"), "carve_end")
            obj = carve(following, end, high, "next", label)
            asm = _padding_file(source_root, following.get("compiled_asm"), ".s", f"{label} next compiled assembly")
            requested = _first_object_align(asm.decode("utf-8"), following["section"])
            if obj["align"] != requested:
                raise MapError(f"{label}: the next object alignment is {obj['align']}, not the first object's {requested}")
        if requested != align:
            raise MapError(f"{label}: the compiler requested alignment {requested} for the next object, not {align}")
        if following.get("first_object_align") != align:
            raise MapError(f"{label}: the recorded first-object alignment is not {align}")
        index = bisect.bisect_right(starts, start) - 1
        row = base_rows[index] if index >= 0 else None
        if row is None or row[2:] != ["cpu_data", image_id] or not row[0] <= low < start < end < high <= row[1]:
            raise MapError(f"{label} and its two objects do not lie inside one cpu_data row of {image_id}")
        carve(previous, low, start, "previous", label)
        pieces.append((start, end, "cpu_data_padding", CPU_DATA_PADDING_RULE, record["id"], (row[0], row[1])))
    return pieces


def witness_subranges(document: object) -> list[tuple[int, int, str, str, str, tuple[int, int]]]:
    """(start, end, class, ref, witness id, (parent start, parent end)) of every asset format witness
    range record with "within"; `assetformat.replay` checks the records in full after the split."""
    pieces = []
    witnesses = document.get("witnesses") if isinstance(document, dict) else None
    for record in witnesses if isinstance(witnesses, list) else []:
        for row in record.get("ranges", []) if isinstance(record, dict) else []:
            if not isinstance(row, dict) or "within" not in row:
                continue
            parent = row["within"]
            if not isinstance(parent, dict):
                raise MapError(f"Witness sub-range in {record.get('id')!r} names a malformed parent")
            label = f"Witness sub-range in {record.get('id')!r}"
            pieces.append((_int(row.get("start"), label), _int(row.get("end"), label), _text(row.get("class"), label),
                           _text(row.get("ref"), label), _text(record.get("id"), label),
                           (_int(parent.get("start"), label), _int(parent.get("end"), label))))
    return pieces


def split_rows(rows: list[list[Any]], pieces: Iterable[tuple[Any, ...]]) -> tuple[list[list[Any]], list[dict[str, Any]]]:
    """Split sorted, tiling generated rows exactly at reviewed witness sub-ranges.

    Each piece is (start, end, class, ref, source[, (parent start, parent end)]). It must lie inside
    exactly one row (its parent; a named parent must be that row) and overlap no other piece. Keeping
    the parent's class and ref requires a located parent and a proper sub-range; any other class must
    be allowed by SPLIT_RECLASS for the parent's class. Remainders keep the parent's class and ref.
    Returns the split rows and the records that explain the partition change:
    [{"parent": [start, end, class, ref], "pieces": [[start, end, class, ref, source], ...]}].
    """
    starts = [row[0] for row in rows]
    groups: dict[int, list[tuple[Any, ...]]] = {}
    ordered = sorted(pieces, key=lambda piece: (piece[0], piece[1]))
    for before, after in zip(ordered, ordered[1:]):
        if after[0] < before[1]:
            raise MapError(f"Witness sub-ranges overlap at {after[0]:#x} ({before[4]}, {after[4]})")
    for piece in ordered:
        start, end, cls, ref, source = piece[:5]
        index = bisect.bisect_right(starts, start) - 1
        if not start < end or index < 0 or end > rows[index][1]:
            raise MapError(f"Witness sub-range {start:#x}..{end:#x} ({source}) crosses a map row boundary")
        parent = rows[index]
        if len(piece) > 5 and tuple(piece[5]) != (parent[0], parent[1]):
            raise MapError(f"Witness sub-range {start:#x} ({source}) names a parent that is not its containing row")
        if cls not in CLASSES:
            raise MapError(f"Witness sub-range {start:#x} ({source}) has an unsupported class {cls}")
        if (cls, ref) == (parent[2], parent[3]):
            if research_status(cls) != "located" or (start, end) == (parent[0], parent[1]):
                raise MapError(f"Witness sub-range {start:#x} ({source}) is not a proper sub-range of a located row")
        elif cls not in SPLIT_RECLASS.get(parent[2], frozenset()):
            raise MapError(f"Witness sub-range {start:#x} ({source}) changes class {parent[2]} -> {cls}")
        groups.setdefault(index, []).append(piece)
    result: list[list[Any]] = []
    splits: list[dict[str, Any]] = []
    for index, row in enumerate(rows):
        if index not in groups:
            result.append(row)
            continue
        cursor, record = row[0], {"parent": list(row), "pieces": []}
        for start, end, cls, ref, source, *_ in groups[index]:
            if start > cursor:
                result.append([cursor, start, row[2], row[3]])
            result.append([start, end, cls, ref])
            record["pieces"].append([start, end, cls, ref, source])
            cursor = end
        if cursor < row[1]:
            result.append([cursor, row[1], row[2], row[3]])
        splits.append(record)
    check_splits(result, splits)
    return result, splits


def _split_parents(splits: object) -> list[list[Any]]:
    """The split records' parents, sorted; MapError unless every record is well formed and the parents
    are pairwise disjoint (no duplicate, overlapping or nested parent: SolReviewAssetStack F2 P3)."""
    if not isinstance(splits, list):
        raise MapError("ROM map splits are malformed")
    parents = []
    for split in splits:
        if (not isinstance(split, dict) or set(split) != {"parent", "pieces"} or not isinstance(split["parent"], list)
                or len(split["parent"]) != 4 or not isinstance(split["pieces"], list) or not split["pieces"]
                or type(split["parent"][0]) is not int or type(split["parent"][1]) is not int
                or not 0 <= split["parent"][0] < split["parent"][1]):
            raise MapError("ROM map split record is malformed")
        parents.append(split["parent"])
    parents.sort(key=lambda parent: parent[0])
    for before, after in zip(parents, parents[1:]):
        if after[0] < before[1]:
            raise MapError(f"Split parents {before[0]:#x}..{before[1]:#x} and {after[0]:#x}..{after[1]:#x} "
                           f"are duplicate, overlapping or nested")
    return parents


def check_splits(rows: list[list[Any]], splits: object) -> None:
    """The split parents are disjoint, and within every parent the rows are exactly its pieces and
    maximal remainders of the parent's class and ref: a remainder never changes class and never gains
    an unexplained boundary."""
    _split_parents(splits)
    starts = [row[0] for row in rows]
    for split in splits:
        a, b, cls, ref = split["parent"]
        pieces = {(p[0], p[1]): tuple(p[2:4]) for p in split["pieces"] if isinstance(p, list) and len(p) == 5}
        if len(pieces) != len(split["pieces"]):
            raise MapError("ROM map split piece is malformed")
        index, position, remainder = bisect.bisect_left(starts, a), a, False
        while position < b:
            if index >= len(rows) or rows[index][0] != position or rows[index][1] > b:
                raise MapError(f"Split parent {a:#x}..{b:#x} is not tiled by its pieces and remainders")
            start, end, row_cls, row_ref = rows[index]
            piece = pieces.pop((start, end), None)
            if piece is None and (remainder or (row_cls, row_ref) != (cls, ref)):
                raise MapError(f"Split remainder {start:#x}..{end:#x} changes class or boundary ({cls} -> {row_cls})")
            if piece is not None and (row_cls, row_ref) != piece:
                raise MapError(f"Split piece {start:#x}..{end:#x} is not its declared class and ref")
            position, index, remainder = end, index + 1, piece is None
        if pieces:
            raise MapError(f"Split piece {min(pieces)[0]:#x} is not a row of its parent {a:#x}")


def unsplit_rows(rows: list[list[Any]], splits: list[dict[str, Any]]) -> list[list[Any]]:
    """The partition before witness sub-range splits (each split parent restored), for replays of a
    stored map; `check_splits` must already have accepted `rows` and `splits` (the parents are checked
    disjoint again here, so the restored rows never overlap)."""
    parents = {parent[0]: parent for parent in _split_parents(splits)}
    result: list[list[Any]] = []
    for row in rows:
        if row[0] in parents:
            result.append(list(parents[row[0]]))
        elif not (result and row[1] <= result[-1][1] and result[-1][0] in parents):
            result.append(row)
    return result


def format_witnesses(original: bytes, catalogue: Mapping[str, Any], rows: Iterable[list[Any]],
                     rules: Mapping[str, Any], source_root: Path = PROJECT,
                     base_rows: Iterable[list[Any]] | None = None) -> dict[tuple[int, int], str]:
    """Replay every pinned asset format witness; {exact located row interval: witness id}.

    A witness closes only whole located rows (or their proper sub-ranges, split from `base_rows`, the
    partition before witness splits) whose class and owner ref it names; the parser must type every
    byte and agree with the catalogued consumers' accesses.
    """
    table = {(row[0], row[1]): (row[2], row[3]) for row in rows}
    base = table if base_rows is None else {(row[0], row[1]): (row[2], row[3]) for row in base_rows}
    evidence = load_evidence(original, rules, source_root)
    words = owner_words(original, rules, evidence)
    try:
        witnessed = assetformat.replay(original, evidence[assetformat.INPUT], catalogue["functions"], table,
                                       immediate_at, EXPECTED_SHA256, {row["id"]: row for row in rules["tables"]},
                                       words, base)
    except assetformat.AssetFormatError as error:
        raise MapError(f"Asset format witness: {error}") from error
    for interval in witnessed:
        if research_status(table[interval][0]) != "located":
            raise MapError(f"Asset format witness names a range that is not located: {interval[0]:#x}")
    return witnessed


def cpu_vtables(original: bytes, catalogue: Mapping[str, Any], evidence: Mapping[str, Any],
                base_rows: Iterable[list[Any]]) -> dict[tuple[int, int], str]:
    """Replay every pinned g++ vtable witness against the base (unsplit) cpu_data rows.

    {exact (start, end): table id}; each interval becomes a `compiler_generated_vtable` piece
    with ref = id, carved out of its cpu_data parent by the witness split.
    """
    try:
        return gxxvtable.replay(original, evidence[gxxvtable.INPUT], catalogue, base_rows, EXPECTED_SHA256)
    except gxxvtable.VtableError as error:
        raise MapError(f"Vtable witness: {error}") from error


def _content_digest(original: bytes, rows: Iterable[list[Any]]) -> str:
    digest = hashlib.sha256()
    for start, end, *_ in rows:
        digest.update(hashlib.sha256(original[start:end]).digest())
    return digest.hexdigest()


def _document(original: bytes, rows: list[list[Any]], *, rules_sha256: str, catalogue_id: str,
              catalogue: Mapping[str, Any], splits: list[dict[str, Any]] | None = None) -> dict[str, Any]:
    content = _content_digest(original, rows)
    partition = json_digest({"target_sha256": EXPECTED_SHA256,
                             "ranges": [[row[0], row[1]] for row in rows], "content_sha256": content})
    policy = json_digest({"evidence_sha256": EVIDENCE_SHA256, "rules_sha256": rules_sha256,
                          "classes": dict(CLASSES), "range_classes": [row[2:] for row in rows],
                          "research_statuses": {cls: research_status(cls) for cls in CLASSES},
                          "audio_validator_sha256": sha256_bytes(Path(rspaudio.__file__).read_bytes()),
                          "asset_format_validator_sha256": sha256_bytes(Path(assetformat.__file__).read_bytes()),
                          "flow_engine_sha256": sha256_bytes(Path(mipsflow.__file__).read_bytes()),
                          "vtable_validator_sha256": sha256_bytes(Path(gxxvtable.__file__).read_bytes()),
                          "library_policy_sha256": sha256_bytes(Path(toolchain_library.__file__).read_bytes()),
                          "validator_sha256": sha256_bytes(Path(__file__).read_bytes())})
    identity = {"target_sha256": EXPECTED_SHA256, "target_size": EXPECTED_SIZE,
                "cpu_catalogue_id": catalogue_id, "partition_sha256": partition,
                "evidence_policy_sha256": policy}
    # `splits` explains every row boundary that a witness sub-range adds to the generated partition.
    return {"schema_version": 2, "kind": MAP_KIND, "map_id": json_digest(identity), **identity,
            "rules_sha256": rules_sha256, "ranges": rows, "content_sha256": content,
            "cpu_catalogue": catalogue, "range_count": len(rows), "splits": splits or []}


# ---------------------------------------------------------------------------- validation


@dataclass(frozen=True)
class RomMap:
    map_id: str
    rules_sha256: str
    cpu_catalogue_id: str
    ranges: tuple[tuple[int, int, str, str], ...]
    partition_sha256: str
    evidence_policy_sha256: str
    # Located ranges closed by a replayed asset format witness (derived by `validate`).
    format_witnessed: frozenset[tuple[int, int]] = field(default_factory=frozenset)

    def of_class(self, *classes: str) -> list[tuple[int, int, str, str]]:
        return [r for r in self.ranges if r[2] in classes]


def validate(document: object, original: bytes, inventory: ImageInventory, rules_document: object,
             *, rules_sha256: str, source_root: Path = PROJECT) -> RomMap:
    """Structural, hash and evidence checks; cheap enough for every build and verification."""
    attest(original)
    record = _record(document, "ROM map", {"schema_version", "kind", "map_id", "target_sha256", "target_size",
                                           "rules_sha256", "cpu_catalogue_id", "cpu_catalogue", "ranges", "content_sha256", "range_count",
                                           "partition_sha256", "evidence_policy_sha256", "splits"})
    if (record["schema_version"] != 2 or record["kind"] != MAP_KIND or record["target_sha256"] != EXPECTED_SHA256
            or record["target_size"] != EXPECTED_SIZE or len(original) != EXPECTED_SIZE):
        raise MapError("ROM map does not describe the fixed target")
    if record["rules_sha256"] != rules_sha256:
        raise MapError("ROM map was generated from different region rules")
    if not isinstance(record["cpu_catalogue_id"], str) or not _SHA.fullmatch(record["cpu_catalogue_id"]):
        raise MapError("ROM map must name its frozen CPU catalogue")
    rules = load_rules(rules_document)
    windows = rule_windows(rules, original, source_root)
    rows = record["ranges"]
    if not isinstance(rows, list) or record["range_count"] != len(rows):
        raise MapError("ROM map range inventory is malformed")
    refs = {"boot", "catalogue", "align", "archive", "unattributed", *inventory.images,
            *(c.component_id for c in inventory.components), *CPU_PADDING_RULES}
    for key in ("tables", "chains", "immediate_windows", "inferred_windows", "code_bases", "sprite_palettes"):
        refs.update(row["id"] for row in rules[key])
    refs.add(rules["iff_archive"]["id"])
    refs.update(row["id"] for row in load_evidence(original, rules, source_root)[RESIDUAL_INPUT]["ranges"])
    check_splits(rows if isinstance(rows, list) and all(isinstance(r, list) and len(r) == 4 for r in rows) else [],
                 record["splits"])
    refs.update(piece[3] for split in record["splits"] for piece in split["pieces"] if isinstance(piece[3], str))
    position = 0
    ranges = []
    for row in rows:
        if (not isinstance(row, list) or len(row) != 4 or type(row[0]) is not int or type(row[1]) is not int
                or row[2] not in CLASSES
                or (row[3] not in refs if row[2] != gxxvtable.CLASS else row[3] != _vtable_ref(inventory, row[0]))):
            raise MapError(f"Malformed ROM map range: {row!r}"[:200])
        start, end, cls, ref = row
        if start != position or end <= start:
            raise MapError(f"ROM map ranges do not tile the ROM at {position:#x}")
        if cls in _ZERO_CLASSES and not _zero(original, start, end):
            raise MapError(f"Padding range {start:#x} holds nonzero bytes")
        if cls == "pad_align_zero" and end - start >= 16:
            raise MapError(f"Alignment padding at {start:#x} is too long")
        if cls == "asset_yay0" and original[start:start + 4] != b"Yay0":
            raise MapError(f"Yay0 range {start:#x} lacks its signature")
        if cls in {"asset_audio_bank_ptrtable", "asset_audio_wavetable", "asset_hvqm2_video", "asset_demo_input"}:
            if not any(w.ref == ref and w.start <= start < end <= w.end
                       and _window_class(original, w) == cls for w in windows):
                raise MapError(f"Raw asset format/owner evidence disagrees at {start:#x}")
        position = end
        ranges.append((start, end, cls, ref))
    if position != EXPECTED_SIZE:
        raise MapError("ROM map ranges do not end at the ROM size")
    if ranges[-1][2] != "rom_padding_zero" or ranges[-1][0] != rules["content_end"]:
        raise MapError("ROM map must end with the final zero fill at content_end")
    if (ranges[0][:3], ranges[1][:3]) != ((0, 0x40, "rom_header"), (0x40, 0x1000, "ipl3_bootcode")):
        raise MapError("ROM header/IPL3 ranges changed")
    if hashlib.md5(original[0x40:0x1000]).hexdigest() != rules["ipl3_md5"]:
        raise MapError("IPL3 identity changed")
    components = set(_rsp_components(original, inventory, source_root).values())
    if {r for r in ranges if CLASSES[r[2]] == "rsp_ucode"} != components:
        raise MapError("RSP ranges disagree with reviewed images.json components")
    for image in inventory.images.values():
        inside = [r for r in ranges if image.rom_start <= r[0] < image.rom_end]
        if not inside or inside[0][0] != image.rom_start or inside[-1][1] != image.rom_end:
            raise MapError(f"ROM map ranges straddle image {image.image_id}")
        hints = [(h.rom_start, h.rom_end) for h in image.provisional_cpu_ranges]
        for start, end, cls, _ in inside:
            in_cpu = any(a <= start and end <= b for a, b in hints)
            if cls in ("cpu_text_function", "cpu_text_unowned", "cpu_text_padding", "cpu_text_unexplained") and not in_cpu:
                raise MapError(f"CPU text range {start:#x} lies outside reviewed CPU ranges")
            if cls in ("cpu_data", "image_opaque", gxxvtable.CLASS, "cpu_data_padding") and any(
                    start < b and a < end for a, b in hints):
                raise MapError(f"Data range {start:#x} overlaps a reviewed CPU range")
            if cls == "image_opaque" and hints:
                raise MapError(f"Split image {image.image_id} still has opaque storage")
        for a, b in hints:
            covered = [r for r in inside if a <= r[0] and r[1] <= b]
            if sum(r[1] - r[0] for r in covered) != b - a or any(
                    r[2] not in ("cpu_text_function", "cpu_text_unowned", "cpu_text_padding", "cpu_text_unexplained")
                    for r in covered):
                raise MapError(f"Reviewed CPU range {a:#x} is not tiled by function/unowned/padding/residual ranges")
    for start, end, cls, _ in ranges:
        if start >= rules["tail_start"] and CLASSES[cls] in ("cpu_code", "cpu_data", "cpu_image_opaque", "rsp_ucode", "boot"):
            raise MapError(f"CPU/RSP class {cls} in the stored tail at {start:#x}")
    if _content_digest(original, rows) != record["content_sha256"]:
        raise MapError("ROM map content digest disagrees with the original")
    expected = _document(original, rows, rules_sha256=rules_sha256,
                         catalogue_id=record["cpu_catalogue_id"], catalogue=record["cpu_catalogue"])
    if any(record[key] != expected[key] for key in ("partition_sha256", "evidence_policy_sha256", "map_id")):
        raise MapError("ROM map partition or evidence-policy identity digest changed")
    rom_map = RomMap(record["map_id"], record["rules_sha256"], record["cpu_catalogue_id"], tuple(ranges),
                     record["partition_sha256"], record["evidence_policy_sha256"])
    catalogue = _read_catalogue(source_root / CATALOGUE_PATH)
    check_catalogue(rom_map, catalogue)
    if record["cpu_catalogue"] != catalogue["identity"]:
        raise MapError("Embedded CPU catalogue differs from the build denominator")
    # Regeneration uses pinned stream evidence, not compression. It replays the
    # structural classification of every range; rehashing forged rows cannot pass.
    regenerated, witnessed = _generate(original, inventory, record["cpu_catalogue"], rules,
                                       catalogue_id=record["cpu_catalogue_id"], rules_sha256=rules_sha256,
                                       source_root=source_root)
    if regenerated["ranges"] != rows or regenerated["splits"] != record["splits"]:
        raise MapError("ROM map classifications disagree with replayed range properties")
    return replace(rom_map, format_witnessed=frozenset(witnessed))


def load(source_root: Path, original: bytes, inventory: ImageInventory | None = None) -> RomMap:
    inventory = inventory if inventory is not None else load_inventory(source_root / "config/images.json")
    rules_bytes = (source_root / RULES_PATH).read_bytes()
    return validate(json.loads((source_root / MAP_PATH).read_text()), original, inventory, json.loads(rules_bytes),
                    rules_sha256=sha256_bytes(rules_bytes), source_root=source_root)


def check_catalogue(rom_map: RomMap, catalogue: Mapping[str, Any]) -> None:
    """The map's function ranges are exactly the frozen catalogue's function extents."""
    if (json_digest(catalogue["identity"]) != catalogue["denominator_id"]
            or catalogue["identity"]["target_sha256"] != EXPECTED_SHA256):
        raise MapError("CPU catalogue identity is invalid")
    expected = sorted((f["rom_start"], f["rom_start"] + f["size"]) for f in catalogue["identity"]["functions"])
    actual = sorted((r[0], r[1]) for r in rom_map.of_class("cpu_text_function"))
    if catalogue["denominator_id"] != rom_map.cpu_catalogue_id or expected != actual:
        raise MapError("ROM map function ranges disagree with the frozen CPU catalogue")


# ---------------------------------------------------------------------------- accounting


def splat_rows(source_root: Path) -> list[tuple[int, int, str, str | None]]:
    """Flatten frozen YAML subsegments into (start, end, type, name) ROM intervals."""
    config = yaml.safe_load((source_root / "config/shiren2.jp.yaml").read_text())
    segments = config["segments"]

    def start_of(item: object) -> int | None:
        value = item[0] if isinstance(item, list) else item.get("start") if isinstance(item, dict) else None
        return value if type(value) is int else None

    rows = []
    for index, segment in enumerate(segments[:-1]):
        end = start_of(segments[index + 1])
        if not isinstance(segment, dict) or not segment.get("subsegments"):
            continue
        parts = [p for p in segment["subsegments"] if start_of(p) is not None]
        for k, part in enumerate(parts):
            stop = start_of(parts[k + 1]) if k + 1 < len(parts) else end
            kind = part[1] if isinstance(part, list) else part.get("type")
            name = (part[2] if len(part) > 2 else None) if isinstance(part, list) else part.get("name")
            if stop > start_of(part):
                rows.append((start_of(part), stop, kind, name))
    return rows


@dataclass(frozen=True)
class _HandwrittenContext:
    """Only the proof inputs consumed by H13; not an accepted receipt."""
    root: Path
    original: bytes
    document: dict[str, Any]


def _handwritten(source_root: Path, original: bytes,
                 matches: list[dict[str, Any]]) -> set[tuple[int, int]]:
    from progress import validate_handwritten
    document = json.loads((source_root / HANDWRITTEN_PATH).read_text())
    catalogue = _read_catalogue(source_root / CATALOGUE_PATH)
    context = _HandwrittenContext(source_root, original, {"c_matches": matches})
    validate_handwritten(context, catalogue, document)
    return {(row["rom_start"], row["rom_start"] + row["size"]) for row in document["functions"]}


def research_status(cls: str, witnessed: bool = False) -> str:
    """Research disposition after `validate`; never consult receipt/status assertions.

    `witnessed` is true only for an exact range that `format_witnesses` replayed; it turns a
    located range into classified_binary and is refused for every other disposition.
    """
    if cls not in CLASSES:
        raise MapError(f"Unsupported range class: {cls}")
    if cls in RESIDUAL_CLASSES.values():
        status = "closed_unexplained_residual"
    elif cls in {"cpu_text_function", "cpu_text_unowned", "cpu_data", "image_opaque", "asset_raw_unattributed"}:
        status = "open"
    elif CLASSES[cls] == "padding":
        status = "closed"
    elif (cls == "asset_yay0" or cls == "asset_iff_container_meta" or cls == gxxvtable.CLASS
          or CLASSES[cls] in {"boot", "rsp_ucode"}):
        status = "classified_binary"
    else:
        status = "located"
    if witnessed:
        if status != "located":
            raise MapError(f"Only a located range can be closed by a format witness: {cls}")
        return "classified_binary"
    return status


def _vtable_ref(inventory: ImageInventory, start: int) -> str | None:
    """The only ref a compiler_generated_vtable row may carry: its table id in its own image."""
    image = next((i.image_id for i in inventory.images.values() if i.rom_start <= start < i.rom_end), None)
    return gxxvtable.table_id(image, start) if image else None


def refuse_vtable_credit(rom_map: RomMap, owned: Iterable[tuple[int, int]]) -> None:
    """Compiler-generated vtables earn no owned-data credit (docs/OMP_WORKFLOW.md:200-201, user ruling)."""
    vtables = rom_map.of_class(gxxvtable.CLASS)
    for start, end in owned:
        if any(start < b and a < end for a, b, *_ in vtables):
            raise MapError(f"Owned C data {start:#x}..{end:#x} overlaps a compiler-generated vtable")


def account(rom_map: RomMap, original: bytes, source_root: Path, matches: list[dict[str, Any]],
            rules_document: Mapping[str, Any], build_directory: Path | None = None) -> dict[str, Any]:
    """Separate research disposition and candidate build-source bytes.

    Only certification's bound receipt/replay pipeline supplies build_directory.
    This report is not acceptance. Naked boolean roundtrip assertions are rejected.
    """
    rules = load_rules(rules_document)
    functions = {(r[0], r[1]) for r in rom_map.of_class("cpu_text_function")}
    credited: dict[tuple[int, int], str] = {}
    for match in matches:
        key = (match["rom_start"], match["rom_start"] + match["size"])
        if key not in functions:
            raise MapError(f"C/C++ credit outside a catalogued cpu_text_function range: {match['symbol']}")
        if key in credited:
            raise MapError(f"Duplicate C/C++ credit for one function range: {match['symbol']}")
        if Path(str(match["source"])).suffix not in COMPILED_SUFFIXES.values():
            raise MapError("C/C++ credit requires a compiled C/C++ source")
        family = match_family(match)
        if family == FAMILY:
            validate_source(source_root, match["source"])
        credited[key] = f"matched_{family}"
    handwritten = _handwritten(source_root, original, matches)
    matched_sources = {str(match["source"]) for match in matches}
    owned = sorted((s, e) for s, e, kind, name in splat_rows(source_root)
                   if build_directory is not None and kind in {".data", ".rodata"} and name is not None
                   and any(f"src/{name}{suffix}" in matched_sources
                           and (source_root / f"src/{name}{suffix}").is_file()
                           for suffix in COMPILED_SUFFIXES.values()))
    refuse_vtable_credit(rom_map, owned)
    asset_streams = None
    if build_directory is not None:
        if not isinstance(build_directory, Path):
            raise MapError("Roundtrip requires a build directory, not boolean claims")
        from assets import read_manifest
        asset_streams = read_manifest(build_directory, rom_map)
        if (set(asset_streams) != {r[0] for r in rom_map.of_class("asset_yay0")}
                or any(type(value) is not bool for value in asset_streams.values())):
            raise MapError("Asset roundtrip manifest does not cover exactly the map's Yay0 streams")
    classes: dict[str, dict[str, int]] = {}
    status: dict[str, int] = {}
    cpu = {"functions": 0, "instruction_bytes": 0}
    compressed = {"streams": 0, "stream_bytes": 0, "decoded_bytes": 0, "roundtrip_streams": 0, "roundtrip_stream_bytes": 0}
    dispositions: list[dict[str, Any]] = []
    research = 0

    def credit(group: str, state: str, size: int) -> None:
        key = f"{group}:{state}"
        status[key] = status.get(key, 0) + size
        if state not in BUILD_STATUSES | RESEARCH_STATUSES | OPEN_STATUSES:
            raise MapError(f"Unsupported range status: {state}")
        if size:
            dispositions.append({"start": credit_start, "end": credit_start + size,
                                 "class": cls, "ref": ref, "status": state})

    for start, end, cls, ref in rom_map.ranges:
        size, group = end - start, CLASSES[cls]
        credit_start = start
        witnessed = (start, end) in rom_map.format_witnessed
        if research_status(cls, witnessed) in RESEARCH_STATUSES:
            research += size
        row = classes.setdefault(cls, {"ranges": 0, "bytes": 0})
        row["ranges"] += 1
        row["bytes"] += size
        if cls == "cpu_text_function":
            state = credited.get((start, end),
                                 "classified_handwritten" if (start, end) in handwritten else "open")
            if state == "classified_handwritten":
                research += size
            cpu["functions"] += 1
            cpu["instruction_bytes"] += size
            cpu[f"{state}_functions"] = cpu.get(f"{state}_functions", 0) + 1
            cpu[f"{state}_bytes"] = cpu.get(f"{state}_bytes", 0) + size
            credit(group, state, size)
        elif cls == "cpu_data":
            cursor = start
            for a, b in owned:
                a, b = max(start, a), min(end, b)
                if a >= b:
                    continue
                credit_start = cursor
                credit(group, "open", a - cursor)
                credit_start = a
                credit(group, "owned_c", b - a)
                cursor = b
            credit_start = cursor
            credit(group, "open", end - cursor)
        elif cls == "image_opaque":
            credit(group, "open", size)
        elif group in ("rsp_ucode", "boot"):
            credit(group, "classified_binary", size)
        elif group == "padding":
            credit(group, "closed", size)
        elif cls == "asset_yay0":
            exact = bool(asset_streams and asset_streams.get(start))
            compressed["streams"] += 1
            compressed["stream_bytes"] += size
            compressed["decoded_bytes"] += struct.unpack_from(">I", original, start + 4)[0]
            compressed["roundtrip_streams"] += exact
            compressed["roundtrip_stream_bytes"] += size if exact else 0
            credit(group, "roundtrip" if exact else "classified_binary", size)
        elif cls == "asset_raw_unattributed":
            credit(group, "open", size)
        else:
            credit(group, research_status(cls, witnessed), size)
    build_source = (sum(v for k, v in status.items() if k.split(":")[1] in BUILD_STATUSES)
                    if build_directory is not None else 0)
    total = sum(status.values())
    if total != EXPECTED_SIZE:
        raise MapError("Accounted bytes do not sum to the ROM size")
    return {"map_id": rom_map.map_id, "rom_size": EXPECTED_SIZE, "classes": dict(sorted(classes.items())),
            "status_bytes": dict(sorted(status.items())), "cpu_code": cpu, "asset_compressed": compressed,
            "research_classified_bytes": research, "research_open_bytes": total - research,
            "build_source_bytes": build_source, "build_source_state": "unaccepted",
            "range_statuses": dispositions, "whole_denominator_closed": False,
            "partition_sha256": rom_map.partition_sha256, "evidence_policy_sha256": rom_map.evidence_policy_sha256,
            "credit_rule": "C/C++ credit only on exact cpu_text_function ranges; asm, data, assets and padding never count as matched C"}


# ---------------------------------------------------------------------------- CLI


def _read_catalogue(path: Path) -> dict[str, Any]:
    document = json.loads(path.read_text())
    if document.get("schema_version") != 1 or json_digest(document["identity"]) != document["denominator_id"]:
        raise MapError("CPU catalogue identity is invalid")
    return document


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    commands = parser.add_subparsers(dest="command", required=True)
    gen = commands.add_parser("generate", help="regenerate config/rom_map.json from the original and reviewed rules")
    gen.add_argument("--catalogue", type=Path, required=True)
    gen.add_argument("--output", type=Path, default=PROJECT / MAP_PATH)
    check = commands.add_parser("check", help="validate the frozen map; --full also regenerates it")
    check.add_argument("--full", action="store_true")
    check.add_argument("--catalogue", type=Path)
    for sub in (gen, check):
        sub.add_argument("--root", type=Path, default=PROJECT)
        sub.add_argument("--rom", type=Path, default=CANONICAL_ROM)
    args = parser.parse_args()
    try:
        original = args.rom.read_bytes()
        inventory = load_inventory(args.root / "config/images.json")
        rules_bytes = (args.root / RULES_PATH).read_bytes()
        rules_sha = sha256_bytes(rules_bytes)
        if args.command == "generate":
            catalogue = _read_catalogue(args.catalogue)
            document = generate(original, inventory, catalogue["identity"], json.loads(rules_bytes),
                                catalogue_id=catalogue["denominator_id"], rules_sha256=rules_sha,
                                source_root=args.root)
            rom_map = validate(document, original, inventory, json.loads(rules_bytes),
                               rules_sha256=rules_sha, source_root=args.root)
            check_catalogue(rom_map, catalogue)
            text = "{\n" + ",\n".join(f'  {json.dumps(k)}: {json.dumps(v)}' for k, v in document.items() if k != "ranges")
            text += ',\n  "ranges": [\n' + ",\n".join("    " + json.dumps(r) for r in document["ranges"]) + "\n  ]\n}\n"
            args.output.write_text(text)
            print(json.dumps({"map_id": rom_map.map_id, "ranges": len(rom_map.ranges), "output": str(args.output),
                              "partition_sha256": rom_map.partition_sha256,
                              "evidence_policy_sha256": rom_map.evidence_policy_sha256}))
            return
        rom_map = load(args.root, original, inventory)
        if args.full:
            if args.catalogue is None:
                parser.error("--full needs --catalogue")
            catalogue = _read_catalogue(args.catalogue)
            check_catalogue(rom_map, catalogue)
            regenerated = generate(original, inventory, catalogue["identity"], json.loads(rules_bytes),
                                   catalogue_id=catalogue["denominator_id"], rules_sha256=rules_sha,
                                   source_root=args.root)
            if regenerated["map_id"] != rom_map.map_id:
                raise MapError("Regenerated map differs from the frozen map")
        totals: dict[str, int] = {}
        for start, end, cls, _ in rom_map.ranges:
            totals[cls] = totals.get(cls, 0) + end - start
        print(json.dumps({"status": "PASS", "map_id": rom_map.map_id, "ranges": len(rom_map.ranges),
                          "full": bool(args.full), "class_bytes": dict(sorted(totals.items()))}, indent=2))
    except (MapError, ValueError, OSError) as error:
        parser.exit(1, f"{error}\n")


if __name__ == "__main__":
    main()
