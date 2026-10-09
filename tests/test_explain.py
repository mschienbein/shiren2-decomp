from __future__ import annotations

from contextlib import redirect_stderr
from io import StringIO
import json
import sys
import tempfile
import unittest
from pathlib import Path
from types import SimpleNamespace
from unittest.mock import patch

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "tools"))
import explain
import match
from rtl_alloc import parse_allocation
from rtl_sched import parse_scheduler

FIXTURES = Path(__file__).parent / "fixtures/explain"


def fixture(name: str) -> str:
    return (FIXTURES / name).read_text()


class RtlOriginTests(unittest.TestCase):
    def test_real_cpp_inline_attributes_and_source_notes(self) -> None:
        facts = explain.rtl_instructions(fixture("engine_gxx281.lreg"), "func_800BFEA0")
        self.assertEqual(facts["unavailable"], [])
        self.assertEqual(facts["instructions"][26]["registers"], [80])
        self.assertEqual(facts["instructions"][26]["source"]["line"], 51)
        self.assertEqual(facts["instructions"][39]["registers"], [87])
        self.assertEqual(facts["origins"][87][0]["line"], 59)
        self.assertNotIn(80, facts["origins"])

    def test_unknown_rtl_record_is_not_silently_ignored(self) -> None:
        raw = fixture("engine_gxx281.lreg") + "\n(new_insn 999 0 0)\n"
        facts = explain.rtl_instructions(raw, "func_800BFEA0")
        self.assertIn("Unknown top-level", facts["unavailable"][0]["reason"])

    def test_missing_or_ambiguous_function_fails_closed(self) -> None:
        raw = fixture("engine_gxx281.lreg")
        for text, function in ((raw, "other"), (raw + raw, "func_800BFEA0")):
            with self.assertRaises(ValueError):
                explain.rtl_instructions(text, function)


class ListingTests(unittest.TestCase):
    # Exact own pinned gcc281 + GNU gas 2.9.1 output. Object encoded as hex so
    # the increment remains a plain-text patch, not a platform binary patch.
    def setUp(self) -> None:
        self.elf = match.Elf(bytes.fromhex(fixture("engine_gcc281.object.hex")))
        self.text = self.elf.contents(self.elf.section(".text"))
        self.assembly = fixture("engine_gcc281.s")
        self.listing = fixture("engine_gcc281.listing")

    def test_all_real_words_map_with_checked_bytes(self) -> None:
        mapped = explain.assembly_listing(self.assembly, self.listing, self.text)
        self.assertEqual(mapped["unavailable"], [])
        self.assertEqual(len(mapped["words"]), len(self.text) // 4)
        self.assertEqual(mapped["words"][0x2C]["uid"], 38)
        self.assertEqual(mapped["words"][0x30]["uid"], 33)
        self.assertEqual(mapped["words"][0x38]["uid"], 35)

    def test_changed_object_bytes_withhold_mapping(self) -> None:
        mapped = explain.assembly_listing(self.assembly, self.listing, b"\xff" * len(self.text))
        self.assertFalse(mapped["words"])
        self.assertTrue(mapped["unavailable"])

    def test_unknown_listing_row_is_explicit(self) -> None:
        mapped = explain.assembly_listing(self.assembly, "unexpected listing row\n" + self.listing, self.text)
        self.assertIn("Unknown listing row", mapped["unavailable"][0]["reason"])

    def test_real_modern_source_rows_do_not_change_word_mapping(self) -> None:
        expected = explain.assembly_listing(self.assembly, self.listing, self.text)
        mapped = explain.assembly_listing(
            self.assembly, fixture("source_modern237.listing") + self.listing, self.text
        )
        self.assertEqual(mapped, expected)
        malformed = explain.assembly_listing(self.assembly, "1:/file.c *** not a source row\n", self.text)
        self.assertEqual(malformed["words"], {})
        self.assertEqual(malformed["unavailable"][0]["reason"], "Unknown listing row")

    def test_real_kmc_dwarf_short_rows_are_known_noncode(self) -> None:
        # Exact source/listing excerpt from the pinned KMC -g diagnostic for
        # func_80025E80; leading blank lines preserve real gas source numbers.
        assembly = "\n" * 58 + (
            "\t.section\t.debug\n.L_debug_b:\n.L_D1:\n"
            "\t.4byte\t.L_D1_e-.L_D1\n\t.2byte\t0x11\n\t.2byte\t0x12\n"
        )
        listing = "  63 0004 0011     \t\t.2byte\t0x11\n  64 0006 0012     \t\t.2byte\t0x12\n"
        mapped = explain.assembly_listing(assembly, listing, b"")
        self.assertEqual(mapped, {"words": {}, "unavailable": []})

    def test_previous_returns_from_debug_to_text(self) -> None:
        assembly = ".section .debug\n.previous\n.ent f\nmove $16,$4 # 4 movsi_internal2/1\n"
        listing = "   4 0000 0080802D \tmove $16,$4 # 4 movsi_internal2/1\n"
        mapped = explain.assembly_listing(assembly, listing, bytes.fromhex("0080802d"))
        self.assertEqual(mapped["unavailable"], [])
        self.assertEqual(mapped["words"][0]["uid"], 4)

    def test_real_kmc_section_switch_padding_is_not_text(self) -> None:
        # controller_init's source excerpt, preserving gas line numbers.
        assembly = "\n" * 14 + (
            "\t.data\n\t.align\t2\n\t.type\t D_80036F80,@object\n"
            "\t.size\t D_80036F80,4\nD_80036F80:\n\t.word\t0\n"
            "\t.text\n\t.align\t2\n\t.globl\tfunc_80027910\n"
            "\t.type\t func_80027910,@function\n\t.ent\tfunc_80027910\n"
            "func_80027910:\n\t.frame\t$sp,128,$31\n\t.mask\t0x807f0000,-4\n"
            "\t.fmask\t0x00000000,0\n\tlw\t$2,D_80036F80 \t\t#  18 movsi_internal2/6\n"
        )
        mapped = explain.assembly_listing(
            assembly, fixture("section_kmc26.listing"), bytes.fromhex("3c0200008c420000")
        )
        self.assertEqual(mapped["unavailable"], [])
        self.assertEqual(set(mapped["words"]), {0, 4})
        self.assertEqual([item["uid"] for item in mapped["words"].values()], [18, 18])

    def test_empty_debug_section_does_not_change_codegen_identity(self) -> None:
        expected = explain._object_identity(self.elf)
        self.elf.sections.append(match.Section(len(self.elf.sections), ".rodata", 1, 2, 0, 0, 0, 0, 0, 1))
        self.assertEqual(explain._object_identity(self.elf), expected)

    def test_explicit_text_data_does_not_inherit_an_instruction_uid(self) -> None:
        assembly = ".ent f\naddiu $2,$0,1 # 11 movsi_internal2/3\n.word 0x24020001\n"
        listing = "  2 0000 24020001 \taddiu $2,$0,1\n  3 0004 24020001 \t.word 0x24020001\n"
        mapped = explain.assembly_listing(assembly, listing, bytes.fromhex("2402000124020001"))
        self.assertEqual(mapped["words"][0]["uid"], 11)
        self.assertIsNone(mapped["words"][4]["uid"])
        self.assertEqual(mapped["words"][4]["attribution"], "data")
        self.assertEqual(mapped["unavailable"], [])

    def test_reorder_push_pop_restores_noreorder_ownership(self) -> None:
        assembly = (
            ".ent f\n.set noreorder\n.set push\n.set reorder\n"
            "move $16,$4 # 4 movsi_internal2/1\nj $31 # 5 return\n.set pop\n"
            "move $17,$5 # 6 movsi_internal2/1\nj $31 # 7 return\n"
        )
        listing = (
            "  5 0000 03E00008 \tmove $16,$4\n  6 0004 0080802D \tj $31\n"
            "  8 0008 03E00008 \tmove $17,$5\n  9 000c 00A0882D \tj $31\n"
        )
        mapped = explain.assembly_listing(assembly, listing, bytes.fromhex("03e000080080802d03e0000800a0882d"))
        self.assertEqual(mapped["words"][0]["uid"], 5)
        self.assertEqual(mapped["words"][4]["uid"], 4)
        self.assertIsNone(mapped["words"][8]["uid"])
        self.assertIsNone(mapped["words"][12]["uid"])
        self.assertEqual(len(mapped["unavailable"]), 2)


class WordExplanationTests(unittest.TestCase):
    def test_real_global_competitor_and_minimum_threshold(self) -> None:
        symbol = "func_800BFEA0"
        allocation = parse_allocation(fixture("alloc_gxx281.lreg"), fixture("alloc_gxx281.greg"), symbol)
        rtl = explain.rtl_instructions(fixture("engine_gxx281.lreg"), symbol)
        facts = {symbol: {"allocation": allocation, "rtl": rtl, "scheduler": {"instructions": {}, "decisions": []}}}
        target = match.Target("main_14400", symbol, 0, 0x800BFEA0, 4, 4)
        result = explain.explain_words(bytes.fromhex("ae630404"), bytes.fromhex("ae830404"), [target], facts, {0: {"uid": 26}})
        contender = result[0]["register_differences"][0]["candidates"][0]
        self.assertEqual(contender["pseudo"]["pseudo"], 80)
        holder = next(p for p in contender["competitors"] if p["pseudo"]["pseudo"] == 169)
        self.assertEqual(holder["flip"]["candidate_refs_increase"], 2)
        self.assertEqual(holder["flip"]["candidate_live_reduction"], 12)
        self.assertIn({"pseudo": 80, "metric": "refs", "operator": ">=", "value": 27, "delta": 2}, holder["flip"]["thresholds"])
        self.assertTrue(json.dumps(result))

    def test_real_scheduler_order_for_relocated_words(self) -> None:
        symbol = "func_8008C6B8"
        scheduler = parse_scheduler(fixture("sched_gcc281.txt"), symbol)
        facts = {symbol: {"allocation": {"pseudos": {}, "conflicts": {}}, "rtl": {"instructions": {}, "origins": {}}, "scheduler": scheduler}}
        target = match.Target("main_14400", symbol, 0, 0x8008C6B8, 8, 8)
        result = explain.explain_words(bytes.fromhex("0220282d24060020"), bytes.fromhex("240600200220282d"), [target], facts, {0: {"uid": 21}, 4: {"uid": 19}})
        schedule = result[0]["scheduling"]
        self.assertTrue(schedule["mutual_swap"])
        self.assertEqual(schedule["target_word_candidate_offset"], 4)
        self.assertEqual(schedule["shared_ready_lists"][0]["direction"], "backward")
        self.assertEqual(schedule["shared_ready_lists"][0]["selection_order"], [21, 19])

    def test_equal_words_require_no_mapping_or_pseudo_guess(self) -> None:
        self.assertEqual(explain.explain_words(b"\0" * 8, b"\0" * 8, [match.Target("resident", "f", 0, 0, 8, 8)], {}, {}), [])

    def test_unrelated_operand_formats_are_explicitly_unavailable(self) -> None:
        facts = {"f": {"allocation": {"pseudos": {}, "conflicts": {}}, "rtl": {"instructions": {}, "origins": {}}, "scheduler": {"instructions": {}, "decisions": []}}}
        rows = explain.explain_words(bytes.fromhex("2403000a"), bytes.fromhex("0080982d"), [match.Target("resident", "f", 0, 0, 4, 4)], facts, {})
        self.assertTrue(any("Different instruction" in r["reason"] for r in rows[0]["unavailable"]))

    def test_floating_point_register_fields_do_not_decode_format_as_gpr(self) -> None:
        fields = explain.register_fields(0x46041080)  # add.s f2,f2,f4
        self.assertEqual(fields["fd"][0], 34)
        self.assertEqual(fields["ft"][0], 36)
        self.assertNotIn("rs", fields)


class DiagnosticBoundaryTests(unittest.TestCase):
    def test_override_is_unavailable_and_never_rewrites_normal_result(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            directory = Path(temporary)
            normal = '{"status":"match"}\n'
            (directory / "result.json").write_text(normal)
            report = explain.collect(SimpleNamespace(profile_id="profile+diagnostic", status="match"), [], Path("unused.c"), "profile", directory,
                                     source_root=directory, rom=b"")
            self.assertEqual(report["status"], "unavailable")
            self.assertTrue(report["diagnostic_only"])
            self.assertFalse(report["receipt_eligible"])
            self.assertEqual((directory / "result.json").read_text(), normal)
            self.assertTrue((directory / "explain.json").is_file())

    def collect_fixture(self, nonneutral: set[str], *, expect_mapping: bool) -> dict:
        """Exercise collect's gates with real ELF/listing fixtures and a fake
        transport; only the compared object identity is deliberately changed."""
        raw = bytes.fromhex(fixture("engine_gcc281.object.hex"))
        elf = match.Elf(raw)
        code = elf.contents(elf.section(".text"))
        original = code[:0x2C] + bytes.fromhex("24020001") + code[0x30:]
        baseline = explain._object_identity(elf)
        stages = ["da", "annotated", "uid"]
        identities = [baseline] + [
            {**baseline, "functions": [["changed-layout", 0, 1]]} if stage in nonneutral else baseline
            for stage in stages
        ]
        profile = {
            "profiles": {"fixture": {"compiler": "gcc281pm", "compiler_flags": [], "assembler_flags": []}},
            "compilers": {"gcc281pm": {"cc1": {"sha256": "fixture"}}},
        }
        source_note = {"file": "candidate.c", "line": 38}
        rtl = {"instructions": {38: {"uid": 38, "registers": [], "source": source_note}},
               "origins": {80: [source_note]}, "unavailable": []}
        with tempfile.TemporaryDirectory() as temporary:
            directory = Path(temporary)
            source = directory / "candidate.c"
            source.write_text("/* Compiler transport is replaced by recorded fixtures. */\n")
            normal = '{"status":"mismatch"}\n'
            (directory / "result.json").write_text(normal)
            (directory / "candidate.o").write_bytes(raw)
            (directory / "candidate.elf").write_bytes(raw)

            def run(argv, *, cwd, **kwargs):
                cwd = Path(cwd)
                if "-S" in argv:
                    (cwd / "candidate.s").write_text(fixture("engine_gcc281.s"))
                    for suffix, name in (("lreg", "alloc_gcc272.lreg"), ("greg", "alloc_gcc272.greg"),
                                         ("sched2", "sched_gcc281.txt")):
                        (cwd / f"candidate.c.{suffix}").write_text(fixture(name))
                    stdout = ""
                else:
                    (cwd / "diagnostic.o").write_bytes(raw)
                    stdout = fixture("engine_gcc281.listing")
                return SimpleNamespace(returncode=0, stdout=stdout, stderr="")

            with (
                patch.object(match, "tool_profile", return_value=profile),
                patch.object(match, "profile_toolchain", return_value=(["fixture-gcc"], "fixture-as")),
                patch.object(match, "compiler_environment", return_value={}),
                patch.object(explain.subprocess, "run", side_effect=run),
                patch.object(explain, "_object_identity", side_effect=identities),
                patch.object(explain, "rtl_instructions", return_value=rtl),
                patch.object(explain, "explain_words", wraps=explain.explain_words) as mapping,
            ):
                report = explain.collect(
                    SimpleNamespace(profile_id="fixture", status="mismatch", differing_words=1, participating_files=[]),
                    [match.Target("main_14400", "func_8008C6B8", 0, 0, len(code), len(code))],
                    source, "fixture", directory, source_root=directory, rom=original,
                )
                if expect_mapping:
                    mapping.assert_called_once()
                else:
                    mapping.assert_not_called()
            self.assertEqual((directory / "result.json").read_text(), normal)
            self.assertEqual((directory / "candidate.o").read_bytes(), raw)
            self.assertEqual((directory / "candidate.elf").read_bytes(), raw)
            self.assertFalse(report["receipt_eligible"])
            return report

    def test_non_neutral_dump_stage_withholds_all_explanations(self) -> None:
        report = self.collect_fixture({"da"}, expect_mapping=False)
        self.assertEqual(report["status"], "unavailable")
        self.assertEqual(report["differences"], [])
        self.assertEqual(list(report["codegen_neutrality"]), ["da"])
        self.assertFalse(report["codegen_neutrality"]["da"]["equal"])

    def test_non_neutral_uid_fallback_withholds_all_explanations(self) -> None:
        report = self.collect_fixture({"annotated", "uid"}, expect_mapping=False)
        self.assertEqual(report["status"], "unavailable")
        self.assertEqual(report["differences"], [])
        self.assertEqual(list(report["codegen_neutrality"]), ["da", "annotated", "uid"])
        self.assertFalse(report["codegen_neutrality"]["uid"]["equal"])

    def test_non_neutral_debug_uses_neutral_uid_stage_without_source_notes(self) -> None:
        report = self.collect_fixture({"annotated"}, expect_mapping=True)
        self.assertEqual(report["status"], "partial")
        self.assertEqual(report["mapping_stage"], "uid")
        self.assertFalse(report["codegen_neutrality"]["annotated"]["equal"])
        self.assertTrue(report["codegen_neutrality"]["uid"]["equal"])
        self.assertEqual(report["source_notes"]["status"], "unavailable")
        self.assertEqual(len(report["differences"]), 1)
        difference = report["differences"][0]
        self.assertEqual(difference["candidate_instruction"]["uid"], 38)
        self.assertIsNone(difference["source"])
        self.assertEqual(difference["scheduling"]["source_lines"], [])
        self.assertEqual(report["functions"]["func_8008C6B8"]["rtl"]["origins"], {})
        self.assertIn("without source notes", explain.render(report))

    def test_neutral_debug_keeps_source_notes_without_extra_compile(self) -> None:
        report = self.collect_fixture(set(), expect_mapping=True)
        self.assertEqual(report["mapping_stage"], "annotated")
        self.assertEqual(report["source_notes"]["status"], "available")
        self.assertEqual(list(report["codegen_neutrality"]), ["da", "annotated"])
        self.assertEqual(report["differences"][0]["source"]["line"], 38)

    def test_cli_rejects_every_incompatible_explain_mode_before_resolution(self) -> None:
        for incompatible in ("--asm", "--m2c", "--diag-gcc=/other/gcc",
                             "--diag-cflags=-O1", "--diag-as=/other/as", "--diag-asflags="):
            with (
                self.subTest(option=incompatible),
                patch.object(sys, "argv", ["match.py", "f", "candidate.c", "--explain", incompatible]),
                patch.object(match.os, "nice"),
                patch.object(match, "resolve_targets", side_effect=AssertionError("guard must precede resolution")) as resolve,
                redirect_stderr(StringIO()) as stderr,
                self.assertRaises(SystemExit) as raised,
            ):
                match.main()
            self.assertEqual(raised.exception.code, 2)
            self.assertIn("--explain requires", stderr.getvalue())
            resolve.assert_not_called()

    def test_human_no_difference_message(self) -> None:
        text = explain.render({"status": "no-differences", "codegen_neutrality": {}, "differences": [], "unavailable": [], "evidence_directory": "evidence"})
        self.assertIn("No differing", text)
        self.assertIn("never a receipt", text)


if __name__ == "__main__":
    unittest.main()
