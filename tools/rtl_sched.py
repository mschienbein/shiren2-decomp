"""Read GCC 2.x scheduler diagnostics without interpreting RTL bodies.

The pinned GCC sched.c selects ready[0] and prepends it to the schedule.
Thus dump selection order is backwards relative to emitted instruction order.
"""

import re


_BANNER = re.compile(r"^;;\s*Function\s+(.+?)\s*$")
_BLOCK = re.compile(r"-- basic block number (\d+) from (\d+) to (\d+) --")
_INSN = re.compile(r"insn\[\s*(\d+)\]: priority =\s*(-?\d+), ref_count =\s*(-?\d+)")
_READY = re.compile(r"ready list at T-(\d+):(.*)")
_ITEM = re.compile(r"(\d+)\s+\(([0-9a-fA-F]+)\)")
_HAZARD = re.compile(r"insn (\d+) has a greater potential hazard")
_BLOCKING = re.compile(r"blocking insn (\d+) for (\d+) cycles")
_LAUNCH = re.compile(r"launching (\d+) before (\d+) with (no|\d+) stalls at T-(\d+)")


def _matches_banner(banner: str, function: str) -> bool:
    if banner == function:
        return True
    # A C++ return type is not part of the function name. Match the name
    # immediately before its argument list, not an argument's type/name.
    return re.search(r"(?<![\w:~])" + re.escape(function) + r"\s*\(", banner) is not None


def parse_scheduler(text: str, function: str) -> dict:
    """Return JSON-safe scheduler evidence for one unambiguous function.

    ``order`` is the final printed ``now`` list, after hazard reordering;
    ``chosen`` is its first UID, not the last. ``clock`` is the positive N
    in T-N. ``unavailable`` entries retain original one-based dump lines.
    ``birth_promotion`` requires a low base priority raised into GCC's
    launch-priority range; a large fixed tail priority alone is not birth.
    """
    result = {"status": "unavailable", "function": function,
              "direction": "backward", "blocks": [], "instructions": {},
              "decisions": [], "events": [], "unavailable": []}
    lines = text.splitlines()
    banners = [(i, m.group(1)) for i, line in enumerate(lines)
               if (m := _BANNER.fullmatch(line))]
    matches = [(i, name) for i, name in banners if _matches_banner(name, function)]

    def unavailable(number: int, raw: str, reason: str, decision=None) -> None:
        evidence = {"stage": "scheduler", "line": number, "text": raw, "reason": reason}
        result["unavailable"].append(evidence)
        if decision is not None:
            decision["unavailable"].append(evidence)

    if len(matches) != 1:
        unavailable(0, function, "function section not found" if not matches
                    else "ambiguous function sections; request the full banner")
        return result
    start, result["banner"] = matches[0]
    end = next((i for i, _ in banners if i > start), len(lines))
    block = None
    decision = None
    initial_pending = False

    def set_order(value: str, number: int, raw: str) -> None:
        if decision is None:
            unavailable(number, raw, "order without a ready-list decision")
            return
        if not re.fullmatch(r"\s*\d+(?:\s+\d+)*\s*", value):
            unavailable(number, raw, "malformed selected order", decision)
            return
        order = [int(uid) for uid in value.split()]
        ready_uids = {item["uid"] for item in decision["ready"]}
        if len(set(order)) != len(order) or not set(order) <= ready_uids:
            unavailable(number, raw, "selected order does not match ready-list UIDs", decision)
            return
        decision["order"] = order
        decision["chosen"] = order[0]
        decision["selection_line"] = number

    for index in range(start + 1, end):
        raw = lines[index]
        number = index + 1
        if not raw.strip() or not raw.lstrip().startswith(";;"):
            continue
        row = raw.lstrip()[2:].strip()
        if not row:
            if initial_pending and block is not None:
                block["initial_ready"] = []
                initial_pending = False
            continue
        if m := _BLOCK.fullmatch(row):
            block = {"block": int(m[1]), "head": int(m[2]), "end": int(m[3]),
                     "line": number, "initial_ready": [], "already_scheduled": False}
            result["blocks"].append(block)
            decision = None
            initial_pending = False
            continue
        if row == "ready list initially:" and block is not None:
            initial_pending = True
            continue
        if initial_pending:
            initial_pending = False
            if re.fullmatch(r"\d+(?:\s+\d+)*", row):
                block["initial_ready"] = [int(uid) for uid in row.split()]
                continue
            unavailable(number, raw, "malformed initial ready list")
        if m := _INSN.fullmatch(row):
            result["instructions"][int(m[1])] = {
                "base_priority": int(m[2]), "ref_count": int(m[3]), "line": number}
            continue
        if m := _READY.fullmatch(row):
            if block is None:
                unavailable(number, raw, "ready list without basic block")
                continue
            decision = {"block": block["block"], "clock": int(m[1]), "ready": [],
                        "order": [], "chosen": None, "hazard": [], "blocking": [],
                        "line": number, "text": raw, "unavailable": []}
            result["decisions"].append(decision)
            ready_text, separator, order_text = m[2].partition(", now")
            if _ITEM.sub("", ready_text).strip() or not _ITEM.search(ready_text):
                unavailable(number, raw, "malformed ready list", decision)
            for item in _ITEM.finditer(ready_text):
                uid, priority = int(item[1]), int(item[2], 16)
                base = result["instructions"].get(uid, {}).get("base_priority")
                promoted = (base is not None and base & 0x7f000000 == 0
                            and 0x7f000001 <= priority < 0x7ffffffe and priority > base)
                decision["ready"].append({"uid": uid, "priority": priority,
                                          "birth_promotion": bool(promoted)})
                if base is None:
                    unavailable(number, raw, f"base priority unavailable for UID {uid}", decision)
            if separator:
                set_order(order_text, number, raw)
            continue
        summary, separator, order_text = row.partition(", now")
        if m := _HAZARD.fullmatch(summary):
            if decision is None:
                unavailable(number, raw, "hazard without ready-list decision")
            else:
                decision["hazard"].append({"uid": int(m[1]), "line": number,
                                           "before": list(decision["order"]), "text": raw})
                decision["chosen"] = None
                decision["order"] = []
                if separator:
                    set_order(order_text, number, raw)
            continue
        if m := _BLOCKING.fullmatch(summary):
            if decision is None:
                unavailable(number, raw, "blocking without ready-list decision")
            else:
                decision["blocking"].append({"uid": int(m[1]), "cycles": int(m[2]),
                                             "line": number})
                if separator:
                    set_order(order_text, number, raw)
            continue
        if m := _LAUNCH.fullmatch(row):
            result["events"].append({"kind": "launch", "block": block["block"] if block else None,
                                     "uid": int(m[1]), "before": int(m[2]),
                                     "stalls": 0 if m[3] == "no" else int(m[3]),
                                     "clock": int(m[4]), "line": number})
            continue
        if row == "already scheduled" and block is not None:
            block["already_scheduled"] = True
            continue
        if m := re.fullmatch(r"(total time|new basic block head|new basic block end) = (\d+)", row):
            if block is None:
                unavailable(number, raw, "block summary without basic block")
            else:
                key = {"total time": "total_time", "new basic block head": "new_head",
                       "new basic block end": "new_end"}[m[1]]
                block[key] = int(m[2])
            decision = None
            continue
        if (re.fullmatch(r"(?:added|deleted) \d+ line-number notes", row)
                or re.fullmatch(r"register \d+ life (?:shortened|extended) from -?\d+ to -?\d+", row)
                or re.fullmatch(r"register \d+ (?:now|no longer) crosses calls", row)):
            result["events"].append({"kind": "annotation", "line": number, "text": raw})
            continue
        # These belong to the RTL body, which the caller reads separately.
        if (row == "Insn is not within a basic block"
                or re.fullmatch(r"(?:Start|End) of basic block \d+\.?", row)
                or re.fullmatch(r"End of basic block \d+; start of basic block \d+\.", row)
                or row.startswith("Registers live:")):
            continue
        unavailable(number, raw, "unrecognized scheduler summary row", decision)

    for item in result["decisions"]:
        blocked = {entry["uid"] for entry in item["blocking"]}
        if item["chosen"] is None and not ({r["uid"] for r in item["ready"]} <= blocked):
            unavailable(item["line"], item["text"], "final selection order unavailable", item)
        if item["unavailable"]:
            item["chosen"] = None
    if result["blocks"]:
        result["status"] = "partial" if result["unavailable"] else "available"
    else:
        unavailable(start + 1, lines[start], "scheduler basic-block summaries unavailable")
    return result


def scheduling_competition(parsed: dict, uids: list[int]) -> list[dict]:
    """Collect decisions containing at least two distinct requested UIDs.

    Preserve the full ready/order evidence; ``queried`` adds base/current
    priorities. ``selection_order`` lists observed chosen UIDs for this block
    from this clock onwards (reverse of emitted order, not a prediction).
    """
    wanted = set(uids)
    competitions = []
    decisions = parsed.get("decisions", [])
    for index, decision in enumerate(decisions):
        queried = [item for item in decision["ready"] if item["uid"] in wanted]
        if len({item["uid"] for item in queried}) < 2:
            continue
        evidence = dict(decision)
        evidence["direction"] = "backward"
        evidence["queried"] = [dict(item, base_priority=parsed["instructions"].get(
            item["uid"], {}).get("base_priority")) for item in queried]
        queried_uids = {item["uid"] for item in queried}
        evidence["selection_order"] = [later["chosen"] for later in decisions[index:]
                                       if later["block"] == decision["block"]
                                       and later["chosen"] in queried_uids]
        competitions.append(evidence)
    return competitions
