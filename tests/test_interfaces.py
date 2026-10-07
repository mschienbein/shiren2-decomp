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

from interfaces import PROJECT, Gate, analyse, collect, display, main, normalise, parse_tu  # noqa: E402

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
        cls.header = root / "inc/probe_api.h"
        profile = "gcc281pm-gnu291-O2-unsigned"
        # One canonical collection serves the gate and the candidate-preprocessing checks.
        cls.results = collect(PROJECT, [{"source": str(root / "good.c"), "profile": profile, "includes": [str(root / "inc")]},
                                        {"source": str(root / "bad.c"), "profile": profile}])

    @classmethod
    def tearDownClass(cls) -> None:
        cls.temporary.cleanup()

    def test_canonical_tree_has_no_interface_findings(self) -> None:
        report = analyse(r for r in self.results if r.candidate is None)
        self.assertGreater(len(report.results), 0)
        self.assertEqual([], [f.summary() for f in report.findings])

    def test_candidates_preprocess_with_their_include_paths(self) -> None:
        good, bad = (r for r in self.results if r.candidate is not None)
        self.assertIsNone(good.error)
        self.assertEqual([(display(str(self.header)), 1, "declaration"), ("probe_caller", "definition")],
                         [(s.file, s.line, s.role) if s.symbol == "probe_api" else (s.symbol, s.role) for s in good.sites])
        self.assertTrue(bad.error.startswith("preprocess: exit"), bad.error)
        gate = Gate(self.results)
        self.assertEqual({0: [], 1: ["parse-failure"]}, {index: [f.kind for f in gate.check(index)] for index in (0, 1)})


if __name__ == "__main__":
    unittest.main()
