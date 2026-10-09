from __future__ import annotations

import json
import sys
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "tools"))
from rtl_sched import parse_scheduler, scheduling_competition


FIXTURES = Path(__file__).parent / "fixtures" / "explain"


def fixture(name: str) -> str:
    return (FIXTURES / name).read_text()


class SchedulerTests(unittest.TestCase):
    def test_real_gcc281_blocks_hazards_and_launches(self):
        parsed = parse_scheduler(fixture("sched_gcc281.txt"), "func_8008C6B8")
        self.assertEqual(parsed["status"], "available")
        self.assertEqual([block["block"] for block in parsed["blocks"]], [0, 7])
        decision = parsed["decisions"][0]
        self.assertEqual(decision["clock"], 10)
        self.assertEqual(decision["hazard"][0]["before"], [21, 19, 17, 130])
        self.assertEqual(decision["order"], [130, 21, 19, 17])
        self.assertEqual(decision["chosen"], 130)
        self.assertEqual(parsed["events"][0]["stalls"], 1)
        self.assertEqual(parsed["events"][0]["clock"], 4)
        self.assertEqual(parsed["blocks"][1]["total_time"], 8)
        self.assertEqual(parsed["instructions"][145]["base_priority"], 2147483556)
        json.dumps(parsed)

    def test_gcc272_reverse_selection_is_not_emitted_order(self):
        parsed = parse_scheduler(fixture("sched_gcc272.txt"), "func_8008C6B8")
        self.assertEqual(parsed["status"], "available")
        self.assertEqual(parsed["direction"], "backward")
        self.assertEqual([item["chosen"] for item in parsed["decisions"]], [54, 52])
        self.assertEqual(parsed["blocks"][0]["new_head"], 52)
        self.assertEqual(parsed["blocks"][0]["new_end"], 54)

    def test_real_birth_promotion_not_fixed_tail_priority(self):
        parsed = parse_scheduler(fixture("sched_gcc272_birth.txt"), "func_8008C6B8")
        self.assertEqual(parsed["status"], "available")
        self.assertFalse(parsed["decisions"][0]["ready"][0]["birth_promotion"])
        promoted = parsed["decisions"][2]["ready"][1]
        self.assertEqual(promoted, {"uid": 42, "priority": 0x7f000001, "birth_promotion": True})
        self.assertEqual(parsed["instructions"][42]["base_priority"], 1)
        self.assertEqual(len(parsed["events"]), 4)

    def test_cpp_banner_and_function_slicing(self):
        text = fixture("sched_gcc272.txt") + fixture("sched_cc1plus.txt")
        parsed = parse_scheduler(text, "func_800BFEA0")
        self.assertEqual(parsed["status"], "available")
        self.assertNotIn(54, parsed["instructions"])
        self.assertEqual(parsed["decisions"][1]["chosen"], 27)
        self.assertEqual(parse_scheduler(text, "void func_800BFEA0(struct Floor *)"),
                         dict(parsed, function="void func_800BFEA0(struct Floor *)"))
        self.assertEqual(parse_scheduler(text, "Floor")["status"], "unavailable")
        self.assertEqual(parse_scheduler(text, "func_800BFE")["status"], "unavailable")

    def test_ambiguous_cpp_overloads_require_full_banner(self):
        text = fixture("sched_cc1plus.txt")
        text += text.replace("struct Floor *", "int")
        parsed = parse_scheduler(text, "func_800BFEA0")
        self.assertEqual(parsed["status"], "unavailable")
        self.assertIn("ambiguous", parsed["unavailable"][0]["reason"])
        self.assertEqual(parse_scheduler(text, "void func_800BFEA0(int)")["status"], "available")

    def test_competition_attaches_base_current_and_actual_selection(self):
        parsed = parse_scheduler(fixture("sched_gcc281.txt"), "func_8008C6B8")
        evidence = scheduling_competition(parsed, [130, 21, 21])
        self.assertEqual(len(evidence), 1)
        self.assertEqual(evidence[0]["selection_order"], [130, 21])
        self.assertEqual(evidence[0]["queried"][0]["base_priority"], 1)
        self.assertEqual(evidence[0]["queried"][0]["priority"], 1)
        self.assertEqual(evidence[0]["direction"], "backward")
        self.assertEqual(scheduling_competition(parsed, [130, 130]), [])
        json.dumps(evidence)

    def test_unknown_summary_is_explicit_and_retains_source_line(self):
        text = fixture("sched_gcc272.txt") + ";; target decision mystery\n"
        parsed = parse_scheduler(text, "func_8008C6B8")
        self.assertEqual(parsed["status"], "partial")
        evidence = parsed["unavailable"][0]
        self.assertEqual(set(evidence), {"stage", "line", "text", "reason"})
        self.assertEqual(evidence["line"], len(text.splitlines()))
        self.assertEqual(evidence["text"], ";; target decision mystery")

    def test_truncated_hazard_does_not_claim_pre_hazard_choice(self):
        text = fixture("sched_cc1plus.txt").split(";; total time")[0]
        text = text.replace("greater potential hazard, now 27 34 46 44 42", "greater potential hazard")
        parsed = parse_scheduler(text, "func_800BFEA0")
        self.assertEqual(parsed["status"], "partial")
        self.assertIsNone(parsed["decisions"][-1]["chosen"])
        self.assertTrue(parsed["decisions"][-1]["unavailable"])

    def test_blocking_split_lines_and_all_blocked(self):
        # Synthetic edge cases using the exact sched.c fprintf grammar.
        prefix = fixture("sched_gcc272.txt").split(";; ready list at T-1")[0]
        text = prefix + (
            ";; ready list at T-1: 54 (7fffffad) 52 (1)\n"
            ";; blocking insn 54 for 2 cycles, now 52\n"
            ";; ready list at T-2: 52 (1)\n"
            ";; blocking insn 52 for 1 cycles\n"
        )
        parsed = parse_scheduler(text, "func_8008C6B8")
        self.assertEqual(parsed["status"], "available")
        self.assertEqual(parsed["decisions"][0]["chosen"], 52)
        self.assertEqual(parsed["decisions"][0]["blocking"][0]["cycles"], 2)
        self.assertIsNone(parsed["decisions"][1]["chosen"])

    def test_malformed_order_and_missing_base_are_not_guessed(self):
        text = fixture("sched_gcc272.txt").replace(", now 54", ", now 999")
        parsed = parse_scheduler(text, "func_8008C6B8")
        self.assertEqual(parsed["status"], "partial")
        self.assertIsNone(parsed["decisions"][0]["chosen"])
        text = fixture("sched_gcc272.txt").replace(";; insn[  54]", ";; missing[  54]")
        parsed = parse_scheduler(text, "func_8008C6B8")
        self.assertIsNone(parsed["decisions"][0]["chosen"])
        self.assertTrue(any("base priority" in row["reason"] for row in parsed["unavailable"]))

    def test_known_annotations_and_empty_input(self):
        text = fixture("sched_gcc272.txt") + (
            ";; added 2 line-number notes\n;; deleted 1 line-number notes\n"
            ";; register 4 now crosses calls\n;; Insn is not within a basic block\n"
            ";; Start of basic block 1.\n;; Registers live: 4 [$4] 29 [$sp]\n"
            ";; End of basic block 1.\n"
        )
        self.assertEqual(parse_scheduler(text, "func_8008C6B8")["status"], "available")
        self.assertEqual(parse_scheduler("", "missing")["status"], "unavailable")
        self.assertEqual(parse_scheduler(";; Function f\n;;\t -- basic block number 0 from 1 to 2 --\n;; already scheduled\n", "f")["status"], "available")


if __name__ == "__main__":
    unittest.main()
