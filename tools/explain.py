"""Read-only author diagnostics from our pinned GCC 2.x RTL dumps.

No diagnostic compile is a receipt. The normal matcher runs unchanged first;
all further code and relocation identities must equal that normal object before
word-to-RTL claims are exposed. GCC's source notes need -g; old gas cannot read
some generated STABS expressions, so only .stab* directives are blanked in the
separate mapping assembly. If -g changes code, a separately proved -da -dp
stage retains UID diagnostics without source attribution.
"""
from __future__ import annotations

import hashlib
import json
import re
import subprocess
import tempfile
from pathlib import Path
from typing import Any

from rtl_alloc import flip_thresholds, parse_allocation
from rtl_asm import encoding_options
from rtl_sched import parse_scheduler, scheduling_competition

FUNCTION = re.compile(r"^;; Function (.+)$", re.MULTILINE)
REG = re.compile(r"\(reg(?:/[\w]+)*(?::[\w]+)?\s+(\d+)\b")
DEF = re.compile(r"\(set\s+\((?:subreg:\w+\s+\()?reg(?:/[\w]+)*(?::[\w]+)?\s+(\d+)\b")
UID = re.compile(r"#\s+(\d+)\s+[A-Za-z_][\w]*(?:/\d+)?\s*$")


def unavailable(stage: str, reason: str, *, line: int | None = None, text: str | None = None) -> dict[str, Any]:
    return {"stage": stage, "reason": reason, "line": line, "text": text}


def function_text(text: str, function: str) -> str:
    banners = list(FUNCTION.finditer(text))
    found = [i for i, banner in enumerate(banners) if banner.group(1) == function
             or re.search(r"(?<![\w])" + re.escape(function) + r"\s*\(", banner.group(1))]
    if len(found) != 1:
        raise ValueError(f"Expected one RTL function {function}, found {len(found)}")
    index = found[0]
    return text[banners[index].end():banners[index + 1].start() if index + 1 < len(banners) else len(text)]


def rtl_instructions(text: str, function: str) -> dict[str, Any]:
    """Read complete top-level RTL records, never the register-death annotations.

    Registers/sets are extracted only from the instruction pattern preceding its
    recognition number. Source notes are observations, not unique C assignments.
    """
    selected = function_text(text, function)
    starts = list(re.finditer(r"^\((\w+)(?:/[\w]+)*\s+(\d+)\b", selected, re.MULTILINE))
    instructions: dict[int, dict[str, Any]] = {}
    origins: dict[int, list[dict[str, Any]]] = {}
    issues = []
    location: dict[str, Any] | None = None
    for i, start in enumerate(starts):
        raw = selected[start.start():starts[i + 1].start() if i + 1 < len(starts) else len(selected)]
        kind, uid = start.group(1), int(start.group(2))
        if kind == "note":
            note = re.match(r'\(note(?:/[\w]+)*\s+\d+\s+\d+\s+\d+\s+\("((?:\\.|[^"\\])*)"\)\s+(\d+)\)', raw)
            if note:
                location = {"file": note.group(1), "line": int(note.group(2))}
            continue
        if kind not in {"insn", "jump_insn", "call_insn", "barrier", "code_label"}:
            issues.append(unavailable("rtl", f"Unknown top-level RTL record {kind}", text=raw[:160]))
            continue
        if kind in {"barrier", "code_label"}:
            continue
        # Patterns end before the recognition number/name and dependency notes.
        pattern_end = re.search(r"\)\s+-?\d+(?:\s+\{[^}]*\})?\s+\(", raw)
        if pattern_end is None:
            issues.append(unavailable("rtl", f"Unrecognized instruction pattern for UID {uid}", text=raw[:160]))
            continue
        pattern = raw[:pattern_end.start() + 1]
        registers = sorted({int(x) for x in REG.findall(pattern)})
        definitions = sorted({int(x) for x in DEF.findall(pattern)})
        instructions[uid] = {"uid": uid, "registers": registers, "definitions": definitions,
                             "source": dict(location) if location else None, "pattern": pattern}
        for pseudo in definitions:
            if pseudo >= 64 and location and location not in origins.setdefault(pseudo, []):
                origins[pseudo].append(dict(location))
    return {"instructions": instructions, "origins": origins, "unavailable": issues}


def assembly_listing(assembly: str, listing: str, text: bytes) -> dict[str, Any]:
    """Prove byte and opcode/operand ownership, not gas's source-line guess.

    In reorder mode gas can report post-swap bytes against pre-swap source
    lines, including macro continuations. Only a unique compatible RTL UID in
    the same bounded source run is attributable. Unknown or ambiguous owners
    remain unavailable. Noreorder words must match their own instruction.
    """
    lines = assembly.splitlines()
    by_line: dict[int, dict[str, Any]] = {}
    groups: dict[int, list[dict[str, Any]]] = {}
    group = 0
    reorder = True
    mode_stack = []
    section = previous_section = ".text"
    uid = function = None
    for number, row in enumerate(lines, 1):
        stripped = row.split("#", 1)[0].strip()
        # Bytes flushed by a boundary belong to the preceding source run and
        # section. The new state applies only to subsequent source lines.
        row_section, row_reorder = section, reorder
        if stripped.startswith(".section"):
            previous_section, section = section, stripped.split()[1].split(",")[0]
            uid = None
        elif stripped in {".text", ".data", ".rodata", ".rdata", ".bss", ".sdata"}:
            previous_section, section = section, stripped
            uid = None
        elif stripped == ".previous":
            section, previous_section = previous_section, section
            uid = None
        mode = re.fullmatch(r"\.set\s+(reorder|noreorder|push|pop)", stripped)
        if mode:
            if mode[1] == "push":
                mode_stack.append(reorder)
            elif mode[1] == "pop":
                reorder = mode_stack.pop() if mode_stack else None
            else:
                reorder = mode[1] == "reorder"
        ent = re.match(r"\.ent\s+(\S+)", stripped)
        if ent:
            function, uid = ent.group(1), None
        if stripped.startswith(".end"):
            function, uid = None, None
        annotation = UID.search(row)
        if annotation:
            uid = int(annotation.group(1))
        code_line = bool(stripped) and not stripped.startswith(".") and not stripped.endswith(":")
        info = {"uid": uid if code_line else None, "function": function, "section": row_section,
                "assembly_line": number, "assembly": row.strip(), "reorder": row_reorder,
                "_group": group, "_code": code_line,
                "_flush": stripped.endswith(":") or bool(re.match(r"\.(?:set|end|align|p2align|balign|section|previous|text|data|rodata|rdata|bss|sdata)\b", stripped)),
                "_data": bool(re.match(r"\.(?:byte|hword|half|short|word|dword|long|quad|2byte|4byte|8byte|ascii|asciz|string|space|zero|fill|incbin)\b", stripped)),
                "_encodings": encoding_options(row) if code_line else None}
        by_line[number] = info
        if code_line and row_section == ".text":
            groups.setdefault(group, []).append(info)
        elif stripped:
            group += 1
    words: dict[int, dict[str, Any]] = {}
    issues = []
    previous_line = next_address = None
    symbols = False
    for number, row in enumerate(listing.splitlines(), 1):
        if row.startswith(("DEFINED SYMBOLS", "UNDEFINED SYMBOLS", "NO DEFINED SYMBOLS", "NO UNDEFINED SYMBOLS")):
            symbols = True
        if symbols or not row.strip() or row.startswith("GAS LISTING"):
            continue
        if re.match(r"\s*\d+:\S.* \*{4}(?: |$)", row):
            continue  # Modern gas -a high-level source, not assembly/byte rows.
        prefix = row.split("\t", 1)[0]
        match = re.fullmatch(r"\s*(\d+)\s+(?:([0-9a-fA-F]{4,}|\?{4})\s+)?((?:[0-9a-fA-F]{2}){1,4}(?:\s+(?:[0-9a-fA-F]{2}){1,4})*)\s*", prefix)
        if not match:
            if re.match(r"\s*\d+\s*$", prefix):
                continue
            issues.append(unavailable("listing", "Unknown listing row", line=number, text=row))
            continue
        source_line = int(match.group(1))
        raw = bytes.fromhex(match.group(3))
        info = by_line.get(source_line)
        eof_padding = info is None and source_line == len(lines) + 1 and section == ".text" and not any(raw)
        if eof_padding:
            info = {"uid": None, "function": None, "section": ".text", "assembly_line": None,
                    "assembly": None, "reorder": reorder, "_group": group, "_code": False,
                    "_encodings": None}
        if info is None:
            issues.append(unavailable("listing", "Listing source line is outside assembly", line=number, text=row))
            continue
        if info["section"] != ".text":
            previous_line, next_address = None, None
            continue
        address = match.group(2)
        if address == "????" or (address is None and (source_line != previous_line or next_address is None)):
            issues.append(unavailable("listing", "Unavailable continuation address", line=number, text=row))
            continue
        offset = int(address, 16) if address else next_address
        if offset % 4 or len(raw) % 4:
            issues.append(unavailable("listing", "Non-word text bytes cannot be mapped to a MIPS instruction", line=number, text=row))
            continue
        if eof_padding and (offset < max(words, default=-4) + 4 or any(text[offset:])):
            issues.append(unavailable("listing", "EOF padding is not a zero-filled text tail", line=number, text=row))
            continue
        for index in range(0, len(raw), 4):
            at = offset + index
            chunk = raw[index:index + 4]
            if text[at:at + 4] != chunk:
                issues.append(unavailable("listing", f"Listing bytes disagree with diagnostic object at +0x{at:X}", line=number, text=row))
                continue
            if at in words:
                issues.append(unavailable("listing", f"Duplicate listing address +0x{at:X}", line=number, text=row))
                words.pop(at, None)
                continue
            word = int.from_bytes(chunk, "big")
            if info["reorder"] is True and (info["_code"] or info.get("_flush")):
                pool = groups.get(info["_group"], [])
            else:
                pool = [info] if info["_code"] else []
            uncertain = info["reorder"] is None or any(not item["_encodings"] or item["uid"] is None for item in pool)
            candidates = [item for item in pool if item["_encodings"]
                          and any(word & mask == value for mask, value in item["_encodings"])]
            owners = {(item["function"], item["uid"]) for item in candidates}
            evidence = {"offset": at, "listing_assembly_line": source_line,
                        "expansion": address is None or index != 0}
            if not uncertain and len(owners) == 1 and not eof_padding:
                owner = next((item for item in candidates if item is info), candidates[0])
                mapped = {key: value for key, value in owner.items() if not key.startswith("_")}
                possible_lines = sorted({item["assembly_line"] for item in candidates})
                if len(possible_lines) > 1:
                    mapped.update(assembly_line=None, assembly=None)
                words[at] = {**mapped, **evidence, "compatible_assembly_lines": possible_lines,
                             "attribution": "reorder-unique-uid" if info["reorder"] else "noreorder-encoding",
                             "opcode_checked": True}
            elif info.get("_data") or eof_padding or (not word and not info["_code"] and not candidates):
                words[at] = {**evidence, "uid": None, "function": info["function"], "section": ".text",
                             "assembly_line": None, "assembly": None,
                             "attribution": "data" if info.get("_data") else "padding"}
            else:
                reason = (f"Assembler-moved word at +0x{at:X}: "
                          + ("unsupported source instruction in this run" if uncertain
                             else "ambiguous RTL owners" if len(owners) > 1 else "no compatible source instruction")
                          + "; RTL UID unavailable")
                words[at] = {**evidence, "uid": None, "function": info["function"], "section": ".text",
                             "assembly_line": None, "assembly": None, "attribution": "unavailable",
                             "unavailable": reason}
                issues.append(unavailable("attribution", reason, line=number, text=row))
        previous_line, next_address = source_line, offset + len(raw)
    return {"words": words, "unavailable": issues}


def register_fields(word: int) -> dict[str, tuple[int, int]] | None:
    """MIPS III explicit allocator register operands: name -> (hard reg, mask)."""
    op, fn = word >> 26, word & 63
    rs = ((word >> 21) & 31, 31 << 21)
    rt = ((word >> 16) & 31, 31 << 16)
    rd = ((word >> 11) & 31, 31 << 11)
    if op == 0:
        if fn in {0, 2, 3, 56, 58, 59, 60, 62, 63}:
            return {"rt": rt, "rd": rd}
        if fn in {4, 6, 7, 20, 22, 23, 32, 33, 34, 35, 36, 37, 38, 39, 42, 43, 44, 45, 46, 47}:
            return {"rs": rs, "rt": rt, "rd": rd}
        if fn == 8:
            return {"rs": rs}
        if fn == 9:
            return {"rs": rs, "rd": rd}
        if fn in {16, 18}:
            return {"rd": rd}
        if fn in {17, 19}:
            return {"rs": rs}
        if fn in {24, 25, 26, 27, 28, 29, 30, 31, 48, 49, 50, 51, 52, 54}:
            return {"rs": rs, "rt": rt}
        return {} if fn in {12, 13, 15} else None
    if op in {2, 3}:
        return {}
    if op in {1, 6, 7, 22, 23}:
        return {"rs": rs}
    if op in {4, 5, 20, 21} or 8 <= op <= 14 or 24 <= op <= 27 or 32 <= op <= 46 or op in {48, 52, 55, 56, 60, 63}:
        return {"rs": rs, "rt": rt}
    if op == 15:
        return {"rt": rt}
    if op in {49, 53, 57, 61}:
        return {"rs": rs, "ft": (rt[0] + 32, rt[1])}
    if op == 17:
        fmt = rs[0]
        fs, ft, fd = (rd[0] + 32, rd[1]), (rt[0] + 32, rt[1]), (((word >> 6) & 31) + 32, 31 << 6)
        if fmt in {0, 2, 4, 6}:
            return {"rt": rt, "fs": fs} if fmt in {0, 4} else {"rt": rt}
        if fmt == 8:
            return {}
        if fmt in {16, 17, 20, 21}:
            if fn <= 3:
                return {"fs": fs, "ft": ft, "fd": fd}
            if fn in {4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 32, 33, 36, 37}:
                return {"fs": fs, "fd": fd}
            if fn >= 48:
                return {"fs": fs, "ft": ft}
    if op == 16 and rs[0] in {0, 4}:
        return {"rt": rt}
    return None


def _object_identity(elf: Any) -> dict[str, Any]:
    import match as matcher
    sections = []
    for section in elf.sections:
        if not section.size or section.name in matcher.IGNORED_SECTIONS or not (section.flags & matcher.SHF_ALLOC or section.name in matcher.DATA_SECTION_NAMES):
            continue
        relocations = []
        for offset, kind, symbol in elf.relocations(section):
            owner = elf.sections[symbol.shndx].name if 0 < symbol.shndx < len(elf.sections) else symbol.shndx
            relocations.append([offset, kind, symbol.name, symbol.value, symbol.size, owner])
        sections.append({"name": section.name, "type": section.type, "size": section.size,
                         "alignment": section.addralign, "flags": section.flags,
                         "sha256": hashlib.sha256(elf.contents(section)).hexdigest(), "relocations": relocations})
    return {"sections": sections, "commons": sorted([s.name, s.size, s.value] for s in elf.symbols if s.shndx == matcher.SHN_COMMON),
            "functions": sorted([s.name, s.value, s.size] for s in elf.symbols if s.type == matcher.STT_FUNC)}


def _pseudo(record: dict[str, Any], origins: dict[int, list[dict[str, Any]]]) -> dict[str, Any]:
    return {**record, "source_lines": origins.get(record["pseudo"], [])}


def _flip(candidate: dict[str, Any], holder: dict[str, Any]) -> dict[str, Any]:
    result = flip_thresholds(candidate, holder)
    thresholds = []
    for key, row, field, sign, operator in (
        ("candidate_refs_increase", candidate, "refs", 1, ">="),
        ("candidate_live_reduction", candidate, "live_length", -1, "<="),
        ("holder_refs_reduction", holder, "refs", -1, "<="),
        ("holder_live_increase", holder, "live_length", 1, ">="),
    ):
        delta = result[key]
        if delta is not None and delta > 0:
            thresholds.append({"pseudo": row["pseudo"], "metric": field, "operator": operator,
                               "value": row[field] + sign * delta, "delta": delta})
    result["thresholds"] = thresholds
    result["scope"] = ("Candidate already sorts earlier: no priority increase can explain this placement."
                       if result["candidate_beats_holder"] else
                       "Independent minimum metric changes with all other metrics fixed; not a promised C edit or hard-register win.")
    return result


def explain_words(original: bytes, candidate: bytes, targets: list[Any], functions: dict[str, Any], mapping: dict[int, Any]) -> list[dict[str, Any]]:
    differences = []
    origin = targets[0].rom_start
    original_words = [int.from_bytes(original[i:i + 4], "big") for i in range(0, len(original) - 3, 4)]
    candidate_words = [int.from_bytes(candidate[i:i + 4], "big") for i in range(0, len(candidate) - 3, 4)]
    for index, (expected, actual) in enumerate(zip(original_words, candidate_words)):
        if expected == actual:
            continue
        offset = index * 4
        owner = next((t for t in targets if t.rom_start - origin <= offset < t.rom_start - origin + t.size), None)
        row: dict[str, Any] = {"offset": offset, "target_word": f"{expected:08x}", "candidate_word": f"{actual:08x}",
                               "function": owner.symbol if owner else None, "register_differences": [], "scheduling": None, "unavailable": []}
        info = mapping.get(offset)
        row["candidate_instruction"] = info
        if owner is None or owner.symbol not in functions:
            row["unavailable"].append(unavailable("alignment", "Outside a requested function (natural padding or size difference)"))
            differences.append(row)
            continue
        facts = functions[owner.symbol]
        allocation, rtl, scheduling = facts["allocation"], facts["rtl"], facts["scheduler"]
        uid = info.get("uid") if info else None
        insn = rtl["instructions"].get(uid)
        if not info or uid is None:
            row["unavailable"].append(unavailable("alignment", "No checked assembler-listing UID for this word"))
        if info and info.get("unavailable"):
            row["unavailable"].append(unavailable("attribution", info["unavailable"]))
        if insn:
            row["source"] = insn["source"]
        else:
            row["unavailable"].append(unavailable("rtl", "UID absent before allocation (prologue, reload, or later transform)"))
        begin = (owner.rom_start - origin) // 4
        end = min(begin + owner.size // 4, len(candidate_words))
        moved = [i for i in range(begin, end) if i != index and candidate_words[i] == expected]
        if len(moved) == 1:
            other_offset = moved[0] * 4
            other = mapping.get(other_offset)
            other_uid = other.get("uid") if other else None
            uids = [x for x in (uid, other_uid) if x is not None]
            decisions = scheduling_competition(scheduling, uids) if len(set(uids)) == 2 else []
            row["scheduling"] = {"target_word_candidate_offset": other_offset, "uids": uids,
                                 "mutual_swap": moved[0] < len(original_words) and original_words[moved[0]] == actual,
                                 "instructions": [scheduling["instructions"].get(x) for x in uids],
                                 "shared_ready_lists": decisions,
                                 "caveat": "Target word relocation is observed, not proof of a scheduler cause. sched2 precedes jump2/delay-slot scheduling."}
            if not decisions:
                row["scheduling"]["unavailable"] = "No common sched2 ready-list decision; dependency or later delay-slot ordering cannot be inferred."
        fields, target_fields = register_fields(actual), register_fields(expected)
        if fields is None or target_fields is None:
            row["unavailable"].append(unavailable("decode", "Unsupported MIPS operand format"))
        elif fields.keys() == target_fields.keys():
            mask = 0
            for value in fields.values():
                mask |= value[1]
            if (actual & ~mask) == (expected & ~mask):
                for operand, (hard, _) in fields.items():
                    wanted = target_fields[operand][0]
                    if hard == wanted:
                        continue
                    candidates = [record for pseudo, record in allocation["pseudos"].items()
                                  if insn and pseudo in insn["registers"] and record.get("hard_register") is not None and record.get("size_words")
                                  and record["hard_register"] <= hard < record["hard_register"] + record["size_words"]]
                    detail: dict[str, Any] = {"operand": operand, "candidate_hard_register": hard, "target_hard_register": wanted,
                                               "candidates": [], "unavailable": []}
                    for contender in candidates:
                        competitors = [record for pseudo, record in allocation["pseudos"].items()
                                       if pseudo != contender["pseudo"] and record.get("hard_register") is not None and record.get("size_words")
                                       and record["hard_register"] <= wanted < record["hard_register"] + record["size_words"]
                                       and pseudo in allocation["conflicts"].get(contender["pseudo"], [])]
                        entry = {"pseudo": _pseudo(contender, rtl["origins"]), "competitors": []}
                        for competitor in competitors:
                            entry["competitors"].append({"pseudo": _pseudo(competitor, rtl["origins"]),
                                                         "flip": _flip(contender, competitor)})
                        if not competitors:
                            entry["unavailable"] = "No conflicting pseudo proved to hold the target register; fixed regs, local allocation, preferences or reload may control it."
                        detail["candidates"].append(entry)
                    if not candidates:
                        detail["unavailable"].append("No pre-allocation pseudo at this UID owns the differing register (may be a fixed register or later transformation).")
                    row["register_differences"].append(detail)
            elif not row["scheduling"]:
                row["unavailable"].append(unavailable("alignment", "Instruction structure/immediate differs, not a pure register substitution or uniquely relocated word"))
        elif not row["scheduling"]:
            row["unavailable"].append(unavailable("alignment", "Different instruction operand formats; no unique exact-word relocation"))
        if row["scheduling"]:
            row["scheduling"]["source_lines"] = [rtl["instructions"][item]["source"] for item in row["scheduling"]["uids"]
                                                  if item in rtl["instructions"] and rtl["instructions"][item]["source"]]
        differences.append(row)
    return differences


def collect(result: Any, targets: list[Any], source: Path, profile_id: str, out_dir: Path, *,
            includes: list[Path] = (), source_root: Path, rom: bytes) -> dict[str, Any]:
    """Run isolated diagnostics; never alter the normal matcher result/receipt."""
    import match as matcher
    out_dir = out_dir.resolve()
    report: dict[str, Any] = {"schema_version": 1, "diagnostic_only": True, "receipt_eligible": False,
                              "status": "unavailable", "profile_id": profile_id, "normal_status": result.status,
                              "codegen_neutrality": {}, "functions": {}, "differences": [], "unavailable": []}
    root = out_dir / "explain"
    root.mkdir(parents=True, exist_ok=True)
    work = Path(tempfile.mkdtemp(prefix="run-", dir=root))
    report["evidence_directory"] = str(work)
    try:
        if result.profile_id != profile_id:
            raise ValueError("--explain cannot use overridden diagnostic compiler/assembler flags")
        if not (out_dir / "candidate.elf").is_file() or result.differing_words is None:
            raise ValueError("Normal matcher did not produce linked comparison evidence")
        for item in result.participating_files:
            if hashlib.sha256(Path(item["path"]).read_bytes()).hexdigest() != item["sha256"]:
                raise ValueError(f"Source changed since normal compilation: {item['path']}")
        profile = matcher.tool_profile(matcher.PROJECT, source_root)
        assigned = profile["profiles"][profile_id]
        compiler = assigned.get("compiler", "gcc272")
        if compiler not in {"gcc272", "gcc281pm"}:
            raise ValueError(f"Unsupported explain compiler {compiler}")
        report["compiler"] = {"key": compiler, "frontend": "cc1plus" if source.suffix == ".cpp" else "cc1",
                              "sha256": profile["compilers"][compiler]["cc1plus" if source.suffix == ".cpp" else "cc1"]["sha256"]}
        gcc, assembler = matcher.profile_toolchain(profile, profile_id)
        flags = [item for path in includes for item in ("-I", str(path.resolve()))] + assigned["compiler_flags"]
        baseline = _object_identity(matcher.Elf((out_dir / "candidate.o").read_bytes()))
        reports = {}
        stages = [("da", ["-da"]), ("annotated", ["-da", "-dp", "-g"])]
        for stage, extra in stages:
            directory = work / stage
            directory.mkdir()
            (directory / "tmp").mkdir()
            (directory / "source").symlink_to(source_root.resolve(), target_is_directory=True)
            environment = matcher.compiler_environment(directory, profile)
            commands = []
            def run(argv: list[str], name: str) -> str:
                completed = subprocess.run(argv, cwd=directory, env=environment, capture_output=True, text=True)
                commands.append({"argv": argv, "cwd": str(directory), "exit_code": completed.returncode})
                (directory / f"{name}.log").write_text(json.dumps(argv) + "\n" + completed.stdout + completed.stderr)
                if completed.returncode:
                    raise ValueError(f"{stage}/{name} failed: {completed.stderr.strip()}")
                return completed.stdout
            run([*gcc, *flags, *extra, "-S", str(source.resolve()), "-o", "candidate.s"], "compile")
            assembly = (directory / "candidate.s").read_text()
            # Preserve line numbers and all labels/instructions; debug metadata only.
            mapping_assembly = "\n".join("" if re.match(r"\s*\.stab[ snd]", line) else line for line in assembly.splitlines()) + "\n"
            (directory / "mapping.s").write_text(mapping_assembly)
            listing = run([assembler, *assigned["assembler_flags"], "-a", "-o", "diagnostic.o", "mapping.s"], "assemble")
            (directory / "listing.txt").write_text(listing)
            elf = matcher.Elf((directory / "diagnostic.o").read_bytes())
            identity = _object_identity(elf)
            equal = identity == baseline
            report["codegen_neutrality"][stage] = {"flags": extra, "equal": equal, "normal": baseline,
                                                     "diagnostic": identity, "commands": commands,
                                                     "debug_metadata_adapter": "blank .stabs/.stabn/.stabd only; preserve instruction/label/line order"}
            if not equal:
                if stage == "annotated":
                    stages.append(("uid", ["-da", "-dp"]))
                    continue
                raise ValueError(f"{stage} diagnostic object changes allocated bytes, relocations or layout; explanations withheld")
            reports[stage] = (directory, mapping_assembly, listing, elf)
        selected = "annotated" if "annotated" in reports else "uid"
        report["mapping_stage"] = selected
        report["source_notes"] = {"status": "available" if selected == "annotated" else "unavailable"}
        if selected == "uid":
            reason = "-g changed codegen; using neutral -da -dp dumps and UID mapping without source notes"
            report["source_notes"]["reason"] = reason
            report["unavailable"].append(unavailable("source_notes", reason))
        directory, assembly, listing, elf = reports[selected]
        mapped = assembly_listing(assembly, listing, elf.contents(elf.section(".text")))
        report["unavailable"].extend(mapped["unavailable"])
        dumps = {}
        for suffix in ("lreg", "greg", "sched2"):
            paths = list(directory.glob(f"*.{suffix}"))
            if len(paths) != 1:
                raise ValueError(f"Expected one .{suffix} dump, found {len(paths)}")
            dumps[suffix] = paths[0].read_text()
        for target in targets:
            allocation = parse_allocation(dumps["lreg"], dumps["greg"], target.symbol)
            scheduler = parse_scheduler(dumps["sched2"], target.symbol)
            rtl = rtl_instructions(dumps["lreg"], target.symbol)
            if selected == "uid":
                rtl["origins"] = {}
                for instruction in rtl["instructions"].values():
                    instruction["source"] = None
            report["functions"][target.symbol] = {"allocation": allocation, "scheduler": scheduler, "rtl": rtl}
            for stage in (allocation, scheduler, rtl):
                report["unavailable"].extend(stage.get("unavailable", []))
        linked = matcher.Elf((out_dir / "candidate.elf").read_bytes())
        code = linked.contents(linked.section(".text"))
        original = rom[targets[0].rom_start:targets[-1].slot_end][:len(code)]
        report["differences"] = explain_words(original, code, targets, report["functions"], mapped["words"])
        if len(report["differences"]) != result.differing_words:
            raise ValueError("Explanation difference count disagrees with the normal matcher")
        for item in result.participating_files:
            if hashlib.sha256(Path(item["path"]).read_bytes()).hexdigest() != item["sha256"]:
                raise ValueError(f"Source changed during diagnostics: {item['path']}")
        partial = bool(report["unavailable"])
        for row in report["differences"]:
            partial |= bool(row["unavailable"] or (row["scheduling"] or {}).get("unavailable"))
            for register in row["register_differences"]:
                partial |= bool(register["unavailable"] or any(p.get("unavailable") for p in register["candidates"]))
        report["status"] = "no-differences" if not report["differences"] else "partial" if partial else "available"
    except (OSError, ValueError, KeyError) as error:
        report["status"] = "unavailable"
        report["differences"] = []
        report["unavailable"].append(unavailable("diagnostics", str(error)))
    (out_dir / "explain.json").write_text(json.dumps(report, indent=2) + "\n")
    return report


def render(report: dict[str, Any]) -> str:
    lines = [f"EXPLAIN ({report['status']}): diagnostic only; never a receipt"]
    for stage, evidence in report["codegen_neutrality"].items():
        lines.append(f"  {stage}: allocated bytes/relocations/layout unchanged = {evidence['equal']}")
    if report["status"] == "no-differences":
        lines.append("  No differing target/candidate words.")
    for row in report["differences"]:
        uid = (row.get("candidate_instruction") or {}).get("uid")
        lines.append(f"  +0x{row['offset']:X}: {row['target_word']} -> {row['candidate_word']} (UID {uid})")
        if row.get("source"):
            lines.append(f"    source: {row['source']['file']}:{row['source']['line']}")
        for register in row["register_differences"]:
            lines.append(f"    {register['operand']}: hard {register['candidate_hard_register']} wants {register['target_hard_register']}")
            for candidate in register["candidates"]:
                pseudo = candidate["pseudo"]
                lines.append(f"      pseudo {pseudo['pseudo']}: refs={pseudo['refs']} live={pseudo['live_length']} size={pseudo['size_words']} priority={pseudo['priority']} source={pseudo['source_lines']}")
                for holder in candidate["competitors"]:
                    rival = holder["pseudo"]
                    lines.append(f"      competing pseudo {rival['pseudo']}: refs={rival['refs']} live={rival['live_length']} size={rival['size_words']} priority={rival['priority']} source={rival['source_lines']}")
                    lines.append(f"        conditional priority flip: {json.dumps(holder['flip'], sort_keys=True)}")
                    for threshold in holder["flip"]["thresholds"]:
                        lines.append(f"        pseudo {threshold['pseudo']} needs {threshold['metric']} {threshold['operator']} {threshold['value']}")
                if candidate.get("unavailable"):
                    lines.append(f"      unavailable: {candidate['unavailable']}")
            lines.extend(f"      unavailable: {item}" for item in register["unavailable"])
        if row["scheduling"]:
            schedule = row["scheduling"]
            lines.append(f"    target word occurs at candidate +0x{schedule['target_word_candidate_offset']:X}; UIDs {schedule['uids']}")
            lines.append(f"    source notes: {json.dumps(schedule['source_lines'])}")
            lines.append(f"    sched2 base priorities: {json.dumps(schedule['instructions'])}")
            lines.append(f"    sched2 shared ready-list order: {json.dumps(schedule['shared_ready_lists'])}")
            if schedule.get("unavailable"):
                lines.append(f"    unavailable: {schedule['unavailable']}")
        lines.extend(f"    unavailable: {item['reason']}" for item in row["unavailable"])
    lines.extend(f"  unavailable: {item.get('reason', item)}" for item in report["unavailable"])
    lines.append(f"  evidence: {report['evidence_directory']}")
    return "\n".join(lines)
