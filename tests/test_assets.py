"""Reference Yay0 decoder controls and the stored-tail pack against the real original."""
from __future__ import annotations

import io
import json
import shutil
import struct
import sys
import tempfile
import unittest
from contextlib import redirect_stderr
from pathlib import Path
from types import SimpleNamespace
from unittest.mock import patch

import yaml

PROJECT = Path(__file__).resolve().parents[1]
OWNED = PROJECT / "scratch"
sys.path.insert(0, str(PROJECT / "tools"))
import assets
import rom
import rommap


TEMP_ROOT = OWNED / "asset-test-tmp"
TEMP_ROOT.mkdir(parents=True, exist_ok=True)


def packed(size: int, mask: int, links: bytes, literals: bytes) -> bytes:
    """Direct decoder controls from the consumed sealed triage test contract."""
    return b"Yay0" + struct.pack(">III", size, 20, 20 + len(links)) + struct.pack(">I", mask) + links + literals


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


class CliTests(unittest.TestCase):
    def test_cli_requires_the_pack_command_and_its_override(self) -> None:
        for argv in (["assets.py"], ["assets.py", "pack"], ["assets.py", "extract"]):
            with self.subTest(argv=argv), patch.object(sys, "argv", argv), redirect_stderr(io.StringIO()), \
                    self.assertRaises(SystemExit) as context:
                assets.main()
            self.assertEqual(context.exception.code, 2)


class PackTests(unittest.TestCase):
    """Tiny actual crunch64 streams exercise the boundary without 13,727 recompressions."""

    def setUp(self) -> None:
        temporary = tempfile.TemporaryDirectory(dir=TEMP_ROOT)
        self.addCleanup(temporary.cleanup)
        self.directory = Path(temporary.name) / "run"
        self.source = self.directory / "source"
        (self.source / "config").mkdir(parents=True)
        encoder = assets._crunch64()
        one = encoder.yay0.compress(b"first editable source" * 3)
        two = encoder.yay0.compress(b"second editable source" * 2)
        self.original = b"HEAD" + one + b"\x00\x00" + two + b"raw passthrough"
        first, second = 4, 4 + len(one) + 2
        self.starts = [first, second]
        self.ranges = [(first, first + len(one), "asset_yay0", "one"),
                       (first + len(one), second, "pad_align_zero", "alignment"),
                       (second, second + len(two), "asset_yay0", "two"),
                       (second + len(two), len(self.original), "asset_raw_unresolved", "raw")]
        self.rom_map = SimpleNamespace(map_id="a" * 64, partition_sha256="b" * 64,
                                       evidence_policy_sha256="c" * 64, ranges=self.ranges)
        self.rom_map.of_class = lambda cls: [row for row in self.ranges if row[2] == cls]
        (self.source / "config/shiren2.jp.yaml").write_text(yaml.safe_dump({
            "segments": [{"start": 4, "name": "remaining", "type": "bin"}, [len(self.original)]]}))
        (self.source / rommap.MAP_PATH).write_text(json.dumps({"ranges": self.ranges}))
        self.reference = self.directory / "private-reference.z64"
        self.reference.write_bytes(self.original)
        (self.directory / "splat.override.yaml").write_text(yaml.safe_dump({
            "options": {"base_path": str(self.directory), "target_path": str(self.reference)}}))
        (self.directory / "generated/assets").mkdir(parents=True)
        (self.directory / assets.TAIL_OUTPUT).write_bytes(self.original[4:])
        for patcher in (patch.object(rom, "attest"), patch.object(rommap, "load", return_value=self.rom_map)):
            patcher.start()
            self.addCleanup(patcher.stop)
        assets.extract(self.source, self.reference)

    def source_manifest(self) -> dict:
        return json.loads((self.source / assets.SOURCE_MANIFEST).read_text())

    def rewrite_source(self, manifest: dict) -> None:
        (self.source / assets.SOURCE_MANIFEST).write_text(json.dumps(manifest))

    def test_real_encoder_roundtrip_and_explicit_passthrough(self) -> None:
        manifest = assets.pack(self.directory)
        self.assertEqual(assets.read_manifest(self.directory, self.rom_map), dict.fromkeys(self.starts, True))
        self.assertEqual((self.directory / assets.TAIL_OUTPUT).read_bytes(), self.original[4:])
        self.assertEqual(len(manifest["ranges"]), 4)
        for row in manifest["ranges"]:
            if row["class"] != "asset_yay0":
                self.assertEqual((row["mode"], row["editable_bytes"]), ("passthrough", 0))
        self.assertFalse(self.source_manifest()["provenance"]["independent_authorship"])
        self.assertEqual(assets.generated_keys(self.source),
                         {"assets/tail/manifest.json", *(assets.sidecar_key(s) for s in self.starts)})

    def test_stale_split_and_foreign_build_directory_fail_before_writes(self) -> None:
        output = self.directory / assets.TAIL_OUTPUT
        output.write_bytes(b"not the split tail")
        with self.assertRaisesRegex(assets.AssetError, "disagrees with the original"):
            assets.pack(self.directory)
        self.assertFalse((self.directory / assets.SIDECAR_ROOT).exists())
        options = self.directory / "splat.override.yaml"
        document = yaml.safe_load(options.read_text())
        document["options"]["base_path"] = str(self.directory.parent)
        options.write_text(yaml.safe_dump(document))
        with self.assertRaisesRegex(assets.AssetError, "build directory"):
            assets.pack(self.directory)

    def test_extract_never_overwrites_existing_sources(self) -> None:
        path = self.source / assets.sidecar_key(self.starts[0])
        path.write_bytes(b"my edits")
        with self.assertRaises(FileExistsError):
            assets.extract(self.source, self.reference)
        self.assertEqual(path.read_bytes(), b"my edits")

    def test_missing_source_cannot_be_rescued(self) -> None:
        (self.source / assets.sidecar_key(self.starts[0])).unlink()
        with patch.object(assets, "_decode", side_effect=AssertionError("build decoded ROM")), \
                self.assertRaisesRegex(assets.AssetError, "Missing"):
            assets.pack(self.directory)
        self.assertFalse((self.directory / assets.SIDECAR_ROOT).exists())

    def test_missing_source_manifest_cannot_be_imported_implicitly(self) -> None:
        (self.source / assets.SOURCE_MANIFEST).unlink()
        with patch.object(assets, "extract", side_effect=AssertionError("implicit import")), \
                self.assertRaises(assets.AssetError):
            assets.pack(self.directory)

    def test_changed_source_and_rewritten_declaration_are_rejected(self) -> None:
        path = self.source / assets.sidecar_key(self.starts[0])
        old = path.read_bytes()
        path.write_bytes(bytes([old[0] ^ 1]) + old[1:])
        with self.assertRaisesRegex(assets.AssetError, "changed"):
            assets.pack(self.directory)
        manifest = self.source_manifest()
        manifest["streams"][0]["sha256"] = assets.sha256(path.read_bytes())
        self.rewrite_source(manifest)
        with self.assertRaisesRegex(assets.AssetError, "imported sources"):
            assets.pack(self.directory)

    def test_stale_map_and_forged_provenance_fields(self) -> None:
        pristine = self.source_manifest()
        mutations = [
            lambda m: m.__setitem__("map_id", "0" * 64),
            lambda m: m.__setitem__("partition_sha256", "0" * 64),
            lambda m: m.__setitem__("evidence_policy_sha256", "0" * 64),
            lambda m: m["provenance"].__setitem__("mode", "decode-at-build"),
            lambda m: m["provenance"].__setitem__("mode", "canonical-copy"),
            lambda m: m["provenance"].__setitem__("phase", "build"),
            lambda m: m["provenance"].__setitem__("independent_authorship", True),
            lambda m: m.__setitem__("receipt_binding", "f" * 64),
            lambda m: m["streams"][0].__setitem__("path", "../outside.bin"),
            lambda m: m["streams"][0].__setitem__("path", "/tmp/outside.bin"),
            lambda m: m["streams"].append(m["streams"][0]),
            lambda m: m["encoder"].__setitem__("files_sha256", "0" * 64),
        ]
        for mutate in mutations:
            manifest = json.loads(json.dumps(pristine))
            mutate(manifest)
            self.rewrite_source(manifest)
            with self.subTest(mutation=mutate), self.assertRaises(assets.AssetError):
                assets.read_source_manifest(self.source, self.rom_map, self.original)

    def test_symlink_source_and_extra_file_are_rejected(self) -> None:
        path = self.source / assets.sidecar_key(self.starts[0])
        outside = self.directory / "outside.bin"
        outside.write_bytes(path.read_bytes())
        path.unlink()
        path.symlink_to(outside)
        with self.assertRaisesRegex(assets.AssetError, "Symlink"):
            assets.pack(self.directory)
        path.unlink()
        path.write_bytes(outside.read_bytes())
        extra = self.source / "assets/tail/yay0/extra.bin"
        extra.write_bytes(b"extra")
        with self.assertRaisesRegex(assets.AssetError, "extra"):
            assets.pack(self.directory)

    def test_encoder_implementation_changes_are_measured_not_version_only(self) -> None:
        implementation = self.directory / "crunch64/__init__.py"
        implementation.parent.mkdir()
        implementation.write_bytes(b"implementation one")
        relative = Path("crunch64/__init__.py")
        distribution = SimpleNamespace(version="0.6.2", files=[relative],
                                       locate_file=lambda name: self.directory / name)
        module = SimpleNamespace(__version__="0.6.2", __file__=str(implementation))
        with patch.object(assets, "_crunch64", return_value=module), \
                patch.object(assets.importlib.metadata, "distribution", return_value=distribution):
            first = assets.encoder_fingerprint()
            implementation.write_bytes(b"implementation two")
            second = assets.encoder_fingerprint()
        self.assertEqual(first["version"], second["version"])
        self.assertNotEqual(first["files_sha256"], second["files_sha256"])
        with patch.object(assets, "encoder_fingerprint", return_value=second), \
                self.assertRaisesRegex(assets.AssetError, "implementation changed"):
            assets.pack(self.directory)

    def test_explicit_rebind_preserves_old_manifest_and_all_streams(self) -> None:
        path = self.source / assets.SOURCE_MANIFEST
        old = self.source_manifest()
        old["map_id"] = "d" * 64
        old["encoder"]["files_sha256"] = "e" * 64
        self.rewrite_source(old)
        before = path.read_bytes()
        result = assets.rebind(self.source, self.reference)
        self.assertEqual(result["streams"], 2)
        self.assertEqual((self.source / result["old_manifest"]["path"]).read_bytes(), before)
        self.assertEqual(assets.read_source_manifest(self.source, self.rom_map, self.original)["streams"],
                         old["streams"])
        assets.pack(self.directory)
        self.assertEqual(assets.read_manifest(self.directory, self.rom_map), dict.fromkeys(self.starts, True))

    def test_rebind_checks_last_stream_before_any_manifest_write(self) -> None:
        path = self.source / assets.SOURCE_MANIFEST
        manifest = self.source_manifest()
        changed = self.source / assets.sidecar_key(self.starts[-1])
        body = changed.read_bytes()
        changed.write_bytes(bytes([body[0] ^ 1]) + body[1:])
        manifest["streams"][-1]["sha256"] = assets.sha256(changed.read_bytes())
        imported = manifest["provenance"]["imported_sources"]
        data = assets._json_bytes(manifest["streams"])
        (self.source / imported["path"]).write_bytes(data)
        imported.update(sha256=assets.sha256(data), size=len(data))
        self.rewrite_source(manifest)
        before = path.read_bytes()
        inventory = assets._inventory(self.source, "assets/tail")
        with self.assertRaisesRegex(assets.AssetError, "nonexact"):
            assets.rebind(self.source, self.reference)
        self.assertEqual(path.read_bytes(), before)
        self.assertEqual(assets._inventory(self.source, "assets/tail"), inventory)

    def test_inexact_encoder_cannot_copy_original_as_rescue(self) -> None:
        with patch.object(assets._crunch64().yay0, "compress", return_value=b"Yay0"), \
                self.assertRaisesRegex(assets.AssetError, "does not reproduce"):
            assets.pack(self.directory)
        self.assertFalse((self.directory / assets.SIDECAR_ROOT).exists())

    def test_forged_exact_flags_hashes_and_passthrough_credit_rejected(self) -> None:
        pristine = assets.pack(self.directory)
        path = self.directory / assets.SIDECAR_ROOT / "manifest.json"
        mutations = [
            lambda m: m.__setitem__("exact_streams", 999),
            lambda m: m.__setitem__("receipt_binding", "a" * 64),
            lambda m: m.__setitem__("source_manifest_sha256", "a" * 64),
            lambda m: m["streams"][0].__setitem__("exact", True),
            lambda m: m["ranges"][1].__setitem__("editable_bytes", 2),
            lambda m: m["ranges"][1].__setitem__("mode", "encoded-frozen-source"),
            lambda m: m["ranges"][0].__setitem__("sha256", "a" * 64),
            lambda m: m["encoder"].__setitem__("files_sha256", "a" * 64),
            lambda m: m["ranges"].pop(),
        ]
        for mutate in mutations:
            manifest = json.loads(json.dumps(pristine))
            mutate(manifest)
            path.write_text(json.dumps(manifest))
            with self.subTest(mutation=mutate), self.assertRaises(assets.AssetError):
                assets.read_manifest(self.directory, self.rom_map)

    def test_modified_packed_bytes_even_with_rehashed_manifest_rejected(self) -> None:
        manifest = assets.pack(self.directory)
        path = self.directory / assets.TAIL_OUTPUT
        body = bytearray(path.read_bytes())
        body[0] ^= 1
        path.write_bytes(body)
        manifest["tail"]["sha256"] = assets.sha256(body)
        (self.directory / assets.SIDECAR_ROOT / "manifest.json").write_text(json.dumps(manifest))
        with self.assertRaisesRegex(assets.AssetError, "attested reference"):
            assets.read_manifest(self.directory, self.rom_map)

    def test_coherent_forged_sources_ledger_and_pack_require_actual_encoding(self) -> None:
        assets.pack(self.directory)
        frozen = self.source_manifest()
        row = frozen["streams"][0]
        garbage = b"!" * row["size"]
        (self.source / row["path"]).write_bytes(garbage)
        (self.directory / "generated" / row["path"]).write_bytes(garbage)
        row["sha256"] = assets.sha256(garbage)
        ledger = assets._json_bytes(frozen["streams"])
        declaration = frozen["provenance"]["imported_sources"]
        (self.source / declaration["path"]).write_bytes(ledger)
        declaration.update(sha256=assets.sha256(ledger), size=len(ledger))
        self.rewrite_source(frozen)
        packed = (self.directory / assets.TAIL_OUTPUT).read_bytes()
        forged = assets._pack_record(self.source, self.original, self.rom_map, frozen, packed)
        (self.directory / assets.SIDECAR_ROOT / "manifest.json").write_bytes(assets._json_bytes(forged))
        # All hashes, extents, mirrored files and provenance declarations are internally
        # consistent. Only actual encoding distinguishes this forgery from pack evidence.
        self.assertEqual(assets.read_source_manifest(self.source, self.rom_map, self.original), frozen)
        with self.assertRaisesRegex(assets.AssetError, "actual encoder roundtrip"):
            assets.read_manifest(self.directory, self.rom_map)

    def test_modified_generated_sidecar_and_foreign_map_rejected(self) -> None:
        assets.pack(self.directory)
        path = self.directory / "generated" / assets.sidecar_key(self.starts[0])
        path.write_bytes(b"forged sidecar")
        with self.assertRaisesRegex(assets.AssetError, "sidecar changed"):
            assets.read_manifest(self.directory, self.rom_map)
        foreign = SimpleNamespace(map_id="0" * 64, partition_sha256="0" * 64,
                                  evidence_policy_sha256="0" * 64)
        with self.assertRaisesRegex(assets.AssetError, "does not belong"):
            assets.read_manifest(self.directory, foreign)

    def test_cli_rejects_decode_at_build_and_canonical_copy_provenance(self) -> None:
        pristine = self.source_manifest()
        for mode in ("decode-at-build", "canonical-copy"):
            manifest = json.loads(json.dumps(pristine))
            manifest["provenance"]["mode"] = mode
            self.rewrite_source(manifest)
            argv = ["assets.py", "validate-sources", "--root", str(self.source), "--rom", str(self.reference)]
            with patch.object(sys, "argv", argv), redirect_stderr(io.StringIO()), \
                    self.assertRaises(SystemExit) as context:
                assets.main()
            self.assertEqual(context.exception.code, 1)

    def test_explicit_historical_migration_without_any_codec_work(self) -> None:
        frozen = self.source_manifest()
        shutil.rmtree(self.source / assets.PROVENANCE_ROOT)
        (self.source / assets.SOURCE_MANIFEST).unlink()
        historical = self.directory / "history"
        (historical / assets.SIDECAR_ROOT).mkdir(parents=True)
        (historical / "source/tools").mkdir(parents=True)
        extractor = b"historical extractor bytes"
        (historical / "source/tools/assets.py").write_bytes(extractor)
        inputs = {"files": {r["path"]: {"sha256": r["sha256"], "size": r["size"]}
                            for r in frozen["streams"]}}
        inputs["files"]["tools/assets.py"] = {"sha256": assets.sha256(extractor), "size": len(extractor)}
        old_pack = {"schema_version": 1, "kind": assets.MANIFEST_KIND,
                    "streams": [[r["rom_start"], r["rom_end"], r["sha256"], True] for r in frozen["streams"]],
                    "exact_streams": len(frozen["streams"])}
        pack_bytes = assets._json_bytes(old_pack)
        receipt = {"input_manifest": inputs, "target_sha256": rom.EXPECTED_SHA256,
                   "generated": {"assets/tail/manifest.json": assets.sha256(pack_bytes)}}
        receipt_bytes = assets._json_bytes(receipt)
        (historical / "input-manifest.json").write_bytes(assets._json_bytes(inputs))
        (historical / "receipt.json").write_bytes(receipt_bytes)
        (historical / assets.SIDECAR_ROOT / "manifest.json").write_bytes(pack_bytes)
        with patch.object(assets, "R1_RECEIPT_SHA256", assets.sha256(receipt_bytes)), \
                patch.object(assets, "R1_PACK_SHA256", assets.sha256(pack_bytes)), \
                patch.object(assets, "_decode", side_effect=AssertionError("migration decoded")), \
                patch.object(assets._crunch64().yay0, "compress", side_effect=AssertionError("migration encoded")):
            source_path = self.source / frozen["streams"][0]["path"]
            source_bytes = source_path.read_bytes()
            source_path.write_bytes(bytes([source_bytes[0] ^ 1]) + source_bytes[1:])
            with self.assertRaisesRegex(assets.AssetError, "does not match"):
                assets.import_manifest(self.source, self.reference, historical)
            self.assertFalse((self.source / assets.SOURCE_MANIFEST).exists())
            self.assertFalse((self.source / assets.PROVENANCE_ROOT).exists())
            self.assertNotEqual(source_path.read_bytes(), source_bytes)
            source_path.write_bytes(source_bytes)
            result = assets.import_manifest(self.source, self.reference, historical)
            self.assertEqual((result["decodes"], result["recompressions"]), (0, 0))
            self.assertEqual(assets.read_source_manifest(self.source, self.rom_map, self.original)["streams"],
                             frozen["streams"])
            with self.assertRaisesRegex(assets.AssetError, "overwrite"):
                assets.import_manifest(self.source, self.reference, historical)
            evidence = self.source_manifest()["provenance"]["evidence"]
            path = self.source / evidence["receipt"]["path"]
            path.write_bytes(b"forged historical receipt")
            with self.assertRaises(assets.AssetError):
                assets.read_source_manifest(self.source, self.rom_map, self.original)

    def test_tail_keys_are_empty_without_a_stored_tail_segment(self) -> None:
        (self.source / "config/shiren2.jp.yaml").write_text(
            yaml.safe_dump({"segments": [[0x0, "bin", "header"], [0x40]]}))
        self.assertEqual(assets.generated_keys(self.source), set())
        with self.assertRaisesRegex(assets.AssetError, "no stored-tail"):
            assets.tail_interval(self.source)


PRIVATE_ROM = PROJECT.parent / "inputs/r1-build/shiren2.z64"


@unittest.skipUnless(PRIVATE_ROM.is_file(), "Copied private r1 ROM required")
class BoundedRealStreamTests(unittest.TestCase):
    def test_one_small_real_stream_matches_frozen_source(self) -> None:
        # Exact historical extent is read from the copied pack; never scan/decode all streams.
        historical = json.loads((PROJECT.parent / "inputs/r1-build/generated/assets/tail/manifest.json").read_text())
        row = next(row for row in historical["streams"] if row[0] == 0xE7C5A0)
        with PRIVATE_ROM.open("rb") as stream:
            stream.seek(row[0])
            stored = stream.read(row[1] - row[0])
        decoded, evidence = assets.decode_yay0(stored)
        self.assertEqual(evidence.consumed_end, len(stored))
        self.assertEqual(assets.sha256(decoded), row[2])
        self.assertEqual(decoded, (PROJECT / assets.sidecar_key(row[0])).read_bytes())
        self.assertEqual(assets._crunch64().yay0.compress(decoded), stored)


if __name__ == "__main__":
    unittest.main(verbosity=2)
