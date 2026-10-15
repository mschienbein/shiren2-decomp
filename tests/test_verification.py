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
from certification import REQUIRED_ARTIFACTS, REQUIRED_RECEIPT_FIELDS, build_graph, compiler_environment, elf_code, file_inventory, image_evidence, intermediate_keys, linked_image_records, object_commands, read_json, retained_trial, sha256, source_policy_violations, splat_bindings, splat_options, tool_profile
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
            for name in ("source", "generated", "compiled", "obj", "trusted-orchestration"):
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

    def test_kmc_range_profile_replaces_only_the_assembler(self) -> None:
        profiles = read_json(PROJECT / "config/compiler_profiles.json")["profiles"]
        gnu = profiles["gcc272-gnu291-mips2-unsigned"]
        kmc = profiles["gcc272-kmc26-mips2-unsigned"]
        self.assertEqual(kmc["compiler"], gnu["compiler"])
        self.assertEqual(kmc["compiler_flags"], gnu["compiler_flags"])
        self.assertEqual(kmc["assembler"], "as_kmc26")
        self.assertEqual(kmc["assembler_flags"], ["-mips2", "-EB", "-G", "0", "-I", "source/include"])
        self.assertNotIn("language", kmc)

    def test_addu_range_units_bind_only_range_profiles(self) -> None:
        declaration = read_json(PROJECT / "config/compiler_profiles.json")
        pointer = read_json(PROJECT / "docs/progress.json")["denominator"]
        catalogue = read_json(PROJECT / pointer["path"])["identity"]["functions"]
        inside = {item["symbol"] for item in catalogue
                  if item["image_id"] == "main_14400" and 0x80132050 <= item["vram_start"] < 0x80135FB0}
        self.assertEqual(len(inside), 14)
        range_profiles = {"gcc272-kmc26-mips2-unsigned", "gcc272-gnu291-mips2-unsigned"}
        matches = read_json(PROJECT / "config/matches.json")["functions"]
        sources = {item["source"] for item in matches if item["symbol"] in inside}
        bound = {source for source, profile_id in declaration["tu_profiles"].items() if profile_id in range_profiles}
        self.assertEqual(bound, sources)
        self.assertTrue(all(item["symbol"] in inside for item in matches if item["source"] in bound))

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


class OverlayLinkageTests(unittest.TestCase):
    """Synthetic YAML/ELF/linker-script tests prove validator behavior, never real linkage or ROM certification."""
    overlays = (
        ("overlay_1339f0", 0x1339F0, 0x136DC0, 0x801E8270, 0x801E8700, (0x136380, 0x136A40)),
        ("overlay_136dc0", 0x136DC0, 0x148460, 0x801F6540, 0x801F7200, (0x147030, 0x147C50)),
    )

    def images(self) -> dict[str, SimpleNamespace]:
        return {name: SimpleNamespace(image_id=name, split_status="active", rom_start=start, rom_end=end,
                    vram_start=0x801E4EA0, vram_end=0x801E4EA0 + end - start, vram_rom_delta=0x801E4EA0 - start,
                    load_slot="overlay_801e4ea0", splat_segments=(name,), bss_ranges=(SimpleNamespace(vram_start=bss_start, vram_end=bss_end),))
                for name, start, end, bss_start, bss_end, _ in self.overlays}

    def segments(self) -> list:
        result: list = []
        for name, start, end, bss_start, bss_end, (data, rodata) in self.overlays:
            result.append({"name": name, "type": "code", "start": start, "vram": 0x801E4EA0,
                "exclusive_ram_id": "overlay_801e4ea0", "symbol_name_format": "$VRAM_$ROM", "subalign": 1,
                "bss_size": bss_end - bss_start, "subsegments": [[start, "asm", f"{name}/text"], [data, "data", f"{name}/data"],
                [rodata, "rodata", f"{name}/rodata"], {"type": "bss", "vram": bss_start, "name": f"{name}/bss"}]})
        return result + [{"name": "remaining", "type": "bin", "start": 0x148460, "vram": 0x148460, "exclusive_ram_id": "rom_tail_storage"}, [0x2000000]]

    def write_config(self, root: Path, segments: list | None = None) -> SimpleNamespace:
        (root/"config").mkdir(parents=True, exist_ok=True)
        (root/"config/shiren2.jp.yaml").write_text(yaml.safe_dump({"segments": self.segments() if segments is None else segments}))
        return SimpleNamespace(images=self.images(), source_bindings={}, components=(), evidence={})

    @staticmethod
    def elf_bytes(sections: list[tuple[str, int, int, int, bytes | int]]) -> bytes:
        """ELF32 big-endian MIPS with (name, type, flags, address, bytes-or-NOBITS-size) sections."""
        names = [""] + [section[0] for section in sections] + [".shstrtab"]
        shstrings = b""; offsets = {}
        for name in names:
            offsets[name] = len(shstrings); shstrings += name.encode() + b"\0"
        data = bytearray(0x100); headers = [bytes(40)]
        for name, kind, flags, address, body in sections:
            offset = len(data)
            if isinstance(body, int):
                size = body
            else:
                data.extend(body); size = len(body)
            headers.append(struct.pack(">IIIIIIIIII", offsets[name], kind, flags, address, offset, size, 0, 0, 4, 0))
        string_offset = len(data); data.extend(shstrings)
        headers.append(struct.pack(">IIIIIIIIII", offsets[".shstrtab"], 3, 0, 0, string_offset, len(shstrings), 0, 0, 1, 0))
        data.extend(bytes((-len(data)) % 4)); header_offset = len(data)
        for header in headers:
            data.extend(header)
        data[:52] = b"\x7fELF\x01\x02\x01" + bytes(9) + struct.pack(">HHIIIIIHHHHHH", 2, 8, 1, 0x80025C00, 0, header_offset, 0, 52, 32, 0, 40, len(headers), len(headers) - 1)
        return bytes(data)

    def linked_fixture(self, root: Path, *, bss_sizes: dict[str, int] | None = None, addresses: dict[str, int] | None = None, omit: str | None = None) -> tuple[SimpleNamespace, bytes]:
        reference = bytearray(bytes(range(256)) * (0x148460 // 256 + 1))[:0x148460]
        sections = []
        for name, start, end, bss_start, bss_end, _ in self.overlays:
            sections.append((f".{name}", 1, 7, (addresses or {}).get(name, 0x801E4EA0), bytes(reference[start:end])))
            if omit != name:
                sections.append((f".{name}_bss", 8, 3, bss_start, (bss_sizes or {}).get(name, bss_end - bss_start)))
        (root/"shiren2.elf").write_bytes(self.elf_bytes(sections))
        return self.write_config(root/"source"), bytes(reference)

    def test_two_overlay_parents_and_the_tail_bin_bind_their_sections(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary); inventory = self.write_config(root)
            sections, sources = splat_bindings(root, inventory)
            self.assertEqual(sections, {"overlay_1339f0": "overlay_1339f0", "overlay_1339f0_bss": "overlay_1339f0",
                "overlay_136dc0": "overlay_136dc0", "overlay_136dc0_bss": "overlay_136dc0", "remaining": "rom_storage:remaining"})
            self.assertEqual(sources, {})

    def test_overlay_parent_may_lower_its_section_end_alignment(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary); inventory = self.write_config(root)
            for align in (1, 4, 16):
                segments = self.segments(); segments[0]["align"] = align
                self.write_config(root, segments)
                with self.subTest(align=align):
                    self.assertIn("overlay_1339f0", splat_bindings(root, inventory)[0])

    def test_unsupported_overlay_parent_shapes_are_rejected(self) -> None:
        mutations = [
            ("name", "overlay_a"), ("symbol_name_format", "$VRAM"), ("symbol_name_format", None),
            ("exclusive_ram_id", "overlay_136dc0"), ("exclusive_ram_id", None), ("subalign", 4), ("subalign", None),
            ("bss_size", 0x480), ("bss_size", None), ("vram", 0x801E4EA4), ("start", 4), ("bss_contains_common", True),
            ("align", 3), ("align", 32), ("align", "4"), ("align", True),
        ]
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary); inventory = self.write_config(root)
            for index in (0, 1):
                for field, value in mutations:
                    segments = self.segments(); segment = segments[index]
                    if value is None:
                        segment.pop(field)
                    elif field == "start":
                        segment["start"] += value; segment["subsegments"][0][0] += value
                    else:
                        segment[field] = value
                    self.write_config(root, segments)
                    with self.subTest(index=index, field=field, value=value), self.assertRaises(ValueError):
                        splat_bindings(root, inventory)

    def test_tail_bin_needs_its_own_exclusive_id_and_exact_interval(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary); inventory = self.write_config(root)
            for mutation in ("no_id", "shared_id", "ram", "end", "old_opaque_a"):
                segments = self.segments(); tail = segments[2]
                if mutation == "no_id":
                    tail.pop("exclusive_ram_id")
                elif mutation == "shared_id":
                    tail["exclusive_ram_id"] = "overlay_801e4ea0"
                elif mutation == "ram":
                    tail["vram"] = 0x801E4EA0
                elif mutation == "end":
                    segments[3] = [0x1FFFFF0]
                else:
                    segments.insert(0, {"name": "overlay_1339f0_opaque", "type": "bin", "start": 0x1339F0, "vram": 0x1339F0})
                self.write_config(root, segments)
                with self.subTest(mutation=mutation), self.assertRaises(ValueError):
                    splat_bindings(root, inventory)

    def test_duplicate_output_section_owner_is_rejected(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary); inventory = self.write_config(root)
            segments = self.segments(); segments.insert(0, [0, "bin", "overlay_136dc0"])
            self.write_config(root, segments)
            with self.assertRaisesRegex(ValueError, "Duplicate splat"):
                splat_bindings(root, inventory)

    def test_linked_records_bind_both_overlays_and_their_reviewed_bss(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary); inventory, reference = self.linked_fixture(root)
            result = linked_image_records(root, inventory, reference)
            for name, start, end, bss_start, bss_end, _ in self.overlays:
                with self.subTest(image=name):
                    self.assertEqual([(item["rom_start"], item["rom_end"], item["vram_start"]) for item in result[name]["initialized"]], [(start, end, 0x801E4EA0)])
                    self.assertEqual(result[name]["bss"], [{"section": f".{name}_bss", "vram_start": bss_start, "vram_end": bss_end}])

    def test_linked_bss_bytes_and_placement_must_match_loader_authority(self) -> None:
        cases = [
            ({"bss_sizes": {"overlay_136dc0": 0xCB0}}, "Actual linked BSS disagrees"),
            ({"omit": "overlay_1339f0"}, "Actual linked BSS disagrees"),
            ({"addresses": {"overlay_136dc0": 0x801E4EB0}}, "Actual initialized ELF section disagrees"),
        ]
        for options, message in cases:
            with self.subTest(options=options), tempfile.TemporaryDirectory() as temporary:
                root = Path(temporary); inventory, reference = self.linked_fixture(root, **options)
                with self.assertRaisesRegex(ValueError, message):
                    linked_image_records(root, inventory, reference)
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary); inventory, reference = self.linked_fixture(root)
            changed = bytearray(reference); changed[0x140000] ^= 1
            with self.assertRaisesRegex(ValueError, "Actual initialized image bytes disagree"):
                linked_image_records(root, inventory, bytes(changed))

    def write_graph_fixture(self, root: Path) -> SimpleNamespace:
        inventory = self.write_config(root/"source")
        (root/"source/config/rom_map.json").write_text(json.dumps({"ranges": [
            [0x148460, 0x148470, "asset_yay0", "archive"],
            [0x148470, 0x2000000, "rom_padding_zero", "boot"]]}))
        blocks = []
        for name, *_ in self.overlays:
            inputs = [f"obj/generated/asm/{name}/text.o(.text)", f"obj/generated/asm/data/{name}/data.data.o(.data)", f"obj/generated/asm/data/{name}/rodata.rodata.o(.rodata)"]
            blocks.append(f"    .{name} 0x801E4EA0 : AT({name}_ROM_START) SUBALIGN(1)\n    {{\n" + "".join(f"        {item};\n" for item in inputs) + "    }\n")
            blocks.append(f"    .{name}_bss (NOLOAD) : SUBALIGN(1)\n    {{\n        obj/generated/asm/data/{name}/bss.bss.o(.bss);\n    }}\n")
            for source in (f"generated/asm/{name}/text.s", f"generated/asm/data/{name}/data.data.s", f"generated/asm/data/{name}/rodata.rodata.s", f"generated/asm/data/{name}/bss.bss.s"):
                path = root/source; path.parent.mkdir(parents=True, exist_ok=True); path.write_text("/* synthetic asm */\n")
        path = root/"generated/assets/remaining.bin"; path.parent.mkdir(parents=True, exist_ok=True); path.write_bytes(b"synthetic bin")
        blocks.append("    .remaining 0x148460 : AT(remaining_ROM_START)\n    {\n        obj/generated/assets/remaining.o(.data);\n    }\n")
        (root/"shiren2.ld").write_text("SECTIONS\n{\n" + "".join(blocks) + "}\n")
        return inventory

    def test_generated_graph_assigns_overlay_objects_to_their_own_images(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary); inventory = self.write_graph_fixture(root)
            with patch("certification.load_inventory", return_value=inventory):
                graph = build_graph(root)
                for name, *_ in self.overlays:
                    with self.subTest(image=name):
                        self.assertEqual(graph[f"obj/generated/asm/{name}/text.o"], {"source": f"generated/asm/{name}/text.s", "input": f"generated/asm/{name}/text.s", "source_kind": "asm", "image_id": name})
                        self.assertEqual(graph[f"obj/generated/asm/data/{name}/bss.bss.o"]["image_id"], name)
                self.assertEqual(graph["obj/generated/assets/remaining.o"]["image_id"], "rom_storage:remaining")
                # A same-VRAM object cannot be claimed by both overlay sections.
                (root/"shiren2.ld").write_text((root/"shiren2.ld").read_text().replace("obj/generated/asm/overlay_1339f0/text.o", "obj/generated/asm/overlay_136dc0/text.o"))
                with self.assertRaisesRegex(ValueError, "no unique original image/storage owner"):
                    build_graph(root)

    def test_intermediate_inventory_lists_overlay_asm_inputs(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary); inventory = self.write_graph_fixture(root)
            with patch("certification.load_inventory", return_value=inventory):
                graph = build_graph(root)
            generated, compiled = intermediate_keys(graph, [], root/"source")
            self.assertEqual(compiled, set())
            self.assertIn("asm/overlay_136dc0/text.s", generated)
            self.assertIn("asm/data/overlay_1339f0/bss.bss.s", generated)
            self.assertIn("assets/remaining.bin", generated)
            self.assertIn("assets/tail/manifest.json", generated)
            self.assertIn("assets/tail/yay0/00148460.bin", generated)

    def test_shape_valid_wrong_digest_fails_original_byte_seam(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary); inventory, reference = self.linked_fixture(root)
            inventory.images["overlay_1339f0"].rom_sha256 = hashlib.sha256(reference[0x1339F0:0x136DC0]).hexdigest()
            inventory.images["overlay_136dc0"].rom_sha256 = "0" * 64
            (root/"config").mkdir(); (root/"config/images.json").write_text("{}")
            # Only target identity/parser setup are mocked; the actual interval digest check runs.
            with patch("certification.attest"), patch("certification.load_inventory", return_value=inventory), self.assertRaisesRegex(ValueError, "Original image digest"):
                image_evidence(root, reference)



class RetainedTrialTests(unittest.TestCase):
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


class EditableSourceRecipeTests(unittest.TestCase):
    """ROM import and a substituted packer cannot masquerade as frozen-source use."""

    def setUp(self) -> None:
        import certification
        self.certification = certification
        temporary = tempfile.TemporaryDirectory()
        self.addCleanup(temporary.cleanup)
        self.source = Path(temporary.name) / "source"
        (self.source / "tools").mkdir(parents=True)
        for name in ("assets.py", "rommap.py", "rspaudio.py", "assetformat.py", "mipsflow.py", "gxxvtable.py", "rom.py", "workspace.py", "toolchain_library.py",
                     "images.py", "evidence.py", "packet.py", "progress.py", "certification.py", "generated_symbols.py"):
            shutil.copy2(Path(certification.__file__).parent / name, self.source / "tools" / name)
        (self.source / "config").mkdir()
        for relative in (certification.rommap.CATALOGUE_PATH, certification.rommap.HANDWRITTEN_PATH):
            shutil.copy2(Path(certification.__file__).parent.parent / relative, self.source / relative)
        self.certification.validate_asset_recipe(self.source, self.certification.split_commands())

    def test_snapshot_cannot_be_its_own_trusted_verifier(self) -> None:
        with patch.object(self.certification, "__file__", str(self.source / "tools/certification.py")):
            with self.assertRaisesRegex(ValueError, "outside the snapshot"):
                self.certification.validate_asset_recipe(self.source, self.certification.split_commands())

    def test_current_catalogue_advance_does_not_reject_frozen_recipe(self) -> None:
        trusted = self.source.parent / "trusted"
        shutil.copytree(self.source / "tools", trusted / "tools")
        (trusted / "config").mkdir()
        (trusted / self.certification.rommap.CATALOGUE_PATH).write_text('{"different": "current denominator"}')
        with patch.object(self.certification, "__file__", str(trusted / "tools/certification.py")):
            self.certification.validate_asset_recipe(self.source, self.certification.split_commands())

    def test_unrelated_trusted_helpers_do_not_invalidate_historical_snapshot(self) -> None:
        trusted = self.source.parent / "trusted"
        shutil.copytree(self.source / "tools", trusted / "tools")
        for name in ("packet.py", "progress.py", "certification.py", "workspace.py"):
            path = trusted / "tools" / name
            path.write_bytes(path.read_bytes() + b"\n# unrelated verifier-side evolution\n")
        with patch.object(self.certification, "__file__", str(trusted / "tools/certification.py")):
            self.certification.validate_asset_recipe(self.source, self.certification.split_commands())

    def test_every_snapshot_packer_module_is_bound(self) -> None:
        for name in ("assets.py", "rommap.py", "rspaudio.py", "assetformat.py", "mipsflow.py", "gxxvtable.py", "images.py", "evidence.py", "rom.py", "toolchain_library.py", "generated_symbols.py"):
            path = self.source / "tools" / name
            before = path.read_bytes()
            path.write_bytes(before + b"\n# substituted executed module\n")
            try:
                with self.subTest(name=name), self.assertRaisesRegex(ValueError, "differs from the independent verifier"):
                    self.certification.validate_asset_recipe(self.source, self.certification.split_commands())
            finally:
                path.write_bytes(before)

    def test_import_shadow_packages_and_bytecode_are_refused(self) -> None:
        for relative in ("yaml.py", "rom/__init__.py", "__pycache__/rom.cpython-312.pyc",
                         "rom.pyc", "rom.cpython-312-darwin.so"):
            path = self.source / "tools" / relative
            path.parent.mkdir(parents=True, exist_ok=True)
            path.write_bytes(b"unreviewed import payload")
            try:
                with self.subTest(path=relative), self.assertRaisesRegex(ValueError, "namespace|alternate loaders"):
                    self.certification.validate_asset_recipe(self.source, self.certification.split_commands())
            finally:
                path.unlink()
                if path.parent != self.source / "tools":
                    path.parent.rmdir()

    def test_real_and_preflight_child_share_the_gate_before_split(self) -> None:
        import build
        trusted = Path(build.__file__).resolve().parent.parent
        for preflight_only in (False, True):
            with self.subTest(preflight_only=preflight_only), \
                    patch.object(build, "validate_asset_recipe", side_effect=ValueError("gate stopped execution")) as gate, \
                    patch.object(build.subprocess, "run") as run:
                with self.assertRaisesRegex(ValueError, "gate stopped execution"):
                    build.execute_snapshot(self.source.parent, trusted, self.source.parent / "absent.z64",
                                           preflight_only=preflight_only)
                gate.assert_called_once_with(self.source, self.certification.split_commands())
                run.assert_not_called()

    def test_changed_or_missing_assetformat_stops_the_child_before_split(self) -> None:
        import build
        trusted = Path(build.__file__).resolve().parent.parent
        path = self.source / "tools/assetformat.py"
        before = path.read_bytes()
        cases = (("changed", lambda: path.write_bytes(before + b"\n# substituted format witness\n"),
                  ValueError, "differs from the independent verifier: assetformat.py"),
                 ("missing", path.unlink, FileNotFoundError, "assetformat.py"))
        for name, mutate, error, message in cases:
            mutate()
            try:
                for preflight_only in (False, True):
                    with self.subTest(name, preflight_only=preflight_only), patch.object(build.subprocess, "run") as run:
                        with self.assertRaisesRegex(error, message):
                            build.execute_snapshot(self.source.parent, trusted, self.source.parent / "absent.z64",
                                                   preflight_only=preflight_only)
                        run.assert_not_called()
            finally:
                path.write_bytes(before)

    def test_changed_or_missing_mipsflow_stops_the_child_before_split(self) -> None:
        import build
        trusted = Path(build.__file__).resolve().parent.parent
        path = self.source / "tools/mipsflow.py"
        before = path.read_bytes()
        cases = (("changed", lambda: path.write_bytes(before + b"\n# substituted flow engine\n"),
                  ValueError, "differs from the independent verifier: mipsflow.py"),
                 ("missing", path.unlink, FileNotFoundError, "mipsflow.py"))
        for name, mutate, error, message in cases:
            mutate()
            try:
                for preflight_only in (False, True):
                    with self.subTest(name, preflight_only=preflight_only), patch.object(build.subprocess, "run") as run:
                        with self.assertRaisesRegex(error, message):
                            build.execute_snapshot(self.source.parent, trusted, self.source.parent / "absent.z64",
                                                   preflight_only=preflight_only)
                        run.assert_not_called()
            finally:
                path.write_bytes(before)

    def test_changed_or_missing_gxxvtable_stops_the_child_before_split(self) -> None:
        import build
        trusted = Path(build.__file__).resolve().parent.parent
        path = self.source / "tools/gxxvtable.py"
        before = path.read_bytes()
        cases = (("changed", lambda: path.write_bytes(before + b"\n# substituted vtable witness\n"),
                  ValueError, "differs from the independent verifier: gxxvtable.py"),
                 ("missing", path.unlink, FileNotFoundError, "gxxvtable.py"))
        for name, mutate, error, message in cases:
            mutate()
            try:
                for preflight_only in (False, True):
                    with self.subTest(name, preflight_only=preflight_only), patch.object(build.subprocess, "run") as run:
                        with self.assertRaisesRegex(error, message):
                            build.execute_snapshot(self.source.parent, trusted, self.source.parent / "absent.z64",
                                                   preflight_only=preflight_only)
                        run.assert_not_called()
            finally:
                path.write_bytes(before)

    def test_orchestration_archive_is_inert_and_bound(self) -> None:
        trusted = Path(self.certification.__file__).resolve().parent
        record = self.certification.archive_orchestration(self.source.parent, trusted / "build.py")
        archive = self.source.parent / "trusted-orchestration"
        self.assertEqual(record["entrypoint_sha256"], self.certification.sha256(trusted / "build.py"))
        with patch.object(self.certification, "__file__", "/unrelated/current/tools/certification.py"):
            self.certification.validate_orchestration(self.source.parent, record)
        path = archive / "build.py"
        path.write_bytes(path.read_bytes() + b"\n# archive changed\n")
        with self.assertRaisesRegex(ValueError, "Changed trusted orchestration"):
            self.certification.validate_orchestration(self.source.parent, record)

    def test_coherent_orchestration_rewrite_cannot_authorize_snapshot_tools(self) -> None:
        trusted = Path(self.certification.__file__).resolve().parent
        record = self.certification.archive_orchestration(self.source.parent, trusted / "build.py")
        archive = self.source.parent / "trusted-orchestration"
        payload = b"raise AssertionError('archive must never execute')\n"
        for name in ("build.py", "assets.py"):
            (archive / name).write_bytes(payload)
            record["source_files"][name] = self.certification.sha256(archive / name)
        record["entrypoint"] = str(archive / "build.py")
        record["entrypoint_sha256"] = record["source_files"]["build.py"]
        record["child_argv"] = ["python", str(archive / "build.py")]
        # Coherent declarations pass integrity only. They neither execute nor
        # choose the independent verifier, even when they name the archive.
        self.certification.validate_orchestration(self.source.parent, record)
        (self.source / "tools/assets.py").write_bytes(payload)
        with self.assertRaisesRegex(ValueError, "differs from the independent verifier"):
            self.certification.validate_asset_recipe(self.source, self.certification.split_commands())

    def test_extract_in_build_and_replay_is_rejected(self) -> None:
        recipe = self.certification.split_commands()
        recipe[1] = [recipe[1][0], "source/tools/assets.py", "extract", "--root", "source"]
        with self.assertRaises(ValueError):
            self.certification.validate_asset_recipe(self.source, recipe)

    def test_custom_canonical_decoder_is_rejected(self) -> None:
        recipe = self.certification.split_commands()
        recipe[1] = [recipe[1][0], "-c",
                     "import crunch64; crunch64.yay0.decompress(open('baserom.jp.z64','rb').read())"]
        with self.assertRaises(ValueError):
            self.certification.validate_asset_recipe(self.source, recipe)

    def test_pack_argv_cannot_hide_a_rom_decoding_implementation(self) -> None:
        path = self.source / "tools/assets.py"
        path.write_text("import crunch64\ncrunch64.yay0.decompress(open('baserom.jp.z64','rb').read())\n")
        with self.assertRaises(ValueError):
            self.certification.validate_asset_recipe(self.source, self.certification.split_commands())

    def test_extra_import_after_pack_is_rejected(self) -> None:
        recipe = self.certification.split_commands()
        recipe.append([recipe[1][0], "source/tools/assets.py", "extract", "--root", "source"])
        with self.assertRaises(ValueError):
            self.certification.validate_asset_recipe(self.source, recipe)


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
        write_json(directory / "receipt.json", receipt)
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
            # Shrinking remains linkable below the overlay slot, so this fixture
            # reaches the independent original-loader extent check.
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
            # The map is not part of the linked-image inventory; fresh linking alone rejects it.
            with self.assertRaisesRegex(ValueError, "Independent full linker replay disagrees: shiren2.map"):
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
