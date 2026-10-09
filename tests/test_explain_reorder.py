from __future__ import annotations

import sys
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "tools"))
import explain

FIXTURES = Path(__file__).parent / "fixtures/explain"


def fixture(name: str) -> str:
    return (FIXTURES / name).read_text()


def assembly_excerpt(name: str, first_line: int) -> str:
    """Keep original gas line numbers without retaining unrelated functions."""
    return "\n" * (first_line - 1) + fixture(name)


class ReorderedListingTests(unittest.TestCase):
    def assert_unattributed(self, mapped: dict, offsets: tuple[int, ...]) -> None:
        self.assertTrue(mapped["unavailable"])
        for offset in offsets:
            with self.subTest(offset=offset):
                self.assertIsNone(mapped["words"].get(offset, {}).get("uid"))

    def test_controller_absolute_store_and_call_have_correct_owners(self) -> None:
        # Own KMC capture, relative to scratch/omp/harness/merge-007:
        # validation/func_80027910_func_80027AAC/explain/run-ow_7lddn/uid/
        # mapping.s lines 54..62; listing.txt rows 66..69.
        # Unlisted object prefix is a placeholder, never evidence for a UID.
        assembly = assembly_excerpt("controller_reorder_kmc26.s", 54)
        listing = fixture("controller_reorder_kmc26.listing")
        code = bytes(0x44) + bytes.fromhex("24020001 3c010000 0c000000 ac220000")
        mapped = explain.assembly_listing(assembly, listing, code)
        self.assertEqual(mapped["unavailable"], [])
        expected = {0x44: (32, 57), 0x48: (34, 58), 0x4C: (37, 59), 0x50: (34, 58)}
        self.assertEqual(set(mapped["words"]), set(expected))
        for offset, (uid, source_line) in expected.items():
            with self.subTest(offset=offset):
                self.assertEqual(mapped["words"][offset]["uid"], uid)
                self.assertEqual(mapped["words"][offset]["assembly_line"], source_line)

    def test_return_and_load_use_owner_lines_not_gas_rows(self) -> None:
        # Own gcc281pm-gnu291-O2-unsigned capture:
        # review-H7ReviewOpus/delta/attribution/
        # gcc281pm-gnu291-O2-unsigned/func_800FFC08/
        # mapping.s lines 4..17; listing.txt rows 16..17.
        assembly = assembly_excerpt("return_reorder_gcc281.s", 4)
        mapped = explain.assembly_listing(
            assembly, fixture("return_reorder_gcc281.listing"),
            bytes.fromhex("03e00008 90820089"),
        )
        self.assertEqual(mapped["unavailable"], [])
        self.assertEqual(set(mapped["words"]), {0, 4})
        self.assertEqual(mapped["words"][0]["uid"], 23)
        self.assertEqual(mapped["words"][0]["assembly_line"], 14)
        self.assertEqual(mapped["words"][4]["uid"], 11)
        self.assertEqual(mapped["words"][4]["assembly_line"], 13)

    def test_reorder_identical_encodings_with_different_uids_are_ambiguous(self) -> None:
        assembly = (
            ".text\n.set reorder\n.ent f\nf:\n"
            "addu $2,$4,$5 # 11 addsi3_internal\n"
            "addu $2,$4,$5 # 12 addsi3_internal\n.end f\n"
        )
        listing = "   5 0000 00851021 \taddu $2,$4,$5\n   6 0004 00851021 \taddu $2,$4,$5\n"
        mapped = explain.assembly_listing(assembly, listing, bytes.fromhex("00851021 00851021"))
        self.assert_unattributed(mapped, (0, 4))

    def test_unknown_mnemonic_blocks_proof_throughout_reorder_run(self) -> None:
        assembly = (
            ".text\n.set reorder\n.ent f\nf:\n"
            "unknown_instruction $2,$4,$5 # 11 unknown_pattern\n"
            "addu $2,$4,$5 # 12 addsi3_internal\n.end f\n"
        )
        listing = (
            "   5 0000 00851021 \tunknown_instruction $2,$4,$5\n"
            "   6 0004 00851021 \taddu $2,$4,$5\n"
        )
        mapped = explain.assembly_listing(assembly, listing, bytes.fromhex("00851021 00851021"))
        self.assert_unattributed(mapped, (0, 4))

    def test_noreorder_incompatible_word_is_not_reassociated(self) -> None:
        assembly = (
            ".text\n.set noreorder\n.ent f\nf:\n"
            "lbu $2,137($4) # 11 zero_extendqisi2/2\n"
            "j $31 # 23 return\n.end f\n"
        )
        listing = "   5 0000 03E00008 \tlbu $2,137($4)\n   6 0004 90820089 \tj $31\n"
        mapped = explain.assembly_listing(assembly, listing, bytes.fromhex("03e00008 90820089"))
        self.assert_unattributed(mapped, (0, 4))


class EofPaddingListingTests(unittest.TestCase):
    def setUp(self) -> None:
        # Own gcc272-kmc26-O2-signed capture:
        # review-H7ReviewOpus/delta/attribution/
        # gcc272-kmc26-O2-signed/func_800302D0/
        # mapping.s lines 1121..1131; listing.txt rows 1220..1221.
        # Only the final eight object bytes are represented by listing rows;
        # the unlisted prefix is a placeholder, not copied code evidence.
        self.assembly = assembly_excerpt("eof_padding_kmc26.s", 1121)
        self.listing = fixture("eof_padding_kmc26.listing")
        self.code = bytes(0xBA0)

    def test_real_eof_zero_padding_is_checked_without_an_rtl_uid(self) -> None:
        self.assertEqual(len(self.assembly.splitlines()), 1131)
        mapped = explain.assembly_listing(self.assembly, self.listing, self.code)
        self.assertEqual(mapped["unavailable"], [])
        self.assertEqual(set(mapped["words"]), {0xB98, 0xB9C})
        self.assertIsNone(mapped["words"][0xB98]["uid"])
        self.assertIsNone(mapped["words"][0xB9C]["uid"])

    def test_eof_padding_with_wrong_object_bytes_is_unavailable(self) -> None:
        # Each listed padding word must independently pass the byte check.
        for offset in (0xB98, 0xB9C):
            with self.subTest(offset=offset):
                code = bytearray(self.code)
                code[offset] = 0xFF
                mapped = explain.assembly_listing(self.assembly, self.listing, bytes(code))
                self.assertTrue(mapped["unavailable"])
                self.assertIsNone(mapped["words"].get(offset, {}).get("uid"))

    def test_matching_nonzero_eof_word_is_not_accepted_as_padding(self) -> None:
        listing = self.listing.replace("00000000", "24020001")
        code = self.code[:0xB98] + bytes.fromhex("24020001 24020001")
        mapped = explain.assembly_listing(self.assembly, listing, code)
        self.assertTrue(mapped["unavailable"])
        for offset in (0xB98, 0xB9C):
            self.assertIsNone(mapped["words"].get(offset, {}).get("uid"))

    def test_eof_source_line_beyond_one_past_end_is_unavailable(self) -> None:
        listing = self.listing.replace("1132", "1133")
        mapped = explain.assembly_listing(self.assembly, listing, self.code)
        self.assertTrue(mapped["unavailable"])
        for offset in (0xB98, 0xB9C):
            self.assertIsNone(mapped["words"].get(offset, {}).get("uid"))

    def test_eof_padding_outside_object_is_unavailable(self) -> None:
        mapped = explain.assembly_listing(self.assembly, self.listing, self.code[:0xB98])
        self.assertTrue(mapped["unavailable"])
        for offset in (0xB98, 0xB9C):
            self.assertIsNone(mapped["words"].get(offset, {}).get("uid"))


if __name__ == "__main__":
    unittest.main()
