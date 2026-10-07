"""Focused private proposal tests using controls and four fixed original windows."""
from __future__ import annotations

import copy
import hashlib
import io
import json
import struct
import sys
import tempfile
import unittest
from contextlib import redirect_stderr
from pathlib import Path
from unittest.mock import patch


PROJECT = Path(__file__).resolve().parents[1]
OWNED = PROJECT / "scratch"
sys.path.insert(0, str(PROJECT / "tools"))
import assets
import rom


CONFIG = PROJECT / "config/asset_regions.json"
TEMP_ROOT = OWNED / "asset-test-tmp"
TEMP_ROOT.mkdir(exist_ok=True)


def packed(size: int, mask: int, links: bytes, literals: bytes) -> bytes:
    """Direct decoder controls from the consumed sealed triage test contract."""
    return b"Yay0" + struct.pack(">III", size, 20, 20 + len(links)) + struct.pack(">I", mask) + links + literals


def document() -> dict:
    return json.loads(CONFIG.read_text())


class DecodeTests(unittest.TestCase):
    def test_literals_and_unused_mask_fetch(self) -> None:
        decoded, evidence = assets.decode_yay0(packed(3, 0xE0000000, b"", b"ABC"))
        self.assertEqual(decoded, b"ABC")
        self.assertEqual((evidence.tokens, evidence.mask_end, evidence.minimum_flag_byte_end), (3, 20, 17))

    def test_overlap_and_extended_lengths(self) -> None:
        for size, code, count in [(5, b"\x20\x00", b""), (19, b"\x00\x00", b"\x00"), (274, b"\x00\x00", b"\xff")]:
            with self.subTest(size=size):
                decoded, evidence = assets.decode_yay0(packed(size, 0x80000000, code, b"A" + count))
                self.assertEqual(decoded, b"A" * size)
                self.assertEqual(evidence.overlapping_links, 1)

    def test_nonoverlapping_copy(self) -> None:
        self.assertEqual(assets.decode_yay0(packed(6, 0xE0000000, b"\x10\x02", b"ABC"))[0], b"ABCABC")

    def test_second_mask_word_is_needed(self) -> None:
        data = b"Yay0" + struct.pack(">III", 33, 24, 24) + b"\xff\xff\xff\xff\x80\0\0\0" + bytes(range(33))
        decoded, evidence = assets.decode_yay0(data)
        self.assertEqual(decoded, bytes(range(33)))
        self.assertEqual((evidence.mask_end, evidence.minimum_flag_byte_end, evidence.unused_mask_bits), (24, 21, 31))

    def test_malformed_headers_and_stream_geometry(self) -> None:
        valid = packed(3, 0xE0000000, b"", b"ABC")
        cases = [(valid[:15], "header"), (b"Yaz0" + valid[4:], "magic")]
        for at, value, message in [(4, 0, "size"), (8, 24, "offset"), (12, 32, "offset"), (8, 19, "unaligned"), (12, 21, "unaligned")]:
            data = bytearray(valid)
            struct.pack_into(">I", data, at, value)
            cases.append((bytes(data), message))
        for data, message in cases:
            with self.subTest(message=message, data=data.hex()), self.assertRaisesRegex(assets.AssetError, message):
                assets.decode_yay0(data)

    def test_stream_reads_cannot_cross_into_neighbors(self) -> None:
        with self.assertRaisesRegex(assets.AssetError, "mask stream.*crosses"):
            assets.decode_yay0(packed(33, 0xFFFFFFFF, b"", b"A" * 33))
        with self.assertRaisesRegex(assets.AssetError, "link stream.*crosses"):
            assets.decode_yay0(packed(4, 0x80000000, b"", b"A"))

    def test_truncated_literal_and_extended_length(self) -> None:
        for data, message in [(packed(2, 0xC0000000, b"", b"A"), "truncated literal"),
                              (packed(19, 0x80000000, b"\0\0", b"A"), "truncated extended")]:
            with self.subTest(message=message), self.assertRaisesRegex(assets.AssetError, message):
                assets.decode_yay0(data)

    def test_invalid_distance_and_declared_run(self) -> None:
        for data, message in [(packed(3, 0, b"\x10\x00", b""), "precedes"),
                              (packed(4, 0x80000000, b"\x1f\xff", b"A"), "precedes"),
                              (packed(4, 0x80000000, b"\x20\x00", b"A"), "run exceeds")]:
            with self.subTest(message=message), self.assertRaisesRegex(assets.AssetError, message):
                assets.decode_yay0(data)

    def test_input_output_and_work_limits(self) -> None:
        data = packed(3, 0xE0000000, b"", b"ABC")
        for limits, message in [({"max_input": 22}, "input limit"), ({"max_output": 2}, "output limit"),
                                ({"max_tokens": 2}, "work limit"), ({"max_tokens": True}, "invalid token")]:
            with self.subTest(limits=limits), self.assertRaisesRegex(assets.AssetError, message):
                assets.decode_yay0(data, **limits)


class PolicyTests(unittest.TestCase):
    def test_scope_cannot_expand_or_shift_or_use_path_names(self) -> None:
        for mutation in ("extra", "shift", "name", "target", "limit", "boolean", "unknown"):
            value = document()
            if mutation == "extra": value["regions"].append(copy.deepcopy(value["regions"][0]))
            elif mutation == "shift": value["regions"][0]["rom_start"] += 16
            elif mutation == "name": value["regions"][0]["id"] = "../escape"
            elif mutation == "target": value["target_sha256"] = "0" * 64
            elif mutation == "limit": value["limits"]["max_input"] *= 2
            elif mutation == "boolean": value["schema_version"] = True
            else: value["extra"] = "ignored?"
            with self.subTest(mutation=mutation), self.assertRaises(assets.AssetError):
                assets.validate_regions(value)

    def test_cli_requires_opt_in_and_explicit_output(self) -> None:
        with patch.object(sys, "argv", ["assets.py"]), redirect_stderr(io.StringIO()), self.assertRaises(SystemExit) as context:
            assets.main()
        self.assertEqual(context.exception.code, 2)
        with patch.object(sys, "argv", ["assets.py", "extract"]), redirect_stderr(io.StringIO()), self.assertRaises(SystemExit) as context:
            assets.main()
        self.assertEqual(context.exception.code, 2)


class OriginalAndOutputTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls) -> None:
        cls.original = rom.CANONICAL_ROM.read_bytes()
        cls.original_identity = rom.attest(cls.original)
        cls.regions = assets.validate_regions(document())

    def setUp(self) -> None:
        self.temporary = tempfile.TemporaryDirectory(dir=TEMP_ROOT)
        self.addCleanup(self.temporary.cleanup)
        self.root = Path(self.temporary.name)

    def extract(self, output: Path, config: Path = CONFIG, original: Path = rom.CANONICAL_ROM) -> dict:
        return assets.extract_sidecars(original, config, output, allowed_root=self.root)

    def test_original_exact_end_and_last_byte_are_required(self) -> None:
        for region in self.regions:
            with self.subTest(start=hex(region.start)):
                exact = self.original[region.start:region.consumed_end]
                decoded, evidence = assets.decode_yay0(exact)
                self.assertEqual(hashlib.sha256(decoded).hexdigest(), region.decoded_sha256)
                self.assertEqual(evidence.consumed_end, len(exact))
                with self.assertRaises(assets.AssetError):
                    assets.decode_yay0(exact[:-1])

    def test_complete_sidecars_and_manifest_are_destination_independent(self) -> None:
        a, b = self.root / "first", self.root / "second"
        manifest = self.extract(a)
        self.extract(b)
        self.assertEqual({p.name: p.read_bytes() for p in a.iterdir()}, {p.name: p.read_bytes() for p in b.iterdir()})
        self.assertEqual(len(list(a.iterdir())), 5)
        self.assertEqual(len(manifest["regions"]), 4)
        for region, item in zip(self.regions, manifest["regions"], strict=True):
            self.assertEqual(item["stored_window"]["sha256"], region.window_sha256)
            self.assertEqual(item["consumed_stream"]["rom_end"], region.consumed_end)
            self.assertEqual(item["gap"]["sha256"], region.gap_sha256)
            self.assertEqual(item["decoded_sidecar"]["sha256"], hashlib.sha256((a / item["decoded_sidecar"]["path"]).read_bytes()).hexdigest())
        self.assertEqual(hashlib.sha256(rom.CANONICAL_ROM.read_bytes()).hexdigest(), rom.EXPECTED_SHA256)

    def test_existing_directory_or_file_is_never_overwritten(self) -> None:
        output = self.root / "existing"
        self.extract(output)
        before = {p.name: p.read_bytes() for p in output.iterdir()}
        with self.assertRaisesRegex(assets.AssetError, "collision"):
            self.extract(output)
        self.assertEqual(before, {p.name: p.read_bytes() for p in output.iterdir()})
        file = self.root / "file"
        file.write_bytes(b"user sentinel")
        with self.assertRaisesRegex(assets.AssetError, "collision"):
            self.extract(file)
        self.assertEqual(file.read_bytes(), b"user sentinel")

    def test_symlinks_and_parent_escape_rejected(self) -> None:
        link = self.root / "dangling"
        link.symlink_to(self.root / "absent")
        with self.assertRaisesRegex(assets.AssetError, "symlink"):
            self.extract(link)
        parent = self.root / "real"
        parent.mkdir()
        indirect = self.root / "indirect"
        indirect.symlink_to(parent, target_is_directory=True)
        with self.assertRaisesRegex(assets.AssetError, "symlink"):
            self.extract(indirect / "new")
        with self.assertRaisesRegex(assets.AssetError, "unsafe"):
            self.extract(self.root / ".." / "escape")
        with self.assertRaisesRegex(assets.AssetError, "private output root"):
            self.extract(OWNED / "outside-test-root")

    def test_changed_real_rom_is_rejected_before_any_output(self) -> None:
        changed = bytearray(self.original)
        changed[0x240C20 + 20] ^= 1
        path = self.root / "changed.z64"
        path.write_bytes(changed)
        output = self.root / "not-created"
        with self.assertRaisesRegex(ValueError, "Wrong target"):
            self.extract(output, original=path)
        self.assertFalse(output.exists())
        self.assertEqual(hashlib.sha256(rom.CANONICAL_ROM.read_bytes()).hexdigest(), rom.EXPECTED_SHA256)

    def test_changed_window_and_consumed_binding_reject_before_writes(self) -> None:
        mutations = [("window_sha256", "0" * 64), ("consumed_rom_end", self.regions[0].consumed_end - 1),
                     ("decoded_sha256", "0" * 64), ("gap_sha256", "0" * 64)]
        for key, value in mutations:
            data = document()
            data["regions"][0][key] = value
            config = self.root / f"{key}.json"
            config.write_text(json.dumps(data))
            output = self.root / f"no-{key}"
            with self.subTest(key=key), self.assertRaisesRegex(assets.AssetError, "identity changed"):
                self.extract(output, config=config)
            self.assertFalse(output.exists())

    def test_changed_stream_cursor_rejects_before_writes(self) -> None:
        data = document()
        data["regions"][3]["expected_cursors"]["mask_end"] -= 4
        config = self.root / "wrong-cursor.json"
        config.write_text(json.dumps(data))
        output = self.root / "no-cursor"
        with self.assertRaisesRegex(assets.AssetError, "identity changed"):
            self.extract(output, config=config)
        self.assertFalse(output.exists())

    def test_exclusive_directory_reservation_rejects_racing_collision(self) -> None:
        output = self.root / "race"
        real_mkdir = Path.mkdir
        def raced(path: Path, *args, **kwargs) -> None:
            if path == output:
                real_mkdir(path)
                (path / "sentinel").write_bytes(b"other owner")
            return real_mkdir(path, *args, **kwargs)
        with patch.object(Path, "mkdir", raced), self.assertRaises(FileExistsError):
            self.extract(output)
        self.assertEqual({p.name: p.read_bytes() for p in output.iterdir()}, {"sentinel": b"other owner"})


if __name__ == "__main__":
    unittest.main(verbosity=2)
