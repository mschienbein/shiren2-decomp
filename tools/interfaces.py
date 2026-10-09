#!/usr/bin/env python3
"""Cross-translation-unit function and external object interface audit.

    uv run --frozen python tools/interfaces.py [--source-root DIR] [--extra FILE.json] [--json]
    uv run --frozen python tools/interfaces.py --prototype func_800B4D80 [SYMBOL ...]
    uv run --frozen python tools/interfaces.py --registry OUT.json

Byte equality does not make a program valid C. Every translation unit assigned in
`config/compiler_profiles.json` `tu_profiles` (plus optional candidates, a JSON list of
`{"source", "profile", "includes"}`) is preprocessed (`-E`, threaded) with its exact profile
driver, flags and environment; linemarkers are kept so a header declaration is one site at its
own file and line. GNU spellings are normalised for the diagnostic AST only
(`__attribute__((...))` dropped, `__alignof__` -> `sizeof`, `__inline`/`__inline__` -> `inline`)
and the result is parsed with pycparser. Typedefs are resolved and every return/parameter is
classified as pointer (arrays and functions decay in parameters), aggregate, void, float, double
or an integer kind. Integer compatibility is ISO-strict: besides width, the sub-kinds
signedness (int vs unsigned int, ...), int-vs-long (equal width, e.g. a `u32` typedef of
`unsigned long` against `unsigned int`), char-signedness (plain `char`, whatever the profile's
`-funsigned-char`, against explicit `signed`/`unsigned char`) and enum-underlying are
incompatible. As in GCC 2.8.1, an enum is compatible with unsigned int unless an enumerator is
negative, then with int; that underlying type is compared (so two declarations of one enum agree).
Different pointee struct views per TU are an accepted convention, and by-value aggregates
compare by category only (no layout).
External object declarations/definitions (including block-scope externs) are also classified
after typedef resolution, without array decay. Pointer vs integer, array vs pointer and
float/double vs integer are conflicts. Object pointee/element types, aggregate tags, integer
width/signedness and array-vs-scalar/aggregate storage views are accepted.

Findings (exit status 1):
- declaration-vs-definition: a declaration (or a further definition) of a function disagrees
  with its C definition in return or parameter category/integer type, parameter count or
  variadic;
- declaration-disagreement: declarations of a function without C definition (an assembly
  callee) disagree with each other in the same respects;
- unprototyped: a declaration or definition without prototype, including implicit declarations;
- call-arity: a direct call passes a different argument count than the C definition takes;
- object-category: declarations/definitions of an external object disagree in one of those
  conflicting categories; every site is reported;
- parse-failure: a translation unit failed to preprocess or parse; it is never skipped.
Static functions and objects are TU-local. `--prototype` prints the authoritative shape of a
function (its C definition, else the agreed declaration) for authors; `--registry` writes
external function shapes and object categories/sites, with disagreements. `tools/adopt.py`
uses `Gate` to refuse a candidate that introduces findings over the canonical tree plus the
candidates adopted before it.

C++ translation units (`.cpp`, a `language: c++` profile) take the same preprocessing step with
their own driver and flags, so cc1plus's view of the expanded TU (`-D__cplusplus`, headers) is
what is audited. pycparser cannot parse C++, so the preprocessed text is handed to the clang pinned
as `clang_ast` in toolchain.lock.json (hash-checked, `-fsyntax-only -Xclang -ast-dump=json` for a
32-bit MIPS target) and its JSON AST is walked: every function with C language linkage (declared
inside `extern "C"` or redeclaring one) contributes its declarations, definitions (including
inline bodies) and every direct call whose callee is that declaration, with the same categories
and integer kinds as C. Types are bound by clang's declaration identities, never by a spelling
looked up at the wrong scope: a typedef by its id and clang's type tree (implicit builtin typedefs
included), a record or enum by its declaration. A parameter or object spelling without a typedef
id is no typedef; its record/enum is the tag declaration clang's printed name (which omits
function scopes) denotes, and every tag it can denote must agree, else the TU is refused. Only a
written return type is looked up by name, through modelled scopes: blocks in order, selection and
iteration statements (each unbraced substatement its own block), complete classes inside member and
friend bodies, a namespace with its anonymous namespace (clang's implicit using-directive), and an
out-of-line definition's own class or namespace. A tag only a friend declaration has declared is
invisible to that lookup, and a lookup that would pass over one refuses; elaborated tags a local
class declares belong to its innermost enclosing non-class scope. `bool` and
`wchar_t` are distinct 4-byte integer kinds (g++ 2.8.1 on this target) compatible only with
themselves, references are pointers, and an enum's compatible type follows the same GCC 2.8.1 rule
as C. C++-linkage functions, methods and constructors are not part of the C interface (the C++
object may not export or import mangled names; see `tools/match.py`). External objects with
unmangled linkage, including ordinary global C++ objects and block-scope externs, join the
unchanged object-category gate without array decay. Clang's mangled identities distinguish these
from static, const, local-static and anonymous-namespace objects; externally mangled object names
fail closed. A missing or changed pinned clang, a clang error, a template, an asm label or alias,
an inline namespace, a using-declaration/directive, a member pointer anywhere in a type (behind a
typedef, pointer, reference or array too), a typeof/decltype spelling without a typedef's type tree,
a statement kind the scope model does not know, sugared function types or any type the classifier
cannot resolve makes the TU a parse-failure finding. Qualified names bind their
first prefix in the nearest scope; inherited, aliased or later-member (ill-formed, no diagnostic)
class lookups refuse rather than fall back to an outer shadow. Enum values require layout-independent
initializer dependencies (including static members): no sizeof/alignof/offsetof or type trait and
no pointer, reference, array or function value (pointer arithmetic scales by clang's element sizes),
and value-preserving known integer conversions of at most 32 bits; values outside the supported
32-bit enum range, or a negative enumerator with one above 0x7FFFFFFF, are refused.
"""
from __future__ import annotations

import argparse
import bisect
import json
import operator
import os
import re
import subprocess
import sys
import tempfile
import threading
import time
from collections import Counter, defaultdict
from concurrent.futures import ThreadPoolExecutor
from dataclasses import dataclass, field, replace
from pathlib import Path
from typing import Any, Iterable, NamedTuple

from pycparser import c_ast, c_generator, c_parser

from certification import compiler_environment, profile_kind, profile_toolchain, read_json, sha256, tool_profile
from evidence import COMPILED_SUFFIXES, compiled_kind
from rom import PROJECT

KINDS = ("parse-failure", "declaration-vs-definition", "declaration-disagreement", "unprototyped", "call-arity", "object-category")
OBJECT_CONFLICTS = (("pointer", "integer", "pointer-vs-integer"), ("array", "pointer", "array-vs-pointer"),
                    ("float", "integer", "float-vs-integer"), ("double", "integer", "float-vs-integer"))
ATTRIBUTE = re.compile(r"\b__attribute(?:__)?\s*\(")
ALIGNOF = re.compile(r"\b__alignof(?:__)?\b")
INLINE = re.compile(r"\b__inline(?:__)?\b")
INTEGER_WIDTHS = {"char": 1, "short": 2, "int": 4, "long": 4, "long long": 8}
BASIC = {"void", "char", "short", "int", "long", "float", "double", "signed", "unsigned"}
TYPE_NODES = (c_ast.Typename, c_ast.TypeDecl, c_ast.PtrDecl, c_ast.ArrayDecl, c_ast.FuncDecl, c_ast.Struct,
              c_ast.Union, c_ast.Enum, c_ast.IdentifierType)
WORK_LINK = "source"
# Enumerator constant expressions (32-bit int/unsigned int arithmetic).
INTEGER_LITERAL = re.compile(r"(0[xX][0-9a-fA-F]+|0[0-7]*|[1-9][0-9]*)([uUlL]*)")
OCTAL_ESCAPE = re.compile(r"\\[0-7]{1,3}")
CHAR_ESCAPES = {"\\n": 10, "\\t": 9, "\\r": 13, "\\a": 7, "\\b": 8, "\\f": 12, "\\v": 11, "\\\\": 92, "\\'": 39,
                '\\"': 34, "\\?": 63}
COMPARISONS = {"==": operator.eq, "!=": operator.ne, "<": operator.lt, ">": operator.gt, "<=": operator.le, ">=": operator.ge}
ARITHMETIC = {"+": operator.add, "-": operator.sub, "*": operator.mul, "&": operator.and_, "|": operator.or_, "^": operator.xor}


# ---------------------------------------------------------------- shapes

@dataclass(frozen=True)
class Kind:
    """One return or parameter type: `name` is shown and, for integers, is the exact compared type."""
    name: str
    category: str  # pointer | aggregate | void | float | double | integer
    width: int | None
    underlying: str | None = None  # an enum's compatible integer type, as GCC 2.8.1 chooses it

    def __str__(self) -> str:
        return self.name

    @property
    def rank(self) -> str:
        """Integer base type: char, short, int, long or long long."""
        return self.name.removesuffix(" unsigned").removesuffix(" signed")

    @property
    def sign(self) -> str:
        """unsigned, signed, or plain (`char`, a type distinct from both explicit forms)."""
        return "unsigned" if self.name.endswith(" unsigned") else "plain" if self.name == "char" else "signed"

    @property
    def compared(self) -> str:
        """What compatibility depends on: the exact integer type (an enum's underlying one), else the
        category (pointee views exempt)."""
        return (self.underlying or self.name) if self.category == "integer" else self.category

    def shown(self) -> str:
        return f"{self.name} ({self.underlying})" if self.underlying else self.name


POINTER = Kind("pointer", "pointer", 4)
VOID = Kind("void", "void", 0)
INT = Kind("int", "integer", 4)
# g++ 2.8.1 on this target: sizeof(bool) == sizeof(wchar_t) == 4 (measured with cc1plus).
CXX_INTEGERS = {"bool": Kind("bool", "integer", 4), "wchar_t": Kind("wchar_t", "integer", 4)}


@dataclass(frozen=True)
class Shape:
    returns: Kind
    params: tuple[Kind, ...] | None  # None: no prototype
    variadic: bool = False
    arity: int | None = None  # identifier count of an unprototyped (K&R or empty) definition

    @property
    def signature(self) -> tuple[Any, ...]:
        params = None if self.params is None else tuple(p.compared for p in self.params)
        return (self.returns.compared, params, self.variadic)

    def render(self, symbol: str) -> str:
        if self.params is None:
            inside = ""
        else:
            inside = ", ".join([*(p.shown() for p in self.params), *(["..."] if self.variadic else [])]) or "void"
        return f"{self.returns.shown()} {symbol}({inside})"

    def to_json(self) -> dict[str, Any]:
        return {"returns": self.returns.shown(), "params": None if self.params is None else [p.shown() for p in self.params],
                "variadic": self.variadic}


def incompatibilities(a: Kind, b: Kind) -> list[str]:
    """Sub-kinds by which two return/parameter kinds are incompatible C types.

    category; for integers (ISO-strict, even at equal width): width, int-vs-long, signedness,
    char-signedness when plain `char` meets an explicitly signed or unsigned char, and
    enum-underlying when an enum's compatible type (unsigned int without negative enumerators,
    else int) differs from the other side's integer or enum type. C++ `bool` and `wchar_t` are
    distinct types compatible only with themselves (`cxx-type`). Pointers are compatible
    whatever their pointee; by-value aggregates compare by category.
    """
    if a.category != b.category:
        return ["category"]
    if a.category != "integer" or a.compared == b.compared:
        return []
    if a.width != b.width:
        return ["width"]
    if a.underlying or b.underlying:
        return ["enum-underlying"]
    if {a.rank, b.rank} & CXX_INTEGERS.keys():
        return ["cxx-type"]
    subkinds = ["int-vs-long"] if a.rank != b.rank else []
    if "plain" in (a.sign, b.sign):
        subkinds.append("char-signedness")
    elif a.sign != b.sign:
        subkinds.append("signedness")
    return subkinds


def compare(expected: Shape, found: Shape) -> list[dict[str, Any]]:
    """Differences of `found` from `expected`; parameters only when both are prototyped."""
    differences: list[dict[str, Any]] = []

    def kinds(aspect: str, a: Kind, b: Kind) -> None:
        for subkind in incompatibilities(a, b):
            shown = (lambda k: f"{k.name} ({k.width})") if subkind == "width" else Kind.shown
            differences.append({"aspect": aspect if subkind == "category" else f"{aspect} {subkind}", "subkind": subkind,
                                "expected": shown(a), "found": shown(b)})

    kinds("return", expected.returns, found.returns)
    if expected.params is not None and found.params is not None:
        if len(expected.params) != len(found.params):
            differences.append({"aspect": "parameter count", "subkind": "count",
                                "expected": len(expected.params), "found": len(found.params)})
        for index, (a, b) in enumerate(zip(expected.params, found.params), 1):
            kinds(f"parameter {index}", a, b)
        if expected.variadic != found.variadic:
            differences.append({"aspect": "variadic", "subkind": "variadic", "expected": expected.variadic, "found": found.variadic})
    return differences


# ---------------------------------------------------------------- parsing

def normalise(text: str) -> str:
    """GNU spellings pycparser lacks, rewritten for the diagnostic AST only (line numbers kept)."""
    pieces = []
    position = 0
    while match := ATTRIBUTE.search(text, position):
        depth, index = 0, match.end() - 1
        while True:
            if index >= len(text):
                raise ValueError(f"unbalanced __attribute__ at offset {match.start()}")
            depth += {"(": 1, ")": -1}.get(text[index], 0)
            index += 1
            if depth == 0:
                break
        pieces += [text[position:match.start()], "\n" * text.count("\n", match.start(), index)]
        position = index
    pieces.append(text[position:])
    return INLINE.sub("inline", ALIGNOF.sub("sizeof", "".join(pieces)))


@dataclass(frozen=True)
class Site:
    """One declaration, definition or implicit declaration of a function."""
    symbol: str
    role: str  # definition | declaration | implicit
    file: str
    line: int
    column: int
    text: str
    shape: Shape
    local: bool = False  # internal linkage: TU-local

    @property
    def key(self) -> tuple[Any, ...]:
        return (self.role, self.file, self.line, self.column, self.shape.signature)

    def location(self) -> str:
        return f"{self.file}:{self.line}"

    def to_json(self) -> dict[str, Any]:
        return {"file": self.file, "line": self.line, "column": self.column, "role": self.role, "text": self.text,
                "shape": self.shape.to_json()}


@dataclass(frozen=True)
class ObjectSite:
    """One external object declaration or definition; arrays retain their storage category."""
    symbol: str
    role: str
    file: str
    line: int
    column: int
    text: str
    category: str

    @property
    def key(self) -> tuple[Any, ...]:
        return (self.role, self.file, self.line, self.column, self.category)

    def location(self) -> str:
        return f"{self.file}:{self.line}"

    def to_json(self) -> dict[str, Any]:
        return {"file": self.file, "line": self.line, "column": self.column, "role": self.role, "text": self.text,
                "category": self.category}


@dataclass(frozen=True)
class Call:
    symbol: str
    file: str
    line: int
    column: int
    arguments: int
    local: bool = False

    @property
    def key(self) -> tuple[Any, ...]:
        return ("call", self.file, self.line, self.column, self.arguments)

    def to_json(self) -> dict[str, Any]:
        return {"file": self.file, "line": self.line, "column": self.column, "role": "call", "arguments": self.arguments}


@dataclass
class TUResult:
    tu: str
    profile: str | None = None
    sites: list[Site] = field(default_factory=list)
    calls: list[Call] = field(default_factory=list)
    error: str | None = None
    candidate: int | None = None  # index into the audited extra candidates
    objects: list[ObjectSite] = field(default_factory=list)


def display(path: str, source_root: Path | None = None) -> str:
    """Report path: relative to the source root (`source/` link) or the project when possible."""
    if path.startswith(WORK_LINK + "/"):
        return path[len(WORK_LINK) + 1:]
    candidate = Path(path)
    if candidate.is_absolute():
        for root in (source_root, PROJECT):
            if root is not None and candidate.is_relative_to(root.resolve()):
                return candidate.relative_to(root.resolve()).as_posix()
    return path


class _Collector:
    """Walks one TU in order with C scopes; records function/object sites and direct calls."""

    def __init__(self, tu: str) -> None:
        self.tu = tu
        self.scopes: list[dict[str, tuple[str, Any]]] = [{}]
        self.raw_sites: list[tuple[Site, bool, bool]] = []  # site, file scope, declared static
        self.raw_calls: list[Call] = []
        self.objects: list[ObjectSite] = []
        self.generator = c_generator.CGenerator()
        self.enum_tags: dict[str, str] = {}  # tag -> compatible integer type
        self.enum_bodies: dict[int, str] = {}  # id(Enum with enumerators) -> compatible integer type
        self.enumerators: dict[str, tuple[int, bool]] = {}  # constant -> (value, unsigned)

    # -- types
    def lookup(self, name: str) -> tuple[str, Any] | None:
        for scope in reversed(self.scopes):
            if name in scope:
                return scope[name]
        return None

    def typedef(self, names: list[str]) -> Any | None:
        if len(names) == 1 and names[0] not in BASIC:
            entry = self.lookup(names[0])
            if entry is None or entry[0] != "typedef":
                raise ValueError(f"{self.tu}: unknown type name {names[0]}")
            return entry[1]
        return None

    def function_type(self, node: Any) -> c_ast.FuncDecl | None:
        while True:
            if isinstance(node, c_ast.FuncDecl):
                return node
            if isinstance(node, c_ast.TypeDecl) and isinstance(node.type, c_ast.IdentifierType):
                resolved = self.typedef(node.type.names)
                if resolved is None:
                    return None
                node = resolved
                continue
            return None

    def classify(self, node: Any, parameter: bool) -> Kind:
        alias = None  # typedef name, shown for an anonymous struct/union
        while True:
            if isinstance(node, c_ast.Typename):
                node = node.type
            elif isinstance(node, c_ast.PtrDecl):
                return POINTER
            elif isinstance(node, (c_ast.ArrayDecl, c_ast.FuncDecl)):
                if parameter:
                    return POINTER
                raise ValueError(f"{self.tu}: function returns an array or function type")
            elif isinstance(node, c_ast.TypeDecl):
                inner = node.type
                if isinstance(inner, (c_ast.Struct, c_ast.Union)):
                    keyword = "struct" if isinstance(inner, c_ast.Struct) else "union"
                    return Kind(f"{keyword} {inner.name or alias or '<anonymous>'}", "aggregate", None)
                if isinstance(inner, c_ast.Enum):
                    return Kind(f"enum {inner.name or alias or '<anonymous>'}", "integer", 4, self.enum_underlying(inner))
                resolved = self.typedef(inner.names)
                if resolved is not None:
                    alias, node = inner.names[0], resolved
                    continue
                return basic_kind(inner.names)
            else:
                raise ValueError(f"{self.tu}: unclassifiable type node {type(node).__name__}")

    def object_category(self, node: Any) -> str:
        """Resolve outer typedefs before classification, without decaying array objects."""
        while isinstance(node, c_ast.TypeDecl) and isinstance(node.type, c_ast.IdentifierType):
            resolved = self.typedef(node.type.names)
            if resolved is None:
                break
            node = resolved
        return "array" if isinstance(node, c_ast.ArrayDecl) else self.classify(node, parameter=False).category

    # -- enums
    def register_enums(self, node: Any) -> None:
        """Evaluate, in source order, every enum whose enumerators a declaration's type defines."""
        if isinstance(node, c_ast.Enum) and node.values is not None:
            self.enum_underlying(node)
        elif node is not None:
            for _, child in node.children():
                self.register_enums(child)

    def enum_underlying(self, node: c_ast.Enum) -> str:
        """GCC 2.8.1's compatible type: unsigned int, or int when any enumerator is negative."""
        if node.values is None:
            if node.name not in self.enum_tags:
                raise ValueError(f"{self.tu}: enum {node.name} used before its enumerators")
            return self.enum_tags[node.name]
        if id(node) not in self.enum_bodies:
            value, negative = (-1, False), False
            for enumerator in node.values.enumerators:
                value = self.constant(enumerator.value) if enumerator.value is not None else _wrap(value[0] + 1, value[1])
                self.enumerators[enumerator.name] = value
                negative |= not value[1] and value[0] < 0
            self.enum_bodies[id(node)] = "int" if negative else "int unsigned"
            if node.name:
                self.enum_tags[node.name] = self.enum_bodies[id(node)]
        return self.enum_bodies[id(node)]

    def constant(self, node: Any) -> tuple[int, bool]:
        """An enumerator's integer constant expression in 32-bit int/unsigned int arithmetic."""
        if isinstance(node, c_ast.Constant):
            return _literal(node.value)
        if isinstance(node, c_ast.ID) and node.name in self.enumerators:
            return self.enumerators[node.name]
        if isinstance(node, c_ast.UnaryOp) and node.op in ("-", "+", "~", "!"):
            value, unsigned = self.constant(node.expr)
            if node.op == "!":
                return int(not value), False
            return _wrap({"-": -value, "+": value, "~": ~value}[node.op], unsigned)
        if isinstance(node, c_ast.TernaryOp):
            return self.constant(node.iftrue if self.constant(node.cond)[0] else node.iffalse)
        if isinstance(node, c_ast.BinaryOp):
            (a, unsigned_a), (b, unsigned_b) = self.constant(node.left), self.constant(node.right)
            op = node.op
            if op in ("<<", ">>"):  # the result has the left operand's type
                return _wrap(a << (b & 31) if op == "<<" else a >> (b & 31), unsigned_a)
            if op in ("&&", "||"):
                return int(bool(a) and bool(b) if op == "&&" else bool(a) or bool(b)), False
            unsigned = unsigned_a or unsigned_b  # usual arithmetic conversions
            if unsigned:
                a, b = a & 0xFFFFFFFF, b & 0xFFFFFFFF
            if op in COMPARISONS:
                return int(COMPARISONS[op](a, b)), False
            if op in ("/", "%"):
                if b == 0:
                    raise ValueError(f"{self.tu}: division by zero in an enumerator")
                quotient = abs(a) // abs(b) * (-1 if (a < 0) != (b < 0) else 1)  # C truncates toward zero
                return _wrap(quotient if op == "/" else a - quotient * b, unsigned)
            if op in ARITHMETIC:
                return _wrap(ARITHMETIC[op](a, b), unsigned)
        raise ValueError(f"{self.tu}: unsupported enumerator expression {self.generator.visit(node)}")

    def shape(self, function: c_ast.FuncDecl, definition: c_ast.FuncDef | None = None) -> Shape:
        returns = self.classify(function.type, parameter=False)
        if function.args is None:
            return Shape(returns, None, arity=0 if definition is not None else None)
        params = function.args.params
        if any(isinstance(p, c_ast.ID) for p in params):  # K&R identifier list
            return Shape(returns, None, arity=len(params))
        variadic = bool(params) and isinstance(params[-1], c_ast.EllipsisParam)
        fixed = params[:-1] if variadic else params
        kinds = tuple(self.classify(p.type, parameter=True) for p in fixed)
        if len(kinds) == 1 and kinds[0] == VOID and not getattr(fixed[0], "name", None):
            kinds = ()
        return Shape(returns, kinds, variadic)

    # -- sites
    def coord(self, node: Any) -> tuple[str, int, int]:
        coord = node.coord
        return display(coord.file) if coord and coord.file else self.tu, coord.line if coord else 0, (coord.column or 0) if coord else 0

    def site(self, node: Any, name: str, role: str, text: str, shape: Shape, storage: list[str]) -> None:
        file, line, column = self.coord(node)
        self.raw_sites.append((Site(name, role, file, line, column, text, shape), len(self.scopes) == 1, "static" in storage))

    def declare(self, node: c_ast.Decl) -> None:
        self.register_enums(node.type)
        if node.name is None:
            return
        function = self.function_type(node.type)
        if function is None:
            file_scope = len(self.scopes) == 1
            extern = "extern" in node.storage
            # An extern inherits a visible object's linkage, but not a local's lack of linkage.
            previous = self.lookup(node.name) if extern else None
            linkage = "external" if file_scope or extern else None
            if "static" in node.storage:
                linkage = "internal" if file_scope else None
            elif previous is not None and previous[0] == "object" and previous[1] is not None:
                linkage = previous[1]
            if linkage == "external":
                category = self.object_category(node.type)
                file, line, column = self.coord(node)
                role = "definition" if node.init is not None or (file_scope and not extern) else "declaration"
                self.objects.append(ObjectSite(node.name, role, file, line, column, self.generator.visit(node), category))
            self.scopes[-1][node.name] = ("object", linkage)
            if node.init is not None:
                self.walk(node.init)
            return
        self.site(node, node.name, "declaration", self.generator.visit(node), self.shape(function), node.storage)
        self.scopes[-1][node.name] = ("function", None)

    def define(self, node: c_ast.FuncDef) -> None:
        decl = node.decl
        self.register_enums(decl.type)
        function = self.function_type(decl.type)
        # GNU89 `extern inline` bodies are never the external definition.
        role = "declaration" if "extern" in decl.storage and "inline" in decl.funcspec else "definition"
        self.site(decl, decl.name, role, self.generator.visit(decl), self.shape(function, node), decl.storage)
        self.scopes[-1][decl.name] = ("function", None)
        self.scopes.append({})
        params = function.args.params if function.args is not None else []
        for param in params:
            name = getattr(param, "name", None)
            if name:
                self.scopes[-1][name] = ("object", None)
        for param in node.param_decls or []:
            self.scopes[-1][param.name] = ("object", None)
        self.walk(node.body)
        self.scopes.pop()

    def call(self, node: c_ast.FuncCall) -> None:
        arguments = len(node.args.exprs) if node.args is not None else 0
        if isinstance(node.name, c_ast.ID):
            name = node.name.name
            entry = self.lookup(name)
            if entry is None and not name.startswith("__builtin_"):
                file, line, column = self.coord(node)
                self.raw_sites.append((Site(name, "implicit", file, line, column, f"int {name}() /* implicit */",
                                            Shape(INT, None)), True, False))
                self.scopes[0][name] = ("function", None)
                entry = self.scopes[0][name]
            if entry is not None and entry[0] == "function":
                file, line, column = self.coord(node)
                self.raw_calls.append(Call(name, file, line, column, arguments))
        else:
            self.walk(node.name)
        if node.args is not None:
            self.walk(node.args)

    # -- traversal
    def top(self, ast: c_ast.FileAST) -> None:
        for node in ast.ext:
            if isinstance(node, c_ast.FuncDef):
                self.define(node)
            elif isinstance(node, c_ast.Decl):
                self.declare(node)
            elif isinstance(node, c_ast.Typedef):
                self.register_enums(node.type)
                self.scopes[0][node.name] = ("typedef", node.type)

    def walk(self, node: Any) -> None:
        if node is None or isinstance(node, TYPE_NODES):
            return
        if isinstance(node, c_ast.Decl):
            self.declare(node)
        elif isinstance(node, c_ast.Typedef):
            self.register_enums(node.type)
            self.scopes[-1][node.name] = ("typedef", node.type)
        elif isinstance(node, c_ast.FuncCall):
            self.call(node)
        elif isinstance(node, (c_ast.Compound, c_ast.For)):
            self.scopes.append({})
            for _, child in node.children():
                self.walk(child)
            self.scopes.pop()
        else:
            for _, child in node.children():
                self.walk(child)

    def result(self, profile: str | None) -> TUResult:
        static = {site.symbol for site, file_scope, is_static in self.raw_sites if file_scope and is_static}
        sites = [replace(site, local=site.symbol in static) for site, _, _ in self.raw_sites]
        calls = [replace(call, local=call.symbol in static) for call in self.raw_calls]
        return TUResult(self.tu, profile, sites, calls, objects=self.objects)


def basic_kind(names: list[str]) -> Kind:
    words = Counter(names)
    if "void" in words:
        return VOID
    if "float" in words:
        return Kind("float", "float", 4)
    if "double" in words:  # long double is double on this target
        return Kind("double", "double", 8)
    if "char" in words:
        base = "char"
    elif "short" in words:
        base = "short"
    elif words["long"] >= 2:
        base = "long long"
    elif "long" in words:
        base = "long"
    else:
        base = "int"
    if "unsigned" in words:
        return Kind(f"{base} unsigned", "integer", INTEGER_WIDTHS[base])
    if base == "char" and "signed" in words:
        return Kind("char signed", "integer", 1)
    return Kind(base, "integer", INTEGER_WIDTHS[base])


def _wrap(value: int, unsigned: bool) -> tuple[int, bool]:
    """A 32-bit int (two's complement) or unsigned int value."""
    value &= 0xFFFFFFFF
    return (value - (1 << 32) if not unsigned and value & 0x80000000 else value), unsigned


def _literal(text: str) -> tuple[int, bool]:
    """(value, unsigned) of a C89 integer or plain-ASCII character constant on this 32-bit target."""
    if text.startswith("'"):
        body = text[1:-1]
        value = (ord(body) if len(body) == 1 else CHAR_ESCAPES.get(body) if body in CHAR_ESCAPES
                 else int(body[2:], 16) if body.startswith("\\x") else int(body[1:], 8) if OCTAL_ESCAPE.fullmatch(body) else None)
        if value is None or value > 0x7F:  # beyond ASCII the value depends on the profile's char signedness
            raise ValueError(f"unsupported character constant {text} in an enumerator")
        return value, False
    match = INTEGER_LITERAL.fullmatch(text)
    if match is None:
        raise ValueError(f"unsupported integer constant {text} in an enumerator")
    digits, suffix = match.groups()
    value = int(digits, 16 if digits[:2] in ("0x", "0X") else 8 if digits.startswith("0") else 10)
    if value > 0xFFFFFFFF:
        raise ValueError(f"integer constant {text} wider than 32 bits in an enumerator")
    # Without a suffix, a value beyond int is unsigned int (hex/octal) or unsigned long (decimal).
    return value, "u" in suffix.lower() or value > 0x7FFFFFFF


_parsers = threading.local()


def parse_tu(preprocessed: str, tu: str, profile: str | None = None) -> TUResult:
    """Collect function/object sites and direct calls from one preprocessed TU; failures are recorded."""
    parser = getattr(_parsers, "parser", None)
    if parser is None:
        parser = _parsers.parser = c_parser.CParser()
    try:
        ast = parser.parse(normalise(preprocessed), filename=tu)
        collector = _Collector(tu)
        collector.top(ast)
        return collector.result(profile)
    except Exception as error:  # every failure becomes a parse-failure finding
        return TUResult(tu, profile, error=f"parse: {type(error).__name__}: {error}")


# ---------------------------------------------------------------- C++ (pinned clang JSON AST)

LINE_DIRECTIVE = re.compile(r'#(?:line)? (\d+) "([^"]*)"')
CLANG_LOCATION = re.compile(r"<stdin>:(\d+):(\d+)")
CXX_QUALIFIERS = {"const", "volatile", "__restrict", "restrict"}
CXX_QUALIFIER_EDGES = re.compile(r"^(?:(?:const|volatile|__restrict|restrict)\s+)+|(?:\s+(?:const|volatile|__restrict|restrict))+$")
CXX_BUILTIN_WORDS = {"void", "bool", "wchar_t", "char", "short", "int", "long", "float", "double", "signed", "unsigned"}
CXX_TYPE_SUFFIXES = (" throw()", " noexcept", " __attribute__((noreturn))")
# Constructs whose C interface this walker does not model; a TU using one is a parse-failure.
CXX_UNSUPPORTED = {"FunctionTemplateDecl", "ClassTemplateDecl", "ClassTemplatePartialSpecializationDecl",
                   "VarTemplateDecl", "TypeAliasTemplateDecl", "UnresolvedLookupExpr", "UnresolvedMemberExpr",
                   "CXXDependentScopeMemberExpr", "AsmLabelAttr", "AliasAttr",
                   "UsingDecl", "UsingDirectiveDecl", "NamespaceAliasDecl"}
CXX_FUNCTIONS = {"FunctionDecl", "CXXMethodDecl", "CXXConstructorDecl", "CXXDestructorDecl", "CXXConversionDecl"}
CXX_RECORDS = {"CXXRecordDecl", "RecordDecl"}
CXX_TAGS = CXX_RECORDS | {"EnumDecl"}
CXX_TYPEDEFS = {"TypedefDecl", "TypeAliasDecl"}
# Not ordinary-lookup class members: constructors, destructors and conversions are found through the
# class name, never by their own.
CXX_NOT_MEMBERS = CXX_RECORDS | CXX_TYPEDEFS | {"EnumDecl", "FriendDecl", "AccessSpecDecl", "StaticAssertDecl",
                                                "CXXConstructorDecl", "CXXDestructorDecl", "CXXConversionDecl"}
CXX_RECORD_KEYWORDS = {"struct", "class", "union"}
CXX_ARRAY = Kind("array", "array", None)
CXX_ANONYMOUS_NAMESPACE = "(anonymous namespace)"
CXX_ANONYMOUS_TAG = re.compile(r"\((?:anonymous|unnamed) (struct|class|union|enum) at [^()]*\)")
# The outer pointer, reference or array declarator decides the category of these type nodes.
CXX_DECLARATOR_TYPES = {"PointerType", "LValueReferenceType", "RValueReferenceType", "ConstantArrayType",
                        "IncompleteArrayType"}
# typeof/decltype spell an operand, not the type tree it stands for.
CXX_OPAQUE_SPELLING = re.compile(r"\b(?:typeof|typeof_unqual|__typeof__|__typeof|decltype|__decltype)\s*\(")
# Selection and iteration statements are scopes and so is each of their substatements, braced or not
# (C++98 6.4/1, 6.5/2). Any other statement kind must be scope-neutral and listed here, or it refuses.
CXX_STATEMENT_SCOPES = {"IfStmt", "SwitchStmt", "WhileStmt", "DoStmt", "ForStmt", "CXXForRangeStmt"}
CXX_PLAIN_STATEMENTS = {"CompoundStmt", "DeclStmt", "NullStmt", "LabelStmt", "CaseStmt", "DefaultStmt", "ReturnStmt",
                        "BreakStmt", "ContinueStmt", "GotoStmt", "IndirectGotoStmt", "AttributedStmt", "GCCAsmStmt"}
# Name lookup: a nested-name-specifier names a namespace or type; an elaborated name ignores non-types.
# A "friend" entry (a tag only a friend declaration has declared) is never found.
CXX_TYPE_ENTRIES = frozenset({"typedef", "record", "enum"})
CXX_QUALIFIER_ENTRIES = CXX_TYPE_ENTRIES | {"namespace"}
CXX_ORDINARY_ENTRIES = CXX_QUALIFIER_ENTRIES | {"other"}
# Enumerator dependencies whose value clang derives from its own layout or type properties.
CXX_LAYOUT_EXPRESSIONS = {"UnaryExprOrTypeTraitExpr", "OffsetOfExpr", "ArraySubscriptExpr", "TypeTraitExpr",
                          "ArrayTypeTraitExpr", "ExpressionTraitExpr", "AddrLabelExpr"}
CXX_POINTER_CASTS = {"PointerToIntegral", "PointerToBoolean", "IntegralToPointer", "NullToPointer",
                     "ArrayToPointerDecay", "FunctionToPointerDecay", "BuiltinFnToFnPtr", "BitCast", "LValueBitCast"}
MANGLED_LENGTH = re.compile(r"[1-9][0-9]*")


def clang_ast_command(tool_root: Path, source_root: Path) -> tuple[list[str] | None, str | None]:
    """The pinned clang argv dumping a preprocessed C++ TU (stdin) as a JSON AST, or why it cannot run."""
    try:
        spec = read_json(source_root / "toolchain.lock.json")["clang_ast"]
        relative = Path(spec["install_directory"])
        invocation = spec["invocation"]
        if relative.is_absolute() or relative.parts[0] != ".cache" or ".." in relative.parts:
            return None, "invalid clang_ast install directory"
        if not isinstance(invocation, list) or any(not isinstance(flag, str) for flag in invocation):
            return None, "invalid clang_ast invocation"
        path = tool_root / relative / "clang"
        if not path.is_file() or sha256(path) != spec["binary_sha256"]:
            return None, f"pinned clang_ast {relative / 'clang'} is missing or changed; run tools/setup.py"
        return [str(path.resolve()), *invocation, "-"], None
    except (OSError, KeyError, TypeError, ValueError) as error:
        return None, f"no usable clang_ast pin in toolchain.lock.json ({type(error).__name__}: {error})"


class _LineMap:
    """Preprocessed-text offsets -> (displayed file, presumed line, column) through cpp linemarkers."""

    def __init__(self, text: str, tu: str) -> None:
        self.tu = tu
        self.starts: list[int] = []
        self.markers: list[tuple[int, str, int]] = []  # physical line, file, presumed line of the next line
        offset = 0
        for index, line in enumerate(text.split("\n")):
            self.starts.append(offset)
            offset += len(line) + 1
            found = LINE_DIRECTIVE.match(line)
            if found:
                self.markers.append((index, found[2], int(found[1])))
        self.marker_lines = [marker[0] for marker in self.markers]

    def __call__(self, offset: int) -> tuple[str, int, int]:
        index = bisect.bisect_right(self.starts, offset) - 1
        column = offset - self.starts[index] + 1
        position = bisect.bisect_left(self.marker_lines, index) - 1  # last marker before this line
        if position < 0:
            return self.tu, index + 1, column
        line, file, presumed = self.markers[position]
        return display(file), presumed + index - line - 1, column


def split_function_type(text: str) -> tuple[str, list[str]]:
    """`R (P1, P2, ...)` -> (R, [P1, P2, ...]); `...` stays as the last parameter."""
    text = text.strip()
    for suffix in CXX_TYPE_SUFFIXES:
        text = text.removesuffix(suffix)
    if not text.endswith(")"):
        raise ValueError(f"unsupported function type {text!r}")
    depth = 0
    for index in range(len(text) - 1, -1, -1):
        depth += {")": 1, "(": -1}.get(text[index], 0)
        if depth == 0:
            break
    else:
        raise ValueError(f"unbalanced function type {text!r}")
    inside, parameters, depth, start = text[index + 1:-1], [], 0, 0
    for position, character in enumerate(inside + ","):
        depth += {"(": 1, "[": 1, "<": 1, ")": -1, "]": -1, ">": -1}.get(character, 0)
        if character == "," and depth == 0:
            parameters.append(inside[start:position].strip())
            start = position + 1
    parameters = [] if parameters in ([""], ["void"]) else parameters
    return text[:index].rstrip(), parameters


def _without_qualifiers(text: str) -> str:
    return CXX_QUALIFIER_EDGES.sub("", text.strip())


def _split_keyword(text: str) -> tuple[str | None, str]:
    """`struct T` -> ("struct", "T"); an unelaborated spelling has no keyword."""
    head, _, rest = text.partition(" ")
    return (head, rest.strip()) if head in CXX_RECORD_KEYWORDS | {"enum"} and rest.strip() else (None, text)


def _layout_free(type_: dict[str, Any]) -> bool:
    """A value of this clang type has no pointer, reference, array or function component."""
    spelling = type_.get("desugaredQualType", type_.get("qualType", ""))
    spelling = CXX_ANONYMOUS_TAG.sub("", spelling).replace(CXX_ANONYMOUS_NAMESPACE, "")
    return not any(character in spelling for character in "*&[(")


def _internal_mangling(emitted: str) -> bool:
    """Itanium names of internal-linkage objects: `_ZL`, local statics (`_ZZ`), and nested names inside
    an anonymous namespace or whose last component is `L`-prefixed (a namespace-scope const object)."""
    if emitted.startswith(("_ZL", "_ZZ")):
        return True
    if not emitted.startswith("_ZN") or not emitted.endswith("E"):
        return False
    index, internal, names = 3, False, []
    while index < len(emitted) - 1:
        internal = emitted[index] == "L"
        index += internal
        length = MANGLED_LENGTH.match(emitted, index)
        if length is None:
            return False
        index = length.end() + int(length[0])
        names.append(emitted[length.end():index])
    return index == len(emitted) - 1 and (internal or any(name.startswith("_GLOBAL__N") for name in names))


class _Entry(NamedTuple):
    order: int  # AST pre-order position of the declaration
    kind: str  # typedef | record | enum | namespace | other (a non-type class member) | friend (see CXX_TYPE_ENTRIES)
    ref: str  # declaration id, or a namespace's scope


@dataclass
class _Tag:
    kind: str  # record | enum
    printed: str | None  # clang's printed name (no function scopes); None when not derived here
    names: set[str]  # its own name, or the typedef names of an unnamed tag
    node: dict[str, Any]
    members: str | None = None  # a record's member scope


class _CxxCollector:
    """Walk clang's JSON AST: C-linkage function sites/calls and unmangled external objects.

    Types are bound by clang's declaration identities: a typedef by its id and type tree, a record or
    enum by its declaration. A spelling without an id is no typedef; its tag must be the only agreeing
    one under clang's printed name. Only a written return type is looked up by name, in modelled scopes
    (blocks, selection/iteration statements and their substatements, complete classes, namespaces with
    their anonymous namespaces, an out-of-line body's own scope), among visible declarations: a
    friend-only tag never binds and a lookup past one refuses; any other step refuses.
    """

    def __init__(self, tu: str, text: str) -> None:
        self.tu = tu
        self.locate = _LineMap(text, tu)
        self.orders: dict[str, int] = {}
        self.entries: dict[str, dict[str, list[_Entry]]] = defaultdict(lambda: defaultdict(list))  # scope -> name
        self.contexts: dict[str, tuple[str | None, str | None]] = {}  # declaration context id -> (scope, printed prefix)
        self.lookups: dict[str, tuple[str, frozenset[str]]] = {}  # function id -> (lexical scope, complete classes)
        self.class_scopes: set[str] = set()
        self.namespace_scopes: set[str] = set()
        self.inherited_scopes: set[str] = set()  # unmodelled base lookup must not fall through to an outer typedef
        self.tags: dict[str, _Tag] = {}
        self.typedef_nodes: dict[str, dict[str, Any]] = {}  # every typedef, implicit ones included
        self.typedef_kinds: dict[str, Kind] = {}
        self.resolving: set[str] = set()
        self.enum_kinds: dict[str, str] = {}
        self.constants: dict[str, dict[str, Any]] = {}
        self.enum_predecessors: dict[str, str] = {}
        self.c_functions: dict[str, str] = {}  # clang declaration id -> name, every C-linkage redeclaration
        self.function_local: dict[str, bool] = {}  # linkage follows declaration identity, not a namespace's short name
        self.raw_sites: list[Site] = []
        self.raw_calls: list[Call] = []
        self.objects: list[ObjectSite] = []

    def location(self, node: dict[str, Any]) -> tuple[str, int, int]:
        loc = node.get("loc") or node.get("range", {}).get("begin") or {}
        offset = loc.get("offset", loc.get("spellingLoc", {}).get("offset"))
        return (self.tu, 0, 0) if offset is None else self.locate(offset)

    def where(self, node: dict[str, Any]) -> str:
        return ":".join(map(str, self.location(node)[:2]))

    # -- pre-pass: declaration identities, lexical scopes and clang's printed tag names
    def index(self, root: dict[str, Any]) -> None:
        self.contexts[root.get("id", "")] = ("", "")
        self._index(root, "", "", frozenset())

    def _index(self, node: dict[str, Any], scope: str, printed: str | None, complete: frozenset[str],
               friend: bool = False) -> None:
        identity, kind, name = node.get("id", ""), node.get("kind"), node.get("name")
        order = self.orders.setdefault(identity, len(self.orders))
        children = node.get("inner", [])
        if kind in ("VarDecl", "EnumConstantDecl"):
            self.constants[identity] = node  # declaration identities, not DeclRefExpr's initializer-free summaries
        if kind == "NamespaceDecl":
            if node.get("isInline"):  # an implicit scope import that is not modelled
                raise ValueError(f"{self.tu}: unsupported C++ construct inline NamespaceDecl at {self.where(node)}")
            part = name or CXX_ANONYMOUS_NAMESPACE
            self.add(scope, part, "namespace", f"{scope}{part}::", order)
            scope, printed = f"{scope}{part}::", None if printed is None else f"{printed}{part}::"
            self.namespace_scopes.add(scope)
            self.contexts[identity] = (scope, printed)
        elif kind == "LinkageSpecDecl":
            self.contexts[identity] = (scope, printed)
        elif kind in CXX_RECORDS and not node.get("isImplicit"):
            home, prefix = self.home(node, scope, printed)
            if name:  # a friend-only tag is invisible to ordinary lookup until redeclared
                self.add(home, name, "friend" if friend else "record", identity, order)
            scope = f"{home}{name}::" if name else f"{home}(record {identity})::"  # unnamed: members stay inside
            # Clang prints an unnamed class's nested names through its typedef name; not derived here.
            tag_printed = f"{prefix}{name}" if name and prefix is not None else None
            self.tags[identity] = _Tag("record", tag_printed, {name} if name else set(), node, scope)
            self.class_scopes.add(scope)
            if node.get("bases"):
                self.inherited_scopes.add(scope)
            printed = None if tag_printed is None else f"{tag_printed}::"
            self.contexts[identity] = (scope, printed)
            for child in children:
                self._index(child, scope, printed, complete)
                self.add_member(scope, child)
            return
        elif kind == "EnumDecl":
            home, prefix = self.home(node, scope, printed)
            if name:
                self.add(home, name, "friend" if friend else "enum", identity, order)
            self.tags[identity] = _Tag("enum", f"{prefix}{name}" if name and prefix is not None else None,
                                       {name} if name else set(), node)
            enumerators = [child["id"] for child in children if child.get("kind") == "EnumConstantDecl"]
            self.enum_predecessors.update(zip(enumerators[1:], enumerators))
        elif kind in CXX_TYPEDEFS:
            self.typedef_nodes[identity] = node
            if name and not node.get("isImplicit"):
                self.add(scope, name, "typedef", identity, order)
                self.name_unnamed_tag(node)
            return  # a type tree declares nothing
        elif kind in CXX_FUNCTIONS:
            self.lookups[identity] = (scope, complete)
            self.contexts[identity] = (None, "")  # clang prints function-local types without outer scopes
            body = scope
            if ("parentDeclContextId" in node and scope not in self.class_scopes
                    and any(child.get("kind") == "CompoundStmt" for child in children)):
                body = self.home(node, scope, printed)[0]  # an out-of-line body looks up in its own class/namespace
            # Every body inside a class definition is a complete-class context of the enclosing classes.
            inside = complete | {prefix for prefix in self.prefixes(body) if prefix in self.class_scopes}
            for child in children:
                compound = child.get("kind") == "CompoundStmt"
                self._index(child, body if compound else scope, "", inside if compound else complete)
            return
        elif kind == "FriendDecl":
            for child in children:
                self._index(child, scope, printed, complete, friend=child.get("kind") in CXX_TAGS)
            return
        elif kind in CXX_STATEMENT_SCOPES:
            scope = f"{scope}(statement {identity})::"
            substatements = self.substatements(node)
            for position, child in enumerate(children):
                inner = f"{scope}(substatement {position})::" if position in substatements else scope
                self._index(child, inner, printed, complete)
            return
        elif kind == "CompoundStmt":
            scope = f"{scope}(block {identity})::"
        elif str(kind).endswith("Stmt") and kind not in CXX_PLAIN_STATEMENTS:
            raise ValueError(f"{self.tu}: unsupported C++ statement {kind} at {self.where(node)}")
        for child in children:
            self._index(child, scope, printed, complete)

    def substatements(self, node: dict[str, Any]) -> set[int]:
        """Positions of a selection/iteration statement's substatements in clang's children: an optional
        init statement and condition variable precede the condition, absent `for` parts are `{}`."""
        kind, count = node["kind"], len(node.get("inner", []))
        head = bool(node.get("hasInit")) + bool(node.get("hasVar"))
        expected, positions = {"IfStmt": (head + 2 + bool(node.get("hasElse")), {head + 1, head + 2}),
                               "SwitchStmt": (head + 2, {head + 1}), "WhileStmt": (head + 2, {head + 1}),
                               "DoStmt": (2, {0}), "ForStmt": (5, {4}), "CXXForRangeStmt": (8, {7})}[kind]
        if count != expected or node.get("isConsteval"):
            raise ValueError(f"{self.tu}: unsupported C++ {kind} layout at {self.where(node)}")
        return {position for position in positions if position < count}

    def home(self, node: dict[str, Any], scope: str, printed: str | None) -> tuple[str, str | None]:
        """Scope and printed prefix of a declaration's semantic context (friend, elaborated, out-of-line).
        A function context (a local class's friend or elaborated tag) is the innermost non-class scope
        around the declaration, a block or statement scope."""
        context = node.get("parentDeclContextId")
        if context is None:
            return scope, printed
        if context not in self.contexts:
            raise ValueError(f"{self.tu}: unmodelled declaration context of {node.get('kind')} {node.get('name')}")
        home, prefix = self.contexts[context]
        if home is None:
            home = scope
            while home in self.class_scopes:
                home = self.enclosing(home)
        return home, prefix

    def add(self, scope: str, name: str, kind: str, ref: str, order: int) -> None:
        self.entries[scope][name].append(_Entry(order, kind, ref))

    def add_member(self, scope: str, child: dict[str, Any]) -> None:
        """Non-type class members (and enumerators) also take part in class-scope lookup."""
        if child.get("isImplicit"):
            return
        kind = child.get("kind")
        members = ([item for item in child.get("inner", []) if item.get("kind") == "EnumConstantDecl"]
                   if kind == "EnumDecl" else [] if kind in CXX_NOT_MEMBERS else [child])
        for member in members:
            if member.get("name"):
                self.add(scope, member["name"], "other", member["id"], self.orders[member["id"]])

    def name_unnamed_tag(self, node: dict[str, Any]) -> None:
        """`typedef struct { ... } S;`: clang prints the unnamed class as S."""
        trees = self.type_children(node)
        tree = trees[0] if len(trees) == 1 else {}
        while tree.get("kind") == "ElaboratedType" and len(self.type_children(tree)) == 1:
            tree = self.type_children(tree)[0]
        tag = self.tags.get(tree.get("decl", {}).get("id"))
        if tag is not None and not tag.node.get("name"):
            tag.names.add(node["name"])

    @staticmethod
    def prefixes(scope: str) -> list[str]:
        parts = scope.split("::")[:-1]
        return ["".join(f"{part}::" for part in parts[:count]) for count in range(1, len(parts) + 1)]

    @staticmethod
    def enclosing(scope: str) -> str:
        parent = scope[:-2].rpartition("::")[0]
        return parent + "::" if parent else ""

    # -- pass 1: C-linkage declarations, definitions and objects
    def declarations(self, node: dict[str, Any], c_linkage: bool = False, linkage_extern: bool = False) -> None:
        kind, name = node.get("kind"), node.get("name")
        if kind in CXX_UNSUPPORTED and not self.anonymous_using(node):
            raise ValueError(f"{self.tu}: unsupported C++ construct {kind} at {self.where(node)}")
        if kind == "LinkageSpecDecl":
            c_linkage = node.get("language") == "C"
            linkage_extern = not node.get("hasBraces", False)
        elif kind in CXX_TYPEDEFS:
            if name and not node.get("isImplicit"):
                self.typedef_by_id(node["id"])  # every written typedef must classify, used or not
            return
        elif kind == "EnumDecl" and name:
            self.enum_kind(node["id"])
        elif kind == "FunctionDecl" and not node.get("isImplicit"):
            self.function(node, c_linkage)
        elif kind == "VarDecl" and not node.get("isImplicit"):
            self.object(node, linkage_extern)
        for child in node.get("inner", []):
            self.declarations(child, c_linkage, linkage_extern if kind == "LinkageSpecDecl" else False)

    @staticmethod
    def anonymous_using(node: dict[str, Any]) -> bool:
        """Clang's implicit using-directive after an anonymous namespace (modelled by `visible`)."""
        return (node.get("kind") == "UsingDirectiveDecl" and bool(node.get("isImplicit"))
                and node.get("nominatedNamespace", {}).get("name") == "")

    def enum_underlying(self, node: dict[str, Any]) -> str:
        """GCC 2.8.1's compatible type, as for C: unsigned int, or int when any enumerator is negative."""
        if "fixedUnderlyingType" in node:
            raise ValueError(f"{self.tu}: enum {node.get('name')} has a fixed underlying type")
        if not self.target_independent_constant(node):
            # Clang's bool/record layout is not g++ 2.8.1's; follow const aliases before trusting a value.
            raise ValueError(f"{self.tu}: layout-dependent or unresolved enum {node.get('name')} is unsupported")
        value, low, high = -1, 0, 0
        for child in node.get("inner", []):
            if child.get("kind") != "EnumConstantDecl":
                continue
            if child.get("inner"):
                value = self.enum_constant_value(child["inner"][0])
            else:
                value += 1
            if not -(1 << 31) <= value <= 0xFFFFFFFF:
                raise ValueError(f"{self.tu}: enumerator {child.get('name')} is outside supported 32-bit range")
            low, high = min(low, value), max(high, value)
        if low < 0 and high > 0x7FFFFFFF:  # no 32-bit type holds both: g++ and clang widen the enum
            raise ValueError(f"{self.tu}: enum {node.get('name')} spans {low}..{high}, outside supported 32-bit range")
        return "int" if low < 0 else "int unsigned"

    def enum_constant_value(self, node: dict[str, Any]) -> int:
        """Read a folded value through only proven value-preserving enum conversions."""
        if node.get("kind") == "ConstantExpr" and "value" in node:
            return int(node["value"])
        children = node.get("inner", [])
        if len(children) == 1:
            if node.get("kind") == "ParenExpr":
                return self.enum_constant_value(children[0])
            if node.get("kind") == "ImplicitCastExpr" and node.get("castKind") in ("IntegralCast", "NoOp"):
                value = self.enum_constant_value(children[0])
                type_ = node.get("type", {})
                words = type_.get("desugaredQualType", type_.get("qualType", "")).split()
                # These target integer widths are known; bool, wchar_t, plain char and unknown
                # types do not justify unwrapping a representation conversion.
                if words and set(words) <= {"char", "short", "int", "long", "signed", "unsigned"}:
                    converted = basic_kind(words)
                    if converted.width > 4:  # clang widened the enum beyond any 32-bit type
                        raise ValueError(f"{self.tu}: enum constant conversion to {type_.get('qualType')} "
                                         "is outside supported 32-bit range")
                    if converted.name != "char":
                        bits = converted.width * 8
                        low = 0 if converted.sign == "unsigned" else -(1 << (bits - 1))
                        high = (1 << (bits if converted.sign == "unsigned" else bits - 1)) - 1
                        if low <= value <= high:
                            return value
                raise ValueError(f"{self.tu}: unsupported enum constant conversion to {type_}")
        raise ValueError(f"{self.tu}: enumerator has no supported constant value")

    def target_independent_constant(self, node: dict[str, Any], active: frozenset[str] = frozenset()) -> bool:
        """Prove that the expression and all referenced initializers avoid foreign layout: no sizeof,
        alignof, offsetof or type trait, and no pointer, reference, array or function value (pointer
        arithmetic, subscripts and address casts scale by clang's element sizes)."""
        kind = node.get("kind")
        if kind in CXX_LAYOUT_EXPRESSIONS or node.get("castKind") in CXX_POINTER_CASTS:
            return False
        if "type" in node and not _layout_free(node["type"]):
            return False
        if kind in ("DeclRefExpr", "MemberExpr"):
            identity = (node.get("referencedDecl", {}).get("id") if kind == "DeclRefExpr"
                        else node.get("referencedMemberDecl"))
            target = self.constants.get(identity)
            if target is None or identity in active:
                return False
            if not self.target_independent_constant(target, active | {identity}):
                return False
            # MemberExpr's base is also an expression; its initializer cannot hide layout use.
        children = node.get("inner", [])
        if kind == "VarDecl" and (not node.get("init") or not children):
            previous = node.get("previousDecl")
            target = self.constants.get(previous)
            if target is None or previous in active:
                return False  # no initializer whose target independence can be established
            return self.target_independent_constant(target, active | {previous})
        if kind == "EnumConstantDecl" and not children:
            previous = self.enum_predecessors.get(node["id"])
            if previous is not None:
                if previous in active:
                    return False
                return self.target_independent_constant(self.constants[previous], active | {previous})
        return all(self.target_independent_constant(child, active) for child in children)

    def function(self, node: dict[str, Any], c_linkage: bool) -> None:
        if not (c_linkage or node.get("previousDecl") in self.c_functions):
            return  # C++ linkage: mangled, outside the C interface
        name = node.get("name")
        if not name:
            raise ValueError(f"{self.tu}: unnamed C-linkage function")
        self.c_functions[node["id"]] = name
        local = node.get("storageClass") == "static" or self.function_local.get(node.get("previousDecl"), False)
        self.function_local[node["id"]] = local
        body = any(child.get("kind") == "CompoundStmt" for child in node.get("inner", []))
        role = "definition" if body else "declaration"
        type_ = node["type"]
        if "desugaredQualType" in type_ or "typeAliasDeclId" in type_:
            # Spellings inside function-type sugar are written in another scope (a typedef's, an operand's).
            raise ValueError(f"{self.tu}: sugared C++ function type {type_['qualType']!r} of {name} is unsupported")
        returns, written = split_function_type(type_["qualType"])
        variadic = bool(written) and written[-1] == "..."
        written = written[:-1] if variadic else written
        if variadic != bool(node.get("variadic")):
            raise ValueError(f"{self.tu}: inconsistent variadic C++ declaration of {name}")
        parms = [child for child in node.get("inner", []) if child.get("kind") == "ParmVarDecl"]
        if len(parms) != len(written):
            raise ValueError(f"{self.tu}: {name} declares {len(written)} parameter(s) but has {len(parms)} parameter declarations")
        if "(" in returns or "[" in returns:
            raise ValueError(f"{self.tu}: unsupported return type {returns!r} of {name}")
        scope, complete = self.lookups[node["id"]]
        shape = Shape(self.written_kind(returns, scope, self.orders[node["id"]], complete),
                      tuple(self.classify_use(p["type"], parameter=True) for p in parms), variadic)
        file, line, column = self.location(node)
        text = f'extern "C" {returns} {name}({", ".join([*written, *(["..."] if variadic else [])])})'
        self.raw_sites.append(Site(name, role, file, line, column, text, shape, local=local))

    def object(self, node: dict[str, Any], linkage_extern: bool) -> None:
        name, emitted = node.get("name"), node.get("mangledName")
        if not emitted or node.get("storageClass") == "static":
            return  # automatic/local static, or explicit internal linkage
        # These are clang's AST identities only, never names used to compile/link the game.
        # Redeclarations (including block externs) retain the internal identity of their first declaration.
        if emitted != name and _internal_mangling(emitted):
            return
        if emitted != name:
            raise ValueError(f"{self.tu}: external C++ object {name} has unsupported mangled linkage")
        type_ = node["type"]
        category = self.classify_use(type_, parameter=False, object_type=True).category
        if category == "void":
            raise ValueError(f"{self.tu}: external object {name} has void type")
        role = "definition" if "init" in node or (node.get("storageClass") != "extern" and not linkage_extern) else "declaration"
        file, line, column = self.location(node)
        self.objects.append(ObjectSite(name, role, file, line, column, f"{type_['qualType']} {name}", category))

    # -- types by declaration identity
    @staticmethod
    def type_children(node: dict[str, Any]) -> list[dict[str, Any]]:
        return [child for child in node.get("inner", []) if str(child.get("kind", "")).endswith("Type")]

    def typedef_by_id(self, identity: str) -> Kind:
        """A typedef's kind from clang's type tree, every name in it bound by declaration id."""
        if identity in self.typedef_kinds:
            return self.typedef_kinds[identity]
        node = self.typedef_nodes.get(identity)
        if node is None or identity in self.resolving:
            raise ValueError(f"{self.tu}: unresolved C++ typedef identity {identity}")
        trees = self.type_children(node)
        if len(trees) != 1:
            raise ValueError(f"{self.tu}: C++ typedef {node.get('name')} has no single type tree")
        self.resolving.add(identity)
        try:
            kind = self.type_kind(trees[0])
        finally:
            self.resolving.discard(identity)
        self.typedef_kinds[identity] = kind
        return kind

    def type_kind(self, node: dict[str, Any]) -> Kind:
        kind, spelled = node.get("kind"), node.get("type", {}).get("qualType", "")
        children = self.type_children(node)
        if kind in ("ElaboratedType", "ParenType", "QualType") and len(children) == 1:
            return self.type_kind(children[0])
        if kind == "TypedefType":
            return self.typedef_by_id(node.get("decl", {}).get("id", ""))
        if kind == "BuiltinType":
            return self.builtin(spelled)
        if kind in CXX_DECLARATOR_TYPES:
            self.member_pointer_free(node, spelled)  # the outer declarator decides the category, not the subset
            return CXX_ARRAY if kind.endswith("ArrayType") else POINTER  # references pass/store an address
        if kind in ("RecordType", "EnumType"):
            identity = node.get("decl", {}).get("id", "")
            tag = self.tags.get(identity)
            if tag is None or tag.kind != ("record" if kind == "RecordType" else "enum"):
                raise ValueError(f"{self.tu}: unresolved C++ type identity {spelled!r}")
            return Kind(spelled, "aggregate", None) if tag.kind == "record" else self.enum_type(identity, spelled)
        if kind == "MemberPointerType":
            raise ValueError(f"{self.tu}: member pointer type {spelled!r} is unsupported")
        raise ValueError(f"{self.tu}: unclassifiable C++ type {spelled!r} ({kind})")

    def member_pointer_free(self, node: dict[str, Any], outer: str) -> None:
        """Refuse a member pointer anywhere below a type node: pointee, element, function parameter or
        return, and through typedef and typeof nodes, which carry their own type trees."""
        for child in self.type_children(node):
            if child.get("kind") == "MemberPointerType":
                raise ValueError(f"{self.tu}: member pointer type {child.get('type', {}).get('qualType')!r} "
                                 f"inside {outer!r} is unsupported")
            self.member_pointer_free(child, outer)

    def builtin(self, text: str) -> Kind:
        words = [word for word in text.split() if word not in CXX_QUALIFIERS]
        if len(words) == 1 and words[0] in CXX_INTEGERS:
            return CXX_INTEGERS[words[0]]
        if not words or not set(words) <= CXX_BUILTIN_WORDS or set(words) & CXX_INTEGERS.keys():
            raise ValueError(f"{self.tu}: unclassifiable C++ type {text!r}")
        return basic_kind(words)

    def enum_kind(self, identity: str) -> str:
        if identity not in self.enum_kinds:
            self.enum_kinds[identity] = self.enum_underlying(self.tags[identity].node)
        return self.enum_kinds[identity]

    def enum_type(self, identity: str, shown: str) -> Kind:
        return Kind(f"enum {shown}", "integer", 4, self.enum_kind(identity))

    def typedef_kind(self, resolved: Kind, parameter: bool, object_type: bool) -> Kind:
        if resolved.category == "array":
            if parameter:
                return POINTER
            if not object_type:
                raise ValueError(f"{self.tu}: array typedef in a return position")
        return resolved

    def structure(self, text: str, parameter: bool, object_type: bool) -> Kind | None:
        """Pointer, reference and array declarators of a spelling; None for a named type."""
        if "::*" in text:
            raise ValueError(f"{self.tu}: member pointer type {text!r} is unsupported")
        if CXX_OPAQUE_SPELLING.search(CXX_ANONYMOUS_TAG.sub("", text)):  # its operand's type is not spelled
            raise ValueError(f"{self.tu}: typeof/decltype type {text!r} has no type tree to check")
        # Parenthesized abstract declarators distinguish pointer-to-array from array-of-pointers.
        declarator = re.search(r"\(([*&]+)\s*(?:(?:const|volatile|__restrict|restrict)\s*)*((?:\[[^\]]*\])*)\)", text)
        array = bool(declarator[2]) if declarator else "[" in text
        if (declarator and not array) or text.endswith(("*", "&")):
            return POINTER  # references pass/store an address
        if array:
            if parameter:
                return POINTER
            if object_type:
                return CXX_ARRAY
            raise ValueError(f"{self.tu}: array type {text!r} in a return position")
        return None

    def classify_use(self, type_: dict[str, Any], parameter: bool, *, object_type: bool = False) -> Kind:
        """A parameter or object type: a typedef by its id, else clang's canonical spelling."""
        alias = type_.get("typeAliasDeclId")
        if alias is not None:
            if alias not in self.typedef_nodes:
                raise ValueError(f"{self.tu}: unresolved C++ typedef identity for {type_.get('qualType')!r}")
            return self.typedef_kind(self.typedef_by_id(alias), parameter, object_type)
        # Without a typedef id the type is no typedef: a builtin, a declarator, or a record or enum.
        text = _without_qualifiers(type_.get("desugaredQualType", type_["qualType"]))
        shaped = self.structure(text, parameter, object_type)
        if shaped is not None:
            return shaped
        keyword = _split_keyword(_without_qualifiers(type_["qualType"]))[0]
        name = _split_keyword(text)[1]
        anonymous = CXX_ANONYMOUS_TAG.fullmatch(name.split("::")[-1])
        if anonymous:  # clang names an unnamed tag by its location: only its kind is evident
            if anonymous[1] == "enum":
                raise ValueError(f"{self.tu}: unnamed C++ enum type {name!r} has no declaration identity")
            return Kind(name, "aggregate", None)
        bare = name.replace(CXX_ANONYMOUS_NAMESPACE, "")  # clang's spelling of an anonymous namespace scope
        words = bare.split()
        if "(" in bare:
            raise ValueError(f"{self.tu}: unclassifiable C++ type {text!r}")
        if keyword is None and set(words) <= CXX_BUILTIN_WORDS:
            return self.builtin(name)
        if len(words) != 1:
            raise ValueError(f"{self.tu}: unclassifiable C++ type {text!r}")
        if keyword in CXX_RECORD_KEYWORDS:
            return Kind(name, "aggregate", None)  # only a class can be named so
        return self.printed_tag(name, enum_only=keyword == "enum")

    def printed_tag(self, name: str, enum_only: bool) -> Kind:
        """A tag known by clang's printed name, which omits function scopes: every tag declaration it
        can denote (by printed name, or by short name where that is not derived) must agree."""
        last = name.split("::")[-1]
        tags = [identity for identity, tag in self.tags.items()
                if (tag.printed == name or tag.printed is None and last in tag.names)
                and (tag.kind == "enum" or not enum_only)]
        if not tags:
            raise ValueError(f"{self.tu}: unresolved C++ type identity {name!r}")
        kinds = {Kind(name, "aggregate", None) if self.tags[identity].kind == "record" else self.enum_type(identity, name)
                 for identity in tags}
        if len(kinds) != 1:
            raise ValueError(f"{self.tu}: ambiguous C++ type identity {name!r} ({len(tags)} tag declarations)")
        return kinds.pop()

    # -- written return types: name lookup through modelled scopes
    def written_kind(self, text: str, scope: str, at: int, complete: frozenset[str]) -> Kind:
        text = _without_qualifiers(text)
        shaped = self.structure(text, parameter=False, object_type=False)
        if shaped is not None:
            return shaped
        keyword, name = _split_keyword(text)
        words = name.split()
        if "(" in name:
            raise ValueError(f"{self.tu}: unclassifiable C++ type {text!r}")
        if keyword is None and set(words) <= CXX_BUILTIN_WORDS:
            return self.builtin(name)
        if len(words) != 1:
            raise ValueError(f"{self.tu}: unclassifiable C++ type {text!r}")
        if keyword in CXX_RECORD_KEYWORDS:
            return Kind(name, "aggregate", None)  # only a class can be named so
        entries = self.lookup(name, scope, at, complete, elaborated=keyword is not None)
        if keyword == "enum" and any(entry.kind != "enum" for entry in entries):
            raise ValueError(f"{self.tu}: enum {name} does not name an enum")
        return self.entry_kind(entries, name)

    def lookup(self, name: str, scope: str, at: int, complete: frozenset[str], *, elaborated: bool) -> list[_Entry]:
        """Bind the first qualifier lexically; member lookup must never retry an outer prefix."""
        parts = name.removeprefix("::").split("::")
        if not all(parts):
            raise ValueError(f"{self.tu}: unsupported C++ type name {name}")
        final = CXX_TYPE_ENTRIES if elaborated else CXX_ORDINARY_ENTRIES
        accept = CXX_QUALIFIER_ENTRIES if len(parts) > 1 else final
        if name.startswith("::"):
            entries = self.visible("", parts[0], at, complete, accept)
            if not entries:
                raise ValueError(f"{self.tu}: unresolved C++ type {name}")
        else:
            entries = self.unqualified(parts[0], scope, at, complete, accept, name)
        for index, member in enumerate(parts[1:], 2):
            target = self.qualifier(entries, parts[index - 2], name)
            entries = self.visible(target, member, at, complete, final if index == len(parts) else CXX_QUALIFIER_ENTRIES)
            if not entries:
                if target in self.inherited_scopes:
                    raise ValueError(f"{self.tu}: inherited type lookup for {name} in {target} is unsupported")
                raise ValueError(f"{self.tu}: unresolved C++ type {name} in {target}")
        return entries

    def unqualified(self, name: str, scope: str, at: int, complete: frozenset[str], accept: frozenset[str],
                    written: str) -> list[_Entry]:
        while True:
            entries = self.visible(scope, name, at, complete, accept)
            if entries:
                return entries
            if scope in self.inherited_scopes:
                raise ValueError(f"{self.tu}: inherited type lookup for {written} in {scope} is unsupported")
            if not scope:
                raise ValueError(f"{self.tu}: unresolved C++ type {written}")
            scope = self.enclosing(scope)

    def visible(self, scope: str, name: str, at: int, complete: frozenset[str], accept: frozenset[str]) -> list[_Entry]:
        """Declarations of `name` in one scope at AST position `at`. A class shows all its members in a
        complete-class context; elsewhere a later member would change the meaning (ill-formed, no
        diagnostic required), so it refuses. A namespace also shows its anonymous namespace's members
        (clang's implicit using-directive). A tag only a friend declaration has declared is never found
        (C++98 7.3.1.2/3), but pre-standard friend injection made it visible: a lookup that would pass
        over one refuses instead of binding an outer name."""
        entries = self.entries.get(scope, {}).get(name, [])
        if scope in self.class_scopes:
            if scope not in complete and any(entry.order > at for entry in entries):
                raise ValueError(f"{self.tu}: C++ class member {name} of {scope} is declared after its use")
            declared = entries
        else:
            declared = [entry for entry in entries if entry.order < at]
        found = [entry for entry in declared if entry.kind in accept]
        anonymous = f"{scope}{CXX_ANONYMOUS_NAMESPACE}::"
        if anonymous in self.namespace_scopes:
            found += self.visible(anonymous, name, at, complete, accept)
        if not found and any(entry.kind == "friend" for entry in declared):
            raise ValueError(f"{self.tu}: C++ name {name} in {scope or '::'} is only declared by a friend declaration; "
                             "lookup past it is unsupported")
        return found

    def qualifier(self, entries: list[_Entry], component: str, written: str) -> str:
        """The one namespace or class a nested-name-specifier component names."""
        kinds = {entry.kind for entry in entries}
        if kinds == {"namespace"}:
            scopes = {entry.ref for entry in entries}
        elif kinds == {"record"}:
            scopes = {self.tags[entry.ref].members for entry in entries}
        else:  # a typedef keeps its ABI kind, not a member scope: never read its members elsewhere
            raise ValueError(f"{self.tu}: unsupported C++ type qualifier {component} in {written}")
        if len(scopes) != 1:
            raise ValueError(f"{self.tu}: ambiguous C++ type qualifier {component} in {written}")
        return scopes.pop()

    def entry_kind(self, entries: list[_Entry], written: str) -> Kind:
        kinds: list[Kind] = []
        for entry in entries:
            if entry.kind == "typedef":
                kinds.append(self.typedef_by_id(entry.ref))
            elif entry.kind == "record":
                kinds.append(Kind(self.tags[entry.ref].printed or written, "aggregate", None))
            elif entry.kind == "enum":
                kinds.append(self.enum_type(entry.ref, self.tags[entry.ref].printed or written))
            else:
                raise ValueError(f"{self.tu}: C++ name {written} does not denote a type")
        if len({(kind.category, kind.compared) for kind in kinds}) != 1:
            raise ValueError(f"{self.tu}: ambiguous C++ type name {written}")
        return kinds[0]

    # -- pass 2: direct calls of C-linkage functions
    def calls(self, node: dict[str, Any]) -> None:
        if node.get("kind") == "CallExpr":
            children = node.get("inner", [])
            callee = children[0] if children else None
            while callee is not None and (callee.get("kind") in ("ImplicitCastExpr", "ParenExpr", "CStyleCastExpr",
                                                                "CXXStaticCastExpr", "CXXReinterpretCastExpr")
                                          or callee.get("kind") == "UnaryOperator" and callee.get("opcode") in ("*", "&")):
                callee = (callee.get("inner") or [None])[0]
            target = callee.get("referencedDecl", {}) if callee is not None and callee.get("kind") == "DeclRefExpr" else {}
            if target.get("kind") == "FunctionDecl" and target.get("id") in self.c_functions:
                file, line, column = self.location(node)
                self.raw_calls.append(Call(self.c_functions[target["id"]], file, line, column, len(children) - 1,
                                           local=self.function_local[target["id"]]))
        for child in node.get("inner", []):
            self.calls(child)

    def result(self, profile: str | None) -> TUResult:
        return TUResult(self.tu, profile, self.raw_sites, self.raw_calls, objects=self.objects)


def parse_cxx_tu(preprocessed: str, tu: str, profile: str | None, command: list[str] | None, *,
                 environment: dict[str, str] | None = None, unavailable: str | None = None) -> TUResult:
    """Collect C-linkage sites and direct calls from one preprocessed C++ TU through the pinned clang."""
    if command is None:
        return TUResult(tu, profile, error=f"parse: C++ AST unavailable: {unavailable}")
    try:
        completed = subprocess.run(command, input=preprocessed.encode("latin-1"), env=environment, capture_output=True)
        if completed.returncode != 0:
            locate = _LineMap(preprocessed, tu)
            errors = [CLANG_LOCATION.sub(lambda m: "{}:{}".format(*locate(locate.starts[int(m[1]) - 1])[:2]), line)
                      for line in completed.stderr.decode("latin-1").splitlines() if "error:" in line]
            return TUResult(tu, profile, error=f"parse: clang exit {completed.returncode}: {'; '.join(errors[:5])}")
        collector = _CxxCollector(tu, preprocessed)
        ast = json.loads(completed.stdout)
        collector.index(ast)
        collector.declarations(ast)
        collector.calls(ast)
        return collector.result(profile)
    except Exception as error:  # every failure becomes a parse-failure finding
        return TUResult(tu, profile, error=f"parse: {type(error).__name__}: {error}")


# ---------------------------------------------------------------- collection

@dataclass(frozen=True)
class TranslationUnit:
    tu: str
    argument: str  # path handed to the driver, relative to the work directory
    profile: str | None
    includes: tuple[str, ...] = ()
    candidate: int | None = None
    error: str | None = None
    cxx: bool = False  # a C++ (.cpp) TU, analysed through the pinned clang


def translation_units(profile: dict[str, Any], source_root: Path, extra: Iterable[dict[str, Any]]) -> list[TranslationUnit]:
    units = [TranslationUnit(source, f"{WORK_LINK}/{source}", key, cxx=profile_kind(profile, key) == "cpp")
             for source, key in sorted(profile["tu_profiles"].items())]
    for index, item in enumerate(extra):
        path = Path(item["source"]).resolve()
        tu = display(str(path))  # as its linemarkers display
        key = item.get("profile") or profile["tu_profiles"].get(display(str(path), source_root))
        includes = tuple(str(Path(p).resolve()) for p in item.get("includes") or [])
        cxx = path.suffix == COMPILED_SUFFIXES["cpp"]
        error = None
        if not path.is_file():
            error = f"missing candidate source {path}"
        elif path.suffix not in COMPILED_SUFFIXES.values():
            error = f"candidate source must be .c or .cpp: {path.name}"
        elif key is None:
            error = "candidate needs a profile"
        elif key not in profile["profiles"]:
            error = f"unknown profile {key}"
        elif profile_kind(profile, key) != compiled_kind(path.name):
            error = f"profile {key} does not compile {path.suffix} sources"
        units.append(TranslationUnit(tu, str(path), key, includes, index, error, cxx))
    return units


def preprocess_and_parse(unit: TranslationUnit, profile: dict[str, Any], work: Path, environment: dict[str, str],
                         cxx: tuple[list[str] | None, str | None] = (None, "no C++ AST tool")) -> TUResult:
    if unit.error:
        result = TUResult(unit.tu, unit.profile, error=f"input: {unit.error}")
    else:
        driver, _ = profile_toolchain(profile, unit.profile)
        flags = [item for path in unit.includes for item in ("-I", path)] + profile["profiles"][unit.profile]["compiler_flags"]
        completed = subprocess.run([*driver, *flags, "-E", unit.argument], cwd=work, env=environment, capture_output=True)
        if completed.returncode != 0:
            stderr = completed.stderr.decode("latin-1").strip()
            result = TUResult(unit.tu, unit.profile, error=f"preprocess: exit {completed.returncode}: {stderr}")
        elif unit.cxx:
            result = parse_cxx_tu(completed.stdout.decode("latin-1"), unit.tu, unit.profile, cxx[0],
                                  environment=environment, unavailable=cxx[1])
        else:
            result = parse_tu(completed.stdout.decode("latin-1"), unit.tu, unit.profile)
    result.candidate = unit.candidate
    return result


def collect(source_root: Path = PROJECT, extra: Iterable[dict[str, Any]] = (), *, jobs: int | None = None) -> list[TUResult]:
    """Preprocess (threaded) and parse every canonical TU, then every extra candidate, in order.

    The pinned clang is located and hash-checked once, only when a C++ TU takes part."""
    source_root = Path(source_root).resolve()
    profile = tool_profile(PROJECT, source_root)
    units = translation_units(profile, source_root, extra)
    cxx = clang_ast_command(PROJECT, source_root) if any(unit.cxx and not unit.error for unit in units) else (None, None)
    with tempfile.TemporaryDirectory(prefix="interfaces-") as directory:
        work = Path(directory)
        (work / WORK_LINK).symlink_to(source_root, target_is_directory=True)
        (work / "tmp").mkdir()
        environment = compiler_environment(work, profile)
        with ThreadPoolExecutor(max_workers=jobs or os.cpu_count() or 4) as pool:
            return list(pool.map(lambda unit: preprocess_and_parse(unit, profile, work, environment, cxx), units))


# ---------------------------------------------------------------- analysis

@dataclass
class Finding:
    kind: str
    symbol: str
    detail: str
    site: dict[str, Any] | None = None
    reference: dict[str, Any] | None = None
    differences: list[dict[str, Any]] = field(default_factory=list)
    locations: list[dict[str, Any]] = field(default_factory=list)
    tus: set[str] = field(default_factory=set)
    key: tuple[Any, ...] = ()

    def to_json(self) -> dict[str, Any]:
        record = {"kind": self.kind, "symbol": self.symbol, "detail": self.detail, "site": self.site,
                  "reference": self.reference, "differences": self.differences, "tus": sorted(self.tus)}
        if self.locations:
            record["locations"] = self.locations
        return record

    def summary(self) -> str:
        return f"{self.kind} {self.symbol}: {self.detail}"


@dataclass
class _Seen:
    item: Site | Call | ObjectSite
    tus: set[str]


@dataclass
class SymbolRecord:
    symbol: str
    scope: str  # "external" or the TU of a static function
    sites: list[_Seen] = field(default_factory=list)
    calls: list[_Seen] = field(default_factory=list)
    findings: list[Finding] = field(default_factory=list)

    @property
    def definitions(self) -> list[_Seen]:
        return sorted((s for s in self.sites if s.item.role == "definition"), key=lambda s: _order(s.item))

    def groups(self) -> dict[tuple[Any, ...], list[_Seen]]:
        result: dict[tuple[Any, ...], list[_Seen]] = defaultdict(list)
        for seen in sorted(self.sites, key=lambda s: _order(s.item)):
            result[seen.item.shape.signature].append(seen)
        return result

    def authority(self) -> dict[str, Any]:
        """The authoritative shape: the C definition, else the agreed (most widely seen prototyped) declaration."""
        definitions = self.definitions
        if definitions:
            return {"authority": "definition", "shape": definitions[0].item.shape, "source": definitions[0]}
        groups = self.groups()
        prototyped = {sig: seen for sig, seen in groups.items() if sig[1] is not None}
        if len(prototyped) > 1:
            return {"authority": "disagreement", "shape": None, "source": None}
        pool = next(iter(prototyped.values()), None) or [s for seen in groups.values() for s in seen]
        shapes = Counter(s.item.shape for s in pool for _ in s.tus)
        best = max(pool, key=lambda s: shapes[s.item.shape])  # earliest among the most widely seen shapes
        return {"authority": "declarations" if prototyped else "unprototyped", "shape": best.item.shape, "source": best}


@dataclass
class ObjectRecord:
    symbol: str
    sites: list[_Seen] = field(default_factory=list)
    findings: list[Finding] = field(default_factory=list)


def _order(site: Site | Call | ObjectSite) -> tuple[str, int, int]:
    return (site.file, site.line, site.column)


def _site_text(seen: _Seen) -> str:
    return f"{seen.item.location()} `{seen.item.text}`"


@dataclass
class Report:
    findings: list[Finding]
    symbols: dict[tuple[str, str], SymbolRecord]
    results: list[TUResult]
    seconds: float = 0.0
    objects: dict[str, ObjectRecord] = field(default_factory=dict)

    def counts(self) -> dict[str, int]:
        counts = Counter(f.kind for f in self.findings)
        return {kind: counts[kind] for kind in KINDS if counts[kind]}

    def registry(self) -> dict[str, Any]:
        symbols = {}
        for (scope, symbol), record in sorted(self.symbols.items(), key=lambda item: item[0][1]):
            if scope != "external":
                continue
            authority = record.authority()
            shape, source = authority["shape"], authority["source"]
            symbols[symbol] = {
                "authority": authority["authority"],
                "shape": shape.to_json() if shape else None,
                "prototype": shape.render(symbol) if shape else None,
                "source": source.item.to_json() if source else None,
                "sites": len(record.sites), "calls": sum(len(c.tus) for c in record.calls),
                "variants": [{"shape": seen[0].item.shape.to_json(), "sites": [s.item.to_json() for s in seen]}
                             for seen in record.groups().values()] if authority["authority"] == "disagreement" else [],
                "disagreements": [f.to_json() for f in record.findings],
            }
        objects = {
            symbol: {"categories": sorted({s.item.category for s in record.sites}),
                     "sites": [{**s.item.to_json(), "tus": sorted(s.tus)} for s in sorted(record.sites, key=lambda s: _order(s.item))],
                     "disagreements": [f.to_json() for f in record.findings]}
            for symbol, record in sorted(self.objects.items())
        }
        return {"scope": "External functions and objects declared/defined by the audited TUs, plus direct calls; static symbols are TU-local",
                "tus": len(self.results), "symbols": symbols, "objects": objects}


def _finding(findings: dict[tuple[Any, ...], Finding], record: SymbolRecord | ObjectRecord | None, finding: Finding) -> None:
    existing = findings.get(finding.key)
    if existing is not None:
        existing.tus |= finding.tus
        return
    findings[finding.key] = finding
    if record is not None:
        record.findings.append(finding)


def analyse(results: Iterable[TUResult]) -> Report:
    """Compare function sites/direct calls and external object categories across the given TU results."""
    results = list(results)
    records: dict[tuple[str, str], SymbolRecord] = {}
    objects: dict[str, ObjectRecord] = {}
    seen_sites: dict[tuple[Any, ...], _Seen] = {}
    findings: dict[tuple[Any, ...], Finding] = {}
    for result in results:
        if result.error is not None:
            _finding(findings, None, Finding("parse-failure", result.tu, result.error, tus={result.tu},
                                             key=("parse-failure", result.tu)))
            continue
        for item in [*result.sites, *result.calls]:
            scope = result.tu if item.local else "external"
            record = records.setdefault((scope, item.symbol), SymbolRecord(item.symbol, scope))
            key = (scope, item.symbol, item.key)
            if key in seen_sites:
                seen_sites[key].tus.add(result.tu)
                continue
            seen_sites[key] = _Seen(item, {result.tu})
            (record.calls if isinstance(item, Call) else record.sites).append(seen_sites[key])
        for item in result.objects:
            record = objects.setdefault(item.symbol, ObjectRecord(item.symbol))
            key = ("object", item.symbol, item.key)
            if key in seen_sites:
                seen_sites[key].tus.add(result.tu)
                continue
            seen_sites[key] = _Seen(item, {result.tu})
            record.sites.append(seen_sites[key])

    for record in records.values():
        symbol = record.symbol
        for seen in record.sites:
            if seen.item.shape.params is None:
                what = {"implicit": "implicit declaration (no prototype in scope)", "definition": "definition without prototype",
                        "declaration": "declaration without prototype"}[seen.item.role]
                _finding(findings, record, Finding("unprototyped", symbol, f"{what} at {_site_text(seen)}",
                                                   site=seen.item.to_json(), tus=set(seen.tus),
                                                   key=("unprototyped", symbol, seen.item.key)))
        definitions = record.definitions
        if definitions:
            reference = definitions[0]
            for seen in record.sites:
                if seen is reference:
                    continue
                differences = compare(reference.item.shape, seen.item.shape)
                if differences:
                    _finding(findings, record, Finding(
                        "declaration-vs-definition", symbol,
                        f"{_site_text(seen)} vs C definition {_site_text(reference)}: {describe(differences)}",
                        site=seen.item.to_json(), reference=reference.item.to_json(), differences=differences,
                        tus=seen.tus | reference.tus, key=("declaration-vs-definition", symbol, seen.item.key, reference.item.key)))
            shape = reference.item.shape
            fixed = len(shape.params) if shape.params is not None else shape.arity
            for seen in record.calls:
                call = seen.item
                if fixed is None or call.arguments == fixed or (shape.variadic and call.arguments >= fixed):
                    continue
                takes = f"{fixed}{' or more' if shape.variadic else ''}"
                _finding(findings, record, Finding(
                    "call-arity", symbol,
                    f"call at {call.file}:{call.line} passes {call.arguments} argument(s); C definition {_site_text(reference)} takes {takes}",
                    site=call.to_json(), reference=reference.item.to_json(),
                    differences=[{"aspect": "arguments", "subkind": "arity", "expected": takes, "found": call.arguments}],
                    tus=seen.tus | reference.tus, key=("call-arity", symbol, call.key, reference.item.key)))
            continue
        # Assembly callee: every pair of conflicting declaration shapes, the most widely seen shape first.
        groups = sorted(record.groups().items(), key=lambda g: (-sum(len(s.tus) for s in g[1]), repr(g[0])))
        for first, (expected_signature, expected) in enumerate(groups):
            for found_signature, found in groups[first + 1:]:
                differences = compare(expected[0].item.shape, found[0].item.shape)
                if not differences:
                    continue
                locations = [{**s.item.to_json(), "group": index} for index, group in enumerate((expected, found)) for s in group]
                _finding(findings, record, Finding(
                    "declaration-disagreement", symbol,
                    f"no C definition; {len(expected)} site(s) like {_site_text(expected[0])} vs {len(found)} site(s) like "
                    f"{_site_text(found[0])}: {describe(differences)}",
                    site=found[0].item.to_json(), reference=expected[0].item.to_json(), differences=differences,
                    locations=locations, tus=set().union(*(s.tus for s in expected + found)),
                    key=("declaration-disagreement", symbol, "" if record.scope == "external" else record.scope,
                         *sorted([expected_signature, found_signature], key=repr))))
    for record in objects.values():
        sites = sorted(record.sites, key=lambda s: _order(s.item))
        categories: dict[str, list[_Seen]] = defaultdict(list)
        for seen in sites:
            categories[seen.item.category].append(seen)
        for expected, found, subkind in OBJECT_CONFLICTS:
            if expected not in categories or found not in categories:
                continue
            reference, site = categories[expected][0], categories[found][0]
            _finding(findings, record, Finding(
                "object-category", record.symbol,
                f"{subkind}; " + "; ".join(f"{s.item.category} at {_site_text(s)}" for s in sites),
                site=site.item.to_json(), reference=reference.item.to_json(),
                differences=[{"aspect": "object category", "subkind": subkind, "expected": expected, "found": found}],
                locations=[{**s.item.to_json(), "tus": sorted(s.tus)} for s in sites],
                tus=set().union(*(s.tus for s in sites)),
                key=("object-category", record.symbol, expected, found)))
    ordered = sorted(findings.values(), key=lambda f: (KINDS.index(f.kind), f.symbol, f.detail))
    return Report(ordered, records, results, objects=objects)


def describe(differences: list[dict[str, Any]]) -> str:
    return "; ".join(f"{d['aspect']} {d['expected']} vs {d['found']}" for d in differences)


def audit(source_root: Path = PROJECT, extra: Iterable[dict[str, Any]] = (), *, jobs: int | None = None) -> Report:
    """Audit every TU of `tool_profile(...)["tu_profiles"]` under `source_root` plus `extra` candidates."""
    started = time.monotonic()
    report = analyse(collect(source_root, extra, jobs=jobs))
    report.seconds = time.monotonic() - started
    return report


class Gate:
    """Sequential adoption gate over `collect(...)` results: canonical TUs plus indexed candidates.

    `check(i)` audits candidate i with the canonical TUs and the candidates admitted so far and
    returns every finding absent from that audit without it (joining one side of an existing
    disagreement introduces nothing); `admit(i)` adds candidate i once it is adopted.
    """

    def __init__(self, results: list[TUResult]) -> None:
        self.candidates = {r.candidate: r for r in results if r.candidate is not None}
        self.admitted = [r for r in results if r.candidate is None]
        self.previous = {f.key for f in analyse(self.admitted).findings}

    def check(self, index: int) -> list[Finding]:
        return [f for f in analyse([*self.admitted, self.candidates[index]]).findings if f.key not in self.previous]

    def admit(self, index: int) -> None:
        self.admitted.append(self.candidates[index])
        self.previous = {f.key for f in analyse(self.admitted).findings}


# ---------------------------------------------------------------- CLI

def prototype_lines(report: Report, symbol: str) -> tuple[list[str], bool]:
    record = report.symbols.get(("external", symbol))
    if record is None:
        local = [r for (_, name), r in report.symbols.items() if name == symbol]
        if not local:
            return [f"{symbol}: not declared, defined or called by any audited TU"], False
        lines = []
        for item in local:
            authority = item.authority()
            lines.append(f"{symbol}: static in {item.scope}; " + (
                f"{authority['shape'].render(symbol)} from {_site_text(authority['source'])}" if authority["shape"] else "disagreeing"))
        return lines, True
    authority = record.authority()
    if authority["shape"] is None:
        lines = [f"{symbol}: DISAGREEING declarations, no C definition"]
        for seen in record.groups().values():
            lines.append(f"    {seen[0].item.shape.render(symbol)}  [{len(seen)} site(s)] e.g. {_site_text(seen[0])}")
        return lines, False
    source = authority["source"]
    label = {"definition": "C definition", "declarations": f"agreed declaration ({len(record.sites)} site(s))",
             "unprototyped": "UNPROTOTYPED declarations only"}[authority["authority"]]
    lines = [f"{symbol}: {label} {source.item.location()}", f"    {source.item.text}",
             f"    shape: {authority['shape'].render(symbol)}"]
    lines += [f"    finding: {f.summary()}" for f in record.findings]
    return lines, authority["authority"] != "unprototyped"


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--source-root", type=Path, default=PROJECT)
    parser.add_argument("--extra", type=Path, help="JSON list of {source, profile[, includes]} candidates")
    parser.add_argument("--json", action="store_true", help="print the findings as JSON")
    parser.add_argument("--prototype", nargs="+", metavar="SYMBOL", help="print authoritative prototype shapes")
    parser.add_argument("--registry", type=Path, metavar="OUT.json", help="write every referenced symbol's shape")
    parser.add_argument("--jobs", type=int)
    args = parser.parse_args(argv)
    extra = json.loads(args.extra.read_text()) if args.extra else []
    report = audit(args.source_root, extra, jobs=args.jobs)
    if args.registry:
        args.registry.write_text(json.dumps(report.registry(), indent=2) + "\n")
    if args.prototype:
        ok = True
        for symbol in args.prototype:
            lines, known = prototype_lines(report, symbol)
            print("\n".join(lines))
            ok &= known
        return 0 if ok else 1
    summary = {"tus": len(report.results), "findings": len(report.findings), "counts": report.counts(),
               "seconds": round(report.seconds, 2)}
    if args.json:
        print(json.dumps({**summary, "findings": [f.to_json() for f in report.findings]}, indent=2))
    else:
        for finding in report.findings:
            print(finding.summary())
        counts = ", ".join(f"{count} {kind}" for kind, count in summary["counts"].items()) or "none"
        print(f"{summary['findings']} finding(s) ({counts}) over {summary['tus']} TU(s) in {summary['seconds']} s")
    return 1 if report.findings else 0


if __name__ == "__main__":
    sys.exit(main())
