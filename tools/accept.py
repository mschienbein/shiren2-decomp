#!/usr/bin/env python3
"""Integrator: certify the current canonical tree and record accepted progress.

    uv run --frozen python tools/accept.py prepare --name omp-b2
    # a different agent reviews: verify.py <run>-a/-b --snapshot-only, reads the new
    # and changed sources, writes scratch/omp/accept/<name>/review.json (see REVIEW_FIELDS)
    uv run --frozen python tools/accept.py finalize --name omp-b2

`prepare` runs two fresh full-ROM builds (`<name>-a`, `<name>-b`) concurrently
(`--sequential-builds`: one after the other), then the whole test suite against
`<name>-b`, and writes `scratch/omp/accept/<name>/prepare.json`
listing new and source/dependency-changed matches relative to the accepted receipt. `finalize` requires an
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
from concurrent.futures import ThreadPoolExecutor
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
    "reviewed_functions": "list of new and changed symbols whose source you read",
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


def _function_changes(receipt: dict, accepted: dict) -> tuple[list[dict], list[dict]]:
    """Compare function identities and frozen TU dependency closures, not live files."""
    before = {(m.get("image_id", "resident"), m["symbol"]): m for m in accepted["c_matches"]}
    prior_files = accepted["input_manifest"]["files"]
    current_files = receipt["input_manifest"]["files"]
    new, changed = [], []
    for match in receipt["c_matches"]:
        prior = before.get((match.get("image_id", "resident"), match["symbol"]))
        if prior is None:
            new.append(match)
        else:
            prior_source, current_source = prior["source"], match["source"]
            prior_dependencies = {path: prior_files[path]["sha256"]
                                  for path in accepted["dependencies"][prior_source]}
            current_dependencies = {path: current_files[path]["sha256"]
                                    for path in receipt["dependencies"][current_source]}
            changed_dependencies = sorted(path for path in prior_dependencies.keys() | current_dependencies.keys()
                                          if prior_dependencies.get(path) != current_dependencies.get(path))
            if (prior_source != current_source
                    or prior_files[prior_source]["sha256"] != current_files[current_source]["sha256"]
                    or changed_dependencies):
                changed.append({**match, "changed_dependencies": changed_dependencies})
    return new, changed


def _build_environment(run: str) -> dict[str, str] | None:
    """Inherit the environment; a valid opt-in shared replay-evidence root gets one subdirectory per run.

    The suffix is added only when the original root passes the replay helpers' own checks
    (absolute, not a symlink). Any other non-empty value is passed through unchanged, so
    certification.retained_trial and record_observed_output reject it exactly as before.
    """
    root = os.environ.get("SHIREN2_REPLAY_EVIDENCE_ROOT")
    if not root or not Path(root).is_absolute() or Path(root).is_symlink():
        return None
    return {**os.environ, "SHIREN2_REPLAY_EVIDENCE_ROOT": str(Path(root) / run)}


def _fresh_builds(name: str, work: Path, concurrent: bool) -> dict[str, dict]:
    """Build `<name>-a` and `<name>-b`; any failure raises before a receipt is recorded.

    Each build owns `build/<run>/` (snapshot, splat output, TMPDIR, replay evidence)
    and its own `build-<suffix>.log`. A successful build's receipt is hashed as part of
    that build's result. Sequential mode is the old loop: a non-zero exit or a receipt
    error stops it before B starts. Concurrent mode waits for both builds; then, in A-then-B
    order, the first failed build decides: a raised exception (e.g. its receipt cannot be
    read) is re-raised, and a non-zero exit raises a ValueError naming every non-zero build.
    """
    logs = {f"{name}-{suffix}": work / f"build-{suffix}.log" for suffix in ("a", "b")}
    commands = {run: [python_command(), "tools/build.py", "--name", run, "--no-promote"] for run in logs}

    def build(run: str) -> tuple[int, dict | None]:
        code, elapsed = _run(commands[run], logs[run], env=_build_environment(run))
        if code:
            return code, None
        receipt = f"build/{run}/receipt.json"
        return code, {"path": receipt, "sha256": _sha256(PROJECT / receipt), "elapsed_seconds": elapsed}

    outcomes: dict[str, tuple[int, dict | None] | BaseException] = {}
    if concurrent:
        with ThreadPoolExecutor(max_workers=len(logs)) as pool:
            futures = {run: pool.submit(build, run) for run in logs}
        outcomes = {run: future.exception() or future.result() for run, future in futures.items()}
    else:
        for run in logs:
            outcomes[run] = build(run)
            if outcomes[run][0]:
                break
    failures = [f"build {run} failed (exit {outcome[0]}); see {logs[run]}"
                for run, outcome in outcomes.items() if not isinstance(outcome, BaseException) and outcome[0]]
    for run, outcome in outcomes.items():
        if isinstance(outcome, BaseException):
            raise outcome
        if outcome[0]:
            raise ValueError("; ".join(failures))
    return {run: record for run, (_, record) in outcomes.items()}


def prepare(name: str, concurrent_builds: bool = True) -> Path:
    work = PROJECT / "scratch/omp/accept" / name
    work.mkdir(parents=True, exist_ok=False)
    started = time.monotonic()
    receipts = _fresh_builds(name, work, concurrent_builds)
    builds = {"mode": "concurrent" if concurrent_builds else "sequential",
              "wall_seconds": round(time.monotonic() - started, 3)}
    b = PROJECT / "build" / f"{name}-b/receipt.json"
    suite_log = work / "suite.log"
    code, elapsed = _run([python_command(), "-m", "unittest", "discover", "-s", "tests", "-v"], suite_log,
                         env={**os.environ, "SHIREN2_RECEIPT": str(b)})
    text = suite_log.read_text()
    ran = re.search(r"^Ran (\d+) tests? in", text, re.MULTILINE)
    receipt = read_json(b)
    accepted = read_json(PROJECT / read_json(PROJECT / "build/latest.json")["receipt"])
    new, changed = _function_changes(receipt, accepted)
    summary = {
        "schema_version": 1, "name": name, "prepared_at": datetime.now(timezone.utc).isoformat(),
        "receipts": receipts, "builds": builds, "frozen_manifest_sha256": receipt["input_manifest"]["sha256"],
        "coverage": receipt["coverage"],
        "prior_accepted": read_json(PROJECT / "build/latest.json"),
        "new_functions": [{k: m[k] for k in ("image_id", "symbol", "source", "rom_start", "size")} for m in new],
        "changed_functions": [{k: m[k] for k in ("image_id", "symbol", "source", "rom_start", "size", "changed_dependencies")} for m in changed],
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
    unreviewed = sorted({f["symbol"] for f in prepared["new_functions"] + prepared["changed_functions"]} - reviewed)
    if unreviewed:
        raise ValueError(f"{len(unreviewed)} new or changed function(s) have no source review: {unreviewed[:10]}")
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
                     **({"matched_cpp_functions": coverage["matched_cpp_functions"], "matched_cpp_instruction_bytes": coverage["matched_cpp_bytes"],
                         "matched_c_and_cpp_functions": coverage["matched_c_and_cpp_functions"],
                         "matched_c_and_cpp_instruction_bytes": coverage["matched_c_and_cpp_bytes"]}
                        if "matched_cpp_functions" in coverage else {}),
                     "complete_game_denominator_bytes": None},
        "new_C_functions": sum(m["source"].endswith(".c") for m in prepared["new_functions"]),
        "new_unique_C_instruction_bytes": sum(m["size"] for m in prepared["new_functions"] if m["source"].endswith(".c")),
        **({"new_CPP_functions": sum(m["source"].endswith(".cpp") for m in prepared["new_functions"]),
            "new_unique_CPP_instruction_bytes": sum(m["size"] for m in prepared["new_functions"] if m["source"].endswith(".cpp"))}
           if "matched_cpp_functions" in coverage else {}),
        "new_functions": prepared["new_functions"],
        "changed_functions": prepared["changed_functions"],
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
    first.add_argument("--sequential-builds", action="store_true",
                       help="finish <name>-a before starting <name>-b (default: build both concurrently)")
    second = sub.add_parser("finalize")
    second.add_argument("--name", required=True)
    second.add_argument("--function-review", type=Path, action="append", default=[])
    args = parser.parse_args()
    if re.fullmatch(r"[a-zA-Z0-9][a-zA-Z0-9_-]*", args.name) is None:
        parser.error("name must contain only letters, digits, underscores and hyphens")
    try:
        if args.command == "prepare":
            path = prepare(args.name, concurrent_builds=not args.sequential_builds)
            data = read_json(path)
            print(json.dumps({k: data[k] for k in ("receipts", "builds", "coverage", "new_instruction_bytes", "suite")}, indent=2))
            print(f"prepared: {path}\nnew functions: {len(data['new_functions'])}; changed functions: {len(data['changed_functions'])}; review file required: {data['review_required']['path']}")
            sys.exit(0 if data["suite"]["passed"] else 1)
        destination = finalize(args.name, args.function_review)
        print(f"accepted: {destination}")
        subprocess.run([python_command(), "tools/progress.py", "show"], cwd=PROJECT)
    except (ValueError, OSError, KeyError) as error:
        parser.exit(2, f"error: {error}\n")


if __name__ == "__main__":
    main()
