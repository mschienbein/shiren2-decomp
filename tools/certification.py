"""Independently recomputed build evidence for the current resident pilot."""
from __future__ import annotations

import hashlib
from contextlib import contextmanager
import importlib.metadata
import json
import os
import re
import subprocess
import sys
import struct
import tempfile
import shutil
import traceback
from pathlib import Path
from typing import Any

from spimdisasm.elf32 import Elf32File
import yaml

from rom import CANONICAL_ROM, attest
from assets import generated_keys as tail_generated_keys
import rommap
from images import OVERLAY_IMAGES, ImageInventory, image_for_segment, image_for_source, load_inventory, policy_records, require_cpu_range
from evidence import COMPILED_KINDS, COMPILED_SUFFIXES, LANGUAGE_LABELS, compiled_kind, validate_matches
import toolchain_library
import generated_symbols

COMPILER_FLAGS = ["-O2", "-mips3", "-mgp32", "-mfp32", "-G", "0", "-mno-abicalls", "-fno-PIC", "-fno-builtin", "-funsigned-char", "-nostdinc", "-I", "source/include"]
ASSEMBLER_FLAGS = ["-march=vr4300", "-mabi=32", "-mfp32", "-EB", "-G", "0", "-I", "source/include"]
PROFILE_ID = "scoped-tu-toolchains-v4"
# Per-TU toolchain registries: key -> toolchain.lock.json entry, which records
# the project-local install directory and the pinned binary hashes.
DEFAULT_COMPILER = "gcc272"
COMPILERS = {"gcc272": "gcc_candidate", "gcc281pm": "gcc_pm281"}
PINNED_ASSEMBLERS = {"as_kmc26": "binutils_kmc", "as_gnu291": "binutils_gnu291", "as_gnu291_fp32": "binutils_gnu291_fp32"}
ASSEMBLERS = {"as", *PINNED_ASSEMBLERS}
# Profile language -> compiled source kind. The suffix selects the front end (the 2.8.1
# driver runs cc1plus for `.cpp`), so profile flags may not select a language with `-x`.
LANGUAGES = {"c": "c", "c++": "cpp"}
CXX_REQUIRED_FLAGS = ("-fno-exceptions", "-fno-rtti")
REQUIRED_ARTIFACTS = {"shiren2.elf", "shiren2.z64", "shiren2.map", "shiren2.ld", "splat.override.yaml", "input-manifest.json"}
REQUIRED_RECEIPT_FIELDS = {"schema_version", "status", "created_at", "target_sha256", "rom", "input_manifest", "profile", "runtime", "compiler_environment", "coverage", "c_matches", "c_attributions", "graph", "dependencies", "artifacts", "generated", "compiled", "objects", "commands", "reproduction", "image_inventory", "rom_map", "limitations"}


def read_json(path: Path) -> Any:
    def unique_pairs(pairs: list[tuple[str, Any]]) -> dict[str, Any]:
        result = {}
        for key, value in pairs:
            if key in result:
                raise ValueError(f"Duplicate JSON key: {key}")
            result[key] = value
        return result
    return json.loads(path.read_text(), object_pairs_hook=unique_pairs)


def sha256(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def json_digest(value: object) -> str:
    return hashlib.sha256(json.dumps(value, sort_keys=True, separators=(",", ":")).encode()).hexdigest()


def python_command() -> str:
    """Use one venv invocation; python/python3 symlinks share runtime identity."""
    path = Path(sys.prefix) / "bin/python"
    if not path.is_file() or path.resolve() != Path(sys.executable).resolve():
        raise ValueError("Run build tools inside the locked uv virtual environment")
    return str(path)


def checked_path(root: Path, name: str) -> Path:
    if not isinstance(name, str) or not name or Path(name).is_absolute():
        raise ValueError("Evidence paths must be nonempty relative paths")
    path = root / name
    if ".." in Path(name).parts or path.is_symlink() or not path.resolve().is_relative_to(root.resolve()):
        raise ValueError("Receipt path escapes its root or uses a symlink")
    return path


def file_inventory(root: Path) -> dict[str, str]:
    if not root.is_dir():
        raise ValueError(f"Missing evidence directory: {root.name}")
    result = {}
    for path in sorted(root.rglob("*")):
        if path.is_symlink():
            raise ValueError(f"Symlink in generated evidence: {path}")
        if path.is_file():
            result[path.relative_to(root).as_posix()] = sha256(path)
    return result


RETAINED_TRIAL_EVIDENCE = frozenset({"lifecycle.json", "command-evidence", "command.json", "stdout.txt", "stderr.txt"})


@contextmanager
def retained_trial(directory: Path, label: str):
    """Run one replay trial outside certified inventories.

    Failed trials keep every raw output. Successful trials keep only the small
    command evidence (argv, stdout/stderr, return codes) plus ``lifecycle.json``
    and prune bulky copies/objects/links, recording SHA-256 of pruned top-level
    files. ``SHIREN2_RETAIN_SUCCESSFUL_REPLAYS=1`` keeps successful trials whole.
    Callers must not read trial contents after the context exits.
    """
    selected = os.environ.get("SHIREN2_REPLAY_EVIDENCE_ROOT")
    base = Path(selected) if selected else directory / "replay-evidence"
    if not base.is_absolute() or base.is_symlink() or any(base.resolve().is_relative_to((directory / name).resolve()) for name in ("source", "generated", "compiled", "obj")):
        raise ValueError("Replay evidence must use a separate absolute private root")
    base.mkdir(parents=True, exist_ok=True)
    trial = Path(tempfile.mkdtemp(prefix=f"{label}-", dir=base))
    status = {"label": label, "status": "running", "raw_artifacts_retained": True,
        "receipt_paths_excluded": True, "directory": str(trial)}
    (trial / "lifecycle.json").write_text(json.dumps(status, indent=2) + "\n")
    try:
        yield str(trial)
    except BaseException:
        status.update(status="failed", error=traceback.format_exc())
        raise
    else:
        if os.environ.get("SHIREN2_RETAIN_SUCCESSFUL_REPLAYS") != "1":
            pruned_files, pruned_directories = {}, []
            for path in sorted(trial.iterdir()):
                if path.name in RETAINED_TRIAL_EVIDENCE:
                    continue
                if path.is_dir() and not path.is_symlink():
                    shutil.rmtree(path)
                    pruned_directories.append(path.name)
                else:
                    if path.is_file() and not path.is_symlink():
                        pruned_files[path.name] = sha256(path)
                    path.unlink()
            status.update(raw_artifacts_retained=False, bulk_outputs_pruned=True,
                pruned_file_sha256=pruned_files, pruned_directories=pruned_directories)
        status["status"] = "complete"
    finally:
        (trial / "lifecycle.json").write_text(json.dumps(status, indent=2) + "\n")


class CommandLog:
    """``commands.json`` kept byte-identical to ``json.dumps(records, indent=2) + "\\n"`` after every write.

    That serialization is ``"[\\n" + ",\\n".join(chunk(r)) + "\\n]\\n"``, where ``chunk(r)`` is
    ``json.dumps(r, indent=2)`` with every line prefixed by two spaces (strings escape
    newlines, so every literal newline is structural). The byte offset where each chunk
    ends is kept, so ``rewrite_from(k)`` truncates the file after chunk ``k - 1`` and writes
    only chunks ``k..n`` and the closing bracket. Appending or completing the last record
    therefore costs one record, not the whole log, while every write still leaves exactly
    the file a full re-encoding would. Nothing is buffered or deferred, so evidence on disk
    after a sudden process death is the same as before. Nothing is written while
    ``records`` is empty.
    """

    def __init__(self, path: Path) -> None:
        self.path, self.records, self.ends = path, [], []

    @staticmethod
    def chunk(record: Any) -> bytes:
        return ("  " + json.dumps(record, indent=2).replace("\n", "\n  ")).encode()

    def rewrite_from(self, index: int) -> None:
        """Make the file current after ``records[index:]`` were appended or changed."""
        if not self.records:
            return
        index = min(index, len(self.ends))
        del self.ends[index:]
        position = self.ends[-1] if self.ends else 2
        parts = []
        for record in self.records[index:]:
            part = (b",\n" if self.ends else b"") + self.chunk(record)
            parts.append(part)
            position += len(part)
            self.ends.append(position)
        tail = b"".join(parts) + b"\n]\n"
        if index == 0:
            self.path.write_bytes(b"[\n" + tail)
            return
        with self.path.open("r+b") as stream:
            stream.seek(self.ends[index - 1])
            stream.truncate()
            stream.write(tail)



_C_COMMENT_OR_LITERAL = re.compile(r'/\*.*?\*/|//[^\n]*|"(?:\\.|[^"\\\n])*"|\'(?:\\.|[^\'\\\n])*\'', re.DOTALL)
_C_ASSEMBLY_TOKEN = re.compile(r"\b(?:asm|__asm|__asm__|INCLUDE_ASM|GLOBAL_ASM)\b")
# C++ TUs build with -fno-exceptions -fno-rtti; these constructs have no place in them.
CXX_FORBIDDEN_TOKEN = re.compile(r"\b(?:try|catch|throw|typeid|dynamic_cast)\b")
_C_INCLUDE = re.compile(r"^[ \t]*(?:#|%:)[ \t]*(?:include|include_next|import)\b[ \t]*(.*)$", re.MULTILINE)


def source_policy_violations(root: Path) -> list[str]:
    """Return source-policy violations for every ``.c``/``.cpp``/``.h`` under ``root/src`` and ``root/include``.

    ``root`` is a source tree root (the project, or a build's frozen ``source``).
    After splicing backslash-newlines and ignoring comments and string/char
    literals, a file violates policy if it contains an inline-assembly token
    (``asm``, ``__asm``, ``__asm__``, ``INCLUDE_ASM``, ``GLOBAL_ASM``), the
    ``##`` token-paste operator (or its ``%:%:`` digraph), or an include
    directive that is not a literal ``"x.h"``/``<x.h>`` path, is absolute or
    contains ``..``. Non-header includes (e.g. ``.inc``) are rejected because
    only ``.c``/``.cpp``/``.h`` files are scanned. A ``.cpp`` file additionally may
    not use exception or RTTI constructs (``try``, ``catch``, ``throw``,
    ``typeid``, ``dynamic_cast``). This is a conservative token check;
    semantic or raw-byte misuse still needs review. An empty list means pass.
    """
    violations = []
    for directory in ("src", "include"):
        for path in sorted((root / directory).rglob("*")):
            if path.suffix in {".c", ".cpp", ".h"} and path.is_file():
                violations.extend(source_file_violations(path, path.relative_to(root).as_posix(), source_root=root))
    return violations


def source_file_violations(path: Path, name: str, *, source_root: Path | None = None) -> list[str]:
    """Policy violations of one C/C++/H file; see ``source_policy_violations``."""
    violations = []
    if source_root is not None:
        try:
            if toolchain_library.file_exception(source_root, path):
                return []
        except ValueError as error:
            return [str(error)]
    text = path.read_text().replace("\\\r\n", "").replace("\\\n", "")
    without_comments = _C_COMMENT_OR_LITERAL.sub(lambda match: match[0] if match[0][0] in "\"'" else " ", text)
    code = _C_COMMENT_OR_LITERAL.sub(" ", text)
    if generated_symbols.WRITTEN_GENERATED.search(code):
        violations.append(f"{name}: hand-written compiler-generated name")
    if _C_ASSEMBLY_TOKEN.search(code):
        violations.append(f"{name}: inline assembly token")
    if "##" in code or "%:%:" in code:
        violations.append(f"{name}: token-paste operator")
    if path.suffix == ".cpp" and CXX_FORBIDDEN_TOKEN.search(code):
        violations.append(f"{name}: exception/RTTI construct ({CXX_FORBIDDEN_TOKEN.search(code)[0]})")
    for include in _C_INCLUDE.finditer(without_comments):
        literal = re.fullmatch(r'"([^"]*)"|<([^>]*)>', include[1].strip())
        target = (literal[1] if literal[1] is not None else literal[2]) if literal else None
        if target is None or not target.endswith(".h"):
            violations.append(f"{name}: include must be a literal .h path: {include[1].strip()}")
        elif target.startswith(("/", "\\")) or Path(target).is_absolute() or ".." in re.split(r"[/\\]", target):
            violations.append(f"{name}: include escapes the source roots: {target}")
        elif target.startswith("toolchain/gcc281pm/"):
            violations.append(f"{name}: upstream libgcc include outside the three pinned TUs")
    return violations


def record_observed_output(command: list[str], output: str | bytes | None, returncode: int) -> None:
    """Retain already invoked diagnostics/symbol reads; never add a tool invocation."""
    selected = os.environ.get("SHIREN2_REPLAY_EVIDENCE_ROOT")
    if not selected:
        return
    base = Path(selected)
    if not base.is_absolute() or base.is_symlink():
        raise ValueError("Observed tool evidence requires an absolute private root")
    base.mkdir(parents=True, exist_ok=True)
    destination = Path(tempfile.mkdtemp(prefix="observed-tool-", dir=base))
    record = {"argv": command, "cwd": str(Path.cwd()), "returncode": returncode,
        "environment_inherited": True,
        "recorded_environment": {key: os.environ.get(key) for key in ("PATH", "LC_ALL", "LANG", "TZ", "TMPDIR", "PYTHONDONTWRITEBYTECODE", "VR4300MUL", "DYLD_LIBRARY_PATH")},
        "driver_children": "Internal children untraced; no extra flags/diagnostic calls"}
    (destination / "command.json").write_text(json.dumps(record, indent=2) + "\n")
    (destination / "stdout.txt").write_bytes(output.encode() if isinstance(output, str) else output or b"")


def tool_profile(tool_root: Path, source_root: Path | None = None) -> dict[str, Any]:
    """Measure fixed tool locations and source-declared translation-unit profiles.

    A profile's optional ``language`` is ``c`` (default) or ``c++``. A C++ profile must
    disable exceptions and RTTI; its TUs are ``.cpp`` (C TUs ``.c``), and the driver selects
    the front end from that suffix. ``cc1plus`` is measured against its lock pin only when a
    declared C++ profile selects that compiler, so C-only declarations keep their profile.
    """
    source_root = source_root if source_root is not None else tool_root
    declaration = read_json(source_root / "config/compiler_profiles.json")
    if set(declaration) != {"schema_version", "profiles", "tu_profiles", "compiler_environment"} or type(declaration["schema_version"]) is not int or declaration["schema_version"] != 1:
        raise ValueError("Invalid compiler profile declaration")
    if declaration["compiler_environment"] != {"VR4300MUL": "ON"}:
        raise ValueError("Undeclared compiler environment override")
    profile_fields = {"assembler", "compiler_flags", "assembler_flags", "scope"}
    for key, item in declaration["profiles"].items():
        if not isinstance(key, str) or not isinstance(item, dict) or not profile_fields <= set(item) <= profile_fields | {"compiler", "language"}:
            raise ValueError("Invalid translation-unit profile")
        if item["assembler"] not in ASSEMBLERS or not isinstance(item["scope"], str):
            raise ValueError("Unsupported translation-unit assembler")
        if item.get("compiler", DEFAULT_COMPILER) not in COMPILERS:
            raise ValueError("Unsupported translation-unit compiler")
        for field in ["compiler_flags", "assembler_flags"]:
            flags = item[field]
            if not isinstance(flags, list) or not flags or any(not isinstance(flag, str) or not flag for flag in flags):
                raise ValueError("Invalid profile flags")
            if any(flag in {"-o", "-M", "-S", "-c"} or flag.startswith("@") for flag in flags):
                raise ValueError("Profile flags may not override build operations")
        if any(flag.startswith("-B") for flag in item["compiler_flags"]):
            raise ValueError("Profile flags may not override compiler pass selection")
        if any(flag.startswith("-x") for flag in item["compiler_flags"]):
            raise ValueError("Profile flags may not override language selection")
        if item.get("language", "c") not in LANGUAGES:
            raise ValueError("Unsupported translation-unit language")
        if item.get("language") == "c++" and not set(CXX_REQUIRED_FLAGS) <= set(item["compiler_flags"]):
            raise ValueError("C++ profiles must disable exceptions and RTTI")
    for source, key in declaration["tu_profiles"].items():
        relative = Path(source)
        if (relative.is_absolute() or ".." in relative.parts or relative.parts[0] != "src" or key not in declaration["profiles"]
                or relative.suffix != COMPILED_SUFFIXES[LANGUAGES[declaration["profiles"][key].get("language", "c")]]):
            raise ValueError("Invalid translation-unit profile assignment")
    toolchain_library.validate_bindings(declaration)
    lock = read_json(source_root / "toolchain.lock.json")

    def installed(entry: str, *names: str) -> dict[str, Path]:
        relative = Path(lock[entry]["install_directory"])
        if relative.is_absolute() or relative.parts[0] != ".cache" or ".." in relative.parts:
            raise ValueError(f"Invalid install directory for {entry}")
        return {name: tool_root / relative / name for name in names}

    tools = {}
    for name in ["as", "ld", "objcopy", "nm", "objdump"]:
        path = tool_root / ".cache/binutils/bin" / f"mips64-elf-{name}"
        if not path.is_file():
            raise ValueError(f"Missing pinned tool: {name}; run setup first")
        tools[name] = {"path": str(path.resolve()), "sha256": sha256(path)}
    # Measure only the registry entries some declared profile selects; each must match its lock pin.
    for name in sorted({item["assembler"] for item in declaration["profiles"].values()} & set(PINNED_ASSEMBLERS)):
        entry = PINNED_ASSEMBLERS[name]
        path = installed(entry, "as")["as"]
        if not path.is_file() or sha256(path) != lock[entry]["assembler_sha256"]:
            raise ValueError(f"Missing or changed pinned assembler {name}; run setup first")
        tools[name] = {"path": str(path.resolve()), "sha256": sha256(path)}
    compilers = {}
    cxx_compilers = {item.get("compiler", DEFAULT_COMPILER) for item in declaration["profiles"].values() if item.get("language") == "c++"}
    for key in sorted({DEFAULT_COMPILER} | {item.get("compiler", DEFAULT_COMPILER) for item in declaration["profiles"].values()}):
        entry = COMPILERS[key]
        compilers[key] = {}
        passes = ("gcc", "cc1", "cpp", "cc1plus") if key in cxx_compilers else ("gcc", "cc1", "cpp")
        for name, path in installed(entry, *passes).items():
            if not path.is_file() or sha256(path) != lock[entry]["binaries_sha256"].get(name):
                raise ValueError(f"Missing or changed pinned compiler {key}: {name}; run setup first")
            compilers[key][name] = {"path": str(path.resolve()), "sha256": sha256(path)}
    diagnostic = [tools["ld"]["path"], "-V"]
    try:
        supported = subprocess.check_output(diagnostic, text=True)
    except subprocess.CalledProcessError as error:
        record_observed_output(diagnostic, error.output, error.returncode)
        raise
    record_observed_output(diagnostic, supported, 0)
    emulation = next((name for name in ["elf32ebmip", "elf32btsmip"] if name in supported.split()), None)
    if emulation is None:
        raise ValueError("Linker has no big-endian ELF32 MIPS emulation")
    # "compiler" stays the default (COMPILER_PATH) compiler for existing consumers.
    result = {"id": PROFILE_ID, "scope": "Explicit per-TU measured assignments; whole-game compiler provenance remains unproved", "tools": tools, "compiler": compilers[DEFAULT_COMPILER], "compilers": compilers, "compiler_flags": COMPILER_FLAGS, "assembler_flags": ASSEMBLER_FLAGS, "ld_emulation": emulation, "profiles": declaration["profiles"], "tu_profiles": declaration["tu_profiles"], "compiler_environment": declaration["compiler_environment"]}
    if (source_root / generated_symbols.CONFIG).exists():
        result["symbol_identities"] = generated_symbols.profile_policy(source_root)
    return result


def profile_toolchain(profile: dict[str, Any], profile_id: str) -> tuple[list[str], str]:
    """Return (compiler driver argv prefix, assembler path) for one declared profile.

    The default compiler finds its passes through COMPILER_PATH. Any other
    compiler gets ``-B<its directory>/``, which GCC searches before
    COMPILER_PATH, so its own cpp/cc1 run instead of the default's.
    """
    assigned = profile["profiles"][profile_id]
    key = assigned.get("compiler", DEFAULT_COMPILER)
    gcc = profile["compilers"][key]["gcc"]["path"]
    driver = [gcc] if key == DEFAULT_COMPILER else [gcc, f"-B{Path(gcc).parent}/"]
    return driver, profile["tools"][assigned["assembler"]]["path"]


def profile_kind(profile: dict[str, Any], profile_id: str) -> str:
    """Compiled source kind (`c` or `cpp`) a declared profile compiles."""
    return LANGUAGES[profile["profiles"][profile_id].get("language", "c")]


def runtime_fingerprint() -> dict[str, Any]:
    """Hash installed distribution files; standard-library closure is separate."""
    packages = {}
    for distribution in importlib.metadata.distributions():
        name = re.sub(r"[-_.]+", "-", distribution.metadata["Name"]).lower()
        if name in packages or distribution.files is None:
            raise ValueError(f"Missing or ambiguous runtime distribution inventory: {name}")
        files = {}
        for relative in distribution.files:
            if "__pycache__" in relative.parts or relative.suffix in {".pyc", ".pyo"}:
                continue
            path = Path(distribution.locate_file(relative))
            if not path.is_file():
                raise ValueError(f"Missing installed package file: {name}/{relative}")
            files[str(relative)] = sha256(path)
        packages[name] = {"version": distribution.version, "files_sha256": json_digest(files), "file_count": len(files)}
    python = Path(sys.executable)
    return {"python": {"path": str(python.resolve()), "venv_prefix": str(Path(sys.prefix).resolve()), "binary_sha256": sha256(python), "version": sys.version}, "packages": packages, "scope": "interpreter and installed package files; standard library/shared-library closure pending"}


def orchestration_sources(tool_directory: Path) -> dict[str, str]:
    """Inventory trusted local Python sources, not a claim of full runtime closure."""
    result = {}
    for path in sorted(tool_directory.rglob("*.py")):
        if "__pycache__" in path.parts:
            continue
        if path.is_symlink():
            raise ValueError("Trusted orchestration sources must not be symlinks")
        result[path.relative_to(tool_directory).as_posix()] = sha256(path)
    if "build.py" not in result:
        raise ValueError("Trusted orchestration entry source is absent")
    return result


def archive_orchestration(directory: Path, entrypoint: Path) -> dict[str, Any]:
    """Freeze declared source provenance, never execution attestation or authority."""
    trusted = Path(__file__).resolve().parent
    if entrypoint != trusted / "build.py":
        raise ValueError("Build entry and trusted certification implementation disagree")
    files = orchestration_sources(trusted)
    archive = directory / "trusted-orchestration"
    archive.mkdir(exist_ok=False)
    for relative in files:
        target = archive / relative
        target.parent.mkdir(parents=True, exist_ok=True)
        shutil.copy2(trusted / relative, target)
    record = {"kind": "declared-trusted-orchestration-provenance", "entrypoint": str(entrypoint),
              "entrypoint_sha256": files["build.py"], "child_argv": list(sys.orig_argv),
              "source_files": files}
    validate_orchestration(directory, record)
    return record


def validate_orchestration(directory: Path, record: dict[str, Any]) -> None:
    """Check frozen provenance without trusting or executing its declared paths."""
    if (not isinstance(record, dict)
            or set(record) != {"kind", "entrypoint", "entrypoint_sha256", "child_argv", "source_files"}
            or record["kind"] != "declared-trusted-orchestration-provenance"
            or not isinstance(record["entrypoint"], str)
            or not Path(record["entrypoint"]).is_absolute()
            or Path(record["entrypoint"]).name != "build.py"
            or not isinstance(record["entrypoint_sha256"], str)
            or re.fullmatch(r"[0-9a-f]{64}", record["entrypoint_sha256"]) is None
            or not isinstance(record["child_argv"], list) or not record["child_argv"]
            or any(not isinstance(arg, str) for arg in record["child_argv"])):
        raise ValueError("Invalid trusted orchestration provenance")
    # No receipt path chooses the verifier or archive location. Hashes are frozen
    # evidence, never permission to execute historical or substituted tooling.
    files = record["source_files"]
    if (not isinstance(files, dict) or files.get("build.py") != record["entrypoint_sha256"]
            or file_inventory(checked_path(directory, "trusted-orchestration")) != files):
        raise ValueError("Changed trusted orchestration source archive")


def validated_process_runtime(directory: Path, record: object) -> dict[str, Any]:
    """Separate process identity only after checking the required frozen provenance."""
    if not isinstance(record, dict) or "orchestration" not in record:
        raise ValueError("New build receipt omits trusted orchestration provenance")
    runtime = dict(record)
    validate_orchestration(directory, runtime.pop("orchestration"))
    return runtime


def image_evidence(source_root: Path, reference: bytes) -> tuple[ImageInventory, dict[str, Any]]:
    """Validate image policy and recompute every original-byte evidence digest."""
    attest(reference)
    path = source_root / "config/images.json"
    inventory = load_inventory(path)
    for image in inventory.images.values():
        if hashlib.sha256(reference[image.rom_start:image.rom_end]).hexdigest() != image.rom_sha256:
            raise ValueError(f"Original image digest disagrees: {image.image_id}")
    for component in inventory.components:
        if hashlib.sha256(reference[component.rom_start:component.rom_end]).hexdigest() != component.rom_sha256:
            raise ValueError(f"Original component digest disagrees: {component.component_id}")
    for evidence in inventory.evidence.values():
        if hashlib.sha256(reference[evidence.rom_start:evidence.rom_end]).hexdigest() != evidence.sha256:
            raise ValueError(f"Original loader evidence digest disagrees: {evidence.evidence_id}")
    return inventory, {"config_sha256": sha256(path), "inventory": read_json(path), "validation": "fixed target and every image/component/loader evidence interval rehashed"}


def validate_build_catalogue(directory: Path, reference: bytes, matches: list[dict[str, Any]],
                             graph: dict[str, dict[str, str]], profile: dict[str, Any],
                             rom_map: rommap.RomMap) -> None:
    """Bind the declared map to THIS build's independently parsed original functions."""
    from packet import enumerate_build_functions
    from progress import catalogue_identity
    if graph != build_graph(directory):
        raise ValueError("CPU denominator graph differs from this build")
    inventory, _ = image_evidence(directory / "source", reference)
    functions = enumerate_build_functions(directory, reference, graph, inventory, matches,
                                          file_inventory(directory / "generated"), profile)
    identity = catalogue_identity(inventory, functions)
    actual = {"identity": identity, "denominator_id": json_digest(identity)}
    rommap.check_catalogue(rom_map, actual)
    declared = read_json(directory / "source" / rommap.CATALOGUE_PATH)
    if declared["identity"] != identity or declared["denominator_id"] != actual["denominator_id"]:
        raise ValueError("Frozen CPU denominator differs from this build's original catalogue")


def rom_map_report(directory: Path, reference: bytes, matches: list[dict[str, Any]], *,
                   commands: list[list[str]], reproduction: dict[str, Any],
                   graph: dict[str, dict[str, str]], profile: dict[str, Any]) -> dict[str, Any]:
    """Receipt-bound research and source accounting; neither is project acceptance.

    Callers supply actual build/replay evidence. verify.py independently reproduces
    it before accepting this report; progress binds it to the accepted receipt.
    """
    source = directory / "source"
    validate_asset_recipe(source, commands[:2])
    if commands != expected_commands(graph, profile):
        raise ValueError("Source coverage recipe disagrees with the declared build graph")
    manifest = read_json(directory / "input-manifest.json")
    from workspace import verify_manifest
    verify_manifest(source, manifest)
    pack_key = "assets/tail/manifest.json"
    pack_path = directory / "generated" / pack_key
    source_key = "assets/tail/source-manifest.json"
    source_digest = sha256(source / source_key)
    if manifest["files"].get(source_key) != {
            "sha256": source_digest, "size": (source / source_key).stat().st_size}:
        raise ValueError("Editable source manifest is not a frozen receipt input")
    pack_digest = sha256(pack_path)
    if reproduction.get("split", {}).get("generated", {}).get(pack_key) != pack_digest:
        raise ValueError("Tail pack manifest is not bound to full-link replay")
    if reproduction.get("linked", {}).get("shiren2.z64") != sha256(directory / "shiren2.z64"):
        raise ValueError("Source coverage requires the corresponding full-ROM replay")
    rom_map = rommap.load(source, reference)
    validate_build_catalogue(directory, reference, matches, graph, profile, rom_map)
    accounting = rommap.account(rom_map, reference, source, matches, read_json(source / rommap.RULES_PATH),
                                directory)
    return {"path": rommap.MAP_PATH, "sha256": sha256(source / rommap.MAP_PATH), "rules_sha256": rom_map.rules_sha256,
            "cpu_catalogue_id": rom_map.cpu_catalogue_id, **accounting,
            "asset_roundtrip": {"path": "generated/" + pack_key, "sha256": pack_digest,
                                "source_manifest": {"path": source_key, "sha256": source_digest},
                                "input_manifest_sha256": manifest["sha256"],
                                "reproduction_sha256": json_digest(reproduction),
                                "encoder": read_json(pack_path)["encoder"]}}


def image_matches(matches: list[dict[str, Any]], inventory: ImageInventory) -> dict[str, int]:
    counts = validate_matches(matches, policy_records(inventory))
    for match in matches:
        name = match.get("image_id", "resident")
        image = require_cpu_range(name, match["rom_start"], match["size"], inventory)
        if image.split_status != "active" or image_for_source(match["source"], inventory) != image:
            raise ValueError("C match lacks an active image/source binding")
    return counts


def storage_elf_layout(path: Path) -> dict[str, Any]:
    """Read actual ELF32 sections/load records for the binary-only storage proof."""
    data = path.read_bytes()
    if len(data) < 52 or data[:7] != b"\x7fELF\x01\x02\x01" or struct.unpack_from(">H", data, 18)[0] != 8:
        raise ValueError("Storage evidence must be ELF32 big-endian MIPS")
    phoff, shoff = struct.unpack_from(">II", data, 28)
    phsize, phcount, shsize, shcount, strindex = struct.unpack_from(">HHHHH", data, 42)
    if shsize != 40 or not shcount or strindex >= shcount or shoff + shsize * shcount > len(data) or (phcount and (phsize != 32 or phoff + phsize * phcount > len(data))):
        raise ValueError("Invalid ELF storage tables")
    raw = [struct.unpack_from(">IIIIIIIIII", data, shoff + index * shsize) for index in range(shcount)]
    strings = raw[strindex]
    if strings[1] != 3 or strings[4] + strings[5] > len(data):
        raise ValueError("Invalid ELF section string table")
    names = data[strings[4]:strings[4] + strings[5]]
    sections = []
    for index, fields in enumerate(raw):
        name, kind, flags, address, offset, size, link, info, alignment, entsize = fields
        end = names.find(b"\0", name)
        if name >= len(names) or end < 0 or (kind != 8 and offset + size > len(data)):
            raise ValueError("Invalid ELF storage section")
        sections.append({"index": index, "name": names[name:end].decode("ascii"), "type": kind, "flags": flags,
            "vram_start": address, "file_offset": offset, "size": size, "alignment": alignment,
            "link": link, "info": info, "entry_size": entsize,
            "sha256": hashlib.sha256(data[offset:offset + size]).hexdigest() if kind != 8 else None})
    if len({item["name"] for item in sections}) != len(sections):
        raise ValueError("Ambiguous ELF storage section identity")
    loads = []
    for index in range(phcount):
        kind, offset, address, physical, filesz, memsz, flags, alignment = struct.unpack_from(">IIIIIIII", data, phoff + index * phsize)
        if kind == 1:
            if filesz > memsz or offset + filesz > len(data):
                raise ValueError("Invalid ELF load storage")
            loads.append({"index": index, "file_offset": offset, "vram_start": address, "lma_start": physical,
                "file_size": filesz, "memory_size": memsz, "flags": flags, "alignment": alignment})
    symbols = {}
    for section in sections:
        if section["type"] != 2:
            continue
        if section["entry_size"] != 16 or section["size"] % 16 or section["link"] >= len(sections):
            raise ValueError("Invalid ELF storage symbol table")
        string = sections[section["link"]]
        if string["type"] != 3:
            raise ValueError("Invalid ELF symbol names")
        table = data[string["file_offset"]:string["file_offset"] + string["size"]]
        for offset in range(section["file_offset"], section["file_offset"] + section["size"], 16):
            name, value, size, info, other, owner = struct.unpack_from(">IIIBBH", data, offset)
            end = table.find(b"\0", name)
            if name >= len(table) or end < 0:
                raise ValueError("Invalid ELF storage symbol name")
            if not owner or not name:
                continue
            label = table[name:end].decode("ascii")
            symbols.setdefault(label, []).append({"value": value, "size": size, "section_index": owner})
    return {"sha256": hashlib.sha256(data).hexdigest(), "sections": sections, "loads": loads, "symbols": symbols}


def storage_load_mapping(section: dict[str, Any], loads: list[dict[str, Any]]) -> dict[str, Any]:
    candidates = []
    for load in loads:
        relative = section["file_offset"] - load["file_offset"]
        if 0 <= relative and relative + section["size"] <= load["file_size"] and load["vram_start"] + relative == section["vram_start"]:
            candidates.append({"program_header": load["index"], "lma_start": load["lma_start"] + relative,
                "lma_end": load["lma_start"] + relative + section["size"], "file_offset": section["file_offset"],
                "load": load})
    if len(candidates) != 1:
        raise ValueError("Storage section has no unique actual ELF load mapping")
    return candidates[0]


def binary_storage_record(directory: Path, image: Any, section_name: str, graph: dict[str, dict[str, str]], layout: dict[str, Any], reference: bytes, *, allowed_sections: set[str] | None = None) -> dict[str, Any]:
    """Bind the sole supported overlay's complete binary object/map/load bytes."""
    owned = {name: item for name, item in graph.items() if item["image_id"] == image.image_id}
    name = "obj/generated/assets/overlay_136dc0_initialized_opaque.o"
    expected = {"source": "generated/assets/overlay_136dc0_initialized_opaque.bin", "input": "generated/assets/overlay_136dc0_initialized_opaque.bin", "source_kind": "binary", "image_id": image.image_id}
    if owned != {name: expected}:
        raise ValueError("Active B storage requires its one uniquely owned binary object and zero C")
    original = reference[image.rom_start:image.rom_end]
    asset = checked_path(directory, expected["input"])
    if asset.read_bytes() != original:
        raise ValueError("B binary input disagrees with complete original initialized storage")
    object_path = checked_path(directory, name)
    obj = storage_elf_layout(object_path)
    allocated = [part for part in obj["sections"] if part["flags"] & 2 and part["size"]]
    if len(allocated) != 1 or allocated[0]["name"] != ".data" or allocated[0]["type"] != 1 or allocated[0]["flags"] != 3 or allocated[0]["vram_start"] != 0 or allocated[0]["size"] != len(original) or allocated[0]["sha256"] != hashlib.sha256(original).hexdigest():
        raise ValueError("B binary object has extra, missing or changed natural storage")
    if any(part["type"] in {4, 9} and part["size"] for part in obj["sections"]):
        raise ValueError("B binary storage object must not contain relocations")
    selected = [part for part in layout["sections"] if part["name"] == section_name]
    if len(selected) != 1:
        raise ValueError("B storage output section is missing or ambiguous")
    section = selected[0]
    if section["type"] != 1 or not section["flags"] & 2 or section["vram_start"] != image.vram_start or section["size"] != len(original) or section["sha256"] != hashlib.sha256(original).hexdigest():
        raise ValueError("B actual output section disagrees with complete initialized image")
    mapping = storage_load_mapping(section, layout["loads"])
    if mapping["lma_start"] != image.rom_start or mapping["lma_end"] != image.rom_end:
        raise ValueError("B actual ELF load address disagrees with original ROM storage")
    boundaries = {}
    for suffix, value in (("ROM_START", image.rom_start), ("ROM_END", image.rom_end)):
        symbol = f"overlay_b_136dc0_storage_{suffix}"
        records = layout["symbols"].get(symbol, [])
        if len(records) != 1 or records[0]["value"] != value:
            raise ValueError("B actual generated ROM boundary symbol disagrees")
        boundaries[symbol] = records[0]
    map_text = (directory / "shiren2.map").read_text()
    output = re.findall(rf"^{re.escape(section_name)}\s+(0x[0-9a-fA-F]+)\s+(0x[0-9a-fA-F]+)\s+load address\s+(0x[0-9a-fA-F]+)\s*$", map_text, re.MULTILINE)
    entries = re.findall(rf"^\s+\.data\s+(0x[0-9a-fA-F]+)\s+(0x[0-9a-fA-F]+)\s+{re.escape(name)}\s*$", map_text, re.MULTILINE)
    if len(output) != 1 or tuple(int(value, 16) for value in output[0]) != (image.vram_start, len(original), image.rom_start) or len(entries) != 1 or tuple(int(value, 16) for value in entries[0]) != (image.vram_start, len(original)):
        raise ValueError("B actual output/map binary contribution disagrees with original mapping")
    for part in layout["sections"]:
        if part["name"] == section_name or not part["flags"] & 2 or not part["size"]:
            continue
        if part["name"] not in (allowed_sections if allowed_sections is not None else {section_name}):
            raise ValueError("Allocated ELF metadata/runtime storage has no declared section owner")
        overlaps_ram = part["vram_start"] < image.vram_end and image.vram_start < part["vram_start"] + part["size"]
        overlaps_rom = False
        if part["type"] != 8:
            other = storage_load_mapping(part, layout["loads"])
            overlaps_rom = other["lma_start"] < image.rom_end and image.rom_start < other["lma_end"]
        if overlaps_ram or overlaps_rom:
            raise ValueError("Extra allocated ELF storage overlaps the B image")
    return {"stage": "binary-storage-only", "C_credit": 0, "output_section": section,
        "load_mapping": mapping, "ROM_boundary_symbols": boundaries,
        "object": name, "owner": image.image_id, "binary_input_sha256": sha256(asset),
        "object_sha256": obj["sha256"], "natural_object_sections": obj["sections"],
        "map_contribution": {"input_section": ".data", "vram_start": image.vram_start, "size": len(original), "object": name},
        "map_sha256": sha256(directory / "shiren2.map")}


def opaque_storage_segment(segment: dict[str, Any]) -> bool:
    """The stored tail bin uses ROM offsets as ELF bookkeeping VMAs, never RAM identity.

    It carries its own exclusive_ram_id. Otherwise splat's global ROM hull spans the
    overlay ROM, and spimdisasm resolves overlay vroms to the global segment (no
    branch labels, symbols shared between the two same-VRAM overlays).
    """
    if segment.get("type") != "bin":
        return False
    if (segment.get("name") != "remaining" or set(segment) != {"name", "type", "start", "vram", "exclusive_ram_id"}
            or segment["start"] != 0x148460 or segment["vram"] != 0x148460 or segment["exclusive_ram_id"] != "rom_tail_storage"):
        raise ValueError("Unsupported opaque binary storage VMA declaration")
    return True


def linked_image_records(directory: Path, inventory: ImageInventory, reference: bytes) -> dict[str, Any]:
    """Bind actual initialized ELF bytes and NOLOAD extents to loader authority."""
    data = (directory / "shiren2.elf").read_bytes()
    elf = Elf32File(data)
    sections = {}
    for section in elf.sectionHeaders:
        name = elf.shstrtab[section.name]
        if name in sections:
            raise ValueError("Ambiguous linked ELF section identity")
        sections[name] = section
    config = yaml.safe_load((directory / "source/config/shiren2.jp.yaml").read_text())
    result = {name: {"initialized": [], "bss": []} for name, image in inventory.images.items() if image.split_status == "active"}
    for index, segment in enumerate(config["segments"][:-1]):
        if not isinstance(segment, dict) or opaque_storage_segment(segment):
            continue
        image = image_for_segment(segment["name"], inventory)
        if image is None or image.image_id not in result:
            raise ValueError("Linked section belongs to an inactive/unknown image")
        next_segment = config["segments"][index + 1]
        start = segment["start"]
        end = next_segment[0] if isinstance(next_segment, list) else next_segment["start"]
        section = sections.get(f".{segment['name']}")
        if section is None or section.type != 1 or not section.flags & 2 or section.addr != start + image.vram_rom_delta or section.size != end - start:
            raise ValueError("Actual initialized ELF section disagrees with reviewed image mapping")
        body = data[section.offset:section.offset + section.size]
        if len(body) != section.size or body != reference[start:end]:
            raise ValueError("Actual initialized image bytes disagree with original storage")
        record = {"section": f".{segment['name']}", "rom_start": start, "rom_end": end, "vram_start": section.addr, "vram_end": section.addr + section.size, "sha256": hashlib.sha256(body).hexdigest()}
        result[image.image_id]["initialized"].append(record)
        bss = sections.get(f".{segment['name']}_bss")
        if bss is not None and bss.size:
            if bss.type != 8 or not bss.flags & 2:
                raise ValueError("Image BSS must be actual allocated ELF NOBITS storage")
            result[image.image_id]["bss"].append({"section": f".{segment['name']}_bss", "vram_start": bss.addr, "vram_end": bss.addr + bss.size})
    for name, records in result.items():
        image = inventory.images[name]
        initialized = sorted(records["initialized"], key=lambda item: item["rom_start"])
        position = image.rom_start
        for item in initialized:
            if item["rom_start"] != position:
                raise ValueError("Active image initialized sections have a gap or overlap")
            position = item["rom_end"]
        if position != image.rom_end:
            raise ValueError("Active image initialized sections omit reviewed storage")
        observed_bss = sorted((item["vram_start"], item["vram_end"]) for item in records["bss"])
        expected_bss = sorted((part.vram_start, part.vram_end) for part in image.bss_ranges)
        if observed_bss != expected_bss:
            raise ValueError(f"Actual linked BSS disagrees with reviewed loader extent: {name}")
    return result


def splat_bindings(source_root: Path, inventory: ImageInventory) -> tuple[dict[str, str], dict[str, str]]:
    """Bind original initialized sections and C declarations from frozen YAML."""
    config = yaml.safe_load((source_root / "config/shiren2.jp.yaml").read_text())
    sections: dict[str, str] = {}
    sources: dict[str, str] = {}
    stems: set[str] = set()
    segments = config["segments"]
    for index, segment in enumerate(segments[:-1]):
        if isinstance(segment, list) or (isinstance(segment, dict) and opaque_storage_segment(segment)):
            if isinstance(segment, dict):
                next_segment = segments[index + 1]
                end = next_segment[0] if isinstance(next_segment, list) else next_segment["start"]
                if end != 0x2000000:
                    raise ValueError("Opaque storage bin omits exact original ROM interval")
                segment = [segment["start"], "bin", segment["name"]]
            if len(segment) != 3 or segment[1] != "bin":
                raise ValueError("Unsupported top-level splat storage declaration")
            if segment[2] in sections:
                raise ValueError("Duplicate splat storage/section owner")
            sections[segment[2]] = f"rom_storage:{segment[2]}"
            continue
        if not isinstance(segment, dict) or segment.get("type") != "code":
            raise ValueError("Unsupported splat image declaration")
        name = segment["name"]
        image = image_for_segment(name, inventory)
        next_segment = segments[index + 1]
        end = next_segment[0] if isinstance(next_segment, list) else next_segment["start"]
        start, address = segment["start"], segment["vram"]
        if image is None or image.split_status != "active" or type(start) is not int or type(address) is not int or type(end) is not int or not image.rom_start <= start < end <= image.rom_end or address != start + image.vram_rom_delta:
            raise ValueError(f"Splat section lacks an exact active image mapping: {name}")
        if name in sections or f"{name}_bss" in sections:
            raise ValueError("Duplicate splat image/section owner")
        if image.image_id in OVERLAY_IMAGES:
            bss = sum(part.vram_end - part.vram_start for part in image.bss_ranges)
            allowed = {"name", "type", "start", "vram", "exclusive_ram_id", "symbol_name_format", "subalign", "bss_size", "subsegments"}
            # Optional `align`: splat ends each section (and the segment) with `. = ALIGN(., align)`, 16 for
            # code by default. A smaller power of two lets a byte-placed section end stay put (overlay A
            # .data ends at 0x801E7EA4); the full-ROM link still has to prove every address.
            align = segment.get("align", 16)
            if (name != image.image_id or start != image.rom_start or end != image.rom_end or set(segment) - {"align"} != allowed
                    or segment["exclusive_ram_id"] != image.load_slot or segment["symbol_name_format"] != "$VRAM_$ROM"
                    or segment["subalign"] != 1 or segment["bss_size"] != bss or type(align) is not int or align not in {1, 2, 4, 8, 16}):
                raise ValueError(f"Unsupported overlay splat layout: {name} needs its image name and exact ROM interval, "
                                 "the shared slot id, ROM-qualified names, subalign 1, the reviewed BSS size and an optional "
                                 "power-of-two align of at most 16")
        sections[name] = sections[f"{name}_bss"] = image.image_id
        for part in segment["subsegments"]:
            if isinstance(part, list) and len(part) >= 3 and part[1] in COMPILED_KINDS:
                source = f"src/{part[2]}{COMPILED_SUFFIXES[part[1]]}"
                stem = f"src/{part[2]}"
                if stem in stems or image_for_source(source, inventory) != image:
                    raise ValueError(f"{LANGUAGE_LABELS[part[1]]} translation-unit image declaration is missing or ambiguous")
                sources[source] = image.image_id
                stems.add(stem)
    for image in inventory.images.values():
        if image.split_status == "active" and any(name not in sections for name in image.splat_segments):
            raise ValueError(f"Active image has an omitted splat segment: {image.image_id}")
    return sections, sources


def build_graph(directory: Path) -> dict[str, dict[str, str]]:
    """Derive declared object/source kinds from the private linker script.

    splat names every compiled object `obj/source/src/<unit>.o` (`o_as_suffix`), so the
    frozen YAML subsegment type (`c` or `cpp`) decides the source file and kind.
    """
    text = (directory / "shiren2.ld").read_text()
    names = sorted(set(re.findall(r"([A-Za-z0-9_./-]+\.o)\(", text)))
    if not names:
        raise ValueError("Splat linker script contains no input objects")
    inventory = load_inventory(directory / "source/config/images.json")
    section_images, c_sources = splat_bindings(directory / "source", inventory)
    compiled_by_stem = {Path(source).with_suffix("").as_posix(): source for source in c_sources}
    ownership: dict[str, set[str]] = {}
    for block in re.finditer(r"^\s+\.([A-Za-z0-9_]+)[^\n]*\n\s*\{\n(.*?)^\s+\}", text, re.MULTILINE | re.DOTALL):
        if block[1] not in section_images:
            raise ValueError(f"Linker section has no declared storage/image owner: {block[1]}")
        for name in re.findall(r"([A-Za-z0-9_./-]+\.o)\(", block[2]):
            ownership.setdefault(name, set()).add(section_images[block[1]])
    graph = {}
    for name in names:
        path = Path(name)
        checked_path(directory, name)
        if path.parts[0] != "obj":
            raise ValueError(f"Object is outside private object root: {name}")
        base = Path(*path.parts[1:]).with_suffix("")
        if base.parts[:2] == ("source", "src"):
            canonical = compiled_by_stem.get(Path(*base.parts[1:]).as_posix(), Path(*base.parts[1:]).as_posix() + ".c")
            source = f"source/{canonical}"
            source_kind = compiled_kind(canonical)
        elif base.parts[:2] == ("generated", "asm"):
            source = str(base) + ".s"
            source_kind = "asm"
            canonical = source
        elif base.parts[:2] == ("generated", "assets"):
            source = str(base) + ".bin"
            source_kind = "binary"
            canonical = source
        else:
            raise ValueError(f"Undeclared source kind for object: {name}")
        if not checked_path(directory, source).is_file():
            raise ValueError(f"Missing declared {source_kind} input: {source}")
        owners = ownership.get(name, set())
        if len(owners) != 1:
            raise ValueError(f"Object has no unique original image/storage owner: {name}")
        owner = next(iter(owners))
        if source_kind in COMPILED_KINDS and c_sources.get(canonical) != owner:
            raise ValueError(f"{LANGUAGE_LABELS[source_kind]} object disagrees with frozen image/source declaration")
        graph[name] = {"source": canonical, "input": source, "source_kind": source_kind, "image_id": owner}
    if {item["source"] for item in graph.values() if item["source_kind"] in COMPILED_KINDS} != set(c_sources):
        raise ValueError("Declared C/C++ translation unit is absent from object graph")
    return graph


def compiler_environment(directory: Path, profile: dict[str, Any]) -> dict[str, str]:
    return {"PATH": os.defpath, "LC_ALL": "C", "TZ": "UTC", "COMPILER_PATH": str(Path(profile["compiler"]["gcc"]["path"]).parent), "TMPDIR": str(directory / "tmp"), **profile["compiler_environment"]}


def splat_options(directory: Path, reference: Path) -> dict[str, Any]:
    return {"options": {
        "base_path": str(directory.resolve()), "target_path": str(reference.resolve()),
        "build_path": "obj", "asm_path": "generated/asm", "asset_path": "generated/assets",
        "src_path": "source/src", "cache_path": "generated/.splache", "ld_script_path": "shiren2.ld",
        "undefined_funcs_auto_path": "generated/undefined_funcs_auto.txt", "undefined_syms_auto_path": "generated/undefined_syms_auto.txt",
        "symbol_addrs_path": "source/config/symbol_addrs.txt", "reloc_addrs_path": "source/config/reloc_addrs.txt",
        "create_c_files": False, "generate_asm_macros_files": False,
    }}


def intermediate_keys(graph: dict[str, dict[str, str]], matches: list[dict[str, Any]], source_root: Path | None = None,
                      observed: Any = ()) -> tuple[set[str], set[str]]:
    """Expected private split/compiler evidence for the current splat policy.

    For a C/C++ unit that owns a `.rodata` subsegment, splat also writes one
    `asm/nonmatchings/<unit>/<symbol>.s` per rodata symbol; their names come from the
    split itself, so `observed` keys under that prefix are accepted (the fresh-split
    replay independently reproduces the whole generated inventory and its hashes).
    """
    generated = {"undefined_funcs_auto.txt", "undefined_syms_auto.txt"}
    compiled: set[str] = set()
    for item in graph.values():
        if item["source_kind"] in COMPILED_KINDS:
            base = Path(item["source"])
            compiled.update({base.with_suffix(".s").as_posix(), base.with_suffix(".d").as_posix()})
        else:
            generated.add(Path(item["input"]).relative_to("generated").as_posix())
    for match in matches:
        unit = Path(match["source"]).relative_to("src").with_suffix("")
        generated.add((Path("asm/matchings") / unit / f"{match['symbol']}.s").as_posix())
    if source_root is not None:
        generated.update(tail_generated_keys(source_root))
        config = yaml.safe_load((source_root / "config/shiren2.jp.yaml").read_text())
        c_units = {Path(item["source"]).relative_to("src").with_suffix("").as_posix() for item in graph.values() if item["source_kind"] in COMPILED_KINDS}
        for segment in config["segments"]:
            if isinstance(segment, dict) and not opaque_storage_segment(segment):
                for part in segment["subsegments"]:
                    if isinstance(part, list) and len(part) >= 3:
                        kind, name = part[1], part[2]
                    elif isinstance(part, dict):
                        kind, name = part.get("type"), part.get("name")
                    else:
                        continue
                    if kind in {"data", "rodata", "bss", ".data", ".rodata", ".bss"} and name in c_units:
                        generated.add(f"asm/data/{name}.{kind.lstrip('.')}.s")
                        if kind == ".rodata":
                            generated.update(key for key in observed if key.startswith(f"asm/nonmatchings/{name}/") and key.endswith(".s"))
    return generated, compiled


def object_commands(name: str, item: dict[str, str], profile: dict[str, Any]) -> list[list[str]]:
    tools = profile["tools"]
    source = item["input"]
    if item["source_kind"] == "binary":
        return [[tools["ld"]["path"], "-m", profile["ld_emulation"], "-r", "-b", "binary", "-o", name, source]]
    if item["source_kind"] == "asm":
        return [[tools["as"]["path"], *ASSEMBLER_FLAGS, "-o", name, source]]
    key = profile["tu_profiles"].get(item["source"])
    if key is None:
        raise ValueError(f"No measured profile assigned to {LANGUAGE_LABELS[item['source_kind']]} translation unit: {item['source']}")
    if profile_kind(profile, key) != item["source_kind"]:
        raise ValueError(f"Profile {key} does not compile {LANGUAGE_LABELS[item['source_kind']]}: {item['source']}")
    assigned = profile["profiles"][key]
    driver, assembler = profile_toolchain(profile, key)
    compiled = str(Path("compiled") / Path(item["source"]).with_suffix(".s"))
    flags = [*assigned["compiler_flags"], *toolchain_library.extra_flags(item["source"])]
    return [
        [*driver, *flags, "-M", source],
        [*driver, *flags, "-S", source, "-o", compiled],
        [assembler, *assigned["assembler_flags"], "-o", name, compiled],
    ]


def split_commands() -> list[list[str]]:
    """Split, then rebuild the stored tail from decoded Yay0 sidecars (tools/assets.py pack)."""
    return [[python_command(), "-m", "splat", "split", "source/config/shiren2.jp.yaml", "splat.override.yaml", "--disassemble-all"],
            [python_command(), "-B", "source/tools/assets.py", "pack", "splat.override.yaml"]]


def validate_asset_recipe(source_root: Path, recipe: list[list[str]]) -> None:
    """Allow only reviewed split/preserve plus frozen-source pack, never import.

    Exact argv rejects extract, custom decoders and identity wrappers. Binding
    executed tooling to the independent verifier prevents a substituted pack
    implementation from reading the ROM and merely claiming frozen-source use.
    Splat and pack still read reference bytes for preserved ranges and equality;
    those ranges are explicit passthrough, not editable-source provenance.
    """
    trusted_root = Path(__file__).resolve().parent.parent
    if trusted_root.is_relative_to(source_root.resolve()):
        raise ValueError("Verifier must execute outside the snapshot under test from trusted tools")
    approved = [
        [python_command(), "-m", "splat", "split", "source/config/shiren2.jp.yaml", "splat.override.yaml", "--disassemble-all"],
        [python_command(), "-B", "source/tools/assets.py", "pack", "splat.override.yaml"],
    ]
    if recipe != approved:
        raise ValueError("Editable-source recipe must split then pack frozen sources; extraction/decoding at build or replay is forbidden")
    # Complete local closure reachable from assets.main -> pack. rommap's lazy
    # progress import is confined to account(), which pack never calls.
    # Installed yaml/spimdisasm/crunch64 and stdlib remain runtime-pinned inputs.
    for name in ("assets.py", "rommap.py", "rspaudio.py", "assetformat.py", "mipsflow.py", "gxxvtable.py", "rom.py", "images.py", "evidence.py", "toolchain_library.py", "generated_symbols.py"):
        frozen = checked_path(source_root, "tools/" + name)
        trusted = Path(__file__).resolve().parent / name
        if sha256(frozen) != sha256(trusted):
            raise ValueError(f"Editable-source recipe tool differs from the independent verifier: {name}")
    # The script directory precedes installed packages on sys.path. Reject added
    # top-level names/packages that could shadow the audited imports, and reject
    # bytecode/native loaders. -B prevents pack from adding caches after this gate.
    trusted_tools = trusted_root / "tools"
    namespace = {(path.name, path.is_dir()) for path in trusted_tools.iterdir()}
    for path in (source_root / "tools").iterdir():
        if path.name == "__pycache__" or (path.name, path.is_dir()) not in namespace:
            raise ValueError(f"Unreviewed snapshot import namespace: {path.name}")
    for path in (source_root / "tools").rglob("*"):
        if path.is_symlink() or path.name == "__pycache__" or path.suffix in {".pyc", ".pyo", ".so", ".dylib", ".pyd"}:
            raise ValueError(f"Snapshot Python must execute reviewed source, not alternate loaders: {path.name}")


def source_provenance_report(directory: Path, reference: bytes, recipe: list[list[str]]) -> dict[str, Any]:
    """Read-only frozen-input preflight, not a build, roundtrip or acceptance."""
    from assets import SOURCE_MANIFEST, read_source_manifest
    from workspace import verify_manifest
    source = directory / "source"
    validate_asset_recipe(source, recipe)
    manifest = read_json(directory / "input-manifest.json")
    verify_manifest(source, manifest)
    rom_map = rommap.load(source, reference)
    read_source_manifest(source, rom_map, reference)
    return {"status": "DECLARED_PROVENANCE_ONLY_NOT_EDITABILITY", "accepted": False, "build_source_bytes": 0,
            "editability_verified": False, "credit_requires_exact_reencode_and_accepted_replay": True,
            "map_id": rom_map.map_id, "partition_sha256": rom_map.partition_sha256,
            "evidence_policy_sha256": rom_map.evidence_policy_sha256,
            "input_manifest_sha256": manifest["sha256"],
            "source_manifest_sha256": sha256(source / SOURCE_MANIFEST),
            "recipe": recipe, "full_rom_build_or_replay_performed": False}


def expected_commands(graph: dict[str, dict[str, str]], profile: dict[str, Any]) -> list[list[str]]:
    commands = split_commands()
    for name, item in graph.items():
        commands.extend(object_commands(name, item, profile))
    commands.extend([
        # symbol_aliases.ld PROVIDEs library names (e.g. libgcc __udivdi3) for catalogued functions.
        [profile["tools"]["ld"]["path"], "-m", profile["ld_emulation"], "-e", "entrypoint", "-T", "shiren2.ld", "-T", "generated/undefined_funcs_auto.txt", "-T", "generated/undefined_syms_auto.txt", "-T", "source/config/symbol_aliases.ld", *generated_symbols.linker_flags(profile, graph), "-Map", "shiren2.map", "-o", "shiren2.elf"],
        [profile["tools"]["objcopy"]["path"], "-O", "binary", "shiren2.elf", "shiren2.z64"],
    ])
    return commands


def symbol_table(path: Path, profile: dict[str, Any]) -> dict[str, tuple[int, int]]:
    command = [profile["tools"]["nm"]["path"], "-S", "--defined-only", str(path)]
    try:
        output = subprocess.check_output(command, text=True)
    except subprocess.CalledProcessError as error:
        record_observed_output(command, error.output, error.returncode)
        raise
    record_observed_output(command, output, 0)
    # A segment's text and data share one output section, so nm reports data symbols as t/T too.
    # File-local names (e.g. `static float dtor` in two libultra files) can legitimately repeat;
    # they are dropped as unresolvable. Duplicate global names stay an error.
    symbols: dict[str, tuple[int, int]] = {}
    kinds: dict[str, str] = {}
    for line in output.splitlines():
        fields = line.split()
        if len(fields) == 4 and fields[2] in {"T", "t"}:
            name, kind = fields[3], fields[2]
            if name in kinds:
                if kind == "T" or kinds[name] == "T":
                    raise ValueError(f"Ambiguous text symbol: {name}")
                symbols.pop(name, None)
                continue
            kinds[name] = kind
            symbols[name] = (int(fields[0], 16) & 0xFFFFFFFF, int(fields[1], 16))
    return symbols


def linked_code_section(image_id: str) -> str | None:
    """The linked output section of an image's code, when VRAM alone is ambiguous.

    The two overlays share VRAM 0x801E4EA0+, so their code is read from the output
    section splat names after the image (its code parent); other images have unique VRAM.
    """
    return f".{image_id}" if image_id in OVERLAY_IMAGES else None


def elf_code(path: Path, address: int, size: int, *, section_name: str | None = None, allow_relocations: bool = False) -> tuple[bytes, int]:
    """Read actual executable section bytes and return their file offset.

    Reading unresolved object bytes requires an explicit opt-in. C attribution
    permits it only after recompiling the complete object and reproducing the
    complete linked ELF with the pinned linker; relocations are never ignored in
    the final original-byte comparison.
    """
    return _elf_code(path.read_bytes(), address, size, section_name=section_name, allow_relocations=allow_relocations)


class _LinkedElfMemo:
    """One invocation's last immutable ELF image, compared directly without hashing."""

    def __init__(self) -> None:
        self._data: bytes | None = None
        self._elf: Elf32File | None = None

    def __call__(self, data: bytes) -> Elf32File:
        if self._elf is None or data != self._data:
            elf = Elf32File(data)
            self._data, self._elf = data, elf
        return self._elf


def _elf_code(data: bytes, address: int, size: int, *, section_name: str | None = None,
              allow_relocations: bool = False, parse=Elf32File) -> tuple[bytes, int]:
    """Check each range independently; parsing may be memoized by immutable bytes."""
    if data[:6] != b"\x7fELF\x01\x02":
        raise ValueError("Code evidence must be ELF32 big-endian")
    try:
        elf = parse(data)
        selected = [(index, section) for index, section in enumerate(elf.sectionHeaders)
                    if section.type == 1 and section.flags & 4
                    and (section_name is None or elf.shstrtab[section.name] == section_name)
                    and section.addr <= address and address + size <= section.addr + section.size]
        if len(selected) != 1:
            raise ValueError("Code range has no unique executable ELF section")
        index, section = selected[0]
        for relocation in elf.sectionHeaders:
            if relocation.type == 9 and relocation.info == index:
                for offset in range(relocation.offset, relocation.offset + relocation.size, 8):
                    relative = struct.unpack_from(">I", data, offset)[0]
                    if address - section.addr <= relative < address - section.addr + size:
                        if not allow_relocations:
                            raise ValueError("Relocation-aware C attribution is required for this object")
        start = section.offset + address - section.addr
        body = data[start:start + size]
        if len(body) != size:
            raise ValueError("Truncated ELF code evidence")
        return body, start
    except (IndexError, struct.error) as error:
        raise ValueError("Invalid ELF section evidence") from error


def check_elf_rom(directory: Path, profile: dict[str, Any]) -> None:
    """Re-extract the complete ELF independently; do not trust a separate ROM."""
    with retained_trial(directory, "elf-extraction") as temporary:
        output = Path(temporary) / "elf.z64"
        command = [profile["tools"]["objcopy"]["path"], "-O", "binary", str(directory / "shiren2.elf"), str(output)]
        record = {"argv": command, "cwd": str(Path.cwd()), "environment_inherited": True}
        (Path(temporary) / "command.json").write_text(json.dumps(record, indent=2) + "\n")
        completed = subprocess.run(command, capture_output=True)
        (Path(temporary) / "stdout.txt").write_bytes(completed.stdout)
        (Path(temporary) / "stderr.txt").write_bytes(completed.stderr)
        record["returncode"] = completed.returncode
        (Path(temporary) / "command.json").write_text(json.dumps(record, indent=2) + "\n")
        completed.check_returncode()
        if output.read_bytes() != (directory / "shiren2.z64").read_bytes():
            raise ValueError("ELF extraction disagrees with the certified ROM")


def validate_compiled_graph_profiles(graph: dict[str, dict[str, str]], profile: dict[str, Any]) -> None:
    """Check the complete compiled-TU assignment without compiling or linking."""
    sources = {item["source"] for item in graph.values() if item["source_kind"] in COMPILED_KINDS}
    if set(profile["tu_profiles"]) != sources:
        raise ValueError("C graph and translation-unit profile assignments disagree")


def generated_table_discards(directory: Path, graph: dict[str, dict[str, str]], profile: dict[str, Any], reference: bytes) -> dict[str, list[dict[str, Any]]]:
    """Re-derive every compiled object's table-discard permission before linking."""
    from match import Elf
    units = profile.get("symbol_identities", {}).get("active_units", {})
    proofs = {}
    for name, item in graph.items():
        if item["source_kind"] not in COMPILED_KINDS:
            continue
        obj = Elf(checked_path(directory, name).read_bytes())
        records = generated_symbols.table_discards(directory / "source", obj, units.get(item["source"], {}), reference)
        if records:
            proofs[name] = records
    return proofs


def reproduce_c_objects_and_link(directory: Path, graph: dict[str, dict[str, str]], profile: dict[str, Any], reference: Path = CANONICAL_ROM) -> dict[str, Any]:
    """Regenerate the split, reproduce all objects and independently relink.

    The linker, rather than a homegrown relocation evaluator, applies MIPS
    relocations. Raw objects, compiler output, map, linked ELF and ROM must all
    reproduce exactly. Private verification outputs never modify the build.
    Replaying the split also binds every code/data/BSS placement to frozen YAML.
    ASM and binary objects retain zero C credit even when they reproduce exactly.
    C++ (`.cpp`) objects take the same compiled-TU path as C objects.
    """
    from workspace import verify_manifest
    manifest = read_json(directory / "input-manifest.json")
    validate_compiled_graph_profiles(graph, profile)
    evidence: dict[str, Any] = {"c_objects": {}, "objects": {}, "split": {}, "linked": {}, "method": "fresh frozen-source split, all-object reproduction and full pinned-linker replay"}
    with retained_trial(directory, "full-link") as temporary:
        trial = Path(temporary)
        command_log = CommandLog(trial / "command-evidence" / "commands.json")
        replay_commands = command_log.records

        def run_replay(command: list[str]) -> None:
            number = len(replay_commands) + 1
            record = {"argv": command, "cwd": str(trial), "environment": environment,
                "driver_children": "Internal compiler/Splat children untraced; argv/recipes unchanged"}
            replay_commands.append(record)
            log = trial / "command-evidence"
            log.mkdir(exist_ok=True)
            command_log.rewrite_from(len(replay_commands) - 1)
            completed = subprocess.run(command, cwd=trial, env=environment, capture_output=True)
            (log / f"{number:03d}.stdout").write_bytes(completed.stdout)
            (log / f"{number:03d}.stderr").write_bytes(completed.stderr)
            record["returncode"] = completed.returncode
            command_log.rewrite_from(len(replay_commands) - 1)
            completed.check_returncode()

        shutil.copytree(directory / "source", trial / "source", ignore=shutil.ignore_patterns("__pycache__"))
        verify_manifest(trial / "source", manifest)
        (trial / "tmp").mkdir()
        environment = compiler_environment(trial, profile)
        (trial / "splat.override.yaml").write_text(yaml.safe_dump(splat_options(trial, reference)), encoding="utf-8")
        recipe = split_commands()
        validate_asset_recipe(trial / "source", recipe)
        for operation in recipe:
            run_replay(operation)
        if (trial / "shiren2.ld").read_bytes() != (directory / "shiren2.ld").read_bytes():
            raise ValueError("Frozen YAML does not reproduce the linker script/split placement")
        generated = file_inventory(trial / "generated")
        if generated != file_inventory(directory / "generated") or build_graph(trial) != graph:
            raise ValueError("Frozen YAML does not reproduce the complete generated split/graph")
        evidence["split"] = {"linker_script_sha256": sha256(trial / "shiren2.ld"), "generated": generated}
        for name, item in graph.items():
            destination = checked_path(trial, name)
            destination.parent.mkdir(parents=True, exist_ok=True)
            operations = object_commands(name, item, profile)
            if item["source_kind"] in COMPILED_KINDS:
                compiled = Path("compiled") / Path(item["source"]).with_suffix(".s")
                (trial / compiled).parent.mkdir(parents=True, exist_ok=True)
                operations = operations[1:]
            for operation in operations:
                run_replay(operation)
            if item["source_kind"] in COMPILED_KINDS and (trial / compiled).read_bytes() != (directory / compiled).read_bytes():
                raise ValueError(f"{LANGUAGE_LABELS[item['source_kind']]} compiler assembly does not reproduce from frozen source: {item['source']}")
            if destination.read_bytes() != checked_path(directory, name).read_bytes():
                raise ValueError(f"{LANGUAGE_LABELS.get(item['source_kind'], 'Non-C')} object instruction bytes disagree with fresh frozen-source assembly: {name}")
            evidence["objects"][name] = {"source_kind": item["source_kind"], "sha256": sha256(destination)}
            if item["source_kind"] in COMPILED_KINDS:
                evidence["c_objects"][name] = {"source": item["source"], "profile_id": profile["tu_profiles"][item["source"]], "compiler_assembly_sha256": sha256(trial / compiled), "object_sha256": sha256(destination)}
        generated_table_discards(trial, graph, profile, reference.read_bytes())
        for operation in expected_commands(graph, profile)[-2:]:
            run_replay(operation)
        for name in ["shiren2.elf", "shiren2.map", "shiren2.z64"]:
            if (trial / name).read_bytes() != (directory / name).read_bytes():
                raise ValueError(f"Independent full linker replay disagrees: {name}")
            evidence["linked"][name] = sha256(trial / name)
        verify_manifest(trial / "source", manifest)
    verify_manifest(directory / "source", manifest)
    return evidence


def text_relocations(path: Path) -> list[dict[str, Any]]:
    """Describe calibrated ELF32 REL records without rewriting their contents."""
    data = path.read_bytes()
    if data[:6] != b"\x7fELF\x01\x02" or struct.unpack_from(">H", data, 18)[0] != 8:
        raise ValueError("Relocations require ELF32 big-endian MIPS")
    elf = Elf32File(data)
    text = [index for index, section in enumerate(elf.sectionHeaders) if elf.shstrtab[section.name] == ".text"]
    if len(text) != 1:
        raise ValueError("Object has no unique .text section")
    result = []
    for section in elf.sectionHeaders:
        if section.info != text[0] or section.type not in {4, 9}:
            continue
        if section.type != 9 or section.entsize != 8 or section.size % 8:
            raise ValueError("Unsupported text relocation encoding")
        symbols = elf.sectionHeaders[section.link]
        strings = elf.sectionHeaders[symbols.link]
        if symbols.type != 2 or symbols.entsize != 16 or symbols.size % 16 or strings.type != 3:
            raise ValueError("Invalid relocation symbol table")
        for start in range(section.offset, section.offset + section.size, 8):
            offset, info = struct.unpack_from(">II", data, start)
            symbol_index, kind = info >> 8, info & 0xFF
            if kind not in {4, 5, 6} or symbol_index >= symbols.size // 16 or offset % 4 or offset + 4 > elf.sectionHeaders[text[0]].size:
                raise ValueError("Uncalibrated or invalid text relocation")
            name_offset, value, size, symbol_info, other, symbol_section = struct.unpack_from(">IIIBBH", data, symbols.offset + 16 * symbol_index)
            if name_offset >= strings.size:
                raise ValueError("Invalid relocation symbol string")
            string_start = strings.offset + name_offset
            end = data.find(b"\0", string_start, strings.offset + strings.size)
            if end == -1:
                raise ValueError("Unterminated relocation symbol")
            result.append({"offset": offset, "type": {4: "R_MIPS_26", 5: "R_MIPS_HI16", 6: "R_MIPS_LO16"}[kind], "symbol_index": symbol_index, "symbol": data[string_start:end].decode("ascii"), "symbol_section": symbol_section, "symbol_value": value, "symbol_size": size, "symbol_info": symbol_info})
    return result


# g++ 2.8.1 (pre-ABI-3) C++-linkage names: constructors `__<n>Class...`, methods and free C++
# functions `name__<n>Class...`/`name__F...`/`name__C<n>...`/`__Q...`/templates `__t<n>`/`__H<n>`,
# and compiler data with `.`/`$` (`_vt.3Obj`, destructors `_$_3Obj`, static members, `_GLOBAL_.`).
GXX_MANGLED = re.compile(r"__(?:[0-9]|F|C[0-9]|Q[0-9_]|t[0-9]|H[0-9])|[.$]|^__t[fi]")


def global_symbol_names(path: Path) -> list[str]:
    """Every global/weak symbol an ELF32 object defines or references, from its symbol tables."""
    data = path.read_bytes()
    if data[:6] != b"\x7fELF\x01\x02":
        raise ValueError("Symbol evidence must be ELF32 big-endian")
    try:
        shoff, = struct.unpack_from(">I", data, 0x20)
        shentsize, shnum = struct.unpack_from(">HH", data, 0x2E)
        headers = [struct.unpack_from(">10I", data, shoff + i * shentsize) for i in range(shnum)]
        names = []
        for header in headers:
            if header[1] != 2:  # SHT_SYMTAB
                continue
            strings = headers[header[6]]
            for offset in range(header[4], header[4] + header[5], 16):
                name_offset, _value, _size, info, _other, _section = struct.unpack_from(">IIIBBH", data, offset)
                if info >> 4 in {1, 2} and name_offset:
                    start = strings[4] + name_offset
                    names.append(data[start:data.index(b"\0", start)].decode("latin-1"))
        return sorted(set(names))
    except (IndexError, ValueError, struct.error) as error:
        raise ValueError(f"Invalid ELF symbol table: {path.name}") from error


def c_attributions(directory: Path, matches: list[dict[str, Any]], graph: dict[str, dict[str, str]], profile: dict[str, Any], reference: bytes, *, reproduction: dict[str, Any] | None = None) -> list[dict[str, Any]]:
    """Bind every accepted C/C++ function to its object, map contribution and original bytes.

    C++ linkage is forbidden except for declared, independently proven generated
    identities. Catalogue aliases carry no credit without their emitting object.
    """
    attest(reference)
    from match import Elf, cxx_object_problems
    identities = generated_symbols.load(directory / "source", reference)
    units = generated_symbols.active_units(identities, matches)
    generated_symbols.validate_active(directory / "source", units, reference)
    linked_elf = Elf((directory / "shiren2.elf").read_bytes()) if units else None
    if units:
        recorded = profile.get("symbol_identities")
        if recorded != {"declaration": identities, "active_units": units}:
            raise ValueError("Generated identity profile disagrees with frozen config")
    discarded_tables = generated_table_discards(directory, graph, profile, reference)
    for name, records in discarded_tables.items():
        generated_symbols.prove_linker_discard((directory / "shiren2.map").read_text(), name, records)
    actual = (directory / "shiren2.z64").read_bytes()
    linked = symbol_table(directory / "shiren2.elf", profile)
    sections = {}
    for line in (directory / "shiren2.map").read_text().splitlines():
        match = re.fullmatch(r"\s+\.text\s+(0x[0-9a-fA-F]+)\s+(0x[0-9a-fA-F]+)\s+(\S+)\s*", line)
        if match and match[3] in graph and graph[match[3]]["source_kind"] in COMPILED_KINDS:
            if match[3] in sections:
                raise ValueError("Ambiguous C object map section")
            sections[match[3]] = (int(match[1], 16) & 0xFFFFFFFF, int(match[2], 16))
    by_source = {(item["image_id"], item["source"]): name for name, item in graph.items() if item["source_kind"] in COMPILED_KINDS}
    if {match["source"] for match in matches} != {item["source"] for item in graph.values() if item["source_kind"] in COMPILED_KINDS}:
        raise ValueError("C manifest does not agree with declared C objects")
    for name, item in graph.items():
        if item["source_kind"] not in COMPILED_KINDS:
            continue
        unit = units.get(item["source"], {"generated": {}, "runtime_names": []})
        globals_used = global_symbol_names(directory / name)
        if (set(globals_used) & set(generated_symbols.REVIEWED_RUNTIME)) - set(unit["runtime_names"]):
            raise ValueError(f"Undeclared compiler runtime name: {name}")
        if item["source_kind"] == "cpp":
            mangled = [symbol for symbol in globals_used if GXX_MANGLED.search(symbol) and symbol not in unit["generated"]]
            if mangled:
                raise ValueError(f"C++ object uses C++-linkage symbols (export/import through extern \"C\"): {name}: {mangled}")
        if item["source"] in units:
            obj = Elf((directory / name).read_bytes())
            text = obj.section(".text")
            if text is None:
                raise ValueError("Generated TU has no .text")
            problems = generated_symbols.object_problems(obj, text, unit["rows"], unit, cxx=item["source_kind"] == "cpp")
            problems += cxx_object_problems(obj, text, unit["generated"])
            if problems:
                raise ValueError("; ".join(problems))
            known = {s.name: s.value for s in linked_elf.symbols if s.bind == 1 and s.shndx != 0}
            generated_symbols.runtime_addresses(identities, unit, known)
            for runtime in unit["runtime_names"]:
                if known.get(runtime) != identities["runtime_bindings"][runtime]["vram_start"]:
                    raise ValueError(f"Runtime name not bound at link time: {runtime}")
    policy = source_policy_violations(directory / "source")
    if policy:
        raise ValueError("Source policy forbids C credit: " + "; ".join(policy))
    result = []
    object_symbols = {}
    declared_symbols = {}
    for match in matches:
        declared_symbols.setdefault((match.get("image_id", "resident"), match["source"]), set()).add(match.get("generated", match["symbol"]))
    # Re-read every attribution: path/stat identity cannot detect same-size tampering.
    # Only this invocation can reuse the last byte-identical linked ELF parse.
    parse_linked = _LinkedElfMemo()
    for match in matches:
        image = match.get("image_id", "resident")
        name = by_source[(image, match["source"])]
        checked_path(directory / "source", match["source"])
        if name not in object_symbols:
            object_symbols[name] = symbol_table(directory / name, profile)
        symbols = object_symbols[name]
        emitted = match.get("generated", match["symbol"])
        if emitted not in symbols or symbols[emitted][1] != match["size"]:
            raise ValueError(f"C symbol not defined by declared object: {match['symbol']} ({emitted})")
        if name not in sections:
            raise ValueError("C object has no unique .text map contribution")
        base, section_size = sections[name]
        offset, size = symbols[emitted]
        linked_identity = linked.get(match["symbol"]) == (match["vram_start"], size)
        if "generated" in match:
            section = linked_code_section(image)
            emitted_hits = [s for s in linked_elf.symbols if s.name == emitted and s.type == 2
                            and s.value == match["vram_start"] and s.size == size
                            and 0 < s.shndx < len(linked_elf.sections)
                            and (section is None or linked_elf.sections[s.shndx].name == section)]
            aliases = [s for s in linked_elf.symbols if s.name == match["symbol"] and s.bind == 1 and s.shndx == 0xFFF1]
            linked_identity = len(emitted_hits) == 1 and len(aliases) == 1 and aliases[0].value == match["vram_start"]
        if offset + size > section_size or base + offset != match["vram_start"] or not linked_identity:
            raise ValueError("C object/map/linked symbol attribution disagrees")
        start = match["rom_start"]
        expected = reference[start:start + size]
        observed = actual[start:start + size]
        if observed != expected or len(expected) != size:
            raise ValueError("C linked bytes do not match the original interval")
        relocation_records = [record for record in text_relocations(directory / name) if offset <= record["offset"] < offset + size]
        if relocation_records and (reproduction is None or reproduction["c_objects"].get(name, {}).get("object_sha256") != sha256(directory / name) or reproduction["linked"].get("shiren2.elf") != sha256(directory / "shiren2.elf")):
            raise ValueError("Relocated C needs a fresh frozen-source object and full linker reproduction")
        object_body, object_file_offset = elf_code(directory / name, offset, size, section_name=".text", allow_relocations=bool(relocation_records))
        linked_body, elf_file_offset = _elf_code((directory / "shiren2.elf").read_bytes(), base + offset, size, parse=parse_linked, section_name=linked_code_section(image))
        if (not relocation_records and object_body != linked_body) or linked_body != expected:
            raise ValueError("C object/ELF/original instruction bytes disagree")
        result.append({"image_id": image, "symbol": match["symbol"], "source": match["source"], "object": name, "vram_start": base + offset, "size": size, "object_symbol_offset": offset, "object_file_offset": object_file_offset, "elf_file_offset": elf_file_offset, "map_section": {"vram_start": base, "size": section_size}, "reference_sha256": hashlib.sha256(expected).hexdigest(), "linked_sha256": hashlib.sha256(linked_body).hexdigest(), "object_sha256": hashlib.sha256(object_body).hexdigest(), "profile_id": profile["tu_profiles"][match["source"]], "text_relocations": relocation_records, "relocation_proof": "fresh full object and pinned-linker reproduction" if relocation_records else "raw object/linked/original equality"})
        if "generated" in match:
            result[-1]["generated"] = match["generated"]
        unit = units.get(match["source"])
        if unit and unit["runtime_names"]:
            result[-1]["runtime_bindings"] = {runtime: identities["runtime_bindings"][runtime] for runtime in unit["runtime_names"]}
        if name in discarded_tables:
            result[-1]["discarded_tables"] = discarded_tables[name]
    for name, symbols in object_symbols.items():
        declared = declared_symbols[(graph[name]["image_id"], graph[name]["source"])]
        if set(symbols) != declared:
            raise ValueError("C object contains undeclared text functions")
    return result


def check_elf(directory: Path) -> None:
    header = (directory / "shiren2.elf").read_bytes()[:32]
    if header[:6] != b"\x7fELF\x01\x02" or int.from_bytes(header[18:20], "big") != 8 or int.from_bytes(header[24:28], "big") != 0x80025C00:
        raise ValueError("Output must be ELF32 big-endian MIPS with the correct entrypoint")
