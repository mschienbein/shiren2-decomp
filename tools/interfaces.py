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
"""
from __future__ import annotations

import argparse
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
from typing import Any, Iterable

from pycparser import c_ast, c_generator, c_parser

from certification import compiler_environment, profile_toolchain, tool_profile
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
    else int) differs from the other side's integer or enum type. Pointers are compatible
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


# ---------------------------------------------------------------- collection

@dataclass(frozen=True)
class TranslationUnit:
    tu: str
    argument: str  # path handed to the driver, relative to the work directory
    profile: str | None
    includes: tuple[str, ...] = ()
    candidate: int | None = None
    error: str | None = None


def translation_units(profile: dict[str, Any], source_root: Path, extra: Iterable[dict[str, Any]]) -> list[TranslationUnit]:
    units = [TranslationUnit(source, f"{WORK_LINK}/{source}", key) for source, key in sorted(profile["tu_profiles"].items())]
    for index, item in enumerate(extra):
        path = Path(item["source"]).resolve()
        tu = display(str(path))  # as its linemarkers display
        key = item.get("profile") or profile["tu_profiles"].get(display(str(path), source_root))
        includes = tuple(str(Path(p).resolve()) for p in item.get("includes") or [])
        error = None
        if not path.is_file():
            error = f"missing candidate source {path}"
        elif key is None:
            error = "candidate needs a profile"
        elif key not in profile["profiles"]:
            error = f"unknown profile {key}"
        units.append(TranslationUnit(tu, str(path), key, includes, index, error))
    return units


def preprocess_and_parse(unit: TranslationUnit, profile: dict[str, Any], work: Path, environment: dict[str, str]) -> TUResult:
    if unit.error:
        result = TUResult(unit.tu, unit.profile, error=f"input: {unit.error}")
    else:
        driver, _ = profile_toolchain(profile, unit.profile)
        flags = [item for path in unit.includes for item in ("-I", path)] + profile["profiles"][unit.profile]["compiler_flags"]
        completed = subprocess.run([*driver, *flags, "-E", unit.argument], cwd=work, env=environment, capture_output=True)
        if completed.returncode != 0:
            stderr = completed.stderr.decode("latin-1").strip()
            result = TUResult(unit.tu, unit.profile, error=f"preprocess: exit {completed.returncode}: {stderr}")
        else:
            result = parse_tu(completed.stdout.decode("latin-1"), unit.tu, unit.profile)
    result.candidate = unit.candidate
    return result


def collect(source_root: Path = PROJECT, extra: Iterable[dict[str, Any]] = (), *, jobs: int | None = None) -> list[TUResult]:
    """Preprocess (threaded) and parse every canonical TU, then every extra candidate, in order."""
    source_root = Path(source_root).resolve()
    profile = tool_profile(PROJECT, source_root)
    units = translation_units(profile, source_root, extra)
    with tempfile.TemporaryDirectory(prefix="interfaces-") as directory:
        work = Path(directory)
        (work / WORK_LINK).symlink_to(source_root, target_is_directory=True)
        (work / "tmp").mkdir()
        environment = compiler_environment(work, profile)
        with ThreadPoolExecutor(max_workers=jobs or os.cpu_count() or 4) as pool:
            return list(pool.map(lambda unit: preprocess_and_parse(unit, profile, work, environment), units))


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
