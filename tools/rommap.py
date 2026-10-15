#!/usr/bin/env python3
"""Frozen whole-ROM classification map: every original byte in exactly one classified range.

`config/rom_map.json` is generated from the fixed original ROM, `config/images.json`
(images and RSP components), the frozen CPU function catalogue and the reviewed rules in
`config/rom_regions.json`. It is frozen: matching work never changes it. Range classes say
what the bytes are; their matching status is derived per accepted receipt by `account`.
Only `cpu_text_function` ranges can carry C/C++ credit, and only through exact receipt matches.

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
from dataclasses import dataclass
from pathlib import Path
from typing import Any, Iterable, Mapping

import yaml
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
EVIDENCE_SHA256 = "1a5c024929b3ce2d9d2eb057cc22a37d4d409f7ec51cf49225ac53cc7834acd1"

# Class -> accounting group. Groups have their own units in `account`.
CLASSES: Mapping[str, str] = {
    "rom_header": "boot", "ipl3_bootcode": "boot",
    "cpu_text_function": "cpu_code",
    "cpu_data": "cpu_data", "image_opaque": "cpu_image_opaque",
    "cpu_text_unowned": "cpu_image_opaque",
    "rsp_boot": "rsp_ucode", "rsp_ucode_text": "rsp_ucode", "rsp_ucode_data": "rsp_ucode",
    "rsp_ucode_unresolved": "rsp_ucode",
    "asset_yay0": "asset_compressed",
    "asset_audio_bank_ptrtable": "asset_raw", "asset_audio_wavetable": "asset_raw",
    "asset_hvqm2_video": "asset_raw", "asset_demo_input": "asset_raw",
    "asset_raw_referenced": "asset_raw", "asset_raw_code_based": "asset_raw",
    "asset_iff_container_meta": "asset_raw", "asset_iff_payload": "asset_raw", "asset_raw_unattributed": "asset_raw",
    "pad_align_zero": "padding", "pad_align_ff": "padding", "rom_padding_zero": "padding",
}
GROUP_UNITS: Mapping[str, str] = {
    "boot": "bytes", "cpu_code": "instruction bytes", "cpu_data": "bytes", "cpu_image_opaque": "bytes",
    "rsp_ucode": "bytes", "asset_compressed": "stream bytes", "asset_raw": "bytes", "padding": "bytes",
}
BUILD_STATUSES = frozenset({"matched_c", "matched_cpp", "matched_toolchain_library", "owned_c", "roundtrip"})
RESEARCH_STATUSES = frozenset({"classified_binary", "classified_handwritten", "closed"})
OPEN_STATUSES = frozenset({"open", "located"})
_ZERO_CLASSES = frozenset({"pad_align_zero", "rom_padding_zero"})
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


def load_rules(document: object) -> dict[str, Any]:
    rules = _record(document, "Region rules", {
        "schema_version", "kind", "target_sha256", "target_size", "ipl3_md5", "tail_start", "content_end",
        "tables", "chains", "immediate_windows", "inferred_windows", "code_bases", "iff_archive"})
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


def _cpu_text_pieces(start: int, end: int, functions: list[tuple[int, int]]) -> Iterable[tuple[int, int, str, str]]:
    """Partition a CPU range without mistaking uncovered bytes for proved padding.

    No producer-padding records are currently bound for these gaps. Zero content
    alone is not ownership evidence; both inter-function gaps and tails stay open.
    """
    cursor = start
    for f_start, f_end in sorted(functions):
        if not start <= f_start < f_end <= end:
            continue
        if f_start > cursor:
            yield cursor, f_start, "cpu_text_unowned", "catalogue"
        yield f_start, f_end, "cpu_text_function", "catalogue"
        cursor = f_end
    if cursor < end:
        yield cursor, end, "cpu_text_unowned", "catalogue"

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
    attest(original)
    rules = load_rules(rules)
    rsp_components = _rsp_components(original, inventory, source_root)
    rows: list[list[Any]] = []

    def add(start: int, end: int, cls: str, ref: str) -> None:
        if end <= start:
            raise MapError(f"Empty range at {start:#x}")
        # Catalogue completeness is separate from the ownership of zero gaps.
        if cls == "cpu_text_unowned" and not _zero(original, start, end):
            raise MapError(f"Nonzero uncatalogued CPU bytes at {start:#x}..{end:#x}")
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
    for image in images:
        position = image.rom_start
        pieces: list[tuple[int, int, str, str]] = []
        for start, end in cpu_ranges.get(image.image_id, []):
            pieces.extend(_cpu_text_pieces(start, end, functions.get(image.image_id, [])))
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
    # The audio boundary word has unresolved ownership; never infer padding from zero.
    boundary = 0x11D770
    for index, row in enumerate(rows):
        if row[0] <= boundary < boundary + 4 <= row[1] and row[2] == "cpu_data":
            a, b, cls, ref = row
            rows[index:index + 1] = ([[a, boundary, cls, ref]] if a < boundary else []) + [
                [boundary, boundary + 4, "rsp_ucode_unresolved", "rsp_audio_boundary"]
            ] + ([[boundary + 4, b, cls, ref]] if boundary + 4 < b else [])
            break
    _generate_tail(original, rules, add, source_root)
    rows.sort(key=lambda row: row[0])
    return _document(original, rows, rules_sha256=rules_sha256, catalogue_id=catalogue_id, catalogue=catalogue)


def _generate_tail(original: bytes, rules: Mapping[str, Any], add, source_root: Path = PROJECT) -> None:
    tail, content_end = rules["tail_start"], rules["content_end"]
    if any(original[content_end:]):
        raise MapError("content_end must start the final all-zero fill")
    windows = rule_windows(rules, original, source_root)
    evidence = load_evidence(original, rules, source_root)
    palettes = _sprite_palette_ranges(original, evidence["sprite-palettes.json"])
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
            add(a, b, _window_class(original, window), window.ref)
        elif first_base <= a < last_base_end:
            base = max(c for c in bases if c <= a)
            add(a, b, "asset_raw_code_based", bases[base])
        elif any(start + 12 <= a < b <= end for start, end in forms):
            add(a, b, "asset_iff_payload", iff["id"])
        elif (a, b) in palettes:
            add(a, b, "asset_raw_referenced", "sprite_palettes")
            palettes.remove((a, b))
        else:
            add(a, b, "asset_raw_unattributed", "unattributed")
    if palettes:
        raise MapError("Palette evidence must name unchanged whole unattributed intervals")


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


def _content_digest(original: bytes, rows: Iterable[list[Any]]) -> str:
    digest = hashlib.sha256()
    for start, end, *_ in rows:
        digest.update(hashlib.sha256(original[start:end]).digest())
    return digest.hexdigest()


def _document(original: bytes, rows: list[list[Any]], *, rules_sha256: str, catalogue_id: str,
              catalogue: Mapping[str, Any]) -> dict[str, Any]:
    content = _content_digest(original, rows)
    partition = json_digest({"target_sha256": EXPECTED_SHA256,
                             "ranges": [[row[0], row[1]] for row in rows], "content_sha256": content})
    policy = json_digest({"evidence_sha256": EVIDENCE_SHA256, "rules_sha256": rules_sha256,
                          "classes": dict(CLASSES), "range_classes": [row[2:] for row in rows],
                          "research_statuses": {cls: research_status(cls) for cls in CLASSES},
                          "audio_validator_sha256": sha256_bytes(Path(rspaudio.__file__).read_bytes()),
                          "library_policy_sha256": sha256_bytes(Path(toolchain_library.__file__).read_bytes()),
                          "validator_sha256": sha256_bytes(Path(__file__).read_bytes())})
    identity = {"target_sha256": EXPECTED_SHA256, "target_size": EXPECTED_SIZE,
                "cpu_catalogue_id": catalogue_id, "partition_sha256": partition,
                "evidence_policy_sha256": policy}
    return {"schema_version": 2, "kind": MAP_KIND, "map_id": json_digest(identity), **identity,
            "rules_sha256": rules_sha256, "ranges": rows, "content_sha256": content,
            "cpu_catalogue": catalogue, "range_count": len(rows)}


# ---------------------------------------------------------------------------- validation


@dataclass(frozen=True)
class RomMap:
    map_id: str
    rules_sha256: str
    cpu_catalogue_id: str
    ranges: tuple[tuple[int, int, str, str], ...]
    partition_sha256: str
    evidence_policy_sha256: str

    def of_class(self, *classes: str) -> list[tuple[int, int, str, str]]:
        return [r for r in self.ranges if r[2] in classes]


def validate(document: object, original: bytes, inventory: ImageInventory, rules_document: object,
             *, rules_sha256: str, source_root: Path = PROJECT) -> RomMap:
    """Structural, hash and evidence checks; cheap enough for every build and verification."""
    attest(original)
    record = _record(document, "ROM map", {"schema_version", "kind", "map_id", "target_sha256", "target_size",
                                           "rules_sha256", "cpu_catalogue_id", "cpu_catalogue", "ranges", "content_sha256", "range_count",
                                           "partition_sha256", "evidence_policy_sha256"})
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
    refs = {"boot", "catalogue", "align", "archive", "unattributed", "rsp_audio_boundary", *inventory.images,
            *(c.component_id for c in inventory.components)}
    for key in ("tables", "chains", "immediate_windows", "inferred_windows", "code_bases"):
        refs.update(row["id"] for row in rules[key])
    refs.add(rules["iff_archive"]["id"])
    refs.add("sprite_palettes")
    position = 0
    ranges = []
    for row in rows:
        if (not isinstance(row, list) or len(row) != 4 or type(row[0]) is not int or type(row[1]) is not int
                or row[2] not in CLASSES or row[3] not in refs):
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
    components.add((0x11D770, 0x11D774, "rsp_ucode_unresolved", "rsp_audio_boundary"))
    if {r for r in ranges if CLASSES[r[2]] == "rsp_ucode"} != components:
        raise MapError("RSP ranges disagree with reviewed images.json components")
    for image in inventory.images.values():
        inside = [r for r in ranges if image.rom_start <= r[0] < image.rom_end]
        if not inside or inside[0][0] != image.rom_start or inside[-1][1] != image.rom_end:
            raise MapError(f"ROM map ranges straddle image {image.image_id}")
        hints = [(h.rom_start, h.rom_end) for h in image.provisional_cpu_ranges]
        for start, end, cls, _ in inside:
            in_cpu = any(a <= start and end <= b for a, b in hints)
            if cls in ("cpu_text_function", "cpu_text_unowned") and not in_cpu:
                raise MapError(f"CPU text range {start:#x} lies outside reviewed CPU ranges")
            if cls in ("cpu_data", "image_opaque") and any(start < b and a < end for a, b in hints):
                raise MapError(f"Data range {start:#x} overlaps a reviewed CPU range")
            if cls == "image_opaque" and hints:
                raise MapError(f"Split image {image.image_id} still has opaque storage")
        for a, b in hints:
            covered = [r for r in inside if a <= r[0] and r[1] <= b]
            if sum(r[1] - r[0] for r in covered) != b - a or any(r[2] not in ("cpu_text_function", "cpu_text_unowned") for r in covered):
                raise MapError(f"Reviewed CPU range {a:#x} is not tiled by function/unowned ranges")
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
    regenerated = generate(original, inventory, record["cpu_catalogue"], rules,
                           catalogue_id=record["cpu_catalogue_id"], rules_sha256=rules_sha256,
                           source_root=source_root)
    if regenerated["ranges"] != rows:
        raise MapError("ROM map classifications disagree with replayed range properties")
    return rom_map


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


def research_status(cls: str) -> str:
    """Research disposition after `validate`; never consult receipt/status assertions."""
    if cls not in CLASSES:
        raise MapError(f"Unsupported range class: {cls}")
    if cls in {"cpu_text_function", "cpu_text_unowned", "cpu_data", "image_opaque", "rsp_ucode_unresolved", "asset_raw_unattributed"}:
        return "open"
    if CLASSES[cls] == "padding":
        return "closed"
    if cls == "asset_yay0" or cls == "asset_iff_container_meta" or CLASSES[cls] in {"boot", "rsp_ucode"}:
        return "classified_binary"
    return "located"


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
        if research_status(cls) in RESEARCH_STATUSES:
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
            credit(group, "open" if cls == "rsp_ucode_unresolved" else "classified_binary", size)
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
            credit(group, research_status(cls), size)
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
