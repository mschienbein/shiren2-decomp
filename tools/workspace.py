"""Freeze the project's allowlisted build inputs without relying on Git state.

Snapshots preserve input bytes and ordinary permissions. They are immutable by
workflow, not by OS protection; failed copies remain available for diagnosis.
ROMs, installed tools, Python environments and generated outputs are external
inputs and are deliberately excluded from this source-only manifest.
"""
from __future__ import annotations

import hashlib
import json
import os
import stat
from collections.abc import Mapping
from pathlib import Path, PurePosixPath
from typing import TypedDict


class FileFingerprint(TypedDict):
    sha256: str
    size: int


class InputManifest(TypedDict):
    schema_version: int
    files: dict[str, FileFingerprint]
    sha256: str


SOURCE_DIRECTORIES = ("src", "include", "config", "tools", "tests")
REQUIRED_FILES = (
    ".python-version", "Makefile", "diff_settings.py", "pyproject.toml",
    "toolchain.lock.json", "uv.lock",
)
OPTIONAL_FILES = ("AGENTS.md", "README.md")
EXCLUDED_NAMES = frozenset({
    ".DS_Store", "__pycache__", ".git", ".venv", "scratch", "build",
    "generated", ".cache",
})


def _root_path(root: Path) -> Path:
    try:
        mode = root.lstat().st_mode
    except OSError as error:
        raise ValueError(f"Cannot inspect source root: {root}") from error
    if not stat.S_ISDIR(mode):
        raise ValueError(f"Source root must be a real directory: {root}")
    return root.resolve()


def _relative_path(value: str) -> PurePosixPath:
    path = PurePosixPath(value)
    if (
        not value or "\\" in value or path.is_absolute()
        or ".." in path.parts or path.as_posix() != value or value == "."
        or any(part in EXCLUDED_NAMES for part in path.parts)
    ):
        raise ValueError(f"Unsafe input path: {value!r}")
    if not (
        value in REQUIRED_FILES or value in OPTIONAL_FILES
        or (len(path.parts) > 1 and path.parts[0] in SOURCE_DIRECTORIES)
    ):
        raise ValueError(f"Input path is outside the allowlist: {value!r}")
    return path


def _checked_input(root: Path, relative: str) -> Path:
    path = root
    parts = _relative_path(relative).parts
    for index, part in enumerate(parts):
        path /= part
        try:
            mode = path.lstat().st_mode
        except OSError as error:
            raise ValueError(f"Cannot inspect input: {relative}") from error
        expected = stat.S_ISREG if index == len(parts) - 1 else stat.S_ISDIR
        if not expected(mode):
            raise ValueError(f"Input must contain only real directories and regular files: {relative}")
    return path


def _input_paths(root: Path) -> dict[str, Path]:
    result: dict[str, Path] = {}

    def visit(path: Path) -> None:
        if path.name in EXCLUDED_NAMES:
            return
        relative = path.relative_to(root).as_posix()
        try:
            mode = path.lstat().st_mode
        except OSError as error:
            raise ValueError(f"Cannot inspect input: {relative}") from error
        if stat.S_ISDIR(mode):
            try:
                children = sorted(path.iterdir(), key=lambda child: child.name)
            except OSError as error:
                raise ValueError(f"Cannot enumerate input directory: {relative}") from error
            for child in children:
                visit(child)
        elif stat.S_ISREG(mode):
            result[relative] = _checked_input(root, relative)
        else:
            raise ValueError(f"Input must be a regular file or real directory: {relative}")

    for relative in REQUIRED_FILES:
        result[relative] = _checked_input(root, relative)
    for relative in OPTIONAL_FILES:
        path = root / relative
        if path.exists() or path.is_symlink():
            result[relative] = _checked_input(root, relative)
    for relative in SOURCE_DIRECTORIES:
        path = root / relative
        if path.exists() or path.is_symlink():
            visit(path)
    return dict(sorted(result.items()))


def _identity(info: os.stat_result) -> tuple[int, int, int, int, int]:
    return info.st_dev, info.st_ino, info.st_size, info.st_mtime_ns, info.st_ctime_ns


def _read_regular_file(path: Path) -> tuple[bytes, int]:
    try:
        before = path.lstat()
        if not stat.S_ISREG(before.st_mode):
            raise ValueError(f"Input must be a regular file: {path}")
        with path.open("rb") as stream:
            opened = os.fstat(stream.fileno())
            if not stat.S_ISREG(opened.st_mode) or _identity(opened) != _identity(before):
                raise ValueError(f"Input changed before reading: {path}")
            data = stream.read()
            after_read = os.fstat(stream.fileno())
        after = path.lstat()
    except OSError as error:
        raise ValueError(f"Cannot read input: {path}") from error
    if (
        _identity(before) != _identity(after_read)
        or _identity(before) != _identity(after)
        or len(data) != before.st_size
    ):
        raise ValueError(f"Input changed while reading: {path}")
    return data, stat.S_IMODE(before.st_mode)


def _fingerprint(data: bytes) -> FileFingerprint:
    return {"sha256": hashlib.sha256(data).hexdigest(), "size": len(data)}


def _entries_digest(files: Mapping[str, FileFingerprint]) -> str:
    encoded = json.dumps(files, sort_keys=True, separators=(",", ":"), ensure_ascii=True)
    return hashlib.sha256(encoded.encode("utf-8")).hexdigest()


def input_manifest(root: Path) -> InputManifest:
    """Hash all allowlisted inputs, including nested and untracked files."""
    root = _root_path(root)
    files = {
        relative: _fingerprint(_read_regular_file(path)[0])
        for relative, path in _input_paths(root).items()
    }
    return {"schema_version": 1, "files": files, "sha256": _entries_digest(files)}


def _validated_manifest(manifest: Mapping[str, object]) -> InputManifest:
    if set(manifest) != {"schema_version", "files", "sha256"}:
        raise ValueError("Input manifest has missing or unsupported fields")
    if type(manifest["schema_version"]) is not int or manifest["schema_version"] != 1:
        raise ValueError("Unsupported input manifest schema")
    entries = manifest["files"]
    if not isinstance(entries, dict):
        raise ValueError("Input manifest files must be a dictionary")
    files: dict[str, FileFingerprint] = {}
    for relative, entry in entries.items():
        if not isinstance(relative, str):
            raise ValueError("Input manifest paths must be strings")
        _relative_path(relative)
        if not isinstance(entry, dict) or set(entry) != {"sha256", "size"}:
            raise ValueError(f"Invalid input fingerprint: {relative}")
        digest, size = entry["sha256"], entry["size"]
        if (
            not isinstance(digest, str) or len(digest) != 64
            or any(character not in "0123456789abcdef" for character in digest)
            or type(size) is not int or size < 0
        ):
            raise ValueError(f"Invalid input fingerprint: {relative}")
        files[relative] = {"sha256": digest, "size": size}
    if not set(REQUIRED_FILES).issubset(files):
        raise ValueError("Input manifest omits mandatory build files")
    digest = _entries_digest(files)
    if manifest["sha256"] != digest:
        raise ValueError("Input manifest digest disagrees with its file entries")
    return {"schema_version": 1, "files": dict(sorted(files.items())), "sha256": digest}


def verify_manifest(root: Path, manifest: Mapping[str, object]) -> None:
    """Require the complete current allowlist to equal a validated manifest."""
    expected = _validated_manifest(manifest)
    actual = input_manifest(root)
    if actual != expected:
        expected_files, actual_files = expected["files"], actual["files"]
        missing = sorted(expected_files.keys() - actual_files.keys())
        extra = sorted(actual_files.keys() - expected_files.keys())
        changed = sorted(
            path for path in expected_files.keys() & actual_files.keys()
            if expected_files[path] != actual_files[path]
        )
        raise ValueError(f"Input manifest mismatch: missing={missing}, extra={extra}, changed={changed}")


def _destination_path(root: Path, destination: Path) -> Path:
    if ".." in destination.parts:
        raise ValueError("Snapshot destination must not contain parent traversal")
    destination = destination.absolute()
    # Existing destination ancestors must be ordinary directories. This also
    # refuses a symlink that would redirect a copy outside the selected location.
    for ancestor in reversed((destination.parent, *destination.parent.parents)):
        try:
            mode = ancestor.lstat().st_mode
        except FileNotFoundError:
            continue
        if not stat.S_ISDIR(mode):
            raise ValueError(f"Snapshot parent must be a real directory: {ancestor}")
    if destination.is_symlink():
        raise ValueError(f"Snapshot destination must not be a symlink: {destination}")
    if destination.exists():
        raise FileExistsError(f"Snapshot destination already exists: {destination}")
    destination = destination.parent.resolve() / destination.name
    if destination.is_relative_to(root):
        relative = destination.relative_to(root)
        if relative.parts and relative.parts[0] in SOURCE_DIRECTORIES:
            raise ValueError("Snapshot destination must be outside the source allowlist")
    return destination


def _copy_file(source: Path, destination: Path, expected: FileFingerprint) -> None:
    data, mode = _read_regular_file(source)
    if _fingerprint(data) != expected:
        raise ValueError(f"Input changed before copying: {source}")
    destination.parent.mkdir(parents=True, exist_ok=True)
    with destination.open("xb") as stream:
        stream.write(data)
    destination.chmod(mode)


def create_snapshot(root: Path, destination: Path) -> InputManifest:
    """Copy an exclusive source snapshot and validate live and frozen contents.

    No existing destination is reused. A failed copy is intentionally retained
    but has no successful manifest returned. Callers should use canonical paths
    for external destinations; symlinked destination ancestors are rejected.
    """
    original_root = root.absolute()
    root = _root_path(root)
    # Canonicalize aliases above the caller's selected root (for example macOS
    # /var -> /private/var) without accepting symlinks inside the project.
    if destination.absolute().is_relative_to(original_root):
        destination = root / destination.absolute().relative_to(original_root)
    destination = _destination_path(root, destination)
    manifest = input_manifest(root)
    destination.mkdir(parents=True, exist_ok=False)
    for relative, expected in manifest["files"].items():
        source = _checked_input(root, relative)
        _copy_file(source, destination / relative, expected)
    verify_manifest(root, manifest)
    verify_manifest(destination, manifest)
    return manifest
