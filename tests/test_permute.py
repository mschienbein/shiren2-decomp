"""Behaviour regression for the proposed tools/permute.py search ladder (no game compiles).

Run: uv run --frozen python -m unittest discover -s <proposal>/tests -p test_permute.py

Process tests use only finite children. Each descendant holds the write end of a test pipe, so its
death is observed as EOF, never by probing or signalling a remembered PID. The only signals sent
by tests go to a direct, unreaped helper child. Import tests run in fresh child interpreters.
"""
from __future__ import annotations

import hashlib
import importlib.util
import json
import marshal
import os
import select
import shutil
import signal
import subprocess
import sys
import tempfile
import time
import unittest
from pathlib import Path
from unittest import mock

TOOLS = Path(__file__).resolve().parents[1] / "tools"
sys.path.insert(0, str(TOOLS))
import owned_process as O  # noqa: E402
import permute as P  # noqa: E402
import permuter_engine as E  # noqa: E402
import pinned_tree as T  # noqa: E402
import source_policy as S  # noqa: E402

TMP = os.environ.get("TMPDIR")
SEED = """#include "common.h"
typedef struct { s32 x, y; } Pos;
void *func_X(Pos *out, Pos *in) {
    Pos t;
    for (;;) { if (in) break; }
    t.x = in->x;
    t.y = in->y;
    out->x = t.x;
    out->y = t.y;
    return out;
}
"""


def fn(text: str) -> str:
    s, e = P.function_span(text, "func_X")
    return text[s:e]


class LexicalPolicy(unittest.TestCase):
    def added(self, after: str, before: str = "out->x = t.x;\n    out->y = t.y;") -> list[str]:
        return S.forbidden_delta(S.lexical_counts(fn(SEED)), S.lexical_counts(fn(SEED.replace(before, after))))

    def test_clean_rewrite_is_allowed(self):
        self.assertEqual(self.added("*out = t;"), [])

    def test_each_forbidden_artifact_is_rejected(self):
        cases = {
            "asm": "__asm__(\"nop\"); *out = t;",
            "token_paste": "*out = t; /* */ X##Y;",
            "volatile": "{ volatile s32 v = t.x; out->x = v; out->y = t.y; }",
            "register": "{ register s32 v = t.x; out->x = v; out->y = t.y; }",
            "constant_condition": "if (1) { *out = t; }",
            "do_while_constant": "do { *out = t; } while (0);",
            "identical_branches": "if (in) { *out = t; } else { *out = t; }",
            "self_assignment": "*out = t; t.x = t.x; out = out;",
            "comma_dummy": "*out = (0, t);",
            "xor_or_mult_zero": "out->x = t.x ^ 0; out->y = t.y;",
            "pragma_attribute": "__attribute__((unused)) s32 z; *out = t;",
            "empty_statement": "*out = t;;",
        }
        for name, text in cases.items():
            with self.subTest(name):
                self.assertIn(name, self.added(text))

    def test_equivalent_constant_spellings_are_rejected(self):
        """SL-ASTRA-03: 0U, 1-1 and friends are the same dummy wrapper as 0."""
        for condition in ("0U", "1-1", "(0)", "'\\0'", "0x0L", "(int)0", "!1", "sizeof(int) - 4"):
            with self.subTest(condition):
                self.assertIn("do_while_constant", self.added(f"do {{ *out = t; }} while ({condition});"))
        self.assertIn("constant_loop", self.added("while (1) { *out = t; break; }"))
        self.assertIn("constant_loop", self.added("for (;;) { *out = t; break; }"))
        self.assertIn("constant_condition", self.added("switch (0) { default: *out = t; }"))

    def test_gnu_qualifier_spellings_count_as_volatile(self):
        for qualifier in ("__volatile__", "__volatile"):
            with self.subTest(qualifier):
                self.assertIn("volatile", self.added(f"{{ {qualifier} s32 v = t.x; out->x = v; out->y = t.y; }}"))

    def test_legitimate_loops_are_not_banned(self):
        for loop in ("while (in->x--) { *out = t; }", "do { *out = t; } while (--in->x);",
                     "for (t.x = 0; t.x < 4; t.x++) { *out = t; }"):
            with self.subTest(loop):
                self.assertEqual(self.added(loop), [])

    def test_preexisting_for_ever_is_not_a_new_artifact(self):
        self.assertEqual(self.added("t.x = in->x;", before="t.x = in->x;"), [])

    def test_asm_is_rejected_even_if_preexisting(self):
        seeded = S.lexical_counts(fn(SEED.replace("return out;", "__asm__(\"\"); return out;")))
        self.assertIn("asm", S.forbidden_delta(seeded, seeded))

    def test_seed_artifacts_reported_but_legit_loops_are_not(self):
        seeded = fn(SEED.replace("t.x = in->x;", "do { t.x = in->x; } while (0U);"))
        self.assertEqual(S.seed_artifacts(S.lexical_counts(seeded)), {"do_while_constant": 1})
        self.assertEqual(S.seed_artifacts(S.lexical_counts(fn(SEED))), {})

    def test_hex_multiplier_is_not_mult_zero(self):
        self.assertNotIn("xor_or_mult_zero", self.added("out->x = t.x * 0x10; out->y = t.y;"))


TU = """typedef unsigned char u8;
typedef int s32;
enum { FALSE, TRUE };
typedef struct { s32 x, y; } Pos;
s32 g(s32 a, s32 b);
void *func_X(Pos *out, Pos *in) {
    Pos t;
    t.x = in->x;
    t.y = in->y;
    BODY
    return out;
}
"""


VTU = """typedef int s32;
typedef volatile s32 VS32;
typedef VS32 VS32Alias;
typedef volatile s32 *VS32Ptr;
typedef struct { volatile s32 v; } VBox;
struct Reg { s32 a; struct Inner *inner; };
struct Inner { volatile s32 b; };
typedef struct { s32 x, y; } Pos;
extern volatile s32 hw_status;
extern s32 plain;
void *func_X(Pos *out, Pos *in) {
    s32 t;
    t = in->x;
    BODY
    return out;
}
"""


class SyntacticPolicy(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        _, blobs = P.permuter_identity(P.DEFAULT_PERMUTER.resolve())
        cls.parser = P.Parser(P.DEFAULT_PERMUTER.resolve(), blobs)

    def added(self, body: str, *, casts: bool = False) -> list[str]:
        seed = self.parser.counts(TU.replace("BODY", "out->x = t.x; out->y = t.y;"), "func_X", count_casts=casts)
        cand = self.parser.counts(TU.replace("BODY", body), "func_X", count_casts=casts)
        return S.forbidden_delta(seed, cand)

    def volatile_added(self, body: str, seed: str = "out->x = t; out->y = in->y;") -> list[str]:
        before = self.parser.counts(VTU.replace("BODY", seed), "func_X", count_casts=False)
        after = self.parser.counts(VTU.replace("BODY", body), "func_X", count_casts=False)
        return S.forbidden_delta(before, after)

    def test_constant_forms_hidden_from_lexing(self):
        cases = {
            "typedef cast": ("do { out->x = t.x; } while ((u8)0); out->y = t.y;", "do_while_constant"),
            "enumerator": ("do { out->x = t.x; } while (FALSE); out->y = t.y;", "do_while_constant"),
            "enumerator if": ("if (TRUE) { out->x = t.x; } out->y = t.y;", "constant_condition"),
            "ternary": ("out->x = 1 ? t.x : t.y; out->y = t.y;", "constant_condition"),
            "for without condition": ("for (;;) { out->x = t.x; break; } out->y = t.y;", "constant_loop"),
            "volatile cast": ("out->x = *(volatile s32 *)&t.x; out->y = t.y;", "volatile"),
            "gnu volatile": ("{ __volatile__ s32 v = t.x; out->x = v; } out->y = t.y;", "volatile"),
            "comma": ("out->x = (t.y, t.x); out->y = t.y;", "comma_expression"),
            "attribute": ("{ s32 z __attribute__((unused)) = t.x; out->x = z; } out->y = t.y;", "pragma_attribute"),
            "asm statement": ('__asm__("nop"); out->x = t.x; out->y = t.y;', "asm"),
        }
        for name, (body, key) in cases.items():
            with self.subTest(name):
                self.assertIn(key, self.added(body))

    def test_calls_and_real_loops_are_clean(self):
        for body in ("out->x = g(t.x, t.y); out->y = t.y;",
                     "while (in->x--) { out->x = t.x; } out->y = t.y;",
                     "do { out->x = t.x; } while (--in->y); out->y = t.y;"):
            with self.subTest(body):
                self.assertEqual(self.added(body), [])

    def test_new_casts_count_for_permuter_only(self):
        body = "out->x = (s32)t.x; out->y = t.y;"
        self.assertEqual(self.added(body), [])
        self.assertIn("cast", self.added(body, casts=True))

    def test_unparsable_source_raises(self):
        with self.assertRaises(Exception):
            self.parser.counts(TU.replace("BODY", "out->x = ;"), "func_X", count_casts=False)

    def test_volatile_reached_through_typedefs_tags_and_objects(self):
        """SL-R2-ASTRA-03: a qualifier carried by a declaration outside the function still counts."""
        cases = {
            "typedef cast (review control)": "*((VS32 *)&out->x) = t; out->y = in->y;",
            "typedef chain": "*((VS32Alias *)&out->x) = t; out->y = in->y;",
            "typedef of a pointer to volatile": "*(VS32Ptr)&out->x = t; out->y = in->y;",
            "struct member": "((VBox *)&out->x)->v = t; out->y = in->y;",
            "member of a tag defined later": "out->x = ((struct Reg *)in)->inner->b; out->y = in->y;",
            "object declared outside": "out->x = hw_status; out->y = in->y;",
            "local declared through a typedef": "{ VS32 v = t; out->x = v; } out->y = in->y;",
        }
        for name, body in cases.items():
            with self.subTest(name):
                self.assertIn("volatile", self.volatile_added(body))

    def test_keyword_like_typedefs_keep_volatile_through_derived_types(self):
        for alias in ("int_", "_int", "__builtin_va_list"):
            tu = VTU.replace("typedef int s32;", f"""typedef int s32;
typedef volatile s32 {alias};
typedef {alias} AliasChain;
typedef {alias} *AliasPointer;
typedef struct {{ {alias} value; }} AliasBox;""")
            before = self.parser.counts(tu.replace("BODY", "out->x = t; out->y = in->y;"),
                                        "func_X", count_casts=False)
            for body in (f"*(({alias} *)&out->x) = t;",
                         "*((AliasChain *)&out->x) = t;",
                         "*(AliasPointer)&out->x = t;",
                         "((AliasBox *)&out->x)->value = t;"):
                with self.subTest(alias=alias, body=body):
                    after = self.parser.counts(tu.replace("BODY", body + " out->y = in->y;"),
                                               "func_X", count_casts=False)
                    added = S.forbidden_delta(before, after)
                    self.assertIn("volatile", added)
                    self.assertNotIn("volatile_unverifiable", added)

    def test_keyword_like_alias_seed_accounting_and_block_shadow(self):
        tu = VTU.replace("typedef int s32;", """typedef int s32;
typedef volatile s32 int_;
extern int_ suffix_status;""")
        def counts(body):
            return self.parser.counts(tu.replace("BODY", body), "func_X", count_casts=False)
        seed = counts("out->x = suffix_status; out->y = in->y;")
        self.assertEqual(S.seed_artifacts(seed), {"volatile": 1})
        self.assertEqual(S.forbidden_delta(seed, counts("out->y = in->y; out->x = suffix_status;")), [])
        self.assertIn("volatile", S.forbidden_delta(seed, counts("out->x = suffix_status; out->y = suffix_status;")))
        clean = counts("out->x = t; out->y = in->y;")
        shadow = counts("{ typedef s32 int_; *((int_ *)&out->x) = t; } out->y = in->y;")
        self.assertEqual(S.forbidden_delta(clean, shadow), [])

    def test_gnu_signed_keyword_spellings_remain_resolvable(self):
        for word in ("__signed", "__signed__"):
            with self.subTest(word=word):
                self.assertEqual(self.added(f"{{ {word} int value = t.x; out->x = value; }} out->y = t.y;"), [])

    def test_scoping_and_unchanged_seed_uses_are_not_rejected(self):
        clean = {
            "block typedef shadows the volatile one": "{ typedef s32 VS32; *((VS32 *)&out->x) = t; } out->y = in->y;",
            "local object shadows the volatile one": "{ s32 hw_status = t; out->x = hw_status; } out->y = in->y;",
            "plain object": "out->x = plain; out->y = in->y;",
            "implicitly declared call": "out->x = undeclared_call(t); out->y = in->y;",
        }
        for name, body in clean.items():
            with self.subTest(name):
                self.assertEqual(self.volatile_added(body), [])
        seed = "out->x = hw_status; out->y = in->y;"
        self.assertEqual(self.volatile_added("out->y = in->y; out->x = hw_status;", seed=seed), [])

    def test_more_or_different_volatile_uses_than_the_seed_are_rejected(self):
        seed = "out->x = hw_status; out->y = in->y;"
        self.assertIn("volatile", self.volatile_added("out->x = hw_status; out->y = hw_status;", seed=seed))
        swapped = self.volatile_added("out->x = *(VS32 *)&in->x; out->y = in->y;", seed=seed)
        self.assertNotIn("volatile", swapped)  # same number of uses...
        self.assertTrue(any(k.startswith("volatile[type ") for k in swapped), swapped)  # ...but a new one

    def test_unresolvable_uses_fail_closed(self):
        for body in ("__typeof__(t) z = t; out->x = z; out->y = in->y;",
                     "out->x = undeclared_object; out->y = in->y;"):
            with self.subTest(body):
                self.assertIn("volatile_unverifiable", self.volatile_added(body))


class Edits(unittest.TestCase):
    def test_edit_inside_function_only_and_unique(self):
        text = SEED + "\nstatic int g(void) { int t; t = 1; return t; }\n"
        new = P.apply_edits(text, "func_X", [{"find": "t.x = in->x;", "replace": "t = *in;"}])
        self.assertIn("t = *in;", new)
        self.assertIn("static int g(void) { int t; t = 1; return t; }", new)
        with self.assertRaisesRegex(ValueError, "occurs 0 times"):
            P.apply_edits(text, "func_X", [{"find": "t = 1;", "replace": "t = 2;"}])
        with self.assertRaisesRegex(ValueError, "occurs 2 times"):
            P.apply_edits(text, "func_X", [{"find": "in->", "replace": "(*in)."}])


class Hypotheses(unittest.TestCase):
    def write(self, document) -> Path:
        handle = tempfile.NamedTemporaryFile("w", suffix=".json", delete=False, dir=TMP)
        json.dump(document, handle)
        handle.close()
        return Path(handle.name)

    def good(self, **change):
        h = {"id": "a", "class": "lifetime", "mechanism": "m", "semantics": "s",
             "edits": [{"find": "x", "replace": "y"}]}
        h.update(change)
        return {"function": "func_X", "hypotheses": [h]}

    def test_valid(self):
        self.assertEqual(len(P.load_hypotheses(self.write(self.good()), "func_X")), 1)

    def test_rejections(self):
        bad = {
            "wrong function": ({"function": "func_Y", "hypotheses": []}, "func_Y"),
            "class": (self.good(**{"class": "lottery"}), "class must be"),
            "mechanism": (self.good(mechanism=" "), "required"),
            "semantics": (self.good(semantics=""), "required"),
            "no edits": (self.good(edits=[]), "required"),
            "edit shape": (self.good(edits=[{"find": "x", "replace": "y", "extra": 1}]), "exactly"),
            "forbidden replacement": (self.good(edits=[{"find": "a = b;", "replace": "do { a = b; } while (0);"}]), "forbidden"),
            "unsigned dummy loop": (self.good(edits=[{"find": "a = b;", "replace": "do { a = b; } while (0U);"}]), "forbidden"),
            "gnu volatile": (self.good(edits=[{"find": "a = b;", "replace": "__volatile__ s32 p; a = b;"}]), "forbidden"),
        }
        for name, (doc, message) in bad.items():
            with self.subTest(name):
                with self.assertRaisesRegex(ValueError, message):
                    P.load_hypotheses(self.write(doc), "func_X")


class Ranking(unittest.TestCase):
    def test_status_then_words_then_size(self):
        rows = [{"status": "policy", "words": 1, "text_bytes": 132, "target_bytes": 128},
                {"status": "mismatch", "words": 9, "text_bytes": 128, "target_bytes": 128},
                {"status": "mismatch", "words": 9, "text_bytes": 124, "target_bytes": 128},
                {"status": "match", "words": 0, "text_bytes": 128, "target_bytes": 128},
                {"status": "error", "words": None}]
        ordered = sorted(rows, key=P.rank)
        self.assertEqual([r["status"] for r in ordered], ["match", "mismatch", "mismatch", "policy", "error"])
        self.assertEqual(ordered[1]["text_bytes"], 128)

    def test_pass_classes_exclude_forbidden_passes(self):
        enabled = {p for ps in P.PASS_CLASSES.values() for p in ps}
        for forbidden in ("perm_ins_block", "perm_var_cond_block", "perm_refer_to_var", "perm_add_mask",
                          "perm_xor_zero", "perm_mult_zero", "perm_dummy_comma_expr", "perm_add_self_assignment",
                          "perm_duplicate_assignment", "perm_pad_var_decl", "perm_empty_stmt", "perm_inline",
                          "perm_remove_ast", "perm_cast_simple", "perm_randomize_external_type",
                          "perm_randomize_function_type", "perm_randomize_internal_type", "perm_alias_array"):
            self.assertNotIn(forbidden, enabled)


class Trouble(unittest.TestCase):
    def test_permuter_best_is_labelled_unverified_and_rejections_shown(self):
        base = {"stage": "baseline", "status": "mismatch", "words": 10, "text_bytes": 128, "target_bytes": 128,
                "classification": "mismatch", "source_sha256": "a" * 64, "forbidden": []}
        perm = {**base, "stage": "permuter", "words": 8, "passes": ["perm_reorder_stmts"], "source_sha256": "b" * 64}
        dummy = {**base, "stage": "beam", "chain": ["dummy"], "words": 3, "classification": "rejected-forbidden",
                 "forbidden": ["do_while_constant"]}
        ctx = {"function": "func_X", "profile": "p", "ledger": [base, perm, dummy]}
        text = P.trouble(ctx, base, perm, base, None, [], None)
        self.assertIn("UNVERIFIED semantics", text)
        self.assertIn("stated (not proven) semantic argument that passed the source policy (baseline/beam): mismatch 10 words", text)
        self.assertIn("| permuter | perm_reorder_stmts | mismatch | mismatch | 8 | 128/128 | unverified | - |", text)
        self.assertIn("| beam | dummy | rejected-forbidden | mismatch | 3 | 128/128 | rejected | do_while_constant |", text)


class PinnedCheckout(unittest.TestCase):
    def test_real_checkout_verifies(self):
        result, blobs = P.permuter_identity(P.DEFAULT_PERMUTER.resolve())
        self.assertEqual(result["commit"], P.PERMUTER_COMMIT)
        self.assertEqual(result["git_tree"], "0ce3ec148f4a40f28a1f401a733992f613c5448c")
        self.assertEqual(result["tracked_files"], 255)
        self.assertEqual(blobs["src/randomizer.py"], (P.DEFAULT_PERMUTER.resolve() / "src/randomizer.py").read_bytes())

    def test_modified_missing_and_untracked_files_are_rejected(self):
        """SL-ASTRA-02: changed contents fail even though HEAD still names the pinned commit."""
        with tempfile.TemporaryDirectory(dir=TMP) as tmp:
            copy = Path(tmp) / "checkout"
            shutil.copytree(P.DEFAULT_PERMUTER.resolve(), copy, symlinks=True)
            target = copy / "src/randomizer.py"
            original = target.read_bytes()
            target.write_bytes(original + b"# reviewer edit\n")
            with self.assertRaisesRegex(ValueError, "changed src/randomizer.py"):
                P.permuter_identity(copy)
            target.write_bytes(original)
            (copy / "src/sitecustomize.py").write_text("")
            with self.assertRaisesRegex(ValueError, "untracked src/sitecustomize.py"):
                P.permuter_identity(copy)
            (copy / "src/sitecustomize.py").unlink()
            (copy / "src/__pycache__").mkdir(exist_ok=True)
            (copy / "src/__pycache__/x.cpython-312.pyc").write_bytes(b"inert")
            self.assertGreaterEqual(P.permuter_identity(copy)[0]["inert_bytecode_cache_files"], 1)
            (copy / "default_weights.toml").unlink()
            with self.assertRaisesRegex(ValueError, "missing default_weights.toml"):
                P.permuter_identity(copy)

    def test_fake_checkout_without_objects_is_rejected(self):
        with tempfile.TemporaryDirectory(dir=TMP) as tmp:
            root = Path(tmp)
            (root / ".git/refs/heads").mkdir(parents=True)
            (root / ".git/HEAD").write_text("ref: refs/heads/main\n")
            (root / ".git/refs/heads/main").write_text(P.PERMUTER_COMMIT + "\n")
            with self.assertRaisesRegex(ValueError, "not the pinned checkout"):
                P.permuter_identity(root)

    def test_toml_manifest_rejects_changes(self):
        with tempfile.TemporaryDirectory(dir=TMP) as tmp:
            package = Path(tmp) / "toml"
            package.mkdir()
            for name in T.TOML_FILES:
                (package / name).write_text("")
            with self.assertRaisesRegex(T.PinError, "differs from the pinned sha256"):
                T.verify_toml(package)
            (package / "extra.py").write_text("")
            with self.assertRaisesRegex(T.PinError, "differ from the pinned set"):
                T.verify_toml(package)


def planted_pyc(source: Path, marker: Path, prefix: Path | None) -> Path:
    """A timestamp-valid bytecode cache for `source` (PEP 552 layout) whose code also writes `marker`,
    where CPython's own loader looks for it: `__pycache__`, or under `prefix` (PYTHONPYCACHEPREFIX)."""
    original = source.read_bytes()
    payload = original.decode() + f"\nfrom pathlib import Path as _P\n_P({str(marker)!r}).write_text('UNVERIFIED BYTECODE RAN')\n"
    header = (importlib.util.MAGIC_NUMBER + (0).to_bytes(4, "little")
              + (int(source.stat().st_mtime) & 0xFFFFFFFF).to_bytes(4, "little") + (len(original) & 0xFFFFFFFF).to_bytes(4, "little"))
    previous = sys.pycache_prefix
    sys.pycache_prefix = str(prefix) if prefix else None
    try:
        cache = Path(importlib.util.cache_from_source(str(source)))
    finally:
        sys.pycache_prefix = previous
    cache.parent.mkdir(parents=True, exist_ok=True)
    cache.write_bytes(header + marshal.dumps(compile(payload, str(source), "exec", dont_inherit=True)))
    return cache


VERIFIED_CHILD = (
    "import json, sys\n"
    "from pathlib import Path\n"
    "sys.path.insert(0, sys.argv[1])\n"
    "import permute as P\n"
    "root = Path(sys.argv[2])\n"
    "ident, blobs = P.permuter_identity(root)\n"
    "parser = P.Parser(root, blobs)\n"
    "counts = parser.counts(sys.argv[3], 'func_X', count_casts=False)\n"
    "import perm_pycparser.c_ast as c_ast, src.ast_util as ast_util\n"
    "print(json.dumps({'loaders': [type(m.__loader__).__name__ for m in (c_ast, ast_util)],"
    " 'served': parser.served, 'volatile': counts['volatile']}))\n")


class VerifiedImports(unittest.TestCase):
    """SL-R2-ASTRA-02: the bytes that run are the bytes that were verified; no cache is executed."""

    def test_planted_bytecode_caches_never_run(self):
        with tempfile.TemporaryDirectory(dir=TMP) as tmp:
            tmp = Path(tmp)
            copy = tmp / "checkout"
            shutil.copytree(P.DEFAULT_PERMUTER.resolve(), copy, symlinks=True)
            prefix = tmp / "prefix"
            markers = {}
            for module in ("perm_pycparser/c_ast.py", "src/ast_util.py"):
                for where in (None, prefix):
                    marker = tmp / f"ran-{module.replace('/', '_')}-{'prefix' if where else 'pycache'}"
                    planted_pyc(copy / module, marker, where)
                    markers[(module, where)] = marker
            base_env = {**os.environ, "PYTHONDONTWRITEBYTECODE": "1"}
            for env in (base_env, {**base_env, "PYTHONPYCACHEPREFIX": str(prefix)}):
                with self.subTest(prefix="PYTHONPYCACHEPREFIX" in env):
                    run = subprocess.run([sys.executable, "-c", VERIFIED_CHILD, str(TOOLS), str(copy),
                                          VTU.replace("BODY", "out->x = hw_status;")],
                                         env=env, capture_output=True, text=True, timeout=120)
                    self.assertEqual(run.returncode, 0, run.stderr[-2000:])
                    result = json.loads(run.stdout.splitlines()[-1])
                    self.assertEqual(result["loaders"], ["VerifiedImporter", "VerifiedImporter"])
                    self.assertEqual(result["served"]["perm_pycparser.c_ast"],
                                     hashlib.sha256((copy / "perm_pycparser/c_ast.py").read_bytes()).hexdigest())
                    self.assertEqual(result["volatile"], 1)
                    self.assertEqual([m.name for m in markers.values() if m.exists()], [])
            # Control: the plants are live for CPython's own loader, in both locations.
            plain = "import sys; sys.path.insert(0, sys.argv[1]); import perm_pycparser.c_ast"
            for env, where in ((base_env, None), ({**base_env, "PYTHONPYCACHEPREFIX": str(prefix)}, prefix)):
                subprocess.run([sys.executable, "-c", plain, str(copy)], env=env, check=True, timeout=120)
                self.assertTrue(markers[("perm_pycparser/c_ast.py", where)].exists())

    def test_unverified_toml_never_runs(self):
        with tempfile.TemporaryDirectory(dir=TMP) as tmp:
            site = Path(tmp) / "site"
            (site / "toml").mkdir(parents=True)
            marker = Path(tmp) / "toml-ran"
            (site / "toml/__init__.py").write_text(f"from pathlib import Path\nPath({str(marker)!r}).write_text('ran')\n")
            sys.path.insert(0, str(site))
            try:
                located = T.locate_toml()
                with self.assertRaisesRegex(T.PinError, "differ from the pinned set"):
                    T.verify_toml(located)
            finally:
                sys.path.remove(str(site))
                sys.path_importer_cache.pop(str(site), None)
            self.assertEqual(located, (site / "toml").resolve())
            self.assertFalse(marker.exists())
            self.assertNotIn("toml", sys.modules)

    def test_importer_serves_only_verified_bytes(self):
        child = ("import json, sys\n"
                 "from pathlib import Path\n"
                 "sys.path.insert(0, sys.argv[1]); sys.path.insert(0, sys.argv[2])\n"
                 "import pinned_tree as T\n"
                 "T.install_verified({'vpkg': (Path(sys.argv[2]), {'vpkg/__init__.py': b'', 'vpkg/a.py': b'X = 1\\n'})})\n"
                 "import vpkg.a\n"
                 "try:\n    import vpkg.b\n    b = 'imported'\nexcept ModuleNotFoundError:\n    b = 'refused'\n"
                 "print(json.dumps({'a': vpkg.a.X, 'b': b}))\n")
        with tempfile.TemporaryDirectory(dir=TMP) as tmp:
            (Path(tmp) / "vpkg").mkdir()
            (Path(tmp) / "vpkg/__init__.py").write_text("")
            (Path(tmp) / "vpkg/a.py").write_text("X = 2\n")  # changed on disk after verification
            (Path(tmp) / "vpkg/b.py").write_text("X = 3\n")  # never verified
            run = subprocess.run([sys.executable, "-B", "-c", child, str(TOOLS), tmp], capture_output=True, text=True,
                                 timeout=60)
        self.assertEqual(run.returncode, 0, run.stderr[-2000:])
        self.assertEqual(json.loads(run.stdout), {"a": 1, "b": "refused"})


def write_events(path: Path, events: list[dict], tail: str = "") -> Path:
    path.write_text("".join(json.dumps({"seq": i, **e}) + "\n" for i, e in enumerate(events)) + tail)
    return path


def generation(index: int, **fields) -> list[dict]:
    return [{"event": "generation-started", "index": index, "rng_seed": 100 + index},
            {"event": "generated", "index": index, "rng_seed": 100 + index, **fields}]


class EngineProgress(unittest.TestCase):
    """SL-R2-ASTRA-04: every candidate index is terminal, incomplete or never started; nothing is relabelled."""

    def test_generation_in_flight_is_incomplete_not_unattempted(self):
        events = [{"event": "start"}, *generation(0, function_sha256="a"), *generation(1, outcome="duplicate"),
                  *generation(2, outcome="rejected-forbidden"),
                  {"event": "generation-started", "index": 3, "rng_seed": 103}]
        with tempfile.TemporaryDirectory(dir=TMP) as tmp:
            progress = P.engine_progress(write_events(Path(tmp) / "events.jsonl", events), 10)
        self.assertFalse(progress["complete"])
        self.assertEqual((progress["generation_started"], progress["generated"]), (4, 3))
        self.assertEqual(progress["incomplete_generation"], [{"index": 3, "rng_seed": 103}])
        self.assertEqual(progress["generation_not_started"], 6)
        self.assertEqual(progress["outcomes"], {"duplicate": 1, "rejected-forbidden": 1})
        self.assertEqual(progress["incomplete_evaluation_not_started"], [0])

    def test_evaluation_in_flight_and_torn_final_line(self):
        events = [{"event": "start"}, *generation(0, function_sha256="a"), *generation(1, function_sha256="b"),
                  {"event": "evaluation-started", "index": 0},
                  {"event": "evaluated", "index": 0, "outcome": "scored", "score": 5},
                  {"event": "evaluation-started", "index": 1}]
        with tempfile.TemporaryDirectory(dir=TMP) as tmp:
            progress = P.engine_progress(write_events(Path(tmp) / "events.jsonl", events, '{"seq": 8, "event": "evalu'), 2)
        self.assertEqual(progress["outcomes"], {"scored": 1})
        self.assertEqual(progress["incomplete_evaluation_started"], [1])
        self.assertEqual(progress["generation_not_started"], 0)
        self.assertEqual(progress["torn_final_line"], '{"seq": 8, "event": "evalu')

    def test_damaged_history_is_corruption_not_progress(self):
        cases = {
            "seq gap": json.dumps({"seq": 0, "event": "start"}) + "\n" + json.dumps({"seq": 5, "event": "start"}) + "\n",
            "generated without its start": json.dumps({"seq": 0, "event": "generated", "index": 0, "rng_seed": 100}) + "\n",
            "generation out of order": json.dumps({"seq": 0, "event": "generation-started", "index": 1, "rng_seed": 101}) + "\n",
            "damaged middle line": '{"seq": 0, "ev\n' + json.dumps({"seq": 1, "event": "start"}) + "\n",
        }
        for name, text in cases.items():
            with self.subTest(name), tempfile.TemporaryDirectory(dir=TMP) as tmp:
                path = Path(tmp) / "events.jsonl"
                path.write_text(text)
                with self.assertRaises(ValueError):
                    P.engine_progress(path, 2)

    def test_engine_writer_and_parent_reader_agree(self):
        with tempfile.TemporaryDirectory(dir=TMP) as tmp:
            path = Path(tmp) / "events.jsonl"
            log = E.EventLog(path)
            log.emit("start", candidates=1)
            log.emit("generation-started", index=0, rng_seed=7)
            progress = P.engine_progress(path, 1)
            with self.assertRaises(FileExistsError):  # a run never appends to another run's events
                E.EventLog(path)
        self.assertEqual(progress["incomplete_generation"], [{"index": 0, "rng_seed": 7}])


class AttemptLedger(unittest.TestCase):
    """SL-R2-ASTRA-04: attempts are registered before their work; interrupted ones stay incomplete."""

    def test_interrupted_and_cut_off_attempts_are_incomplete_with_identity_and_results(self):
        with tempfile.TemporaryDirectory(dir=TMP) as tmp:
            tmp = Path(tmp)
            ledger = P.Ledger(tmp / "ledger.jsonl")
            with P.attempt_scope(ledger, "baseline", out=str(tmp / "baseline")) as number:
                ledger.complete(number, {"stage": "baseline", "status": "mismatch"})
            out = tmp / "beam/d1-h"
            with self.assertRaises(KeyboardInterrupt):
                with P.attempt_scope(ledger, "beam", depth=1, chain=["h"], out=str(out)):
                    out.mkdir(parents=True)
                    (out / "result.json").write_text("{}")  # the check finished; its row was never committed
                    raise KeyboardInterrupt("interrupted by SIGINT")
            ledger.start("beam", depth=1, chain=["g"], out=str(tmp / "beam/d1-g"))  # then a hard kill: no terminal line
            progress = P.ledger_progress(tmp / "ledger.jsonl")
            with self.assertRaises(FileExistsError):
                P.Ledger(tmp / "ledger.jsonl")
        self.assertEqual((progress["attempts_started"], progress["completed"]), (3, 1))
        interrupted, cut_off = progress["incomplete"]
        self.assertEqual((interrupted["attempt"], interrupted["identity"]["chain"], interrupted["result_json_present"]),
                         (1, ["h"], True))
        self.assertIn("KeyboardInterrupt", interrupted["aborted"])
        self.assertEqual((cut_off["attempt"], cut_off["aborted"], cut_off["result_json_present"]), (2, None, False))
        self.assertEqual(progress["by_stage"]["beam"], {"started": 2, "completed": 0, "incomplete": 2})
        self.assertIsNone(progress["torn_final_line"])


DESCENDANT = "import time; time.sleep(30)"


def leader(exit_code: int | None) -> str:
    tail = f"sys.exit({exit_code})" if exit_code is not None else "time.sleep(30)"
    return ("import subprocess, sys, time\n"
            f"subprocess.Popen([sys.executable, '-c', {DESCENDANT!r}], pass_fds=(int(sys.argv[1]),))\n" + tail)


def eof_within(fd: int, seconds: float) -> bool:
    deadline = time.monotonic() + seconds
    while time.monotonic() < deadline:
        ready, _, _ = select.select([fd], [], [], max(0.0, deadline - time.monotonic()))
        if ready and os.read(fd, 1) == b"":
            return True
    return False


class OwnedProcess(unittest.TestCase):
    """SL-ASTRA-01: signal-before-reap retirement on every exit path; no unowned signals."""

    def run_leader(self, exit_code, timeout=20.0):
        read, write = os.pipe()
        try:
            with tempfile.TemporaryDirectory(dir=TMP) as tmp:
                calls = []
                real_killpg, real_wait = os.killpg, subprocess.Popen.wait

                def killpg(pgid, signum):
                    calls.append(("signal", pgid, signum))
                    return real_killpg(pgid, signum)

                def wait(process, *a, **k):
                    calls.append(("reap", process.pid, None))
                    return real_wait(process, *a, **k)

                with mock.patch.object(O.os, "killpg", killpg), mock.patch.object(subprocess.Popen, "wait", wait):
                    run = O.run_owned([sys.executable, "-c", leader(exit_code), str(write)], Path(tmp) / "log",
                                      cwd=Path(tmp), env=dict(os.environ), timeout=timeout, grace=0.5, pass_fds=(write,))
            os.close(write)
            write = None
            return run, calls, eof_within(read, 5.0)
        finally:
            os.close(read)
            if write is not None:
                os.close(write)

    def assert_signals_precede_reap(self, calls):
        reaps = [i for i, c in enumerate(calls) if c[0] == "reap"]
        signals = [i for i, c in enumerate(calls) if c[0] == "signal"]
        self.assertTrue(signals and reaps)
        self.assertLess(max(signals), min(reaps))

    def test_ordinary_exits_kill_same_group_descendants(self):
        for code in (0, 7):
            with self.subTest(exit_code=code):
                run, calls, dead = self.run_leader(code)
                self.assertEqual(run["exit_code"], code)
                self.assertFalse(run["timed_out"])
                self.assertEqual(run["group_signals"], ["SIGKILL"])
                self.assertEqual(run["signals_after_reap"], 0)
                self.assert_signals_precede_reap(calls)
                self.assertTrue(dead, "descendant outlived run_owned")

    def test_timeout_terminates_then_kills_before_reaping(self):
        run, calls, dead = self.run_leader(None, timeout=0.5)
        self.assertTrue(run["timed_out"])
        self.assertEqual(run["group_signals"], ["SIGTERM", "SIGKILL"])
        self.assert_signals_precede_reap(calls)
        self.assertTrue(dead)

    def test_guarded_child_stops_on_term_within_grace(self):
        """The spawn-time signal mask must not make the TERM grace period dead time."""
        child = (f"import sys, time; sys.path.insert(0, {str(TOOLS)!r}); import owned_process as O\n"
                 "O.guard_owner(int(sys.argv[1])); time.sleep(30)\n")
        with tempfile.TemporaryDirectory(dir=TMP) as tmp:
            run = O.run_owned([sys.executable, "-c", child, O.OWNER_FD], Path(tmp) / "log", cwd=Path(tmp),
                              env=dict(os.environ), timeout=1.0, grace=20.0)
        self.assertTrue(run["timed_out"])
        self.assertEqual(run["exit_code"], -signal.SIGTERM)
        self.assertLess(run["seconds"], 10.0)

    def test_retired_group_is_never_signalled_again(self):
        with tempfile.TemporaryDirectory(dir=TMP) as tmp, (Path(tmp) / "log").open("w") as log:
            owned = []
            group = O.launch([sys.executable, "-c", "pass"], log, owned, cwd=Path(tmp), env=dict(os.environ))
            group.retire(0.5)
            count = len(group.signals)
            group.retire(0.5)
            self.assertEqual(len(group.signals), count)
            with self.assertRaisesRegex(RuntimeError, "retired"):
                group._signal(signal.SIGKILL)

    def helper(self, write: int, tmp: str) -> subprocess.Popen:
        child = (f"import subprocess, sys, time; sys.path.insert(0, {str(TOOLS)!r}); import owned_process as O\n"
                 "O.guard_owner(int(sys.argv[1]))\n"
                 "w = int(sys.argv[2])\n"
                 f"subprocess.Popen([sys.executable, '-c', {DESCENDANT!r}], pass_fds=(w,))\n"
                 "os = __import__('os'); os.write(w, b'R'); time.sleep(30)\n")
        helper = (f"import os, sys; sys.path.insert(0, {str(TOOLS)!r}); import owned_process as O\n"
                  "from pathlib import Path\n"
                  f"O.run_owned([sys.executable, '-c', {child!r}, O.OWNER_FD, str({write})], Path({tmp!r}) / 'log',\n"
                  f"            cwd=Path({tmp!r}), env=dict(os.environ), timeout=60, grace=0.5, pass_fds=({write},))\n")
        return subprocess.Popen([sys.executable, "-c", helper], pass_fds=(write,), stderr=subprocess.DEVNULL)

    def owner_death(self, signum: int) -> tuple[bool, int]:
        read, write = os.pipe()
        with tempfile.TemporaryDirectory(dir=TMP) as tmp:
            process = self.helper(write, tmp)
            os.close(write)
            try:
                ready, _, _ = select.select([read], [], [], 20)
                self.assertTrue(ready and os.read(read, 1) == b"R", "owned child never started")
                os.kill(process.pid, signum)  # direct, unreaped child of this test
                dead = eof_within(read, 5.0)
            finally:
                code = process.wait(timeout=30)
                os.close(read)
        return dead, code

    def test_owner_sigkill_guard_kills_group(self):
        dead, code = self.owner_death(signal.SIGKILL)
        self.assertEqual(code, -signal.SIGKILL)
        self.assertTrue(dead, "guard did not kill the owned group after owner SIGKILL")

    def test_owner_sigterm_interrupt_retires_group(self):
        dead, code = self.owner_death(signal.SIGTERM)
        self.assertNotEqual(code, 0)
        self.assertTrue(dead, "interrupt did not retire the owned group")


if __name__ == "__main__":
    unittest.main()
