"""Invocation-local ELF parsing must preserve attribution and failure evidence."""
from __future__ import annotations

import json
from pathlib import Path
import shutil
import struct
import subprocess
import sys
import tempfile
import unittest
from unittest.mock import patch

TOOLS = Path(__file__).resolve().parents[1] / "tools"
sys.path.insert(0, str(TOOLS))
from certification import _LinkedElfMemo, _elf_code, c_attributions, elf_code, reproduce_c_objects_and_link
from spimdisasm.elf32 import Elf32File
from workspace import REQUIRED_FILES, input_manifest


class LinkedElfMemoTests(unittest.TestCase):
    def image(self, *, executable: bool = True, relocation: int = 0, address: int = 0x1000) -> bytes:
        names = b"\0.text\0.shstrtab\0.rel.text\0"
        body = b"\x01\x02\x03\x04\x05\x06\x07\x08"
        data = bytearray(64)
        data.extend(body)
        names_offset = len(data)
        data.extend(names)
        data.extend(bytes(-len(data) % 4))
        relocation_offset = len(data)
        data.extend(struct.pack(">II", relocation, 4))
        shoff = len(data)
        headers = [bytes(40),
                   struct.pack(">10I", 1, 1, 6 if executable else 2, address, 64, 8, 0, 0, 4, 0),
                   struct.pack(">10I", 7, 3, 0, 0, names_offset, len(names), 0, 0, 1, 0),
                   struct.pack(">10I", 17, 9, 0, 0, relocation_offset, 8, 0, 1, 4, 8)]
        for header in headers:
            data.extend(header)
        data[:52] = b"\x7fELF\x01\x02\x01" + bytes(9) + struct.pack(">HHIIIIIHHHHHH", 1, 8, 1, 0, 0, shoff, 0, 52, 0, 0, 40, 4, 2)
        return bytes(data)

    def test_direct_comparison_reuses_equal_bytes_without_hashing(self):
        class UnhashableBytes(bytes):
            def __hash__(self) -> int:
                raise AssertionError("Linked ELF memo must not hash its bytes")

        parse = _LinkedElfMemo()
        data = UnhashableBytes(self.image())
        equal = UnhashableBytes(self.image())
        self.assertIsNot(data, equal)
        with patch("certification.Elf32File", wraps=Elf32File) as constructor:
            first = parse(data)
            self.assertIs(parse(equal), first)
            self.assertEqual(constructor.call_count, 1)
            changed = self.image(relocation=4)
            self.assertIsNot(parse(changed), first)
            self.assertIsNot(parse(data), first)
            self.assertEqual(constructor.call_count, 3)
            self.assertIsNot(_LinkedElfMemo()(data), first)
            self.assertEqual(constructor.call_count, 4)

    def test_cached_parse_still_checks_each_range_section_and_relocation(self):
        parse = _LinkedElfMemo()
        data = self.image()
        self.assertEqual(_elf_code(data, 0x1004, 4, parse=parse), (b"\x05\x06\x07\x08", 68))
        self.assertEqual(_elf_code(data, 0x1000, 4, allow_relocations=True, parse=parse)[0], b"\x01\x02\x03\x04")
        with self.assertRaisesRegex(ValueError, "Relocation-aware C attribution is required"):
            _elf_code(data, 0x1000, 4, parse=parse)
        with self.assertRaisesRegex(ValueError, "no unique executable ELF section"):
            _elf_code(data, 0x1004, 8, parse=parse)
        with self.assertRaisesRegex(ValueError, "no unique executable ELF section"):
            _elf_code(data, 0x1004, 4, section_name=".data", parse=parse)

    def test_same_size_changed_bytes_do_not_reuse_executable_or_relocation_state(self):
        parse = _LinkedElfMemo()
        data = self.image()
        self.assertEqual(_elf_code(data, 0x1004, 4, parse=parse)[0], b"\x05\x06\x07\x08")
        no_execute = self.image(executable=False)
        relocated = self.image(relocation=4)
        self.assertEqual(len(data), len(no_execute))
        self.assertEqual(len(data), len(relocated))
        with self.assertRaisesRegex(ValueError, "no unique executable ELF section"):
            _elf_code(no_execute, 0x1004, 4, parse=parse)
        with self.assertRaisesRegex(ValueError, "Relocation-aware C attribution is required"):
            _elf_code(relocated, 0x1004, 4, parse=parse)
        changed = bytearray(data)
        changed[68:72] = b"FAIL"
        self.assertEqual(_elf_code(bytes(changed), 0x1004, 4, parse=parse)[0], b"FAIL")

    def test_cached_parse_still_checks_truncated_code(self):
        data = bytearray(self.image())
        shoff = struct.unpack_from(">I", data, 32)[0]
        struct.pack_into(">I", data, shoff + 40 + 16, len(data) - 4)
        data = bytes(data)
        parse = _LinkedElfMemo()
        _elf_code(data, 0x1000, 4, allow_relocations=True, parse=parse)
        with self.assertRaisesRegex(ValueError, "Truncated ELF code evidence"):
            _elf_code(data, 0x1004, 4, parse=parse)

    def test_attributions_reread_same_path_and_do_not_share_receipt_cache(self):
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            (root / "source").mkdir()
            (root / "source/main.c").write_text("fixture\n")
            reference = b"\x01\x02\x03\x04\x05\x06\x07\x08"
            (root / "shiren2.z64").write_bytes(reference)
            linked_path = root / "shiren2.elf"
            linked_path.write_bytes(self.image(relocation=8))
            (root / "main.o").write_bytes(self.image(relocation=8, address=0))
            (root / "shiren2.map").write_text(" .text 0x1000 0x8 main.o\n")
            graph = {"main.o": {"image_id": "resident", "source": "main.c", "source_kind": "c"}}
            profile = {"tu_profiles": {"main.c": "fixture"}}
            matches = [{"source": "main.c", "symbol": name, "size": 4, "vram_start": 0x1000 + offset, "rom_start": offset}
                       for name, offset in (("first", 0), ("second", 4))]

            def symbols(path, profile):
                base = 0x1000 if path.name == "shiren2.elf" else 0
                return {"first": (base, 4), "second": (base + 4, 4)}

            def tamper_between_matches(path, address, size, **kwargs):
                if address == 4:
                    linked_path.write_bytes(self.image(relocation=8, executable=False))
                return elf_code(path, address, size, **kwargs)

            with patch("certification.attest"), patch("certification.symbol_table", side_effect=symbols), patch("certification.source_policy_violations", return_value=[]), patch("certification.text_relocations", return_value=[]), patch("certification.Elf32File", wraps=Elf32File) as constructor:
                self.assertEqual(len(c_attributions(root, matches, graph, profile, reference)), 2)
                self.assertEqual(constructor.call_count, 1)
                self.assertEqual(len(c_attributions(root, matches, graph, profile, reference)), 2)
                self.assertEqual(constructor.call_count, 2)
                with patch("certification.elf_code", side_effect=tamper_between_matches):
                    with self.assertRaisesRegex(ValueError, "no unique executable ELF section"):
                        c_attributions(root, matches, graph, profile, reference)
                self.assertEqual(constructor.call_count, 4)


class ReplayFailureEvidenceTests(unittest.TestCase):
    def fixture(self, directory: Path) -> dict:
        source = directory / "source"
        source.mkdir()
        for name in REQUIRED_FILES:
            (source / name).write_text("fixture\n")
        tools = source / "tools"
        tools.mkdir()
        # Real closure and manifest: the recipe guard is not mocked.
        for name in ("assets.py", "rommap.py", "rspaudio.py", "rom.py", "images.py", "evidence.py", "toolchain_library.py", "generated_symbols.py"):
            shutil.copy2(TOOLS / name, tools / name)
        (directory / "input-manifest.json").write_text(json.dumps(input_manifest(source)))
        return {"tu_profiles": {}, "compiler": {"gcc": {"path": sys.executable}}, "compiler_environment": {}}

    def test_nonzero_child_keeps_raw_outputs_and_complete_command_history(self):
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            profile = self.fixture(root)
            child = root / "failing-driver"
            child.write_text("#!/bin/sh\nprintf 'partial stdout\\n'\nprintf 'driver failure\\n' >&2\nexit 7\n")
            child.chmod(0o755)
            with patch.dict("os.environ", {"SHIREN2_REPLAY_EVIDENCE_ROOT": str(root / "replay")}), patch("certification.python_command", return_value=str(child)):
                with self.assertRaises(subprocess.CalledProcessError) as caught:
                    reproduce_c_objects_and_link(root, {}, profile, root / "original.z64")
            self.assertEqual(caught.exception.returncode, 7)
            trial, = (root / "replay").iterdir()
            log = trial / "command-evidence"
            self.assertEqual((log / "001.stdout").read_text(), "partial stdout\n")
            self.assertEqual((log / "001.stderr").read_text(), "driver failure\n")
            records = json.loads((log / "commands.json").read_text())
            self.assertEqual(records[-1]["returncode"], 7)
            self.assertEqual(records[-1]["argv"], caught.exception.cmd)
            self.assertEqual(json.loads((trial / "lifecycle.json").read_text())["status"], "failed")
            self.assertEqual((trial / "source/Makefile").read_text(), "fixture\n")

    def test_spawn_failure_keeps_attempted_command_without_success_code(self):
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            profile = self.fixture(root)
            missing = root / "missing-driver"
            with patch.dict("os.environ", {"SHIREN2_REPLAY_EVIDENCE_ROOT": str(root / "replay")}), patch("certification.python_command", return_value=str(missing)):
                with self.assertRaises(FileNotFoundError):
                    reproduce_c_objects_and_link(root, {}, profile, root / "original.z64")
            trial, = (root / "replay").iterdir()
            records = json.loads((trial / "command-evidence/commands.json").read_text())
            self.assertEqual(records[-1]["argv"][0], str(missing))
            self.assertNotIn("returncode", records[-1])
            self.assertEqual(json.loads((trial / "lifecycle.json").read_text())["status"], "failed")
