"""Packet policy fixtures replace the expensive private-ROM certification step.

They exercise parsing, mapping, inventories, identity races and output policy;
actual compiler/ROM compatibility is a separate private validation run.
"""
from __future__ import annotations

import copy
import hashlib
import json
import subprocess
import sys
import tempfile
import unittest
from dataclasses import replace
from pathlib import Path
from types import MappingProxyType
from unittest.mock import patch

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "tools"))
import packet
from certification import file_inventory, sha256
from images import ComponentRecord, ImageInventory, ImageRecord
from rom import EXPECTED_SHA256
from workspace import REQUIRED_FILES, input_manifest


class PacketTests(unittest.TestCase):
    def setUp(self) -> None:
        self.temporary = tempfile.TemporaryDirectory()
        self.addCleanup(self.temporary.cleanup)
        self.root = Path(self.temporary.name).resolve() / "project"
        self.directory = self.root / "build/frozen"
        source = self.directory / "source"
        source.mkdir(parents=True)
        for name in REQUIRED_FILES:
            (source / name).write_text(f"fixture {name}\n", encoding="utf-8")
        for name, content in {
            "include/types.h": "typedef unsigned int u32;\n",
            "config/images.json": "{}\n", "config/compiler_profiles.json": "{}\n",
            "config/symbol_addrs.txt": "fixture\n", "config/reloc_addrs.txt": "fixture\n",
        }.items():
            path = source / name
            path.parent.mkdir(parents=True, exist_ok=True)
            path.write_text(content, encoding="utf-8")
        (self.directory / "shiren2.ld").write_text("fixture linker\n", encoding="utf-8")
        self.sdk_words = (0x03E00008, 0)
        self.game_words = (0x0C009700, 0, 0x03E00008, 0)
        original = bytearray(0x3000)
        original[0x1000:0x1008] = b"".join(word.to_bytes(4, "big") for word in self.sdk_words)
        original[0x2000:0x2010] = b"".join(word.to_bytes(4, "big") for word in self.game_words)
        self.reference = self.root / "original.z64"
        self.reference.write_bytes(original)
        self.inventory = ImageInventory(
            EXPECTED_SHA256, len(original), False, False, None, "fixture-only image policy",
            MappingProxyType({
                "resident": self.image("resident", 0x1000, 0x80025C00),
                "main_14400": self.image("main_14400", 0x2000, 0x800413C0),
            }), (), (), (), MappingProxyType({}), MappingProxyType({}),
        )
        self.profile = {"id": "fixture", "profiles": {"candidate": {"scope": "fixture only"}}, "tu_profiles": {}}
        self.runtime = {"scope": "fixture only"}
        self.graph = {
            "obj/generated/asm/sdk.o": {"image_id": "resident", "input": "generated/asm/sdk.s", "source": "generated/asm/sdk.s", "source_kind": "asm"},
            "obj/generated/asm/game.o": {"image_id": "main_14400", "input": "generated/asm/game.s", "source": "generated/asm/game.s", "source_kind": "asm"},
        }
        self.matches: list[dict] = []
        self.write_asm("sdk", self.assembly("sdk", 0x1000, 0x80025C00, self.sdk_words))
        self.write_asm("game", self.assembly("game", 0x2000, 0x800413C0, self.game_words))
        self.receipt_path = self.directory / "receipt.json"
        self.rewrite_receipt()
        self.verifier = self.start_patch("verify_receipt", side_effect=lambda path, **kwargs: json.loads(path.read_text()))
        self.start_patch("PROJECT", self.root)
        self.start_patch("build_graph", side_effect=lambda directory: copy.deepcopy(self.graph))
        self.start_patch("image_evidence", side_effect=lambda source, reference: (self.inventory, copy.deepcopy(self.image_report())))
        self.start_patch("linked_image_records", return_value={})
        self.start_patch("tool_profile", side_effect=lambda *args: copy.deepcopy(self.profile))
        self.start_patch("runtime_fingerprint", side_effect=lambda: copy.deepcopy(self.runtime))
        self.start_patch("attest", return_value={"sha256": EXPECTED_SHA256})
        executable = self.root / "bin/m2c"
        executable.parent.mkdir()
        executable.write_text("fixture m2c\n", encoding="utf-8")
        self.start_patch("_m2c_metadata", side_effect=lambda: {
            "executable": str(executable), "executable_sha256": sha256(executable), "package_version": "fixture",
        })

    def start_patch(self, name: str, *args: object, **kwargs: object):
        patcher = patch.object(packet, name, *args, **kwargs)
        self.addCleanup(patcher.stop)
        return patcher.start()

    def image(self, name: str, rom_start: int, vram_start: int) -> ImageRecord:
        return ImageRecord(
            name, rom_start, rom_start + 0x80, vram_start, vram_start + 0x80,
            "0" * 64, "uncompressed-linear", name, "fixture", "reviewed-initialized-extent", (),
            "unknown", (), (name,), "active", (), ("fixture, not original-game evidence",),
        )

    def image_report(self) -> dict:
        return {"config_sha256": sha256(self.directory / "source/config/images.json"), "inventory": {"fixture": True}}

    def assembly(self, symbol: str, offset: int, vram: int, words: tuple[int, ...]) -> str:
        body = []
        for index, word in enumerate(words):
            mnemonic = "jal sdk" if word >> 26 == 3 else "jr $ra" if word == 0x03E00008 else "nop"
            body.append(f"/* {offset + index * 4:X} {vram + index * 4:X} {word:08X} */ {mnemonic}")
        return ".section .text, \"ax\"\n\nglabel " + symbol + "\n" + "\n".join(body) + "\nendlabel " + symbol + "\n"

    def write_asm(self, unit: str, text: str) -> Path:
        path = self.directory / f"generated/asm/{unit}.s"
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_text(text, encoding="utf-8")
        return path

    def rewrite_receipt(self) -> None:
        manifest = input_manifest(self.directory / "source")
        (self.directory / "input-manifest.json").write_text(json.dumps(manifest), encoding="utf-8")
        self.receipt = {
            "schema_version": 3, "target_sha256": EXPECTED_SHA256, "input_manifest": manifest,
            "profile": self.profile, "runtime": self.runtime, "graph": self.graph,
            "image_inventory": {**self.image_report(), "linked_images": {}},
            "c_matches": self.matches, "generated": file_inventory(self.directory / "generated"),
            "artifacts": {"shiren2.ld": sha256(self.directory / "shiren2.ld")},
        }
        self.receipt_path.write_text(json.dumps(self.receipt), encoding="utf-8")

    def catalogue(self) -> packet.FunctionCatalogue:
        return packet.enumerate_functions(self.receipt_path, reference=self.reference)

    def output(self, name: str = "packet") -> Path:
        return self.root / "scratch" / name

    def test_frozen_enumeration_is_image_qualified_and_calls_are_byte_decoded(self) -> None:
        catalogue = self.catalogue()
        self.assertEqual(set(catalogue), {("resident", "sdk"), ("main_14400", "game")})
        sdk, game = catalogue[("resident", "sdk")], catalogue[("main_14400", "game")]
        self.assertEqual(sdk.size, 8)
        self.assertEqual(game.size, 16)
        self.assertEqual(game.direct_calls[0].target_vram, sdk.vram_start)
        self.assertEqual(game.direct_calls[0].possible_targets, (sdk.key,))
        self.assertEqual(sdk.callers, (game.key,))
        self.assertFalse(sdk.already_accepted)
        self.assertIsNone(game.profile_id)
        self.verifier.assert_called_once_with(self.receipt_path, require_current=False, reference=self.reference)
        with self.assertRaises(TypeError):
            catalogue.functions[sdk.key] = game

    def test_duplicate_labels_and_missing_or_mismatched_end_labels_fail(self) -> None:
        original = self.assembly("sdk", 0x1000, 0x80025C00, self.sdk_words)
        for text in (original + original, original.replace("endlabel sdk", "endlabel other"), original.replace("endlabel sdk", ""), original.replace("endlabel sdk", "glabel nested\nendlabel sdk")):
            with self.subTest(text=text):
                self.write_asm("sdk", text)
                self.rewrite_receipt()
                with self.assertRaises(ValueError):
                    self.catalogue()

    def test_jump_table_labels_keep_function_extents(self) -> None:
        # Rodata split emits `jlabel .Lxxxxxxxx` case targets inside a function, and
        # `jlabel func_X` when a catalogued function start is itself a case target.
        original = self.assembly("sdk", 0x1000, 0x80025C00, self.sdk_words)
        lines = original.splitlines(keepends=True)
        case_label = lines[:4] + ["jlabel .L80025C08\n"] + lines[4:]
        as_start = original.replace("glabel sdk", "jlabel sdk")
        for text in ("".join(case_label), as_start):
            with self.subTest(text=text):
                self.write_asm("sdk", text)
                self.rewrite_receipt()
                sdk = next(function for function in self.catalogue().values() if function.symbol == "sdk")
                self.assertEqual((sdk.rom_start, sdk.size), (0x1000, 4 * len(self.sdk_words)))

    def test_duplicate_and_overlapping_function_identities_across_units_fail(self) -> None:
        for symbol in ("sdk", "different"):
            with self.subTest(symbol=symbol):
                self.write_asm("extra", self.assembly(symbol, 0x1000, 0x80025C00, self.sdk_words))
                self.graph["obj/generated/asm/extra.o"] = {"image_id": "resident", "input": "generated/asm/extra.s", "source": "generated/asm/extra.s", "source_kind": "asm"}
                self.rewrite_receipt()
                with self.assertRaisesRegex(ValueError, "Duplicate|Overlapping"):
                    self.catalogue()

    def test_same_symbol_in_distinct_images_is_retained_with_explicit_identity(self) -> None:
        self.write_asm("game", self.assembly("sdk", 0x2000, 0x800413C0, self.game_words))
        self.rewrite_receipt()
        self.assertEqual(set(self.catalogue()), {("resident", "sdk"), ("main_14400", "sdk")})

    def test_noncontiguous_rom_or_vram_wrong_mapping_and_wrong_words_fail(self) -> None:
        original = self.assembly("sdk", 0x1000, 0x80025C00, self.sdk_words)
        mutations = [
            original.replace("1004 80025C04", "1008 80025C04"),
            original.replace("1004 80025C04", "1004 80025C08"),
            original.replace("80025C00", "80025C20").replace("80025C04", "80025C24"),
            original.replace("03E00008", "03E00009"),
            original.replace("jr $ra", ".word 0x03E00008"),
            original.replace("endlabel sdk", "nop\nendlabel sdk"),
        ]
        for text in mutations:
            with self.subTest(text=text):
                self.write_asm("sdk", text)
                self.rewrite_receipt()
                with self.assertRaises(ValueError):
                    self.catalogue()

    def test_pinned_splat_internal_alias_is_preserved_without_changing_extent(self) -> None:
        text = self.assembly("sdk", 0x1000, 0x80025C00, self.sdk_words)
        text = text.replace("/* 1004", "  alabel D_internal\n/* 1004")
        self.write_asm("sdk", text)
        self.rewrite_receipt()
        function = self.catalogue()[("resident", "sdk")]
        self.assertEqual(function.internal_aliases, ("D_internal",))
        self.assertEqual(function.size, 8)
        self.assertIn("  alabel D_internal", function.assembly)

    def test_malformed_duplicate_or_function_colliding_internal_aliases_fail(self) -> None:
        original = self.assembly("sdk", 0x1000, 0x80025C00, self.sdk_words)
        for aliases in ("alabel bad.name", "alabel D_internal, extra", "alabel D_internal\nalabel D_internal", "alabel sdk"):
            with self.subTest(aliases=aliases):
                self.write_asm("sdk", original.replace("/* 1004", aliases + "\n/* 1004"))
                self.rewrite_receipt()
                with self.assertRaisesRegex(ValueError, "alias"):
                    self.catalogue()

    def test_graph_wrong_image_source_escape_and_missing_inventory_fail(self) -> None:
        self.graph["obj/generated/asm/sdk.o"]["image_id"] = "unknown_image"
        self.rewrite_receipt()
        with self.assertRaisesRegex(ValueError, "declared original image"):
            self.catalogue()
        self.graph["obj/generated/asm/sdk.o"]["image_id"] = "main_14400"
        self.rewrite_receipt()
        with self.assertRaisesRegex(ValueError, "image"):
            self.catalogue()
        self.graph["obj/generated/asm/sdk.o"]["image_id"] = "resident"
        self.graph["obj/generated/asm/sdk.o"]["input"] = "generated/asm/../../outside.s"
        self.rewrite_receipt()
        with self.assertRaises(ValueError):
            self.catalogue()
        self.graph["obj/generated/asm/sdk.o"]["input"] = "generated/asm/sdk.s"
        self.rewrite_receipt()
        del self.receipt["generated"]["asm/sdk.s"]
        self.receipt_path.write_text(json.dumps(self.receipt), encoding="utf-8")
        with self.assertRaisesRegex(ValueError, "inventory"):
            self.catalogue()

    def test_confirmed_rsp_overlap_is_excluded_from_cpu_references(self) -> None:
        component = ComponentRecord("rsp", "resident", "rsp", 0x1000, 0x1008, 0x80025C00, 0x80025C08, 0, 8, "0" * 64, True, "reviewed-component-extent", (), "fixture")
        self.inventory = replace(self.inventory, components=(component,))
        with self.assertRaisesRegex(ValueError, "non-CPU"):
            self.catalogue()

    def test_handwritten_annotation_is_a_hint_without_blanket_enumeration_filter(self) -> None:
        self.write_asm("sdk", "/* Handwritten function */\n" + self.assembly("sdk", 0x1000, 0x80025C00, self.sdk_words))
        self.rewrite_receipt()
        function = self.catalogue()[("resident", "sdk")]
        self.assertTrue(function.handwritten_annotation)
        self.assertFalse(function.already_accepted)

    def test_accepted_c_diagnostic_reference_has_explicit_source_and_profile(self) -> None:
        self.graph.pop("obj/generated/asm/sdk.o")
        self.graph["obj/source/src/sdk.o"] = {"image_id": "resident", "input": "source/src/sdk.c", "source": "src/sdk.c", "source_kind": "c"}
        path = self.directory / "source/src/sdk.c"
        path.parent.mkdir()
        path.write_text("unsigned sdk(void) { return 0; }\n", encoding="utf-8")
        text = (self.directory / "generated/asm/sdk.s").read_text()
        (self.directory / "generated/asm/sdk.s").unlink()
        matching = self.directory / "generated/asm/matchings/sdk/sdk.s"
        matching.parent.mkdir(parents=True)
        matching.write_text(text, encoding="utf-8")
        self.matches = [{"image_id": "resident", "symbol": "sdk", "source": "src/sdk.c", "rom_start": 0x1000, "vram_start": 0x80025C00, "size": 8}]
        self.profile["tu_profiles"]["src/sdk.c"] = "candidate"
        self.rewrite_receipt()
        function = self.catalogue()[("resident", "sdk")]
        self.assertTrue(function.already_accepted)
        self.assertEqual(function.accepted_source, "src/sdk.c")
        self.assertEqual(function.profile_id, "candidate")

    def test_packet_copies_frozen_context_and_original_bytes_with_zero_c_credit(self) -> None:
        catalogue = self.catalogue()
        destination = packet.create_packet(catalogue, "resident", "sdk", self.output(), run_m2c=False, editable_files=("src/tasks/sdk.c",))
        metadata = json.loads((destination / "task.json").read_text())
        self.assertEqual(metadata["status"], "prepared-unverified")
        self.assertEqual(metadata["baseline"]["receipt_sha256"], catalogue.baseline.receipt_sha256)
        self.assertEqual(metadata["candidate"]["c_credit_bytes"], 0)
        self.assertEqual(metadata["boundary"]["confidence"], "provisional")
        self.assertFalse(metadata["m2c"]["invoked"])
        self.assertEqual((destination / "expected.bin").read_bytes(), catalogue[("resident", "sdk")].instruction_bytes)
        header = (destination / "context/include/types.h").read_bytes()
        self.assertEqual(metadata["context"]["files"]["include/types.h"]["sha256"], hashlib.sha256(header).hexdigest())
        self.assertEqual(metadata["ownership"]["worker_source_files"], ["src/tasks/sdk.c"])
        packet.create_packet(catalogue, "main_14400", "game", self.output("other"), run_m2c=False)
        self.assertEqual(self.verifier.call_count, 1)

    def test_output_collision_escape_and_symlink_parents_are_rejected(self) -> None:
        catalogue = self.catalogue()
        outside = self.root / "outside"
        with self.assertRaises(ValueError):
            packet.create_packet(catalogue, "resident", "sdk", outside, run_m2c=False)
        with self.assertRaises(ValueError):
            packet.create_packet(catalogue, "resident", "sdk", self.root / "scratch/../outside", run_m2c=False)
        self.output().mkdir(parents=True)
        with self.assertRaises(FileExistsError):
            packet.create_packet(catalogue, "resident", "sdk", self.output(), run_m2c=False)
        link = self.root / "scratch/link"
        link.symlink_to(self.root, target_is_directory=True)
        with self.assertRaises(ValueError):
            packet.create_packet(catalogue, "resident", "sdk", link / "nested/packet", run_m2c=False)
        self.assertFalse((self.root / "nested").exists())

    def test_invalid_worker_source_ownership_is_rejected(self) -> None:
        catalogue = self.catalogue()
        for owned in (("../outside.c",), ("include/type.h",), ("src/file.c", "src/file.c")):
            with self.subTest(owned=owned):
                with self.assertRaises(ValueError):
                    packet.create_packet(catalogue, "resident", "sdk", self.output(), run_m2c=False, editable_files=owned)

    def test_stale_catalogue_receipt_generated_source_and_runtime_fail_before_packet(self) -> None:
        for mutation in ("receipt", "assembly", "header", "runtime"):
            with self.subTest(mutation=mutation):
                self.rewrite_receipt()
                catalogue = self.catalogue()
                if mutation == "receipt":
                    self.receipt_path.write_text(self.receipt_path.read_text() + "\n")
                elif mutation == "assembly":
                    path = self.directory / "generated/asm/sdk.s"
                    path.write_text(path.read_text() + "\n")
                elif mutation == "header":
                    path = self.directory / "source/include/types.h"
                    path.write_text("changed header\n")
                else:
                    self.runtime["changed"] = True
                with self.assertRaises(ValueError):
                    packet.create_packet(catalogue, "resident", "sdk", self.output(mutation), run_m2c=False)
                self.assertFalse(self.output(mutation).exists())

    def test_m2c_success_or_failure_is_always_unverified_output(self) -> None:
        catalogue = self.catalogue()
        for returncode in (0, 2):
            with self.subTest(returncode=returncode):
                with patch.object(packet.subprocess, "run", return_value=subprocess.CompletedProcess([], returncode, "? sdk(void);\n", "fixture diagnostic\n")):
                    destination = packet.create_packet(catalogue, "resident", "sdk", self.output(f"m2c-{returncode}"))
                metadata = json.loads((destination / "task.json").read_text())
                self.assertEqual(metadata["m2c"]["returncode"], returncode)
                self.assertEqual(metadata["candidate"]["status"], "unverified")
                self.assertEqual(metadata["candidate"]["c_credit_bytes"], 0)
                self.assertTrue(metadata["candidate"]["contains_unknown_type_marker"])

    def test_baseline_race_during_m2c_retains_failed_packet_without_acceptance(self) -> None:
        catalogue = self.catalogue()

        def racing_run(*args: object, **kwargs: object):
            path = self.directory / "generated/asm/sdk.s"
            path.write_text(path.read_text() + "\n")
            return subprocess.CompletedProcess([], 0, "candidate\n", "")

        with patch.object(packet.subprocess, "run", side_effect=racing_run):
            with self.assertRaisesRegex(ValueError, "generated reference inventory changed"):
                packet.create_packet(catalogue, "resident", "sdk", self.output())
        failure = json.loads((self.output() / "failure.json").read_text())
        self.assertEqual(failure["status"], "invalidated")
        self.assertFalse((self.output() / "task.json").exists())

    def test_race_after_task_write_invalidates_metadata(self) -> None:
        catalogue = self.catalogue()
        original_write = packet._write_exclusive

        def racing_write(path: Path, content: bytes) -> None:
            original_write(path, content)
            if path.name == "task.json":
                self.receipt_path.write_text(self.receipt_path.read_text() + "\n")

        with patch.object(packet, "_write_exclusive", side_effect=racing_write):
            with self.assertRaisesRegex(ValueError, "receipt changed"):
                packet.create_packet(catalogue, "resident", "sdk", self.output(), run_m2c=False)
        self.assertEqual(json.loads((self.output() / "task.json").read_text())["status"], "invalidated")

    def test_corrupted_packet_reference_during_write_fails_required_inventory(self) -> None:
        catalogue = self.catalogue()
        original_write = packet._write_exclusive

        def corrupting_write(path: Path, content: bytes) -> None:
            original_write(path, content)
            if path.name == "target.s":
                path.write_bytes(content + b"corrupted\n")

        with patch.object(packet, "_write_exclusive", side_effect=corrupting_write):
            with self.assertRaisesRegex(ValueError, "required output inventory changed"):
                packet.create_packet(catalogue, "resident", "sdk", self.output(), run_m2c=False)

    def test_modified_task_metadata_after_write_is_not_self_certifying(self) -> None:
        catalogue = self.catalogue()
        original_write = packet._write_exclusive

        def corrupting_write(path: Path, content: bytes) -> None:
            original_write(path, content)
            if path.name == "task.json":
                path.write_bytes(content + b"\n")

        with patch.object(packet, "_write_exclusive", side_effect=corrupting_write):
            with self.assertRaisesRegex(ValueError, "output inventory changed after metadata write"):
                packet.create_packet(catalogue, "resident", "sdk", self.output(), run_m2c=False)
        self.assertEqual(json.loads((self.output() / "task.json").read_text())["status"], "invalidated")

    def test_nested_output_context_symlink_cannot_redirect_a_write(self) -> None:
        catalogue = self.catalogue()
        original_write = packet._write_exclusive
        outside = self.root / "outside"
        outside.mkdir()

        def redirecting_write(path: Path, content: bytes) -> None:
            original_write(path, content)
            if path.name == "target.s":
                context = self.output() / "context"
                context.mkdir()
                (context / "config").symlink_to(outside, target_is_directory=True)

        with patch.object(packet, "_write_exclusive", side_effect=redirecting_write):
            with self.assertRaisesRegex(ValueError, "symlink or non-directory"):
                packet.create_packet(catalogue, "resident", "sdk", self.output(), run_m2c=False)
        self.assertEqual(list(outside.iterdir()), [])


if __name__ == "__main__":
    unittest.main()
