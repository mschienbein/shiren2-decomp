"""Private-input guarded checks for external identity, extent and transfer separation."""
from __future__ import annotations

import os
from pathlib import Path
import shutil
import sys
import tempfile
import unittest

PROJECT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(PROJECT / "tools"))
import rspaudio

PRIVATE_ROM = Path(os.environ.get("H21_TEST_ROM", str(PROJECT.parent / "inputs/r1-build/shiren2.z64")))


@unittest.skipUnless(PRIVATE_ROM.is_file() and (PROJECT / rspaudio.OBJECT_PATH).is_file(),
                     "Private ROM and separately acquired SDK witness required")
class AudioIdentityTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls) -> None:
        cls.original = PRIVATE_ROM.read_bytes()

    def test_section_symbols_not_dma_sizes_bound_storage(self) -> None:
        result = rspaudio.verify(self.original, PROJECT)
        text, data = result["sections"]
        self.assertEqual((text["rom_start"], text["rom_end"]), (0x10AEB0, 0x10BB10))
        self.assertEqual((data["rom_start"], data["rom_end"]), (0x11D4C0, 0x11D770))
        self.assertEqual(result["task"]["descriptor_text_size"], 0x1000)
        self.assertEqual(result["task"]["text_request"]["rom_end"], 0x10BE30)
        self.assertEqual(result["task"]["data_request"]["rom_end"], 0x11DCC0)
        self.assertEqual([(r["rom_start"], r["rom_end"], r["status"]) for r in result["unresolved"]],
                         [(0x11D770, 0x11D774, "OPEN")])

    def test_zero_text_tail_is_part_of_external_identity(self) -> None:
        for site in (0x10BB04, 0x10BB08, 0x10BB0C, 0x11D76F):
            changed = bytearray(self.original)
            changed[site] ^= 1
            with self.subTest(site=site), self.assertRaisesRegex(ValueError, "section differs from ROM"):
                rspaudio.verify(bytes(changed), PROJECT)

    def test_substituted_or_truncated_object_fails_closed(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            (root / "config/rom-evidence").mkdir(parents=True)
            shutil.copyfile(PROJECT / "config/images.json", root / "config/images.json")
            original = (PROJECT / rspaudio.OBJECT_PATH).read_bytes()
            for replacement in (original[:-1], original[:0x34] + b"\x00" + original[0x35:]):
                (root / rspaudio.OBJECT_PATH).write_bytes(replacement)
                with self.assertRaisesRegex(ValueError, "SDK object identity changed"):
                    rspaudio.verify(self.original, root)
            (root / rspaudio.OBJECT_PATH).unlink()
            with self.assertRaises(FileNotFoundError):
                rspaudio.verify(self.original, root)

    def test_changed_loader_cannot_rebind_an_identical_blob(self) -> None:
        changed = bytearray(self.original)
        changed[0x1006F3] ^= 0x10
        with self.assertRaisesRegex(ValueError, "loader identity changed"):
            rspaudio.verify(bytes(changed), PROJECT)


if __name__ == "__main__":
    unittest.main()
