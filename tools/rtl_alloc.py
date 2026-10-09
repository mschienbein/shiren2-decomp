"""Diagnostic readers for GCC 2.x MIPS allocation summaries, not RTL syntax."""
from __future__ import annotations

import re


def global_priority(refs: int, live_length: int, size_words: int) -> int:
    """GCC global.c allocno_compare: double arithmetic, then C int truncation.

    Negative sentinel live lengths are not ordinary allocatable live ranges.
    Local allocation uses quantity birth/death ranges, NOT this function.
    """
    if any(type(v) is not int for v in (refs, live_length, size_words)):
        raise ValueError("priority inputs must be integers")
    if refs <= 0 or live_length <= 0 or size_words <= 0:
        raise ValueError("priority requires positive refs, live length and word size")
    priority = int(((refs.bit_length() - 1) * refs / live_length) * 10000 * size_words)
    if priority > 2147483647:
        raise ValueError("priority exceeds GCC's signed int range")
    return priority


def flip_thresholds(candidate: dict, holder: dict) -> dict:
    """Independent one-variable changes, keeping sizes and the other row fixed.

    Ties use allocno when supplied, else ascending pseudo for unshared allocnos.
    These are ordering counterfactuals, not promises of register assignment.
    """
    result = dict(candidate_refs_increase=None, candidate_live_reduction=None,
                  holder_refs_reduction=None, holder_live_increase=None,
                  candidate_beats_holder=None, unavailable=[])
    def priority(row: dict) -> int:
        if row.get("allocation") == "local":
            raise ValueError("local quantity priority cannot be reconstructed from global metrics")
        if row.get("shared_allocno"):
            raise ValueError("shared allocno aggregate metrics are unavailable")
        return global_priority(row["refs"], row["live_length"], row["size_words"])
    def tie(row: dict) -> int:
        value = row.get("allocno", row.get("pseudo"))
        if type(value) is not int:
            raise ValueError("missing allocno/pseudo tiebreak evidence")
        return value
    try:
        priority(candidate)
        priority(holder)
        ct, ht = tie(candidate), tie(holder)
        def wins(c: dict, h: dict) -> bool:
            return (-priority(c), ct) < (-priority(h), ht)
        result["candidate_beats_holder"] = wins(candidate, holder)
        if result["candidate_beats_holder"]:
            for key in tuple(result)[:4]:
                result[key] = 0
            return result
        # Each predicate is monotone over a bounded integer domain. Binary search
        # avoids linear scans even for large reference counts or live ranges.
        def first(limit: int, predicate) -> int | None:
            if limit < 1 or not predicate(limit):
                return None
            lo, hi = 1, limit
            while lo < hi:
                mid = (lo + hi) // 2
                if predicate(mid):
                    hi = mid
                else:
                    lo = mid + 1
            return lo
        def changed(row: dict, field: str, delta: int) -> dict:
            return {**row, field: row[field] + delta}
        # Exponential bound stays within representable input/priority space.
        def increasing(row: dict, field: str, predicate) -> int | None:
            bound = 1
            limit = 2147483647 - row[field]
            while bound < limit:
                try:
                    if predicate(bound):
                        return first(bound, predicate)
                except ValueError:
                    break
                bound = min(bound * 2, limit)
            # Overflow is not evidence of a real GCC ordering threshold.
            try:
                return first(bound, predicate)
            except ValueError:
                return None
        result["candidate_refs_increase"] = increasing(candidate, "refs", lambda d: wins(changed(candidate, "refs", d), holder))
        result["candidate_live_reduction"] = first(candidate["live_length"] - 1, lambda d: wins(changed(candidate, "live_length", -d), holder))
        result["holder_refs_reduction"] = first(holder["refs"] - 1, lambda d: wins(candidate, changed(holder, "refs", -d)))
        result["holder_live_increase"] = increasing(holder, "live_length", lambda d: wins(candidate, changed(holder, "live_length", d)))
    except (KeyError, ValueError, TypeError) as exc:
        result["unavailable"].append({"stage": "allocation", "line": None, "text": "", "reason": str(exc)})
    return result


def parse_allocation(lreg_text: str, greg_text: str, function: str) -> dict:
    """Read one exact C identifier from C or demangled C++ function headers.

    Integer dictionary keys are deliberate; JSON serializers stringify them.
    Unknown pre-RTL rows are retained as unavailable evidence. All line numbers
    are one-based in the original dump. No global priority is assigned to locals.
    """
    result = {"status": "unavailable", "pseudos": {}, "allocation_order": [],
              "conflicts": {}, "preferences": {}, "unavailable": []}
    pseudos = result["pseudos"]
    def issue(stage: str, line: int | None, text: str, reason: str) -> None:
        result["unavailable"].append(dict(stage=stage, line=line, text=text, reason=reason))
    def block(text: str, stage: str) -> list[tuple[int, str]]:
        lines = text.splitlines()
        heads = [(i, s[12:].strip()) for i, s in enumerate(lines) if s.startswith(";; Function ")]
        matches = [(n, i) for n, (i, name) in enumerate(heads)
                   if name == function or re.search(r"(?<![\w])" + re.escape(function) + r"\s*\(", name)]
        if len(matches) != 1:
            issue(stage, None, function, "missing or ambiguous function block")
            return []
        n, start = matches[0]
        end = heads[n + 1][0] if n + 1 < len(heads) else len(lines)
        rows = []
        for i in range(start + 1, end):
            if lines[i].lstrip().startswith("("):
                break
            if lines[i].strip():
                rows.append((i + 1, lines[i].strip()))
        if not rows:
            issue(stage, start + 1, lines[start], "missing allocation summary")
        return rows
    def pseudo(number: int) -> dict:
        return pseudos.setdefault(number, dict(pseudo=number, hard_register=None, refs=None,
                                              live_length=None, size_bytes=None, size_words=None,
                                              allocation="unallocated", priority=None))
    local_rows = block(lreg_text, "lreg")
    global_rows = block(greg_text, "greg")
    max_reg = None
    locals_seen = set()
    for line, text in local_rows:
        count = re.fullmatch(r"(\d+) registers\.", text)
        row = re.fullmatch(r"Register (\d+) used (\d+) times across (-?\d+) insns(?: in block \d+)?((?:; [^;]+)*)\.", text)
        local = re.fullmatch(r";; Register (\d+) in (\d+)\.", text)
        if count:
            max_reg = int(count[1])
            if max_reg <= 0:
                issue("lreg", line, text, "invalid register count")
        elif row:
            number, refs, live = map(int, row.group(1, 2, 3))
            size = 4
            valid = refs > 0 and live != 0 and live >= -2
            clauses = row[4].split("; ")[1:]
            for clause in clauses:
                size_match = re.fullmatch(r"(\d+) bytes", clause)
                if size_match:
                    size = int(size_match[1])
                    valid &= size > 0
                elif not re.fullmatch(r"dies in \d+ places|crosses 1 call|crosses \d+ calls|pointer|[A-Z_0-9]+ or none|pref [A-Z_0-9]+(?:, else [A-Z_0-9]+)?", clause):
                    issue("lreg", line, text, "unknown register summary clause: " + clause)
            if not valid or (max_reg is not None and number >= max_reg):
                issue("lreg", line, text, "malformed register metrics or out-of-range pseudo")
                continue
            item = pseudo(number)
            if item["refs"] is not None:
                issue("lreg", line, text, "duplicate register metrics")
                continue
            item.update(refs=refs, live_length=live, size_bytes=size, size_words=(size + 3) // 4)
        elif local:
            number, hard = map(int, local.groups())
            if hard >= 76 or number in locals_seen:
                issue("lreg", line, text, "invalid or duplicate local disposition")
                continue
            locals_seen.add(number)
            pseudo(number).update(hard_register=hard, allocation="local")
        elif not re.fullmatch(r"\d+ basic blocks\.|Basic block \d+: first insn \d+, last \d+\.|Registers live at start:(?: \d+)*|Reached from blocks:(?: \d+)*(?: previous)?", text):
            issue("lreg", line, text, "unknown allocation summary row")
    disposition = False
    disposition_seen = False
    assigned = set()
    order_seen = False
    for line, text in global_rows:
        order = re.fullmatch(r";; (\d+) regs to allocate:(.*)", text)
        relation = re.fullmatch(r";; (\d+) (conflicts|preferences):((?: \d+)*)", text)
        if order:
            if order_seen:
                issue("greg", line, text, "duplicate allocation order")
                continue
            order_seen = True
            entries = re.findall(r"(\d+(?:\+\d+)*)(?: \((\d+)\))?", order[2])
            remainder = re.sub(r"\d+(?:\+\d+)*(?: \(\d+\))?", "", order[2]).strip()
            numbers = [int(group.split("+")[0]) for group, _size in entries]
            if remainder or len(entries) != int(order[1]) or len(set(numbers)) != len(numbers):
                issue("greg", line, text, "malformed allocation order/count")
                continue
            result["allocation_order"] = numbers
            for group, size in entries:
                item = pseudo(int(group.split("+")[0]))
                if "+" in group:
                    issue("greg", line, text, "shared allocno aggregate priority unavailable")
                    item["shared_allocno"] = True
                if size:
                    if int(size) <= 0:
                        issue("greg", line, text, "invalid allocno word size")
                        item["shared_allocno"] = True
                    else:
                        item["size_words"] = int(size)
        elif relation:
            number = int(relation[1])
            if number in result[relation[2]]:
                issue("greg", line, text, "duplicate conflict/preference row")
            else:
                result[relation[2]][number] = [int(v) for v in relation[3].split()]
        elif text == ";; Register dispositions:":
            disposition = True
            disposition_seen = True
        elif re.fullmatch(r";; Hard regs used:(?: +\d+)*", text):
            disposition = False
        elif re.fullmatch(
            r";; Need \d+ (?:(?:nongroup )?regs?|groups? \(\w+mode\)) of class [A-Z_0-9]+ \(for insn \d+\)\."
            r"|Spilling reg \d+\.|Register \d+ now (?:on stack|in \d+)\.", text
        ):
            pass
        elif disposition:
            pairs = re.findall(r"(\d+) in (\d+)", text)
            if not pairs or re.sub(r"\d+ in \d+", "", text).strip():
                issue("greg", line, text, "malformed register disposition row")
                continue
            for number, hard in pairs:
                number, hard = int(number), int(hard)
                if hard >= 76 or number in assigned or (max_reg is not None and number >= max_reg):
                    issue("greg", line, text, "invalid or duplicate global disposition")
                    continue
                assigned.add(number)
                item = pseudo(number)
                item.update(hard_register=hard, allocation="local" if number in locals_seen else "global")
        else:
            issue("greg", line, text, "unknown allocation summary row")
    if local_rows and max_reg is None:
        issue("lreg", None, "", "missing register count")
    if global_rows and not disposition_seen:
        issue("greg", None, "", "missing register dispositions")
    for number in result["allocation_order"]:
        if number not in result["conflicts"]:
            issue("greg", None, str(number), "missing conflict row")
    for number, item in pseudos.items():
        if item["refs"] is None:
            issue("lreg", None, str(number), "missing pseudo metrics")
        if disposition_seen and number not in assigned:
            item.update(hard_register=None, allocation="unallocated")
        if number in result["allocation_order"] and not item.get("shared_allocno"):
            try:
                item["priority"] = global_priority(item["refs"], item["live_length"], item["size_words"])
            except (ValueError, TypeError):
                issue("greg", None, str(number), "global priority unavailable for missing metrics or sentinel live length")
    if not order_seen and any(item["allocation"] == "global" for item in pseudos.values()):
        issue("greg", None, "", "missing global allocation order")
    if pseudos or order_seen or disposition_seen:
        result["status"] = "partial" if result["unavailable"] else "available"
    return result
