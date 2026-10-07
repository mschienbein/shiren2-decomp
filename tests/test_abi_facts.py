"""Original o32 ABI regressions using splat-shaped, non-executable fixtures."""
from __future__ import annotations

import json
import sys
import unittest
from pathlib import Path

PROJECT_ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(PROJECT_ROOT / "tools"))
from abi_facts import analyze_project, analyze_text  # noqa: E402


def assembly(*functions: tuple[str, list[str]], rodata: str = "") -> str:
    """Assign unique real-shaped ROM/VRAM addresses; labels consume no words."""
    lines = [".section .text"]
    addresses = {symbol: 0x80050000 + index * 0x1000 for index, (symbol, _) in enumerate(functions)}
    for index, (symbol, instructions) in enumerate(functions):
        address = 0x80050000 + index * 0x1000
        rom = 0x23040 + index * 0x1000
        lines.append(f"glabel {symbol}")
        for instruction in instructions:
            if instruction.endswith(":") or instruction.startswith("jlabel "):
                lines.append(instruction)
                continue
            word = "03E00008" if instruction == "jr $ra" else "00000000"
            if instruction.startswith(("jal ", "j ")):
                mnemonic, target = instruction.split(maxsplit=1)
                if target in addresses:
                    opcode = 0x0C000000 if mnemonic == "jal" else 0x08000000
                    word = f"{opcode | ((addresses[target] >> 2) & 0x03FFFFFF):08X}"
            lines.append(f"/* {rom:06X} {address:08X} {word} */ {instruction}")
            address += 4
            rom += 4
        lines.append(f"endlabel {symbol}")
    if rodata:
        lines.extend([".section .rodata", rodata])
    return "\n".join(lines) + "\n"


def facts(*functions: tuple[str, list[str]], rodata: str = "") -> dict[str, dict]:
    report = analyze_text(assembly(*functions, rodata=rodata))
    return {fact["symbol"]: fact for fact in report["functions"]}


def consumer(fact: dict, caller: str) -> dict:
    matches = [item for item in fact["result_consumers"] if item["caller"] == caller]
    if len(matches) != 1:
        raise AssertionError(f"Expected one {caller} call site, got {matches!r}")
    if matches[0]["consumed"] != matches[0]["proven_consumed"]:
        raise AssertionError("consumed must alias proven_consumed")
    if matches[0]["proven_consumed"] and not matches[0]["conservative_consumed"]:
        raise AssertionError("Conservative consumption must include every proven consumer")
    return matches[0]


class AbiFactsSyntheticTests(unittest.TestCase):
    def test_empty_input_and_deterministic_json(self) -> None:
        empty = analyze_text("")
        self.assertEqual(empty["schema_version"], 1)
        self.assertEqual(empty["functions"], [])
        text = assembly(("leaf", ["jr $ra", "nop"]))
        self.assertEqual(json.dumps(analyze_text(text), sort_keys=True),
                         json.dumps(analyze_text(text), sort_keys=True))

    def test_leaf_identity_and_no_arguments(self) -> None:
        report = analyze_text(assembly(("leaf", ["jr $ra", "nop"])), image_id="resident")
        leaf = report["functions"][0]
        self.assertEqual(leaf["image_id"], "resident")
        self.assertEqual(int(leaf["vram_start"], 16), 0x80050000)
        self.assertEqual(int(leaf["rom_start"], 16), 0x23040)
        self.assertEqual(leaf["size"], 8)
        self.assertEqual(leaf["live_in"], 0)
        self.assertEqual(leaf["live_in_registers"], [])
        self.assertFalse(leaf["indirect_conservative"])
        self.assertTrue(leaf["returns_value_paths"]["has_returns"])
        self.assertFalse(leaf["returns_value_paths"]["v0_written_on_all_returns"])

    def test_return_delay_slot_reads_argument(self) -> None:
        leaf = facts(("leaf", ["jr $ra", "addu $v0, $a2, $zero"]))["leaf"]
        self.assertEqual(leaf["live_in"], 3)
        self.assertEqual(leaf["live_in_registers"], ["a2"])
        self.assertTrue(leaf["returns_value_paths"]["v0_written_on_all_returns"])
        evidence = [item for item in leaf["live_in_evidence"] if item["argument_index"] == 2]
        self.assertTrue(evidence)
        self.assertIn(0x80050004, {int(address, 16) for item in evidence for address in item["addresses"]})

    def test_write_before_read_kills_incoming_argument(self) -> None:
        leaf = facts(("leaf", ["addiu $a3, $zero, 9", "jr $ra", "addu $v0, $a3, $zero"]))["leaf"]
        self.assertEqual(leaf["live_in"], 0)

    def test_read_modify_write_still_reads_incoming_argument(self) -> None:
        leaf = facts(("leaf", ["addiu $a1, $a1, 1", "jr $ra", "nop"]))["leaf"]
        self.assertEqual(leaf["live_in_registers"], ["a1"])
        self.assertEqual(leaf["live_in"], 2)

    def test_unreachable_read_does_not_create_argument(self) -> None:
        leaf = facts(("leaf", ["b .Ldone", "nop", "addu $v0, $a3, $zero",
                               ".Ldone:", "jr $ra", "nop"]))["leaf"]
        self.assertEqual(leaf["live_in"], 0)

    def test_branch_likely_delay_read_on_taken_path(self) -> None:
        leaf = facts(("leaf", ["beql $a0, $zero, .Ldone", "addu $v0, $a3, $zero",
                               "addiu $v0, $zero, 1", ".Ldone:", "jr $ra", "nop"]))["leaf"]
        self.assertEqual(leaf["live_in_registers"], ["a0", "a3"])
        self.assertEqual(leaf["live_in"], 4)

    def test_branch_likely_annuls_delay_write_on_fallthrough(self) -> None:
        leaf = facts(("leaf", ["beql $a0, $zero, .Ltaken", "addiu $a2, $zero, 7",
                               "addu $v0, $a2, $zero", "jr $ra", "nop",
                               ".Ltaken:", "jr $ra", "nop"]))["leaf"]
        self.assertEqual(leaf["live_in_registers"], ["a0", "a2"])
        self.assertEqual(leaf["live_in"], 3)

    def test_ordinary_branch_delay_write_executes_on_both_paths(self) -> None:
        leaf = facts(("leaf", ["beq $a0, $zero, .Ltaken", "addiu $a2, $zero, 7",
                               "addu $v0, $a2, $zero", "jr $ra", "nop",
                               ".Ltaken:", "jr $ra", "nop"]))["leaf"]
        self.assertEqual(leaf["live_in_registers"], ["a0"])

    def test_forwarding_reaches_fixed_point_across_three_calls(self) -> None:
        result = facts(
            ("outer", ["jal middle", "nop", "jr $ra", "nop"]),
            ("middle", ["jal inner", "nop", "jr $ra", "nop"]),
            ("inner", ["jal leaf", "nop", "jr $ra", "nop"]),
            ("leaf", ["jr $ra", "addu $v0, $a2, $zero"]),
        )
        for name in result:
            with self.subTest(symbol=name):
                self.assertEqual(result[name]["live_in"], 3)
                self.assertIn("a2", result[name]["live_in_registers"])
        self.assertTrue(any(item.get("callee") == "middle" for item in result["outer"]["live_in_evidence"]))

    def test_recursive_call_graph_converges(self) -> None:
        result = facts(
            ("left", ["addu $s0, $a2, $zero", "jal right", "nop", "jr $ra", "nop"]),
            ("right", ["jal left", "nop", "jr $ra", "nop"]),
        )
        self.assertEqual(result["left"]["live_in"], 3)
        self.assertEqual(result["right"]["live_in"], 3)

    def test_call_delay_slot_sets_callee_argument_before_call(self) -> None:
        result = facts(
            ("caller", ["jal leaf", "addiu $a2, $zero, 1", "jr $ra", "nop"]),
            ("leaf", ["jr $ra", "addu $v0, $a2, $zero"]),
        )
        self.assertEqual(result["caller"]["live_in"], 0)
        self.assertEqual(result["leaf"]["live_in"], 3)

    def test_call_delay_slot_can_forward_different_incoming_register(self) -> None:
        result = facts(
            ("caller", ["jal leaf", "addu $a0, $a2, $zero", "jr $ra", "nop"]),
            ("leaf", ["jr $ra", "addu $v0, $a0, $zero"]),
        )
        self.assertEqual(result["caller"]["live_in_registers"], ["a2"])
        self.assertEqual(result["caller"]["live_in"], 3)

    def test_direct_call_clobbers_argument_registers(self) -> None:
        result = facts(
            ("caller", ["jal leaf", "nop", "addu $v0, $a3, $zero", "jr $ra", "nop"]),
            ("leaf", ["jr $ra", "nop"]),
        )
        self.assertEqual(result["caller"]["live_in"], 0)

    def test_indirect_call_reads_only_unwritten_incoming_registers(self) -> None:
        leaf = facts(("leaf", ["addiu $a0, $zero, 1", "addiu $a1, $zero, 2",
                               "jalr $t9", "addiu $a3, $zero, 4", "jr $ra", "nop"]))["leaf"]
        self.assertTrue(leaf["indirect_conservative"])
        self.assertEqual(leaf["live_in"], 0)
        self.assertEqual(leaf["proven_live_in"], 0)
        self.assertEqual(leaf["conservative_live_in"], 3)

    def test_indirect_call_clobbers_arguments_before_later_read(self) -> None:
        leaf = facts(("leaf", ["addiu $a0, $zero, 0", "addiu $a1, $zero, 0",
                               "addiu $a2, $zero, 0", "addiu $a3, $zero, 0",
                               "jalr $ra, $t9", "nop", "addu $v0, $a3, $zero",
                               "jr $ra", "nop"]))["leaf"]
        self.assertTrue(leaf["indirect_conservative"])
        self.assertEqual(leaf["live_in"], 0)

    def test_indirect_tail_jump_is_conservative(self) -> None:
        leaf = facts(("leaf", ["jr $t9", "nop"]))["leaf"]
        self.assertTrue(leaf["indirect_conservative"])
        self.assertEqual(leaf["live_in"], 0)
        self.assertEqual(leaf["proven_live_in"], 0)
        self.assertEqual(leaf["conservative_live_in"], 4)
        self.assertFalse(leaf["returns_value_paths"]["has_returns"])

    def test_indirect_assumption_propagates_only_conservative_across_direct_calls(self) -> None:
        result = facts(
            ("outer", ["jal middle", "nop", "jr $ra", "nop"]),
            ("middle", ["jal leaf", "nop", "jr $ra", "nop"]),
            ("leaf", ["jalr $t9", "nop", "jr $ra", "nop"]),
        )
        for symbol in ("outer", "middle", "leaf"):
            with self.subTest(symbol=symbol):
                self.assertEqual(result[symbol]["live_in"], 0)
                self.assertEqual(result[symbol]["proven_live_in"], 0)
                self.assertEqual(result[symbol]["conservative_live_in"], 4)

    def test_indirect_target_in_argument_register_is_a_proven_read(self) -> None:
        leaf = facts(("leaf", ["jr $a1", "nop"]))["leaf"]
        self.assertEqual(leaf["live_in"], 2)
        self.assertEqual(leaf["proven_live_in"], 2)
        self.assertEqual(leaf["conservative_live_in"], 4)

    def test_incoming_stack_arguments_are_relative_to_entry_sp(self) -> None:
        leaf = facts(("leaf", ["addiu $sp, $sp, -0x30", "lw $t0, 0x40($sp)",
                               "lw $v0, 0x48($sp)", "jr $ra", "addiu $sp, $sp, 0x30"]))["leaf"]
        self.assertEqual(leaf["live_in"], 7)
        self.assertEqual(leaf["live_in_registers"], [])
        self.assertEqual({(item["argument_index"], item["offset"]) for item in leaf["live_in_stack"]},
                         {(4, 0x10), (6, 0x18)})
        self.assertTrue(all(item["addresses"] for item in leaf["live_in_stack"]))

    def test_local_stack_slots_do_not_count_as_arguments(self) -> None:
        leaf = facts(("leaf", ["addiu $sp, $sp, -0x30", "sw $ra, 0x2c($sp)",
                               "lw $v0, 0x10($sp)", "lw $ra, 0x2c($sp)",
                               "jr $ra", "addiu $sp, $sp, 0x30"]))["leaf"]
        self.assertEqual(leaf["live_in"], 0)
        self.assertEqual(leaf["live_in_stack"], [])

    def test_frameless_stack_argument(self) -> None:
        leaf = facts(("leaf", ["jr $ra", "lw $v0, 0x10($sp)"]))["leaf"]
        self.assertEqual(leaf["live_in"], 5)
        self.assertEqual(leaf["live_in_stack"][0]["argument_index"], 4)

    def test_jump_table_resolves_rodata_targets_without_indirect_fallback(self) -> None:
        leaf = facts(("leaf", ["sll $v0, $a0, 2", "lui $at, %hi(jtbl_80140000)",
                               "addu $at, $at, $v0", "lw $v0, %lo(jtbl_80140000)($at)",
                               "jr $v0", "nop", "jlabel .Lcase0",
                               "jr $ra", "addu $v0, $a2, $zero", ".Lcase1:",
                               "jr $ra", "addu $v0, $a1, $zero"]),
                     rodata="glabel jtbl_80140000\n.word .Lcase0\n.word .Lcase1\n.word 0x00000000\nendlabel jtbl_80140000")["leaf"]
        self.assertFalse(leaf["indirect_conservative"])
        self.assertEqual(leaf["live_in"], 3)
        self.assertEqual(leaf["live_in_registers"], ["a0", "a1", "a2"])
        self.assertEqual(leaf["warnings"], [])

    def test_trailing_handwritten_comment_is_not_part_of_branch_target(self) -> None:
        leaf = facts(("leaf", ["j .Ldone /* handwritten instruction */", "nop",
                               "addu $v0, $a3, $zero", ".Ldone:", "jr $ra", "nop"]))["leaf"]
        self.assertEqual(leaf["proven_live_in"], 0)
        self.assertEqual(leaf["conservative_live_in"], 0)
        self.assertEqual(leaf["warnings"], [])

    def test_varargs_prologue_fixed_parameter_counts(self) -> None:
        for fixed in (1, 2, 3):
            with self.subTest(fixed=fixed):
                saves = [f"sw $a{slot}, 0x{0x20 + 4 * slot:x}($sp)" for slot in range(fixed, 4)]
                leaf = facts(("leaf", ["addiu $sp, $sp, -0x20", *saves,
                                       f"addiu $v0, $sp, 0x{0x20 + 4 * fixed:x}",
                                       "sw $v0, 0x10($sp)", "jr $ra", "addiu $sp, $sp, 0x20"]))["leaf"]
                prologue = leaf["varargs_prologue"]
                self.assertIsNotNone(prologue)
                self.assertEqual(prologue["fixed_parameter_count"], fixed)
                self.assertEqual(prologue["first_variadic_slot"], fixed)
                self.assertEqual(set(prologue["saved_registers"]), {f"a{slot}" for slot in range(fixed, 4)})
                self.assertTrue(prologue["evidence_addresses"])

    def test_homing_registers_without_va_start_is_not_varargs(self) -> None:
        leaf = facts(("leaf", ["sw $a1, 4($sp)", "sw $a2, 8($sp)", "sw $a3, 12($sp)",
                               "jr $ra", "nop"]))["leaf"]
        self.assertIsNone(leaf["varargs_prologue"])

    def test_float_arguments_are_separate_from_integer_arguments(self) -> None:
        leaf = facts(("leaf", ["add.s $f0, $f12, $f14", "jr $ra", "nop"]))["leaf"]
        self.assertEqual(leaf["float_args"], ["f12", "f14"])
        self.assertEqual(leaf["live_in"], 0)
        self.assertEqual({item["register"] for item in leaf["float_evidence"]}, {"f12", "f14"})
        self.assertTrue(leaf["returns_value_paths"]["f0_written_on_all_returns"])

    def test_float_write_before_read_kills_argument(self) -> None:
        leaf = facts(("leaf", ["mtc1 $zero, $f12", "jr $ra", "mov.s $f0, $f12"]))["leaf"]
        self.assertEqual(leaf["float_args"], [])

    def test_consumed_and_discarded_integer_results(self) -> None:
        result = facts(
            ("uses", ["jal leaf", "nop", "sw $v0, 0($s0)", "jr $ra", "nop"]),
            ("discards", ["jal leaf", "nop", "addiu $v0, $zero, 0", "jr $ra", "nop"]),
            ("leaf", ["jr $ra", "addiu $v0, $zero, 1"]),
        )
        used = consumer(result["leaf"], "uses")
        self.assertTrue(used["consumed"])
        self.assertIn("v0", used["registers"])
        self.assertTrue(used["evidence_addresses"])
        self.assertFalse(consumer(result["leaf"], "discards")["consumed"])

    def test_call_delay_read_is_not_consumption_of_new_result(self) -> None:
        result = facts(
            ("caller", ["jal leaf", "sw $v0, 0($s0)", "addiu $v0, $zero, 0", "jr $ra", "nop"]),
            ("leaf", ["jr $ra", "addiu $v0, $zero, 1"]),
        )
        self.assertFalse(consumer(result["leaf"], "caller")["consumed"])

    def test_next_call_overwrites_previous_result(self) -> None:
        result = facts(
            ("caller", ["jal first", "nop", "jal second", "nop", "sw $v0, 0($s0)", "jr $ra", "nop"]),
            ("first", ["jr $ra", "addiu $v0, $zero, 1"]),
            ("second", ["jr $ra", "addiu $v0, $zero, 2"]),
        )
        self.assertFalse(consumer(result["first"], "caller")["consumed"])
        self.assertTrue(consumer(result["second"], "caller")["consumed"])

    def test_next_call_delay_can_consume_previous_result(self) -> None:
        result = facts(
            ("caller", ["jal first", "nop", "jal second", "sw $v0, 0($s0)",
                         "addiu $v0, $zero, 0", "jr $ra", "nop"]),
            ("first", ["jr $ra", "addiu $v0, $zero, 1"]),
            ("second", ["jr $ra", "addiu $v0, $zero, 2"]),
        )
        self.assertTrue(consumer(result["first"], "caller")["consumed"])
        self.assertFalse(consumer(result["second"], "caller")["consumed"])

    def test_float_return_consumption(self) -> None:
        result = facts(
            ("caller", ["jal leaf", "nop", "swc1 $f0, 0($s0)", "jr $ra", "nop"]),
            ("leaf", ["jr $ra", "mov.s $f0, $f12"]),
        )
        use = consumer(result["leaf"], "caller")
        self.assertTrue(use["consumed"])
        self.assertIn("f0", use["registers"])

    def test_result_forwarded_to_indirect_argument_is_only_conservative(self) -> None:
        for call_sequence in (
            ["move $a0, $v0", "jalr $t9", "nop"],
            ["jalr $t9", "addu $a0, $v0, $zero"],
        ):
            with self.subTest(call_sequence=call_sequence):
                result = facts(
                    ("caller", ["jal leaf", "nop", *call_sequence, "jr $ra", "nop"]),
                    ("leaf", ["jr $ra", "addiu $v0, $zero, 1"]),
                )
                use = consumer(result["leaf"], "caller")
                self.assertFalse(use["consumed"])
                self.assertFalse(use["proven_consumed"])
                self.assertTrue(use["conservative_consumed"])

    def test_result_forwarded_to_read_direct_or_tail_argument_is_proven(self) -> None:
        for call, epilogue in (("jal sink", ["jr $ra", "nop"]), ("j sink", [])):
            with self.subTest(call=call):
                result = facts(
                    ("caller", ["jal leaf", "nop", call, "move $a1, $v0", *epilogue]),
                    ("leaf", ["jr $ra", "addiu $v0, $zero, 1"]),
                    ("sink", ["sw $a1, 0($s1)", "jr $ra", "nop"]),
                )
                use = consumer(result["leaf"], "caller")
                self.assertTrue(use["proven_consumed"])
                self.assertTrue(use["conservative_consumed"])

    def test_result_forwarded_to_unused_direct_or_tail_argument_is_not_consumed(self) -> None:
        for call, epilogue in (("jal sink", ["jr $ra", "nop"]), ("j sink", [])):
            with self.subTest(call=call):
                result = facts(
                    ("caller", ["jal leaf", "nop", call, "move $a1, $v0", *epilogue]),
                    ("leaf", ["jr $ra", "addiu $v0, $zero, 1"]),
                    ("sink", ["sw $a0, 0($s1)", "jr $ra", "nop"]),
                )
                use = consumer(result["leaf"], "caller")
                self.assertFalse(use["proven_consumed"])
                self.assertFalse(use["conservative_consumed"])

    def test_result_dependency_survives_transparent_saved_register_chain(self) -> None:
        for sink_body, expected in (
            (["sw $a1, 0($s1)", "jr $ra", "nop"], (True, True)),
            (["jr $ra", "nop"], (False, False)),
            (["jalr $t9", "nop", "jr $ra", "nop"], (False, True)),
        ):
            with self.subTest(sink_body=sink_body):
                result = facts(
                    ("caller", ["jal leaf", "nop", "move $s0, $v0", "jal sink",
                                "or $a1, $s0, $zero", "jr $ra", "nop"]),
                    ("leaf", ["jr $ra", "addiu $v0, $zero, 1"]),
                    ("sink", sink_body),
                )
                use = consumer(result["leaf"], "caller")
                self.assertEqual((use["proven_consumed"], use["conservative_consumed"]), expected)

    def test_overwriting_transparent_result_alias_before_call_kills_dependency(self) -> None:
        result = facts(
            ("caller", ["jal leaf", "nop", "move $a1, $v0", "addiu $a1, $zero, 0",
                        "jal sink", "nop", "jr $ra", "nop"]),
            ("leaf", ["jr $ra", "addiu $v0, $zero, 1"]),
            ("sink", ["sw $a1, 0($s1)", "jr $ra", "nop"]),
        )
        use = consumer(result["leaf"], "caller")
        self.assertFalse(use["proven_consumed"])
        self.assertFalse(use["conservative_consumed"])

    def test_arithmetic_and_memory_result_dependencies_are_proven(self) -> None:
        for operation in (
            "addiu $t0, $v0, 1",
            "addu $t0, $v0, $s1",
            "sw $v0, 0($s1)",
            "lw $t0, 0($v0)",
            "sw $zero, 0($v0)",
        ):
            with self.subTest(operation=operation):
                result = facts(
                    ("caller", ["jal leaf", "nop", operation, "addiu $v0, $zero, 0",
                                "jr $ra", "nop"]),
                    ("leaf", ["jr $ra", "addiu $v0, $zero, 1"]),
                )
                self.assertTrue(consumer(result["leaf"], "caller")["proven_consumed"])

    def test_branch_on_transparent_result_alias_is_proven(self) -> None:
        result = facts(
            ("caller", ["jal leaf", "nop", "move $s0, $v0", "beq $s0, $zero, .Ldone",
                        "nop", "addiu $v0, $zero, 0", ".Ldone:", "jr $ra", "nop"]),
            ("leaf", ["jr $ra", "addiu $v0, $zero, 1"]),
        )
        self.assertTrue(consumer(result["leaf"], "caller")["proven_consumed"])

    def test_result_used_as_indirect_target_is_proven(self) -> None:
        for transfer in ("jalr $t9", "jr $t9"):
            with self.subTest(transfer=transfer):
                result = facts(
                    ("caller", ["jal leaf", "nop", "move $t9, $v0", transfer, "nop",
                                "jr $ra", "nop"]),
                    ("leaf", ["jr $ra", "addiu $v0, $zero, 1"]),
                )
                self.assertTrue(consumer(result["leaf"], "caller")["proven_consumed"])

    def test_wrapper_propagates_conservative_only_downstream_result_demand(self) -> None:
        result = facts(
            ("caller", ["jal outer", "nop", "jalr $t9", "move $a0, $v0", "jr $ra", "nop"]),
            ("outer", ["jal inner", "nop", "jr $ra", "nop"]),
            ("inner", ["jal leaf", "nop", "jr $ra", "nop"]),
            ("leaf", ["jr $ra", "addiu $v0, $zero, 1"]),
        )
        for callee, caller in (("outer", "caller"), ("inner", "outer"), ("leaf", "inner")):
            with self.subTest(callee=callee, caller=caller):
                use = consumer(result[callee], caller)
                self.assertFalse(use["consumed"])
                self.assertFalse(use["proven_consumed"])
                self.assertTrue(use["conservative_consumed"])

    def test_wrapper_result_consumption_propagates_multiple_levels(self) -> None:
        result = facts(
            ("uses", ["jal outer", "nop", "sw $v0, 0($s0)", "jr $ra", "nop"]),
            ("outer", ["jal inner", "nop", "jr $ra", "nop"]),
            ("inner", ["jal leaf", "nop", "jr $ra", "nop"]),
            ("leaf", ["jr $ra", "addiu $v0, $zero, 1"]),
        )
        self.assertTrue(consumer(result["outer"], "uses")["consumed"])
        self.assertTrue(consumer(result["inner"], "outer")["consumed"])
        propagated = consumer(result["leaf"], "inner")
        self.assertTrue(propagated["consumed"])
        self.assertTrue(propagated["propagation"])

    def test_unused_wrapper_does_not_establish_consumption(self) -> None:
        result = facts(
            ("wrapper", ["jal leaf", "nop", "jr $ra", "nop"]),
            ("leaf", ["jr $ra", "addiu $v0, $zero, 1"]),
        )
        self.assertFalse(consumer(result["leaf"], "wrapper")["consumed"])

    def test_overwritten_wrapper_result_does_not_propagate(self) -> None:
        result = facts(
            ("uses", ["jal wrapper", "nop", "sw $v0, 0($s0)", "jr $ra", "nop"]),
            ("wrapper", ["jal leaf", "nop", "jr $ra", "addiu $v0, $zero, 0"]),
            ("leaf", ["jr $ra", "addiu $v0, $zero, 1"]),
        )
        self.assertTrue(consumer(result["wrapper"], "uses")["consumed"])
        self.assertFalse(consumer(result["leaf"], "wrapper")["consumed"])

    def test_tail_call_forwards_arguments_and_consumed_result(self) -> None:
        result = facts(
            ("uses", ["jal wrapper", "nop", "sw $v0, 0($s0)", "jr $ra", "nop"]),
            ("wrapper", ["j leaf", "nop"]),
            ("leaf", ["jr $ra", "addu $v0, $a2, $zero"]),
        )
        self.assertEqual(result["wrapper"]["live_in"], 3)
        use = consumer(result["leaf"], "wrapper")
        self.assertTrue(use["tail"])
        self.assertTrue(use["consumed"])

    def test_value_written_on_every_return_path(self) -> None:
        for second_delay, expected in (("nop", False), ("addiu $v0, $zero, 2", True)):
            with self.subTest(second_delay=second_delay):
                leaf = facts(("leaf", ["beq $a0, $zero, .Lsecond", "nop", "jr $ra",
                                       "addiu $v0, $zero, 1", ".Lsecond:", "jr $ra", second_delay]))["leaf"]
                paths = leaf["returns_value_paths"]
                self.assertTrue(paths["has_returns"])
                self.assertEqual(paths["v0_written_on_all_returns"], expected)
                self.assertEqual(len(paths["return_addresses"]), 2)


    def test_complementary_unaligned_load_pair_does_not_use_old_argument(self) -> None:
        result = facts(("leaf", ["lwl $a3, 0($a0)", "lwr $a3, 3($a0)",
                                 "sw $a3, 0($s0)", "jr $ra", "nop"]))
        self.assertEqual(result["leaf"]["proven_live_in"], 1)

    def test_standalone_unaligned_load_preserves_incoming_argument_bytes(self) -> None:
        result = facts(("leaf", ["lwl $a3, 0($a0)", "sw $a3, 0($s0)", "jr $ra", "nop"]))
        self.assertEqual(result["leaf"]["proven_live_in"], 4)

    def test_unaligned_delay_load_is_not_folded_across_taken_branch(self) -> None:
        result = facts(("leaf", ["beq $a0, $zero, .Ldone", "lwl $a3, 0($a1)",
                                 "lwr $a3, 3($a1)", ".Ldone:", "sw $a3, 0($s0)",
                                 "jr $ra", "nop"]))
        self.assertEqual(result["leaf"]["proven_live_in"], 4)

    def test_double_conversion_kills_both_result_halves(self) -> None:
        result = facts(
            ("caller", ["jal leaf", "nop", "lwc1 $f0, 0($s0)", "cvt.d.s $f0, $f0",
                        "mul.d $f0, $f0, $f4", "jr $ra", "nop"]),
            ("leaf", ["jr $ra", "nop"]),
        )
        self.assertFalse(consumer(result["leaf"], "caller")["conservative_consumed"])

    def test_repurposed_argument_homes_are_not_varargs(self) -> None:
        result = facts(("leaf", ["sw $a1, 4($sp)", "sw $a2, 8($sp)", "sw $a3, 12($sp)",
                                 "sw $zero, 4($sp)", "addiu $v0, $sp, 4",
                                 "lw $v1, 0($v0)", "jr $ra", "nop"]))
        self.assertIsNone(result["leaf"]["varargs_prologue"])

    def test_unused_incoming_home_spills_are_not_live_ins(self) -> None:
        for frame in (0, 32):
            with self.subTest(frame=frame):
                instructions = ([f"addiu $sp, $sp, -{frame}"] if frame else [])
                instructions += [f"sw $a{slot}, {frame + 4 * slot}($sp)" for slot in range(3)]
                instructions += ["jr $ra", f"sw $a3, {frame + 12}($sp)"]
                result = facts(("leaf", instructions), ("caller", ["j leaf", "nop"]))
                leaf = result["leaf"]
                self.assertEqual(leaf["proven_live_in"], 0)
                self.assertEqual(leaf["conservative_live_in"], 0)
                self.assertEqual([spill["argument_index"] for spill in leaf["dead_home_spills"]],
                                 [0, 1, 2, 3])
                self.assertTrue(leaf["spill_only_varargs"]["compatible"])
                self.assertIsNone(leaf["varargs_prologue"])
                self.assertEqual(result["caller"]["proven_live_in"], 0)

    def test_home_reload_or_other_read_preserves_input(self) -> None:
        for read in ("lw $v0, 0($sp)", "lbu $v0, 1($sp)", "addiu $v0, $a0, 1"):
            with self.subTest(read=read):
                leaf = facts(("leaf", ["sw $a0, 0($sp)", read, "jr $ra", "nop"]))["leaf"]
                self.assertEqual(leaf["proven_live_in"], 1)
                self.assertFalse(leaf["dead_home_spills"])

    def test_store_outside_own_home_remains_consumption(self) -> None:
        leaf = facts(("leaf", ["sw $a0, 4($sp)", "jr $ra", "nop"]))["leaf"]
        self.assertEqual(leaf["proven_live_in"], 1)
        self.assertFalse(leaf["dead_home_spills"])

    def test_home_address_escapes_preserve_input(self) -> None:
        escapes = (
            ["addiu $v0, $sp, 0"],
            ["addiu $t0, $sp, 0", "sw $t0, -4($sp)"],
            ["addiu $a0, $sp, 0", "jal sink", "nop"],
            ["addu $t0, $sp, $t1", "lw $v0, 0($t0)"],
            ["bnez $t1, .Lother", "nop", "addiu $t0, $sp, 0",
             "b .Ljoin", "nop", ".Lother:", "addiu $t0, $sp, 4",
             ".Ljoin:", "lw $v0, 0($t0)"],
        )
        for instructions in escapes:
            with self.subTest(escape=instructions):
                leaf = facts(("leaf", ["sw $a0, 0($sp)", *instructions, "jr $ra", "nop"]),
                             ("sink", ["jr $ra", "nop"]))["leaf"]
                self.assertEqual(leaf["proven_live_in"], 1)
                self.assertFalse(leaf["dead_home_spills"])

    def test_unobserved_home_address_alone_is_not_an_escape(self) -> None:
        leaf = facts(("leaf", ["sw $a0, 0($sp)", "addiu $t0, $sp, 0",
                               "jr $ra", "nop"]))["leaf"]
        self.assertEqual(leaf["proven_live_in"], 0)

    def test_homing_does_not_hide_callee_input(self) -> None:
        result = facts(
            ("caller", ["sw $a2, 8($sp)", "jal leaf", "nop", "jr $ra", "nop"]),
            ("leaf", ["addiu $v0, $a2, 1", "jr $ra", "nop"]),
        )
        self.assertEqual(result["caller"]["proven_live_in"], 3)
        self.assertIsNone(result["caller"]["spill_only_varargs"])

    def test_variadic_optional_inputs_do_not_propagate_to_callers(self) -> None:
        for fixed in (1, 2, 3):
            with self.subTest(fixed=fixed):
                saves = [f"sw $a{slot}, {4 * slot}($sp)" for slot in range(fixed, 4)]
                result = facts(
                    ("caller", ["jal variadic", "nop", "jr $ra", "nop"]),
                    ("tail", ["j caller", "nop"]),
                    ("variadic", [*saves, f"addiu $t0, $sp, {4 * fixed}", "sw $t0, -4($sp)",
                                  "lw $v0, 16($sp)", f"addu $v0, $v0, $a{fixed - 1}",
                                  "jalr $t9", "nop", "jr $ra", "nop"]),
                )
                self.assertEqual(result["variadic"]["proven_live_in"], 5)
                self.assertEqual(result["variadic"]["varargs_prologue"]["first_variadic_slot"], fixed)
                for symbol in ("caller", "tail"):
                    self.assertEqual(result[symbol]["proven_live_in"], fixed)
                    self.assertEqual(result[symbol]["conservative_live_in"], fixed)
                    self.assertTrue(all(row["argument_index"] < fixed
                                        for row in result[symbol]["live_in_evidence"]))

    def test_dead_homing_is_not_result_consumption(self) -> None:
        result = facts(
            ("caller", ["jal value", "nop", "move $a0, $v0",
                        "jal stub", "nop", "jr $ra", "nop"]),
            ("value", ["li $v0, 5", "jr $ra", "nop"]),
            ("stub", ["sw $a0, 0($sp)", "sw $a1, 4($sp)", "sw $a2, 8($sp)",
                      "jr $ra", "sw $a3, 12($sp)"]),
        )
        self.assertFalse(consumer(result["value"], "caller")["conservative_consumed"])


class AbiFactsOriginalAssemblyTests(unittest.TestCase):
    """Required accepted-build checks: missing project data is an error, not a skip."""

    @classmethod
    def setUpClass(cls) -> None:
        cls.report = analyze_project(PROJECT_ROOT)
        cls.by_symbol = {item["symbol"]: item for item in cls.report["functions"]
                         if item["image_id"] == "main_14400"}

    def test_forwarded_three_argument_contract(self) -> None:
        for symbol in ("func_8005ECF8", "func_8005ECA8"):
            with self.subTest(symbol=symbol):
                self.assertGreaterEqual(self.by_symbol[symbol]["live_in"], 3)
                self.assertIn("a1", self.by_symbol[symbol]["live_in_registers"])
                self.assertIn("a2", self.by_symbol[symbol]["live_in_registers"])

    def test_original_variadic_prologues(self) -> None:
        for symbol in ("func_800497F0", "func_80049AE8"):
            with self.subTest(symbol=symbol):
                prologue = self.by_symbol[symbol]["varargs_prologue"]
                self.assertIsNotNone(prologue)
                self.assertTrue(prologue["evidence_addresses"])

    def test_original_two_argument_function(self) -> None:
        self.assertEqual(self.by_symbol["func_80121D80"]["live_in"], 2)
        self.assertEqual(self.by_symbol["func_80121D80"]["proven_live_in"], 2)
        self.assertEqual(self.by_symbol["func_80121D80"]["conservative_live_in"], 4)

    def test_original_empty_function(self) -> None:
        self.assertEqual(self.by_symbol["func_800D8FE8"]["live_in"], 0)

    def test_original_unused_variadic_stubs(self) -> None:
        resident = {row["symbol"]: row for row in self.report["functions"]
                    if row["image_id"] == "resident"}
        for symbol in ("func_80033048", "func_8003305C"):
            with self.subTest(symbol=symbol):
                self.assertEqual(resident[symbol]["proven_live_in"], 0)
                self.assertEqual(resident[symbol]["conservative_live_in"], 0)
                self.assertTrue(resident[symbol]["spill_only_varargs"]["compatible"])
                self.assertEqual(len(resident[symbol]["dead_home_spills"]), 4)

    def test_original_variadic_callers_need_only_fixed_inputs(self) -> None:
        limits = {"func_8005457C": 1, "func_800A665C": 2, "func_800B512C": 1,
                  "func_800C93F4": 0, "func_800E20F0": 1, "func_8011FD98": 3}
        for symbol, limit in limits.items():
            with self.subTest(symbol=symbol):
                self.assertEqual(self.by_symbol[symbol]["proven_live_in"], limit)


if __name__ == "__main__":
    unittest.main()
