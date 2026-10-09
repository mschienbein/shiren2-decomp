from __future__ import annotations

import copy
import hashlib
import json
import os
import re
import shutil
import stat
import struct
import subprocess
import sys
import tempfile
import unittest
from unittest.mock import patch
from pathlib import Path
from types import SimpleNamespace

import yaml

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "tools"))
from rom import CANONICAL_ROM, EXPECTED_SHA256, PROJECT, attest, normalize
from build import build, compare_rom, execute_snapshot, write_json
from verify import checked_path, verify_receipt
from certification import REQUIRED_ARTIFACTS, REQUIRED_RECEIPT_FIELDS, binary_storage_record, build_graph, compiler_environment, elf_code, file_inventory, image_evidence, intermediate_keys, linked_image_records, object_commands, read_json, retained_trial, sha256, source_policy_violations, splat_bindings, splat_options, storage_elf_layout, storage_load_mapping, tool_profile
from workspace import _validated_manifest, create_snapshot, input_manifest
from evidence import require_inventory
from spimdisasm.elf32 import Elf32File


def copy_private_evidence_core(receipt_path: Path, directory: Path) -> None:
    """Copy the complete receipt core, never a retained top-level audit tree.

    This is selection/preflight, not acceptance. The unchanged verifier still
    checks bytes, dependencies, full inventories, profiles and actual replay.
    """
    def absolute(path: Path) -> Path:
        if not isinstance(path, Path) or ".." in path.parts or "\\" in str(path):
            raise ValueError("Fixture roots must be ordinary non-traversing paths")
        return path.absolute()

    def open_directory(path: Path) -> int:
        descriptor = os.open(path.anchor, os.O_RDONLY | os.O_DIRECTORY)
        try:
            for part in path.parts[1:]:
                child = os.open(part, os.O_RDONLY | os.O_DIRECTORY | os.O_NOFOLLOW,
                                dir_fd=descriptor)
                os.close(descriptor)
                descriptor = child
            return descriptor
        except BaseException:
            os.close(descriptor)
            raise

    def identity(info: os.stat_result) -> tuple:
        return (info.st_dev, info.st_ino, info.st_mode, info.st_size,
                info.st_mtime_ns, info.st_ctime_ns)

    def copy_file(source: int, target: int, name: str) -> None:
        before = os.stat(name, dir_fd=source, follow_symlinks=False)
        if not stat.S_ISREG(before.st_mode):
            raise ValueError(f"Fixture input must be a regular file: {name}")
        descriptor = os.open(name, os.O_RDONLY | os.O_NOFOLLOW | os.O_NONBLOCK,
                             dir_fd=source)
        with os.fdopen(descriptor, "rb") as original:
            if identity(os.fstat(original.fileno())) != identity(before):
                raise ValueError(f"Fixture input changed before copying: {name}")
            copied = os.open(name, os.O_WRONLY | os.O_CREAT | os.O_EXCL | os.O_NOFOLLOW,
                             stat.S_IMODE(before.st_mode) | 0o600, dir_fd=target)
            with os.fdopen(copied, "wb") as output:
                shutil.copyfileobj(original, output)
            if (identity(os.fstat(original.fileno())) != identity(before)
                    or identity(os.stat(name, dir_fd=source, follow_symlinks=False)) != identity(before)):
                raise ValueError(f"Fixture input changed while copying: {name}")

    def copy_tree(source: int, target: int, name: str) -> None:
        before = os.stat(name, dir_fd=source, follow_symlinks=False)
        if not stat.S_ISDIR(before.st_mode):
            raise ValueError(f"Fixture input must be a real directory: {name}")
        opened = os.open(name, os.O_RDONLY | os.O_DIRECTORY | os.O_NOFOLLOW, dir_fd=source)
        try:
            if identity(os.fstat(opened)) != identity(before):
                raise ValueError(f"Fixture directory changed before copying: {name}")
            os.mkdir(name, stat.S_IMODE(before.st_mode) | 0o700, dir_fd=target)
            copied = os.open(name, os.O_RDONLY | os.O_DIRECTORY | os.O_NOFOLLOW, dir_fd=target)
            try:
                for entry in sorted(os.listdir(opened)):
                    mode = os.stat(entry, dir_fd=opened, follow_symlinks=False).st_mode
                    if stat.S_ISDIR(mode):
                        copy_tree(opened, copied, entry)
                    else:
                        copy_file(opened, copied, entry)
                if (identity(os.fstat(opened)) != identity(before)
                        or identity(os.stat(name, dir_fd=source, follow_symlinks=False)) != identity(before)):
                    raise ValueError(f"Fixture directory changed while copying: {name}")
            finally:
                os.close(copied)
        finally:
            os.close(opened)

    def relative(name: str) -> None:
        if (not isinstance(name, str) or not name or "\\" in name
                or Path(name).is_absolute() or ".." in Path(name).parts
                or Path(name).as_posix() != name or name == "."):
            raise ValueError("Fixture receipt contains a malformed relative path")

    receipt_path, directory = absolute(receipt_path), absolute(directory)
    source_root = receipt_path.parent
    if receipt_path.name != "receipt.json" or directory.is_relative_to(source_root) or source_root.is_relative_to(directory):
        raise ValueError("Fixture source and destination must be separate receipt roots")
    source = open_directory(source_root)
    try:
        parent = open_directory(directory.parent)
        try:
            os.mkdir(directory.name, 0o700, dir_fd=parent)
            target = os.open(directory.name, os.O_RDONLY | os.O_DIRECTORY | os.O_NOFOLLOW, dir_fd=parent)
        finally:
            os.close(parent)
        try:
            # These two independent copies make JSON parsing confined to this new fixture.
            for name in ("receipt.json", "input-manifest.json"):
                copy_file(source, target, name)
            receipt = read_json(directory / "receipt.json")
            require_inventory(receipt, REQUIRED_RECEIPT_FIELDS, "receipt fields")
            if (type(receipt["schema_version"]) is not int or receipt["schema_version"] != 3
                    or receipt["status"] != "byte-identical" or receipt["target_sha256"] != EXPECTED_SHA256):
                raise ValueError("Fixture receipt must use the current target/schema")
            require_inventory(receipt["artifacts"], REQUIRED_ARTIFACTS, "artifacts")
            manifest = read_json(directory / "input-manifest.json")
            _validated_manifest(manifest)
            if receipt["input_manifest"] != manifest:
                raise ValueError("Fixture input manifest differs from its receipt")
            for field in ("generated", "compiled", "objects", "graph", "dependencies"):
                if not isinstance(receipt[field], dict):
                    raise ValueError(f"Fixture {field} must be a path dictionary")
                for name in receipt[field]:
                    relative(name)
            for item in receipt["graph"].values():
                if not isinstance(item, dict) or "source" not in item or "input" not in item:
                    raise ValueError("Fixture graph omits its source/input path")
                relative(item["source"])
                relative(item["input"])
            for paths in receipt["dependencies"].values():
                if not isinstance(paths, list):
                    raise ValueError("Fixture dependencies must contain path lists")
                for name in paths:
                    relative(name)
            for name in sorted(REQUIRED_ARTIFACTS - {"input-manifest.json"}):
                copy_file(source, target, name)
            if "build.log" in os.listdir(source):
                copy_file(source, target, "build.log")
            # Never select by receipt inventory: unknown entries and nested cache/tmp
            # names must remain visible to the unchanged policies after copying.
            for name in ("source", "generated", "compiled", "obj"):
                copy_tree(source, target, name)
        finally:
            os.close(target)
    finally:
        os.close(source)


class ByteOrderTests(unittest.TestCase):
    def test_all_supported_orders(self) -> None:
        canonical = bytes.fromhex("80371240") + bytes(range(60))
        swapped = bytearray(canonical)
        swapped[::2], swapped[1::2] = canonical[1::2], canonical[::2]
        little = b"".join(canonical[i:i + 4][::-1] for i in range(0, len(canonical), 4))
        for data in [canonical, bytes(swapped), little]:
            with self.subTest(magic=data[:4].hex()):
                self.assertEqual(normalize(data)[0], canonical)

    def test_invalid_header_or_length(self) -> None:
        for data in [b"", b"x" * 64, bytes.fromhex("80371240") + b"x" * 61]:
            with self.subTest(size=len(data)):
                with self.assertRaises(ValueError):
                    normalize(data)

    def test_receipt_path_cannot_escape_root(self) -> None:
        with self.assertRaises(ValueError):
            checked_path(PROJECT, "../../outside")


class ToolchainRegistryTests(unittest.TestCase):
    """Synthetic tool roots prove registry validation and per-TU driver selection."""
    FLAGS = ["-O2", "-G", "0", "-I", "source/include"]

    def fixture(self, root: Path, profiles: dict[str, dict]) -> Path:
        lock = {"gcc_candidate": {"install_directory": ".cache/gcc-kmc", "binaries_sha256": {}},
                "gcc_pm281": {"install_directory": ".cache/gcc-pm281", "binaries_sha256": {}},
                "binutils_kmc": {"install_directory": ".cache/binutils-kmc-2.6"},
                "binutils_gnu291": {"install_directory": ".cache/binutils-gnu-2.9.1"},
                "binutils_gnu291_fp32": {"install_directory": ".cache/binutils-gnu-2.9.1-fp32"}}
        for name in ["as", "objcopy", "nm", "objdump"]:
            (root / ".cache/binutils/bin").mkdir(parents=True, exist_ok=True)
            (root / f".cache/binutils/bin/mips64-elf-{name}").write_text(name)
        linker = root / ".cache/binutils/bin/mips64-elf-ld"
        linker.write_text("#!/bin/sh\necho 'Supported emulations: elf32btsmip'\n")
        linker.chmod(0o755)
        for entry in ["gcc_candidate", "gcc_pm281"]:
            for name in ["gcc", "cc1", "cpp"] + (["cc1plus"] if entry == "gcc_pm281" else []):
                path = root / lock[entry]["install_directory"] / name
                path.parent.mkdir(parents=True, exist_ok=True)
                path.write_text(f"{entry} {name}")
                lock[entry]["binaries_sha256"][name] = sha256(path)
        for entry in ["binutils_kmc", "binutils_gnu291", "binutils_gnu291_fp32"]:
            path = root / lock[entry]["install_directory"] / "as"
            path.parent.mkdir(parents=True, exist_ok=True)
            path.write_text(entry)
            lock[entry]["assembler_sha256"] = sha256(path)
        (root / "config").mkdir()
        (root / "toolchain.lock.json").write_text(json.dumps(lock))
        self.write_profiles(root, profiles)
        return root

    def write_profiles(self, root: Path, profiles: dict[str, dict]) -> None:
        declared = {key: {"assembler_flags": ["-EB"], "compiler_flags": self.FLAGS, "scope": "fixture", **item} for key, item in profiles.items()}
        (root / "config/compiler_profiles.json").write_text(json.dumps({
            "schema_version": 1, "profiles": declared, "compiler_environment": {"VR4300MUL": "ON"},
            "tu_profiles": {f"src/{key}.{'cpp' if item.get('language') == 'c++' else 'c'}": key for key, item in declared.items()}}))

    def test_unknown_compiler_assembler_or_pass_override_is_rejected(self) -> None:
        cases = [({"assembler": "as", "compiler": "gcc999"}, "Unsupported translation-unit compiler"),
                 ({"assembler": "as_gnu999"}, "Unsupported translation-unit assembler"),
                 ({"assembler": "as", "compiler_flags": [*self.FLAGS, "-B/tmp/"]}, "compiler pass selection"),
                 ({"assembler": "as", "linker": "ld"}, "Invalid translation-unit profile")]
        for item, message in cases:
            with self.subTest(item=item), tempfile.TemporaryDirectory() as temporary:
                root = self.fixture(Path(temporary), {"bad": item})
                with self.assertRaisesRegex(ValueError, message):
                    tool_profile(root)

    def test_changed_pinned_compiler_or_assembler_is_rejected(self) -> None:
        for changed, message in [(".cache/gcc-pm281/cc1", "pinned compiler gcc281pm: cc1"), (".cache/binutils-gnu-2.9.1/as", "pinned assembler as_gnu291")]:
            with self.subTest(changed=changed), tempfile.TemporaryDirectory() as temporary:
                root = self.fixture(Path(temporary), {"new": {"compiler": "gcc281pm", "assembler": "as_gnu291"}})
                (root / changed).write_text("tampered")
                with self.assertRaisesRegex(ValueError, message):
                    tool_profile(root)

    def test_changed_pinned_fp32_assembler_is_rejected(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = self.fixture(Path(temporary), {"fp32": {"compiler": "gcc281pm", "assembler": "as_gnu291_fp32"}})
            profile = tool_profile(root)
            item = {"source_kind": "c", "source": "src/fp32.c", "input": "source/src/fp32.c"}
            assembler = root / ".cache/binutils-gnu-2.9.1-fp32/as"
            self.assertEqual(object_commands("obj/fp32.o", item, profile)[2][0], str(assembler.resolve()))
            assembler.write_text("tampered")
            with self.assertRaisesRegex(ValueError, "pinned assembler as_gnu291_fp32"):
                tool_profile(root)

    def test_asg_profile_adds_only_assembler_debug_scheduling(self) -> None:
        profiles = read_json(PROJECT / "config/compiler_profiles.json")["profiles"]
        main = profiles["gcc281pm-gnu291-O2-unsigned"]
        asg = profiles["gcc281pm-gnu291-O2-unsigned-asg"]
        self.assertEqual(asg["compiler"], main["compiler"])
        self.assertEqual(asg["assembler"], "as_gnu291_fp32")
        self.assertEqual(asg["assembler"], main["assembler"])
        self.assertEqual(asg["compiler_flags"], main["compiler_flags"])
        self.assertEqual(asg["assembler_flags"], [*main["assembler_flags"], "-g"])
        self.assertNotIn("-g", asg["compiler_flags"])

    def test_cxx_fp32_profile_uses_the_same_main_assembler_recipe(self) -> None:
        profiles = read_json(PROJECT / "config/compiler_profiles.json")["profiles"]
        main = profiles["gcc281pm-gnu291-O2-unsigned"]
        cxx = profiles["gxx281pm-gnu291-O2-unsigned"]
        self.assertEqual(cxx["language"], "c++")
        self.assertEqual(cxx["compiler"], main["compiler"])
        self.assertEqual(cxx["compiler_flags"],
                         [*main["compiler_flags"], "-fno-exceptions", "-fno-rtti"])
        self.assertEqual(cxx["assembler"], "as_gnu291_fp32")
        self.assertEqual(cxx["assembler_flags"], main["assembler_flags"])

    def test_asg_profile_binds_the_complete_service_manager_range(self) -> None:
        declaration = read_json(PROJECT / "config/compiler_profiles.json")
        pointer = read_json(PROJECT / "docs/progress.json")["denominator"]
        catalogue = read_json(PROJECT / pointer["path"])["identity"]["functions"]
        symbols = {item["symbol"] for item in catalogue
                   if item["image_id"] == "main_14400" and 0x80130A00 <= item["vram_start"] < 0x80132050}
        self.assertEqual(len(symbols), 48)
        matches = read_json(PROJECT / "config/matches.json")["functions"]
        expected = {item["source"] for item in matches if item["symbol"] in symbols}
        self.assertTrue(expected)
        actual = {source for source, profile_id in declaration["tu_profiles"].items()
                  if profile_id == "gcc281pm-gnu291-O2-unsigned-asg"}
        self.assertEqual(actual, expected)
        for item in matches:
            if item["source"] in actual:
                self.assertIn(item["symbol"], symbols)
        self.assertTrue(all((PROJECT / source).is_file() for source in actual))
        self.assertEqual(declaration["tu_profiles"]["src/units/main_14400/func_80130930.c"],
                         "gcc281pm-gnu291-O2-unsigned")
        self.assertEqual(declaration["tu_profiles"]["src/units/main_14400/func_80132050.c"],
                         "gcc272-gnu291-mips2-unsigned")

    @unittest.skipUnless(
        (PROJECT / ".cache/gcc-pm281/gcc").is_file()
        and (PROJECT / ".cache/gcc-kmc/cc1").is_file()
        and (PROJECT / ".cache/binutils-gnu-2.9.1-fp32/as").is_file(),
        "Pinned GCC 2.8.1, GCC 2.7.2 and FP32 GNU gas installs required",
    )
    def test_asg_bound_units_use_the_real_fp32_object_recipe(self) -> None:
        profile = tool_profile(PROJECT)
        for source, profile_id in profile["tu_profiles"].items():
            if profile_id != "gcc281pm-gnu291-O2-unsigned-asg":
                continue
            with self.subTest(source=source):
                item = {"source_kind": "c", "source": source, "input": f"source/{source}"}
                commands = object_commands(f"obj/{source}.o", item, profile)
                self.assertNotIn("-g", commands[0])
                self.assertNotIn("-g", commands[1])
                self.assertEqual(commands[2][0], profile["tools"]["as_gnu291_fp32"]["path"])
                self.assertEqual(commands[2][1:-4],
                                 profile["profiles"][profile_id]["assembler_flags"][:-1])
                self.assertEqual(commands[2][-4], "-g")

    def test_object_commands_use_the_profile_compiler_and_assembler(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = self.fixture(Path(temporary), {"old": {"assembler": "as_kmc26"}, "new": {"compiler": "gcc281pm", "assembler": "as_gnu291"}})
            profile = tool_profile(root)
            self.assertEqual(set(profile["compilers"]), {"gcc272", "gcc281pm"})
            self.assertEqual(profile["compiler"], profile["compilers"]["gcc272"])
            pm281 = (root / ".cache/gcc-pm281").resolve()
            new = object_commands("obj/new.o", {"source_kind": "c", "source": "src/new.c", "input": "source/src/new.c"}, profile)
            self.assertEqual([command[:2] for command in new[:2]], [[str(pm281 / "gcc"), f"-B{pm281}/"]] * 2)
            self.assertEqual(new[2][0], str((root / ".cache/binutils-gnu-2.9.1/as").resolve()))
            old = object_commands("obj/old.o", {"source_kind": "c", "source": "src/old.c", "input": "source/src/old.c"}, profile)
            self.assertEqual(old[0][:2], [str((root / ".cache/gcc-kmc/gcc").resolve()), *self.FLAGS[:1]])
            self.assertFalse(any(flag.startswith("-B") for command in old for flag in command))
            # The default compiler keeps finding its passes through COMPILER_PATH.
            self.assertEqual(compiler_environment(root, profile)["COMPILER_PATH"], str((root / ".cache/gcc-kmc").resolve()))

    def test_unselected_registry_tools_are_not_required(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = self.fixture(Path(temporary), {"old": {"assembler": "as"}})
            shutil.rmtree(root / ".cache/gcc-pm281")
            shutil.rmtree(root / ".cache/binutils-gnu-2.9.1")
            shutil.rmtree(root / ".cache/binutils-gnu-2.9.1-fp32")
            profile = tool_profile(root)
            self.assertEqual(set(profile["compilers"]), {"gcc272"})
            self.assertNotIn("as_gnu291", profile["tools"])
            self.assertNotIn("as_gnu291_fp32", profile["tools"])

    @unittest.skipUnless((PROJECT / ".cache/gcc-pm281/gcc").is_file() and (PROJECT / ".cache/gcc-kmc/cc1").is_file(), "Pinned GCC 2.8.1 and 2.7.2 installs required")
    def test_installed_gcc281_driver_runs_its_own_passes_despite_compiler_path(self) -> None:
        profile = tool_profile(PROJECT)
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            (root / "tmp").mkdir()
            (root / "compiled/src/units/main_14400").mkdir(parents=True)
            (root / "f.c").write_text("int f(void) { return 0; }\n")
            item = {"source_kind": "c", "source": "src/units/main_14400/f.c", "input": "f.c"}
            with patch.dict(profile["tu_profiles"], {item["source"]: "gcc281pm-gnu291-O2-unsigned"}):
                command = object_commands("f.o", item, profile)[1]
            command = [flag for flag in command if flag not in {"-I", "source/include"}]
            completed = subprocess.run([*command[:2], "-v", *command[2:]], cwd=root, env=compiler_environment(root, profile), capture_output=True, text=True)
            self.assertEqual(completed.returncode, 0, completed.stderr)
            passes = [line.split()[0] for line in completed.stderr.splitlines() if line.startswith(" /")]
            pm281 = Path(profile["compilers"]["gcc281pm"]["gcc"]["path"]).parent
            self.assertEqual(passes, [str(pm281 / "cpp"), str(pm281 / "cc1")])
            self.assertIn("GNU C version 2.8.1", completed.stderr)

    CXX = {"compiler": "gcc281pm", "assembler": "as_gnu291", "language": "c++", "compiler_flags": [*FLAGS, "-fno-exceptions", "-fno-rtti"]}

    def test_cxx_profile_measures_cc1plus_and_compiles_only_cpp_sources(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = self.fixture(Path(temporary), {"old": {"assembler": "as"}, "c281": {"compiler": "gcc281pm", "assembler": "as_gnu291"}})
            self.assertNotIn("cc1plus", tool_profile(root)["compilers"]["gcc281pm"])  # C-only declarations keep their profile
            self.write_profiles(root, {"old": {"assembler": "as"}, "cxx": self.CXX})
            profile = tool_profile(root)
            pm281 = (root / ".cache/gcc-pm281").resolve()
            self.assertEqual(profile["compilers"]["gcc281pm"]["cc1plus"]["path"], str(pm281 / "cc1plus"))
            self.assertEqual(profile["tu_profiles"], {"src/old.c": "old", "src/cxx.cpp": "cxx"})
            item = {"source_kind": "cpp", "source": "src/cxx.cpp", "input": "source/src/cxx.cpp"}
            commands = object_commands("obj/source/src/cxx.o", item, profile)
            self.assertEqual([command[:2] for command in commands[:2]], [[str(pm281 / "gcc"), f"-B{pm281}/"]] * 2)
            self.assertEqual(commands[1][-4:], ["-S", "source/src/cxx.cpp", "-o", "compiled/src/cxx.s"])
            self.assertTrue({"-fno-exceptions", "-fno-rtti"} <= set(commands[0]))
            with self.assertRaisesRegex(ValueError, "does not compile C\\+\\+"):
                object_commands("obj/source/src/old.o", {**item, "source": "src/old.c"}, {**profile, "tu_profiles": {"src/old.c": "old"}})
            with self.assertRaisesRegex(ValueError, "does not compile C"):
                object_commands("obj/source/src/cxx.o", {**item, "source_kind": "c"}, profile)

    def test_cxx_profile_declarations_are_validated(self) -> None:
        cases = [({**self.CXX, "compiler_flags": [*self.FLAGS, "-fno-exceptions"]}, "disable exceptions and RTTI"),
                 ({**self.CXX, "compiler_flags": [*self.CXX["compiler_flags"], "-xc++"]}, "language selection"),
                 ({"assembler": "as", "compiler_flags": ["-x", "c++", *self.FLAGS]}, "language selection"),
                 ({**self.CXX, "language": "objc"}, "Unsupported translation-unit language")]
        for item, message in cases:
            with self.subTest(item=item), tempfile.TemporaryDirectory() as temporary:
                root = self.fixture(Path(temporary), {"bad": item})
                with self.assertRaisesRegex(ValueError, message):
                    tool_profile(root)
        for source in ["src/cxx.c", "src/cxx.cc"]:
            with self.subTest(source=source), tempfile.TemporaryDirectory() as temporary:
                root = self.fixture(Path(temporary), {"cxx": self.CXX})
                path = root / "config/compiler_profiles.json"
                declaration = json.loads(path.read_text())
                declaration["tu_profiles"] = {source: "cxx"}
                path.write_text(json.dumps(declaration))
                with self.assertRaisesRegex(ValueError, "Invalid translation-unit profile assignment"):
                    tool_profile(root)
        with tempfile.TemporaryDirectory() as temporary:
            root = self.fixture(Path(temporary), {"cxx": self.CXX})
            (root / ".cache/gcc-pm281/cc1plus").write_text("tampered")
            with self.assertRaisesRegex(ValueError, "pinned compiler gcc281pm: cc1plus"):
                tool_profile(root)

    @unittest.skipUnless((PROJECT / ".cache/gcc-pm281/cc1plus").is_file() and (PROJECT / ".cache/gcc-kmc/cc1").is_file(), "Pinned GCC 2.8.1 cc1plus and 2.7.2 installs required")
    def test_installed_gcc281_driver_runs_cc1plus_for_cpp_sources(self) -> None:
        profile = tool_profile(PROJECT)
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            (root / "tmp").mkdir()
            (root / "compiled/src/units/main_14400").mkdir(parents=True)
            (root / "f.cpp").write_text('struct S { int a; S() : a(1) {} };\nextern "C" int f(void) { S s; return s.a; }\n')
            item = {"source_kind": "cpp", "source": "src/units/main_14400/f.cpp", "input": "f.cpp"}
            with patch.dict(profile["tu_profiles"], {item["source"]: "gxx281pm-gnu291-O2-unsigned"}):
                commands = object_commands("f.o", item, profile)
            command = [flag for flag in commands[1] if flag not in {"-I", "source/include"}]
            completed = subprocess.run([*command[:2], "-v", *command[2:]], cwd=root, env=compiler_environment(root, profile), capture_output=True, text=True)
            self.assertEqual(completed.returncode, 0, completed.stderr)
            passes = [line.split()[0] for line in completed.stderr.splitlines() if line.startswith(" /")]
            pm281 = Path(profile["compilers"]["gcc281pm"]["gcc"]["path"]).parent
            self.assertEqual(passes, [str(pm281 / "cpp"), str(pm281 / "cc1plus")])
            self.assertIn("GNU C++ version 2.8.1", completed.stderr)
            assembly = (root / "compiled/src/units/main_14400/f.s").read_text()
            self.assertIn(".globl\tf\n", assembly)
            self.assertNotIn("eh_frame", assembly)


class Fp32AssemblerBehaviorTests(unittest.TestCase):
    """Exercise pinned gas binaries, not patch text or compiler source spelling."""

    FLAGS = ["-non_shared", "-EB", "-G", "0", "-mips3", "-mcpu=r3000", "-32"]
    CONTROL = """\
.text
.align 2
.globl lid_features
.ent lid_features
lid_features:
    move $2,$4
    la $3,lid_external
    li.d $f0,4294967296.0
    add.d $f2,$f0,$f0
    mtc1 $4,$f10
    cvt.s.w $f10,$f10
    c.eq.s $f10,$f12
    bc1t 1f
    nop
    mult $4,$5
    mflo $2
    mult $6,$7
    mflo $3
1:
    jr $31
    nop
.end lid_features
"""

    def setUp(self) -> None:
        lock = read_json(PROJECT / "toolchain.lock.json")
        self.assemblers = {
            key: PROJECT / lock[key]["install_directory"] / "as"
            for key in ("binutils_gnu291", "binutils_gnu291_fp32")
        }
        self.objdump = PROJECT / ".cache/binutils/bin/mips64-elf-objdump"
        self.objcopy = PROJECT / ".cache/binutils/bin/mips64-elf-objcopy"
        for path in [*self.assemblers.values(), self.objdump, self.objcopy]:
            if not path.is_file():
                self.skipTest(f"Required installed assembler control tool is absent: {path}")
        for key, path in self.assemblers.items():
            self.assertEqual(sha256(path), lock[key]["assembler_sha256"], f"Installed {key} pin differs")
        self.temporary = tempfile.TemporaryDirectory()
        self.addCleanup(self.temporary.cleanup)
        self.root = Path(self.temporary.name)

    def assemble(self, key: str, text: str, *, fp32: bool = False) -> tuple[list[str], bytes]:
        source = self.root / "control.s"
        obj = self.root / "control.o"
        source.write_text(text)
        result = subprocess.run(
            [str(self.assemblers[key]), *self.FLAGS, *(["-mfp32"] if fp32 else []),
             "-o", str(obj), str(source)], capture_output=True, text=True,
        )
        self.assertEqual(result.returncode, 0, result.stderr)
        result = subprocess.run(
            [str(self.objdump), "-drz", "-m", "mips:4300", "-M", "no-aliases,reg-names=32", str(obj)],
            capture_output=True, text=True,
        )
        self.assertEqual(result.returncode, 0, result.stderr)
        words = re.findall(r"^\s*[0-9a-f]+:\s+([0-9a-f]{8})\s", result.stdout, re.MULTILINE)
        self.assertTrue(words, result.stdout)
        return words, obj.read_bytes()

    def assert_sequence(self, words: list[str], sequence: list[str]) -> None:
        self.assertTrue(
            any(words[index:index + len(sequence)] == sequence for index in range(len(words) - len(sequence) + 1)),
            f"Missing {sequence!r} in {words!r}",
        )

    def test_fp32_literal_keeps_move_address_and_hazard_macros(self) -> None:
        words, _ = self.assemble("binutils_gnu291_fp32", self.CONTROL, fp32=True)
        self.assertEqual(words[:3], ["0080102d", "3c030000", "24630000"])
        self.assert_sequence(words, ["3c0141f0", "44810800", "44800000", "00000000", "46200080"])
        self.assert_sequence(words, ["44845000", "00000000", "468052a0"])
        self.assert_sequence(words, ["460c5032", "00000000"])
        self.assert_sequence(words, ["00001012", "00000000", "00000000", "00c70018"])
        self.assertFalse(any(int(word, 16) & 0xFFE00000 == 0x44A00000 for word in words), words)

    def test_without_fp32_matches_stock_and_retains_dmtc1(self) -> None:
        stock_words, stock_object = self.assemble("binutils_gnu291", self.CONTROL)
        words, obj = self.assemble("binutils_gnu291_fp32", self.CONTROL)
        self.assertEqual(words, stock_words)
        self.assertEqual(obj, stock_object)
        self.assertIn("44a10000", words)

    def test_fp32_does_not_change_gpr_target_li_d(self) -> None:
        text = ".text\nli.d $2,4294967296.0\n"
        stock_words, stock_object = self.assemble("binutils_gnu291", text)
        for fp32 in (False, True):
            with self.subTest(fp32=fp32):
                words, obj = self.assemble("binutils_gnu291_fp32", text, fp32=fp32)
                self.assertEqual(words, stock_words)
                self.assertEqual(obj, stock_object)
                self.assertFalse(any(int(word, 16) >> 26 == 0x11 for word in words), words)

    def test_set_mips3_and_mips4_reset_command_line_fp32(self) -> None:
        for isa in (3, 4):
            with self.subTest(isa=isa):
                text = f".text\n.set mips{isa}\nli.d $f0,4294967296.0\n"
                stock_words, stock_object = self.assemble("binutils_gnu291", text)
                words, obj = self.assemble("binutils_gnu291_fp32", text, fp32=True)
                self.assertEqual(words, stock_words)
                self.assertEqual(obj, stock_object)
                self.assertIn("44a10000", words)

    def test_set_mips1_and_mips2_select_fpr_pairs(self) -> None:
        for isa in (1, 2):
            with self.subTest(isa=isa):
                text = f".text\n.set mips{isa}\nli.d $f0,4294967296.0\n"
                stock_words, stock_object = self.assemble("binutils_gnu291", text)
                words, obj = self.assemble("binutils_gnu291_fp32", text, fp32=True)
                self.assertEqual(words, stock_words)
                self.assertEqual(obj, stock_object)
                self.assert_sequence(words, ["3c0141f0", "44810800", "44800000"])

    def test_set_mips0_restores_command_line_fpr_width(self) -> None:
        literal = "li.d $f0,4294967296.0\n"
        for fp32 in (False, True):
            with self.subTest(fp32=fp32):
                baseline = self.assemble("binutils_gnu291_fp32", ".text\n" + literal, fp32=fp32)
                restored = self.assemble("binutils_gnu291_fp32", ".text\n.set mips3\n.set mips0\n" + literal, fp32=fp32)
                self.assertEqual(restored, baseline)

    def test_set_push_pop_restores_fp32_after_isa_reset(self) -> None:
        literal = "li.d $f0,4294967296.0\n"
        baseline = self.assemble("binutils_gnu291_fp32", ".text\n" + literal, fp32=True)
        restored = self.assemble("binutils_gnu291_fp32", ".text\n.set push\n.set mips3\n.set pop\n" + literal, fp32=True)
        self.assertEqual(restored, baseline)

    def test_fp32_preserves_odd_sized_data_without_added_padding(self) -> None:
        payload = bytes(range(123))
        text = ".text\nnop\n.section .rodata\n.byte " + ",".join(map(str, payload)) + "\n"
        for key, fp32 in (("binutils_gnu291", False), ("binutils_gnu291_fp32", False), ("binutils_gnu291_fp32", True)):
            with self.subTest(assembler=key, fp32=fp32):
                self.assemble(key, text, fp32=fp32)
                output = self.root / "rodata.bin"
                result = subprocess.run(
                    [str(self.objcopy), "-O", "binary", "-j", ".rodata", str(self.root / "control.o"), str(output)],
                    capture_output=True, text=True,
                )
                self.assertEqual(result.returncode, 0, result.stderr)
                self.assertEqual(output.read_bytes(), payload)


class IntermediateInventoryTests(unittest.TestCase):
    def assert_c_owned_section_references(self, *, dictionary: bool) -> None:
        unit = "units/resident/contpfs"
        source = f"src/{unit}.c"
        graph = {
            f"obj/source/{source[:-2]}.o": {
                "source": source, "input": f"source/{source}",
                "source_kind": "c", "image_id": "resident",
            },
            "obj/generated/asm/resident.o": {
                "source": "generated/asm/resident.s", "input": "generated/asm/resident.s",
                "source_kind": "asm", "image_id": "resident",
            },
        }
        matches = [{"source": source, "symbol": "func_80026800"}]
        for kind in ["data", ".data", "rodata", ".rodata", "bss", ".bss"]:
            with self.subTest(shape="dictionary" if dictionary else "list", kind=kind), tempfile.TemporaryDirectory() as temporary:
                root = Path(temporary)
                (root / "config").mkdir()
                if dictionary:
                    section = {"type": kind, "name": unit, "vram": 0x800411D0} if kind.lstrip(".") == "bss" else {"start": 0x12360, "type": kind, "name": unit}
                    asm_section = {**section, "name": "resident"}
                else:
                    section = [0x14400 if kind.lstrip(".") == "bss" else 0x12360, kind, unit]
                    asm_section = [section[0], kind, "resident"]
                config = {"segments": [{"name": "main", "type": "code", "subsegments": [
                    [0x1060, "asm", "resident"], [0x1C00, "c", unit], section, asm_section,
                ]}, [0x2000000]]}
                (root / "config/shiren2.jp.yaml").write_text(yaml.safe_dump(config))
                reference = f"asm/nonmatchings/{unit}/D_800386F0.s"
                observed = {
                    reference,
                    "asm/nonmatchings/resident/D_800386F0.s",
                    f"asm/nonmatchings/{unit}_other/D_800386F0.s",
                    f"asm/nonmatchings/{unit}/D_800386F0.bin",
                }
                generated, compiled = intermediate_keys(graph, matches, root, observed=observed)
                self.assertEqual(generated, {
                    "undefined_funcs_auto.txt", "undefined_syms_auto.txt", "asm/resident.s",
                    f"asm/matchings/{unit}/func_80026800.s",
                    f"asm/data/{unit}.{kind.lstrip('.')}.s",
                } | ({reference} if kind == ".rodata" else set()))
                # YAML reference evidence adds no compiler input or C object;
                # only the graph's C translation unit supplies .s/.d evidence.
                self.assertEqual(compiled, {f"src/{unit}.s", f"src/{unit}.d"})
                self.assertEqual(len(graph), 2)

    def test_list_sections_require_c_owned_reference_evidence(self) -> None:
        self.assert_c_owned_section_references(dictionary=False)

    def test_dictionary_sections_require_c_owned_reference_evidence(self) -> None:
        self.assert_c_owned_section_references(dictionary=True)


class CxxGraphTests(unittest.TestCase):
    """A `cpp` splat subsegment is a C++ TU: same object path scheme, `.cpp` source, own kind."""
    image = SimpleNamespace(image_id="main_14400", split_status="active", rom_start=0x14400, rom_end=0x1339F0,
        vram_start=0x800413C0, vram_end=0x801609B0, vram_rom_delta=0x8002CFC0, splat_segments=("main",), bss_ranges=())
    units = {"c": "units/main_14400/c_unit", "cpp": "units/main_14400/cxx_unit"}

    def write(self, root: Path, *, kinds: dict[str, str] | None = None, files: tuple[str, ...] = ("c_unit.c", "cxx_unit.cpp"),
              bindings: dict[str, str] | None = None) -> SimpleNamespace:
        kinds = kinds or {"c_unit": "c", "cxx_unit": "cpp"}
        config = {"segments": [{"name": "main", "type": "code", "start": 0x14400, "vram": 0x800413C0, "subsegments": [
            [0x14400, "asm", "main_head"], [0x15A80, kinds["c_unit"], self.units["c"]],
            [0x15AC0, kinds["cxx_unit"], self.units["cpp"]], [0x15B00, "asm", "main_tail"]]}, [0x1339F0]]}
        (root / "source/config").mkdir(parents=True)
        (root / "source/config/shiren2.jp.yaml").write_text(yaml.safe_dump(config))
        for name in files:
            path = root / "source/src/units/main_14400" / name
            path.parent.mkdir(parents=True, exist_ok=True)
            path.write_text("/* fixture */\n")
        for name in ("main_head", "main_tail"):
            path = root / f"generated/asm/{name}.s"
            path.parent.mkdir(parents=True, exist_ok=True)
            path.write_text("/* fixture */\n")
        objects = ["obj/generated/asm/main_head.o", f"obj/source/src/{self.units['c']}.o", f"obj/source/src/{self.units['cpp']}.o",
                   "obj/generated/asm/main_tail.o"]
        (root / "shiren2.ld").write_text("SECTIONS\n{\n    .main :\n    {\n" + "".join(f"        {name}(.text);\n" for name in objects) + "    }\n}\n")
        bindings = bindings if bindings is not None else {f"src/{self.units['c']}.c": "main_14400", f"src/{self.units['cpp']}.cpp": "main_14400"}
        return SimpleNamespace(images={"main_14400": self.image}, source_bindings=bindings, components=(), evidence={})

    def test_cpp_subsegment_yields_a_cpp_graph_object_and_intermediates(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            inventory = self.write(root)
            with patch("certification.load_inventory", return_value=inventory):
                graph = build_graph(root)
            cxx = graph[f"obj/source/src/{self.units['cpp']}.o"]
            self.assertEqual(cxx, {"source": f"src/{self.units['cpp']}.cpp", "input": f"source/src/{self.units['cpp']}.cpp",
                                   "source_kind": "cpp", "image_id": "main_14400"})
            self.assertEqual(graph[f"obj/source/src/{self.units['c']}.o"]["source_kind"], "c")
            matches = [{"source": f"src/{self.units['cpp']}.cpp", "symbol": "func_80042E80"}]
            generated, compiled = intermediate_keys(graph, matches, root / "source")
            self.assertEqual(compiled, {f"src/{self.units[kind]}.{suffix}" for kind in ("c", "cpp") for suffix in ("s", "d")})
            self.assertIn(f"asm/matchings/{self.units['cpp']}/func_80042E80.s", generated)

    def test_cpp_declarations_need_their_file_binding_and_one_unit_name(self) -> None:
        cases = [({"files": ("c_unit.c", "cxx_unit.c")}, "Missing declared cpp input"),
                 ({"bindings": {f"src/{self.units['c']}.c": "main_14400", f"src/{self.units['cpp']}.c": "main_14400"}},
                  "C\\+\\+ translation-unit image declaration"),
                 ({"kinds": {"c_unit": "c", "cxx_unit": "c"}}, "C translation-unit image declaration")]
        for options, message in cases:
            with self.subTest(options=options), tempfile.TemporaryDirectory() as temporary:
                root = Path(temporary)
                inventory = self.write(root, **options)
                with patch("certification.load_inventory", return_value=inventory), self.assertRaisesRegex(ValueError, message):
                    build_graph(root)
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            inventory = self.write(root)
            path = root / "source/config/shiren2.jp.yaml"
            config = yaml.safe_load(path.read_text())
            config["segments"][0]["subsegments"].insert(3, [0x15AE0, "c", self.units["cpp"]])
            path.write_text(yaml.safe_dump(config))
            with self.assertRaisesRegex(ValueError, "translation-unit image declaration is missing or ambiguous"):
                splat_bindings(root / "source", inventory)

    def test_duplicate_compiled_stems_fail_with_valid_source_bindings(self) -> None:
        for first, second in (("c", "cpp"), ("cpp", "c"), ("c", "c"), ("cpp", "cpp")):
            for split_segments in (False, True):
                with self.subTest(kinds=(first, second), split_segments=split_segments), tempfile.TemporaryDirectory() as temporary:
                    root = Path(temporary)
                    inventory = self.write(root)
                    path = root / "source/config/shiren2.jp.yaml"
                    config = yaml.safe_load(path.read_text())
                    segment = config["segments"][0]
                    parts = [[0x14400, first, self.units["c"]],
                             [0x15A80, "c", self.units["cpp"]],
                             [0x15AC0, second, self.units["c"]]]
                    segment["subsegments"] = parts
                    inventory.source_bindings = {
                        f"src/{unit}.{kind}": "main_14400" for _, kind, unit in parts
                    }
                    if split_segments:
                        segment["subsegments"] = parts[:2]
                        config["segments"].insert(1, {
                            **segment, "name": "main_second", "start": 0x15AC0,
                            "vram": 0x15AC0 + self.image.vram_rom_delta, "subsegments": parts[2:],
                        })
                        inventory.images["main_14400"] = SimpleNamespace(
                            **{**vars(self.image), "splat_segments": ("main", "main_second")}
                        )
                    path.write_text(yaml.safe_dump(config))
                    with self.assertRaisesRegex(ValueError, "translation-unit image declaration is missing or ambiguous"):
                        splat_bindings(root / "source", inventory)

    def test_compiled_stems_keep_directory_and_embedded_suffix_identity(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            inventory = self.write(root)
            path = root / "source/config/shiren2.jp.yaml"
            config = yaml.safe_load(path.read_text())
            parts = [[0x14400, "c", "units/main_14400/left/shared.v1"],
                     [0x15A80, "cpp", "units/main_14400/right/shared.v1"],
                     [0x15AC0, "cpp", "units/main_14400/left/shared.v2"]]
            config["segments"][0]["subsegments"] = parts
            inventory.source_bindings = {
                f"src/{unit}.{kind}": "main_14400" for _, kind, unit in parts
            }
            path.write_text(yaml.safe_dump(config))
            sections, sources = splat_bindings(root / "source", inventory)
            self.assertEqual(sections, {"main": "main_14400", "main_bss": "main_14400"})
            self.assertEqual(sources, inventory.source_bindings)


class Stage1StorageTests(unittest.TestCase):
    """Synthetic ELF/map tests prove validator behavior, never real linkage or ROM certification."""
    parent = "overlay_b_136dc0_storage"
    object_name = "obj/generated/assets/overlay_136dc0_initialized_opaque.o"
    input_name = "generated/assets/overlay_136dc0_initialized_opaque.bin"

    @staticmethod
    def elf_bytes(body: bytes, *, linked: bool, address: int = 0x801E4EA0, lma: int = 0x136DC0, extra_bss: bool = False) -> bytes:
        name = ".overlay_b_136dc0_storage" if linked else ".data"
        section_names = ["", name, ".shstrtab", ".strtab", ".symtab"] + ([".extra_bss"] if extra_bss else [])
        shstrings = b""; offsets = {}
        for item in section_names:
            offsets[item] = len(shstrings); shstrings += item.encode() + b"\0"
        labels = ("overlay_b_136dc0_storage_ROM_START", "overlay_b_136dc0_storage_ROM_END") if linked else ()
        strings = b"\0"; positions = {}
        for label in labels:
            positions[label] = len(strings); strings += label.encode() + b"\0"
        symbols = bytes(16)
        for label, value in zip(labels, (0x136DC0, 0x148460)):
            symbols += struct.pack(">IIIBBH", positions[label], value, 0, 0x10, 0, 0xFFF1)
        data = bytearray(0x100); data.extend(body)
        shstr_offset = len(data); data.extend(shstrings)
        str_offset = len(data); data.extend(strings)
        data.extend(bytes((-len(data)) % 4)); sym_offset = len(data); data.extend(symbols)
        shoff = len(data)
        headers = [bytes(40), struct.pack(">IIIIIIIIII", offsets[name], 1, 3, address if linked else 0, 0x100, len(body), 0, 0, 1, 0),
            struct.pack(">IIIIIIIIII", offsets[".shstrtab"], 3, 0, 0, shstr_offset, len(shstrings), 0, 0, 1, 0),
            struct.pack(">IIIIIIIIII", offsets[".strtab"], 3, 0, 0, str_offset, len(strings), 0, 0, 1, 0),
            struct.pack(">IIIIIIIIII", offsets[".symtab"], 2, 0, 0, sym_offset, len(symbols), 3, 1, 4, 16)]
        if extra_bss:
            headers.append(struct.pack(">IIIIIIIIII", offsets[".extra_bss"], 8, 3, address + len(body), 0, 6, 0, 0, 1, 0))
        for header in headers:
            data.extend(header)
        ident = b"\x7fELF\x01\x02\x01" + bytes(9)
        data[:52] = ident + struct.pack(">HHIIIIIHHHHHH", 2 if linked else 1, 8, 1, 0, 52 if linked else 0, shoff, 0, 52, 32 if linked else 0, 1 if linked else 0, 40, len(headers), 2)
        if linked:
            data[52:84] = struct.pack(">IIIIIIII", 1, 0x100, address, lma, len(body), len(body), 6, 16)
        return bytes(data)

    def fixture(self, root: Path) -> tuple[SimpleNamespace, dict, bytes, dict]:
        body = (bytes(range(256)) * 279)[:0x116A0]
        reference = bytes(0x136DC0) + body
        image = SimpleNamespace(image_id="overlay_136dc0", split_status="active", rom_start=0x136DC0, rom_end=0x148460,
            vram_start=0x801E4EA0, vram_end=0x801F6540, vram_rom_delta=0x800AE0E0, splat_segments=(self.parent,), bss_ranges=())
        graph = {self.object_name: {"source": self.input_name, "input": self.input_name, "source_kind": "binary", "image_id": image.image_id}}
        for name, data in ((self.input_name, body), (self.object_name, self.elf_bytes(body, linked=False)), ("shiren2.elf", self.elf_bytes(body, linked=True))):
            path = root/name; path.parent.mkdir(parents=True, exist_ok=True); path.write_bytes(data)
        (root/"shiren2.map").write_text(f".{self.parent}\n  0x801e4ea0 0x116a0 load address 0x136dc0\n .data 0x801e4ea0 0x116a0 {self.object_name}\n")
        return image, graph, reference, storage_elf_layout(root/"shiren2.elf")

    def write_config(self, root: Path) -> SimpleNamespace:
        image = SimpleNamespace(image_id="overlay_136dc0", split_status="active", rom_start=0x136DC0, rom_end=0x148460,
            vram_start=0x801E4EA0, vram_end=0x801F6540, vram_rom_delta=0x800AE0E0, splat_segments=(self.parent,), bss_ranges=())
        config = {"segments": [
            {"name": "overlay_1339f0_opaque", "type": "bin", "start": 0x1339F0, "vram": 0x1339F0},
            {"name": self.parent, "type": "code", "start": 0x136DC0, "vram": 0x801E4EA0, "bss_size": 0, "subsegments": [[0x136DC0, "bin", "overlay_136dc0_initialized_opaque"]]},
            {"name": "remaining", "type": "bin", "start": 0x148460, "vram": 0x148460}, [0x2000000]]}
        (root/"config").mkdir(parents=True, exist_ok=True)
        (root/"config/shiren2.jp.yaml").write_text(yaml.safe_dump(config))
        return SimpleNamespace(images={image.image_id: image}, source_bindings={}, components=(), evidence={})

    def test_complete_binary_storage_record_has_zero_C(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary); image, graph, original, layout = self.fixture(root)
            result = binary_storage_record(root, image, f".{self.parent}", graph, layout, original)
            self.assertEqual(result["C_credit"], 0)
            self.assertEqual((result["load_mapping"]["lma_start"], result["load_mapping"]["lma_end"]), (0x136DC0, 0x148460))
            self.assertEqual(result["natural_object_sections"][1]["size"], 0x116A0)

    def test_wrong_or_ambiguous_actual_load_mapping_is_rejected(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary); image, graph, original, layout = self.fixture(root)
            for mutation in ("wrong", "duplicate", "missing"):
                bad = copy.deepcopy(layout)
                if mutation == "wrong": bad["loads"][0]["lma_start"] += 4
                elif mutation == "duplicate": bad["loads"].append(copy.deepcopy(bad["loads"][0]))
                else: bad["loads"] = []
                with self.subTest(mutation=mutation), self.assertRaises(ValueError):
                    binary_storage_record(root, image, f".{self.parent}", graph, bad, original)

    def test_wrong_section_bounds_or_bytes_are_rejected(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary); image, graph, original, layout = self.fixture(root)
            for field, value in (("size", 0x116A1), ("vram_start", 0x801E4EA4), ("sha256", "0" * 64), ("type", 8)):
                bad = copy.deepcopy(layout); bad["sections"][1][field] = value
                with self.subTest(field=field), self.assertRaises(ValueError):
                    binary_storage_record(root, image, f".{self.parent}", graph, bad, original)

    def test_wrong_boundary_symbols_or_map_are_rejected(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary); image, graph, original, layout = self.fixture(root)
            bad = copy.deepcopy(layout); bad["symbols"][f"{self.parent}_ROM_END"][0]["value"] += 1
            with self.assertRaises(ValueError): binary_storage_record(root, image, f".{self.parent}", graph, bad, original)
            (root/"shiren2.map").write_text((root/"shiren2.map").read_text().replace("0x136dc0", "0x136dc4"))
            with self.assertRaises(ValueError): binary_storage_record(root, image, f".{self.parent}", graph, layout, original)

    def test_changed_binary_input_or_natural_object_BSS_is_rejected(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary); image, graph, original, layout = self.fixture(root)
            body = (root/self.input_name).read_bytes()
            (root/self.input_name).write_bytes(body[:-1] + bytes([body[-1] ^ 1]))
            with self.assertRaises(ValueError): binary_storage_record(root, image, f".{self.parent}", graph, layout, original)
            (root/self.input_name).write_bytes(body)
            (root/self.object_name).write_bytes(self.elf_bytes(body, linked=False, extra_bss=True))
            with self.assertRaises(ValueError): binary_storage_record(root, image, f".{self.parent}", graph, layout, original)

    def test_extra_RAM_or_ROM_allocation_is_rejected(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary); image, graph, original, layout = self.fixture(root)
            for kind in ("RAM", "ROM"):
                bad = copy.deepcopy(layout); extra = copy.deepcopy(bad["sections"][1]); extra.update(name=".orphan", size=4)
                if kind == "ROM":
                    extra["vram_start"] = 0x80010000; extra["file_offset"] += 8
                    load = copy.deepcopy(bad["loads"][0]); load.update(index=1, vram_start=extra["vram_start"], file_offset=extra["file_offset"], lma_start=0x136DC0, file_size=4, memory_size=4)
                    bad["loads"].append(load)
                bad["sections"].append(extra)
                with self.subTest(kind=kind), self.assertRaises(ValueError):
                    binary_storage_record(root, image, f".{self.parent}", graph, bad, original)

    def test_wrong_object_owner_or_nonbinary_kind_is_rejected(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary); image, graph, original, layout = self.fixture(root)
            for field, value in (("image_id", "overlay_1339f0"), ("source_kind", "c")):
                bad = copy.deepcopy(graph); bad[self.object_name][field] = value
                with self.subTest(field=field), self.assertRaises(ValueError):
                    binary_storage_record(root, image, f".{self.parent}", bad, layout, original)

    def test_supported_splat_storage_binding_preserves_unknown_BSS_and_zero_C(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary); inventory = self.write_config(root)
            sections, sources = splat_bindings(root, inventory)
            self.assertEqual(sections[self.parent], "overlay_136dc0")
            self.assertEqual(sections["overlay_1339f0_opaque"], "rom_storage:overlay_1339f0_opaque")
            self.assertEqual(sources, {})

    def test_actual_generated_graph_has_one_B_binary_owner(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary); inventory = self.write_config(root/"source")
            for name in ("overlay_1339f0_opaque", "overlay_136dc0_initialized_opaque", "remaining"):
                path = root/f"generated/assets/{name}.bin"; path.parent.mkdir(parents=True, exist_ok=True); path.write_bytes(b"synthetic bin")
            blocks = []
            for section, name in (("overlay_1339f0_opaque", "overlay_1339f0_opaque"), (self.parent, "overlay_136dc0_initialized_opaque"), ("remaining", "remaining")):
                blocks.append(f"    .{section} :\n    {{\n        obj/generated/assets/{name}.o(.data);\n    }}\n")
            (root/"shiren2.ld").write_text("SECTIONS\n{\n" + "".join(blocks) + "}\n")
            with patch("certification.load_inventory", return_value=inventory):
                graph = build_graph(root)
                self.assertEqual(graph[self.object_name]["image_id"], "overlay_136dc0")
                self.assertEqual({item["source_kind"] for item in graph.values()}, {"binary"})
                # A declaration cannot reuse B's same object under a second owner.
                (root/"shiren2.ld").write_text((root/"shiren2.ld").read_text().replace("obj/generated/assets/overlay_1339f0_opaque.o", self.object_name))
                with self.assertRaisesRegex(ValueError, "no unique original image/storage owner"):
                    build_graph(root)

    def test_intermediate_inventory_handles_dict_bins_and_preserves_C_references(self) -> None:
        for with_existing_C in (False, True):
            with self.subTest(with_existing_C=with_existing_C), tempfile.TemporaryDirectory() as temporary:
                root = Path(temporary); self.write_config(root)
                graph = {self.object_name: {"source": self.input_name, "input": self.input_name, "source_kind": "binary", "image_id": "overlay_136dc0"}}
                matches = []
                if with_existing_C:
                    unit = "units/resident/contpfs"; source = f"src/{unit}.c"
                    graph[f"obj/source/{source[:-2]}.o"] = {"source": source, "input": f"source/{source}", "source_kind": "c", "image_id": "resident"}
                    matches = [{"source": source, "symbol": "func_existing"}]
                    path = root/"config/shiren2.jp.yaml"; config = yaml.safe_load(path.read_text())
                    config["segments"].insert(0, {"name": "existing_main", "type": "code", "subsegments": [[0x1060, "c", unit], [0x14400, "bss", unit]]})
                    path.write_text(yaml.safe_dump(config))
                generated, compiled = intermediate_keys(graph, matches, root)
                self.assertIn("assets/overlay_136dc0_initialized_opaque.bin", generated)
                if with_existing_C:
                    self.assertEqual(compiled, {f"src/{unit}.s", f"src/{unit}.d"})
                    self.assertIn(f"asm/data/{unit}.bss.s", generated)
                    self.assertIn(f"asm/matchings/{unit}/func_existing.s", generated)
                else:
                    self.assertEqual(compiled, set())
                    self.assertEqual(generated, {"undefined_funcs_auto.txt", "undefined_syms_auto.txt", "assets/overlay_136dc0_initialized_opaque.bin"})

    def test_unsupported_padding_BSS_or_C_split_is_rejected(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary); inventory = self.write_config(root); path = root/"config/shiren2.jp.yaml"
            config = yaml.safe_load(path.read_text())
            for field, value in (("subalign", 4), ("bss_size", 6), ("subsegments", [[0x136DC0, "c", "invented_B"]])):
                bad = copy.deepcopy(config); bad["segments"][1][field] = value; path.write_text(yaml.safe_dump(bad))
                with self.subTest(field=field), self.assertRaises(ValueError): splat_bindings(root, inventory)

    def test_opaque_storage_VMA_cannot_claim_A_runtime_or_change_ROM_end(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary); inventory = self.write_config(root); path = root/"config/shiren2.jp.yaml"
            config = yaml.safe_load(path.read_text())
            for mutation in ("RAM", "end"):
                bad = copy.deepcopy(config)
                if mutation == "RAM": bad["segments"][0]["vram"] = 0x801E4EA0
                else: bad["segments"][1]["start"] += 1
                path.write_text(yaml.safe_dump(bad))
                with self.subTest(mutation=mutation), self.assertRaises(ValueError): splat_bindings(root, inventory)

    def test_duplicate_output_section_owner_is_rejected(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary); inventory = self.write_config(root); path = root/"config/shiren2.jp.yaml"
            config = yaml.safe_load(path.read_text()); config["segments"].insert(0, [0, "bin", self.parent])
            path.write_text(yaml.safe_dump(config))
            with self.assertRaises(ValueError): splat_bindings(root, inventory)

    def test_linked_image_record_keeps_complete_window_and_empty_BSS(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary); image, graph, original, layout = self.fixture(root)
            inventory = self.write_config(root/"source")
            result = linked_image_records(root, inventory, original, graph=graph)
            self.assertEqual(result["overlay_136dc0"]["bss"], [])
            self.assertEqual(result["overlay_136dc0"]["initialized"][0]["rom_end"], 0x148460)

    def test_shape_valid_wrong_digest_fails_original_byte_seam(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary); image, graph, original, layout = self.fixture(root)
            inventory = self.write_config(root); inventory.images["overlay_136dc0"].rom_sha256 = "0" * 64
            (root/"config/images.json").write_text("{}")
            # Only target identity/parser setup are mocked; the actual interval digest check runs.
            with patch("certification.attest"), patch("certification.load_inventory", return_value=inventory), self.assertRaisesRegex(ValueError, "Original image digest"):
                image_evidence(root, original)

    def test_orphan_allocated_metadata_outside_B_is_rejected(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary); image, graph, original, layout = self.fixture(root)
            extra = copy.deepcopy(layout["sections"][1]); extra.update(name=".orphan_metadata", vram_start=0x80010000, size=4)
            layout["sections"].append(extra)
            with self.assertRaisesRegex(ValueError, "no declared section owner"):
                binary_storage_record(root, image, f".{self.parent}", graph, layout, original)

    def test_failed_replay_lifecycle_retains_raw_outputs(self) -> None:
        with tempfile.TemporaryDirectory() as temporary, patch.dict(os.environ, {}, clear=True):
            root = Path(temporary)
            with self.assertRaisesRegex(ValueError, "synthetic replay failure"):
                with retained_trial(root, "fixture") as trial:
                    retained = Path(trial); (retained/"raw-object.o").write_bytes(b"retained negative")
                    raise ValueError("synthetic replay failure")
            self.assertEqual((retained/"raw-object.o").read_bytes(), b"retained negative")
            self.assertEqual(json.loads((retained/"lifecycle.json").read_text())["status"], "failed")

    def test_replay_evidence_cannot_pollute_source_inventory(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            with patch.dict(os.environ, {"SHIREN2_REPLAY_EVIDENCE_ROOT": str(root/"source/replay")}), self.assertRaises(ValueError):
                with retained_trial(root, "fixture"):
                    self.fail("Unsupported replay evidence placement")

    def test_successful_replay_prunes_bulk_but_keeps_command_evidence(self) -> None:
        with tempfile.TemporaryDirectory() as temporary, patch.dict(os.environ, {}, clear=True):
            root = Path(temporary)
            with retained_trial(root, "fixture") as trial:
                retained = Path(trial)
                (retained/"source/src").mkdir(parents=True); (retained/"source/src/a.c").write_text("int a;\n")
                (retained/"shiren2.elf").write_bytes(b"linked elf")
                (retained/"command-evidence").mkdir(); (retained/"command-evidence/001.stdout").write_bytes(b"out")
                (retained/"stderr.txt").write_bytes(b"err")
            self.assertEqual(sorted(path.name for path in retained.iterdir()), ["command-evidence", "lifecycle.json", "stderr.txt"])
            self.assertEqual((retained/"command-evidence/001.stdout").read_bytes(), b"out")
            lifecycle = json.loads((retained/"lifecycle.json").read_text())
            self.assertEqual((lifecycle["status"], lifecycle["raw_artifacts_retained"], lifecycle["bulk_outputs_pruned"]), ("complete", False, True))
            self.assertEqual(lifecycle["pruned_file_sha256"], {"shiren2.elf": hashlib.sha256(b"linked elf").hexdigest()})
            self.assertEqual(lifecycle["pruned_directories"], ["source"])

    def test_successful_replay_full_retention_is_opt_in(self) -> None:
        with tempfile.TemporaryDirectory() as temporary, patch.dict(os.environ, {"SHIREN2_RETAIN_SUCCESSFUL_REPLAYS": "1"}, clear=True):
            root = Path(temporary)
            with retained_trial(root, "fixture") as trial:
                retained = Path(trial); (retained/"shiren2.elf").write_bytes(b"linked elf")
            self.assertEqual((retained/"shiren2.elf").read_bytes(), b"linked elf")
            self.assertTrue(json.loads((retained/"lifecycle.json").read_text())["raw_artifacts_retained"])


class SourcePolicyTests(unittest.TestCase):
    def violations(self, files: dict[str, str]) -> list[str]:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            (root/"include").mkdir(); (root/"include/common.h").write_text("typedef int s32;\n")
            for name, text in files.items():
                (root/name).parent.mkdir(parents=True, exist_ok=True); (root/name).write_text(text)
            return source_policy_violations(root)

    def test_current_project_sources_pass(self) -> None:
        self.assertEqual(source_policy_violations(PROJECT), [])

    def test_src_local_header_with_inline_assembly_is_rejected(self) -> None:
        for body in ["#define BARRIER() __asm__ volatile(\"nop\")\n", "#define BARRIER() as\\\nm(\"nop\")\n"]:
            with self.subTest(body=body):
                violations = self.violations({"src/units/main/f.c": '#include "f_types.h"\nvoid f(void) {}\n', "src/units/main/f_types.h": body})
                self.assertEqual(violations, ["src/units/main/f_types.h: inline assembly token"])

    def test_token_paste_is_rejected_outside_comments_and_strings(self) -> None:
        self.assertEqual(self.violations({"src/f.c": "#define CAT(a, b) a ## b\nCAT(a, sm)(\"nop\");\n"}), ["src/f.c: token-paste operator"])
        self.assertEqual(self.violations({"src/f.c": "/* a ## b */ const char *s = \"##\";\n"}), [])

    def test_escaping_or_non_header_includes_are_rejected(self) -> None:
        for include in ['"/tmp/evil.h"', '"../../generated/x.h"', '<sub/../../x.h>', '"macro.inc"', 'TARGET']:
            with self.subTest(include=include):
                violations = self.violations({"src/f.c": f"#include {include}\nint f;\n"})
                self.assertEqual(len(violations), 1)
                self.assertIn("include", violations[0])
        self.assertEqual(self.violations({"src/f.c": '#include "common.h"\n#include <sub/ok.h>\n// #include "../x.h"\n'}), [])

    def test_cpp_sources_are_scanned_and_reject_exception_and_rtti_constructs(self) -> None:
        self.assertEqual(self.violations({"src/f.cpp": 'extern "C" void f(void) { asm("nop"); }\n'}), ["src/f.cpp: inline assembly token"])
        for body in ["void f() { throw 1; }\n", "void f() { try { } catch (...) { } }\n", "int f(S *s) { return typeid(*s) == 0; }\n",
                     "T *f(S *s) { return dynamic_cast<T *>(s); }\n"]:
            with self.subTest(body=body):
                violations = self.violations({"src/f.cpp": body})
                self.assertEqual(len(violations), 1, violations)
                self.assertIn("exception/RTTI construct", violations[0])
        # The same words are ordinary identifiers in C, and comments/strings are ignored in C++.
        self.assertEqual(self.violations({"src/f.c": "int try, catch, throw;\n",
                                          "src/g.cpp": '// throw away\nconst char *s = "try";\n'}), [])


@unittest.skipUnless(CANONICAL_ROM.is_file() and (PROJECT / "build/latest.json").is_file(), "Private ROM and a completed build required")
class LiveVerificationTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls) -> None:
        cls.rom = CANONICAL_ROM.read_bytes()
        cls.receipt_path = Path(os.environ["SHIREN2_RECEIPT"]) if os.environ.get("SHIREN2_RECEIPT") else PROJECT / json.loads((PROJECT / "build/latest.json").read_text())["receipt"]
        cls.receipt = json.loads(cls.receipt_path.read_text())

    def private_evidence_copy(self, root: Path) -> tuple[Path, dict]:
        directory = root / "run"
        copy_private_evidence_core(self.receipt_path, directory)
        (directory / "tmp").mkdir()
        receipt = copy.deepcopy(self.receipt)
        receipt["compiler_environment"] = compiler_environment(directory, receipt["profile"])
        override = directory / "splat.override.yaml"
        override.write_text(yaml.safe_dump(splat_options(directory, CANONICAL_ROM)))
        receipt["artifacts"]["splat.override.yaml"] = sha256(override)
        return directory, receipt

    def test_current_build_passes(self) -> None:
        verify_receipt(self.receipt_path)
        compare_rom(self.receipt_path.parent / "shiren2.z64")

    def test_cpp_tu_builds_and_replays_with_language_coverage(self) -> None:
        """Real private-ROM proof, intentionally run only with the integrator's full suite."""
        with tempfile.TemporaryDirectory(dir=PROJECT / "scratch") as temporary:
            directory = Path(temporary) / "cpp-run"
            directory.mkdir()
            create_snapshot(self.receipt_path.parent / "source", directory / "source")
            source_root = directory / "source"
            matches_path = source_root / "config/matches.json"
            matches = read_json(matches_path)
            row = next(m for m in matches["functions"] if m["symbol"] == "func_80042A50")
            old = row["source"]
            new = Path(old).with_suffix(".cpp").as_posix()
            if old != new:
                original = source_root / old
                (source_root / new).write_text('extern "C" {\n' + original.read_text() + '\n}\n')
                original.unlink()
                row["source"] = new
                write_json(matches_path, matches)
                images_path = source_root / "config/images.json"
                images = read_json(images_path)
                images["source_bindings"][new] = images["source_bindings"].pop(old)
                write_json(images_path, images)
                split_path = source_root / "config/shiren2.jp.yaml"
                split = yaml.safe_load(split_path.read_text())
                unit = Path(old).relative_to("src").with_suffix("").as_posix()
                for segment in split["segments"]:
                    if isinstance(segment, dict):
                        for part in segment.get("subsegments", []):
                            if isinstance(part, list) and len(part) >= 3 and part[1:3] == ["c", unit]:
                                part[1] = "cpp"
                split_path.write_text(yaml.safe_dump(split))
            profiles_path = source_root / "config/compiler_profiles.json"
            profiles = read_json(profiles_path)
            profiles["tu_profiles"].pop(old, None)
            profiles["tu_profiles"][new] = "gxx281pm-gnu291-O2-unsigned"
            write_json(profiles_path, profiles)
            write_json(directory / "input-manifest.json", input_manifest(source_root))
            receipt = read_json(execute_snapshot(directory, PROJECT, CANONICAL_ROM))
            object_path = f"obj/source/{Path(new).with_suffix('.o').as_posix()}"
            self.assertEqual(receipt["graph"][object_path]["source_kind"], "cpp")
            self.assertIn(new, receipt["dependencies"])
            self.assertIn(object_path, receipt["reproduction"]["c_objects"])
            self.assertGreaterEqual(receipt["coverage"]["matched_cpp_functions"], 1)
            self.assertGreaterEqual(receipt["coverage"]["matched_cpp_bytes"], 24)
            verify_receipt(directory / "receipt.json", require_current=False)

    def test_python_aliases_share_the_same_venv_runtime_identity(self) -> None:
        for name in ["python", "python3"]:
            with self.subTest(alias=name), patch("certification.sys.executable", str(PROJECT / ".venv/bin" / name)):
                verify_receipt(self.receipt_path, require_current=False)

    def test_wrong_rom_bytes_are_rejected(self) -> None:
        mutations = {"constant": 0x1093, "call_target": 0x1073, "delay_slot": 0x1077, "c_mmio_address": 0x12C7, "opaque_asset": 0x1F00000}
        for name, offset in mutations.items():
            bad = bytearray(self.rom)
            bad[offset] ^= 1
            with self.subTest(mutation=name):
                with self.assertRaisesRegex(ValueError, "Wrong target"):
                    attest(bytes(bad))
        with self.assertRaisesRegex(ValueError, "Wrong target"):
            attest(self.rom[:-4])

    def test_comparison_rejects_actual_corrupted_output(self) -> None:
        bad = bytearray(self.rom)
        bad[0x12C7] ^= 1
        with tempfile.TemporaryDirectory(dir=PROJECT / "scratch") as temporary:
            path = Path(temporary) / "bad.z64"
            path.write_bytes(bad)
            with self.assertRaisesRegex(ValueError, "ROM mismatch at offset 0x12C7"):
                compare_rom(path)

    def test_stale_c_source_receipt_is_rejected(self) -> None:
        changed = copy.deepcopy(self.receipt)
        changed["input_manifest"]["files"]["src/hardware_ai_length.c"]["sha256"] = "0" * 64
        with self.assertRaisesRegex(ValueError, "input manifest"):
            verify_receipt(self.receipt_path, changed)

    def test_omitted_evidence_is_rejected(self) -> None:
        for key in ["artifacts", "objects", "graph", "generated", "compiled", "dependencies", "reproduction", "image_inventory"]:
            changed = copy.deepcopy(self.receipt)
            changed[key] = {}
            with self.subTest(inventory=key), self.assertRaises(ValueError):
                verify_receipt(self.receipt_path, changed)
        changed = copy.deepcopy(self.receipt)
        del changed["input_manifest"]
        with self.assertRaises(ValueError):
            verify_receipt(self.receipt_path, changed)

    def test_wrong_attribution_is_rejected(self) -> None:
        changed = copy.deepcopy(self.receipt)
        changed["c_attributions"][0]["object"] = "obj/generated/asm/resident.o"
        with self.assertRaises(ValueError):
            verify_receipt(self.receipt_path, changed)

    def test_package_profile_and_command_omissions_are_rejected(self) -> None:
        for key in ["runtime", "profile", "commands", "compiler_environment"]:
            changed = copy.deepcopy(self.receipt)
            changed[key] = [] if key == "commands" else {}
            with self.subTest(inventory=key), self.assertRaises(ValueError):
                verify_receipt(self.receipt_path, changed)

    def test_duplicate_json_keys_are_rejected(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            path = Path(temporary) / "duplicate.json"
            path.write_text('{"coverage":{},"coverage":{"matched_c_bytes":32}}')
            with self.assertRaisesRegex(ValueError, "Duplicate JSON key"):
                read_json(path)

    def test_intermediate_omission_cannot_be_self_consistent(self) -> None:
        for key in ["compiled", "generated"]:
            changed = copy.deepcopy(self.receipt)
            omitted = next(name for name in changed[key] if name.endswith(".s"))
            del changed[key][omitted]
            with self.subTest(inventory=key), self.assertRaisesRegex(ValueError, "inventory"):
                verify_receipt(self.receipt_path, changed, require_current=False)

    def test_coherent_contpfs_bss_reference_omission_is_rejected(self) -> None:
        key = "asm/data/units/resident/contpfs.bss.s"
        with tempfile.TemporaryDirectory(dir=PROJECT / "scratch") as temporary:
            directory, receipt = self.private_evidence_copy(Path(temporary))
            override = directory / "splat.override.yaml"
            self.assertEqual(yaml.safe_load(override.read_text()), splat_options(directory, CANONICAL_ROM))
            self.assertEqual(receipt["compiler_environment"], compiler_environment(directory, receipt["profile"]))
            self.assertEqual(receipt["compiler_environment"]["TMPDIR"], str(directory / "tmp"))
            self.assertEqual(receipt["artifacts"]["splat.override.yaml"], sha256(override))

            source = "src/units/resident/contpfs.c"
            self.assertEqual(receipt["graph"]["obj/source/src/units/resident/contpfs.o"], {
                "source": source, "input": f"source/{source}",
                "source_kind": "c", "image_id": "resident",
            })
            self.assertRegex((directory / "compiled" / Path(source).with_suffix(".s")).read_text(),
                             r"\.comm\s+D_800411D0,\s*256(?:,\s*2)?\b")
            reference = directory / "generated" / key
            self.assertEqual(sha256(reference), receipt["generated"][key])
            self.assertEqual(receipt["generated"][key], receipt["reproduction"]["split"]["generated"][key])

            frozen_source = input_manifest(directory / "source")
            object_hashes = {name: sha256(directory / name) for name in receipt["objects"]}
            compiled_hashes = file_inventory(directory / "compiled")
            elf_hash = sha256(directory / "shiren2.elf")
            rom_hash = sha256(directory / "shiren2.z64")
            self.assertEqual(frozen_source, receipt["input_manifest"])
            self.assertEqual(object_hashes, receipt["objects"])
            self.assertEqual(compiled_hashes, receipt["compiled"])
            self.assertEqual(elf_hash, receipt["artifacts"]["shiren2.elf"])
            self.assertEqual(rom_hash, receipt["artifacts"]["shiren2.z64"])
            reference.unlink()
            del receipt["generated"][key]
            del receipt["reproduction"]["split"]["generated"][key]
            write_json(directory / "receipt.json", receipt)

            self.assertFalse(reference.exists())
            self.assertEqual(receipt["generated"], file_inventory(directory / "generated"))
            self.assertEqual(receipt["generated"], receipt["reproduction"]["split"]["generated"])
            self.assertEqual(input_manifest(directory / "source"), frozen_source)
            self.assertEqual({name: sha256(directory / name) for name in receipt["objects"]}, object_hashes)
            self.assertEqual(file_inventory(directory / "compiled"), compiled_hashes)
            self.assertEqual(sha256(directory / "shiren2.elf"), elf_hash)
            self.assertEqual(sha256(directory / "shiren2.z64"), rom_hash)
            with self.assertRaisesRegex(ValueError, r"generated inputs inventory disagrees.*missing=.*contpfs\.bss\.s"):
                verify_receipt(directory / "receipt.json", require_current=False)

    def test_coherent_rodata_symbol_omission_is_rejected_by_split_replay(self) -> None:
        key = next((name for name in self.receipt["generated"] if name.startswith("asm/nonmatchings/")), None)
        if key is None:
            self.skipTest("Receipt has no C-owned rodata symbol reference")
        with tempfile.TemporaryDirectory(dir=PROJECT / "scratch") as temporary:
            directory, receipt = self.private_evidence_copy(Path(temporary))
            (directory / "generated" / key).unlink()
            del receipt["generated"][key]
            del receipt["reproduction"]["split"]["generated"][key]
            write_json(directory / "receipt.json", receipt)
            with self.assertRaisesRegex(ValueError, "Frozen YAML does not reproduce the complete generated split/graph"):
                verify_receipt(directory / "receipt.json", require_current=False)

    def test_live_tree_edit_blocks_freshness_but_not_frozen_trial(self) -> None:
        with tempfile.TemporaryDirectory(dir=PROJECT / "scratch") as temporary:
            live = Path(temporary) / "live"
            create_snapshot(PROJECT, live)
            source = live / "src/hardware_ai_length.c"
            source.write_text(source.read_text() + "\n/* later live edit */\n")
            with patch("verify.PROJECT", live):
                verify_receipt(self.receipt_path, require_current=False)
                with self.assertRaisesRegex(ValueError, "Input manifest mismatch"):
                    verify_receipt(self.receipt_path, require_current=True)

    def test_changed_object_bytes_fail_even_with_updated_receipt_hash(self) -> None:
        with tempfile.TemporaryDirectory(dir=PROJECT / "scratch") as temporary:
            directory, receipt = self.private_evidence_copy(Path(temporary))
            match = receipt["c_matches"][0]
            attribution = receipt["c_attributions"][0]
            path = directory / attribution["object"]
            _, offset = elf_code(path, attribution["object_symbol_offset"], match["size"], section_name=".text", allow_relocations=True)
            data = bytearray(path.read_bytes())
            data[offset] ^= 1
            path.write_bytes(data)
            receipt["objects"][attribution["object"]] = sha256(path)
            with self.assertRaisesRegex(ValueError, "C object instruction bytes disagree with fresh frozen-source assembly"):
                verify_receipt(directory / "receipt.json", receipt, require_current=False)

    def test_untracked_object_file_is_rejected(self) -> None:
        with tempfile.TemporaryDirectory(dir=PROJECT / "scratch") as temporary:
            directory, receipt = self.private_evidence_copy(Path(temporary))
            name = "obj/untracked-extra.o"
            self.assertNotIn(name, receipt["objects"])
            (directory / name).write_bytes(b"untracked fixture object\n")
            with self.assertRaisesRegex(ValueError, "Changed or omitted object evidence inventory"):
                verify_receipt(directory / "receipt.json", receipt, require_current=False)

    def test_changed_elf_c_bytes_fail_with_an_unchanged_rom(self) -> None:
        with tempfile.TemporaryDirectory(dir=PROJECT / "scratch") as temporary:
            directory, receipt = self.private_evidence_copy(Path(temporary))
            match = receipt["c_matches"][0]
            path = directory / "shiren2.elf"
            _, offset = elf_code(path, match["vram_start"], match["size"])
            data = bytearray(path.read_bytes())
            data[offset] ^= 1
            path.write_bytes(data)
            receipt["artifacts"]["shiren2.elf"] = sha256(path)
            with self.assertRaisesRegex(ValueError, "Actual initialized image bytes disagree with original storage"):
                verify_receipt(directory / "receipt.json", receipt, require_current=False)

    def test_changed_elf_outside_c_fails_full_extraction_proof(self) -> None:
        with tempfile.TemporaryDirectory(dir=PROJECT / "scratch") as temporary:
            directory, receipt = self.private_evidence_copy(Path(temporary))
            path = directory / "shiren2.elf"
            _, offset = elf_code(path, 0x80025C60, 4)
            data = bytearray(path.read_bytes())
            data[offset] ^= 1
            path.write_bytes(data)
            receipt["artifacts"]["shiren2.elf"] = sha256(path)
            with self.assertRaisesRegex(ValueError, "Actual initialized image bytes disagree with original storage"):
                verify_receipt(directory / "receipt.json", receipt, require_current=False)

    def test_forged_frozen_c_source_cannot_reuse_an_old_matching_object(self) -> None:
        with tempfile.TemporaryDirectory(dir=PROJECT / "scratch") as temporary:
            directory, receipt = self.private_evidence_copy(Path(temporary))
            source = directory / "source/src/hardware_ai_length.c"
            source.write_text(source.read_text().replace("0xA4500004", "0xA4500008"))
            manifest = input_manifest(directory / "source")
            path = directory / "input-manifest.json"
            path.write_text(json.dumps(manifest, indent=2) + "\n")
            receipt["input_manifest"] = manifest
            receipt["artifacts"]["input-manifest.json"] = sha256(path)
            with self.assertRaisesRegex(ValueError, "C compiler assembly does not reproduce from frozen source"):
                verify_receipt(directory / "receipt.json", receipt, require_current=False)

    def test_rewritten_split_placement_cannot_reuse_old_generated_inputs(self) -> None:
        with tempfile.TemporaryDirectory(dir=PROJECT / "scratch") as temporary:
            directory, receipt = self.private_evidence_copy(Path(temporary))
            split = directory / "source/config/shiren2.jp.yaml"
            split.write_text(split.read_text().replace("[0x1400, c, hardware_ai_next_buffer]", "[0x1404, c, hardware_ai_next_buffer]"))
            manifest = input_manifest(directory / "source")
            path = directory / "input-manifest.json"
            path.write_text(json.dumps(manifest, indent=2) + "\n")
            receipt["input_manifest"] = manifest
            receipt["artifacts"]["input-manifest.json"] = sha256(path)
            with self.assertRaisesRegex(ValueError, "Frozen YAML does not reproduce"):
                verify_receipt(directory / "receipt.json", receipt, require_current=False)

    def test_coherent_yaml_bss_error_fails_original_loader_extent_authority(self) -> None:
        with tempfile.TemporaryDirectory(dir=PROJECT / "scratch") as temporary:
            directory = Path(temporary) / "bss-run"
            directory.mkdir()
            create_snapshot(self.receipt_path.parent / "source", directory / "source")
            split = directory / "source/config/shiren2.jp.yaml"
            # Shrinking remains linkable beside fixed overlay B storage, so this
            # fixture reaches the independent original-loader extent check.
            original = split.read_text()
            self.assertEqual(original.count("bss_size: 0x844F0"), 1)
            split.write_text(original.replace("bss_size: 0x844F0", "bss_size: 0x844E0"))
            write_json(directory / "input-manifest.json", input_manifest(directory / "source"))
            with self.assertRaisesRegex(ValueError, "Actual linked BSS disagrees with reviewed loader extent"):
                execute_snapshot(directory, PROJECT, CANONICAL_ROM)

    def test_relocation_bytes_cannot_be_changed_behind_an_updated_object_hash(self) -> None:
        attribution = next(item for item in self.receipt["c_attributions"] if item["text_relocations"])
        with tempfile.TemporaryDirectory(dir=PROJECT / "scratch") as temporary:
            directory, receipt = self.private_evidence_copy(Path(temporary))
            path = directory / attribution["object"]
            data = bytearray(path.read_bytes())
            elf = Elf32File(data)
            section = next(section for section in elf.sectionHeaders if elf.shstrtab[section.name] == ".rel.text")
            data[section.offset + 7] ^= 1
            path.write_bytes(data)
            receipt["objects"][attribution["object"]] = sha256(path)
            with self.assertRaisesRegex(ValueError, "C object instruction bytes disagree with fresh frozen-source assembly"):
                verify_receipt(directory / "receipt.json", receipt, require_current=False)

    def test_rewritten_map_with_an_updated_artifact_hash_fails_linker_replay(self) -> None:
        with tempfile.TemporaryDirectory(dir=PROJECT / "scratch") as temporary:
            directory, receipt = self.private_evidence_copy(Path(temporary))
            path = directory / "shiren2.map"
            path.write_text(path.read_text() + "\nforged map trailer\n")
            receipt["artifacts"]["shiren2.map"] = sha256(path)
            # A coherent forgery also updates the storage record's map identity;
            # fresh linking must reject it independently of that earlier guard.
            original = CANONICAL_ROM.read_bytes()
            inventory, _ = image_evidence(directory / "source", original)
            receipt["image_inventory"]["linked_images"] = linked_image_records(
                directory, inventory, original, graph=build_graph(directory)
            )
            with self.assertRaisesRegex(ValueError, "Independent full linker replay disagrees: shiren2.map"):
                verify_receipt(directory / "receipt.json", receipt, require_current=False)

    def test_rewritten_map_requires_matching_linked_storage_evidence(self) -> None:
        images = read_json(self.receipt_path.parent / "source/config/images.json")
        if not any(image["image_id"] == "overlay_136dc0" and image["split_status"] == "active"
                   for image in images["images"]):
            self.skipTest("The selected receipt predates overlay B storage evidence")
        with tempfile.TemporaryDirectory(dir=PROJECT / "scratch") as temporary:
            directory, receipt = self.private_evidence_copy(Path(temporary))
            path = directory / "shiren2.map"
            path.write_text(path.read_text() + "\nforged map trailer\n")
            receipt["artifacts"]["shiren2.map"] = sha256(path)
            with self.assertRaisesRegex(ValueError, "Original image inventory disagrees"):
                verify_receipt(directory / "receipt.json", receipt, require_current=False)

    def test_rewritten_dependency_list_fails_independent_compiler_check(self) -> None:
        with tempfile.TemporaryDirectory(dir=PROJECT / "scratch") as temporary:
            directory, receipt = self.private_evidence_copy(Path(temporary))
            source = next(source for source, paths in receipt["dependencies"].items() if len(paths) > 1)
            relative = Path(source).with_suffix(".d").as_posix()
            dependency = directory / "compiled" / relative
            dependency.write_text(f"candidate.o: source/{source}\n")
            receipt["compiled"][relative] = sha256(dependency)
            receipt["dependencies"][source] = [source]
            with self.assertRaisesRegex(ValueError, "dependency closure disagrees"):
                verify_receipt(directory / "receipt.json", receipt, require_current=False)

    def test_wrong_target_receipt_is_rejected(self) -> None:
        changed = copy.deepcopy(self.receipt)
        changed["target_sha256"] = "0" * 64
        with self.assertRaisesRegex(ValueError, "expected target"):
            verify_receipt(self.receipt_path, changed)

    def test_false_c_coverage_is_rejected(self) -> None:
        changed = copy.deepcopy(self.receipt)
        changed["coverage"]["matched_c_bytes"] = 0x2000000
        with self.assertRaisesRegex(ValueError, "C coverage disagrees"):
            verify_receipt(self.receipt_path, changed, require_current=False)

    def test_existing_build_directory_is_not_reused(self) -> None:
        with tempfile.TemporaryDirectory(prefix="existing-", dir=PROJECT / "build") as temporary:
            with self.assertRaises(FileExistsError):
                build(Path(temporary).name)


if __name__ == "__main__":
    unittest.main()
