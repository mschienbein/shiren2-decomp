"""Pure evidence policy; callers supply independently derived build inventories.

These helpers do not read files, inspect ELF/map data, or prove that C was compiled.
The live verifier must establish those facts before supplying attribution records.
Image records currently describe uncompressed, byte-linear ROM-to-VRAM mappings;
compressed images and intentional ROM aliases need a separate reviewed policy.
"""
from __future__ import annotations

import re
from dataclasses import dataclass
from pathlib import PurePosixPath


ROM_SIZE = 0x2000000
ADDRESS_LIMIT = 0x100000000
RESIDENT_IMAGE = {
    "rom_start": 0x1000,
    "rom_end": 0x12340,
    "vram_start": 0x80025C00,
}
SOURCE_KINDS = frozenset({"c", "cpp", "asm", "binary"})
# Compiled translation-unit kinds and their canonical source suffixes. A `.cpp` TU is
# compiled by the C++ front end (cc1plus) and exports only extern "C" names.
COMPILED_SUFFIXES = {"c": ".c", "cpp": ".cpp"}
COMPILED_KINDS = frozenset(COMPILED_SUFFIXES)
LANGUAGE_LABELS = {"c": "C", "cpp": "C++"}
_C_SYMBOL = re.compile(r"[A-Za-z_][A-Za-z0-9_]*\Z")
_MATCH_FIELDS = {"symbol", "source", "rom_start", "vram_start", "size"}
_ATTRIBUTION_FIELDS = {"symbol", "source", "object", "vram_start", "size"}


@dataclass(frozen=True)
class _Match:
    image_id: str
    symbol: str
    source: str
    rom_start: int
    vram_start: int
    size: int

    @property
    def key(self) -> tuple[str, str]:
        return self.image_id, self.symbol


@dataclass(frozen=True)
class _Image:
    rom_start: int
    rom_end: int
    vram_start: int
    vram_end: int


def require_inventory(actual: dict[str, object], expected_keys: set[str], label: str) -> None:
    """Require exactly the independent inventory's keys; values are checked elsewhere."""
    if not isinstance(actual, dict):
        raise ValueError(f"{label} inventory must be a dictionary")
    if not isinstance(expected_keys, (set, frozenset)):
        raise ValueError(f"{label} expected inventory must be a set")
    if any(not isinstance(key, str) or not key for key in actual):
        raise ValueError(f"{label} inventory keys must be nonempty strings")
    if any(not isinstance(key, str) or not key for key in expected_keys):
        raise ValueError(f"{label} expected inventory keys must be nonempty strings")
    missing = expected_keys - actual.keys()
    extra = actual.keys() - expected_keys
    if missing or extra:
        raise ValueError(
            f"{label} inventory disagrees with the declared inputs: "
            f"missing={sorted(missing)}, extra={sorted(extra)}"
        )


def _record(value: object, label: str, required: set[str]) -> dict[str, object]:
    if not isinstance(value, dict):
        raise ValueError(f"{label} must be a dictionary")
    if any(not isinstance(key, str) for key in value):
        raise ValueError(f"{label} field names must be strings")
    missing = required - value.keys()
    if missing:
        raise ValueError(f"{label} is missing fields: {sorted(missing)}")
    return value


def _integer(value: object, label: str) -> int:
    if type(value) is not int:
        raise ValueError(f"{label} must be an integer, not a boolean or other type")
    return value


def _identity(value: object, label: str) -> str:
    if not isinstance(value, str) or not value or "\x00" in value or any(c.isspace() for c in value):
        raise ValueError(f"{label} must be a nonempty identifier without whitespace")
    return value


def _symbol(value: object, label: str) -> str:
    if not isinstance(value, str) or _C_SYMBOL.fullmatch(value) is None:
        raise ValueError(f"{label} must be a C function identifier")
    return value


def _relative_path(value: object, label: str, *, root: str | None = None, suffix: str | tuple[str, ...] | None = None) -> str:
    if not isinstance(value, str) or not value or "\\" in value or "\x00" in value:
        raise ValueError(f"{label} must be a canonical relative path")
    path = PurePosixPath(value)
    if path.is_absolute() or ".." in path.parts or path.as_posix() != value or value == ".":
        raise ValueError(f"{label} must be a canonical relative path without escapes")
    if root is not None and (len(path.parts) < 2 or path.parts[0] != root):
        raise ValueError(f"{label} must be under {root}/")
    suffixes = (suffix,) if isinstance(suffix, str) else suffix
    if suffixes is not None and path.suffix not in suffixes:
        raise ValueError(f"{label} must have a {' or '.join(suffixes)} suffix")
    return value


def _source(value: object, label: str) -> str:
    return _relative_path(value, label, root="src", suffix=tuple(COMPILED_SUFFIXES.values()))


def compiled_kind(source: str) -> str:
    """Source kind (`c` or `cpp`) of a canonical compiled translation-unit path, by suffix."""
    suffix = PurePosixPath(source).suffix
    for kind, expected in COMPILED_SUFFIXES.items():
        if suffix == expected:
            return kind
    raise ValueError(f"Compiled translation unit must end in .c or .cpp: {source}")


def _extent(start: object, size: object, label: str, limit: int) -> tuple[int, int]:
    address = _integer(start, f"{label} start")
    count = _integer(size, f"{label} size")
    if address < 0 or count <= 0 or address % 4 or count % 4 or address + count > limit:
        raise ValueError(f"{label} must be a positive, word-aligned interval within bounds")
    return address, count


def _no_overlap(intervals: list[tuple[int, int, str]], label: str) -> None:
    previous_end = -1
    previous_name = ""
    for start, end, name in sorted(intervals):
        if start < previous_end:
            raise ValueError(f"Overlapping {label}: {previous_name} and {name}")
        previous_end, previous_name = end, name


def _matches(matches: list[dict[str, object]]) -> list[_Match]:
    if not isinstance(matches, list):
        raise ValueError("C matches must be a list")
    parsed: list[_Match] = []
    keys: set[tuple[str, str]] = set()
    for index, value in enumerate(matches):
        label = f"C match {index}"
        record = _record(value, label, _MATCH_FIELDS)
        image_id = _identity(record.get("image_id", "resident"), f"{label} image_id")
        symbol = _symbol(record["symbol"], f"{label} symbol")
        source = _source(record["source"], f"{label} source")
        rom_start, size = _extent(record["rom_start"], record["size"], f"{label} ROM", ROM_SIZE)
        vram_start, _ = _extent(record["vram_start"], record["size"], f"{label} VRAM", ADDRESS_LIMIT)
        match = _Match(image_id, symbol, source, rom_start, vram_start, size)
        if match.key in keys:
            raise ValueError(f"Duplicate C match identity: {image_id}/{symbol}")
        keys.add(match.key)
        parsed.append(match)
    _no_overlap(
        [(match.rom_start, match.rom_start + match.size, f"{match.image_id}/{match.symbol}") for match in parsed],
        "original ROM C ranges",
    )
    return parsed


def _images(image_records: dict[str, dict[str, object]] | None) -> dict[str, _Image]:
    records = {"resident": RESIDENT_IMAGE} if image_records is None else image_records
    if not isinstance(records, dict):
        raise ValueError("Image records must be a dictionary keyed by image identity")
    parsed: dict[str, _Image] = {}
    for key, value in records.items():
        image_id = _identity(key, "Image identity")
        label = f"Image {image_id}"
        record = _record(value, label, {"rom_start", "rom_end", "vram_start"})
        rom_start = _integer(record["rom_start"], f"{label} rom_start")
        rom_end = _integer(record["rom_end"], f"{label} rom_end")
        vram_start = _integer(record["vram_start"], f"{label} vram_start")
        vram_end = _integer(record.get("vram_end", vram_start + rom_end - rom_start), f"{label} vram_end")
        if not (0 <= rom_start < rom_end <= ROM_SIZE and 0 <= vram_start < vram_end <= ADDRESS_LIMIT):
            raise ValueError(f"{label} has invalid original/virtual extents")
        if rom_end - rom_start != vram_end - vram_start:
            raise ValueError(f"{label} must describe a byte-linear initialized image")
        parsed[image_id] = _Image(rom_start, rom_end, vram_start, vram_end)
    _no_overlap([(image.rom_start, image.rom_end, key) for key, image in parsed.items()], "original ROM image ranges")
    return parsed


def validate_matches(
    matches: list[dict[str, object]],
    image_records: dict[str, dict[str, object]] | None = None,
) -> dict[str, int]:
    """Validate unique C/C++ ranges and their authoritative image mappings.

    An omitted image_id means resident. Without image_records the only permitted
    image is resident, ROM [0x1000, 0x12340), with VRAM-ROM delta 0x80024C00.
    Explicit records replace that default inventory. Different images may share
    VRAM; their original ROM ranges and counted function ranges must be disjoint.
    Several distinct functions may belong to the same C source/translation unit.

    `matched_c_*` count `.c` sources only. A manifest with any `.cpp` source adds
    `matched_cpp_*` and their sum `matched_c_and_cpp_*`; a C-only manifest keeps
    exactly the two C counters.
    """
    parsed = _matches(matches)
    images = _images(image_records)
    for match in parsed:
        image = images.get(match.image_id)
        if image is None:
            raise ValueError(f"Unknown image for C match: {match.image_id}/{match.symbol}")
        if not (image.rom_start <= match.rom_start and match.rom_start + match.size <= image.rom_end):
            raise ValueError(f"C match exceeds its original image: {match.image_id}/{match.symbol}")
        if match.vram_start != image.vram_start + match.rom_start - image.rom_start:
            raise ValueError(f"C match has inconsistent ROM/VRAM mapping: {match.image_id}/{match.symbol}")
    counts = {"matched_c_functions": 0, "matched_c_bytes": 0}
    for match in parsed:
        kind = compiled_kind(match.source)
        counts[f"matched_{kind}_functions"] = counts.get(f"matched_{kind}_functions", 0) + 1
        counts[f"matched_{kind}_bytes"] = counts.get(f"matched_{kind}_bytes", 0) + match.size
    if "matched_cpp_functions" in counts:
        counts["matched_c_and_cpp_functions"] = len(parsed)
        counts["matched_c_and_cpp_bytes"] = sum(match.size for match in parsed)
    return counts


def validate_attributions(
    matches: list[dict[str, object]],
    attributions: list[dict[str, object]],
    graph: dict[str, dict[str, object]],
) -> None:
    """Bind every image/function to its manifest source and declared C/C++ object.

    graph is independently derived by the live verifier, never accepted solely
    from a receipt. Its keys are run-relative obj/*.o paths; values contain
    source, source_kind ('c', 'cpp', 'asm', 'binary'), and optional image_id
    (resident). A compiled object's source suffix must agree with its kind.
    Attribution records contain symbol, source, object, vram_start, size and
    optional image_id. Extra diagnostic fields are allowed but not certified by
    this pure helper. Call validate_matches with the image inventory separately.
    """
    parsed = _matches(matches)
    expected = {match.key: match for match in parsed}
    if not isinstance(graph, dict):
        raise ValueError("Declared build graph must be a dictionary")
    c_objects: dict[tuple[str, str], str] = {}
    for object_id, value in graph.items():
        object_path = _relative_path(object_id, "Declared object", root="obj", suffix=".o")
        record = _record(value, f"Graph object {object_path}", {"source", "source_kind"})
        kind = record["source_kind"]
        if not isinstance(kind, str) or kind not in SOURCE_KINDS:
            raise ValueError(f"Graph object has unsupported source_kind: {object_path}")
        image_id = _identity(record.get("image_id", "resident"), f"Graph object {object_path} image_id")
        source = _relative_path(record["source"], f"Graph object {object_path} source")
        if kind in COMPILED_KINDS:
            source = _relative_path(source, f"Graph {LANGUAGE_LABELS[kind]} object {object_path} source",
                                    root="src", suffix=COMPILED_SUFFIXES[kind])
            key = image_id, source
            if key in c_objects:
                raise ValueError(f"Ambiguous declared C objects for {image_id}/{source}")
            c_objects[key] = object_path
    if not isinstance(attributions, list):
        raise ValueError("C attributions must be a list")
    seen: set[tuple[str, str]] = set()
    for index, value in enumerate(attributions):
        label = f"C attribution {index}"
        record = _record(value, label, _ATTRIBUTION_FIELDS)
        image_id = _identity(record.get("image_id", "resident"), f"{label} image_id")
        symbol = _symbol(record["symbol"], f"{label} symbol")
        key = image_id, symbol
        if key in seen:
            raise ValueError(f"Duplicate C attribution: {image_id}/{symbol}")
        seen.add(key)
        match = expected.get(key)
        if match is None:
            raise ValueError(f"Undeclared C attribution: {image_id}/{symbol}")
        source = _source(record["source"], f"{label} source")
        object_path = _relative_path(record["object"], f"{label} object", root="obj", suffix=".o")
        vram_start, size = _extent(record["vram_start"], record["size"], f"{label} VRAM", ADDRESS_LIMIT)
        if source != match.source or vram_start != match.vram_start or size != match.size:
            raise ValueError(f"C attribution disagrees with the match manifest: {image_id}/{symbol}")
        declared_object = c_objects.get((image_id, source))
        if declared_object is None or object_path != declared_object:
            raise ValueError(f"C attribution disagrees with the declared C object: {image_id}/{symbol}")
    missing = expected.keys() - seen
    if missing:
        raise ValueError(f"Missing C attributions: {sorted(missing)}")
