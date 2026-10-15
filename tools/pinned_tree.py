"""Verify executable inputs against their pins without running git, then import only the
verified bytes (stdlib only).

The trust anchor is the pinned commit ID. Git objects are content-addressed, so every object
read from `.git` (loose or packed, including OFS/REF deltas) is re-hashed and must equal the ID
that referenced it: commit -> root tree -> subtrees -> blobs. Each tracked working file is then
compared to its blob: the sha1 of "blob <len>\\0<bytes>", the executable bit for 100755, and
the link target for 120000.

Any missing or changed tracked file is rejected. So is every untracked file, with one
exception: files inside `__pycache__` directories are reported and never executed.
`.git/` itself is never imported and is excluded from the walk.

`verify_checkout()` returns the bytes it hashed. `VerifiedImporter` compiles pinned packages from
exactly those in-memory bytes: no bytecode cache is read or written for them (not the checkout's
`__pycache__`, not `sys.pycache_prefix`/PYTHONPYCACHEPREFIX), no file is re-read between check and
import, and a module without verified bytes raises instead of falling through to the path finders.
`-B` alone would only stop bytecode writes, not reads.

The one third-party dependency, toml 0.10.2, is located with `PathFinder.find_spec` (which runs no
package code). `verify_toml()` reads and hashes every file against pinned per-file sha256 values
(taken from the cached wheel's RECORD when this tool was authored) and rejects extra files. Only
then does `VerifiedImporter` execute those same bytes.
"""
from __future__ import annotations

import hashlib
import importlib.machinery
import importlib.util
import os
import stat
import struct
import sys
import zlib
from pathlib import Path

TOML_FILES = {
    "__init__.py": "02ede4a82c0a0f47236dfe322463a9505c29b18d161ec0b56511af5a020a9a97",
    "decoder.py": "8521932dffb65810d9fdd768087585cba37ae3b5f2312875a37acdda8e1d1058",
    "encoder.py": "5e305cf1acafbe5b0bc9dfea0c0e2d3160cd98c1514b80e9c2db8349606aef3a",
    "ordered.py": "9b3d37959995d1b99cf65b1844850eba3ec3b2ee4fb70abe52d195ab915d559e",
    "tz.py": "fb9be0f30920fdab67562d939c4bde7b121513b4ff171055afffb65957ced680",
}
_TYPES = {1: b"commit", 2: b"tree", 3: b"blob", 4: b"tag"}


class PinError(ValueError):
    pass


class ObjectStore:
    def __init__(self, git_dir: Path):
        self.git = git_dir
        self.packs = []
        for idx in sorted((git_dir / "objects/pack").glob("*.idx")):
            self.packs.append((idx.read_bytes(), idx.with_suffix(".pack").read_bytes()))

    def _pack_offset(self, index: bytes, oid: bytes) -> int | None:
        if index[:8] != b"\377tOc\x00\x00\x00\x02":
            raise PinError("unsupported pack index version")
        fanout = struct.unpack_from(">256I", index, 8)
        total = fanout[255]
        lo = fanout[oid[0] - 1] if oid[0] else 0
        hi = fanout[oid[0]]
        names = 8 + 1024
        while lo < hi:
            mid = (lo + hi) // 2
            name = index[names + 20 * mid: names + 20 * mid + 20]
            if name == oid:
                offsets = names + 24 * total
                off = struct.unpack_from(">I", index, offsets + 4 * mid)[0]
                if off & 0x80000000:
                    large = offsets + 4 * total
                    off = struct.unpack_from(">Q", index, large + 8 * (off & 0x7FFFFFFF))[0]
                return off
            if name < oid:
                lo = mid + 1
            else:
                hi = mid
        return None

    @staticmethod
    def _inflate(pack: bytes, pos: int) -> bytes:
        decomp = zlib.decompressobj()
        view = memoryview(pack)
        out = bytearray()
        while not decomp.eof:
            if pos >= len(pack):
                raise PinError("truncated pack object")
            out += decomp.decompress(view[pos:pos + 65536])
            pos += 65536
        return bytes(out)

    def read(self, oid: bytes) -> tuple[bytes, bytes]:
        """(type, data) for `oid`, verified against its own content hash."""
        hexid = oid.hex()
        loose = self.git / "objects" / hexid[:2] / hexid[2:]
        if loose.is_file():
            raw = zlib.decompress(loose.read_bytes())
            header, data = raw.split(b"\0", 1)
            kind, size = header.split(b" ")
            if int(size) != len(data):
                raise PinError(f"loose object {hexid} has a wrong size")
        else:
            for index, pack in self.packs:
                start = self._pack_offset(index, oid)
                if start is not None:
                    kind, data = self._read_at(pack, index, start)
                    break
            else:
                raise PinError(f"object {hexid} is missing from the store")
        if hashlib.sha1(kind + b" " + str(len(data)).encode() + b"\0" + data).digest() != oid:
            raise PinError(f"object {hexid} does not hash to its ID")
        return kind, data

    def _read_at(self, pack: bytes, index: bytes, start: int) -> tuple[bytes, bytes]:
        c = pack[start]
        kind = (c >> 4) & 7
        pos = start + 1
        while c & 0x80:
            c = pack[pos]
            pos += 1
        if kind in _TYPES:
            return _TYPES[kind], self._inflate(pack, pos)
        if kind == 6:
            c = pack[pos]
            pos += 1
            back = c & 0x7F
            while c & 0x80:
                c = pack[pos]
                pos += 1
                back = ((back + 1) << 7) | (c & 0x7F)
            base_type, base = self._read_at(pack, index, start - back)
        elif kind == 7:
            base_type, base = self.read(pack[pos:pos + 20])
            pos += 20
        else:
            raise PinError(f"unsupported pack object type {kind}")
        return base_type, _apply_delta(base, self._inflate(pack, pos))


def _varint(data: bytes, pos: int) -> tuple[int, int]:
    value = shift = 0
    while True:
        c = data[pos]
        pos += 1
        value |= (c & 0x7F) << shift
        shift += 7
        if not c & 0x80:
            return value, pos


def _apply_delta(base: bytes, delta: bytes) -> bytes:
    size, pos = _varint(delta, 0)
    if size != len(base):
        raise PinError("delta base size mismatch")
    target, pos = _varint(delta, pos)
    out = bytearray()
    while pos < len(delta):
        op = delta[pos]
        pos += 1
        if op & 0x80:
            offset = length = 0
            for i in range(4):
                if op & (1 << i):
                    offset |= delta[pos] << (8 * i)
                    pos += 1
            for i in range(3):
                if op & (0x10 << i):
                    length |= delta[pos] << (8 * i)
                    pos += 1
            length = length or 0x10000
            out += base[offset:offset + length]
        elif op:
            out += delta[pos:pos + op]
            pos += op
        else:
            raise PinError("invalid delta opcode 0")
    if len(out) != target:
        raise PinError("delta result size mismatch")
    return bytes(out)


def _tree_entries(store: ObjectStore, oid: bytes, prefix: str, out: dict) -> None:
    kind, data = store.read(oid)
    if kind != b"tree":
        raise PinError(f"{prefix or '/'} is not a tree")
    pos = 0
    while pos < len(data):
        space = data.index(b" ", pos)
        nul = data.index(b"\0", space)
        mode, name = data[pos:space].decode(), data[space + 1:nul].decode()
        child = data[nul + 1:nul + 21]
        pos = nul + 21
        path = f"{prefix}{name}"
        if mode == "40000":
            _tree_entries(store, child, path + "/", out)
        else:
            out[path] = (mode, child)


def tracked_files(root: Path, commit: str) -> tuple[str, dict]:
    store = ObjectStore(root / ".git")
    kind, data = store.read(bytes.fromhex(commit))
    if kind != b"commit":
        raise PinError(f"{commit} is not a commit")
    tree = data.split(b"\n", 1)[0]
    if not tree.startswith(b"tree "):
        raise PinError("commit has no tree line")
    tree_id = tree[5:].decode()
    entries: dict = {}
    _tree_entries(store, bytes.fromhex(tree_id), "", entries)
    return tree_id, entries


def verify_checkout(root: Path, commit: str) -> tuple[dict, dict[str, bytes]]:
    """Reject unless every tracked file equals the pinned commit and nothing extra is present.
    Returns the evidence and the hashed bytes of every tracked regular file (relative path ->
    bytes); `VerifiedImporter` executes exactly these bytes."""
    root = root.resolve()
    tree_id, entries = tracked_files(root, commit)
    problems = []
    blobs: dict[str, bytes] = {}
    for path, (mode, oid) in sorted(entries.items()):
        file = root / path
        try:
            info = os.lstat(file)
        except FileNotFoundError:
            problems.append(f"missing {path}")
            continue
        if mode == "160000":
            problems.append(f"submodule {path} is not supported")
            continue
        if mode == "120000":
            if not stat.S_ISLNK(info.st_mode):
                problems.append(f"{path} should be a symlink")
                continue
            content = os.readlink(file).encode()
        else:
            if not stat.S_ISREG(info.st_mode):
                problems.append(f"{path} should be a regular file")
                continue
            if (mode == "100755") != bool(info.st_mode & stat.S_IXUSR):
                problems.append(f"{path} executable bit differs from {mode}")
            content = file.read_bytes()
        if hashlib.sha1(b"blob " + str(len(content)).encode() + b"\0" + content).digest() != oid:
            problems.append(f"changed {path}")
        elif mode != "120000":
            blobs[path] = content
    inert, extra = [], []
    for directory, dirs, files in os.walk(root):
        rel = Path(directory).relative_to(root)
        if rel.parts[:1] == (".git",):
            dirs[:] = []
            continue
        dirs[:] = [d for d in dirs if not (rel / d).parts[:1] == (".git",)]
        for name in files + [d for d in dirs if os.path.islink(Path(directory) / d)]:
            path = (rel / name).as_posix()
            if path in entries:
                continue
            (inert if "__pycache__" in Path(path).parts else extra).append(path)
    problems += [f"untracked {p}" for p in sorted(extra)]
    if problems:
        raise PinError(f"checkout {root} differs from pinned commit {commit}: " + "; ".join(problems[:8])
                       + (f" (+{len(problems) - 8} more)" if len(problems) > 8 else ""))
    return ({"commit": commit, "git_tree": tree_id, "tracked_files": len(entries),
             "inert_bytecode_cache_files": len(inert)}, blobs)


def locate_toml() -> Path:
    """The toml package directory this interpreter would import, found without executing it."""
    if "toml" in sys.modules:
        raise PinError("toml was imported before its bytes were verified")
    spec = importlib.machinery.PathFinder.find_spec("toml")
    if spec is None or not spec.submodule_search_locations or not spec.origin or Path(spec.origin).name != "__init__.py":
        raise PinError("toml is not an importable package directory in this environment")
    return Path(spec.origin).resolve().parent


def verify_toml(package: Path) -> tuple[dict, dict[str, bytes]]:
    """Read and hash every toml file once, before any of it runs. Returns the evidence and the
    verified bytes keyed `toml/<file>` (relative to the package's parent directory)."""
    package = Path(package)
    present = sorted(p.name for p in package.iterdir() if p.name != "__pycache__")
    if present != sorted(TOML_FILES):
        raise PinError(f"toml package files {present} differ from the pinned set {sorted(TOML_FILES)}")
    blobs = {}
    for name, digest in TOML_FILES.items():
        if not stat.S_ISREG(os.lstat(package / name).st_mode):
            raise PinError(f"toml/{name} is not a regular file")
        data = (package / name).read_bytes()
        if hashlib.sha256(data).hexdigest() != digest:
            raise PinError(f"toml/{name} differs from the pinned sha256")
        blobs[f"toml/{name}"] = data
    return {"package": str(package), "files": dict(TOML_FILES)}, blobs


class VerifiedImporter:
    """`sys.meta_path` finder and loader that owns the given top-level package names.

    `roots` maps each owned name to (base directory, verified bytes keyed by path relative to that
    base). A module under an owned name is compiled from those bytes in memory (`dont_inherit`, so
    this file's future flags do not leak into it) and executed; no bytecode cache is consulted.
    A name without verified bytes raises ModuleNotFoundError rather than falling through to the
    path finders. `served` maps every executed module to the sha256 of the bytes that ran."""

    def __init__(self, roots: dict[str, tuple[Path, dict[str, bytes]]]):
        self.roots = {name: (Path(base), blobs) for name, (base, blobs) in roots.items()}
        self.served: dict[str, str] = {}

    def find_spec(self, fullname, path=None, target=None):
        top = fullname.partition(".")[0]
        if top not in self.roots:
            return None
        base, blobs = self.roots[top]
        rel = fullname.replace(".", "/")
        for key, package in ((f"{rel}/__init__.py", True), (f"{rel}.py", False)):
            if key in blobs:
                spec = importlib.machinery.ModuleSpec(fullname, self, origin=str(base / key), loader_state=key,
                                                      is_package=package)
                if package:
                    spec.submodule_search_locations = [str(base / rel)]
                spec.has_location = True
                return spec
        raise ModuleNotFoundError(f"{fullname} has no verified source in the pinned input", name=fullname)

    def create_module(self, spec):
        return None

    def exec_module(self, module) -> None:
        spec = module.__spec__
        source = self.roots[spec.name.partition(".")[0]][1][spec.loader_state]
        code = compile(source, spec.origin, "exec", dont_inherit=True)
        self.served[spec.name] = hashlib.sha256(source).hexdigest()
        exec(code, module.__dict__)

    def get_source(self, fullname: str) -> str:
        spec = self.find_spec(fullname)
        return importlib.util.decode_source(self.roots[fullname.partition(".")[0]][1][spec.loader_state])

    def is_package(self, fullname: str) -> bool:
        return self.find_spec(fullname).submodule_search_locations is not None

    def fingerprint(self) -> dict:
        return {name: (str(base), hashlib.sha256(b"".join(hashlib.sha256(k.encode() + b"\0" + v).digest()
                                                         for k, v in sorted(blobs.items()))).hexdigest())
                for name, (base, blobs) in sorted(self.roots.items())}

    def audit(self) -> dict[str, str]:
        """Fail unless every loaded module under an owned name was executed by this importer."""
        foreign = sorted(name for name, module in list(sys.modules.items())
                         if name.partition(".")[0] in self.roots and getattr(module, "__loader__", None) is not self)
        if foreign:
            raise PinError(f"modules not loaded from verified bytes: {foreign[:4]}")
        return dict(sorted(self.served.items()))


def install_verified(roots: dict[str, tuple[Path, dict[str, bytes]]]) -> VerifiedImporter:
    """Put a VerifiedImporter for `roots` first on `sys.meta_path` (once per process). Refuses when
    an owned package was already imported some other way or another verified input owns it."""
    importer = VerifiedImporter(roots)
    for finder in sys.meta_path:
        if isinstance(finder, VerifiedImporter) and set(finder.roots) & set(importer.roots):
            if finder.fingerprint() == importer.fingerprint():
                return finder
            raise PinError(f"{sorted(set(finder.roots) & set(importer.roots))} already served from other verified bytes")
    loaded = sorted(name for name in sys.modules if name.partition(".")[0] in importer.roots)
    if loaded:
        raise PinError(f"{loaded[:4]} imported before verification")
    sys.meta_path.insert(0, importer)
    return importer
