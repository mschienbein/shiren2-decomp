"""Property-replayed research partition; never equate research with accepted source."""
from __future__ import annotations

import copy
from collections import Counter
import hashlib
import json
import os
import sys
import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch

PROJECT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(PROJECT / "tools"))
import rommap
from images import load_inventory
from rom import EXPECTED_SIZE

PRIVATE_ROM = Path(os.environ.get("H21_TEST_ROM", str(PROJECT.parent / "inputs/r1-build/shiren2.z64")))
CATALOGUE = PROJECT / rommap.CATALOGUE_PATH


class UnownedCpuGapTests(unittest.TestCase):
    def test_gap_and_tail_are_open_without_producer_evidence(self) -> None:
        pieces = list(rommap._cpu_text_pieces(0, 20, [(4, 8), (12, 16)]))
        self.assertEqual(pieces, [
            (0, 4, "cpu_text_unowned", "catalogue"),
            (4, 8, "cpu_text_function", "catalogue"),
            (8, 12, "cpu_text_unowned", "catalogue"),
            (12, 16, "cpu_text_function", "catalogue"),
            (16, 20, "cpu_text_unowned", "catalogue"),
        ])
        self.assertEqual(rommap.research_status("cpu_text_unowned"), "open")

    def test_only_witnessed_exact_intervals_become_cpu_padding(self) -> None:
        pieces = list(rommap._cpu_text_pieces(0, 20, [(4, 8), (12, 16)], {(8, 12): "pad_object_rounding"}))
        self.assertEqual(pieces, [
            (0, 4, "cpu_text_unowned", "catalogue"),
            (4, 8, "cpu_text_function", "catalogue"),
            (8, 12, "cpu_text_padding", "pad_object_rounding"),
            (12, 16, "cpu_text_function", "catalogue"),
            (16, 20, "cpu_text_unowned", "catalogue"),
        ])
        self.assertEqual(list(rommap._cpu_text_pieces(0, 20, [(4, 8)], {(8, 12): "pad_object_rounding"}))[-1],
                         (8, 20, "cpu_text_unowned", "catalogue"))
        self.assertEqual(rommap.research_status("cpu_text_padding"), "closed")
        self.assertNotIn("rsp_ucode_unresolved", rommap.CLASSES)
        self.assertEqual(rommap.CLASSES["cpu_text_padding"], "padding")

    def test_unsupported_padding_class_is_rejected(self) -> None:
        with self.assertRaises(rommap.MapError):
            rommap.research_status("cpu_text_alignment")


@unittest.skipUnless(PRIVATE_ROM.is_file(), "Private copied original required")
class RomMapTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls) -> None:
        cls.original = PRIVATE_ROM.read_bytes()
        cls.inventory = load_inventory(PROJECT / "config/images.json")
        cls.rules_bytes = (PROJECT / rommap.RULES_PATH).read_bytes()
        cls.rules = json.loads(cls.rules_bytes)
        cls.document = json.loads((PROJECT / rommap.MAP_PATH).read_text())
        cls.rom_map = rommap.load(PROJECT, cls.original)
        cls.evidence = rommap.load_evidence(cls.original, cls.rules)

    def validate(self, document: dict, rules: dict | None = None) -> rommap.RomMap:
        return rommap.validate(document, self.original, self.inventory, rules or self.rules,
                               rules_sha256=hashlib.sha256(self.rules_bytes).hexdigest())

    def index(self, kind: str) -> int:
        return next(i for i, row in enumerate(self.document["ranges"]) if row[2] == kind)

    def rehash(self, document: dict) -> dict:
        """Recompute the identities of an edited map, keeping its split ledger."""
        return rommap._document(self.original, document["ranges"],
                                rules_sha256=document["rules_sha256"],
                                catalogue_id=document["cpu_catalogue_id"],
                                catalogue=document["cpu_catalogue"], splits=document["splits"])

    def account(self, matches: list[dict] | None = None, root: Path = PROJECT, build_directory=None) -> dict:
        return rommap.account(self.rom_map, self.original, root, matches or [], self.rules, build_directory)

    def match(self, start: int, end: int, source: str = "src/x.c") -> dict:
        identity = json.loads(CATALOGUE.read_text())["identity"]
        image_id = next((f["image_id"] for f in identity["functions"]
                         if f["rom_start"] <= start < f["rom_start"] + f["size"]), "outside_cpu")
        return {"image_id": image_id, "symbol": f"func_{start:x}", "source": source, "rom_start": start, "size": end - start}

    def test_frozen_map_tiles_and_retains_cpu_scope(self) -> None:
        ranges = self.rom_map.ranges
        self.assertEqual(sum(e - s for s, e, *_ in ranges), EXPECTED_SIZE)
        self.assertTrue(all(a[1] == b[0] for a, b in zip(ranges, ranges[1:])))
        self.assertEqual(sum(e - s for s, e, *_ in self.rom_map.of_class("cpu_text_function")),
                         sum(f["size"] for f in json.loads(CATALOGUE.read_text())["identity"]["functions"]))

    def test_unknown_cpu_ranges_remain_open_in_public_accounting(self) -> None:
        report = self.account()
        expected = [
            {"start": start, "end": end, "class": kind, "ref": ref, "status": "open"}
            for start, end, kind, ref in self.rom_map.of_class("cpu_text_unowned")
        ]
        actual = [row for row in report["range_statuses"] if row["class"] == "cpu_text_unowned"]
        self.assertEqual(actual, expected)
        opaque_bytes = sum(
            end - start
            for start, end, *_ in self.rom_map.of_class("cpu_text_unowned", "image_opaque")
        )
        self.assertEqual(report["status_bytes"]["cpu_image_opaque:open"], opaque_bytes)

    def test_generation_never_decodes_or_recompresses_inherited_streams(self) -> None:
        import crunch64
        with patch.object(crunch64.yay0, "decompress", side_effect=AssertionError("no new decode")), \
                patch.object(crunch64.yay0, "compress", side_effect=AssertionError("no new encode")):
            generated = rommap.generate(self.original, self.inventory, self.document["cpu_catalogue"], self.rules,
                                        catalogue_id=self.document["cpu_catalogue_id"],
                                        rules_sha256=hashlib.sha256(self.rules_bytes).hexdigest())
        self.assertEqual(generated, self.document)

    def test_historical_component_absence_cannot_generate_current_rsp_classification(self) -> None:
        from dataclasses import replace
        historical = replace(self.inventory, components=tuple(
            part for part in self.inventory.components if part.component_id == "rsp_boot"))
        with self.assertRaisesRegex(rommap.MapError, "complete reviewed RSP components"):
            rommap.generate(self.original, historical, self.document["cpu_catalogue"], self.rules,
                            catalogue_id=self.document["cpu_catalogue_id"],
                            rules_sha256=hashlib.sha256(self.rules_bytes).hexdigest())

    @unittest.skipUnless(CATALOGUE.is_file(), "Frozen CPU catalogue required")
    def test_catalogue_hook_remains_exact(self) -> None:
        rommap.check_catalogue(self.rom_map, json.loads(CATALOGUE.read_text()))

    def test_partition_and_policy_identities_are_independent(self) -> None:
        for key in ("partition_sha256", "evidence_policy_sha256", "map_id", "content_sha256"):
            document = copy.deepcopy(self.document)
            document[key] = "0" * 64
            with self.subTest(key=key), self.assertRaises(rommap.MapError):
                self.validate(document)
        changed = rommap._document(self.original, self.document["ranges"], rules_sha256="0" * 64,
                                   catalogue_id=self.document["cpu_catalogue_id"], catalogue=self.document["cpu_catalogue"])
        self.assertEqual(changed["partition_sha256"], self.document["partition_sha256"])
        self.assertNotEqual(changed["evidence_policy_sha256"], self.document["evidence_policy_sha256"])
        rows = copy.deepcopy(self.document["ranges"])
        rows[self.index("asset_raw_referenced")][2] = "asset_audio_bank_ptrtable"
        changed = rommap._document(self.original, rows, rules_sha256=self.document["rules_sha256"],
                                   catalogue_id=self.document["cpu_catalogue_id"], catalogue=self.document["cpu_catalogue"])
        self.assertEqual(changed["partition_sha256"], self.document["partition_sha256"])
        self.assertNotEqual(changed["evidence_policy_sha256"], self.document["evidence_policy_sha256"])

    def test_rehashed_forged_format_and_structural_flags_fail(self) -> None:
        cases = [("asset_raw_referenced", "asset_audio_bank_ptrtable"),
                 ("asset_iff_payload", "asset_iff_container_meta"),
                 ("pad_align_zero", "pad_align_ff"), ("cpu_data", "cpu_text_unowned"),
                 ("asset_yay0", "asset_raw_referenced")]
        for before, after in cases:
            document = copy.deepcopy(self.document)
            document["ranges"][self.index(before)][2] = after
            with self.subTest(before=before, after=after), self.assertRaises(rommap.MapError):
                self.validate(self.rehash(document))
        document = copy.deepcopy(self.document)
        document["ranges"][self.index("cpu_data")].append({"structural_fill": True, "format": {"state": "proven"}})
        with self.assertRaises(rommap.MapError):
            self.validate(document)

    def test_rehashed_unowned_cpu_gap_cannot_be_reclosed_as_padding(self) -> None:
        for kind, ref in (("pad_align_zero", "align"), ("rom_padding_zero", "boot"),
                          ("cpu_text_padding", "pad_object_rounding")):
            document = copy.deepcopy(self.document)
            row = document["ranges"][self.index("cpu_text_unowned")]
            row[2:4] = kind, ref
            with self.subTest(kind=kind), self.assertRaises(rommap.MapError):
                self.validate(self.rehash(document))

    def padding_document(self) -> dict:
        return copy.deepcopy(self.evidence[rommap.CPU_PADDING_INPUT])

    def replay_padding(self, document: dict, root: Path = PROJECT, original: bytes | None = None) -> dict:
        return rommap.cpu_padding(original or self.original, self.document["cpu_catalogue"],
                                  {rommap.CPU_PADDING_INPUT: document}, root)

    def test_cpu_padding_rows_are_exactly_the_replayed_witnesses(self) -> None:
        replayed = rommap.cpu_padding(self.original, self.document["cpu_catalogue"], self.evidence)
        rows = self.rom_map.of_class("cpu_text_padding")
        self.assertEqual({(a, b): ref for a, b, _, ref in rows}, replayed)
        self.assertTrue(all(not self.original[a:b].strip(b"\0") for a, b, *_ in rows))
        report = self.account()
        closed = [row for row in report["range_statuses"] if row["class"] == "cpu_text_padding"]
        self.assertEqual({row["status"] for row in closed}, {"closed"})
        self.assertEqual(sum(row["end"] - row["start"] for row in closed), sum(b - a for a, b in replayed))
        with self.assertRaises(rommap.MapError):
            self.account([self.match(rows[0][0], rows[0][1])])

    def test_cpu_padding_witness_tamper_fails_closed(self) -> None:
        document = self.padding_document()
        first = document["ranges"][0]
        changed = bytearray(self.original)
        changed[first["start"]] = 1
        with self.assertRaises(rommap.MapError):
            self.replay_padding(document, original=bytes(changed))
        mutants = []
        longer = copy.deepcopy(document)
        longer["ranges"][0]["end"] += 4
        longer["ranges"][0]["parts"][-1]["end"] += 4
        mutants.append(("end", longer))
        tier = copy.deepcopy(document)
        tier["ranges"][0]["tier"] = "geometry_only"
        mutants.append(("tier", tier))
        raw = copy.deepcopy(document)
        relocated = next(r for r in raw["ranges"] if r["parts"][0].get("identity") == "nonreloc")
        relocated["parts"][0]["identity"] = "raw"
        mutants.append(("identity", raw))
        unwitnessed = copy.deepcopy(document)
        fill = next(p for r in unwitnessed["ranges"] for p in r["parts"] if p["kind"] == "alignment_fill")
        unwitnessed["ranges"] = [r for r in unwitnessed["ranges"]
                                 if not any(p.get("object_rom_start") == fill["next_object"]["object_rom_start"]
                                            and p["kind"] == "sdk_section" for p in r["parts"])]
        mutants.append(("next object", unwitnessed))
        duplicate = copy.deepcopy(document)
        duplicate["ranges"].append(copy.deepcopy(duplicate["ranges"][0]))
        mutants.append(("duplicate", duplicate))
        zero_object = copy.deepcopy(document)
        composite = next(r for r in zero_object["ranges"] if any(p["kind"] == "alignment_fill" for p in r["parts"]))
        composite["parts"][-1] = {"start": composite["parts"][-1]["start"], "end": composite["end"], "kind": "zero_object",
                                  "object": composite["parts"][0]["object"],
                                  "object_rom_start": composite["parts"][-1]["start"]}
        mutants.append(("size-only zero object", zero_object))
        for name, mutant in mutants:
            with self.subTest(name=name), self.assertRaises(rommap.MapError):
                self.replay_padding(mutant)

    def test_cpu_padding_witness_objects_are_content_addressed(self) -> None:
        document = self.padding_document()
        pin = document["ranges"][0]["parts"][0]["object"]
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            directory = root / rommap.CPU_PADDING_DIR
            directory.mkdir(parents=True)
            for path in (PROJECT / rommap.CPU_PADDING_DIR).iterdir():
                (directory / path.name).write_bytes(path.read_bytes())
            target = directory / f"{pin['sha256']}.o"
            data = bytearray(target.read_bytes())
            data[-1] ^= 1
            target.write_bytes(bytes(data))
            with self.assertRaisesRegex(rommap.MapError, "identity"):
                self.replay_padding(document, root)

    def test_embedded_toolchain_object_without_rounding_cannot_witness(self) -> None:
        document = self.padding_document()
        record = document["ranges"][0]
        pin = record["parts"][0]["object"]
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            directory = root / rommap.CPU_PADDING_DIR
            directory.mkdir(parents=True)
            data = bytearray((PROJECT / rommap.CPU_PADDING_DIR / f"{pin['sha256']}.o").read_bytes())
            shoff = int.from_bytes(data[0x20:0x24], "big")
            entsize, count = int.from_bytes(data[0x2E:0x30], "big"), int.from_bytes(data[0x30:0x32], "big")
            names = int.from_bytes(data[0x32:0x34], "big")
            base = int.from_bytes(data[shoff + names * entsize + 16:shoff + names * entsize + 20], "big")
            for index in range(count):
                header = shoff + index * entsize
                name = int.from_bytes(data[header:header + 4], "big")
                if data[base + name:base + name + 6] == b".text\0":
                    data[header + 32:header + 36] = (4).to_bytes(4, "big")
            digest = hashlib.sha256(data).hexdigest()
            (directory / f"{digest}.o").write_bytes(bytes(data))
            record["parts"][0]["object"] = {"path": f"{rommap.CPU_PADDING_DIR}/{digest}.o", "sha256": digest}
            with self.assertRaisesRegex(rommap.MapError, "alignment"):
                self.replay_padding(document, root)

    def test_only_an_unreferenced_local_end_label_may_sit_at_the_code_end(self) -> None:
        document = self.padding_document()
        record = next(r for r in document["ranges"] if r["tier"] == "witness_assembler_reproduction"
                      and r["parts"][0]["producer"].get("profile", "").endswith("-asg"))
        pin = record["parts"][0]["object"]
        data = (PROJECT / rommap.CPU_PADDING_DIR / f"{pin['sha256']}.o").read_bytes()
        self.replay_padding(dict(document, ranges=[record]))
        shoff = int.from_bytes(data[0x20:0x24], "big")
        entsize, count = int.from_bytes(data[0x2E:0x30], "big"), int.from_bytes(data[0x30:0x32], "big")
        headers = [data[shoff + i * entsize:shoff + (i + 1) * entsize] for i in range(count)]
        names = int.from_bytes(headers[int.from_bytes(data[0x32:0x34], "big")][16:20], "big")
        text = next(i for i, h in enumerate(headers)
                    if data[names + int.from_bytes(h[0:4], "big"):].startswith(b".text\0"))
        symtab = next(h for h in headers if int.from_bytes(h[4:8], "big") == 2)
        offset, size = int.from_bytes(symtab[16:20], "big"), int.from_bytes(symtab[20:24], "big")
        code = record["parts"][0]["start"] - record["parts"][0]["object_rom_start"]
        site = next(offset + 16 * k for k in range(size // 16)
                    if int.from_bytes(data[offset + 16 * k + 4:offset + 16 * k + 8], "big") == code
                    and int.from_bytes(data[offset + 16 * k + 14:offset + 16 * k + 16], "big") == text)
        changed = bytearray(data)
        changed[site + 12] = 0x10 | changed[site + 12] & 0xF  # make the end label global
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            directory = root / rommap.CPU_PADDING_DIR
            directory.mkdir(parents=True)
            digest = hashlib.sha256(changed).hexdigest()
            (directory / f"{digest}.o").write_bytes(bytes(changed))
            record["parts"][0]["object"] = {"path": f"{rommap.CPU_PADDING_DIR}/{digest}.o", "sha256": digest}
            with self.assertRaisesRegex(rommap.MapError, "rounding"):
                self.replay_padding(dict(document, ranges=[record]), root)

    def test_omitting_nonzero_function_cannot_shrink_cpu_catalogue(self) -> None:
        catalogue = copy.deepcopy(self.document["cpu_catalogue"])
        omitted = next(row for row in catalogue["functions"]
                       if any(self.original[row["rom_start"]:row["rom_start"] + row["size"]]))
        catalogue["functions"].remove(omitted)
        with self.assertRaises(rommap.MapError):
            rommap.generate(self.original, self.inventory, catalogue, self.rules,
                            catalogue_id=rommap.json_digest(catalogue),
                            rules_sha256=hashlib.sha256(self.rules_bytes).hexdigest())

    def test_zero_cpu_data_is_not_padding(self) -> None:
        zero_row = next(row for row in self.rom_map.ranges
                        if row[2] == "cpu_data" and b"\0" * 4 in self.original[row[0]:row[1]])
        self.assertEqual(rommap.research_status(zero_row[2]), "open")
        a, b, kind, ref = zero_row
        zero = a + self.original[a:b].index(b"\0" * 4)
        self.assertEqual(self.original[zero:zero + 4], b"\0" * 4)
        document = copy.deepcopy(self.document)
        index = next(i for i, row in enumerate(document["ranges"]) if row[0] == a)
        document["ranges"][index:index + 1] = ([[a, zero, kind, ref]] if a < zero else []) + [
            [zero, zero + 4, "pad_align_zero", "align"]
        ] + ([[zero + 4, b, kind, ref]] if zero + 4 < b else [])
        with self.assertRaises(rommap.MapError):
            self.validate(self.rehash(document))
        with self.assertRaises(rommap.MapError):
            rommap.research_status("pad_zero_block")

    def test_gap_overlap_unknown_class_and_status_fail_closed(self) -> None:
        for index, value in ((0, 1), (1, EXPECTED_SIZE), (2, "roundtrip"), (3, "proven")):
            document = copy.deepcopy(self.document)
            document["ranges"][self.index("asset_yay0")][index] = value
            with self.subTest(index=index), self.assertRaises(rommap.MapError):
                self.validate(document)
        document = copy.deepcopy(self.document)
        document["status"] = "classified_handwritten"
        with self.assertRaises(rommap.MapError):
            self.validate(document)

    def test_loader_geometry_numerical_non_rom_bases_and_instruction_tamper(self) -> None:
        for key, field, value in (("code_bases", "site", 0x3C114),
                                  ("immediate_windows", "start_site", 0x4FF50),
                                  ("inferred_windows", "end_rom", 0x16DC18),
                                  ("tables", "record_size", 8)):
            rules = copy.deepcopy(self.rules)
            rules[key][0][field] = value
            with self.subTest(key=key), self.assertRaises(rommap.MapError):
                rommap.rule_windows(rules, self.original)
        original = bytearray(self.original)
        original[0x32234] ^= 1
        with self.assertRaisesRegex(rommap.MapError, "instruction"):
            rommap.rule_windows(self.rules, bytes(original))
        windows = rommap.rule_windows(self.rules, self.original)
        self.assertTrue(any(w.start == 0x157460 and w.end == 0x1575C0 and w.explicit for w in windows))
        self.assertTrue(all(not w.explicit for w in windows if w.ref == "T5"))
        self.assertFalse({0x16DC18, 0x16DC98, 0x1CE000, 0x1EC080, 0x1F0084} & {r[0] for r in self.rom_map.ranges})

    def test_sprite_palettes_come_from_original_operands_and_table_words(self) -> None:
        windows = rommap.rule_windows(self.rules, self.original)
        palettes = [w for w in windows if w.ref == "S_sprite_palette"]
        self.assertEqual(len(palettes), 756)
        self.assertTrue(all(w.end - w.start == 32 and w.explicit for w in palettes))
        self.assertTrue(any((w.start, w.end, w.ref) == (0xE7BE70, 0xE7BE90, "N_palette_e7be70") for w in windows))
        rows = [r for r in self.rom_map.ranges if r[3] == "S_sprite_palette"]
        self.assertEqual(len(rows), 756)
        self.assertEqual({r[2] for r in rows}, {"asset_raw_referenced"})
        for site, opcode in ((0x44250, 0x0A), (0x45120, 0x09)):  # addiu, not slti; jal, not addiu
            with self.subTest(site=hex(site)), self.assertRaisesRegex(rommap.MapError, "expected instruction"):
                rommap.instruction_immediate(self.original, site, opcode, base=0 if opcode == 0x09 else None)
        changed = bytearray(self.original)
        changed[0xE55730] ^= 0x01  # segment nibble of the first palette table word (0x06000000)
        with self.assertRaisesRegex(rommap.MapError, "not segment 6"):
            rommap.rule_windows(self.rules, bytes(changed))

    def test_audio_binary_identity_closes_only_sdk_sections(self) -> None:
        result = self.account()
        self.assertNotIn("rsp_ucode:open", result["status_bytes"])
        self.assertEqual(result["status_bytes"]["rsp_ucode:classified_binary"], 0xD0 + 0x1390 + 0x420 + 0xC60 + 0x2B0)
        self.assertEqual(result["build_source_bytes"], 0)
        # The word after the SDK audio .data is ordinary main-image CPU data and stays open.
        boundary = next(row for row in result["range_statuses"] if row["start"] <= 0x11D770 < row["end"])
        self.assertEqual((boundary["class"], boundary["ref"], boundary["status"]), ("cpu_data", "main_14400", "open"))
        self.assertGreaterEqual(boundary["end"], 0x11D774)
        for start, end in ((0x10AEB0, 0x10BB10), (0x11D4C0, 0x11D770)):
            with self.subTest(start=start), self.assertRaises(rommap.MapError):
                self.account([self.match(start, end)])
        interval = copy.deepcopy(self.evidence["audio-intervals.json"]["intervals"][0])
        for key, value in (("end", "0x010bb10"), ("md5", "9a2c951186b958b5dae6abf3deb48884")):
            forged = dict(interval, **{key: value})
            with self.subTest(key=key), self.assertRaises(rommap.MapError):
                rommap._bound_slice(self.original, forged, "Audio interval")
        document = copy.deepcopy(self.document)
        index = next(i for i, row in enumerate(document["ranges"]) if row[0] <= 0x11D770 < row[1])
        start, end, kind, ref = document["ranges"][index]
        self.assertEqual((kind, ref), ("cpu_data", "main_14400"))
        document["ranges"][index:index + 1] = ([[start, 0x11D770, kind, ref]] if start < 0x11D770 else []) + [
            [0x11D770, 0x11D774, "pad_align_zero", "align"],
            [0x11D774, end, kind, ref],
        ]
        with self.assertRaises(rommap.MapError):
            self.validate(self.rehash(document))

    def test_audio_section_zero_tail_is_not_inferred_padding(self) -> None:
        document = copy.deepcopy(self.document)
        index = next(i for i, row in enumerate(document["ranges"]) if row[3] == "rsp_audio_text")
        document["ranges"][index:index + 1] = [
            [0x10AEB0, 0x10BB04, "rsp_ucode_text", "rsp_audio_text"],
            [0x10BB04, 0x10BB10, "pad_align_zero", "align"],
        ]
        with self.assertRaises(rommap.MapError):
            self.validate(self.rehash(document))

    def test_efat_directory_form_lengths_and_ff_gaps_are_properties(self) -> None:
        evidence = self.evidence["efat-evidence.json"]
        headers, gaps, forms = rommap.efat_structure(self.original, self.rules, evidence)
        self.assertEqual((len(headers), len(gaps), len(forms)), (464, 215, 463))
        for site in (int(evidence["forms"][0]["ref_rom"], 16) + 4,
                     forms[0][0] + 4, min(gaps)[0]):
            changed = bytearray(self.original)
            changed[site] ^= 1
            with self.subTest(site=hex(site)), self.assertRaises(rommap.MapError):
                rommap.efat_structure(bytes(changed), self.rules, evidence)
        forged = copy.deepcopy(evidence)
        forged["ff_padding"].pop()
        with self.assertRaises(rommap.MapError):
            rommap.efat_structure(self.original, self.rules, forged)

    def test_palette_references_require_positive_count_and_bounded_indices(self) -> None:
        evidence = self.evidence["sprite-palettes.json"]
        first = evidence["whole_intervals"][0]["payloads"][0]["references"][0]
        metadata = int(evidence["descriptor_table"]["start"], 16)
        member_limit = self.original[metadata + first["descriptor"] * 16] & 31
        for field, value in (("descriptor", 0), ("descriptor", 405), ("member", member_limit)):
            forged = copy.deepcopy(evidence)
            forged["whole_intervals"][0]["payloads"][0]["references"][0][field] = value
            with self.subTest(field=field, value=value), self.assertRaises(rommap.MapError):
                rommap._sprite_palette_ranges(self.original, forged)

    def test_palette_ownership_requires_pointer_mode_and_complete_cover(self) -> None:
        evidence = self.evidence["sprite-palettes.json"]
        for mutation in ("pointer", "mode", "missing_reference", "missing_payload", "overlap"):
            forged = copy.deepcopy(evidence)
            parent = forged["whole_intervals"][0]
            payload = parent["payloads"][0]
            ref = payload["references"][0]
            if mutation == "pointer":
                ref["pointer_word"] = forged["whole_intervals"][1]["payloads"][0]["references"][0]["pointer_word"]
            elif mutation == "mode":
                ref["tlut_mode"] = "IA16" if ref["tlut_mode"] == "RGBA16" else "RGBA16"
            elif mutation == "missing_reference":
                payload["references"].clear()
            elif mutation == "missing_payload":
                parent["payloads"].clear()
            else:
                forged["whole_intervals"].append(copy.deepcopy(parent))
            with self.subTest(mutation=mutation), self.assertRaises(rommap.MapError):
                rommap._sprite_palette_ranges(self.original, forged)

    def test_rehashed_palette_reference_cannot_claim_another_raw_interval(self) -> None:
        document = copy.deepcopy(self.document)
        document["ranges"][self.index("asset_raw_unexplained")][2:4] = [
            "asset_raw_referenced", "S_sprite_palette"]
        with self.assertRaises(rommap.MapError):
            self.validate(self.rehash(document))

    def generate_map(self) -> dict:
        return rommap.generate(self.original, self.inventory, self.document["cpu_catalogue"], self.rules,
                               catalogue_id=self.document["cpu_catalogue_id"],
                               rules_sha256=hashlib.sha256(self.rules_bytes).hexdigest())

    def palette_intervals(self) -> list[tuple[int, int]]:
        return sorted(rommap._sprite_palette_ranges(self.original, self.evidence["sprite-palettes.json"]))

    def test_palette_evidence_is_tiled_exactly_by_sprite_palette_windows(self) -> None:
        intervals = self.palette_intervals()
        self.assertEqual((len(intervals), sum(b - a for a, b in intervals)), (25, 1376))
        tiles = 0
        for a, b in intervals:
            pieces = [row for row in self.rom_map.ranges if row[0] < b and row[1] > a]
            self.assertEqual((pieces[0][0], pieces[-1][1]), (a, b))
            self.assertEqual({(row[2], row[3]) for row in pieces}, {("asset_raw_referenced", "S_sprite_palette")})
            neighbours = [row for row in self.rom_map.ranges if row[1] == a or row[0] == b]
            self.assertEqual(len(neighbours), 2)
            self.assertNotIn("S_sprite_palette", {row[3] for row in neighbours})
            tiles += len(pieces)
        self.assertEqual(tiles, 43)
        self.assertFalse([row for row in self.rom_map.ranges if row[3] == "sprite_palettes"])

    def test_palette_interval_not_tiled_by_its_windows_fails_generation(self) -> None:
        intervals = self.palette_intervals()
        first, wide = intervals[0], next((a, b) for a, b in intervals if b - a > 32)
        real = rommap.rule_windows
        for name, dropped in (("whole interval", lambda w: first[0] <= w.start < first[1]),
                              ("one inner window", lambda w: w.start == wide[0] + 32)):
            def without(*args, dropped=dropped, **kwargs):
                return [w for w in real(*args, **kwargs) if not (w.ref == "S_sprite_palette" and dropped(w))]
            with self.subTest(name=name), patch.object(rommap, "rule_windows", without), \
                    self.assertRaisesRegex(rommap.MapError, "tiled exactly"):
                self.generate_map()

    def test_truncated_palette_interval_is_not_whole(self) -> None:
        evidence = copy.deepcopy(self.evidence)
        document = evidence["sprite-palettes.json"]
        parent = next(w for w in document["whole_intervals"] if len(w["payloads"]) > 1)
        parent["payloads"].pop()
        start, end = int(parent["start"], 16), int(parent["payloads"][-1]["end"], 16)
        data = self.original[start:end]
        parent.update(end=f"{end:#x}", size=end - start, sha256=hashlib.sha256(data).hexdigest(),
                      md5=hashlib.md5(data).hexdigest())
        self.assertIn((start, end), rommap._sprite_palette_ranges(self.original, document))
        with patch.object(rommap, "load_evidence", return_value=evidence), \
                self.assertRaisesRegex(rommap.MapError, "tiled exactly"):
            self.generate_map()

    def test_tampered_palette_evidence_fails_closed(self) -> None:
        def forge(document: dict) -> None:
            ref = document["whole_intervals"][0]["payloads"][0]["references"][0]
            ref["tlut_mode"] = "IA16" if ref["tlut_mode"] == "RGBA16" else "RGBA16"
        policy = json.loads((PROJECT / rommap.EVIDENCE_PATH).read_bytes())
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            (root / "config/rom-evidence").mkdir(parents=True)
            (root / rommap.EVIDENCE_PATH).write_bytes((PROJECT / rommap.EVIDENCE_PATH).read_bytes())
            for name in policy["inputs"]:
                (root / "config/rom-evidence" / name).write_bytes((PROJECT / "config/rom-evidence" / name).read_bytes())
            target = root / "config/rom-evidence/sprite-palettes.json"
            forged = json.loads(target.read_bytes())
            forge(forged)
            target.write_text(json.dumps(forged, indent=1) + "\n")
            with self.assertRaisesRegex(rommap.MapError, "sprite-palettes.json"):
                rommap.load_evidence(self.original, self.rules, root)
        evidence = copy.deepcopy(self.evidence)
        forge(evidence["sprite-palettes.json"])
        with patch.object(rommap, "load_evidence", return_value=evidence), \
                self.assertRaisesRegex(rommap.MapError, "TLUT mode"):
            self.generate_map()

    RULED = {(0x8C79A0, 0x8CA860): "asset_raw_unexplained", (0x1040, 0x1060): "cpu_text_unexplained",
             (0x871C, 0x8780): "cpu_text_unexplained", (0xABF4C, 0xABF50): "cpu_text_unexplained"}

    def residual_document(self) -> dict:
        return copy.deepcopy(self.evidence[rommap.RESIDUAL_INPUT])

    def bind(self, record: dict, start: int, end: int) -> None:
        data = self.original[start:end]
        record.update(start=f"{start:#x}", end=f"{end:#x}", size=end - start, id=f"residual_{start:08x}_{end:08x}",
                      sha256=hashlib.sha256(data).hexdigest(), md5=hashlib.md5(data).hexdigest())

    def test_ruled_residuals_close_exactly_the_ruled_ranges_without_credit(self) -> None:
        rows = {(a, b): (cls, ref) for a, b, cls, ref in self.rom_map.ranges
                if cls in rommap.RESIDUAL_CLASSES.values()}
        self.assertEqual({key: cls for key, (cls, _) in rows.items()}, self.RULED)
        self.assertEqual({ref for _, ref in rows.values()}, {f"residual_{a:08x}_{b:08x}" for a, b in self.RULED})
        self.assertEqual(sum(b - a for a, b in self.RULED), 12104)
        self.assertEqual([(a, b) for a, b, *_ in self.rom_map.of_class("cpu_text_unowned")],
                         [(0x93A54, 0x93A60), (0xFABB4, 0xFABC0)])
        self.assertFalse(self.rom_map.of_class("asset_raw_unattributed"))
        replayed = rommap.unexplained_residuals(self.original, self.evidence)
        self.assertEqual({key: value[2] for key, value in replayed.items()}, self.RULED)
        self.assertTrue(all(record["missing_witness"].strip() for record in self.residual_document()["ranges"]))
        report = self.account()
        statuses = {(row["start"], row["end"]): row["status"] for row in report["range_statuses"]}
        self.assertEqual({statuses[key] for key in self.RULED}, {"closed_unexplained_residual"})
        self.assertEqual(report["status_bytes"]["cpu_image_opaque:closed_unexplained_residual"], 136)
        self.assertEqual(report["status_bytes"]["asset_raw:closed_unexplained_residual"], 11968)
        self.assertEqual(report["build_source_bytes"], 0)
        for cls in rommap.RESIDUAL_CLASSES.values():
            self.assertEqual(rommap.research_status(cls), "closed_unexplained_residual")
            self.assertNotEqual(rommap.CLASSES[cls], "padding")

    def test_shifted_or_missing_ruled_range_fails_closed(self) -> None:
        shifted = {(0x1044, 0x1064): ("cpu_text_unowned", "catalogue", "cpu_text_unexplained", "residual_00001044_00001064")}
        with patch.object(rommap, "unexplained_residuals", return_value=shifted), \
                self.assertRaisesRegex(rommap.MapError, "missing or shifted"):
            self.generate_map()
        document = self.residual_document()
        document["ranges"] = [r for r in document["ranges"] if r["start"] != "0x871c"]
        with self.assertRaisesRegex(rommap.MapError, "no unexplained-residual record"):
            rommap.unexplained_residuals(self.original, {rommap.RESIDUAL_INPUT: document})
        moved = self.residual_document()
        self.bind(next(r for r in moved["ranges"] if r["start"] == "0x1040"), 0x1044, 0x1064)
        with self.assertRaisesRegex(rommap.MapError, "not a range in its ruling's scope"):
            rommap.unexplained_residuals(self.original, {rommap.RESIDUAL_INPUT: moved})

    def test_unruled_range_cannot_close(self) -> None:
        document = self.residual_document()
        extra = copy.deepcopy(next(r for r in document["ranges"] if r["start"] == "0xabf4c"))
        self.bind(extra, 0x93A54, 0x93A60)  # named only as out of scope by the CPU-gap ruling
        document["ranges"].append(extra)
        with self.assertRaisesRegex(rommap.MapError, "not a range in its ruling's scope"):
            rommap.unexplained_residuals(self.original, {rommap.RESIDUAL_INPUT: document})
        rehashed = copy.deepcopy(self.document)
        row = rehashed["ranges"][self.index("cpu_text_unowned")]
        row[2:4] = "cpu_text_unexplained", "residual_00001040_00001060"
        with self.assertRaises(rommap.MapError):
            self.validate(self.rehash(rehashed))

    def test_tampered_ruling_copy_fails_closed(self) -> None:
        document = self.residual_document()
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            (root / rommap.RULINGS_DIR).mkdir(parents=True)
            for name, pin in document["rulings"].items():
                (root / pin["path"]).write_bytes((PROJECT / pin["path"]).read_bytes())
            rommap.unexplained_residuals(self.original, self.evidence, root)
            target = root / document["rulings"]["R0-UNEXPLAINED-RESIDUAL-20261014"]["path"]
            target.write_bytes(target.read_bytes().replace(b"0x8C79A0-0x8CA860", b"0x8C79A0-0x8CA880"))
            with self.assertRaisesRegex(rommap.MapError, "Ruling copy identity changed"):
                rommap.unexplained_residuals(self.original, self.evidence, root)
            target.unlink()
            with self.assertRaisesRegex(rommap.MapError, "Ruling copy identity changed"):
                rommap.unexplained_residuals(self.original, self.evidence, root)

    def test_ruling_scope_is_parsed_from_the_hashed_bytes(self) -> None:
        real_read_bytes, real_read_text = Path.read_bytes, Path.read_text
        reads: dict[str, int] = {}

        def is_ruling(path: Path) -> bool:
            return path.parent.name == Path(rommap.RULINGS_DIR).name and path.suffix == ".md"

        def reader(path: Path) -> bytes:
            data = real_read_bytes(path)
            if is_ruling(path):
                reads[path.name] = reads.get(path.name, 0) + 1
                if reads[path.name] > 1:  # a changed second view: its scope no longer names the ruled ranges
                    return data.replace(b"0x1040-0x1060", b"0x1040-0x1064").replace(b"0x8C79A0-0x8CA860",
                                                                                      b"0x8C79A0-0x8CA880")
            return data

        def text_reader(path: Path, *args, **kwargs) -> str:
            if is_ruling(path):
                raise AssertionError(f"ruling copy reopened as text: {path}")
            return real_read_text(path, *args, **kwargs)

        with patch.object(Path, "read_bytes", autospec=True, side_effect=reader), \
                patch.object(Path, "read_text", autospec=True, side_effect=text_reader), \
                patch.object(rommap, "_ruled_scope", wraps=rommap._ruled_scope) as parse:
            replayed = rommap.unexplained_residuals(self.original, self.evidence)
        self.assertEqual({key: value[2] for key, value in replayed.items()}, self.RULED)
        pins = self.residual_document()["rulings"].values()
        self.assertEqual(reads, {Path(pin["path"]).name: 1 for pin in pins})
        hashed = sorted(real_read_bytes(PROJECT / pin["path"]) for pin in pins)
        self.assertEqual(sorted(hashlib.sha256(data).hexdigest() for data in hashed), sorted(pin["sha256"] for pin in pins))
        self.assertEqual(sorted(call.args[0] for call in parse.call_args_list), [data.decode("utf-8") for data in hashed])

    def test_ruled_range_in_wrong_prior_class_fails_closed(self) -> None:
        real = rommap.cpu_padding

        def witnessed(*args, **kwargs):  # a later padding witness for G3 must not be silently absorbed
            return {**real(*args, **kwargs), (0x1040, 0x1060): "pad_object_rounding"}
        with patch.object(rommap, "cpu_padding", witnessed), \
                self.assertRaisesRegex(rommap.MapError, "not its expected prior class"):
            self.generate_map()
        document = self.residual_document()
        next(r for r in document["ranges"] if r["start"] == "0x1040")["prior_class"] = "cpu_data"
        with self.assertRaisesRegex(rommap.MapError, "unsupported class, prior class"):
            rommap.unexplained_residuals(self.original, {rommap.RESIDUAL_INPUT: document})

    def test_unchanged_rehashed_map_validates(self) -> None:
        # Positive control for every rehash-based negative: only the mutation under test can fail it.
        self.assertTrue(self.document["splits"])
        rehashed = self.rehash(copy.deepcopy(self.document))
        self.assertEqual(rehashed["map_id"], self.document["map_id"])
        self.assertEqual(self.validate(rehashed).map_id, self.document["map_id"])

    def data_padding_replay(self, document: dict | None = None, root: Path = PROJECT,
                            base_rows: list | None = None, vtables: dict | None = None) -> list:
        evidence = dict(self.evidence)
        if document is not None:
            evidence[rommap.CPU_DATA_PADDING_INPUT] = document
        rows = base_rows if base_rows is not None else rommap.unsplit_rows(self.document["ranges"], self.document["splits"])
        if vtables is None:
            vtables = rommap.cpu_vtables(self.original, self.document["cpu_catalogue"], self.evidence, rows)
        return rommap.cpu_data_padding(self.original, self.document["cpu_catalogue"], evidence, rows, vtables, root)

    def data_padding_record(self, align: int, size: int, kind: str = "carve") -> tuple[dict, dict]:
        document = copy.deepcopy(self.evidence[rommap.CPU_DATA_PADDING_INPUT])
        return document, next(r for r in document["runs"] if r["align"] == align and r["next"]["kind"] == kind
                              and r["end"] - r["start"] == size)

    def test_cpu_data_padding_rows_are_exactly_the_replayed_minimal_fills(self) -> None:
        pieces = self.data_padding_replay()
        rows = self.rom_map.of_class("cpu_data_padding")
        self.assertEqual([(a, b, ref) for a, b, _, ref in rows], [(p[0], p[1], p[3]) for p in pieces])
        self.assertEqual((len(rows), sum(b - a for a, b, *_ in rows)), (44, 156))
        runs = self.evidence[rommap.CPU_DATA_PADDING_INPUT]["runs"]
        by_kind = Counter(r["next"]["kind"] for r in runs)
        self.assertEqual(by_kind, {"carve": 27, "vtable": 17})
        self.assertEqual(sum(r["end"] - r["start"] for r in runs if r["next"]["kind"] == "vtable"), 65)
        images = {row["image_id"]: row for row in self.document["cpu_catalogue"]["images"]}
        parents = {tuple(piece[:2]): split["parent"] for split in self.document["splits"] for piece in split["pieces"]}
        vtables = {start: ref for start, _, _, ref in self.rom_map.of_class("compiler_generated_vtable")}
        for record in runs:
            a, b, n = record["start"], record["end"], record["align"]
            vram = images[record["image_id"]]["vram_start"] - images[record["image_id"]]["rom_start"]
            self.assertEqual(b + vram, -(-(a + vram) // n) * n)
            self.assertTrue(0 < b - a < n and not any(self.original[a:b]))
            self.assertEqual(parents[(a, b)][2:], ["cpu_data", record["image_id"]])
            self.assertEqual(record["next"]["first_object_align"], n)
            if record["next"]["kind"] == "vtable":
                self.assertEqual((n, vtables[b]), (8, record["next"]["table"]))
        report = self.account()
        closed = [row for row in report["range_statuses"] if row["class"] == "cpu_data_padding"]
        self.assertEqual({row["status"] for row in closed}, {"closed"})
        self.assertEqual(rommap.CLASSES["cpu_data_padding"], "padding")
        self.assertEqual(report["build_source_bytes"], 0)

    def test_next_alignment_is_the_compilers_first_object_request(self) -> None:
        document = self.evidence[rommap.CPU_DATA_PADDING_INPUT]
        control = document["negative_controls"][0]["record"]
        self.assertEqual((control["start"], control["end"], control["align"]), (0x136A14, 0x136A18, 8))
        # The excluded real case: genuine carves and objects, an 8-aligned next start and an 8-aligned
        # section, but the next section's first object is `.align 2` (N = 4) and the run is 4 bytes.
        for name, first in (("correctly annotated", 4), ("annotation forged to 8", 8)):
            forged = copy.deepcopy(document)
            record = copy.deepcopy(control)
            record["next"]["first_object_align"] = first
            forged["runs"].append(record)
            with self.subTest(name=name), self.assertRaisesRegex(rommap.MapError,
                                                                 "alignment is 8, not the first object's 4"):
                self.data_padding_replay(forged)
        forged = copy.deepcopy(document)
        record = copy.deepcopy(control)
        record.update(align=4)
        record["next"]["first_object_align"] = 4
        forged["runs"].append(record)
        with self.assertRaisesRegex(rommap.MapError, "not shorter than its alignment"):
            self.data_padding_replay(forged)
        forged, record = self.data_padding_record(8, 4)
        record["next"]["first_object_align"] = 4
        with self.assertRaisesRegex(rommap.MapError, "recorded first-object alignment is not 8"):
            self.data_padding_replay(forged)
        forged, record = self.data_padding_record(8, 4)
        four = next(r for r in document["runs"] if r["align"] == 4 and r["next"]["kind"] == "carve"
                    and r["next"]["section"] == ".rodata")
        record["next"]["compiled_asm"] = copy.deepcopy(four["next"]["compiled_asm"])
        with self.assertRaisesRegex(rommap.MapError, "not the first object's 4"):
            self.data_padding_replay(forged)

    def test_cpu_data_padding_before_a_replayed_vtable(self) -> None:
        document, record = self.data_padding_record(8, 4, "vtable")
        self.assertEqual(len(self.data_padding_replay(document)), 44)
        record["next"]["table"] = "VT_main_14400_0"
        with self.assertRaisesRegex(rommap.MapError, "not the replayed vtable"):
            self.data_padding_replay(document)
        document, record = self.data_padding_record(8, 4, "vtable")
        with self.assertRaisesRegex(rommap.MapError, "not the replayed vtable"):
            self.data_padding_replay(document, vtables={})
        carve = next(r for r in document["runs"] if r["next"]["kind"] == "carve" and r["align"] == 8)
        record["next"]["compiler_probe"] = copy.deepcopy(carve["next"]["compiled_asm"])
        with self.assertRaisesRegex(rommap.MapError, "pinned cc1plus probe"):
            self.data_padding_replay(document)
        document, record = self.data_padding_record(8, 4, "vtable")
        record["previous"]["carve_start"] -= 4
        with self.assertRaisesRegex(rommap.MapError, "previous object is not the canonical"):
            self.data_padding_replay(document)

    def test_cpu_data_padding_geometry_and_alignment_fail_closed(self) -> None:
        cases = []
        document, record = self.data_padding_record(4, 3)
        record["start"] = record["end"] - 4
        cases.append(("run not shorter than N", document, record, "not shorter than its alignment"))
        document, record = self.data_padding_record(8, 2)
        record["align"] = 4
        cases.append(("alignment the next object does not have", document, record,
                      "requested alignment 8 for the next object, not 4"))
        document, record = self.data_padding_record(8, 4)
        record["align"] = 16
        cases.append(("a stronger alignment than proven", document, record, "not aligned to 16|requested alignment 8"))
        document, record = self.data_padding_record(4, 1)
        record["end"] += 2
        cases.append(("misaligned next start", document, record, "not aligned"))
        for name, forged, record, message in cases:
            record["id"] = f"cdp_{record['start']:08x}_{record['end']:08x}"
            with self.subTest(name=name), self.assertRaisesRegex(rommap.MapError, message):
                self.data_padding_replay(forged)

    def test_cpu_data_padding_needs_both_canonical_carves(self) -> None:
        document, record = self.data_padding_record(4, 3)
        record["previous"]["carve_start"] -= 4
        with self.assertRaisesRegex(rommap.MapError, "previous object is not the canonical"):
            self.data_padding_replay(document)
        document, record = self.data_padding_record(4, 3)
        record["next"]["unit"] = record["previous"]["unit"]
        with self.assertRaisesRegex(rommap.MapError, "next object is not the canonical"):
            self.data_padding_replay(document)
        document, record = self.data_padding_record(8, 4)
        record["next"]["object"] = copy.deepcopy(record["previous"]["object"])
        with self.assertRaisesRegex(rommap.MapError, "not exactly the original carve|exactly one PROGBITS"):
            self.data_padding_replay(document)
        rows = [list(row) for row in rommap.unsplit_rows(self.document["ranges"], self.document["splits"])]
        vtables = rommap.cpu_vtables(self.original, self.document["cpu_catalogue"], self.evidence, rows)
        first = self.evidence[rommap.CPU_DATA_PADDING_INPUT]["runs"][0]
        parent = next(row for row in rows if row[0] <= first["start"] < row[1])
        parent[3] = "overlay_1339f0" if parent[3] != "overlay_1339f0" else "main_14400"
        with self.assertRaisesRegex(rommap.MapError, "inside one cpu_data row"):
            self.data_padding_replay(base_rows=rows, vtables=vtables)

    def test_cpu_data_padding_files_are_content_addressed(self) -> None:
        document = self.evidence[rommap.CPU_DATA_PADDING_INPUT]
        record = next(r for r in document["runs"] if r["next"]["kind"] == "carve")
        vtable = next(r for r in document["runs"] if r["next"]["kind"] == "vtable")
        for pin in (record["next"]["object"], record["next"]["compiled_asm"], vtable["next"]["compiler_probe"]):
            with tempfile.TemporaryDirectory() as temporary:
                root = Path(temporary)
                (root / "config").mkdir()
                (root / "config/shiren2.jp.yaml").write_bytes((PROJECT / "config/shiren2.jp.yaml").read_bytes())
                (root / rommap.CPU_DATA_PADDING_DIR).mkdir(parents=True)
                for path in (PROJECT / rommap.CPU_DATA_PADDING_DIR).iterdir():
                    (root / rommap.CPU_DATA_PADDING_DIR / path.name).write_bytes(path.read_bytes())
                self.assertEqual(len(self.data_padding_replay(root=root)), len(document["runs"]))
                target = root / pin["path"]
                data = bytearray(target.read_bytes())
                data[-2] ^= 1
                target.write_bytes(bytes(data))
                with self.subTest(pin=pin["path"]), self.assertRaisesRegex(rommap.MapError, "identity changed"):
                    self.data_padding_replay(root=root)

    def test_cpu_data_padding_reclass_only_inside_cpu_data(self) -> None:
        rows = [[0, 16, "cpu_data", "main_14400"], [16, 32, "asset_raw_unattributed", "unattributed"]]
        rommap.split_rows(rows, [(4, 8, "cpu_data_padding", "pad_data_alignment", "t", (0, 16))])
        with self.assertRaisesRegex(rommap.MapError, "changes class"):
            rommap.split_rows(rows, [(20, 24, "cpu_data_padding", "pad_data_alignment", "t", (16, 32))])

    def test_inherited_stream_hash_and_extent_cannot_be_forged(self) -> None:
        row = self.evidence["yay0-streams.json"][0]
        for key, value in (("end", row["end"] + 1), ("stream_sha256", "0" * 64), ("crunch64_exact", False)):
            with self.subTest(key=key), self.assertRaises(rommap.MapError):
                rommap.tail_streams(self.original, self.rules["tail_start"], self.rules["content_end"],
                                    [dict(row, **{key: value})])

    def test_c_credit_only_exact_function_ranges_and_never_acceptance(self) -> None:
        handwritten = rommap._handwritten(PROJECT, self.original, [])
        first, second = [r for r in self.rom_map.of_class("cpu_text_function")
                         if (r[0], r[1]) not in handwritten][:2]
        result = self.account([self.match(first[0], first[1]), self.match(second[0], second[1], "src/y.cpp")])
        self.assertEqual(result["cpu_code"]["matched_c_bytes"], first[1] - first[0])
        self.assertEqual(result["cpu_code"]["matched_cpp_bytes"], second[1] - second[0])
        self.assertEqual(result["build_source_bytes"], 0)
        self.assertEqual(result["build_source_state"], "unaccepted")
        for kind in rommap.CLASSES:
            if kind == "cpu_text_function":
                continue
            rows = self.rom_map.of_class(kind)
            if rows:
                with self.subTest(kind=kind), self.assertRaises(rommap.MapError):
                    self.account([self.match(rows[0][0], rows[0][1])])
        for matches in ([self.match(first[0] + 4, first[1])], [self.match(first[0], first[1])] * 2):
            with self.assertRaises(rommap.MapError):
                self.account(matches)

    def test_research_and_source_denominators_do_not_mix(self) -> None:
        result = self.account()
        self.assertEqual(result["research_classified_bytes"] + result["research_open_bytes"], EXPECTED_SIZE)
        self.assertEqual(sum(result["status_bytes"].values()), EXPECTED_SIZE)
        self.assertEqual(sum(row["end"] - row["start"] for row in result["range_statuses"]), EXPECTED_SIZE)
        self.assertEqual(result["build_source_bytes"], 0)
        self.assertNotIn("closed_bytes", result)
        self.assertNotIn("complete_game_closed_percent", result)
        self.assertFalse(result["whole_denominator_closed"])
        for kind in ("asset_raw_referenced", "asset_audio_wavetable", "asset_demo_input", "asset_hvqm2_video",
                     "asset_audio_bank_ptrtable", "asset_iff_payload"):
            self.assertEqual(rommap.research_status(kind), "located")
        self.assertNotIn("cpu_data:owned_c", result["status_bytes"])

    def test_naked_boolean_roundtrip_claims_are_rejected(self) -> None:
        claims = {r[0]: True for r in self.rom_map.of_class("asset_yay0")}
        for forged in (claims, True, "build/fake"):
            with self.subTest(kind=type(forged).__name__), self.assertRaisesRegex(rommap.MapError, "build directory"):
                self.account(build_directory=forged)
        with tempfile.TemporaryDirectory() as temporary:
            with self.assertRaises((ValueError, OSError)):
                self.account(build_directory=Path(temporary))
        self.assertNotIn("asset_compressed:roundtrip", self.account()["status_bytes"])

    def test_hash_correct_handwritten_proposal_still_cannot_close(self) -> None:
        start, end, *_ = self.rom_map.of_class("cpu_text_function")[0]
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            (root / "config").mkdir()
            record = {"target_sha256": rommap.EXPECTED_SHA256, "functions": [
                {"rom_start": start, "size": end - start,
                 "expected_sha256": hashlib.sha256(self.original[start:end]).hexdigest(),
                 "receipt_binding": "1" * 64, "structural_fill": True}]}
            (root / rommap.HANDWRITTEN_PATH).write_text(json.dumps(record))
            (root / rommap.CATALOGUE_PATH).write_bytes(CATALOGUE.read_bytes())
            with self.assertRaises(ValueError):
                self.account(root=root)

    def test_stale_or_forged_evidence_policy_is_rejected(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            (root / "config").mkdir()
            policy = json.loads((PROJECT / rommap.EVIDENCE_PATH).read_text())
            policy["policy"] = "raw format proven; structural_fill true; audio extent proven"
            (root / rommap.EVIDENCE_PATH).write_text(json.dumps(policy))
            with self.assertRaisesRegex(rommap.MapError, "policy identity"):
                rommap.load_evidence(self.original, self.rules, root)


if __name__ == "__main__":
    unittest.main(verbosity=2)
