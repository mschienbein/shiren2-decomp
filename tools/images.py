"""Reviewed fixed-target image ownership, independent of provisional sections.

This v1 inventory freezes the four loader-backed images reviewed in LAYOUT_PLAN.
It cannot assert whole-game completeness. Extending the reviewed tranche requires
an explicit policy revision as well as new original-loader evidence. These pure
helpers validate the declarations; independent ROM reads establish their digests.
"""
from __future__ import annotations

import json
import re
from dataclasses import dataclass
from pathlib import Path, PurePosixPath
from types import MappingProxyType
from typing import Mapping

from evidence import COMPILED_SUFFIXES
from rom import EXPECTED_SHA256, EXPECTED_SIZE


ADDRESS_LIMIT = 0x100000000
SOURCE_SUFFIXES = tuple(COMPILED_SUFFIXES.values())  # C and C++ translation units
_IDENTIFIER = re.compile(r"[A-Za-z_][A-Za-z0-9_]*\Z")
_SHA256 = re.compile(r"[0-9a-f]{64}\Z")
_REVIEWED_IMAGES = {
    "resident": (0x1000, 0x14400, 0x80025C00, 0x80039000),
    "main_14400": (0x14400, 0x1339F0, 0x800413C0, 0x801609B0),
    "overlay_1339f0": (0x1339F0, 0x136DC0, 0x801E4EA0, 0x801E8270),
    "overlay_136dc0": (0x136DC0, 0x148460, 0x801E4EA0, 0x801F6540),
}
_REVIEWED_BSS = {
    "resident": ((0x80039000, 0x800413C0),),
    "main_14400": ((0x801609B0, 0x801E4EA0),),
    "overlay_1339f0": (),
    "overlay_136dc0": (),
}


@dataclass(frozen=True)
class EvidenceRecord:
    evidence_id: str
    rom_start: int
    rom_end: int
    sha256: str
    confidence: str
    reference: str
    claim: str


@dataclass(frozen=True)
class BssRange:
    vram_start: int
    vram_end: int
    evidence: tuple[str, ...]


@dataclass(frozen=True)
class ProvisionalCpuRange:
    rom_start: int
    rom_end: int
    confidence: str
    evidence: tuple[str, ...]
    note: str


@dataclass(frozen=True)
class ImageRecord:
    image_id: str
    rom_start: int
    rom_end: int
    vram_start: int
    vram_end: int
    rom_sha256: str
    storage_format: str
    load_slot: str
    lifetime: str
    confidence: str
    evidence: tuple[str, ...]
    bss_status: str
    bss_ranges: tuple[BssRange, ...]
    splat_segments: tuple[str, ...]
    split_status: str
    provisional_cpu_ranges: tuple[ProvisionalCpuRange, ...]
    unknowns: tuple[str, ...]

    @property
    def size(self) -> int:
        return self.rom_end - self.rom_start

    @property
    def vram_rom_delta(self) -> int:
        return self.vram_start - self.rom_start


@dataclass(frozen=True)
class ComponentRecord:
    component_id: str
    parent_image_id: str
    kind: str
    rom_start: int
    rom_end: int
    storage_vram_start: int
    storage_vram_end: int
    execution_start: int
    execution_end: int
    rom_sha256: str
    cpu_c_excluded: bool
    confidence: str
    evidence: tuple[str, ...]
    note: str


@dataclass(frozen=True)
class LoadWindow:
    window_id: str
    rom_start: int
    rom_end: int
    vram_start: int
    vram_end: int
    purpose: str
    unique_image: bool
    evidence: tuple[str, ...]
    note: str


@dataclass(frozen=True)
class UnknownRomRange:
    rom_start: int
    rom_end: int
    note: str


@dataclass(frozen=True)
class ImageInventory:
    target_sha256: str
    target_size: int
    inventory_complete: bool
    cpu_inventory_complete: bool
    code_denominator_bytes: int | None
    scope: str
    images: Mapping[str, ImageRecord]
    components: tuple[ComponentRecord, ...]
    load_windows: tuple[LoadWindow, ...]
    unknown_rom_ranges: tuple[UnknownRomRange, ...]
    source_bindings: Mapping[str, str]
    evidence: Mapping[str, EvidenceRecord]


def _record(value: object, label: str, fields: set[str]) -> dict[str, object]:
    if not isinstance(value, dict) or any(not isinstance(key, str) for key in value):
        raise ValueError(f"{label} must be a dictionary with string fields")
    missing, extra = fields - value.keys(), value.keys() - fields
    if missing or extra:
        raise ValueError(f"{label} fields disagree: missing={sorted(missing)}, extra={sorted(extra)}")
    return value


def _items(value: object, label: str) -> list[object]:
    if not isinstance(value, list):
        raise ValueError(f"{label} must be a list")
    return value


def _text(value: object, label: str) -> str:
    if not isinstance(value, str) or not value.strip() or "\x00" in value:
        raise ValueError(f"{label} must be nonempty text without NUL")
    return value


def _identity(value: object, label: str) -> str:
    if not isinstance(value, str) or _IDENTIFIER.fullmatch(value) is None:
        raise ValueError(f"{label} must be a plain identifier without path escapes")
    return value


def _integer(value: object, label: str) -> int:
    if type(value) is not int:
        raise ValueError(f"{label} must be an integer, not a boolean or other type")
    return value


def _digest(value: object, label: str) -> str:
    if not isinstance(value, str) or _SHA256.fullmatch(value) is None:
        raise ValueError(f"{label} must be a lowercase SHA-256 digest")
    return value


def _path(value: object, label: str, *, root: str, suffix: str | tuple[str, ...]) -> str:
    text = _text(value, label)
    path = PurePosixPath(text)
    suffixes = (suffix,) if isinstance(suffix, str) else suffix
    if ("\\" in text or path.is_absolute() or ".." in path.parts
            or path.as_posix() != text or len(path.parts) < 2
            or path.parts[0] != root or path.suffix not in suffixes):
        raise ValueError(f"{label} must be a canonical {root}/ path without escapes, ending in {' or '.join(suffixes)}")
    return text


def _interval(start: object, end: object, label: str, limit: int) -> tuple[int, int]:
    first, last = _integer(start, f"{label} start"), _integer(end, f"{label} end")
    if not (0 <= first < last <= limit) or first % 4 or last % 4:
        raise ValueError(f"{label} must be a positive, word-aligned interval within bounds")
    return first, last


def _strings(value: object, label: str, *, nonempty: bool = True) -> tuple[str, ...]:
    result = tuple(_text(item, label) for item in _items(value, label))
    if nonempty and not result:
        raise ValueError(f"{label} must not be empty")
    return result


def _identities(value: object, label: str, *, nonempty: bool = True) -> tuple[str, ...]:
    result = tuple(_identity(item, label) for item in _items(value, label))
    if (nonempty and not result) or len(set(result)) != len(result):
        raise ValueError(f"{label} must contain distinct identifiers" + (" and must not be empty" if nonempty else ""))
    return result


def _references(value: object, label: str, evidence: Mapping[str, EvidenceRecord]) -> tuple[str, ...]:
    result = _identities(value, label)
    missing = set(result) - evidence.keys()
    if missing:
        raise ValueError(f"{label} references unknown loader evidence: {sorted(missing)}")
    return result


def _overlap(first: tuple[int, int], second: tuple[int, int]) -> bool:
    return first[0] < second[1] and second[0] < first[1]


def _no_overlap(intervals: list[tuple[int, int, str]], label: str) -> None:
    previous_end, previous_name = -1, ""
    for start, end, name in sorted(intervals):
        if start < previous_end:
            raise ValueError(f"Overlapping {label}: {previous_name} and {name}")
        previous_end, previous_name = end, name


def _parse_evidence(value: object) -> dict[str, EvidenceRecord]:
    result: dict[str, EvidenceRecord] = {}
    for index, item in enumerate(_items(value, "Evidence")):
        label = f"Evidence {index}"
        record = _record(item, label, {"evidence_id", "rom_start", "rom_end", "sha256", "confidence", "reference", "claim"})
        name = _identity(record["evidence_id"], label)
        if name in result:
            raise ValueError(f"Duplicate loader evidence: {name}")
        start, end = _interval(record["rom_start"], record["rom_end"], label, EXPECTED_SIZE)
        if record["confidence"] != "reviewed":
            raise ValueError(f"{label} must preserve reviewed evidence confidence")
        result[name] = EvidenceRecord(
            name, start, end, _digest(record["sha256"], label), "reviewed",
            _path(record["reference"], label, root="docs", suffix=".md"),
            _text(record["claim"], label),
        )
    if not result:
        raise ValueError("Loader evidence must not be empty")
    return result


def _parse_image(value: object, evidence: Mapping[str, EvidenceRecord]) -> ImageRecord:
    record = _record(value, "Image", {
        "image_id", "rom_start", "rom_end", "vram_start", "vram_end", "rom_sha256",
        "storage_format", "load_slot", "lifetime", "confidence", "evidence",
        "bss_status", "bss_ranges", "splat_segments", "split_status",
        "provisional_cpu_ranges", "unknowns",
    })
    name = _identity(record["image_id"], "Image identity")
    label = f"Image {name}"
    start, end = _interval(record["rom_start"], record["rom_end"], f"{label} ROM", EXPECTED_SIZE)
    vstart, vend = _interval(record["vram_start"], record["vram_end"], f"{label} VRAM", ADDRESS_LIMIT)
    if record["storage_format"] != "uncompressed-linear" or end - start != vend - vstart:
        raise ValueError(f"{label} must have a byte-linear initialized mapping")
    if record["confidence"] != "reviewed-initialized-extent":
        raise ValueError(f"{label} initialized extent must remain reviewed")
    bss = []
    for item in _items(record["bss_ranges"], f"{label} BSS"):
        part = _record(item, f"{label} BSS", {"vram_start", "vram_end", "evidence"})
        first, last = _interval(part["vram_start"], part["vram_end"], f"{label} BSS", ADDRESS_LIMIT)
        bss.append(BssRange(first, last, _references(part["evidence"], f"{label} BSS", evidence)))
    _no_overlap([(vstart, vend, "initialized")] + [(part.vram_start, part.vram_end, "BSS") for part in bss], f"{label} initialized/BSS RAM")
    bss_status = record["bss_status"]
    if not isinstance(bss_status, str) or bss_status not in {"reviewed", "unknown"} or (bss_status == "reviewed") != bool(bss):
        raise ValueError(f"{label} BSS status disagrees with its separate RAM ranges")
    segments = _identities(record["splat_segments"], f"{label} splat segments", nonempty=False)
    status = record["split_status"]
    if not isinstance(status, str) or status not in {"active", "planned", "opaque"} or (status == "opaque") == bool(segments):
        raise ValueError(f"{label} split status disagrees with its segment bindings")
    cpu = []
    for item in _items(record["provisional_cpu_ranges"], f"{label} CPU hints"):
        part = _record(item, f"{label} CPU hint", {"rom_start", "rom_end", "confidence", "evidence", "note"})
        first, last = _interval(part["rom_start"], part["rom_end"], f"{label} CPU hint", EXPECTED_SIZE)
        if not start <= first < last <= end or part["confidence"] != "provisional":
            raise ValueError(f"{label} CPU hint must be inside its image and remain provisional")
        cpu.append(ProvisionalCpuRange(first, last, "provisional", _references(part["evidence"], f"{label} CPU hint", evidence), _text(part["note"], label)))
    _no_overlap([(part.rom_start, part.rom_end, name) for part in cpu], f"{label} provisional CPU ranges")
    return ImageRecord(
        name, start, end, vstart, vend, _digest(record["rom_sha256"], label),
        "uncompressed-linear", _identity(record["load_slot"], f"{label} load slot"),
        _text(record["lifetime"], label), "reviewed-initialized-extent",
        _references(record["evidence"], label, evidence), bss_status, tuple(bss),
        segments, status, tuple(cpu), _strings(record["unknowns"], f"{label} unknowns"),
    )


def _parse_component(value: object, images: Mapping[str, ImageRecord], evidence: Mapping[str, EvidenceRecord]) -> ComponentRecord:
    record = _record(value, "Component", {
        "component_id", "parent_image_id", "kind", "rom_start", "rom_end",
        "storage_vram_start", "storage_vram_end", "execution_start", "execution_end",
        "rom_sha256", "cpu_c_excluded", "confidence", "evidence", "note",
    })
    name = _identity(record["component_id"], "Component identity")
    parent_id = _identity(record["parent_image_id"], f"Component {name} parent")
    parent = images.get(parent_id)
    if parent is None:
        raise ValueError(f"Component {name} has an unknown parent image")
    start, end = _interval(record["rom_start"], record["rom_end"], f"Component {name} ROM", EXPECTED_SIZE)
    vstart, vend = _interval(record["storage_vram_start"], record["storage_vram_end"], f"Component {name} storage RAM", ADDRESS_LIMIT)
    estart, eend = _interval(record["execution_start"], record["execution_end"], f"Component {name} execution RAM", ADDRESS_LIMIT)
    if not parent.rom_start <= start < end <= parent.rom_end:
        raise ValueError(f"Component {name} exceeds its parent image storage")
    if (vstart, vend) != (start + parent.vram_rom_delta, end + parent.vram_rom_delta) or eend - estart != end - start:
        raise ValueError(f"Component {name} has an inconsistent storage/execution mapping")
    if record["kind"] != "rsp" or record["cpu_c_excluded"] is not True or record["confidence"] != "reviewed-component-extent":
        raise ValueError(f"Component {name} must preserve the confirmed RSP CPU-C exclusion")
    return ComponentRecord(
        name, parent_id, "rsp", start, end, vstart, vend, estart, eend,
        _digest(record["rom_sha256"], name), True, "reviewed-component-extent",
        _references(record["evidence"], f"Component {name}", evidence), _text(record["note"], name),
    )


def _parse_window(value: object, evidence: Mapping[str, EvidenceRecord]) -> LoadWindow:
    record = _record(value, "Load window", {
        "window_id", "rom_start", "rom_end", "vram_start", "vram_end", "purpose",
        "unique_image", "evidence", "note",
    })
    name = _identity(record["window_id"], "Load window identity")
    start, end = _interval(record["rom_start"], record["rom_end"], f"Window {name} ROM", EXPECTED_SIZE)
    vstart, vend = _interval(record["vram_start"], record["vram_end"], f"Window {name} VRAM", ADDRESS_LIMIT)
    if end - start != vend - vstart or record["purpose"] != "temporary-boot-copy" or record["unique_image"] is not False:
        raise ValueError(f"Window {name} must remain a temporary copy, separate from image ownership")
    return LoadWindow(name, start, end, vstart, vend, "temporary-boot-copy", False, _references(record["evidence"], name, evidence), _text(record["note"], name))


def validate_inventory(document: object) -> ImageInventory:
    """Parse v1 declarations and reject changed target, reviewed mappings or ownership.

    Loader digest values have a validated shape here, not independent ROM proof.
    All intervals are exclusive-ended. Only explicitly shared load slots permit
    initialized/BSS VRAM overlap; original image ROM storage never overlaps.
    """
    record = _record(document, "Image inventory", {
        "schema_version", "kind", "target_sha256", "target_size", "inventory_complete",
        "cpu_inventory_complete", "code_denominator_bytes", "scope", "images",
        "components", "load_windows", "unknown_rom_ranges", "source_bindings", "evidence",
    })
    if type(record["schema_version"]) is not int or record["schema_version"] != 1 or record["kind"] != "shiren2-image-inventory":
        raise ValueError("Unsupported image inventory schema")
    if record["target_sha256"] != EXPECTED_SHA256 or _integer(record["target_size"], "Target size") != EXPECTED_SIZE:
        raise ValueError("Image inventory has the wrong fixed ROM target")
    if (record["inventory_complete"] is not False or record["cpu_inventory_complete"] is not False
            or record["code_denominator_bytes"] is not None):
        raise ValueError("This reviewed tranche is incomplete; a full-game code denominator is unknown")
    evidence = _parse_evidence(record["evidence"])
    images: dict[str, ImageRecord] = {}
    segments: set[str] = set()
    for value in _items(record["images"], "Images"):
        image = _parse_image(value, evidence)
        if image.image_id in images:
            raise ValueError(f"Duplicate original image identity: {image.image_id}")
        if segments.intersection(image.splat_segments):
            raise ValueError(f"Ambiguous splat segment image binding: {image.image_id}")
        images[image.image_id] = image
        segments.update(image.splat_segments)
    _no_overlap([(part.rom_start, part.rom_end, name) for name, part in images.items()], "original ROM image storage")
    ordered = list(images.values())
    for index, first in enumerate(ordered):
        first_ram = [(first.vram_start, first.vram_end)] + [(part.vram_start, part.vram_end) for part in first.bss_ranges]
        for second in ordered[index + 1:]:
            second_ram = [(second.vram_start, second.vram_end)] + [(part.vram_start, part.vram_end) for part in second.bss_ranges]
            if any(_overlap(a, b) for a in first_ram for b in second_ram):
                if first.load_slot != second.load_slot or first.vram_start != second.vram_start:
                    raise ValueError(f"Overlapping VRAM requires an explicit shared load slot: {first.image_id}/{second.image_id}")
    if images.keys() != _REVIEWED_IMAGES.keys():
        raise ValueError("The initial reviewed image inventory must contain exactly the four known image identities")
    for name, image in images.items():
        if (image.rom_start, image.rom_end, image.vram_start, image.vram_end) != _REVIEWED_IMAGES[name]:
            raise ValueError(f"Image {name} disagrees with its reviewed fixed-target loader mapping")
        if tuple((part.vram_start, part.vram_end) for part in image.bss_ranges) != _REVIEWED_BSS[name]:
            raise ValueError(f"Image {name} disagrees with its reviewed BSS mapping")
    if images["resident"].splat_segments != ("entry", "main") or images["main_14400"].splat_segments != ("main_14400",):
        raise ValueError("Resident/main splat bindings disagree with the reviewed segment identities")
    if images["overlay_1339f0"].split_status != "opaque":
        raise ValueError("Overlay split/linkage is unsupported for overlay_1339f0")
    overlay_b = images["overlay_136dc0"]
    if overlay_b.split_status != "opaque" and (
            overlay_b.split_status != "active"
            or overlay_b.splat_segments != ("overlay_b_136dc0_storage",)):
        raise ValueError("Only the reviewed storage-only overlay B split/linkage is supported")
    components = tuple(_parse_component(value, images, evidence) for value in _items(record["components"], "Components"))
    _no_overlap([(part.rom_start, part.rom_end, part.component_id) for part in components], "component ROM storage")
    if (len(components) != 1 or components[0].component_id != "rsp_boot" or components[0].parent_image_id != "main_14400"
            or (components[0].rom_start, components[0].rom_end, components[0].execution_start, components[0].execution_end)
            != (0x109A50, 0x109B20, 0x04001000, 0x040010D0)):
        raise ValueError("Confirmed RSP bootstrap identity and extents must be preserved")
    for image in images.values():
        for hint in image.provisional_cpu_ranges:
            if any(part.parent_image_id == image.image_id and _overlap((hint.rom_start, hint.rom_end), (part.rom_start, part.rom_end)) for part in components):
                raise ValueError(f"Provisional CPU hint includes a confirmed RSP exclusion: {image.image_id}")
    windows = tuple(_parse_window(value, evidence) for value in _items(record["load_windows"], "Load windows"))
    if (len(windows) != 1 or windows[0].window_id != "ipl3_boot_window"
            or (windows[0].rom_start, windows[0].rom_end, windows[0].vram_start, windows[0].vram_end)
            != (0x1000, 0x101000, 0x80025C00, 0x80125C00)):
        raise ValueError("The reviewed temporary IPL3 transfer window must be preserved")
    unknown = []
    for value in _items(record["unknown_rom_ranges"], "Unknown ROM ranges"):
        part = _record(value, "Unknown ROM range", {"rom_start", "rom_end", "note"})
        start, end = _interval(part["rom_start"], part["rom_end"], "Unknown ROM range", EXPECTED_SIZE)
        unknown.append(UnknownRomRange(start, end, _text(part["note"], "Unknown ROM range")))
    _no_overlap([(part.rom_start, part.rom_end, "unknown") for part in unknown] + [(part.rom_start, part.rom_end, part.image_id) for part in images.values()], "known/unknown ROM ownership")
    if len(unknown) != 1 or (unknown[0].rom_start, unknown[0].rom_end) != (0x148460, EXPECTED_SIZE):
        raise ValueError("The stored tail after 0x148460 must remain explicitly unknown")
    sources = record["source_bindings"]
    if not isinstance(sources, dict):
        raise ValueError("Source bindings must be a dictionary")
    bindings: dict[str, str] = {}
    stems: set[str] = set()
    for source, image_id in sources.items():
        path = _path(source, "Source binding", root="src", suffix=SOURCE_SUFFIXES)
        stem = PurePosixPath(path).with_suffix("").as_posix()
        if stem in stems:
            raise ValueError(f"Source bindings share one object path: {stem}.c/.cpp")
        stems.add(stem)
        name = _identity(image_id, "Source binding image")
        if name not in images:
            raise ValueError(f"Source binding references unknown image: {name}")
        if name in {"overlay_1339f0", "overlay_136dc0"}:
            raise ValueError("Overlay source bindings are unsupported by the storage-only B path")
        bindings[path] = name
    return ImageInventory(
        EXPECTED_SHA256, EXPECTED_SIZE, False, False, None, _text(record["scope"], "Inventory scope"),
        MappingProxyType(images), components, windows, tuple(unknown),
        MappingProxyType(bindings), MappingProxyType(evidence),
    )


def _unique_object(pairs: list[tuple[str, object]]) -> dict[str, object]:
    result: dict[str, object] = {}
    for name, value in pairs:
        if name in result:
            raise ValueError(f"Duplicate JSON field in image inventory: {name}")
        result[name] = value
    return result


def load_inventory(path: Path) -> ImageInventory:
    """Load an explicit JSON path with duplicate-key rejection; no ROM or writes."""
    return validate_inventory(json.loads(path.read_text(encoding="utf-8"), object_pairs_hook=_unique_object))


def image_for_rom(offset: int, inventory: ImageInventory) -> ImageRecord | None:
    """Return unique initialized ownership, including components, or None for gaps."""
    address = _integer(offset, "ROM offset")
    if not 0 <= address < inventory.target_size:
        raise ValueError("ROM offset is outside the fixed target")
    return next((image for image in inventory.images.values() if image.rom_start <= address < image.rom_end), None)


def policy_records(inventory: ImageInventory) -> dict[str, dict[str, object]]:
    """Return initialized extents for evidence.validate_matches, never a text map.

    Components and temporary windows do not create duplicate ownership. Call
    require_cpu_range separately to enforce confirmed non-CPU exclusions.
    """
    return {
        name: {"rom_start": image.rom_start, "rom_end": image.rom_end,
               "vram_start": image.vram_start, "vram_end": image.vram_end}
        for name, image in inventory.images.items()
    }


def image_for_segment(name: str, inventory: ImageInventory) -> ImageRecord | None:
    """Resolve reviewed active/planned splat names; opaque overlays have no binding."""
    segment = _identity(name, "Splat segment")
    return next((image for image in inventory.images.values() if segment in image.splat_segments), None)


def image_for_source(source: str, inventory: ImageInventory) -> ImageRecord | None:
    """Resolve an explicit canonical C/C++ source binding; never infer it from VRAM."""
    path = _path(source, "C source", root="src", suffix=SOURCE_SUFFIXES)
    name = inventory.source_bindings.get(path)
    return None if name is None else inventory.images[name]


def require_cpu_range(image_id: str, rom_start: int, size: int, inventory: ImageInventory) -> ImageRecord:
    """Require image storage and avoid authoritative CPU-C exclusions.

    This is necessary policy, not proof that a range is CPU text or produced by
    a C compiler. Boundary/reference review remains required. Provisional CPU
    hints never become final section limits or a denominator through this helper.
    """
    name = _identity(image_id, "CPU range image")
    image = inventory.images.get(name)
    if image is None:
        raise ValueError(f"Unknown image for CPU range: {name}")
    start, count = _integer(rom_start, "CPU range ROM start"), _integer(size, "CPU range size")
    first, last = _interval(start, start + count, "CPU range ROM", inventory.target_size)
    if not image.rom_start <= first < last <= image.rom_end:
        raise ValueError(f"CPU range exceeds its original image: {name}")
    for component in inventory.components:
        if component.parent_image_id == name and component.cpu_c_excluded and _overlap((first, last), (component.rom_start, component.rom_end)):
            raise ValueError(f"CPU range includes confirmed non-CPU component: {name}/{component.component_id}")
    return image
