"""Cross-TU function and external object interface audit (tools/interfaces.py)."""
from __future__ import annotations

import io
import json
import sys
import tempfile
import unittest
from contextlib import redirect_stdout
from pathlib import Path
from unittest.mock import patch

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "tools"))

from interfaces import PROJECT, CXX_INTEGERS, Gate, analyse, clang_ast_command, collect, display, main, normalise, parse_cxx_tu, parse_tu  # noqa: E402
from interfaces import _CxxCollector, _internal_mangling  # noqa: E402

CLANG, CLANG_UNAVAILABLE = clang_ast_command(PROJECT, PROJECT)

PRELUDE = ("typedef signed int s32; typedef unsigned int u32; typedef signed short s16; typedef unsigned short u16;\n"
           "typedef unsigned char u8; typedef struct ViewA ViewA; typedef struct ViewB ViewB;\n")


def unit(name: str, text: str, candidate: int | None = None):
    result = parse_tu(PRELUDE + text, name)
    result.candidate = candidate
    return result


def findings(*units: tuple[str, str]):
    return analyse([unit(name, text) for name, text in units]).findings


def aspects(finding) -> list[str]:
    return [d["aspect"] for d in finding.differences]


class ComparatorTests(unittest.TestCase):
    def only(self, items, kind: str):
        self.assertEqual([kind], [f.kind for f in items], [f.summary() for f in items])
        return items[0]

    def test_lost_argument(self) -> None:
        items = findings(("a.c", "void f(s32 a, s32 b) {}\n"), ("b.c", "void f(s32 a);\nvoid g(void) { f(1); }\n"))
        self.assertEqual(["declaration-vs-definition", "call-arity"], [f.kind for f in items], [f.summary() for f in items])
        finding = items[0]
        self.assertEqual(["parameter count"], aspects(finding))
        self.assertEqual((2, 1), (finding.differences[0]["expected"], finding.differences[0]["found"]))
        self.assertEqual("b.c", finding.site["file"])
        self.assertEqual("a.c", finding.reference["file"])

    def test_variadic_mismatch(self) -> None:
        finding = self.only(findings(("a.c", "void say(s32 id, ...) {}\n"), ("b.c", "void say(s32 id);\n")),
                            "declaration-vs-definition")
        self.assertEqual(["variadic"], aspects(finding))

    def test_void_vs_pointer_return(self) -> None:
        finding = self.only(findings(("a.c", "ViewA *make(void) { return 0; }\n"), ("b.c", "void make(void);\n")),
                            "declaration-vs-definition")
        self.assertEqual([{"aspect": "return", "subkind": "category", "expected": "pointer", "found": "void"}], finding.differences)

    def test_pointer_vs_integer_parameter(self) -> None:
        finding = self.only(findings(("a.c", "void put(ViewA *a, s32 b) {}\n"), ("b.c", "extern void put(s32 a, s32 b);\n")),
                            "declaration-vs-definition")
        self.assertEqual([{"aspect": "parameter 1", "subkind": "category", "expected": "pointer", "found": "int"}],
                         finding.differences)

    def test_width_mismatch(self) -> None:
        finding = self.only(findings(("a.c", "s16 w(u8 x) { return x; }\n"), ("b.c", "s32 w(u32 x);\n")),
                            "declaration-vs-definition")
        self.assertEqual(["return width", "parameter 1 width"], aspects(finding))

    def test_signedness_mismatch(self) -> None:
        finding = self.only(findings(("a.c", "u32 sg(s16 a, u8 b) { return a + b; }\n"),
                                     ("b.c", "s32 sg(u16 a, signed char b);\n")), "declaration-vs-definition")
        self.assertEqual([("return signedness", "int unsigned", "int"), ("parameter 1 signedness", "short", "short unsigned"),
                          ("parameter 2 signedness", "char unsigned", "char signed")],
                         [(d["aspect"], d["expected"], d["found"]) for d in finding.differences])

    def test_int_vs_long_mismatch(self) -> None:
        # The u32 typedef drift: `unsigned long` in one header, `unsigned int` in common.h.
        finding = self.only(findings(("a.c", "typedef unsigned long ULong32;\nULong32 il(long a, int b) { return a + b; }\n"),
                                     ("b.c", "u32 il(s32 a, unsigned long b);\n")), "declaration-vs-definition")
        self.assertEqual(["return int-vs-long", "parameter 1 int-vs-long", "parameter 2 int-vs-long", "parameter 2 signedness"],
                         aspects(finding))
        self.assertEqual(("long unsigned", "int unsigned"), (finding.differences[0]["expected"], finding.differences[0]["found"]))

    def test_plain_char_signedness_mismatch(self) -> None:
        finding = self.only(findings(("a.c", "char cs(char c, unsigned char u) { return c + u; }\n"),
                                     ("b.c", "unsigned char cs(signed char c, char u);\n")), "declaration-vs-definition")
        self.assertEqual(["char-signedness"] * 3, [d["subkind"] for d in finding.differences])
        self.assertEqual([("char", "char unsigned"), ("char", "char signed"), ("char unsigned", "char")],
                         [(d["expected"], d["found"]) for d in finding.differences])

    def test_integer_sub_kinds_in_disagreeing_declarations(self) -> None:
        items = findings(("a.c", "void dd(u32 a);\nvoid ch(char *p, char c);\n"), ("b.c", "void dd(unsigned long a);\n"),
                         ("c.c", "void dd(s32 a);\nvoid ch(u8 *p, u8 c);\n"))
        self.assertEqual({"declaration-disagreement"}, {f.kind for f in items}, [f.summary() for f in items])
        dd = [f for f in items if f.symbol == "dd"]
        self.assertEqual(3, len(dd))  # every pair of the three integer types
        self.assertEqual({"int-vs-long", "signedness"}, {d["subkind"] for f in dd for d in f.differences})
        (ch,) = [f for f in items if f.symbol == "ch"]
        self.assertEqual([("parameter 2 char-signedness", "char-signedness")], [(d["aspect"], d["subkind"]) for d in ch.differences])
        self.assertEqual({"char", "char unsigned"}, {ch.differences[0]["expected"], ch.differences[0]["found"]})

    def test_unprototyped_declaration(self) -> None:
        items = findings(("a.c", "void u(void) {}\nvoid v() {}\nint k(a) int a; { return a; }\n"),
                         ("b.c", "void u();\nvoid g(void) { u(); q(1); }\n"))
        self.assertTrue(all(f.kind == "unprototyped" for f in items), [f.summary() for f in items])
        self.assertEqual({"u", "v", "k", "q"}, {f.symbol for f in items})
        self.assertIn("implicit declaration", next(f.detail for f in items if f.symbol == "q"))
        self.assertIn("definition without prototype", next(f.detail for f in items if f.symbol == "k"))

    def test_disagreeing_assembly_callee_declarations(self) -> None:
        finding = self.only(findings(("a.c", "void asm_f(ViewA *p);\n"), ("b.c", "void asm_f(ViewB *p);\n"),
                                     ("c.c", "s32 asm_f(ViewA *p);\n")), "declaration-disagreement")
        self.assertEqual([{"aspect": "return", "subkind": "category", "expected": "void", "found": "int"}], finding.differences)
        self.assertEqual("c.c", finding.site["file"])
        self.assertEqual({"a.c", "b.c", "c.c"}, finding.tus)

    def test_call_arity(self) -> None:
        items = findings(("a.c", "void two(s32 a, s32 b) {}\nvoid many(s32 id, ...) {}\n"),
                         ("b.c", "void two();\nvoid many(s32 id, ...);\nvoid g(void) { two(1); many(1, 2, 3); }\n"),
                         ("c.c", "void many();\nvoid h(void) { many(); }\n"))
        arity = sorted((f.symbol, f.site["file"], f.differences[0]["found"]) for f in items if f.kind == "call-arity")
        self.assertEqual([("many", "c.c", 0), ("two", "b.c", 1)], arity)
        self.assertEqual({"call-arity", "unprototyped"}, {f.kind for f in items})

    def test_different_struct_pointer_views_decay_and_compatible_enums_are_accepted(self) -> None:
        self.assertEqual([], findings(
            ("a.c", "typedef void (*Callback)(s32);\nvoid s(ViewA *a, u32 n, s16 *list, Callback cb) {}\n"
                    "typedef void Fn(ViewA *, u32);\nextern Fn fn;\nvoid call(void) { fn(0, 1); s(0, 1, 0, 0); }\n"),
            ("b.c", "enum Mode { MODE_A };\nvoid s(ViewB *b, enum Mode n, s16 list[4], void cb(u32));\n"
                    "void fn(ViewB *b, unsigned int n);\n")))

    def test_enum_underlying_mismatch_in_both_directions(self) -> None:
        enums = "enum Flag { FLAG_A, FLAG_B = 1 << 3 };\nenum Delta { DOWN = -1, UP };\n"
        items = findings(
            ("a.c", enums + "enum Flag ev(enum Delta d) { return FLAG_A; }\nu32 rv(s32 x) { return x; }\n"),
            ("b.c", "s32 ev(u32 d);\n"),                                    # enum definition, integer declaration
            ("c.c", enums + "u32 ev(s32 d);\nenum Delta rv(enum Flag x);\n"))  # compatible; integer definition, enum declaration
        self.assertEqual({"declaration-vs-definition"}, {f.kind for f in items}, [f.summary() for f in items])
        self.assertEqual({
            ("ev", "b.c"): [("return enum-underlying", "enum Flag (int unsigned)", "int"),
                            ("parameter 1 enum-underlying", "enum Delta (int)", "int unsigned")],
            ("rv", "c.c"): [("return enum-underlying", "int unsigned", "enum Delta (int)"),
                            ("parameter 1 enum-underlying", "int", "enum Flag (int unsigned)")]},
            {(f.symbol, f.site["file"]): [(d["aspect"], d["expected"], d["found"]) for d in f.differences] for f in items})

    def test_same_enum_agrees_and_enumerator_values_follow_gcc(self) -> None:
        enums = ("typedef enum { MODE_LOW = -5, MODE_NEXT } Mode;\nenum Mask { BIT = 1 << 3, ALL = ~0 };\n"
                 "enum Big { HIGH = 0xFFFFFFFF };\nenum Top { TOP = 1 << 31 };\nenum Chain { C0 = 'A', C1 = C0 * 2 - 200 };\n")
        items = findings(("a.c", enums + "void same(Mode m);\nvoid asm_e(enum Big b);\nvoid asm_m(enum Mask m, enum Top t, enum Chain c);\n"),
                         ("b.c", enums + "void same(Mode m);\nvoid asm_e(s32 b);\nvoid asm_m(s32 m, s32 t, s32 c);\n"))
        finding = self.only(items, "declaration-disagreement")
        self.assertEqual("asm_e", finding.symbol)
        self.assertEqual(["parameter 1 enum-underlying"], aspects(finding))
        self.assertEqual({"enum Big (int unsigned)", "int"}, {finding.differences[0]["expected"], finding.differences[0]["found"]})

    def test_typedef_function_declaration_is_compared(self) -> None:
        finding = self.only(findings(("a.c", "void tf(s32 a, s32 b) {}\n"), ("b.c", "typedef void Fn(s32);\nextern Fn tf;\n")),
                            "declaration-vs-definition")
        self.assertEqual(["parameter count"], aspects(finding))

    def test_static_functions_are_tu_local(self) -> None:
        items = findings(("a.c", "static s32 local(s32 x);\nstatic s32 local(s32 x) { return x; }\n"
                                 "s32 user(void) { return local(1); }\n"),
                         ("b.c", "static void local(ViewA *p, s32 y);\nstatic void local(ViewA *p, s32 y) {}\n"),
                         ("c.c", "static void local(s32 x);\nstatic s32 local(s32 x) { return x; }\n"))
        finding = self.only(items, "declaration-vs-definition")
        self.assertEqual("c.c", finding.site["file"])
        self.assertEqual({"c.c"}, finding.tus)

    def test_block_scope_declarations_and_shadowing_variables(self) -> None:
        items = findings(("a.c", "s32 blk(s32 a) { return a; }\n"),
                         ("b.c", "void user(void (*blk)(void)) { blk(); }\n"
                                 "void other(void) { extern void blk(s32); blk(2); }\n"))
        finding = self.only(items, "declaration-vs-definition")
        self.assertEqual(["return"], aspects(finding))

    def test_parse_failure_is_a_finding(self) -> None:
        finding = self.only(findings(("bad.c", "void f(void) { this is not C; }\n")), "parse-failure")
        self.assertEqual("bad.c", finding.symbol)

    def test_gnu_spellings_are_normalised_with_lines_kept(self) -> None:
        text = "typedef struct { int a; } T __attribute__((aligned(16),\n section(\"x\")));\nint n = __alignof__(T);\n__inline__ int f(void);\n"
        normalised = normalise(text)
        self.assertNotIn("__attribute__", normalised)
        self.assertEqual(text.count("\n"), normalised.count("\n"))
        self.assertIn("int n = sizeof(T);", normalised)
        self.assertIn("inline int f(void);", normalised)
        result = parse_tu(text + "void g(void) { __builtin_memcpy(0, 0, 4); }\n", "gnu.c")
        self.assertIsNone(result.error)
        self.assertEqual([], analyse([result]).findings)

    def test_linemarkers_attribute_sites_to_headers(self) -> None:
        header = '# 1 "source/src/a.c"\n# 1 "source/include/api.h" 1\nvoid api(ViewA *p);\n# 2 "source/src/a.c" 2\nvoid api(ViewA *p) {}\n'
        result = parse_tu(PRELUDE + header, "src/a.c")
        self.assertEqual([("include/api.h", 1, "declaration"), ("src/a.c", 2, "definition")],
                         [(s.file, s.line, s.role) for s in result.sites])

    def test_candidates_are_refused_only_for_findings_they_introduce_in_order(self) -> None:
        canonical = [unit("src/a.c", "void callee(ViewA *p);\nvoid split(s32 x);\n"),
                     unit("src/b.c", "void callee(ViewB *p);\ns32 split(s32 x);\n")]
        candidates = [unit("cand0.c", "void callee(ViewA *p) {}\n", candidate=0),          # agrees with canonical
                      unit("cand1.c", "void split(s32 x);\n", candidate=1),               # joins an existing conflict
                      unit("cand2.c", "s32 lost(s32 a, s32 b) { return a; }\n", candidate=2),
                      unit("cand3.c", "s32 lost(s32 a);\nvoid g(void) { lost(1); }\n", candidate=3),
                      unit("cand4.c", "void broken(\n", candidate=4),
                      unit("cand5.c", "void callee(s32 p);\n", candidate=5),              # disagrees with cand0
                      unit("cand6.c", "s32 lost(s32 a, s32 b);\n", candidate=6)]
        gate = Gate(canonical + candidates)
        refused = {}
        for index in range(len(candidates)):
            if introduced := gate.check(index):
                refused[index] = introduced
            else:
                gate.admit(index)
        self.assertEqual([3, 4, 5], sorted(refused))
        self.assertEqual({"declaration-vs-definition", "call-arity"}, {f.kind for f in refused[3]})
        self.assertEqual({"parse-failure"}, {f.kind for f in refused[4]})
        self.assertEqual(["parameter 1"], aspects(refused[5][0]))

    def test_registry_authority(self) -> None:
        report = analyse([unit("a.c", "s32 defined(ViewA *p) { return 0; }\nvoid agreed(ViewA *p, s32 x, ...);\nvoid split(s32 x);\n"),
                          unit("b.c", "void agreed(ViewB *p, int x, ...);\nvoid agreed();\nvoid split(u32 x);\n")])
        symbols = report.registry()["symbols"]
        self.assertEqual(("definition", "int defined(pointer)"), (symbols["defined"]["authority"], symbols["defined"]["prototype"]))
        self.assertEqual("declarations", symbols["agreed"]["authority"])
        self.assertEqual("void agreed(pointer, int, ...)", symbols["agreed"]["prototype"])
        self.assertEqual("disagreement", symbols["split"]["authority"])
        self.assertEqual(["declaration-disagreement"], [d["kind"] for d in symbols["split"]["disagreements"]])


class ObjectCategoryTests(unittest.TestCase):
    def only(self, items, subkind: str):
        self.assertEqual(["object-category"], [f.kind for f in items], [f.summary() for f in items])
        self.assertEqual([subkind], [d["subkind"] for d in items[0].differences])
        return items[0]

    def test_multiple_category_conflicts_report_all_sites(self) -> None:
        items = findings(
            ("a.c", "ViewA *shared = 0;\n"), ("b.c", "extern s32 shared;\n"),
            ("c.c", "extern u32 shared;\n"), ("d.c", "extern u8 shared[];\n"), ("e.c", "extern ViewB shared;\n"))
        self.assertEqual(["object-category", "object-category"], [f.kind for f in items])
        self.assertEqual({"pointer-vs-integer", "array-vs-pointer"}, {d["subkind"] for f in items for d in f.differences})
        for finding in items:
            self.assertEqual("shared", finding.symbol)
            self.assertEqual(5, len(finding.locations))  # Includes the accepted aggregate view.
            self.assertEqual({"a.c", "b.c", "c.c", "d.c", "e.c"}, finding.tus)

    def test_pointer_vs_integer_definition_and_declarations(self) -> None:
        finding = self.only(findings(
            ("a.c", "ViewA *shared = 0;\n"), ("b.c", "extern s32 shared;\n"),
            ("c.c", "extern u32 shared;\n")), "pointer-vs-integer")
        self.assertEqual({"a.c", "b.c", "c.c"}, finding.tus)
        self.assertEqual([("a.c", "definition", "pointer"), ("b.c", "declaration", "integer"), ("c.c", "declaration", "integer")],
                         [(s["file"], s["role"], s["category"]) for s in finding.locations])
        for site in finding.locations:
            self.assertIn(f'{site["file"]}:{site["line"]}', finding.summary())
            self.assertIn(site["text"], finding.summary())

    def test_array_typedef_does_not_decay_to_pointer(self) -> None:
        finding = self.only(findings(
            ("a.c", "typedef u8 Row[4]; typedef Row Table[2]; extern Table shared;\n"),
            ("b.c", "typedef s32 *Pointer; typedef Pointer Alias; extern Alias shared;\n")), "array-vs-pointer")
        self.assertEqual({"array", "pointer"}, {s["category"] for s in finding.locations})

    def test_float_and_double_vs_integer(self) -> None:
        for floating in ("float", "double", "long double"):
            with self.subTest(floating=floating):
                self.only(findings(("a.c", f"typedef {floating} Real; Real shared = 0;\n"),
                                   ("b.c", "extern s32 shared;\n")), "float-vs-integer")

    def test_accepted_object_views(self) -> None:
        pairs = [
            ("extern void *shared;", "extern ViewA *shared;"),
            ("extern ViewA *shared;", "extern ViewB *shared;"),
            ("extern u8 shared[];", "extern s32 shared[8];"),
            ("extern u8 shared[];", "extern ViewA *shared[2][4];"),
            ("extern s32 shared;", "extern u32 shared;"),
            ("extern u8 shared;", "extern unsigned long shared;"),
            ("enum Mode { MODE }; extern enum Mode shared;", "extern s32 shared;"),
            ("extern u8 shared[];", "extern struct ViewA shared;"),
            ("extern u8 shared[];", "extern s32 shared;"),
            ("extern u8 shared[];", "extern double shared;"),
            ("extern struct ViewA shared;", "extern union ViewB shared;"),
            ("extern struct ViewA shared;", "extern s32 shared;"),
            ("extern float shared;", "extern double shared;"),
        ]
        for a, b in pairs:
            with self.subTest(a=a, b=b):
                self.assertEqual([], findings(("a.c", a), ("b.c", b)))

    def test_collects_external_objects_only_and_keeps_roles(self) -> None:
        result = unit("a.c", "s32 tentative;\nextern s32 declaration;\ns32 initialized = 0;\nextern s32 initialized_extern = 1;\n"
                      "typedef void Fn(void); extern Fn function;\ntypedef void (*Callback)(void); Callback callback;\n"
                      "static s32 hidden;\nvoid user(s32 parameter, Callback cb) {\n"
                      "s32 local; static s32 local_static; struct { void *member; } record;\nextern s32 block;\ncb();\n}\n")
        self.assertIsNone(result.error)
        self.assertEqual(
            [("tentative", "definition", "integer"), ("declaration", "declaration", "integer"),
             ("initialized", "definition", "integer"), ("initialized_extern", "definition", "integer"),
             ("callback", "definition", "pointer"), ("block", "declaration", "integer")],
            [(s.symbol, s.role, s.category) for s in result.objects])
        self.assertEqual({"function", "user"}, {s.symbol for s in result.sites})
        self.assertEqual([], result.calls)

    def test_block_scope_extern_resolves_shadowed_typedef(self) -> None:
        finding = self.only(findings(
            ("a.c", "typedef s32 Handle; extern Handle shared;\n"),
            ("b.c", "typedef s32 Handle; void user(void) { typedef ViewA *Handle; extern Handle shared; }\n")),
            "pointer-vs-integer")
        self.assertEqual({"a.c", "b.c"}, finding.tus)

    def test_static_objects_and_externs_inheriting_internal_linkage_are_not_compared(self) -> None:
        local = unit("a.c", "static s32 shared;\nextern s32 shared;\nvoid user(void) { extern s32 shared; }\n")
        other = unit("b.c", "static float shared;\n")
        external = unit("c.c", "extern ViewA *shared;\n")
        self.assertEqual([], local.objects)
        self.assertEqual([], other.objects)
        self.assertEqual([], analyse([local, other, external]).findings)
        self.assertEqual({"shared"}, set(analyse([local, other, external]).objects))

    def test_local_shadow_does_not_give_extern_internal_linkage(self) -> None:
        finding = self.only(findings(
            ("a.c", "static s32 shared; void user(void) { s32 shared; { extern ViewA *shared; } }\n"),
            ("b.c", "extern s32 shared;\n")), "pointer-vs-integer")
        self.assertEqual(2, len(finding.locations))

    def test_header_sites_are_deduplicated_with_tu_provenance(self) -> None:
        header = '# 7 "source/include/objects.h"\nextern s32 shared;\n'
        report = analyse([unit("a.c", header), unit("b.c", header), unit("c.c", "void *shared;\n")])
        finding = self.only(report.findings, "pointer-vs-integer")
        self.assertEqual(2, len(finding.locations))
        site = next(s for s in finding.locations if s["file"] == "include/objects.h")
        self.assertEqual((7, ["a.c", "b.c"]), (site["line"], site["tus"]))
        self.assertEqual({"a.c", "b.c", "c.c"}, finding.tus)

    def test_same_header_location_with_different_typedef_categories_is_not_deduplicated(self) -> None:
        header = '# 7 "source/include/objects.h"\nextern Handle shared;\n'
        finding = self.only(findings(("a.c", "typedef s32 Handle;\n" + header),
                                     ("b.c", "typedef void *Handle;\n" + header)), "pointer-vs-integer")
        self.assertEqual(2, len(finding.locations))
        self.assertEqual({"integer", "pointer"}, {s["category"] for s in finding.locations})

    def test_gate_refuses_new_conflicts_and_accepts_views_in_order(self) -> None:
        gate = Gate([
            unit("base.c", "extern s32 number; extern void *pointer;\n"),
            unit("candidate0.c", "extern u32 number; extern ViewA *pointer; extern u8 table[];\n", candidate=0),
            unit("candidate1.c", "extern void *number;\n", candidate=1),
            unit("candidate2.c", "extern s32 *table;\n", candidate=2),
            unit("candidate3.c", "extern ViewA table;\n", candidate=3),
            unit("candidate4.c", "extern float number;\n", candidate=4),
        ])
        self.assertEqual([], gate.check(0))
        gate.admit(0)
        self.only(gate.check(1), "pointer-vs-integer")
        self.only(gate.check(2), "array-vs-pointer")
        self.assertEqual([], gate.check(3))
        gate.admit(3)
        self.only(gate.check(4), "float-vs-integer")
        self.assertEqual(["base.c", "candidate0.c", "candidate3.c"], [r.tu for r in gate.admitted])

    def test_gate_keys_do_not_change_when_joining_an_existing_conflict(self) -> None:
        gate = Gate([
            unit("base.c", "extern s32 shared;\n"), unit("other.c", "extern void *shared;\n"),
            unit("join.c", "extern u32 shared;\n", candidate=0),
            unit("join_pointer.c", "extern ViewA *shared;\n", candidate=1),
            unit("new.c", "extern u8 shared[];\n", candidate=2),
        ])
        for index in (0, 1):
            self.assertEqual([], gate.check(index))
            gate.admit(index)
        self.only(gate.check(2), "array-vs-pointer")

    def test_registry_and_cli_json_include_objects_without_changing_function_shapes(self) -> None:
        report = analyse([unit("a.c", "extern s32 shared; static s32 hidden; void function(void);\n"),
                          unit("b.c", "extern void *shared;\n")])
        registry = report.registry()
        self.assertEqual({"shared"}, set(registry["objects"]))
        self.assertEqual(["integer", "pointer"], registry["objects"]["shared"]["categories"])
        self.assertEqual(2, len(registry["objects"]["shared"]["sites"]))
        self.assertEqual(["object-category"], [d["kind"] for d in registry["objects"]["shared"]["disagreements"]])
        self.assertEqual("void function(void)", registry["symbols"]["function"]["prototype"])
        output = io.StringIO()
        with tempfile.TemporaryDirectory(prefix="interfaces-json-test-") as directory:
            destination = Path(directory) / "registry.json"
            with patch("interfaces.audit", return_value=report), redirect_stdout(output):
                status = main(["--json", "--registry", str(destination)])
            self.assertEqual(registry, json.loads(destination.read_text()))
        self.assertEqual(1, status)
        payload = json.loads(output.getvalue())
        self.assertEqual({"object-category": 1}, payload["counts"])
        self.assertEqual(report.findings[0].to_json(), payload["findings"][0])


@unittest.skipUnless((PROJECT / ".cache/gcc-pm281/gcc").is_file() and (PROJECT / ".cache/gcc-kmc/cc1").is_file(),
                     "Pinned GCC 2.8.1 and 2.7.2 installs required")
class CanonicalGateTests(unittest.TestCase):
    maxDiff = None

    @classmethod
    def setUpClass(cls) -> None:
        cls.temporary = tempfile.TemporaryDirectory(prefix="interfaces-test-")
        root = Path(cls.temporary.name).resolve()
        (root / "inc").mkdir()
        (root / "inc/probe_api.h").write_text("void probe_api(s32 *p, ...);\n")
        (root / "good.c").write_text('#include "common.h"\n#include "probe_api.h"\nvoid probe_caller(void) { probe_api(0, 1); }\n')
        (root / "bad.c").write_text('#include "probe_missing.h"\n')
        (root / "probe.cpp").write_text('#include "common.h"\nextern "C" void probe_api(s32 *p);\n'
                                        'extern "C" void probe_cpp_caller(void) { probe_api(0); }\n')
        cls.header = root / "inc/probe_api.h"
        profile = "gcc281pm-gnu291-O2-unsigned"
        # One canonical collection serves the gate and the candidate-preprocessing checks.
        cls.results = collect(PROJECT, [{"source": str(root / "good.c"), "profile": profile, "includes": [str(root / "inc")]},
                                        {"source": str(root / "bad.c"), "profile": profile},
                                        {"source": str(root / "probe.cpp"), "profile": "gxx281pm-gnu291-O2-unsigned"},
                                        {"source": str(root / "probe.cpp"), "profile": profile}])

    @classmethod
    def tearDownClass(cls) -> None:
        cls.temporary.cleanup()

    def test_canonical_tree_has_no_interface_findings(self) -> None:
        report = analyse(r for r in self.results if r.candidate is None)
        self.assertGreater(len(report.results), 0)
        self.assertEqual([], [f.summary() for f in report.findings])

    def test_candidates_preprocess_with_their_include_paths(self) -> None:
        good, bad = (r for r in self.results if r.candidate in (0, 1))
        self.assertIsNone(good.error)
        self.assertEqual([(display(str(self.header)), 1, "declaration"), ("probe_caller", "definition")],
                         [(s.file, s.line, s.role) if s.symbol == "probe_api" else (s.symbol, s.role) for s in good.sites])
        self.assertTrue(bad.error.startswith("preprocess: exit"), bad.error)
        gate = Gate(self.results)
        self.assertEqual({0: [], 1: ["parse-failure"]}, {index: [f.kind for f in gate.check(index)] for index in (0, 1)})

    @unittest.skipUnless(CLANG, f"Pinned clang_ast required ({CLANG_UNAVAILABLE})")
    def test_cpp_candidates_join_the_canonical_gate(self) -> None:
        cxx = next(r for r in self.results if r.candidate == 2)
        self.assertIsNone(cxx.error)
        self.assertEqual([("probe_api", "declaration", 2), ("probe_cpp_caller", "definition", 3)],
                         [(s.symbol, s.role, s.line) for s in cxx.sites])
        self.assertEqual([("probe_api", 1)], [(c.symbol, c.arguments) for c in cxx.calls])
        gate = Gate(self.results)
        self.assertEqual([], gate.check(2))  # alone it agrees with the canonical tree
        gate.admit(0)  # the C candidate declares probe_api variadic through its header
        self.assertEqual([("declaration-disagreement", "probe_api", ["variadic"])],
                         [(f.kind, f.symbol, aspects(f)) for f in gate.check(2)])
        wrong = next(r for r in self.results if r.candidate == 3)
        self.assertEqual("input: profile gcc281pm-gnu291-O2-unsigned does not compile .cpp sources", wrong.error)
        self.assertEqual(["parse-failure"], [f.kind for f in gate.check(3)])


class CxxAstBoundaryTests(unittest.TestCase):
    """Deterministic fail-closed checks for missing AST evidence, without a parser dependency."""

    @staticmethod
    def indexed(*declarations: dict) -> _CxxCollector:
        collector = _CxxCollector("scope.cpp", "")
        collector.index({"kind": "TranslationUnitDecl", "id": "tu", "inner": list(declarations)})
        return collector

    def test_missing_typedef_identity_never_uses_the_spelled_global_type(self) -> None:
        collector = self.indexed({"kind": "TypedefDecl", "id": "t", "name": "T", "type": {"qualType": "bool"},
                                  "inner": [{"kind": "BuiltinType", "type": {"qualType": "bool"}}]})
        self.assertIs(CXX_INTEGERS["bool"], collector.classify_use({"qualType": "T", "typeAliasDeclId": "t"}, True))
        with self.assertRaisesRegex(ValueError, "unresolved C\\+\\+ typedef identity"):
            collector.classify_use({"qualType": "T", "typeAliasDeclId": "missing"}, True)
        # Without a typedef id the spelling is no typedef: only a tag declaration can bind it.
        with self.assertRaisesRegex(ValueError, "unresolved C\\+\\+ type identity"):
            collector.classify_use({"qualType": "T"}, True)

    def test_printed_tag_names_bind_only_when_every_candidate_agrees(self) -> None:
        def enum(identity: str, value: str) -> dict:
            return {"kind": "EnumDecl", "id": identity, "name": "E", "inner": [
                {"kind": "EnumConstantDecl", "id": f"{identity}.A", "name": "A", "type": {"qualType": "E"},
                 "inner": [{"kind": "ConstantExpr", "value": value, "type": {"qualType": "int"}}]}]}
        body = {"kind": "CompoundStmt", "id": "body", "inner": [{"kind": "DeclStmt", "id": "d", "inner": [enum("local", "-1")]}]}
        collector = self.indexed(enum("global", "1"), {"kind": "FunctionDecl", "id": "user", "name": "user", "inner": [body]})
        self.assertEqual(("E", "E"), (collector.tags["global"].printed, collector.tags["local"].printed))
        with self.assertRaisesRegex(ValueError, "ambiguous C\\+\\+ type identity"):
            collector.classify_use({"qualType": "enum E", "desugaredQualType": "E"}, True)
        agreeing = self.indexed(enum("global", "1"), {"kind": "CXXRecordDecl", "id": "other", "name": "F"})
        self.assertEqual("int unsigned", agreeing.classify_use({"qualType": "E"}, True).underlying)
        self.assertEqual("aggregate", agreeing.classify_use({"qualType": "struct E", "desugaredQualType": "E"}, True).category)

    def test_internal_mangled_object_names(self) -> None:
        for emitted, internal in (("_ZL1n", True), ("_ZZ4userE5saved", True), ("_ZN1NL1nE", True),
                                  ("_ZN12_GLOBAL__N_19anonymousE", True), ("_ZN1N6sharedE", False),
                                  ("_ZN1S1nE", False), ("_ZN1NL12shortE", False), ("shared", False)):
            with self.subTest(emitted=emitted):
                self.assertIs(internal, _internal_mangling(emitted))

    def test_static_member_dependencies_require_known_acyclic_initializers(self) -> None:
        collector = _CxxCollector("enum.cpp", "")
        member = {"kind": "MemberExpr", "referencedMemberDecl": "n"}
        self.assertFalse(collector.target_independent_constant(member))
        collector.constants["n"] = {"kind": "VarDecl", "id": "n"}
        self.assertFalse(collector.target_independent_constant(member))
        collector.constants["n"] = {"kind": "VarDecl", "id": "n", "init": "c", "inner": [member]}
        self.assertFalse(collector.target_independent_constant(member))
        collector.constants["first"] = {"kind": "VarDecl", "id": "first", "init": "c",
                                         "inner": [{"kind": "IntegerLiteral", "value": "3"}]}
        collector.constants["n"] = {"kind": "VarDecl", "id": "n", "previousDecl": "first"}
        self.assertTrue(collector.target_independent_constant(member))
        collector.constants["first"]["inner"] = [{"kind": "UnaryExprOrTypeTraitExpr", "name": "sizeof"}]
        self.assertFalse(collector.target_independent_constant(member))

    def test_pointer_valued_enum_dependencies_are_layout_dependent(self) -> None:
        collector = _CxxCollector("enum.cpp", "")
        literal = {"kind": "IntegerLiteral", "value": "1", "type": {"qualType": "int"}}
        self.assertTrue(collector.target_independent_constant(literal))
        for node in ({"kind": "ArraySubscriptExpr", "type": {"qualType": "bool"}, "inner": [literal, literal]},
                     {"kind": "CStyleCastExpr", "castKind": "PointerToIntegral", "type": {"qualType": "int"}, "inner": [literal]},
                     {"kind": "BinaryOperator", "opcode": "-", "type": {"qualType": "int"},
                      "inner": [{"kind": "ParenExpr", "type": {"qualType": "char *"}, "inner": [literal]}, literal]},
                     {"kind": "DeclRefExpr", "type": {"qualType": "bool[4]"}}):
            with self.subTest(node=node["kind"]):
                self.assertFalse(collector.target_independent_constant(node))

    def test_only_value_preserving_known_enum_conversion_wrappers_are_unwrapped(self) -> None:
        collector = _CxxCollector("enum.cpp", "")
        folded = {"kind": "ConstantExpr", "value": "255", "type": {"qualType": "int"}}
        wrapper = {"kind": "ImplicitCastExpr", "castKind": "IntegralCast",
                   "type": {"qualType": "unsigned int"}, "inner": [folded]}
        self.assertEqual(255, collector.enum_constant_value(wrapper))
        for changes in ({"kind": "CStyleCastExpr"}, {"castKind": "BitCast"},
                        {"type": {"qualType": "bool"}}, {"type": {"qualType": "char"}},
                        {"type": {"qualType": "signed char"}}, {"inner": []},
                        {"type": {"qualType": "long long"}}, {"type": {"qualType": "unsigned long long"}},
                        {"inner": [{"kind": "ConstantExpr", "value": "-1"}]}):
            with self.subTest(changes=changes), self.assertRaises(ValueError):
                collector.enum_constant_value({**wrapper, **changes})

    @staticmethod
    def typedef(identity: str, spelled: str) -> dict:
        return {"kind": "TypedefDecl", "id": identity, "name": "T", "type": {"qualType": spelled},
                "inner": [{"kind": "BuiltinType", "type": {"qualType": spelled}}]}

    @staticmethod
    def returned(collector: _CxxCollector, function: str) -> str:
        scope, complete = collector.lookups[function]
        return collector.written_kind("T", scope, collector.orders[function], complete).name

    def test_friend_only_tags_neither_bind_nor_let_lookup_pass_them(self) -> None:
        # GXX-IR4-01: the friend's semantic context makes it a namespace member, but no visible one.
        friend = {"kind": "FriendDecl", "id": "friend", "inner": [
            {"kind": "CXXRecordDecl", "id": "hidden", "name": "T", "tagUsed": "class", "parentDeclContextId": "n"}]}
        use = {"kind": "FunctionDecl", "id": "f", "name": "f"}
        namespace = {"kind": "NamespaceDecl", "id": "n", "name": "N", "inner": [
            {"kind": "CXXRecordDecl", "id": "s", "name": "S", "tagUsed": "struct", "inner": [friend]}, use]}
        collector = self.indexed(self.typedef("int", "int"), namespace)
        with self.assertRaisesRegex(ValueError, "only declared by a friend declaration"):
            self.returned(collector, "f")
        # An ordinary declaration in the namespace introduces the name.
        namespace["inner"].insert(1, {"kind": "CXXRecordDecl", "id": "visible", "name": "T", "tagUsed": "class",
                                      "previousDecl": "hidden"})
        self.assertEqual("N::T", self.returned(self.indexed(self.typedef("int", "int"), namespace), "f"))

    def test_statement_scopes_follow_clang_child_layouts_or_refuse(self) -> None:
        # GXX-PREEXISTING-SCOPE-01: a for-init is visible in the body; an unbraced body ends with the loop.
        short = {"kind": "DeclStmt", "id": "decl", "inner": [self.typedef("short", "short")]}
        inside = {"kind": "DeclStmt", "id": "decl-inside", "inner": [{"kind": "FunctionDecl", "id": "inside", "name": "f"}]}
        after = {"kind": "DeclStmt", "id": "decl-after", "inner": [{"kind": "FunctionDecl", "id": "after", "name": "g"}]}
        loop = {"kind": "ForStmt", "id": "for", "inner": [short, {}, {"kind": "CXXBoolLiteralExpr", "id": "c"}, {}, inside]}
        collector = self.indexed(self.typedef("int", "int"), {"kind": "CompoundStmt", "id": "block", "inner": [loop, after]})
        self.assertEqual(("short", "int"), (self.returned(collector, "inside"), self.returned(collector, "after")))
        while_body = {"kind": "WhileStmt", "id": "while", "inner": [{"kind": "CXXBoolLiteralExpr", "id": "w"}, short]}
        collector = self.indexed(self.typedef("int", "int"), {"kind": "CompoundStmt", "id": "block", "inner": [while_body, after]})
        self.assertEqual("int", self.returned(collector, "after"))
        for statement in ({"kind": "IfStmt", "id": "if", "inner": [{}]},
                          {"kind": "ForStmt", "id": "for", "inner": [{}, {}, {}, {}]},
                          {"kind": "WhileStmt", "id": "while", "hasVar": True, "inner": [{}, {}]},
                          {"kind": "IfStmt", "id": "if", "isConsteval": True, "inner": [{}, {}]}):
            with self.subTest(statement=statement), self.assertRaisesRegex(ValueError, "layout"):
                self.indexed(statement)
        with self.assertRaisesRegex(ValueError, "unsupported C\\+\\+ statement CXXTryStmt"):
            self.indexed({"kind": "CXXTryStmt", "id": "try"})

    def test_member_pointers_below_declarators_and_in_typeof_spellings_refuse(self) -> None:
        # GXX-IR4-02: the outer reference decides the category only after its tree is member-pointer free.
        member = {"kind": "MemberPointerType", "type": {"qualType": "int S::*"}, "isData": True,
                  "inner": [{"kind": "BuiltinType", "type": {"qualType": "int"}}]}
        reference = {"kind": "LValueReferenceType", "type": {"qualType": "int S::*const &"}, "inner": [
            {"kind": "QualType", "type": {"qualType": "int S::*const"}, "qualifiers": "const", "inner": [member]}]}
        collector = self.indexed({"kind": "TypedefDecl", "id": "r", "name": "R", "inner": [reference]},
                                 {"kind": "TypedefDecl", "id": "p", "name": "P", "inner": [
                                     {**reference, "kind": "PointerType", "inner": [{"kind": "BuiltinType", "type": {"qualType": "int"}}]}]})
        with self.assertRaisesRegex(ValueError, "member pointer type 'int S::\\*' inside 'int S::\\*const &'"):
            collector.classify_use({"qualType": "R", "typeAliasDeclId": "r"}, True)
        self.assertEqual("pointer", collector.classify_use({"qualType": "P", "typeAliasDeclId": "p"}, True).category)
        with self.assertRaisesRegex(ValueError, "typeof/decltype"):
            collector.classify_use({"qualType": "typeof (mp) *"}, True)
        with self.assertRaisesRegex(ValueError, "typeof/decltype"):
            collector.written_kind("decltype(mp) *", "", 0, frozenset())


@unittest.skipUnless(CLANG, f"Pinned clang_ast required ({CLANG_UNAVAILABLE})")
class CxxGateTests(unittest.TestCase):
    """C++ TUs through the pinned clang JSON AST; the text stands in for 2.8.1-preprocessed output."""
    PRELUDE = ('# 1 "src/units/x.cpp"\ntypedef signed int s32; typedef unsigned int u32; typedef unsigned char u8;\n'
               "typedef signed char s8; typedef u8 Byte; struct Obj;\n")

    def cxx(self, name: str, text: str, candidate: int | None = None):
        result = parse_cxx_tu(self.PRELUDE.replace("src/units/x.cpp", name) + text, name, "gxx", CLANG)
        result.candidate = candidate
        return result

    def outcome(self, text: str, peer: str, name: str = "scope.cpp"):
        """The C++ TU, then analyse's and the Gate's (kind, symbol, aspects) for it against a C peer."""
        cpp = self.cxx(name, text, candidate=0)
        c = unit("peer.c", peer)
        keys = {"object-category": "subkind"}  # object findings name their conflict, functions their aspect
        summary = [(f.kind, f.symbol, [d[keys.get(f.kind, "aspect")] for d in f.differences or []])
                   for f in analyse([c, cpp]).findings]
        gated = [(f.kind, f.symbol) for f in Gate([c, cpp]).check(0)]
        return cpp, summary, gated

    def assert_parse_failure(self, text: str, peer: str, error: str) -> None:
        cpp, summary, gated = self.outcome(text, peer)
        self.assertIn(error, cpp.error or "")
        self.assertEqual(["parse-failure"], [kind for kind, _, _ in summary])
        self.assertEqual(["parse-failure"], [kind for kind, _ in gated])

    def test_c_linkage_sites_calls_and_exact_kinds(self) -> None:
        result = self.cxx("src/units/x.cpp", (
            "enum Mode { MODE_A, MODE_B };\nenum Sign { NEG = -1 };\nnamespace ns { typedef long Count; }\n"
            'extern "C" {\n'
            "Obj *make(Obj *self, s32 kind, Byte flag, bool on, Mode mode, enum Sign sign, Obj &ref, ns::Count n, ...);\n"
            "static void hidden(void);\n"
            "}\n"
            'extern "C" bool probe(unsigned long value) { return value != 0; }\n'
            "void cxx_only(int);\n"
            "struct Obj { Obj(int k) { make(this, k, 1, true, MODE_A, NEG, *this, 3, 9); } };\n"
            "inline void helper() { probe(1); cxx_only(2); }\n"
            'extern "C" inline Byte peek(u8 *p) { return *p; }\n'))
        self.assertIsNone(result.error)
        shapes = {s.symbol: (s.role, s.local, s.shape.render(s.symbol), s.line) for s in result.sites}
        self.assertEqual({
            "make": ("declaration", False, "pointer make(pointer, int, char unsigned, bool, enum Mode (int unsigned), "
                                           "enum Sign (int), pointer, long, ...)", 7),
            "hidden": ("declaration", True, "void hidden(void)", 8),
            "probe": ("definition", False, "bool probe(long unsigned)", 10),
            "peek": ("definition", False, "char unsigned peek(pointer)", 14)}, shapes)
        self.assertEqual([("make", 9, 12), ("probe", 1, 13)], [(c.symbol, c.arguments, c.line) for c in result.calls])
        self.assertIs(CXX_INTEGERS["bool"], next(s for s in result.sites if s.symbol == "probe").shape.returns)

    def test_c_declarations_disagreeing_with_a_cpp_definition_and_the_reverse(self) -> None:
        cpp = self.cxx("src/units/defs.cpp", 'extern "C" s32 shared(u8 a, s32 b) { return a + b; }\n'
                                             'extern "C" bool flag(void) { return true; }\n'
                                             'extern "C" void cdef(u32 a);\n'
                                             'extern "C" void use_cdef(void) { cdef(1); }\n')
        c = unit("src/units/user.c", "s32 shared(signed char a, s32 b);\ns32 flag(void);\nvoid cdef(s32 a, s32 b) {}\n"
                                     "void caller(void) { shared(1); }\n")
        items = analyse([cpp, c]).findings
        summary = {(f.kind, f.symbol, f.site["file"], tuple(aspects(f))) for f in items}
        self.assertEqual({
            ("declaration-vs-definition", "shared", "src/units/user.c", ("parameter 1 signedness",)),
            ("call-arity", "shared", "src/units/user.c", ("arguments",)),
            ("declaration-vs-definition", "flag", "src/units/user.c", ("return cxx-type",)),
            ("declaration-vs-definition", "cdef", "src/units/defs.cpp", ("parameter count", "parameter 1 signedness")),
            ("call-arity", "cdef", "src/units/defs.cpp", ("arguments",)),
        }, summary)
        reference = next(f for f in items if f.symbol == "shared" and f.kind == "declaration-vs-definition").reference
        self.assertEqual(("src/units/defs.cpp", 3, "definition"), (reference["file"], reference["line"], reference["role"]))

    def test_cxx_external_objects_join_the_unchanged_c_gate(self) -> None:
        cpp = self.cxx("objects.cpp", 'typedef u8 Row[4]; typedef Row Table[2];\n'
                       'extern "C" { extern void *number; extern Table table; float real; }\n'
                       'extern u32 compatible;\nextern Obj *view;\n')
        self.assertIsNone(cpp.error)
        c = unit("objects.c", "extern s32 number; extern s32 *table; extern s32 real;\n"
                             "extern s32 compatible; extern ViewA *view;\n")
        report = analyse([cpp, c])
        self.assertEqual({("number", "pointer-vs-integer"), ("table", "array-vs-pointer"), ("real", "float-vs-integer")},
                         {(f.symbol, f.differences[0]["subkind"]) for f in report.findings})
        self.assertTrue(all(f.kind == "object-category" for f in report.findings))
        self.assertEqual(["array", "pointer"], report.registry()["objects"]["table"]["categories"])
        cpp.candidate = 0
        self.assertEqual(3, len(Gate([c, cpp]).check(0)))

    def test_cpp_object_linkage_roles_and_declarators(self) -> None:
        cpp = self.cxx("objects.cpp", 'static s32 hidden; extern s32 hidden;\nconst s32 local_const = 1;\n'
                       'namespace { extern int anonymous; }\nextern const s32 visible;\n'
                       'extern "C" const s32 initialized = 1;\nextern "C" s32 declaration;\n'
                       'extern "C" { s32 definition; const s32 another_local = 1; }\n'
                       'typedef s32 Row[4]; extern Row rows;\nextern s32 (* const row_pointer)[4];\n'
                       'extern void (*callbacks[2])(void);\n'
                       'namespace N { extern "C" s32 namespaced; }\n'
                       'extern "C" void user(void) { s32 automatic; static s32 saved;\n'
                       'typedef void *Row; extern Row block; extern s32 hidden; }\n')
        self.assertIsNone(cpp.error, cpp.error)
        self.assertEqual([
            ("visible", "declaration", "integer"), ("initialized", "definition", "integer"),
            ("declaration", "declaration", "integer"), ("definition", "definition", "integer"),
            ("rows", "declaration", "array"), ("row_pointer", "declaration", "pointer"),
            ("callbacks", "declaration", "array"), ("namespaced", "declaration", "integer"),
            ("block", "declaration", "pointer")], [(s.symbol, s.role, s.category) for s in cpp.objects])

    def test_redeclared_functions_and_transparent_direct_calls(self) -> None:
        cpp = self.cxx("calls.cpp", 'extern "C" void api(s32);\nvoid api(s32 value) {}\n'
                       'extern "C" void user(void) { (api)(1); (*api)(2); (&api)(3); }\n')
        self.assertIsNone(cpp.error, cpp.error)
        self.assertEqual([("api", "declaration"), ("api", "definition"), ("user", "definition")],
                         [(s.symbol, s.role) for s in cpp.sites])
        self.assertEqual([("api", 1)] * 3, [(c.symbol, c.arguments) for c in cpp.calls])

    def test_scoped_return_types_reject_scalar_c_peers_at_the_gate(self) -> None:
        cases = {
            "namespace": ('typedef int T; namespace N { struct T { int x; }; extern "C" T f(void); }', "N::T"),
            "nested namespace": ('typedef int T; namespace N { namespace Inner { struct T { int x; }; '
                                 'extern "C" T f(void); } }', "N::Inner::T"),
            "class typedef": ('typedef int T; struct N { struct T { int x; }; typedef T Return; }; '
                              'extern "C" N::Return f(void);', "N::T"),
        }
        for label, (text, identity) in cases.items():
            with self.subTest(case=label):
                cpp = self.cxx("scope.cpp", text, candidate=0)
                self.assertIsNone(cpp.error, cpp.error)
                self.assertEqual((identity, "aggregate"),
                                 (cpp.sites[0].shape.returns.name, cpp.sites[0].shape.returns.category))
                c = unit("peer.c", "int f(void);")
                report = analyse([c, cpp])
                self.assertEqual([("declaration-disagreement", "f", ["return"])],  # a category difference is the bare aspect
                                 [(f.kind, f.symbol, aspects(f)) for f in report.findings])
                self.assertEqual(report.findings[0].to_json(), Gate([c, cpp]).check(0)[0].to_json())

    def test_scoped_integer_aliases_and_typedef_binding_do_not_leak(self) -> None:
        cpp = self.cxx("scope.cpp", 'typedef int T;\n'
                       'namespace A { typedef short T; extern "C" T narrow(void); }\n'
                       'namespace B { typedef unsigned long T; extern "C" T wide(void); }\n'
                       'namespace N { typedef T Saved; struct T { int x; }; extern "C" Saved saved(void); }\n'
                       'extern "C" T global(void);\n', candidate=0)
        self.assertIsNone(cpp.error, cpp.error)
        self.assertEqual(["short", "long unsigned", "int", "int"], [s.shape.returns.name for s in cpp.sites])
        c = unit("peer.c", "short narrow(void); unsigned long wide(void); int saved(void); int global(void);")
        self.assertEqual([], Gate([c, cpp]).check(0))

    def test_qualified_prefixes_never_fall_back_to_shadowed_outer_types(self) -> None:
        cases = {
            "inherited": ('struct Base { struct T { int x; }; }; struct Derived { typedef int T; }; '
                          'namespace N { struct Derived : Base {}; extern "C" Derived::T f(void); }',
                          "inherited type lookup"),
            "class alias": ('struct A { typedef int T; }; namespace N { struct X { struct T { int x; }; }; '
                            'typedef X A; extern "C" A::T f(void); }', "unsupported C++ type qualifier"),
            "nested inherited": ('namespace Holder { struct Derived { typedef int T; }; } '
                                 'namespace N { namespace Holder { struct Base { struct T { int x; }; }; '
                                 'struct Derived : Base {}; } extern "C" Holder::Derived::T f(void); }',
                                 "inherited type lookup"),
            "nested alias": ('namespace Holder { struct A { typedef int T; }; } '
                             'namespace N { namespace Holder { struct X { struct T { int x; }; }; '
                             'typedef X A; } extern "C" Holder::A::T f(void); }',
                             "unsupported C++ type qualifier"),
        }
        for label, (text, error) in cases.items():
            with self.subTest(case=label):
                cpp = self.cxx("scope.cpp", text, candidate=0)
                self.assertIn(error, cpp.error or "")
                c = unit("peer.c", "int f(void);")
                self.assertEqual(["parse-failure"], [f.kind for f in analyse([c, cpp]).findings])
                self.assertEqual(["parse-failure"], [f.kind for f in Gate([c, cpp]).check(0)])

    def test_resolved_relative_and_absolute_qualifiers_keep_their_own_bindings(self) -> None:
        cases = {
            "relative record": ('struct S { typedef int T; }; namespace N { struct S { typedef short T; }; '
                                'extern "C" S::T f(void); }', "short"),
            "relative namespace": ('namespace S { typedef int T; } namespace N { namespace S { typedef long T; } '
                                   'extern "C" S::T f(void); }', "long"),
            "absolute": ('struct S { typedef int T; }; namespace N { struct S { typedef short T; }; '
                         'extern "C" ::S::T f(void); }', "int"),
            "outer namespace": ('namespace S { typedef short T; } namespace N { namespace Inner { '
                                'extern "C" S::T f(void); } }', "short"),
            "declared derived member": ('struct Base { typedef int T; }; struct Derived : Base { typedef short T; }; '
                                        'extern "C" Derived::T f(void);', "short"),
        }
        for label, (text, returns) in cases.items():
            with self.subTest(case=label):
                cpp = self.cxx("scope.cpp", text, candidate=0)
                self.assertIsNone(cpp.error, cpp.error)
                self.assertEqual(returns, cpp.sites[0].shape.returns.name)
                c = unit("peer.c", f"{returns} f(void);")
                self.assertEqual([], analyse([c, cpp]).findings)
                self.assertEqual([], Gate([c, cpp]).check(0))

    def test_block_alias_identities_survive_unqualified_desugaring_and_later_shadows(self) -> None:
        cases = [
            ('typedef int T; extern "C" void user(void) { struct T { int x; }; typedef T R; '
             'extern void f(R); }', "void f(int);", "parameter 1"),
            ('typedef int T; extern "C" void user(void) { struct T { int x; }; typedef T R; '
             'typedef R Saved; extern Saved f(void); }', "int f(void);", "return"),
            ('struct T { int x; }; typedef T R; extern "C" void user(void) { typedef int T; '
             'extern void f(R); }', "void f(int);", "parameter 1"),
        ]
        for text, peer, aspect in cases:
            with self.subTest(text=text):
                cpp = self.cxx("scope.cpp", text, candidate=0)
                self.assertIsNone(cpp.error, cpp.error)
                c = unit("peer.c", peer)
                report = analyse([c, cpp])
                self.assertEqual([("declaration-disagreement", "f", [aspect])],
                                 [(f.kind, f.symbol, aspects(f)) for f in report.findings])
                self.assertEqual(report.findings[0].to_json(), Gate([c, cpp]).check(0)[0].to_json())

    def test_elaborated_block_tags_bind_by_identity_not_by_scope_less_spelling(self) -> None:
        # GXX-IR3-01: clang prints a function-local tag without its scope; the global typedef of the same
        # spelling must never classify it.
        local = 'extern "C" void user(void) { '
        cases = {
            "struct parameter": ('typedef int T; ' + local + 'struct T { int x; }; extern void f(struct T); }',
                                 "void f(int);", ("declaration-disagreement", "f", ["parameter 1"])),
            "class parameter": ('typedef int T; ' + local + 'class T { int x; }; extern void f(class T); }',
                                "void f(int);", ("declaration-disagreement", "f", ["parameter 1"])),
            "union parameter": ('typedef int T; ' + local + 'union T { int x; }; extern void f(union T); }',
                                "void f(int);", ("declaration-disagreement", "f", ["parameter 1"])),
            "typedef parameter": ('typedef int T; ' + local + 'struct T { int x; }; typedef struct T R; extern void f(R); }',
                                  "void f(int);", ("declaration-disagreement", "f", ["parameter 1"])),
            "typedef return": ('typedef int T; ' + local + 'struct T { int x; }; typedef struct T R; extern R f(void); }',
                               "int f(void);", ("declaration-disagreement", "f", ["return"])),
            "namespaced function": ('typedef int T; namespace N { ' + local + 'struct T { int x; }; '
                                    'extern void f(struct T); } }', "void f(int);",
                                    ("declaration-disagreement", "f", ["parameter 1"])),
            "enum against typedef": ('typedef unsigned short E; ' + local + 'enum E { A = 1 }; extern void f(enum E); }',
                                     "void f(unsigned short);", ("declaration-disagreement", "f", ["parameter 1 width"])),
            "enum return against typedef": ('typedef unsigned short E; ' + local + 'enum E { A = 1 }; '
                                            'extern enum E f(void); }', "unsigned short f(void);",
                                            ("declaration-disagreement", "f", ["return width"])),
            "enum against record": ('struct E { int x; }; ' + local + 'enum E { A = -1 }; extern void f(enum E); }',
                                    "struct E { int x; }; void f(struct E);",
                                    ("declaration-disagreement", "f", ["parameter 1"])),
            "enum object against pointer typedef": ('typedef void *T; ' + local + 'enum T { A = 1 }; extern enum T obj; }',
                                                    "extern int *obj;", ("object-category", "obj", ["pointer-vs-integer"])),
            "unnamed class member typedef": ('typedef int T; namespace N { static struct { typedef short T; } x; '
                                             'extern "C" T f(void); }', "short f(void);",
                                             ("declaration-disagreement", "f", ["return width"])),
        }
        for label, (text, peer, expected) in cases.items():
            with self.subTest(case=label):
                cpp, summary, gated = self.outcome(text, peer)
                self.assertIsNone(cpp.error, cpp.error)
                self.assertEqual([expected], summary)
                self.assertEqual([expected[:2]], gated)

    def test_tag_spellings_without_one_agreeing_declaration_fail_closed(self) -> None:
        self.assert_parse_failure('struct E { int x; }; extern "C" void user(void) { enum E { A = -1 }; extern void f(E); }',
                                  "void f(int);", "ambiguous C++ type identity")
        self.assert_parse_failure('extern "C" { enum { A = -1 } anonymous; }', "extern int *anonymous;",
                                  "has no declaration identity")

    def test_mixed_sign_enums_wider_than_32_bits_fail_closed(self) -> None:
        # GXX-IR3-02: clang widens these to long long; no 32-bit type holds -1 and a value above INT_MAX.
        for enumerators in ("A = -1, B = 0xFFFFFFFFU", "A = -1LL, B = 0x80000000LL"):
            for peer in ("void f(int);", "void f(unsigned int);"):
                with self.subTest(enumerators=enumerators, peer=peer):
                    self.assert_parse_failure('enum E { ' + enumerators + ' }; extern "C" void f(E);', peer,
                                              "outside supported 32-bit range")

    def test_member_function_bodies_see_the_complete_class(self) -> None:
        # GXX-IR3-04: a member body (inline or out-of-line) looks up class members declared after it.
        cases = {
            "inline member body": 'typedef int T; extern "C" { struct S { void m() { extern T f(void); } typedef short T; }; }',
            "inline friend body": ('typedef int T; extern "C" { struct S { friend void g(void) { extern T f(void); } '
                                   'typedef short T; }; }'),
            "out-of-line member body": ('typedef int T; struct C { void m(); typedef short T; }; '
                                        'extern "C" { void C::m() { extern T f(void); } }'),
            "out-of-line namespace member body": ('typedef int T; extern "C" { namespace Q { void h(); typedef short T; } } '
                                                  'extern "C" { void Q::h() { extern T f(void); } }'),
        }
        for label, text in cases.items():
            with self.subTest(case=label):
                cpp, summary, gated = self.outcome(text, "int f(void);")
                self.assertIsNone(cpp.error, cpp.error)
                self.assertEqual([("declaration-disagreement", "f", ["return width"])], summary)
                self.assertEqual([("declaration-disagreement", "f")], gated)
                self.assertEqual([], self.outcome(text, "short f(void);")[2])

    def test_class_scope_lookups_that_a_later_member_or_base_could_change_fail_closed(self) -> None:
        self.assert_parse_failure('typedef int T; extern "C" { struct S { friend T f(void); typedef short T; }; }',
                                  "int f(void);", "declared after its use")
        self.assert_parse_failure('typedef int T; struct B { typedef short T; }; '
                                  'extern "C" { struct S : B { void m() { extern T f(void); } }; }',
                                  "int f(void);", "inherited type lookup")
        # Constructors, destructors and conversions are no later members named like the class.
        cpp, summary, _ = self.outcome('extern "C" { struct S { friend S make(void); S(); ~S(); operator int(); int v; }; }',
                                       "struct S { int v; }; struct S make(void);")
        self.assertEqual((None, []), (cpp.error, summary))

    def test_pointer_arithmetic_in_enum_dependencies_is_layout_dependent(self) -> None:
        # GXX-IR3-05: clang scales by its own sizeof(bool) == 1; g++ 2.8.1's bool is 4 bytes.
        for text in ('enum E { A = (int)&((bool*)0)[1] - 2 }; extern "C" void f(E);',
                     'bool arr[4] = { 0 }; enum E { A = (char*)&arr[2] - (char*)&arr[0] - 3 }; extern "C" void f(E);'):
            with self.subTest(text=text):
                self.assert_parse_failure(text, "void f(int);", "layout-dependent")

    def test_anonymous_namespace_members_are_visible_in_their_enclosing_namespace(self) -> None:
        # GXX-IR3-06: clang's implicit using-directive is modelled, not refused.
        cpp, summary, _ = self.outcome('typedef int T; namespace P { namespace { typedef short T; } '
                                       'extern "C" T f(void); }', "int f(void);")
        self.assertIsNone(cpp.error, cpp.error)
        self.assertEqual([("declaration-disagreement", "f", ["return width"])], summary)
        cpp, summary, gated = self.outcome('namespace { typedef short T; } extern "C" T f(void); '
                                           'namespace N { const int n = 3; }', "short f(void);")
        self.assertEqual((None, [], []), (cpp.error, summary, gated))
        # Clang prints the namespace as "(anonymous namespace)::E"; the enum still binds by identity.
        text = 'namespace { enum E { A = -1 }; } extern "C" void f(E);'
        self.assertEqual([], self.outcome(text, "void f(int);")[1])
        self.assertEqual([("declaration-disagreement", "f", ["parameter 1 enum-underlying"])],
                         self.outcome(text, "void f(unsigned int);")[1])

    def test_unnamed_tags_bind_through_their_typedef_identity(self) -> None:
        text = ('typedef struct { int x; } Anon; typedef enum { X = -1 } Signed; '
                'extern "C" Anon g(void); extern "C" void h(Anon *, Signed);')
        cpp, summary, gated = self.outcome(text, "typedef struct { int x; } Anon; Anon g(void); void h(Anon *, int);")
        self.assertEqual((None, [], []), (cpp.error, summary, gated))
        self.assertEqual(["Anon g(void)", "void h(pointer, enum Signed (int))"], [s.shape.render(s.symbol) for s in cpp.sites])
        _, summary, _ = self.outcome(text, "int g(void); void h(void *, unsigned int);")
        self.assertEqual([("declaration-disagreement", "g", ["return"]),
                          ("declaration-disagreement", "h", ["parameter 2 enum-underlying"])], summary)

    def test_unused_implicit_builtin_typedefs_resolve_by_identity(self) -> None:
        # GXX-IR3-07: __builtin_va_list is an implicit typedef; it no longer refuses the TU.
        cpp, summary, gated = self.outcome('typedef __builtin_va_list va_list; extern "C" void v(int); '
                                           'extern "C" void w(va_list);', "void v(int); void w(void *);")
        self.assertEqual((None, [], []), (cpp.error, summary, gated))
        self.assertEqual("pointer", cpp.sites[1].shape.params[0].category)

    def test_inline_namespaces_fail_closed(self) -> None:
        # GXX-IR3-08: an inline namespace imports its members into the enclosing namespace.
        self.assert_parse_failure('typedef int T; namespace N { inline namespace I { struct T { int x; }; } '
                                  'extern "C" T f(void); }', "int f(void);", "inline NamespaceDecl")

    def test_unmodelled_scope_imports_fail_closed_instead_of_using_outer_typedef(self) -> None:
        for imported in ("using N::T;", "using namespace N;"):
            with self.subTest(imported=imported):
                cpp = self.cxx("scope.cpp", 'typedef int T; namespace N { struct T { int x; }; }\n'
                               'namespace Inner { ' + imported + ' extern "C" T f(void); }\n', candidate=0)
                self.assertIsNotNone(cpp.error)
                self.assertEqual(["parse-failure"], [f.kind for f in Gate([unit("peer.c", "int f(void);"), cpp]).check(0)])

    def test_friend_only_tags_never_bind_and_lookups_past_them_fail_closed(self) -> None:
        # GXX-IR4-01: a tag first declared by a friend is invisible to ordinary lookup (C++98 7.3.1.2/3);
        # it must neither bind (clang binds the outer name) nor be skipped silently (pre-standard friend
        # injection made it visible).
        aggregate = "struct X { int x; }; struct X f(void);"
        cases = {
            "scalar outer": ('typedef int T; namespace N { struct S { friend class T; }; extern "C" T f(void); }', aggregate),
            "scalar outer, agreeing peer": ('typedef int T; namespace N { struct S { friend class T; }; '
                                            'extern "C" T f(void); }', "int f(void);"),
            "namespace alias": ('namespace O { typedef short T; namespace N { struct S { friend class T; }; '
                                'extern "C" T f(void); } }', aggregate),
            "enum outer": ('enum T { A = -1 }; namespace N { struct S { friend class T; }; extern "C" T f(void); }', aggregate),
            "global struct": ('struct T { int x; }; namespace N { struct S { friend class T; }; extern "C" T f(void); }',
                              "int f(void);"),
            "elaborated enum": ('enum T { A = 1 }; namespace N { struct S { friend class T; }; extern "C" enum T f(void); }',
                                "unsigned int f(void);"),
            "anonymous namespace": ('typedef int T; namespace N { namespace { struct S { friend class T; }; } '
                                    'extern "C" T f(void); }', aggregate),
            "member body": ('typedef int T; namespace N { extern "C" { struct S { friend class T; '
                            'void m() { extern T f(void); } }; } }', aggregate),
        }
        for label, (text, peer) in cases.items():
            with self.subTest(case=label):
                self.assert_parse_failure(text, peer, "only declared by a friend declaration")
        # The elaborated spelling names a class without lookup; clang's own lookup finds the typedef.
        self.assert_parse_failure('typedef int T; namespace N { struct S { friend class T; }; extern "C" class T f(void); }',
                                  "int f(void);", "cannot be referenced with the 'class' specifier")

    def test_friend_tags_bind_once_an_ordinary_declaration_introduces_them(self) -> None:
        cases = {
            "later redeclaration": ('typedef int T; namespace N { struct S { friend class T; }; class T { int x; }; '
                                    'extern "C" T f(void); }'),
            "prior declaration": 'namespace N { class T { int x; }; struct S { friend class T; }; extern "C" T f(void); }',
            "out-of-line definition": ('typedef int T; namespace N { struct S { friend class T; }; } class N::T { int x; }; '
                                       'namespace N { extern "C" T f(void); }'),
            "elaborated redeclaration": ('namespace N { struct S { friend class T; }; extern "C" class T g(void); '
                                         'extern "C" T f(void); }'),
            "local friend of an outer class": ('struct T { int x; }; extern "C" void user(void) { '
                                               'struct L { friend class T; }; extern T f(void); }'),
            # A local class's elaborated tag belongs to the enclosing block, not to the class.
            "local elaborated tag": 'extern "C" void user(void) { struct L { struct T *p; }; extern T f(void); }',
        }
        for label, text in cases.items():
            with self.subTest(case=label):
                cpp, summary, gated = self.outcome(text, "int f(void);")
                self.assertIsNone(cpp.error, cpp.error)
                self.assertEqual([("declaration-disagreement", "f", ["return"])], summary)
                self.assertEqual([("declaration-disagreement", "f")], gated)
                self.assertEqual([], self.outcome(text, "struct X { int x; }; struct X f(void);")[2])
        # A parameter binds clang's typedef identity; the friend tag does not touch it.
        text = 'typedef int T; namespace N { struct S { friend class T; }; extern "C" void f(T); }'
        cpp, summary, gated = self.outcome(text, "void f(int);")
        self.assertEqual((None, [], []), (cpp.error, summary, gated))
        self.assertEqual([("declaration-disagreement", "f", ["parameter 1 width"])], self.outcome(text, "void f(short);")[1])

    def test_member_pointers_fail_closed_behind_declarators_typedefs_and_typeof(self) -> None:
        # GXX-IR4-02: the outer pointer, reference or array decides the category, but the unsupported
        # member pointer subset must not pass because it sits below one.
        member = "struct S { int x; void m(); }; "
        cases = {
            "reference parameter": ('typedef int S::* const & R; extern "C" void f(R);', "void f(void *);"),
            "reference return": ('typedef int S::* const & R; extern "C" R f(void);', "void *f(void);"),
            "array object": ('typedef int S::* R[2]; extern "C" R obj;', "extern int obj[2];"),
            "array parameter": ('typedef int S::* R[2]; extern "C" void f(R);', "void f(void *);"),
            "pointer parameter": ('typedef int S::** P; extern "C" void f(P);', "void f(void *);"),
            "typedef chain": ('typedef int S::* M; typedef M *PM; typedef PM A[2]; extern "C" A obj;', "extern int *obj[2];"),
            "member function reference": ('typedef void (S::* const & MR)(); extern "C" void f(MR);', "void f(void *);"),
            "function pointer parameter": ('typedef void (*FP)(int S::*); extern "C" void f(FP);', "void f(void *);"),
            "typeof typedef": ('typedef __typeof__(&S::x) *P; extern "C" void f(P);', "void f(void *);"),
            "decltype typedef": ('static int S::* mp; typedef __decltype(mp) *P; extern "C" void f(P);', "void f(void *);"),
        }
        for label, (text, peer) in cases.items():
            with self.subTest(case=label):
                self.assert_parse_failure(member + text, peer, "member pointer type")
        # Without a typedef's type tree a typeof spelling hides its operand's type.
        for text, peer in (('static int S::* mp; extern "C" void f(__typeof__(mp) *);', "void f(void *);"),
                           ('static int S::* mp; extern "C" __typeof__(mp) *obj;', "extern void *obj;")):
            with self.subTest(text=text):
                self.assert_parse_failure(member + text, peer, "typeof/decltype")
        # Function pointers, references to pointers and a desugared top-level typeof still classify.
        cpp, summary, gated = self.outcome('typedef void (*FP)(int); typedef FP FPA[2]; typedef Obj *OP; typedef OP &OPR; '
                                           'int x; extern "C" void f(FP, OPR, __typeof__(x)); extern "C" FPA obj;',
                                           "struct Obj; void f(void (*)(int), struct Obj *, int); extern void (*obj[2])(int);")
        self.assertEqual((None, [], []), (cpp.error, summary, gated))
        self.assertEqual("void f(pointer, pointer, int)", cpp.sites[0].shape.render("f"))

    def test_selection_and_iteration_statements_scope_their_declarations(self) -> None:
        # GXX-PREEXISTING-SCOPE-01: each selection/iteration statement and each (even unbraced) substatement
        # is a scope; the declarations they hold never reach a later lookup.
        local = 'typedef int T; extern "C" void user(void) { '
        aggregate = "struct X { int x; }; struct X f(void);"
        leaks = {
            "for init": ('for (typedef short T; false;) {} extern T f(void); }', "short f(void);", "return width"),
            "if": ('if (true) typedef short T; extern T f(void); }', "short f(void);", "return width"),
            "while": ('while (false) typedef short T; extern T f(void); }', "short f(void);", "return width"),
            "for record": ('for (struct T { int x; } v; false;) {} extern T f(void); }', aggregate, "return"),
            "switch": ('switch (0) case 0: typedef short T; extern T f(void); }', "short f(void);", "return width"),
            "do": ('do typedef short T; while (false); extern T f(void); }', "short f(void);", "return width"),
            "else branch": ('if (true) typedef short T; else extern T f(void); }', "short f(void);", "return width"),
            "nested statements": ('if (1) for (typedef short T;;) ; extern T f(void); }', "short f(void);", "return width"),
            "range for": ('int a[2]; for (int x : a) typedef short T; extern T f(void); }', "short f(void);", "return width"),
            "else record": ('if (false) ; else struct T { int x; } v; extern T f(void); }', aggregate, "return"),
            "while body block": ('while (int n = 0) { typedef short T; } extern T f(void); }', "short f(void);", "return width"),
        }
        for label, (body, peer, aspect) in leaks.items():
            with self.subTest(case=label):
                cpp, summary, gated = self.outcome(local + body, peer)
                self.assertIsNone(cpp.error, cpp.error)
                self.assertEqual("int", next(s for s in cpp.sites if s.symbol == "f").shape.returns.name)
                self.assertEqual([("declaration-disagreement", "f", [aspect])], summary)
                self.assertEqual([("declaration-disagreement", "f")], gated)
        # Inside the statement its declarations apply; a label is no scope.
        scoped = {
            "for init in body": 'for (typedef short T; false;) extern T f(void); }',
            "if init in branch": 'if (typedef short T; true) { extern T f(void); } }',
            "label": 'L: typedef short T; extern T f(void); }',
            "braced switch": 'switch (0) { case 0: typedef short T; extern T f(void); } }',
        }
        for label, body in scoped.items():
            with self.subTest(case=label):
                cpp, summary, gated = self.outcome(local + body, "int f(void);")
                self.assertIsNone(cpp.error, cpp.error)
                self.assertEqual([("declaration-disagreement", "f", ["return width"])], summary)
                self.assertEqual([], self.outcome(local + body, "short f(void);")[2])
        # A statement kind the scope model does not know refuses (a captured body re-lists its declarations).
        self.assert_parse_failure(local + '\n#pragma clang __debug captured\n{ typedef short T; }\nextern T f(void); }',
                                  "int f(void);", "unsupported C++ statement CapturedStmt")

    def test_transitive_enum_layout_dependence_is_a_consumer_visible_parse_failure(self) -> None:
        cases = [
            'const int n = sizeof(bool); enum E { A = n - 2 }; extern "C" void f(E);',
            'const int n = sizeof(bool); const int alias = n; enum E { A = alias - 2 }; extern "C" void f(E);',
            'namespace N { const int n = sizeof(bool); } const int n = 3; '
            'enum E { A = N::n - 2 }; extern "C" void f(E);',
            'enum { first = sizeof(bool), next }; enum E { A = next - 3 }; extern "C" void f(E);',
        ]
        for text in cases:
            with self.subTest(text=text):
                cpp = self.cxx("enum.cpp", text, candidate=0)
                self.assertIn("layout-dependent", cpp.error or "")
                c = unit("peer.c", "void f(int);")
                self.assertEqual(["parse-failure"], [f.kind for f in analyse([c, cpp]).findings])
                self.assertEqual(["parse-failure"], [f.kind for f in Gate([c, cpp]).check(0)])

    def test_static_member_constant_dependencies_are_checked_by_identity(self) -> None:
        cases = [
            'struct S { static const int n=sizeof(bool); }; const S s={}; '
            'enum E { A=s.n-2 }; extern "C" void f(E);',
            'struct S { static const int n=sizeof(bool); }; const S s={}; const int alias=s.n; '
            'enum E { A=alias-2 }; extern "C" void f(E);',
            'struct S { static const int n=sizeof(bool); }; const S s={}; '
            'enum E { A=static_cast<unsigned int>(s.n+1), B }; extern "C" void f(E);',
            'struct S { static const int n=__alignof__(bool); }; const S s={}; '
            'enum E { A=s.n-2 }; extern "C" void f(E);',
            'struct S { int value; static const int n=1; }; const S s={sizeof(bool)}; '
            'enum E { A=s.n-2 }; extern "C" void f(E);',
        ]
        for text in cases:
            with self.subTest(text=text):
                cpp = self.cxx("enum.cpp", text, candidate=0)
                self.assertIn("layout-dependent", cpp.error or "")
                c = unit("peer.c", "void f(int);")
                self.assertEqual(["parse-failure"], [f.kind for f in analyse([c, cpp]).findings])
                self.assertEqual(["parse-failure"], [f.kind for f in Gate([c, cpp]).check(0)])

    def test_target_independent_constant_aliases_keep_exact_enum_compatibility(self) -> None:
        cpp = self.cxx("enum.cpp", 'const int n = 1; const int alias = n + 2; '
                       'enum E { A = alias - 4, B }; extern "C" void f(E);', candidate=0)
        self.assertIsNone(cpp.error, cpp.error)
        self.assertEqual("int", cpp.sites[0].shape.params[0].underlying)
        self.assertEqual([], Gate([unit("peer.c", "void f(int);"), cpp]).check(0))
        rejected = Gate([unit("peer.c", "void f(unsigned int);"), cpp]).check(0)
        self.assertEqual([("declaration-disagreement", "f", ["parameter 1 enum-underlying"])],
                         [(f.kind, f.symbol, aspects(f)) for f in rejected])

    def test_nonnegative_enum_conversions_keep_exact_unsigned_compatibility(self) -> None:
        cases = [
            'enum E { A=1 };',
            'enum E { A=0, B };',
            'enum E { A=(1 + 2), B };',
            'const int n=1; const int alias=n+2; enum E { A=alias, B };',
            'struct S { static const int n=3; }; const S s={}; enum E { A=s.n, B };',
            'enum E { A=static_cast<unsigned char>(255), B };',
            'enum E { A=0xFFFFFFFFU };',
            'enum E { A=1ULL << 31 };',
        ]
        for text in cases:
            with self.subTest(text=text):
                cpp = self.cxx("enum.cpp", text + ' extern "C" void f(E);', candidate=0)
                self.assertIsNone(cpp.error, cpp.error)
                self.assertEqual("int unsigned", cpp.sites[0].shape.params[0].underlying)
                c = unit("peer.c", "void f(unsigned int);")
                self.assertEqual([], analyse([c, cpp]).findings)
                self.assertEqual([], Gate([c, cpp]).check(0))
                rejected = Gate([unit("peer.c", "void f(int);"), cpp]).check(0)
                self.assertEqual([("declaration-disagreement", "f", ["parameter 1 enum-underlying"])],
                                 [(f.kind, f.symbol, aspects(f)) for f in rejected])

    def test_enum_conversions_do_not_admit_unmodelled_integer_ranges(self) -> None:
        for enumerators in ("A=1ULL << 32", "A=0xFFFFFFFFU, B", "A=-2147483649LL"):
            with self.subTest(enumerators=enumerators):
                cpp = self.cxx("enum.cpp", 'enum E { ' + enumerators + ' }; extern "C" void f(E);', candidate=0)
                self.assertIn("outside supported 32-bit range", cpp.error or "")
                c = unit("peer.c", "void f(unsigned int);")
                self.assertEqual(["parse-failure"], [f.kind for f in analyse([c, cpp]).findings])
                self.assertEqual(["parse-failure"], [f.kind for f in Gate([c, cpp]).check(0)])

    def test_static_member_definitions_preserve_the_mangled_object_gate(self) -> None:
        for initializer in ("3", "sizeof(bool)"):
            with self.subTest(initializer=initializer):
                cpp = self.cxx("enum.cpp", 'struct S { static const int n=' + initializer + '; }; '
                               'const int S::n; const S s={}; enum E { A=s.n, B }; extern "C" void f(E);',
                               candidate=0)
                self.assertIn("external C++ object n has unsupported mangled linkage", cpp.error or "")
                c = unit("peer.c", "void f(unsigned int);")
                self.assertEqual(["parse-failure"], [f.kind for f in analyse([c, cpp]).findings])
                self.assertEqual(["parse-failure"], [f.kind for f in Gate([c, cpp]).check(0)])

    def test_mangled_external_objects_and_layout_dependent_enums_fail_closed(self) -> None:
        for text in ["namespace N { extern int shared; }\n",
                     'enum Mode { A = sizeof(bool) - 2 }; extern "C" void f(Mode);\n']:
            with self.subTest(text=text):
                cpp = self.cxx("unsupported.cpp", text)
                self.assertIsNotNone(cpp.error)
                self.assertEqual(["parse-failure"], [f.kind for f in analyse([cpp]).findings])

    def test_unanalysable_cpp_tus_are_parse_failures(self) -> None:
        cases = {"syntax": "extern \"C\" void broken(\n",
                 "template": "template <class T> T twice(T x) { return x + x; }\n",
                 "member pointer": 'struct S { int a; };\nextern "C" void f(int S::*member);\n',
                 "unknown type": 'extern "C" __int128 wide(void);\n'}
        results = [self.cxx(f"src/{name.replace(' ', '_')}.cpp", text, candidate=index) for index, (name, text) in enumerate(cases.items())]
        results.append(parse_cxx_tu(self.PRELUDE, "src/no_clang.cpp", "gxx", None, unavailable="pinned clang_ast missing"))
        for name, result in zip([*cases, "missing clang"], results):
            with self.subTest(case=name):
                self.assertIsNotNone(result.error)
                self.assertTrue(result.error.startswith("parse: "), result.error)
        self.assertIn("src/syntax.cpp:", results[0].error)  # clang diagnostics map back through linemarkers
        self.assertEqual(["parse-failure"] * 5, [f.kind for f in analyse(results).findings])


if __name__ == "__main__":
    unittest.main()
