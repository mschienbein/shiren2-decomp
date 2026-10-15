"""Property-replayed research partition; never equate research with accepted source."""
from __future__ import annotations

import copy
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

    def test_unsupported_padding_claim_is_rejected(self) -> None:
        with self.assertRaises(rommap.MapError):
            rommap.research_status("cpu_text_padding")


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
        return rommap._document(self.original, document["ranges"],
                                rules_sha256=document["rules_sha256"],
                                catalogue_id=document["cpu_catalogue_id"],
                                catalogue=document["cpu_catalogue"])

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
        for kind, ref in (("pad_align_zero", "align"), ("rom_padding_zero", "boot")):
            document = copy.deepcopy(self.document)
            row = document["ranges"][self.index("cpu_text_unowned")]
            row[2:4] = kind, ref
            with self.subTest(kind=kind), self.assertRaises(rommap.MapError):
                self.validate(self.rehash(document))

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

    def test_audio_binary_identity_closes_only_sdk_sections(self) -> None:
        result = self.account()
        self.assertEqual(result["status_bytes"]["rsp_ucode:open"], 4)
        self.assertEqual(result["status_bytes"]["rsp_ucode:classified_binary"], 0xD0 + 0x1390 + 0x420 + 0xC60 + 0x2B0)
        self.assertEqual(result["build_source_bytes"], 0)
        for start, end in ((0x10AEB0, 0x10BB10), (0x11D4C0, 0x11D770)):
            with self.subTest(start=start), self.assertRaises(rommap.MapError):
                self.account([self.match(start, end)])
        interval = copy.deepcopy(self.evidence["audio-intervals.json"]["intervals"][0])
        for key, value in (("end", "0x010bb10"), ("md5", "9a2c951186b958b5dae6abf3deb48884")):
            forged = dict(interval, **{key: value})
            with self.subTest(key=key), self.assertRaises(rommap.MapError):
                rommap._bound_slice(self.original, forged, "Audio interval")
        document = copy.deepcopy(self.document)
        row = next(row for row in document["ranges"] if row[3] == "rsp_audio_boundary")
        row[2:4] = ["pad_align_zero", "align"]
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
        document["ranges"][self.index("asset_raw_unattributed")][2:4] = [
            "asset_raw_referenced", "sprite_palettes"]
        with self.assertRaises(rommap.MapError):
            self.validate(self.rehash(document))

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
