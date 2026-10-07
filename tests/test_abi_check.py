"""Frozen o32 inventory checks, including slot alignment and evidence certainty."""
from __future__ import annotations

import contextlib
import io
import json
from pathlib import Path
import sys
import tempfile
import unittest

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "tools"))
import abi_check  # noqa: E402


def fact(**updates):
    required = updates.get('proven_live_in', updates.get('live_in', 2))
    evidence = [{'argument_index': required - 1, 'addresses': ['0x80001004']}] if required else []
    return {'symbol': 'target', 'image_id': 'main_14400', 'vram_start': '0x80001000', 'live_in': 2, 'live_in_evidence': evidence, 'varargs_prologue': None, 'result_consumers': [], **updates}


def record(declaration='void target(int a)', **updates):
    return {'symbol': 'target', 'source': 'src/unit.c', 'return_type': 'void', 'declaration': declaration, **updates}


def check(row=None, original=None, **inventory):
    return abi_check.check_inventory({'declarations': [row or record()], **inventory}, {'functions': [original or fact()]})


def kinds(report):
    return {f['kind'] for f in report['findings']}


class AbiCheckTests(unittest.TestCase):

    def test_fallback_declaration_lost_argument_has_addresses(self) -> None:
        report = check()
        self.assertEqual(kinds(report), {'lost-argument'})
        finding = report['findings'][0]
        self.assertEqual(finding['declared_slots'], 1)
        self.assertEqual(finding['evidence_addresses'], ['0x80001004'])
        self.assertEqual(finding['category'], 'canonical-only')
        self.assertEqual(report['summary']['errors'], 1)

    def test_even_slot_alignment_for_wide_parameters(self) -> None:
        for parameter in ['double b', 'long long b', 'unsigned long long b', 's64 b', 'f64 b']:
            with self.subTest(parameter=parameter):
                report = check(record(f'void target(int a, {parameter})'), fact(live_in=4))
                self.assertFalse(report['findings'])
                self.assertFalse(report['uncertainties'])

    def test_alignment_hole_does_not_supply_a_proven_register_argument(self) -> None:
        for variadic in (False, True):
            with self.subTest(variadic=variadic):
                suffix = ', ...' if variadic else ''
                row = record(f'void target(int a, long long b{suffix})')
                prologue = {'first_variadic_slot': 4} if variadic else None
                original = fact(live_in=4, live_in_registers=['a1', 'a3'], varargs_prologue=prologue)
                report = check(row, original)
                self.assertEqual(kinds(report), {'lost-argument'})
                self.assertEqual(report['findings'][0]['missing_slots'], [1])
                self.assertEqual(report['summary']['errors'], 1)

    def test_alignment_hole_does_not_supply_a_proven_stack_argument(self) -> None:
        row = record('void target(int a, int b, int c, int d, int e, long long f)')
        original = fact(live_in=8, live_in_stack=[{'argument_index': 5, 'offset': 20, 'addresses': ['0x80001008']}])
        report = check(row, original)
        self.assertEqual(kinds(report), {'lost-argument'})
        self.assertEqual(report['findings'][0]['missing_slots'], [5])
        self.assertEqual(report['findings'][0]['declared_slots'], 8)

    def test_indirect_alignment_hole_is_advisory_not_a_proven_loss(self) -> None:
        original = fact(live_in=4, proven_live_in=4, conservative_live_in=4,
                        live_in_registers=['a0', 'a2', 'a3'],
                        conservative_live_in_registers=['a0', 'a1', 'a2', 'a3'],
                        live_in_evidence=[{'argument_index': 1, 'certainty': 'indirect-conservative', 'addresses': ['0x80001008']}])
        report = check(record('void target(int a, long long b)'), original)
        self.assertEqual(kinds(report), {'possible-lost-argument (indirect)'})
        self.assertEqual(report['findings'][0]['missing_slots'], [1])
        self.assertEqual(report['summary']['errors'], 0)

    def test_parameter_after_aligned_pair_occupies_stack_slot(self) -> None:
        report = check(record('void target(int a, double b, int c)'), fact(live_in=6))
        self.assertEqual(report['findings'][0]['declared_slots'], 5)

    def test_signature_takes_precedence_over_declaration(self) -> None:
        report = check(record(signature={'params': [{'type': 'int'}, {'type': 'int'}], 'variadic': False}))
        self.assertFalse(report['findings'])

    def test_known_aggregate_layout_and_hidden_return_pointer(self) -> None:
        row = record('Pair target(Pair p)', return_type='Pair')
        report = check(row, fact(live_in=4), types={'Pair': {'kind': 'struct', 'size': 12, 'alignment': 4}})
        self.assertFalse(report['findings'])
        self.assertFalse(report['uncertainties'])

    def test_known_aggregate_alignment_after_hidden_return_pointer(self) -> None:
        row = record('struct Opaque target(Pair p)', return_type='struct Opaque')
        report = check(row, fact(live_in=4), types={'Pair': {'kind': 'struct', 'size': 8, 'alignment': 8}})
        self.assertFalse(report['findings'])

    def test_incomplete_aggregate_is_uncertainty_not_lost_argument(self) -> None:
        report = check(record('void target(struct Opaque value)'), fact(live_in=8))
        self.assertFalse(report['findings'])
        self.assertEqual(report['uncertainties'][0]['symbol'], 'target')

    def test_unsupported_type_is_not_truncated_to_smaller_scalar(self) -> None:
        for parameter in ['Missing value']:
            with self.subTest(parameter=parameter):
                report = check(record(f'void target({parameter})'), fact(live_in=8))
                self.assertFalse(report['findings'])
                self.assertTrue(report['uncertainties'])

    def test_pointer_array_and_callback_parameters_decay(self) -> None:
        row = record('void target(struct Missing *x, int a[4], void (*cb)(int, int))')
        report = check(row, fact(live_in=3))
        self.assertFalse(report['findings'])
        self.assertFalse(report['uncertainties'])

    def test_unprototyped_declaration_does_not_mean_zero_parameters(self) -> None:
        report = check(record('void target()'))
        self.assertFalse(report['findings'])
        self.assertEqual(report['uncertainties'][0]['symbol'], 'target')

    def test_varargs_spills_do_not_count_as_fixed_parameters(self) -> None:
        original = fact(live_in=8, varargs_prologue={'fixed_parameter_count': 1, 'first_variadic_slot': 1, 'saved_registers': ['a1', 'a2', 'a3'], 'evidence_addresses': ['0x80001000']})
        report = check(record('void target(int a, ...)'), original)
        self.assertFalse(report['findings'])

    def test_variadic_mismatch_in_both_directions(self) -> None:
        report = check(record('void target(int a, ...)'), fact(live_in=1))
        self.assertEqual(kinds(report), {'variadic-mismatch'})
        report = check(record(), fact(live_in=4, varargs_prologue={'first_variadic_slot': 1, 'evidence_addresses': ['0x80001008']}))
        self.assertEqual(kinds(report), {'variadic-mismatch'})
        self.assertEqual(report['findings'][0]['evidence_addresses'], ['0x80001008'])

    def test_varargs_fixed_slot_boundary_is_enforced(self) -> None:
        report = check(record('void target(int a, ...)'), fact(live_in=4, varargs_prologue={'first_variadic_slot': 2, 'evidence_addresses': ['0x80001008']}))
        self.assertEqual(kinds(report), {'lost-argument'})

    def test_unused_home_spill_stub_accepts_variadic_declaration(self) -> None:
        original = fact(live_in=0, proven_live_in=0, conservative_live_in=0, spill_only_varargs={'compatible': True, 'evidence_addresses': ['0x80001000']})
        report = check(record('void target(const char *format, ...)'), original)
        self.assertEqual(kinds(report), {'extra-argument'})
        self.assertEqual(report['summary']['errors'], 0)

    def test_unused_home_spills_do_not_require_variadic_declaration(self) -> None:
        original = fact(live_in=0, proven_live_in=0, conservative_live_in=0, spill_only_varargs={'compatible': True})
        report = check(record('void target(void)'), original)
        self.assertFalse(report['findings'])

    def test_extra_arguments_are_informational(self) -> None:
        report = check(record('void target(int a, int b)'), fact(live_in=0))
        self.assertEqual(kinds(report), {'extra-argument'})
        self.assertEqual(report['summary']['errors'], 0)

    def test_consumed_void_result_includes_caller_and_read_evidence(self) -> None:
        consumer = {'caller': 'caller', 'call_address': '0x80002000', 'consumed': True, 'evidence_addresses': ['0x80002008']}
        report = check(original=fact(live_in=1, result_consumers=[consumer]), definitions=[{'symbol': 'caller', 'source': 'src/caller.c', 'return_type': 'void', 'declaration': 'void caller(void)'}])
        finding = report['findings'][0]
        self.assertEqual(finding['kind'], 'void-consumed')
        self.assertEqual(finding['sources'], ['src/caller.c', 'src/unit.c'])
        self.assertEqual(finding['evidence_addresses'], ['0x80002000', '0x80002008'])
        self.assertEqual(finding['category'], 'canonical-only')
    def test_known_void_result_survives_uncertain_parameter_layout(self) -> None:
        consumer = {'caller': 'caller', 'call_address': '0x80002000', 'proven_consumed': True}
        contracts = [
            {'signature': {'returns': 'void', 'params': None, 'variadic': False}},
            {'signature': {'return_type': 'void', 'params': ['int'], 'prototyped': False}},
            {'declaration': 'void target()'},
        ]
        for contract in contracts:
            with self.subTest(contract=contract):
                row = {'symbol': 'target', 'source': 'src/unit.c', **contract}
                report = check(row, fact(result_consumers=[consumer]))
                self.assertEqual(kinds(report), {'void-consumed'})
                self.assertEqual(report['summary']['errors'], 1)
                self.assertEqual(report['summary']['uncertainties'], 1)


    def test_unknown_original_caller_does_not_claim_canonical_only(self) -> None:
        consumer = {'caller': 'asm_caller', 'call_address': '0x80002000', 'consumed': True}
        report = check(original=fact(live_in=1, result_consumers=[consumer]))
        self.assertEqual(report['findings'][0]['category'], 'private')

    def test_private_sources_taint_coalesced_identical_contract(self) -> None:
        inventory = {'declarations': [record(), record(source='scratch/candidate.c')]}
        report = abi_check.check_inventory(inventory, {'functions': [fact()]})
        self.assertEqual(len(report['findings']), 1)
        self.assertEqual(report['findings'][0]['category'], 'private')

    def test_ambiguous_images_are_uncertain_not_arbitrary(self) -> None:
        report = abi_check.check_inventory({'declarations': [record()]}, {'functions': [fact(), fact(image_id='resident')]})
        self.assertFalse(report['findings'])
        self.assertEqual(report['uncertainties'][0]['symbol'], 'target')

    def test_indirect_only_arguments_and_results_are_advisories(self) -> None:
        consumer = {'caller': 'caller', 'call_address': '0x80002000', 'consumed': False, 'proven_consumed': False, 'conservative_consumed': True}
        report = check(original=fact(live_in=4, proven_live_in=1, conservative_live_in=4, result_consumers=[consumer]))
        self.assertEqual(kinds(report), {'possible-lost-argument (indirect)', 'possible-void-consumed (indirect)'})
        self.assertEqual(report['summary']['errors'], 0)

    def test_report_determinism_under_inventory_order(self) -> None:
        rows = [record(), record(source='scratch/private.c'), record('void target(void)')]
        first = abi_check.check_inventory({'declarations': rows}, {'functions': [fact()]})
        second = abi_check.check_inventory({'declarations': rows[::-1]}, {'functions': [fact()]})
        self.assertEqual(first, second)

    def test_cli_exit_status_and_frozen_outputs(self) -> None:
        with tempfile.TemporaryDirectory() as directory, contextlib.redirect_stderr(io.StringIO()):
            root = Path(directory)
            inventory = root / 'inventory.json'
            originals = root / 'facts.json'
            all_output = root / 'all.json'
            canonical = root / 'canonical.json'
            inventory.write_text(json.dumps({'declarations': [record()]}))
            originals.write_text(json.dumps({'functions': [fact(live_in=0)]}))
            args = ['--inventory', str(inventory), '--facts', str(originals), '--json', str(all_output), '--canonical-json', str(canonical)]
            self.assertEqual(abi_check.main(args), 0)
            self.assertEqual(json.loads(all_output.read_text())['summary']['errors'], 0)
            self.assertEqual(json.loads(canonical.read_text())['findings'][0]['kind'], 'extra-argument')
            originals.write_text(json.dumps({'functions': [fact()]}))
            self.assertEqual(abi_check.main(args), 1)
            self.assertEqual(json.loads(all_output.read_text())['findings'][0]['kind'], 'lost-argument')
            originals.write_text('not JSON')
            self.assertEqual(abi_check.main(args), 2)

    def test_struct_pointer_return_does_not_add_hidden_slot(self) -> None:
        report = check(record('struct Opaque *target(int a)', return_type='struct Opaque *'), fact(live_in=1))
        self.assertFalse(report['findings'])

    def test_incomplete_aggregate_return_alias_still_has_hidden_pointer(self) -> None:
        report = check(record('Result target(int a)', return_type='Result'), fact(live_in=2), types={'Result': 'struct Opaque'})
        self.assertFalse(report['findings'])
        self.assertFalse(report['uncertainties'])

    def test_existing_interface_signature_return_field(self) -> None:
        row = {'symbol': 'target', 'source': 'src/unit.c', 'signature': {'returns': 'int', 'params': ['int', 'int'], 'variadic': False}}
        self.assertFalse(check(row)['findings'])

    def test_o32_long_double_parameter_alignment(self) -> None:
        report = check(record('void target(int a, long double b)'), fact(live_in=4))
        self.assertFalse(report['findings'])
        self.assertFalse(report['uncertainties'])


if __name__ == "__main__":
    unittest.main()
