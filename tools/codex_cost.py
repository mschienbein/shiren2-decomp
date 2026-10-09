#!/usr/bin/env python3
"""Read local Codex response usage, include descendants, and estimate API cost.

No network, model calls, subscription changes, or session/database writes.
Only explicitly supplied report paths are written. Rates are a dated snapshot.
"""

from __future__ import annotations

import argparse
from collections import Counter
from datetime import datetime, timezone
from decimal import Decimal
import json
from pathlib import Path
import sqlite3
from typing import Any

Json = dict[str, Any]
FIELDS = (
    "input_tokens", "cached_input_tokens", "cache_write_input_tokens",
    "output_tokens", "reasoning_output_tokens", "total_tokens",
)
DEFAULT_SCOPE = Path(__file__).resolve().parents[1] / "docs/api-cost-scope.json"


def normalize_time(value: str) -> str:
    moment = datetime.now(timezone.utc) if value == "now" else datetime.fromisoformat(value.replace("Z", "+00:00"))
    if moment.tzinfo is None:
        raise ValueError("Timestamps must include a timezone")
    return moment.astimezone(timezone.utc).isoformat(timespec="milliseconds").replace("+00:00", "Z")


def validate_usage(usage: Json) -> None:
    if any(type(usage.get(k, 0)) is not int or usage.get(k, 0) < 0 for k in FIELDS):
        raise ValueError("Missing/invalid token count")
    if any(k not in usage for k in FIELDS if k != "cache_write_input_tokens"):
        raise ValueError("Incomplete usage record")
    if usage["cached_input_tokens"] + usage.get("cache_write_input_tokens", 0) > usage["input_tokens"]:
        raise ValueError("Cached/write tokens exceed total input")
    if usage["reasoning_output_tokens"] > usage["output_tokens"]:
        raise ValueError("Reasoning exceeds total output")
    if usage["total_tokens"] != usage["input_tokens"] + usage["output_tokens"]:
        raise ValueError("Total tokens do not reconcile")


def token_sum(records: list[Json]) -> Json:
    result = {k: sum(r["usage"].get(k, 0) for r in records) for k in FIELDS}
    result["uncached_input_tokens"] = result["input_tokens"] - result["cached_input_tokens"] - result["cache_write_input_tokens"]
    return result


def discover(codex_dir: Path, seeds: dict[str, str]) -> tuple[dict[str, Json], dict[str, str]]:
    database = codex_dir / "state_5.sqlite"
    with sqlite3.connect(database.resolve().as_uri() + "?mode=ro", uri=True) as connection:
        connection.row_factory = sqlite3.Row
        connection.execute("PRAGMA query_only=ON")
        rows = {r["id"]: dict(r) for r in connection.execute("SELECT id,rollout_path,source,agent_path FROM threads")}
        edges = {(r[0], r[1]) for r in connection.execute("SELECT parent_thread_id,child_thread_id FROM thread_spawn_edges")}
    # Source metadata supplies a second, independent descendant inventory.
    for thread_id, row in rows.items():
        try:
            source = json.loads(row["source"])
        except (json.JSONDecodeError, TypeError):
            continue
        if isinstance(source, dict):
            parent = source.get("subagent", {}).get("thread_spawn", {}).get("parent_thread_id")
            if parent:
                edges.add((parent, thread_id))
    owners = dict(seeds)
    while True:
        new = {child: owners[parent] for parent, child in edges if parent in owners and child not in owners}
        if not new:
            break
        owners.update(new)
    missing = set(owners) - rows.keys()
    if missing:
        raise ValueError(f"Missing thread metadata: {sorted(missing)}")
    return {key: rows[key] for key in sorted(owners)}, owners


def read_session(row: Json, pool: str, seed: bool) -> tuple[list[Json], Json]:
    models: dict[str, set[str]] = {}
    records: dict[str, Json] = {}
    last_counter: Json | None = None
    tier: str | None = None
    duplicates = 0
    path = Path(row["rollout_path"])
    with path.open() as stream:
        for line_number, line in enumerate(stream, 1):
            try:
                event = json.loads(line)
            except json.JSONDecodeError as error:
                raise ValueError(f"Invalid JSON in {path.name}:{line_number}; retry after the writer finishes") from error
            payload = event.get("payload", {})
            kind = event.get("type")
            if kind == "turn_context":
                models.setdefault(payload["turn_id"], set()).add(payload["model"])
            elif kind == "event_msg" and payload.get("type") == "thread_settings_applied":
                settings = payload.get("thread_settings", {})
                if "service_tier" in settings:
                    tier = settings["service_tier"]
            elif kind == "token_usage_record":
                if payload["thread_id"] != row["id"]:
                    # Forked parent history is owned by its original session.
                    continue
                validate_usage(payload["usage"])
                response_id = payload.get("response_id")
                if not response_id:
                    raise ValueError(f"Missing response ID in {path.name}:{line_number}")
                record = {"response_id": response_id, "thread_id": row["id"], "turn_id": payload["turn_id"],
                          "timestamp": normalize_time(event["timestamp"]), "usage": payload["usage"],
                          "tier": tier, "pool": pool, "is_seed": seed}
                if response_id in records:
                    duplicates += 1
                    if records[response_id]["usage"] != record["usage"]:
                        raise ValueError(f"Conflicting usage for response {response_id}")
                else:
                    records[response_id] = record
                last_counter = payload["thread_token_usage"]
    result = list(records.values())
    for record in result:
        choices = models.get(record["turn_id"], set())
        if len(choices) != 1:
            raise ValueError(f"Missing/ambiguous model for {record['thread_id']} turn {record['turn_id']}")
        record["model"] = next(iter(choices))
    total = token_sum(result)
    reconciled = last_counter is not None and all(total[k] == last_counter.get(k, 0) for k in FIELDS)
    if not reconciled:
        raise ValueError(f"Response sum does not reconcile with the final thread counter: {row['id']}")
    return result, {"thread_id": row["id"], "pool": pool, "is_seed": seed, "agent_path": row["agent_path"],
                    "response_records": len(result), "duplicates_removed": duplicates,
                    "counter_reconciled": reconciled, "full_history_usage": total}


def aggregate(records: list[Json], pricing: Json) -> Json:
    totals = token_sum(records)
    components = {key: Decimal(0) for key in ("uncached_input", "cached_input", "cache_write", "output")}
    fast = no_cache = logged_low = logged_high = Decimal(0)
    long_requests = 0
    for record in records:
        usage = record["usage"]
        model = pricing["models"].get(record["model"])
        if model is None:
            raise ValueError(f"No verified price for model {record['model']}; update the scope pricing explicitly")
        long_context = usage["input_tokens"] > pricing["long_context_above_input_tokens"]
        long_requests += long_context
        rates = {key: Decimal(value) for key, value in model["standard_long" if long_context else "standard_short"].items()}
        write = usage.get("cache_write_input_tokens", 0)
        pieces = {
            "uncached_input": Decimal(usage["input_tokens"] - usage["cached_input_tokens"] - write) * rates["input"],
            "cached_input": Decimal(usage["cached_input_tokens"]) * rates["cached"],
            "cache_write": Decimal(write) * rates["cache_write"],
            "output": Decimal(usage["output_tokens"]) * rates["output"],
        }
        standard = sum(pieces.values()) / 1_000_000
        fast_cost = standard * Decimal(model["fast_multiplier"])
        fast += fast_cost
        no_cache += (Decimal(usage["input_tokens"]) * rates["input"] + Decimal(usage["output_tokens"]) * rates["output"]) / 1_000_000
        tier = record["tier"]
        if tier in ("priority", "fast"):
            logged_low += fast_cost
            logged_high += fast_cost
        elif tier in ("default", "standard"):
            logged_low += standard
            logged_high += standard
        elif tier is None:
            logged_low += standard
            logged_high += fast_cost
        else:
            raise ValueError(f"Unpriced service tier: {tier}")
        for key, value in pieces.items():
            components[key] += value / 1_000_000
    return {"responses": len(records), "usage": totals, "models": dict(Counter(r["model"] for r in records)),
            "recorded_request_tiers": dict(Counter(r["tier"] or "unknown" for r in records)),
            "long_context_requests": long_requests,
            "max_request_input_tokens": max((r["usage"]["input_tokens"] for r in records), default=0),
            "cost_usd": {"all_standard": float(sum(components.values())), "all_fast": float(fast),
                         "recorded_tiers_unknown_standard": float(logged_low),
                         "recorded_tiers_unknown_fast": float(logged_high),
                         "all_standard_no_cache": float(no_cache)},
            "standard_cost_components_usd": {key: float(value) for key, value in components.items()}}


def audit(scope: Json, codex_dir: Path, until: str) -> Json:
    start = normalize_time(scope["start"])
    seeds = scope["seed_threads"]
    rows, owners = discover(codex_dir, seeds)
    records: dict[str, Json] = {}
    sessions = []
    duplicates = 0
    for thread_id, row in rows.items():
        local, summary = read_session(row, owners[thread_id], thread_id in seeds)
        sessions.append(summary)
        duplicates += summary["duplicates_removed"]
        for record in local:
            key = record["response_id"]
            if key in records:
                duplicates += 1
                if any(records[key][k] != record[k] for k in ("usage", "thread_id", "model")):
                    raise ValueError(f"Conflicting response identity: {key}")
            else:
                records[key] = record
    all_records = sorted(records.values(), key=lambda r: r["timestamp"])
    scoped = [r for r in all_records if start <= r["timestamp"] < until]
    earlier = [r for r in all_records if r["timestamp"] < start]
    price = scope["pricing"]
    return {"schema_version": 1, "name": scope["name"], "generated_at": normalize_time("now"),
            "scope": {"start_inclusive": start, "end_exclusive": until, "seed_threads": seeds},
            "pricing": price, "session_count": len(sessions), "descendant_session_count": len(sessions) - len(seeds),
            "duplicates_removed": duplicates, "counter_checks_passed": len(sessions),
            "project": aggregate(scoped, price), "earlier_research": aggregate(earlier, price),
            "excluded_at_or_after_cutoff": {"responses": len(all_records) - len(scoped) - len(earlier)},
            "groups": {pool: aggregate([r for r in scoped if r["pool"] == pool], price) for pool in seeds.values()},
            "roles": {"seed_chats": aggregate([r for r in scoped if r["is_seed"]], price),
                      "descendant_agents": aggregate([r for r in scoped if not r["is_seed"]], price)},
            "days_UTC": {day: aggregate([r for r in scoped if r["timestamp"][:10] == day], price)
                         for day in sorted({r["timestamp"][:10] for r in scoped})},
            "sessions": sessions,
            "limitations": ["API-equivalent model tokens, not a subscription bill or measured API invoice.",
                            "Observed Codex caching is assumed for the API-equivalent scenarios; another harness can have different cache behavior.",
                            "Recorded tiers are requested settings, not server-confirmed delivery. Unknown tiers are bounded using Standard and Fast rates.",
                            "Coverage is the local seed/descendant graph; deleted, unlogged, remote, or other-harness usage is not reconstructed.",
                            "API hosted-tool fees, voice transcription, and local compute costs are excluded.",
                            "Rates are pinned to the verification date, not automatically refreshed."]}


def markdown(report: Json) -> str:
    project = report["project"]
    cost = project["cost_usd"]
    usage = project["usage"]
    price = report["pricing"]
    span = report["scope"]
    seed_count = len(span["seed_threads"])
    model_summary = ", ".join(f"{name}: {count:,} responses" for name, count in project["models"].items())
    rate_summary = "; ".join(
        f"{name} Standard short-context input ${model['standard_short']['input']}, cached input ${model['standard_short']['cached']}, output ${model['standard_short']['output']} per million tokens"
        for name, model in price["models"].items()
    )
    lines = ["# Shiren 2 API-equivalent cost", "",
             f"**${cost['all_standard']:,.2f} at Standard rates.** Using the recorded speed settings and bounding the unknown ones: **${cost['recorded_tiers_unknown_standard']:,.2f}–${cost['recorded_tiers_unknown_fast']:,.2f}**.", "",
             f"Scope: {span['start_inclusive']} through, excluding, {span['end_exclusive']}. The stored initial cutoff covers the first Shiren request through the start of the cost audit, excluding the audit. A refresh with `--until now` includes later usage in these sessions.", "",
             f"{report['session_count']} local sessions: {seed_count} seed chats and {report['descendant_session_count']} descendant agents. {project['responses']:,} distinct responses. Models: {model_summary}.", "",
             "| Token category | Tokens | Standard equivalent |", "| --- | ---: | ---: |"]
    for label, token_key, component in [("Uncached input", "uncached_input_tokens", "uncached_input"),
                                        ("Cached input", "cached_input_tokens", "cached_input"),
                                        ("Output, including reasoning", "output_tokens", "output")]:
        lines.append(f"| {label} | {usage[token_key]:,} | ${project['standard_cost_components_usd'][component]:,.2f} |")
    lines += ["", f"Total input plus output: {usage['total_tokens']:,} tokens. {usage['cached_input_tokens']/usage['input_tokens']:.2%} of input was cached. The {usage['reasoning_output_tokens']:,} reasoning tokens are already inside output and are charged once.", "",
             f"[Official pricing]({price['source']}), verified {price['verified_on']}: {rate_summary}. [Reasoning billing]({price['reasoning_source']}) includes reasoning in output. {project['long_context_requests']:,} scoped requests exceeded 272K input tokens; the largest input was {project['max_request_input_tokens']:,} tokens. Cache-write usage: {usage['cache_write_input_tokens']:,} tokens.", "",
              "| Pricing scenario | USD |", "| --- | ---: |",
              f"| All Standard, observed cache | ${cost['all_standard']:,.2f} |",
              f"| Recorded speeds, unknown requests Standard | ${cost['recorded_tiers_unknown_standard']:,.2f} |",
              f"| Recorded speeds, unknown requests Fast | ${cost['recorded_tiers_unknown_fast']:,.2f} |",
              f"| All Fast, observed cache | ${cost['all_fast']:,.2f} |",
              f"| All Standard, hypothetically no cache | ${cost['all_standard_no_cache']:,.2f} |", "",
              f"Recorded speed settings: {json.dumps(project['recorded_request_tiers'], sort_keys=True)}. Priority is priced as Fast. The recorded-speed interval concerns missing requested speed settings, not all possible billing differences.", "",
              "## Pool breakdown", "", "Each row includes that pool's coordinator and all its descendant agents. Labels are accounting groups; thread IDs are in the JSON report.", "",
              "| Group | Responses | Standard equivalent |", "| --- | ---: | ---: |"]
    for group, result in report["groups"].items():
        lines.append(f"| {group} | {result['responses']:,} | ${result['cost_usd']['all_standard']:,.2f} |")
    lines += ["", f"Across all pools, the {seed_count} seed chats account for ${report['roles']['seed_chats']['cost_usd']['all_standard']:,.2f}; descendant agents account for ${report['roles']['descendant_agents']['cost_usd']['all_standard']:,.2f}.", "",
              f"The earlier general/Diablo decomp research adds ${report['earlier_research']['cost_usd']['all_standard']:,.2f} at Standard rates. Including it gives ${cost['all_standard']+report['earlier_research']['cost_usd']['all_standard']:,.2f}.", "",
              "## Verification and limits", "",
              f"Each of the {report['counter_checks_passed']} session response sums reconciled with its final cumulative thread counter. {report['duplicates_removed']} duplicate response records were removed. Both spawn edges and source metadata were used to inventory descendants. Repeated cumulative token-count events were not summed.", "",
              f"[Codex subscription pricing]({price['subscription_source']}) is separate from API pricing. This report values the observed tokens at API rates; it does not allocate subscription fees or report extra charges.", ""]
    lines += [f"- {limitation}" for limitation in report["limitations"]]
    lines += ["", "## Refresh", "", "Run from the Shiren project directory:", "", "```sh",
              "python3 tools/codex_cost.py --until now --json scratch/api-cost-latest.json --markdown scratch/api-cost-latest.md",
              "```", "", "Omit `--until now` to reproduce the original project cutoff. The script only reads Codex state/logs and writes the explicitly supplied report paths. Add future worker chat IDs to `docs/api-cost-scope.json`; descendants are discovered automatically. Verify and update the dated rates when changing models or pricing.", ""]
    return "\n".join(lines)


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--scope", type=Path, default=DEFAULT_SCOPE)
    parser.add_argument("--codex-dir", type=Path, default=Path.home() / ".codex")
    parser.add_argument("--until", help="Exclusive UTC/offset timestamp, or 'now'; default is the original audit boundary")
    parser.add_argument("--json", type=Path)
    parser.add_argument("--markdown", type=Path)
    args = parser.parse_args()
    scope = json.loads(args.scope.read_text())
    until = normalize_time(args.until or scope["default_until"])
    if until <= normalize_time(scope["start"]):
        parser.error("End must be after start")
    report = audit(scope, args.codex_dir, until)
    for path, content in [(args.json, json.dumps(report, indent=2) + "\n"), (args.markdown, markdown(report))]:
        if path:
            path.parent.mkdir(parents=True, exist_ok=True)
            path.write_text(content)
    print(json.dumps({"sessions": report["session_count"], "responses": report["project"]["responses"],
                      "cost_usd": report["project"]["cost_usd"], "cutoff": until}, indent=2))


if __name__ == "__main__":
    main()
