"""The three user-approved, verbatim GCC runtime TUs; never game C/C++ credit.

Pins are trusted tool policy, not caller-supplied exemptions. See
LIBGCC-UPSTREAM-ASM (2026-10-10). GCC source commit:
a6afc2afbcaf6682930141d526afdc95801fc2fd, pmret/gcc-papermario.
"""
from __future__ import annotations

import hashlib
import re
from pathlib import Path
from typing import Any, Iterable

CATEGORY = "toolchain library (upstream source)"
FAMILY = "toolchain_library"
PROFILE = "gcc281pm-gnu291-O2-unsigned-asg"
VENDOR = "include/toolchain/gcc281pm"
# symbol, original selection, original name, ROM, VRAM, instruction size, wrapper SHA256
UNITS = {
    "src/units/resident/func_80035F50.c": ("func_80035F50", "L_divdi3", "__divdi3", 0x11350, 0x80035F50, 1468, "6f905b7c771fd81723e221a27aae0655a9b9c605181e933970088955432f1d0a"),
    "src/units/resident/func_80036510.c": ("func_80036510", "L_udivdi3", "__udivdi3", 0x11910, 0x80036510, 1364, "8249c2f1990d36c63850618055e8a20fe31b9d8610c579d7f7d821aa495cf73c"),
    "src/units/resident/func_80036A70.c": ("func_80036A70", "L_umoddi3", "__umoddi3", 0x11E70, 0x80036A70, 1228, "12b32b93423277e59494b330dda406f2ef0ef9bbc3d315dd3430b152428b0eb4"),
}
# All bytes in the dependency closure, not merely the line containing multu.
# tm.h/tconfig.h are minimal equivalent target selectors; the rest are verbatim.
PINS = {
    "libgcc2.c": "095c1a9489e86c26becd8dc3804110b77db9662e1b10bef9ff2cbce4d5b78210",
    "longlong.h": "82e238cb04f2d1a9fcd105d79bd9f67145b8fcd6ca641494d3759bbd48d17362",
    "machmode.h": "9abdff16a106d3ce151586477b86069edfc995b0d071cc165b117a67805d8f3b",
    "machmode.def": "aab7a17fdceeeb8715274b94676e17bddc3e1d12dcea4ab169c08886b77fd9c3",
    "gansidecl.h": "6766403d59516c94e802bb4f794c001bc62e9742199c163a79860f530c6c64d2",
    "defaults.h": "6efad62553e0af5eb6041841b38efa2798b3617816ff0ab81091467cacb4a433",
    "stddef.h": "4f064b1fdac69580f69029d7b06bd581d2bea07d326102d7776f5dc998e450d5",
    "mips/xm-sysv.h": "aca54f48b353249c2755da116b8119741b8315c97cd342a8f0b105c3332cc11e",
    "mips/xm-mips.h": "a9e84ceb745a73b7f7e48c1f74b549dcbbd772959626c5cdbf9d67d5435d04d0",
    "mips/nu64.h": "68830218a9cc2f5620f20aa2f3277a2aadaa50af0ac1b0a1960bec0d83d9bd73",
    "mips/gnu.h": "a4afa6602d54fe587bb2956d6f3a0730010b6c21ad51c32c24d153129628149b",
    "mips/mips.h": "0cc0dd7caa46a725672920632fe7957ff78e8d304e41a67f9d52e92a77eb765b",
    "gnu.h": "1564e7c6d91fb130a147f85645b28960f8bafd816e2b72e8de7ec5f0dcb0c8c6",
    "svr4.h": "d7c38a2eef190e9a8c4ef68cb1b73211b191c8ee82a62baea5ef5c827bc5cd6e",
    "dbx.h": "2798df46f56dc5c914be5c4eecf421fe8e03edcf1187d2de9c4cb2d5d5ac094d",
    "tconfig.h": "6da1245e5da039b6548b6bf9fc0245c6263b16d8c1615becffac329a851cad65",
    "tm.h": "22b0bbe85443fc8e088360719dcc6f7ccaabc781d20758389b5da98cf6e05fab",
    "COPYING": "dcc100d4161cc0b7177545ab6e47216f84857cda3843847c792a25289852dcaa",
}
COMPILER_FLAGS = ["-O2", "-mips3", "-mgp32", "-mfp32", "-G", "0", "-mno-abicalls", "-fno-PIC", "-fno-builtin", "-funsigned-char", "-nostdinc", "-I", "source/include"]
ASSEMBLER_FLAGS = ["-mips3", "-mcpu=r3000", "-mfp32", "-32", "-non_shared", "-EB", "-G", "0", "-I", "source/include", "-g"]
LINEMARKER = re.compile(r'^# \d+ "([^"\n]+)"', re.MULTILINE)


def source_name(root: Path, source: Path) -> str:
    try:
        return source.resolve().relative_to(root.resolve()).as_posix()
    except ValueError:
        return ""


def _require_hash(root: Path, name: str, digest: str) -> None:
    path = root
    for part in Path(name).parts:
        path /= part
        if path.is_symlink():
            raise ValueError(f"Upstream libgcc input is a symlink: {name}")
    if not path.is_file() or hashlib.sha256(path.read_bytes()).hexdigest() != digest:
        raise ValueError(f"Changed or missing pinned upstream libgcc input: {name}")


def validate_vendor(root: Path) -> None:
    for name, digest in PINS.items():
        _require_hash(root, f"{VENDOR}/{name}", digest)
    actual = {p.relative_to(root / VENDOR).as_posix() for p in (root / VENDOR).rglob("*") if p.is_file() or p.is_symlink()}
    if actual != set(PINS):
        raise ValueError("Unpinned file in upstream libgcc dependency directory")


def validate_source(root: Path, source: str) -> None:
    if source not in UNITS:
        raise ValueError(f"Upstream libgcc is restricted to exactly three TUs: {source}")
    _require_hash(root, source, UNITS[source][6])
    validate_vendor(root)


def file_exception(root: Path, path: Path) -> bool:
    """Only exact pinned files can bypass lexical rules; TU consumers are checked separately."""
    name = source_name(root, path)
    if name in UNITS:
        validate_source(root, name)
        return True
    if name.startswith(VENDOR + "/"):
        validate_vendor(root)
        return True
    return False


def validate_dependencies(root: Path, source: str, dependencies: Iterable[str]) -> bool:
    names = set(dependencies)
    protected = {f"{VENDOR}/{name}" for name in PINS}
    if source not in UNITS and not names & protected:
        return False
    validate_source(root, source)
    expected = {source, *(f"{VENDOR}/{name}" for name in PINS if name != "COPYING")}
    if names != expected:
        raise ValueError(f"Upstream libgcc dependency closure differs: missing={sorted(expected - names)}, extra={sorted(names - expected)}")
    return True


def participating_files(preprocessed: str, work: Path) -> list[Path]:
    return sorted({(work / name).resolve() for name in LINEMARKER.findall(preprocessed) if not name.startswith("<")})


def validate_expansion(root: Path, source: Path, profile: str, participating: Iterable[Path]) -> bool:
    name = source_name(root, source)
    paths = list(participating)
    dependencies = [source_name(root, path) for path in paths]
    permitted = validate_dependencies(root, name, dependencies)
    if permitted and profile != PROFILE:
        raise ValueError("Upstream libgcc requires its exact measured profile")
    return permitted


def extra_flags(source: str) -> list[str]:
    # Include lookup only. No altered optimisation, codegen, assembler, or upstream macro.
    return ["-I", f"source/{VENDOR}"] if source in UNITS else []


def validate_bindings(declaration: dict[str, Any]) -> None:
    bindings = declaration["tu_profiles"]
    for source, key in bindings.items():
        if source in UNITS and key != PROFILE:
            raise ValueError(f"Upstream libgcc profile binding changed: {source}")
        if source.startswith("src/units/resident/") and key == PROFILE and source not in UNITS:
            raise ValueError(f"Resident libgcc profile cannot extend to a fourth TU: {source}")
    if set(bindings) & set(UNITS):
        recipe = declaration["profiles"][PROFILE]
        if (recipe.get("compiler") != "gcc281pm" or recipe["assembler"] != "as_gnu291_fp32"
                or recipe["compiler_flags"] != COMPILER_FLAGS or recipe["assembler_flags"] != ASSEMBLER_FLAGS
                or recipe.get("language", "c") != "c"):
            raise ValueError("Upstream libgcc measured recipe changed")


def match_family(match: dict[str, Any]) -> str:
    """Identity-bound classification; neither a suffix nor a claimed category grants credit."""
    source, symbol = match["source"], match["symbol"]
    if source in UNITS or symbol in {unit[0] for unit in UNITS.values()}:
        unit = UNITS.get(source)
        if (unit is None or (symbol, match.get("image_id", "resident"), match["rom_start"], match["vram_start"], match["size"])
                != (unit[0], "resident", unit[3], unit[4], unit[5])):
            raise ValueError("Upstream libgcc accounting identity differs from the three original functions")
        return FAMILY
    return "cpp" if str(source).endswith(".cpp") else "c"


def interface_text(text: str) -> str:
    """Diagnostic AST spelling only, AFTER validating the exact upstream expansion.

    pycparser lacks GNU mode attributes and extended asm. Preserve the actual
    GCC 2.8.1 integer/float widths before its ordinary attribute erasure, and
    omit the local multu expression from interface analysis, not from compilation.
    No external call, declaration, object or function definition is omitted.
    """
    text = text.replace("\f", " ")  # upstream page separators are C whitespace
    types = {"UQItype": "unsigned char", "SItype": "int", "USItype": "unsigned int",
             "DItype": "long long", "UDItype": "unsigned long long",
             "SFtype": "float", "DFtype": "double", "word_type": "int"}
    for name, spelling in types.items():
        pattern = rf"typedef\s+(?:unsigned\s+)?(?:int|float)\s+{name}\s+__attribute__\s*\(\(\s*mode\s*\(\s*\w+\s*\)\s*\)\)\s*;"
        text, count = re.subn(pattern, lambda m: f"typedef {spelling} {name};" + "\n" * m[0].count("\n"), text)
        if count != 1:
            raise ValueError(f"Unexpected pinned upstream mode typedef: {name}")
    text, count = re.subn(r'__asm__\s*\(\s*"multu %2,%3".*?\)\s*;',
                          lambda m: "0;" + "\n" * m[0].count("\n"), text, flags=re.DOTALL)
    if count != 1:
        raise ValueError("Unexpected pinned upstream multu expansion")
    return text
