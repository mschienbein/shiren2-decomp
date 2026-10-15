"""Search-scope source restrictions for tools/permute.py (shared by the ladder and its engine).

These are restrictions of this measured search, not C legality rules. A do-once loop, a GNU
qualifier or a new cast can all be legal C. The ladder refuses to *add* them because they are
the artificial weighting and no-op devices the workflow forbids as a source of match credit.
Passing these checks proves nothing about semantics; independent semantic review still decides.

Two layers, both fail closed:

* Lexical (`lexical_counts`) runs on hypothesis text before anything compiles, and on every
  candidate function. A condition counts as constant when, after literals are removed, it
  contains no identifier other than type keywords or `sizeof`. That covers `0`, `0U`, `1-1`,
  `(0)`, `'\\0'`, `0x0L` and `(int)0`. GNU spellings `__volatile__`/`__volatile` count as
  volatile, and `__asm`/`__asm__` count as asm.
* Syntactic (`ast_counts`) runs on the pinned pycparser AST of the whole preprocessed
  translation unit, so macros are expanded. Here a condition is constant when its expression
  tree holds only constants, enumerators, `sizeof`, casts (including typedef casts such as
  `(u8)0`) and operators over those. It applies to if/switch/ternary conditions,
  while/do-while/for loop conditions (a missing `for` condition counts as constant), `register`,
  `#pragma`, `__attribute__`, inline asm, empty statements, self assignments, comma expressions
  (call argument lists are not comma expressions), and identical branches. New casts are
  counted for permuter candidates only.
* Volatile, syntactically resolved (`_VolatileUses`): every declaration, type name and object
  reference in the target function is resolved with C scope rules (block scoping and shadowing,
  one namespace for typedef names and ordinary identifiers, struct/union tags). A use counts as
  volatile when a `volatile` qualifier is reachable through its type: directly, through typedef
  chains, through derived pointer/array/function-return types, or through struct/union members.
  References count per use, so duplicating a read of a volatile object is an addition. A use
  whose type cannot be resolved (`typeof(expr)`, an unknown type name, an undeclared non-call
  identifier) counts as `volatile_unverifiable`: fail closed. Each use is also counted under its
  own key (`volatile[ref x]`, `volatile[type T *]`, ...), so a candidate cannot trade one seed use
  for a new one.

A candidate is rejected when any count rises over the seed. Unchanged seed code (including its
own volatile accesses) is never a rejection on its own. `asm` and `##` are rejected even when the
seed already has them.
"""
from __future__ import annotations

import re

_IDENT = r"[A-Za-z_]\w*"
_VALUELESS = {"int", "unsigned", "signed", "char", "short", "long", "void", "float", "double", "_Bool",
              "const", "sizeof"}
_LITERAL = re.compile(r"0[xX][0-9a-fA-F]+[uUlL]*|\d+(?:\.\d*)?(?:[eE][+-]?\d+)?[uUlLfF]*|'(?:\\.|[^'\\])+'")
_STRINGS = re.compile(r'"(?:\\.|[^"\\])*"')
LEXICAL_PATTERNS = {
    "asm": r"\b(?:__)?asm(?:__)?\b",
    "token_paste": r"##",
    "volatile": r"\b(?:__)?volatile(?:__)?\b",
    "register": r"\bregister\b",
    "pragma_attribute": r"#\s*pragma|\b_Pragma\b|__attribute__",
    "empty_statement": r"(?:^|[;{}])\s*;",
    "self_assignment": rf"(?<![\w.>])({_IDENT})\s*=\s*\1\s*;",
    "comma_dummy": r"\(\s*0\s*,",
    "xor_or_mult_zero": r"\^\s*0\b|\*\s*0\b(?!x)",
}
ALWAYS_FORBIDDEN = ("asm", "token_paste")
# Seed artifacts that make an exact result "inherits seed artifacts". Constant loops (`for (;;)`,
# `while (1)`) and comma expressions are legitimate in seeds and only count when a candidate adds them.
SEED_BLOCKING = ("asm", "token_paste", "volatile", "register", "pragma_attribute", "constant_condition",
                 "do_while_constant", "identical_branches", "comma_dummy", "xor_or_mult_zero", "self_assignment")


def strip_comments(text: str) -> str:
    return re.sub(r"/\*.*?\*/|//[^\n]*", " ", text, flags=re.S)


def _balanced(text: str, i: int, open_: str, close: str) -> int:
    depth = 0
    while i < len(text):
        depth += (text[i] == open_) - (text[i] == close)
        i += 1
        if depth == 0:
            return i
    return -1


def lexically_constant(expression: str) -> bool:
    """True when the expression has no identifier except type keywords or sizeof."""
    rest = _LITERAL.sub(" ", expression)
    return all(name in _VALUELESS for name in re.findall(_IDENT, rest))


def _conditions(code: str, keyword: str) -> list[tuple[str, int]]:
    found = []
    for match in re.finditer(rf"\b{keyword}\s*\(", code):
        end = _balanced(code, match.end() - 1, "(", ")")
        if end > 0:
            found.append((code[match.end():end - 1], end))
    return found


def identical_branches(text: str) -> int:
    """Count `if (c) A else A` (A a braced block or a single statement) with identical A."""
    count = 0
    for match in re.finditer(r"\bif\s*\(", text):
        cond_end = _balanced(text, match.end() - 1, "(", ")")
        if cond_end < 0:
            continue

        def branch(i: int) -> tuple[str, int]:
            j = len(text) - len(text[i:].lstrip())
            if j < len(text) and text[j] == "{":
                k = _balanced(text, j, "{", "}")
                return text[j:k], k
            k = text.find(";", j)
            return text[j:k + 1], k + 1

        then, after = branch(cond_end)
        rest = text[after:].lstrip()
        if not rest.startswith("else") or re.match(r"else\s+if\b", rest):
            continue
        other, _ = branch(after + (len(text[after:]) - len(rest)) + 4)
        if then and re.sub(r"\s+", "", then) == re.sub(r"\s+", "", other):
            count += 1
    return count


def lexical_counts(function_text: str) -> dict[str, int]:
    code = _STRINGS.sub('""', strip_comments(function_text))
    counts = {name: len(re.findall(pattern, code, re.M)) for name, pattern in LEXICAL_PATTERNS.items()}
    counts["constant_condition"] = sum(lexically_constant(c) for k in ("if", "switch") for c, _ in _conditions(code, k))
    loops = do_whiles = 0
    for condition, end in _conditions(code, "while"):
        if lexically_constant(condition):
            if code[end:].lstrip().startswith(";"):
                do_whiles += 1
            else:
                loops += 1
    for header, _ in _conditions(code, "for"):
        parts = header.split(";")
        if len(parts) == 3 and lexically_constant(parts[1]):
            loops += 1
    counts["constant_loop"], counts["do_while_constant"] = loops, do_whiles
    counts["identical_branches"] = identical_branches(code)
    return counts


def ast_counts(ca, to_c, ast, function: str, *, count_casts: bool) -> dict[str, int]:
    """Syntactic counts for the definition of `function` in a pycparser FileAST (`ca` is the pinned
    c_ast module). The whole translation unit is needed: typedefs, tags and objects declared
    outside the function decide whether a use inside it is volatile."""
    targets = [n for n in ast.ext if isinstance(n, ca.FuncDef) and n.decl.name == function]
    if len(targets) != 1:
        raise ValueError(f"{len(targets)} definitions of {function} in the parsed translation unit")
    enumerators = enumerator_names(ca, ast)
    counts = {"constant_condition": 0, "constant_loop": 0, "do_while_constant": 0, "register": 0,
              "pragma_attribute": 0, "asm": 0, "empty_statement": 0, "self_assignment": 0,
              "comma_expression": 0, "identical_branches": 0, "cast": 0}

    def constant(node) -> bool:
        if node is None:
            return True
        if isinstance(node, ca.Constant):
            return True
        if isinstance(node, ca.ID):
            return node.name in enumerators
        if isinstance(node, ca.UnaryOp):
            return node.op == "sizeof" or (node.op in {"-", "+", "!", "~"} and constant(node.expr))
        if isinstance(node, ca.BinaryOp):
            return constant(node.left) and constant(node.right)
        if isinstance(node, ca.TernaryOp):
            return constant(node.cond) and constant(node.iftrue) and constant(node.iffalse)
        if isinstance(node, ca.Cast):
            return constant(node.expr)
        if isinstance(node, ca.ExprList):
            return all(constant(e) for e in node.exprs)
        return isinstance(node, ca.Typename)

    class Visitor(ca.NodeVisitor):
        def generic_visit(self, node):
            if "register" in (getattr(node, "storage", None) or []):
                counts["register"] += 1
            for _, child in node.children():
                self.visit(child)

        def visit_If(self, n):
            counts["constant_condition"] += constant(n.cond)
            if n.iftrue is not None and n.iffalse is not None and to_c(n.iftrue) == to_c(n.iffalse):
                counts["identical_branches"] += 1
            self.generic_visit(n)

        def visit_Switch(self, n):
            counts["constant_condition"] += constant(n.cond)
            self.generic_visit(n)

        def visit_TernaryOp(self, n):
            counts["constant_condition"] += constant(n.cond)
            if to_c(n.iftrue) == to_c(n.iffalse):
                counts["identical_branches"] += 1
            self.generic_visit(n)

        def visit_While(self, n):
            counts["constant_loop"] += constant(n.cond)
            self.generic_visit(n)

        def visit_For(self, n):
            counts["constant_loop"] += constant(n.cond)
            self.generic_visit(n)

        def visit_DoWhile(self, n):
            counts["do_while_constant"] += constant(n.cond)
            self.generic_visit(n)

        def visit_EmptyStatement(self, n):
            counts["empty_statement"] += 1

        def visit_Pragma(self, n):
            counts["pragma_attribute"] += 1

        def visit_GccAttribute(self, n):
            counts["pragma_attribute"] += 1
            self.generic_visit(n)

        def visit_Asm(self, n):
            counts["asm"] += 1
            self.generic_visit(n)

        def visit_Assignment(self, n):
            if n.op == "=" and to_c(n.lvalue) == to_c(n.rvalue):
                counts["self_assignment"] += 1
            self.generic_visit(n)

        def visit_ExprList(self, n):
            counts["comma_expression"] += 1
            self.generic_visit(n)

        def visit_FuncCall(self, n):
            self.visit(n.name)
            if n.args is not None:  # an argument list is not a comma expression
                for arg in n.args.exprs:
                    self.visit(arg)

        def visit_Cast(self, n):
            counts["cast"] += count_casts
            self.generic_visit(n)

    Visitor().visit(targets[0])
    uses = _VolatileUses(ca, to_c, targets[0])
    uses.walk(ast, False)
    counts.update(uses.counts())
    return counts


_CLEAN = (False, frozenset(), False)  # (volatile reached directly, struct/union tags reached, unresolvable)
_UNKNOWN = (False, frozenset(), True)
_BUILTIN_TYPE_WORDS = {"void", "_Bool", "char", "short", "int", "long", "float", "double", "_Complex", "signed",
                       "unsigned", "__signed", "__signed__", "__int128", "__builtin_va_list"}


def _merge(*facts) -> tuple:
    return (any(f[0] for f in facts), frozenset().union(*(f[1] for f in facts)), any(f[2] for f in facts))


class _VolatileUses:
    """Volatile reachability of every use in one target function, resolved with C scope rules.

    A declaration is visible from its declarator to the end of its block, inner declarations shadow
    outer ones, typedef names and ordinary identifiers share one namespace, and function parameters
    belong to the body's outermost block. Struct/union tags are resolved over every definition of
    the tag in the translation unit (an over-approximation), after the walk, so forward references
    and member chains are covered. Only the return type of a function type is followed: parameter
    qualifiers are prototype scope and are not accesses in the caller."""

    def __init__(self, ca, to_c, target):
        self.ca, self.to_c, self.target = ca, to_c, target
        self.frames: list[dict] = [{}]
        self.tag_defs: dict[tuple[str, str], list[tuple]] = {}
        self.uses: list[tuple[str, tuple]] = []

    def lookup(self, name: str):
        for frame in reversed(self.frames):
            if name in frame:
                return frame[name]
        return None

    def type_fact(self, node, in_target: bool) -> tuple:
        ca = self.ca
        if node is None:
            return _CLEAN
        quals = [*(getattr(node, "quals", None) or []), *(getattr(node, "dim_quals", None) or [])]
        own = (any("volatile" in q for q in quals), frozenset(), False)
        if isinstance(node, (ca.Decl, ca.Typedef, ca.Typename, ca.TypeDecl, ca.PtrDecl)):
            return _merge(own, self.type_fact(node.type, in_target))
        if isinstance(node, ca.ArrayDecl):
            if in_target:
                self.walk(node.dim, True)
            return _merge(own, self.type_fact(node.type, in_target))
        if isinstance(node, ca.FuncDecl):
            return self.type_fact(node.type, in_target)
        if isinstance(node, ca.IdentifierType):
            facts = []
            for word in node.names:
                entry = self.lookup(word)
                if entry is not None:
                    facts.append(entry[1] if entry[0] == "typedef" else _UNKNOWN)
                elif word not in _BUILTIN_TYPE_WORDS:
                    # A user identifier such as int_ is not a spelling of the int keyword.
                    facts.append(_UNKNOWN)
            return _merge(_CLEAN, *facts)
        if isinstance(node, (ca.Struct, ca.Union)):
            key = (type(node).__name__, node.name)
            if node.decls is None:
                return (False, frozenset({key}), False) if node.name else _UNKNOWN
            members = _merge(_CLEAN, *(self.type_fact(d, in_target) if isinstance(d, ca.Decl) else _UNKNOWN
                                       for d in node.decls))
            if node.name is None:
                return members
            self.tag_defs.setdefault(key, []).append(members)
            return (False, frozenset({key}), False)
        if isinstance(node, ca.Enum):
            for enumerator in (node.values.enumerators if node.values is not None else []):
                if in_target:
                    self.walk(enumerator.value, True)
                self.frames[-1][enumerator.name] = ("enumerator", _CLEAN)
            return _CLEAN
        if isinstance(node, ca.Typeof):
            if isinstance(node.expr, ca.Typename):
                return self.type_fact(node.expr, in_target)
            if in_target:
                self.walk(node.expr, True)
            return _UNKNOWN  # the type of an expression is not resolved here: fail closed
        return _UNKNOWN

    def declaration(self, node, in_target: bool) -> None:
        fact = self.type_fact(node, in_target)
        kind = "typedef" if isinstance(node, self.ca.Typedef) or "typedef" in (node.storage or []) else "object"
        if node.name:
            self.frames[-1][node.name] = (kind, fact)
            if in_target:
                self.uses.append((f"{kind} {node.name}", fact))
        if isinstance(node, self.ca.Decl):
            self.walk(node.init, in_target)  # scope starts at the declarator, before its initializer

    def reference(self, name: str, *, call: bool) -> None:
        entry = self.lookup(name)
        if entry is None:
            if not call:  # an implicitly declared function returns int; any other unknown name fails closed
                self.uses.append((f"unresolved {name}", _UNKNOWN))
        elif entry[0] == "object":
            self.uses.append((f"ref {name}", entry[1]))
        elif entry[0] == "typedef":
            self.uses.append((f"unresolved {name}", _UNKNOWN))

    def walk(self, node, in_target: bool) -> None:
        ca = self.ca
        if node is None:
            return
        if isinstance(node, ca.FuncDef):
            inside = node is self.target
            name, ftype = node.decl.name, node.decl.type
            self.frames[-1][name] = ("object", self.type_fact(node.decl, False))
            self.frames.append({})
            params = ftype.args.params if isinstance(ftype, ca.FuncDecl) and ftype.args is not None else []
            for param in params:
                if isinstance(param, ca.Decl):
                    self.declaration(param, inside)
            for param in node.param_decls or []:
                self.declaration(param, inside)
            for param in params:  # K&R names without a declaration are implicitly int
                if isinstance(param, ca.ID) and param.name not in self.frames[-1]:
                    self.frames[-1][param.name] = ("object", _CLEAN)
            for item in node.body.block_items or []:
                self.walk(item, inside)
            self.frames.pop()
        elif isinstance(node, (ca.Decl, ca.Typedef)):
            self.declaration(node, in_target)
        elif isinstance(node, (ca.Compound, ca.For)):
            self.frames.append({})
            for _, child in node.children():
                self.walk(child, in_target)
            self.frames.pop()
        elif isinstance(node, ca.Typename):
            fact = self.type_fact(node, in_target)
            if in_target:
                self.uses.append((f"type {self.to_c(node)}", fact))
        elif isinstance(node, ca.ID):
            if in_target:
                self.reference(node.name, call=False)
        elif isinstance(node, ca.FuncCall):
            if isinstance(node.name, ca.ID):
                if in_target:
                    self.reference(node.name.name, call=True)
            else:
                self.walk(node.name, in_target)
            self.walk(node.args, in_target)
        elif isinstance(node, ca.StructRef):
            self.walk(node.name, in_target)  # the field names a member, not an ordinary identifier
        elif isinstance(node, ca.NamedInitializer):
            for designator in node.name:
                if not isinstance(designator, ca.ID):  # `[i]` designators are expressions; `.f` names a member
                    self.walk(designator, in_target)
            self.walk(node.expr, in_target)
        elif isinstance(node, (ca.Struct, ca.Union, ca.Enum)):
            self.type_fact(node, in_target)
        else:
            for _, child in node.children():
                self.walk(child, in_target)

    def counts(self) -> dict[str, int]:
        tags = {key: (False, False) for key in self.tag_defs}
        changed = True
        while changed:  # monotone fixpoint over (volatile, unresolvable) per tag
            changed = False
            for key, defs in self.tag_defs.items():
                value = (any(d or any(tags.get(t, (False, False))[0] for t in deps) for d, deps, _ in defs),
                         any(u or any(tags.get(t, (False, False))[1] for t in deps) for _, deps, u in defs))
                if value != tags[key]:
                    tags[key], changed = value, True
        counts = {"volatile": 0, "volatile_unverifiable": 0}
        for signature, (direct, deps, unknown) in self.uses:
            if direct or any(tags.get(t, (False, False))[0] for t in deps):
                family = "volatile"
            elif unknown or any(tags.get(t, (False, False))[1] for t in deps):
                family = "volatile_unverifiable"
            else:
                continue
            counts[family] += 1
            counts[f"{family}[{signature}]"] = counts.get(f"{family}[{signature}]", 0) + 1
        return counts


def enumerator_names(ca, ast) -> set[str]:
    names: set[str] = set()

    class Visitor(ca.NodeVisitor):
        def visit_Enumerator(self, n):
            names.add(n.name)

    Visitor().visit(ast)
    return names


def forbidden_delta(before: dict[str, int], after: dict[str, int]) -> list[str]:
    found = sorted(k for k in after if after[k] > before.get(k, 0))
    found += sorted(k for k in ALWAYS_FORBIDDEN if after.get(k) and k not in found)
    return found


def seed_artifacts(counts: dict[str, int]) -> dict[str, int]:
    """Forbidden artifacts already present in the seed (empty statements excluded: `for (;;)`)."""
    return {k: counts[k] for k in SEED_BLOCKING if counts.get(k)}
