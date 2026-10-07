#!/usr/bin/env python3
"""Integrator: certify the current canonical tree and record accepted progress.

    uv run --frozen python tools/accept.py prepare --name omp-b2
    # a different agent reviews: verify.py <run>-a/-b --snapshot-only, reads the new
    # sources, writes scratch/omp/accept/<name>/review.json (see REVIEW_FIELDS)
    uv run --frozen python tools/accept.py finalize --name omp-b2

`prepare` runs two fresh serial full-ROM builds (`<name>-a`, `<name>-b`), then the
whole test suite against `<name>-b`, and writes `scratch/omp/accept/<name>/prepare.json`
listing the new matches relative to the accepted receipt. `finalize` requires an
independent `review.json` bound to both receipts, checks the tree is unchanged,
promotes `<name>-b` (full re-verification), runs default verification, writes
`docs/harness-validation-<name>.json` and records progress.
"""
from __future__ import annotations

import argparse
import hashlib
import json
import os
import re
import subprocess
import sys
import time
from datetime import datetime, timezone
from pathlib import Path

from certification import python_command, read_json
from rom import EXPECTED_SHA256, PROJECT
from workspace import verify_manifest

REVIEW_FIELDS = {
    "status": "PASS or PASS_QUALIFIED... after your own checks (anything else blocks acceptance)",
    "receipt_hashes": "{'<name>-a': sha256(receipt.json), '<name>-b': sha256(receipt.json)}",
    "current_frozen_manifest_sha256": "receipt['input_manifest']['sha256'] of <name>-b",
    "coverage": "receipt['coverage'] of <name>-b, copied exactly",
    "reviewed_functions": "list of new symbols whose source you read",
    "checks": "commands you ran with exit codes",
    "findings": "source-quality notes; blockers make status FAIL",
}


def _sha256(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def _run(argv: list[str], log: Path, env: dict[str, str] | None = None) -> tuple[int, float]:
    started = time.monotonic()
    with log.open("w", encoding="utf-8") as stream:
        stream.write(json.dumps(argv) + "\n")
        stream.flush()
        code = subprocess.run(argv, cwd=PROJECT, stdout=stream, stderr=subprocess.STDOUT, env=env).returncode
    return code, round(time.monotonic() - started, 3)


def _reference(path: Path) -> dict[str, str]:
    return {"path": path.resolve().relative_to(PROJECT).as_posix(), "sha256": _sha256(path)}


def prepare(name: str) -> Path:
    work = PROJECT / "scratch/omp/accept" / name
    work.mkdir(parents=True, exist_ok=False)
    receipts = {}
    for suffix in ("a", "b"):
        run = f"{name}-{suffix}"
        code, elapsed = _run([python_command(), "tools/build.py", "--name", run, "--no-promote"], work / f"build-{suffix}.log")
        if code:
            raise ValueError(f"build {run} failed (exit {code}); see {work / f'build-{suffix}.log'}")
        receipts[run] = {"path": f"build/{run}/receipt.json", "sha256": _sha256(PROJECT / "build" / run / "receipt.json"), "elapsed_seconds": elapsed}
    b = PROJECT / "build" / f"{name}-b/receipt.json"
    suite_log = work / "suite.log"
    code, elapsed = _run([python_command(), "-m", "unittest", "discover", "-s", "tests", "-v"], suite_log,
                         env={**os.environ, "SHIREN2_RECEIPT": str(b)})
    text = suite_log.read_text()
    ran = re.search(r"^Ran (\d+) tests? in", text, re.MULTILINE)
    receipt = read_json(b)
    accepted = read_json(PROJECT / read_json(PROJECT / "build/latest.json")["receipt"])
    before = {(m.get("image_id", "resident"), m["symbol"]) for m in accepted["c_matches"]}
    new = [m for m in receipt["c_matches"] if (m.get("image_id", "resident"), m["symbol"]) not in before]
    summary = {
        "schema_version": 1, "name": name, "prepared_at": datetime.now(timezone.utc).isoformat(),
        "receipts": receipts, "frozen_manifest_sha256": receipt["input_manifest"]["sha256"],
        "coverage": receipt["coverage"],
        "prior_accepted": read_json(PROJECT / "build/latest.json"),
        "new_functions": [{k: m[k] for k in ("image_id", "symbol", "source", "rom_start", "size")} for m in new],
        "new_instruction_bytes": sum(m["size"] for m in new),
        "suite": {"exit_code": code, "tests": int(ran[1]) if ran else 0, "elapsed_seconds": elapsed,
                  "passed": code == 0 and bool(ran) and "OK" in text.splitlines()[-1], "log": _reference(suite_log)},
        "review_required": {"path": f"scratch/omp/accept/{name}/review.json", "fields": REVIEW_FIELDS},
    }
    (work / "prepare.json").write_text(json.dumps(summary, indent=2) + "\n")
    return work / "prepare.json"


def finalize(name: str, function_reviews: list[Path]) -> Path:
    work = PROJECT / "scratch/omp/accept" / name
    prepared = read_json(work / "prepare.json")
    if not prepared["suite"]["passed"]:
        raise ValueError("The prepared suite did not pass")
    b_path = PROJECT / prepared["receipts"][f"{name}-b"]["path"]
    a_path = PROJECT / prepared["receipts"][f"{name}-a"]["path"]
    receipt = read_json(b_path)
    for run, record in prepared["receipts"].items():
        if _sha256(PROJECT / record["path"]) != record["sha256"]:
            raise ValueError(f"Receipt changed since prepare: {run}")
    review_path = work / "review.json"
    review = read_json(review_path)
    if (not str(review.get("status", "")).startswith("PASS")
            or review.get("receipt_hashes") != {run: record["sha256"] for run, record in prepared["receipts"].items()}
            or review.get("current_frozen_manifest_sha256") != receipt["input_manifest"]["sha256"]
            or review.get("coverage") != receipt["coverage"]):
        raise ValueError("review.json does not pass or does not bind both prepared receipts, manifest and coverage")
    reviewed = set(review.get("reviewed_functions", []))
    for path in function_reviews:
        shard = read_json(path)
        if (not str(shard.get("status", "")).startswith("PASS")
                or shard.get("receipt_hashes") != review["receipt_hashes"]
                or not str(shard.get("reviewer", "")).strip()):
            raise ValueError(f"{path} does not pass, binds different receipts or names no reviewer")
        reviewed |= set(shard.get("reviewed_functions", []))
    unreviewed = sorted({f["symbol"] for f in prepared["new_functions"]} - reviewed)
    if unreviewed:
        raise ValueError(f"{len(unreviewed)} new function(s) have no source review: {unreviewed[:10]}")
    verify_manifest(PROJECT, receipt["input_manifest"])
    prior = read_json(PROJECT / "build/latest.json")
    code, _ = _run([python_command(), "tools/verify.py", str(b_path.relative_to(PROJECT)), "--promote"], work / "promote.log")
    if code:
        raise ValueError(f"promotion failed (exit {code}); see {work / 'promote.log'}")
    default_code, _ = _run([python_command(), "tools/verify.py"], work / "default-verify.log")
    if default_code:
        raise ValueError(f"default verification failed (exit {default_code})")
    coverage = receipt["coverage"]
    validation = {
        "schema_version": 1, "kind": "accepted-v3-integration-checkpoint",
        "recorded_at": datetime.now(timezone.utc).isoformat(), "target_sha256": EXPECTED_SHA256, "status": "accepted",
        "accepted_receipt": {"path": prepared["receipts"][f"{name}-b"]["path"], "sha256": prepared["receipts"][f"{name}-b"]["sha256"]},
        "companion_receipt": {"path": prepared["receipts"][f"{name}-a"]["path"], "sha256": _sha256(a_path)},
        "prior_accepted_receipt": {"path": prior["receipt"], "sha256": prior["receipt_sha256"]},
        "frozen_manifest": {"sha256": receipt["input_manifest"]["sha256"], "file_count": len(receipt["input_manifest"]["files"]),
                            "raw_file_sha256": _sha256(b_path.parent / "input-manifest.json")},
        "coverage": {"matched_c_functions": coverage["matched_c_functions"], "matched_c_instruction_bytes": coverage["matched_c_bytes"],
                     "complete_game_denominator_bytes": None},
        "new_C_functions": len(prepared["new_functions"]),
        "new_unique_C_instruction_bytes": prepared["new_instruction_bytes"],
        "new_functions": prepared["new_functions"],
        "full_suite": {"command": "SHIREN2_RECEIPT=<b> python -m unittest discover -s tests -v", "tests": prepared["suite"]["tests"],
                       "passed": True, "exit_code": prepared["suite"]["exit_code"], "elapsed_seconds": prepared["suite"]["elapsed_seconds"],
                       "log": prepared["suite"]["log"]["path"], "log_sha256": prepared["suite"]["log"]["sha256"]},
        "independent_whole_rom_review": _reference(review_path),
        "independent_new_function_reviews": {path.stem: _reference(path) for path in function_reviews} or {"whole-batch": _reference(review_path)},
        "promotion": {"serial": True, "command": f"tools/verify.py {b_path.relative_to(PROJECT)} --promote", "exit_code": code,
                      "default_verification_exit_code": default_code, "source_current_at_acceptance": True,
                      "latest_pointer_sha256": _sha256(PROJECT / "build/latest.json"),
                      "promotion_log": _reference(work / "promote.log"), "default_verification_log": _reference(work / "default-verify.log")},
        "workflow": "docs/OMP_WORKFLOW.md (tools/match.py -> tools/adopt.py -> tools/accept.py)",
        "limits": ["Coverage is the provisional mapped CPU catalogue; complete-game denominator unknown",
                   "Review covers source policy and readability; byte equality is proven by the receipts"],
    }
    destination = PROJECT / "docs" / f"harness-validation-{name}.json"
    if destination.exists():
        raise ValueError(f"{destination} already exists")
    destination.write_text(json.dumps(validation, indent=2) + "\n")
    code, _ = _run([python_command(), "tools/progress.py", "record", "--validation", str(destination.relative_to(PROJECT))], work / "progress-record.log")
    if code:
        raise ValueError(f"progress record failed (exit {code}); see {work / 'progress-record.log'}")
    return destination


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = parser.add_subparsers(dest="command", required=True)
    first = sub.add_parser("prepare")
    first.add_argument("--name", required=True)
    second = sub.add_parser("finalize")
    second.add_argument("--name", required=True)
    second.add_argument("--function-review", type=Path, action="append", default=[])
    args = parser.parse_args()
    if re.fullmatch(r"[a-zA-Z0-9][a-zA-Z0-9_-]*", args.name) is None:
        parser.error("name must contain only letters, digits, underscores and hyphens")
    try:
        if args.command == "prepare":
            path = prepare(args.name)
            data = read_json(path)
            print(json.dumps({k: data[k] for k in ("receipts", "coverage", "new_instruction_bytes", "suite")}, indent=2))
            print(f"prepared: {path}\nnew functions: {len(data['new_functions'])}; review file required: {data['review_required']['path']}")
            sys.exit(0 if data["suite"]["passed"] else 1)
        destination = finalize(args.name, args.function_review)
        print(f"accepted: {destination}")
        subprocess.run([python_command(), "tools/progress.py", "show"], cwd=PROJECT)
    except (ValueError, OSError, KeyError) as error:
        parser.exit(2, f"error: {error}\n")


if __name__ == "__main__":
    main()
