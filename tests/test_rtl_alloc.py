"""Real GCC 2.7.2 and 2.8.1 cc1plus allocation summaries and edge cases."""
from __future__ import annotations

import json
import sys
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "tools"))
from rtl_alloc import flip_thresholds, global_priority, parse_allocation

FIXTURES = Path(__file__).parent / "fixtures" / "explain"


def fixture(prefix: str, stage: str) -> str:
    return (FIXTURES / f"alloc_{prefix}.{stage}").read_text()


class AllocationTests(unittest.TestCase):
    def test_real_gcc272(self) -> None:
        report = parse_allocation(fixture("gcc272", "lreg"), fixture("gcc272", "greg"), "func_8008C6B8")
        self.assertEqual(report["unavailable"], [])
        self.assertEqual(report["status"], "available")
        self.assertEqual(report["allocation_order"], [77, 79, 74, 73])
        self.assertEqual(report["preferences"][77], [2, 4])
        self.assertEqual(report["conflicts"][79], [73, 74, 79, 29])
        self.assertEqual(report["pseudos"][77]["priority"], 20000)
        self.assertEqual(report["pseudos"][79]["priority"], 20000)
        self.assertEqual(report["pseudos"][76]["allocation"], "local")
        self.assertIsNone(report["pseudos"][76]["priority"])
        self.assertEqual(report["pseudos"][76]["size_bytes"], 1)
        self.assertEqual(report["pseudos"][76]["size_words"], 1)
        self.assertEqual(report["pseudos"][74]["hard_register"], 16)
        json.dumps(report)

    def test_real_cc1plus281(self) -> None:
        report = parse_allocation(fixture("gxx281", "lreg"), fixture("gxx281", "greg"), "func_800BFEA0")
        self.assertEqual(report["unavailable"], [])
        self.assertEqual(report["allocation_order"], [168, 117, 169, 80, 167, 91, 96, 93, 126, 97, 94, 113, 107])
        self.assertEqual(report["pseudos"][168]["priority"], global_priority(11, 21, 1))
        self.assertEqual(report["pseudos"][87]["allocation"], "local")
        self.assertEqual(report["pseudos"][163]["size_bytes"], 1)
        self.assertEqual(report["pseudos"][80]["size_bytes"], 4)

    def test_real_reload_diagnostics_are_known_rows(self) -> None:
        # Verbatim KMC controller_init / func_800302D0 greg rows, before the
        # final dispositions. Reload's chatter must not replace those results.
        local, glob = fixture("gcc272", "lreg"), fixture("gcc272", "greg")
        rows = (FIXTURES / "reload_gcc272.txt").read_text()
        changed = glob.replace(";; Register dispositions:", rows + ";; Register dispositions:")
        self.assertEqual(parse_allocation(local, changed, "func_8008C6B8"),
                         parse_allocation(local, glob, "func_8008C6B8"))

    def test_reload_format_variants_and_unknown_rows(self) -> None:
        local, glob = fixture("gcc272", "lreg"), fixture("gcc272", "greg")
        # Grammar boundary cases from reload1.c format strings, not claimed
        # to be additional observed dump rows.
        for row in (
            ";; Need 1 nongroup reg of class GR_REGS (for insn 50).",
            ";; Need 2 nongroup regs of class GR_REGS (for insn 50).",
            ";; Need 2 regs of class GR_REGS (for insn 50).",
            ";; Need 2 groups (DImode) of class GR_REGS (for insn 77).",
            " Register 73 now on stack.", " Register 73 now in 3.",
        ):
            with self.subTest(row=row):
                changed = glob.replace(";; Register dispositions:", row + "\n;; Register dispositions:")
                report = parse_allocation(local, changed, "func_8008C6B8")
                self.assertEqual(report["status"], "available")
                self.assertEqual(report["unavailable"], [])
                self.assertEqual(report["pseudos"][73]["hard_register"], 17)
        for row in (";; Need 1 reg (DImode) of class GR_REGS (for insn 50).",
                    "Register 73 now unknown.", "Register 73 now in 3. extra"):
            with self.subTest(row=row):
                changed = glob.replace(";; Register dispositions:", row + "\n;; Register dispositions:")
                report = parse_allocation(local, changed, "func_8008C6B8")
                self.assertEqual(report["status"], "partial")
                self.assertEqual(report["unavailable"][0]["text"], row)

    def test_unknown_row_keeps_known_facts(self) -> None:
        local = fixture("gcc272", "lreg").replace("81 registers.", "81 registers.\nnew allocation metric: 42")
        report = parse_allocation(local, fixture("gcc272", "greg"), "func_8008C6B8")
        self.assertEqual(report["status"], "partial")
        self.assertEqual(report["pseudos"][74]["hard_register"], 16)
        self.assertEqual(set(report["unavailable"][0]), {"stage", "line", "text", "reason"})
        self.assertIn("new allocation metric", report["unavailable"][0]["text"])

    def test_malformed_order_disposition_and_metrics(self) -> None:
        local = fixture("gcc272", "lreg")
        glob = fixture("gcc272", "greg")
        for changed_local, changed_global in [
            (local, glob.replace("4 regs to allocate", "5 regs to allocate")),
            (local, glob.replace("73 in 17", "73 in unknown")),
            (local.replace("used 5 times", "used -5 times"), glob),
            (local.replace("35 insns", "0 insns"), glob),
            (local.replace("1 bytes", "0 bytes"), glob),
            (local, glob.replace("73 in 17", "73 in 999")),
            (local, glob.replace(";; 77 preferences: 2 4", ";; 77 preferences: 2 wrong")),
        ]:
            with self.subTest(local=changed_local, glob=changed_global):
                report = parse_allocation(changed_local, changed_global, "func_8008C6B8")
                self.assertEqual(report["status"], "partial")
                self.assertTrue(report["unavailable"])

    def test_missing_function_and_exact_boundaries(self) -> None:
        local, glob = fixture("gcc272", "lreg"), fixture("gcc272", "greg")
        report = parse_allocation(local, glob, "func_missing")
        self.assertEqual(report["status"], "unavailable")
        self.assertEqual(len(report["unavailable"]), 2)
        report = parse_allocation(local + "\n;; Function func_other\nBAD\n", glob, "func_8008C6B8")
        self.assertEqual(report["status"], "available")
        report = parse_allocation(local, glob, "func_8008C6")
        self.assertEqual(report["status"], "unavailable")

    def test_unassigned_and_shared_allocno(self) -> None:
        local, glob = fixture("gcc272", "lreg"), fixture("gcc272", "greg")
        report = parse_allocation(local, glob.replace("73 in 17  ", ""), "func_8008C6B8")
        self.assertEqual(report["pseudos"][73]["allocation"], "unallocated")
        self.assertIsNone(report["pseudos"][73]["hard_register"])
        report = parse_allocation(local, glob.replace("77 79 74 73", "77+78 79 74 73"), "func_8008C6B8")
        self.assertEqual(report["status"], "partial")
        self.assertIsNone(report["pseudos"][77]["priority"])

    def test_priority_truncation_word_multiplier_and_sentinels(self) -> None:
        self.assertEqual(global_priority(7, 33, 1), 4242)
        self.assertEqual(global_priority(7, 33, 2), 8484)
        self.assertEqual(global_priority(1, 33, 1), 0)
        for values in [(0, 1, 1), (1, 0, 1), (1, -2, 1), (1, 1, 0)]:
            with self.assertRaises(ValueError):
                global_priority(*values)

    def test_flip_thresholds_include_tiebreak(self) -> None:
        candidate = dict(pseudo=79, refs=4, live_length=4, size_words=1, allocation="global")
        holder = {**candidate, "pseudo": 77}
        thresholds = flip_thresholds(candidate, holder)
        self.assertFalse(thresholds["candidate_beats_holder"])
        for field in ("candidate_refs_increase", "candidate_live_reduction", "holder_refs_reduction", "holder_live_increase"):
            self.assertEqual(thresholds[field], 1)
        reverse = flip_thresholds(holder, candidate)
        self.assertTrue(reverse["candidate_beats_holder"])
        self.assertEqual(reverse["candidate_refs_increase"], 0)
        # An explicit allocno overrides the pseudo fallback.
        reverse = flip_thresholds({**candidate, "allocno": 0}, {**holder, "allocno": 1})
        self.assertTrue(reverse["candidate_beats_holder"])

    def test_threshold_minimality(self) -> None:
        candidate = dict(pseudo=90, refs=3, live_length=15, size_words=1)
        holder = dict(pseudo=80, refs=8, live_length=12, size_words=1)
        thresholds = flip_thresholds(candidate, holder)
        def wins(c, h):
            return (-global_priority(c["refs"], c["live_length"], c["size_words"]), c["pseudo"]) < (-global_priority(h["refs"], h["live_length"], h["size_words"]), h["pseudo"])
        for key, which, field, direction in [
            ("candidate_refs_increase", "candidate", "refs", 1),
            ("candidate_live_reduction", "candidate", "live_length", -1),
            ("holder_refs_reduction", "holder", "refs", -1),
            ("holder_live_increase", "holder", "live_length", 1),
        ]:
            delta = thresholds[key]
            self.assertIsNotNone(delta)
            for value, expected in [(delta, True), (delta - 1, False)]:
                c, h = dict(candidate), dict(holder)
                row = c if which == "candidate" else h
                row[field] += value * direction
                self.assertEqual(wins(c, h), expected)

    def test_local_thresholds_are_unavailable(self) -> None:
        local = dict(pseudo=80, refs=5, live_length=10, size_words=1, allocation="local")
        result = flip_thresholds(local, {**local, "pseudo": 81})
        self.assertTrue(result["unavailable"])
        self.assertIsNone(result["candidate_refs_increase"])
        json.dumps(result)


if __name__ == "__main__":
    unittest.main()
