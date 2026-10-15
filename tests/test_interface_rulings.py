"""Exact ABI ruling pairs, live hashes, and atomic activation (no dormant entries)."""
from __future__ import annotations

import copy
import hashlib
import io
import json
import sys
import tempfile
import unittest
from contextlib import redirect_stdout
from pathlib import Path
from unittest.mock import patch

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "tools"))

from interfaces import (PROJECT, Gate, audit, clang_ast_command, collect, load_rulings, main,  # noqa: E402
                        parse_cxx_tu, parse_tu, translation_units)

DEFINITION = "src/units/main_14400/definition.c"
DECLARATION = "src/units/main_14400/declaring.c"
PRELUDE = "typedef struct { int x; } Value;\n"
DEFINED = "void *make(void *out, void *from, int direction) { return out; }\n"
DECLARED = "Value make(void *from, int direction);\nvoid caller(void) { make(0, 1); }\n"
CLANG, CLANG_UNAVAILABLE = clang_ast_command(PROJECT, PROJECT)


def unit(path: str, text: str, candidate: int | None = None):
    result = parse_tu(PRELUDE + text, path)
    result.candidate = candidate
    return result


class InterfaceRulingTests(unittest.TestCase):
    def setUp(self) -> None:
        self.temporary = tempfile.TemporaryDirectory(prefix="interface-rulings-test-")
        self.addCleanup(self.temporary.cleanup)
        self.root = Path(self.temporary.name)
        (self.root / "config").mkdir()
        (self.root / "config/interface_rulings").mkdir()
        self.proof = self.root / "config/interface_rulings/accepted.txt"
        self.proof.write_text("Reviewed exact hidden-result/explicit-out interface pair.\n")
        self.entry = {
            "symbol": "make", "definition_file": DEFINITION, "declaring_file": DECLARATION,
            "declared_shape": ["aggregate", ["pointer", "int"], False],
            "ruling": {"path": "config/interface_rulings/accepted.txt", "sha256": hashlib.sha256(self.proof.read_bytes()).hexdigest()},
        }
        self.write_entries([self.entry])
        self.profile = {"profiles": {"fixture": {"language": "c"}},
                        "tu_profiles": {name: "fixture" for name in (
                            DEFINITION, DECLARATION, "src/units/main_14400/second.c")}}
        (self.root / "config/compiler_profiles.json").write_text(json.dumps(self.profile))

    def write_entries(self, entries) -> None:
        (self.root / "config/interface_rulings.json").write_text(json.dumps(entries))

    def report(self, *results):
        with patch("interfaces.collect", return_value=list(results)):
            return audit(self.root)

    def pair(self, definition: str = DEFINED, declaration: str = DECLARED):
        return unit(DEFINITION, definition), unit(DECLARATION, declaration)

    def test_exact_pair_and_its_bound_explicit_arity_are_suppressed(self) -> None:
        report = self.report(*self.pair())
        self.assertEqual([], report.findings)
        self.assertEqual(["declaration-vs-definition", "call-arity"], [f.kind for f in report.suppressed])
        self.assertEqual(["aggregate", ["pointer", "int"], False], report.suppressed[0].site["signature"])
        self.assertEqual(DECLARATION, report.suppressed[1].site["declaration"]["file"])
        self.assertEqual([], report.registry()["symbols"]["make"]["disagreements"])

    def test_every_key_mismatch_keeps_reporting(self) -> None:
        changes = {
            "symbol": "other", "definition_file": "src/units/main_14400/other.c",
            "declaring_file": "src/units/main_14400/other.c",
            "declared_shape": ["aggregate", ["pointer", "int unsigned"], False],
        }
        for field, value in changes.items():
            with self.subTest(field=field):
                entry = copy.deepcopy(self.entry)
                entry[field] = value
                self.write_entries([entry])
                report = self.report(*self.pair())
                self.assertEqual(["declaration-vs-definition", "call-arity", "interface-ruling"],
                                 [f.kind for f in report.findings])
                self.assertEqual([], report.suppressed)

    def test_changed_definition_is_not_hidden_by_the_same_declaration(self) -> None:
        definitions = [
            "int make(void *out, void *from, int direction) { return 0; }",
            "void *make(void *out, int from, int direction) { return out; }",
            "void *make(void *out, void *from, unsigned int direction) { return out; }",
            "void *make(void *out, void *from, int direction, ...) { return out; }",
            "void *make(void *from, int direction) { return from; }",
        ]
        for definition in definitions:
            with self.subTest(definition=definition):
                report = self.report(*self.pair(definition=definition))
                self.assertIn("declaration-vs-definition", [f.kind for f in report.findings])
                self.assertIn("interface-ruling", [f.kind for f in report.findings])
                self.assertEqual([], report.suppressed)

    def test_stale_unknown_missing_and_now_compatible_pairs_are_errors(self) -> None:
        cases = [[], [self.pair()[0]], [self.pair()[1]],
                 list(self.pair(declaration="void *make(void *out, void *from, int direction);"))]
        for results in cases:
            with self.subTest(results=[result.tu for result in results]):
                report = self.report(*results)
                self.assertEqual(["interface-ruling"], [f.kind for f in report.findings])
                self.assertIn("stale", report.findings[0].detail)

    def test_future_definition_is_not_a_dormant_entry(self) -> None:
        # The config and all activation partner TUs must land atomically.
        report = self.report(self.pair()[1])
        self.assertEqual(["interface-ruling"], [f.kind for f in report.findings])
        self.assertEqual([], self.report(*self.pair()).findings)

    def test_tampered_ruling_hash_is_an_error_and_suppresses_nothing(self) -> None:
        self.proof.write_text("Changed after review.\n")
        report = self.report(*self.pair())
        self.assertEqual(["declaration-vs-definition", "call-arity", "interface-ruling"],
                         [f.kind for f in report.findings])
        self.assertIn("sha256 mismatch", report.findings[-1].detail)
        self.assertEqual([], report.suppressed)

    def test_json_cli_reports_suppression_and_returns_nonzero_for_stale_entries(self) -> None:
        for results, status, suppressed in ((self.pair(), 0, 2), ((self.pair()[0],), 1, 0)):
            with self.subTest(status=status):
                output = io.StringIO()
                with patch("interfaces.collect", return_value=list(results)), redirect_stdout(output):
                    actual = main(["--source-root", str(self.root), "--json"])
                payload = json.loads(output.getvalue())
                self.assertEqual(status, actual)
                self.assertEqual(suppressed, len(payload["suppressed"]))
                self.assertEqual({} if status == 0 else {"interface-ruling": 1}, payload["counts"])

    def test_both_named_files_must_be_registered_translation_units(self) -> None:
        for field in ("definition_file", "declaring_file"):
            with self.subTest(field=field):
                profile = copy.deepcopy(self.profile)
                profile["tu_profiles"].pop(self.entry[field])
                (self.root / "config/compiler_profiles.json").write_text(json.dumps(profile))
                report = self.report(*self.pair())
                self.assertEqual([], report.suppressed)
                self.assertEqual("interface-ruling", report.findings[-1].kind)
                self.assertIn("registered translation units", report.findings[-1].detail)

    def test_header_cannot_be_a_ruled_declaring_file_even_if_added_to_the_registry(self) -> None:
        self.entry["declaring_file"] = "include/shared.h"
        self.profile["tu_profiles"]["include/shared.h"] = "fixture"
        self.write_entries([self.entry])
        (self.root / "config/compiler_profiles.json").write_text(json.dumps(self.profile))
        header = unit(DECLARATION, '# 2 "source/include/shared.h"\n' + DECLARED)
        report = self.report(self.pair()[0], header)
        self.assertEqual([], report.suppressed)
        self.assertIn("registered translation units", report.findings[-1].detail)

    def test_declaring_tu_provenance_must_be_exact_not_an_included_source(self) -> None:
        inherited = unit("src/units/main_14400/other.c",
                         f'# 2 "source/{DECLARATION}"\nValue make(void *from, int direction);\n')
        report = self.report(*self.pair(), inherited)
        self.assertEqual([], report.suppressed)
        self.assertEqual(["declaration-vs-definition", "call-arity", "interface-ruling"],
                         [finding.kind for finding in report.findings])
        self.assertIn(inherited.tu, report.findings[0].tus)

    def test_definition_tu_provenance_must_also_be_exact(self) -> None:
        inherited = unit("src/units/main_14400/other.c", f'# 2 "source/{DEFINITION}"\n' + DEFINED)
        report = self.report(*self.pair(), inherited)
        self.assertEqual([], report.suppressed)
        self.assertEqual("interface-ruling", report.findings[-1].kind)

    def test_batch_partner_failure_is_reported_not_claimed_to_be_transactional(self) -> None:
        from adopt import Unit, _adopt_batch

        second = copy.deepcopy(self.entry)
        second.update(symbol="next", declaring_file="src/units/main_14400/second.c")
        self.write_entries([self.entry, second])
        base = unit(DEFINITION, DEFINED + DEFINED.replace("make", "next"))
        first = unit(DECLARATION, DECLARED, candidate=0)
        last = unit(second["declaring_file"], DECLARED.replace("make", "next"), candidate=1)
        marker = self.root / "first-partner-written.txt"

        def adopter(unit, *, gate, index, **kwargs):
            if index == 1:
                raise ValueError("second partner match mismatch")
            marker.write_text("The private copy has a partial adoption.\n")
            gate.admit(index)
            return {"functions": 1, "instruction_bytes": 4}

        with patch("adopt.PROJECT", self.root), patch("adopt.collect", return_value=[base, first, last]), \
                patch("adopt.interface_candidate", return_value={}), patch("adopt.adopt", side_effect=adopter):
            totals, failures = _adopt_batch([Unit(["make"], Path("first.c")), Unit(["next"], Path("second.c"))], b"", [])
        self.assertEqual(1, totals["functions"])
        self.assertTrue(marker.exists(), "Main must discard this private copy instead of promoting it")
        self.assertTrue(any("second partner match mismatch" in failure for failure in failures))
        self.assertTrue(any("interface ruling" in failure and "next" in failure for failure in failures))

    def test_prototype_mode_also_fails_for_an_invalid_ruling(self) -> None:
        self.proof.write_text("Changed after review.\n")
        output = io.StringIO()
        with patch("interfaces.collect", return_value=list(self.pair())), redirect_stdout(output):
            status = main(["--source-root", str(self.root), "--prototype", "make"])
        self.assertEqual(1, status)
        self.assertIn("sha256 mismatch", output.getvalue())

    def test_source_snapshot_includes_the_exact_ruling_document(self) -> None:
        from workspace import REQUIRED_FILES, create_snapshot

        for name in REQUIRED_FILES:
            (self.root / name).write_text("")
        detached = self.root / "detached"
        manifest = create_snapshot(self.root, detached)
        relative = self.entry["ruling"]["path"]
        self.assertEqual(self.proof.read_bytes(), (detached / relative).read_bytes())
        self.assertEqual(self.entry["ruling"]["sha256"], manifest["files"][relative]["sha256"])
        self.assertFalse((detached / relative).is_symlink())
        self.assertEqual(1, len(load_rulings(detached)))

    def test_missing_proof_and_bad_config_fail_closed(self) -> None:
        invalid = [
            {"entries": [self.entry]},
            [self.entry, self.entry],
            [{**self.entry, "extra": "not permitted"}],
            [{**self.entry, "declaring_file": "../escape.c"}],
            [{**self.entry, "declaring_file": "/absolute.c"}],
            [{**self.entry, "declared_shape": ["aggregate", ["pointer", "int"], True]}],
            [{**self.entry, "ruling": {**self.entry["ruling"], "path": "rulings/missing.txt"}}],
            [{**self.entry, "ruling": {**self.entry["ruling"], "sha256": "0" * 64}}],
        ]
        for entries in invalid:
            with self.subTest(entries=entries):
                self.write_entries(entries)
                report = self.report(*self.pair())
                self.assertIn("interface-ruling", [f.kind for f in report.findings])
                self.assertEqual([], report.suppressed)

    def test_detached_snapshot_requires_its_own_proof_without_a_tool_project_fallback(self) -> None:
        detached = self.root / "detached"
        (detached / "config/interface_rulings").mkdir(parents=True)
        (detached / "config/interface_rulings.json").write_text(json.dumps([self.entry]))
        (detached / "config/compiler_profiles.json").write_text(json.dumps(self.profile))
        with patch("interfaces.PROJECT", self.root):
            with self.assertRaises(FileNotFoundError):
                load_rulings(detached)
            local = detached / self.entry["ruling"]["path"]
            local.write_bytes(self.proof.read_bytes())
            self.assertEqual(1, len(load_rulings(detached)))
            local.write_text("Tampered local copy.\n")
            with self.assertRaisesRegex(ValueError, "sha256 mismatch"):
                load_rulings(detached)

    def test_unruled_site_and_wrong_explicit_arity_still_report(self) -> None:
        definition, declaration = self.pair(declaration=DECLARED + "void wrong(void) { make(0); }")
        foreign = unit("src/units/main_14400/unruled.c", DECLARED.replace("caller", "foreign"))
        report = self.report(definition, declaration, foreign)
        self.assertEqual(["declaration-vs-definition", "call-arity", "call-arity"], [f.kind for f in report.findings])
        self.assertEqual({DECLARATION, foreign.tu}, {f.site["file"] for f in report.findings if f.kind == "call-arity"})

    def test_ruled_call_cannot_pass_the_definition_arity_instead_of_its_explicit_arity(self) -> None:
        declaration = DECLARED + "void wrong(void) { make(0, 1, 2); }"
        report = self.report(*self.pair(declaration=declaration))
        self.assertEqual(["call-arity"], [f.kind for f in report.findings])
        self.assertEqual("2", report.findings[0].differences[0]["expected"])
        self.assertEqual(DECLARATION, report.findings[0].reference["file"])

    def test_lexically_different_binding_is_not_admitted_by_the_ruled_site(self) -> None:
        declaration = DECLARED + "void wrong(void) { extern int make(void *, int); make(0, 1); }"
        report = self.report(*self.pair(declaration=declaration))
        self.assertEqual(["declaration-vs-definition", "call-arity"], [f.kind for f in report.findings])
        self.assertEqual("int", report.findings[-1].site["declaration"]["shape"]["returns"])
        self.assertEqual(2, len(report.suppressed))

    def test_shared_call_location_preserves_distinct_bindings(self) -> None:
        header_call = '# 10 "source/include/call.h"\nvoid caller(void) { make(0, 1); }\n'
        approved = unit(DECLARATION, "Value make(void *, int);\n" + header_call)
        other = unit("other.c", "int make(void *, int);\n" + header_call)
        report = self.report(self.pair()[0], approved, other)
        self.assertEqual(2, len(report.symbols[("external", "make")].calls))
        self.assertEqual(1, sum(f.kind == "call-arity" for f in report.suppressed))
        self.assertEqual(1, sum(f.kind == "call-arity" for f in report.findings))

    def test_void_result_pair_and_unrelated_findings(self) -> None:
        self.entry.update(symbol="copy", declared_shape=["void", ["pointer", "pointer", "int unsigned"], False])
        self.write_entries([self.entry])
        definition = unit(DEFINITION, "void *copy(void *dst, void *src, unsigned int size) { return dst; }")
        declaration = unit(DECLARATION, "void copy(void *dst, void *src, unsigned int size);\n"
                           "int unrelated(void); void caller(void) { copy(0, 0, 1); }")
        other = unit("other.c", "void unrelated(void) {}")
        report = self.report(definition, declaration, other, unit("bad.c", "void broken("))
        self.assertEqual(["parse-failure", "declaration-vs-definition"], [f.kind for f in report.findings])
        self.assertEqual(["copy"], [f.symbol for f in report.suppressed])

    def test_static_functions_and_further_definitions_cannot_use_the_exception(self) -> None:
        static = self.report(*self.pair(definition="static " + DEFINED, declaration="static " + DECLARED))
        self.assertIn("interface-ruling", [f.kind for f in static.findings])
        self.assertEqual([], static.suppressed)
        extra = unit(DECLARATION, "Value make(void *from, int direction) { Value v; return v; }")
        report = self.report(self.pair()[0], extra)
        self.assertEqual(["declaration-vs-definition", "interface-ruling"], [f.kind for f in report.findings])

    def test_atomic_batch_allows_intermediate_prefixes_but_rejects_missing_partners(self) -> None:
        second = copy.deepcopy(self.entry)
        second.update(symbol="next", declaring_file="src/units/main_14400/second.c")
        self.write_entries([self.entry, second])
        base = unit(DEFINITION, DEFINED + DEFINED.replace("make", "next"))
        first = unit(DECLARATION, DECLARED, candidate=0)
        last = unit(second["declaring_file"], DECLARED.replace("make", "next").replace("caller", "last"), candidate=1)
        gate = Gate([base, first, last], source_root=self.root)
        self.assertEqual([], gate.check(0))
        gate.admit(0)
        self.assertEqual(1, len(gate.ruling_errors()), "an incomplete activation remains an error at final validation")
        self.assertEqual([], gate.check(1))
        gate.admit(1)
        self.assertEqual([], gate.ruling_errors())
        missing = Gate([base, first], source_root=self.root)
        self.assertEqual(["interface-ruling"], [f.kind for f in missing.check(0)])

    def test_gate_hash_errors_are_never_tolerated_as_baseline_findings(self) -> None:
        definition, declaration = self.pair()
        declaration.candidate = 0
        self.proof.write_text("Tampered before the gate.\n")
        gate = Gate([definition, declaration], source_root=self.root)
        self.assertIn("interface-ruling", [f.kind for f in gate.check(0)])
        self.proof.write_text("Reviewed exact hidden-result/explicit-out interface pair.\n")
        gate = Gate([definition, declaration], source_root=self.root)
        self.assertEqual([], gate.check(0))
        self.proof.write_text("Tampered after the gate was initialized.\n")
        self.assertIn("interface-ruling", [f.kind for f in gate.check(0)])

    @unittest.skipUnless(CLANG, f"Pinned clang_ast required ({CLANG_UNAVAILABLE})")
    def test_cpp_binding_uses_declaration_identity_and_refuses_function_casts(self) -> None:
        text = (PRELUDE + 'extern "C" Value make(void *, int);\n'
                'extern "C" void caller(void) { make(0, 1);\n'
                'reinterpret_cast<int (*)(void *, int)>(make)(0, 1); }\n')
        cpp = parse_cxx_tu(text, DECLARATION, "gxx281pm-gnu291-O2-unsigned", CLANG)
        self.assertIsNone(cpp.error, cpp.error)
        self.assertEqual([True, False], [call.declaration is not None for call in cpp.calls])
        report = self.report(self.pair()[0], cpp)
        self.assertEqual(["call-arity"], [f.kind for f in report.findings])
        self.assertEqual(2, len(report.suppressed))

    @unittest.skipUnless((PROJECT / ".cache/gcc-pm281/gcc").is_file(), "Pinned GCC required")
    def test_candidate_destination_is_explicit_and_relocates_the_bound_declaration(self) -> None:
        profile = json.loads((PROJECT / "config/compiler_profiles.json").read_text())
        profile["tu_profiles"] = {DEFINITION: "gcc281pm-gnu291-O2-unsigned"}
        (self.root / "config/compiler_profiles.json").write_text(json.dumps(profile))
        (self.root / "toolchain.lock.json").write_bytes((PROJECT / "toolchain.lock.json").read_bytes())
        definition = self.root / DEFINITION
        definition.parent.mkdir(parents=True)
        definition.write_text(PRELUDE + DEFINED)
        source = self.root / "not_the_destination.c"
        source.write_text(PRELUDE + DECLARED)
        extra = {"source": str(source), "profile": "gcc281pm-gnu291-O2-unsigned", "destination": DECLARATION}
        results = collect(self.root, [extra], jobs=1)
        self.assertTrue(all(result.error is None for result in results), [result.error for result in results])
        self.assertEqual(DECLARATION, results[-1].tu)
        self.assertEqual(DECLARATION, results[-1].calls[0].declaration.file)
        gate = Gate(results, source_root=self.root)
        self.assertEqual([], gate.check(0))
        gate.admit(0)
        self.assertEqual(["interface-ruling"], [f.kind for f in gate.ruling_errors()])
        profile["tu_profiles"][DECLARATION] = extra["profile"]
        (self.root / "config/compiler_profiles.json").write_text(json.dumps(profile))
        self.assertEqual([], gate.ruling_errors())
        profile["tu_profiles"].pop(DECLARATION)
        (self.root / "config/compiler_profiles.json").write_text(json.dumps(profile))
        unrelocated = collect(self.root, [{key: value for key, value in extra.items() if key != "destination"}], jobs=1)
        self.assertIn("interface-ruling", [f.kind for f in Gate(unrelocated, source_root=self.root).check(0)])
        bad = translation_units(profile, self.root, [{**extra, "destination": "../escape.c"}])[-1]
        self.assertIn("invalid candidate destination", bad.error)


if __name__ == "__main__":
    unittest.main()
