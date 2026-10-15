#!/usr/bin/env python3
"""Measured search ladder for one near-miss function: explain -> hypothesis beam -> pinned permuter.

    uv run --frozen python tools/permute.py func_X cand.c --profile P --hypotheses H.json --seed 1
    uv run --frozen python tools/permute.py func_X cand.c --profile P --hypotheses H.json \
        --beam-width 2 --beam-depth 2 --permuter-candidates 400 --permuter-passes lifetime,order --jobs 2

Adapted from the PermuterInfra wrapper (scratch/omp/permuter/tools/permute.py): the same
`prepare()` mapping (base.c from match.check's preprocessed TU, compile.sh = match.check's exact
compile/assemble/link, target.o = the ROM slot linked at the original VRAM) and the same
function splice; `match.check` remains the only verdict. What changed:

1. Rung 0 (explain): `match.check` on the seed plus canonical `explain.collect` (diagnostic only).
2. Rung 1 (beam): a bounded, deterministic beam over the author's *explicit* hypotheses
   (`--hypotheses`, JSON). Each hypothesis is one measured lifetime/order idea with a mechanism
   citation and a semantic-equivalence argument, expressed as exact find/replace edits inside the
   target function. Nothing is generated or guessed: no token or flag lottery.
3. Rung 2 (permuter): the pinned decomp-permuter checkout, driven by `tools/permuter_engine.py`
   with a fixed seed and candidate limit, each candidate from the base, and only the passes of
   the requested hypothesis classes enabled (see PASS_CLASSES). Every candidate is recorded.
4. Re-check: the best non-forbidden improvements are spliced into the seed and run through the
   ordinary `match.check`.
5. Verdict: `EXACT_NEEDS_REVIEW` (a normal `match`/`match-with-data` re-check at the exact size;
   *not* adoption: policy, semantic and independent review still decide), `NEEDS_DATA`, or
   `TROUBLE` with the best result and the complete cause/attempt ledger.

Search-scope restrictions (tools/source_policy.py; not C legality rules, and passing them proves no
semantics) apply wherever source appears, and fail closed:
- Lexical, on hypothesis text before compilation and on every function: inline asm, `##`, new
  `volatile`/`__volatile__`/`register`, `#pragma`/`__attribute__`, new constant-condition
  `if`/`switch`/loops/`do ... while (<constant>)` (0, 0U, 1-1, ...), identical branches, empty or
  self assignments, comma dummies, `^ 0`/`* 0`.
- Syntactic, on the pinned pycparser AST of each candidate's whole preprocessed TU: the same
  families with macros expanded, including typedef casts and enumerators in constant conditions.
  Volatile is resolved with C scope rules through typedefs, derived types, struct/union members and
  object references; a use whose type cannot be resolved counts as `volatile_unverifiable` (fail
  closed). A source the parser cannot read is `rejected-unverifiable`. New casts count only for
  permuter candidates.
Rejected rows stay in the ledger and are never ranked. A zero permuter score is never a verdict;
identical bytes do not prove meaningful legal C.

Executable inputs (tools/pinned_tree.py): the decomp-permuter checkout must equal the pinned commit
file-for-file (git objects verified by content hash, no git process), and the toml dependency is
located without being executed and must equal its pinned file hashes. Only then are they imported,
and only from the verified in-memory bytes: no bytecode cache is read or written for them, here or
in the engine. The engine's own bytecode namespace is a fresh directory inside the run.

Durable attempts: `ledger.jsonl` registers every attempt (baseline, explain, beam child, permuter
engine, re-check) with its identity, fsynced before the attempt's work starts. Each attempt gets at
most one terminal line: completed with its row, or aborted with the reason and any result file it
had already produced. The engine's `events.jsonl` does the same for each candidate's generation and
evaluation. Recovery reads only these files: an attempt without a completed line is incomplete,
never completed and never unattempted. An interrupt writes `interrupted.json` from them.

Child processes (tools/owned_process.py; the H14 r4 ownership invariant): the engine runs as an
owned group. Exit is observed without reaping, and every group signal precedes the reap. The group is
always killed on exit 0, nonzero exit, timeout and interrupt, and is never signalled after
retirement. A guard in the child kills its own group if this process dies.

Exit status: 0 EXACT_NEEDS_REVIEW, 1 TROUBLE or NEEDS_DATA, 2 usage/setup error, 130 interrupted
(SIGINT/SIGTERM/SIGHUP). Requires an absolute private TMPDIR. Heavy use belongs under
scratch/omp/heavy.py, jobs <= 2.
"""
from __future__ import annotations

import argparse
import contextlib
import hashlib
import json
import os
import platform
import re
import shlex
import shutil
import signal
import subprocess
import sys
import time
from pathlib import Path

HERE = Path(__file__).resolve().parent
_TOOLS = HERE if (HERE / "match.py").is_file() else next(p / "tools" for p in HERE.parents if (p / "tools/match.py").is_file())
sys.path.insert(0, str(_TOOLS))

from certification import compiler_environment, profile_toolchain, tool_profile  # noqa: E402
from match import DEFAULT_PROFILES, accepted_build, check, resolve_targets  # noqa: E402
from rom import CANONICAL_ROM, PROJECT  # noqa: E402

sys.path.insert(0, str(HERE))
import pinned_tree  # noqa: E402
import source_policy as policy  # noqa: E402
from owned_process import OWNER_FD, run_owned  # noqa: E402

PERMUTER_COMMIT = "8556c81d80d1c1af98a858c8f4dc951357f29139"
DEFAULT_PERMUTER = PROJECT / "scratch/omp/permuter/decomp-permuter"
ENGINE = HERE / "permuter_engine.py"
TOML = "toml==0.10.2"
ADOPTABLE = {"match", "match-with-data"}
STATUS_RANK = {"match": 0, "match-with-data": 0, "match-needs-data": 1, "mismatch-data": 2, "mismatch": 3,
               "overflow": 4, "policy": 5, "error": 6}
HYPOTHESIS_CLASSES = {"lifetime", "order"}
# Permuter passes per hypothesis class. Everything else (dead blocks, masks, casts, type
# changes, inline helpers, padding, AST removal, arithmetic tricks) stays at weight 0.
PASS_CLASSES = {
    "lifetime": ["perm_temp_for_expr", "perm_expand_expr", "perm_split_assignment", "perm_remove_var",
                 "perm_chain_assignment"],
    "order": ["perm_reorder_stmts", "perm_reorder_decls", "perm_commutative", "perm_reverse_cond",
              "perm_compound_assignment"],
}
GRACE_SECONDS = 30.0


def sha(data: bytes | str) -> str:
    return hashlib.sha256(data.encode() if isinstance(data, str) else data).hexdigest()


# ---------------------------------------------------------------------------------------------
# Source helpers.

def function_span(text: str, symbol: str) -> tuple[int, int]:
    """[start, end) of the top-level definition of `symbol` (return type line included)."""
    for found in re.finditer(rf"\b{re.escape(symbol)}\s*\(", text):
        depth, i = 1, found.end()
        while i < len(text) and depth:
            depth += {"(": 1, ")": -1}.get(text[i], 0)
            i += 1
        body = re.compile(r"\s*\{").match(text, i)
        if not body:
            continue
        start = text.rfind("\n", 0, found.start()) + 1
        if not text[start:found.start()].strip():
            start = text.rfind("\n", 0, start - 1) + 1
        i, depth = body.end(), 1
        while depth:
            literal = re.compile(r'"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'|/\*.*?\*/|//[^\n]*', re.S).match(text, i)
            if literal:
                i = literal.end()
                continue
            depth += {"{": 1, "}": -1}.get(text[i], 0)
            i += 1
        return start, i
    raise ValueError(f"no definition of {symbol}")


def apply_edits(text: str, symbol: str, edits: list[dict]) -> str:
    """Apply exact find/replace edits inside the target function; each find must occur once."""
    start, end = function_span(text, symbol)
    body = text[start:end]
    for edit in edits:
        occurrences = body.count(edit["find"])
        if occurrences != 1:
            raise ValueError(f"edit find text occurs {occurrences} times in {symbol}")
        body = body.replace(edit["find"], edit["replace"], 1)
    return text[:start] + body + text[end:]


def load_hypotheses(path: Path, symbol: str) -> list[dict]:
    document = json.loads(path.read_text())
    if document.get("function") != symbol:
        raise ValueError(f"hypotheses file is for {document.get('function')}, not {symbol}")
    hypotheses = document["hypotheses"]
    ids = set()
    for h in hypotheses:
        missing = {"id", "class", "mechanism", "semantics", "edits"} - set(h)
        if missing:
            raise ValueError(f"hypothesis lacks {sorted(missing)}")
        if h["id"] in ids:
            raise ValueError(f"duplicate hypothesis id {h['id']}")
        ids.add(h["id"])
        if h["class"] not in HYPOTHESIS_CLASSES:
            raise ValueError(f"{h['id']}: class must be one of {sorted(HYPOTHESIS_CLASSES)}")
        if not h["mechanism"].strip() or not h["semantics"].strip() or not h["edits"]:
            raise ValueError(f"{h['id']}: mechanism, semantics and edits are required")
        for edit in h["edits"]:
            if set(edit) != {"find", "replace"} or not edit["find"]:
                raise ValueError(f"{h['id']}: each edit is exactly {{find, replace}}")
            bad = policy.forbidden_delta(policy.lexical_counts(edit["find"]), policy.lexical_counts(edit["replace"]))
            if bad:
                raise ValueError(f"{h['id']}: replacement introduces forbidden artifacts {bad}")
    return hypotheses


# ---------------------------------------------------------------------------------------------
# Identity, evaluation, ledger.

def permuter_identity(path: Path) -> tuple[dict, dict[str, bytes]]:
    """Enforced pin: the checkout must equal PERMUTER_COMMIT file-for-file (tools/pinned_tree.py).
    Returns the evidence and the verified bytes; the Parser and the engine execute only those bytes.
    The 59-file digest of importable sources is kept as evidence only, never as the check."""
    try:
        verified, blobs = pinned_tree.verify_checkout(path, PERMUTER_COMMIT)
    except (pinned_tree.PinError, OSError) as error:
        raise ValueError(f"decomp-permuter at {path} is not the pinned checkout: {error}") from None
    files = sorted((k for k in blobs if k == "default_weights.toml"
                    or (k.endswith(".py") and k.split("/")[0] in ("src", "perm_pycparser"))), key=lambda k: k.split("/"))
    digest = sha("".join(f"{k}\0{sha(blobs[k])}\n" for k in files))
    return {"path": str(path), **verified, "evidence_digest_59": {"sha256": digest, "files": len(files)}}, blobs


class Parser:
    """Pinned pycparser over a candidate's preprocessed TU. Its modules run only from the bytes that
    permuter_identity verified (pinned_tree.VerifiedImporter); no bytecode cache is consulted."""

    def __init__(self, permuter: Path, blobs: dict[str, bytes]):
        importer = pinned_tree.install_verified({"perm_pycparser": (permuter, blobs), "src": (permuter, blobs)})
        from perm_pycparser import c_ast
        from src import ast_util
        self.served = importer.audit()
        self.ca, self.ast_util = c_ast, ast_util

    def counts(self, preprocessed: str, function: str, *, count_casts: bool) -> dict[str, int]:
        text = "".join(line for line in preprocessed.splitlines(keepends=True) if not line.startswith("#"))
        return policy.ast_counts(self.ca, self.ast_util.to_c, self.ast_util.parse_c(text), function,
                                 count_casts=count_casts)


def identity(args, targets, profile_id: str, rom: bytes, build: Path, hypotheses_path: Path | None,
             permuter: dict | None) -> dict:
    profile = tool_profile(PROJECT)
    assigned = profile["profiles"][profile_id]
    progress = json.loads((PROJECT / "docs/progress.json").read_text())
    return {
        "tool": {name: sha((HERE / name).read_bytes()) for name in
                 ("permute.py", "permuter_engine.py", "owned_process.py", "pinned_tree.py", "source_policy.py")}
        | {"match.py": sha((_TOOLS / "match.py").read_bytes()), "explain.py": sha((_TOOLS / "explain.py").read_bytes())},
        "function": args.function, "image_id": targets[0].image_id, "rom_start": targets[0].rom_start,
        "instruction_bytes": targets[0].size, "slot_end": targets[0].slot_end,
        "seed_source": {"path": str(args.source), "sha256": sha(args.source.read_bytes())},
        "hypotheses": {"path": str(hypotheses_path), "sha256": sha(hypotheses_path.read_bytes())} if hypotheses_path else None,
        "profile": {"id": profile_id, "tool_profile_id": profile["id"], "assigned": assigned,
                    "compiler": profile["compilers"].get(assigned.get("compiler"), profile["compiler"]),
                    "assembler": profile["tools"].get(assigned.get("assembler"))},
        "rom_sha256": sha(rom), "accepted_build": str(build),
        "accepted_receipt_sha256": sha((build / "receipt.json").read_bytes()),
        "denominator": progress["denominator"],
        "permuter": permuter,
        "search": {"seed": args.seed, "beam_width": args.beam_width, "beam_depth": args.beam_depth,
                   "beam_candidates": args.beam_candidates, "permuter_candidates": args.permuter_candidates,
                   "permuter_passes": args.permuter_passes, "mutations": args.mutations, "jobs": args.jobs,
                   "recheck_top": args.recheck_top, "permuter_seconds": args.permuter_seconds},
        "python": sys.version.split()[0], "platform": platform.platform(), "TMPDIR": os.environ["TMPDIR"],
    }


STOP_SIGNALS = (signal.SIGINT, signal.SIGTERM, signal.SIGHUP)


def create_exclusive(path: Path) -> int:
    """A new append-only file (never an existing one), with its directory entry fsynced."""
    fd = os.open(path, os.O_WRONLY | os.O_CREAT | os.O_EXCL | os.O_APPEND, 0o644)
    directory = os.open(path.parent, os.O_RDONLY)
    try:
        os.fsync(directory)
    finally:
        os.close(directory)
    return fd


def write_line(fd: int, record: dict) -> None:
    view = memoryview((json.dumps(record, sort_keys=True, default=str) + "\n").encode())
    while view:
        view = view[os.write(fd, view):]
    os.fsync(fd)


class Ledger:
    """Write-ahead attempt ledger (`ledger.jsonl`). Every attempt is registered with
    `attempt-started` and its identity before any of its work starts, and gets at most one terminal
    line: `attempt-completed` with its row, or `attempt-aborted` with the reason and any result file
    it had produced. Lines carry a consecutive `seq`, are fsynced before the ladder moves on, and are
    committed with stop signals held off, so an interrupt never tears a line or skips bookkeeping."""

    def __init__(self, path: Path):
        self.fd = create_exclusive(path)
        self.seq = 0
        self.started: dict[int, dict] = {}
        self.closed: set[int] = set()
        self.rows: list[dict] = []  # completed rows: ranking, report and TROUBLE.md

    def _commit(self, record: dict) -> None:
        previous = signal.pthread_sigmask(signal.SIG_BLOCK, STOP_SIGNALS)
        try:
            write_line(self.fd, {"seq": self.seq, **record})
            self.seq += 1
            if record["event"] == "attempt-started":
                self.started[record["attempt"]] = record
            else:
                self.closed.add(record["attempt"])
                if record["event"] == "attempt-completed":
                    self.rows.append({k: v for k, v in record.items() if k != "event"})
        finally:
            signal.pthread_sigmask(signal.SIG_SETMASK, previous)

    def start(self, stage: str, **identity) -> int:
        attempt = len(self.started)
        self._commit({"event": "attempt-started", "attempt": attempt, "stage": stage, **identity})
        return attempt

    def complete(self, attempt: int, row: dict) -> dict:
        if attempt in self.closed:
            raise RuntimeError(f"attempt {attempt} already has a terminal line")
        self._commit({"event": "attempt-completed", **row, "attempt": attempt})
        return self.rows[-1]

    def abort(self, attempt: int, error: BaseException, **evidence) -> None:
        if attempt in self.closed:
            return
        out = self.started[attempt].get("out")
        result = Path(out) / "result.json" if out else None
        present = result is not None and result.is_file()
        self._commit({"event": "attempt-aborted", "attempt": attempt, "reason": f"{type(error).__name__}: {error}"[:300],
                      "result_json": str(result) if result else None, "result_json_present": present,
                      "result_json_sha256": sha(result.read_bytes()) if present else None, **evidence})


@contextlib.contextmanager
def attempt_scope(ledger: Ledger, stage: str, **identity):
    """Register an attempt before its work; abort it (and re-raise) on any exception or interrupt."""
    number = ledger.start(stage, **identity)
    try:
        yield number
    except BaseException as error:
        ledger.abort(number, error)
        raise
    if number not in ledger.closed:
        raise RuntimeError(f"{stage} attempt {number} ended without a terminal line")


def read_jsonl(path: Path) -> tuple[list[dict], str | None]:
    """Committed lines of a ledger/events file, with consecutive `seq` from 0. A final line without
    its newline (a write cut short by a kill) is reported as torn and not trusted; any other damage
    is corruption."""
    lines = (path.read_bytes() if path.is_file() else b"").split(b"\n")
    torn = lines.pop()
    rows = []
    for number, line in enumerate(lines):
        try:
            row = json.loads(line)
        except json.JSONDecodeError:
            raise ValueError(f"{path} line {number + 1} is corrupt") from None
        if row.get("seq") != number:
            raise ValueError(f"{path} line {number + 1} has seq {row.get('seq')}, expected {number}")
        rows.append(row)
    return rows, torn.decode(errors="replace")[:200] if torn else None


def ledger_progress(path: Path) -> dict:
    """The attempt history from the committed ledger alone. An attempt without a completed line is
    incomplete (aborted or cut off), never completed and never unattempted; its identity and any
    result file it had produced are kept, and that file is never ranked."""
    rows, torn = read_jsonl(path)
    started: dict[int, dict] = {}
    terminal: dict[int, dict] = {}
    for row in rows:
        event, number = row["event"], row["attempt"]
        if event == "attempt-started":
            if number != len(started):
                raise ValueError(f"{path}: attempt {number} registered out of order")
            started[number] = row
        elif event in ("attempt-completed", "attempt-aborted"):
            if number not in started or number in terminal:
                raise ValueError(f"{path}: {event} for attempt {number} without a single registration")
            terminal[number] = row
        else:
            raise ValueError(f"{path}: unknown event {event}")
    incomplete = []
    for number, row in started.items():
        end = terminal.get(number)
        if end is not None and end["event"] == "attempt-completed":
            continue
        result = Path(row["out"]) / "result.json" if row.get("out") else None
        present = result is not None and result.is_file()
        incomplete.append({"attempt": number, "stage": row["stage"],
                           "identity": {k: v for k, v in row.items() if k not in ("seq", "event", "attempt", "stage")},
                           "aborted": end["reason"] if end else None, "result_json": str(result) if result else None,
                           "result_json_present": present,
                           "result_json_sha256": sha(result.read_bytes()) if present else None})
    done = {n for n, r in terminal.items() if r["event"] == "attempt-completed"}
    stages = sorted({r["stage"] for r in started.values()})
    return {"ledger_jsonl": str(path), "ledger_sha256": sha(path.read_bytes()) if path.is_file() else None,
            "attempts_started": len(started), "completed": len(done), "incomplete": incomplete,
            "by_stage": {s: {"started": sum(r["stage"] == s for r in started.values()),
                             "completed": sum(started[n]["stage"] == s for n in done),
                             "incomplete": sum(r["stage"] == s for r in incomplete)} for s in stages},
            "torn_final_line": torn}


def rank(row: dict) -> tuple:
    exact_size = row.get("text_bytes") == row.get("target_bytes")
    words = row.get("words")
    return (STATUS_RANK.get(row.get("status"), 9), words if words is not None else 10**6,
            0 if exact_size else 1, row.get("order", 0))


def policy_check(ctx: dict, function_text: str, preprocessed: Path, *, stage: str) -> tuple[list[str], dict]:
    """Lexical + syntactic search-scope restrictions; returns (forbidden, evidence). Fails closed."""
    lexical = policy.lexical_counts(function_text)
    evidence = {"lexical": lexical, "ast": None, "ast_status": "not-applicable (C++ baseline-only run)"}
    if ctx["parser"] is not None:
        try:
            evidence["ast"] = ctx["parser"].counts(preprocessed.read_text(), ctx["function"],
                                                   count_casts=stage == "permuter")
            evidence["ast_status"] = "checked"
        except Exception as error:  # noqa: BLE001 - an unreadable source is never certified clean
            evidence["ast_status"] = f"unverifiable: {type(error).__name__}: {error}"[:300]
    if stage == "baseline":
        if ctx["parser"] is not None and evidence["ast_status"] != "checked":
            raise ValueError(f"seed cannot be parsed by the pinned C parser: {evidence['ast_status']}")
        ctx["seed_lexical"], ctx["seed_ast"] = lexical, evidence["ast"]
        ctx["seed_artifacts"] = {**policy.seed_artifacts(lexical), **policy.seed_artifacts(evidence["ast"] or {})}
    forbidden = set(policy.forbidden_delta(ctx["seed_lexical"], lexical))
    if evidence["ast"] is not None:
        forbidden |= set(policy.forbidden_delta(ctx["seed_ast"], evidence["ast"]))
    return sorted(forbidden), evidence


def evaluate(ctx: dict, attempt: int, source: Path, out: Path, **extra) -> dict:
    """match.check plus the source policy for a registered attempt; the row is its completed line."""
    result, _ = check(ctx["targets"], source, ctx["profile"], out, includes=ctx["includes"],
                      source_root=ctx["source_root"], rom=ctx["rom"], build=ctx["build"])
    function = source.read_text()
    start, end = function_span(function, ctx["function"])
    forbidden, evidence = policy_check(ctx, function[start:end], out / "candidate.i", stage=extra["stage"])
    row = {"source": str(source), "source_sha256": sha(source.read_bytes()), "status": result.status,
           "words": result.differing_words, "text_bytes": result.text_bytes,
           "target_bytes": result.instruction_bytes, "exact_size": result.text_bytes == result.instruction_bytes,
           "problems": result.problems[:4], "result_json": str(out / "result.json"),
           "forbidden": forbidden, "policy": evidence, **extra}
    if row["forbidden"]:
        row["classification"] = "rejected-forbidden"
    elif ctx["parser"] is not None and evidence["ast_status"] != "checked":
        row["classification"] = "rejected-unverifiable"
    elif result.status in ADOPTABLE and row["exact_size"] and ctx["seed_artifacts"]:
        row["classification"] = "exact-but-inherits-seed-artifacts"
    elif result.status in ADOPTABLE and row["exact_size"]:
        row["classification"] = "exact-normal-match-needs-review"
    elif result.status == "match-needs-data":
        row["classification"] = "text-match-needs-data"
    elif result.status in {"policy", "overflow"}:
        row["classification"] = "policy-or-size"
    else:
        row["classification"] = result.status
    return ctx["attempts"].complete(attempt, row)


def explain_summary(ctx: dict, source: Path, out: Path) -> dict:
    from explain import collect
    result, _ = check(ctx["targets"], source, ctx["profile"], out, includes=ctx["includes"],
                      source_root=ctx["source_root"], rom=ctx["rom"], build=ctx["build"])
    report = collect(result, ctx["targets"], source, ctx["profile"], out, includes=ctx["includes"],
                     source_root=ctx["source_root"], rom=ctx["rom"])
    (out / "explain.json").write_text(json.dumps(report, indent=1, default=str) + "\n")
    return {"status": report.get("status"), "differences": len(report.get("differences", [])),
            "unavailable": len(report.get("unavailable", [])), "explain_json": str(out / "explain.json")}


# ---------------------------------------------------------------------------------------------
# Rungs.

def beam(ctx: dict, args, hypotheses: list[dict], baseline: dict) -> list[dict]:
    ledger = ctx["attempts"]
    frontier = [{"row": baseline, "chain": [], "text": args.source.read_text()}]
    seen = {baseline["source_sha256"]}
    produced = 0
    order = 0
    for depth in range(1, args.beam_depth + 1):
        children = []
        for parent in frontier:
            for h in hypotheses:
                if h["id"] in parent["chain"] or produced >= args.beam_candidates:
                    continue
                chain = [*parent["chain"], h["id"]]
                name = f"d{depth}-{'+'.join(chain)}"
                path, out = ctx["out"] / "beam" / f"{name}.c", ctx["out"] / "beam" / name
                with attempt_scope(ledger, "beam", depth=depth, chain=chain, source=str(path), out=str(out)) as number:
                    try:
                        text = apply_edits(parent["text"], args.function, h["edits"])
                    except ValueError as error:
                        ledger.complete(number, {"stage": "beam", "depth": depth, "chain": chain,
                                                 "classification": "inapplicable", "reason": str(error)})
                        continue
                    digest = sha(text)
                    if digest in seen:
                        ledger.complete(number, {"stage": "beam", "depth": depth, "chain": chain,
                                                 "classification": "duplicate", "source_sha256": digest})
                        continue
                    seen.add(digest)
                    produced += 1
                    order += 1
                    path.parent.mkdir(parents=True, exist_ok=True)
                    path.write_text(text)
                    row = evaluate(ctx, number, path, out, stage="beam", depth=depth, chain=chain,
                                   causes=[x["mechanism"] for x in hypotheses if x["id"] in chain], order=order)
                if not row["classification"].startswith("rejected"):
                    children.append({"row": row, "chain": chain, "text": text})
        if not children:
            break
        frontier = sorted(children, key=lambda c: rank(c["row"]))[: args.beam_width]
    return sorted((r for r in ctx["ledger"] if r.get("stage") == "beam" and "status" in r), key=rank)


def engine_progress(events: Path, candidates: int) -> dict:
    """Engine progress from the committed events.jsonl alone. Every index is in exactly one class:
    a terminal outcome; generation incomplete (started, never committed; index and rng_seed kept);
    evaluation incomplete (started or not); or generation never started. Nothing is relabelled."""
    rows, torn = read_jsonl(events)
    gen_started: dict[int, int] = {}
    generated: dict[int, dict] = {}
    eval_started: set[int] = set()
    terminal: dict[int, str] = {}
    for row in rows:
        event, index = row["event"], row.get("index")
        if event == "generation-started":
            if index != len(gen_started):
                raise ValueError(f"{events}: generation of candidate {index} started out of order")
            gen_started[index] = row["rng_seed"]
        elif event == "generated":
            if gen_started.get(index) != row["rng_seed"] or index in generated:
                raise ValueError(f"{events}: candidate {index} generated without its generation-started")
            generated[index] = row
            if "outcome" in row:
                terminal[index] = row["outcome"]
        elif event == "evaluation-started":
            if index not in generated or index in terminal or index in eval_started:
                raise ValueError(f"{events}: evaluation of candidate {index} started out of order")
            eval_started.add(index)
        elif event == "evaluated":
            if index not in eval_started or index in terminal:
                raise ValueError(f"{events}: candidate {index} evaluated without its evaluation-started")
            terminal[index] = row["outcome"]
    pending = sorted(i for i in generated if i not in terminal)
    return {
        "events_jsonl": str(events), "events_sha256": sha(events.read_bytes()) if events.is_file() else None,
        "started": any(r["event"] == "start" for r in rows), "complete": any(r["event"] == "complete" for r in rows),
        "candidates": candidates, "generation_started": len(gen_started), "generated": len(generated),
        "incomplete_generation": [{"index": i, "rng_seed": s} for i, s in gen_started.items() if i not in generated],
        "generation_not_started": candidates - len(gen_started),
        "outcomes": {k: sum(v == k for v in terminal.values()) for k in sorted(set(terminal.values()))},
        "incomplete_evaluation_started": [i for i in pending if i in eval_started],
        "incomplete_evaluation_not_started": [i for i in pending if i not in eval_started],
        "torn_final_line": torn,
    }


def permuter_rung(ctx: dict, args, seed_row: dict) -> dict:
    ledger = ctx["attempts"]
    seed_path = Path(seed_row["source"])
    perm_root = ctx["out"] / "permuter"
    events = perm_root / "engine/events.jsonl"
    with attempt_scope(ledger, "permuter-engine", seed_source=str(seed_path), seed_sha256=sha(seed_path.read_bytes()),
                       candidates=args.permuter_candidates, seed=args.seed, events=str(events)) as number:
        base = perm_root / "base-check"
        result, _ = check(ctx["targets"], seed_path, ctx["profile"], base, includes=ctx["includes"],
                          source_root=ctx["source_root"], rom=ctx["rom"], build=ctx["build"])
        if not (base / "link.ld").is_file():
            ledger.complete(number, {"stage": "permuter-engine", "classification": "unavailable", "reason": "seed does not link"})
            return {"status": "unavailable", "reason": f"seed does not link: {result.problems[:2]}"}
        perm = perm_root / "perm"
        prepare(perm, base, ctx["targets"], ctx["profile"], ctx["rom"])
        pycache = perm / "pycache"
        pycache.mkdir()  # fresh and exclusive to this run: the engine's stdlib/tool bytecode namespace
        allowed = sorted({p for cls in args.permuter_passes for p in PASS_CLASSES[cls]})
        profile = tool_profile(PROJECT)
        config = {"permuter": str(args.permuter.resolve()), "commit": PERMUTER_COMMIT, "perm_dir": str(perm),
                  "out_dir": str(perm_root / "engine"), "function": ctx["function"], "allowed_passes": allowed,
                  "candidates": args.permuter_candidates, "seed": args.seed, "mutations": args.mutations,
                  "jobs": args.jobs, "emit_top": args.recheck_top,
                  "objdump_command": f"{profile['tools']['objdump']['path']} -drz -m mips:4300"}
        (perm_root / "engine-config.json").write_text(json.dumps(config, indent=2) + "\n")
        env = {**os.environ, "UV_OFFLINE": "1", "TMPDIR": str(perm / "tmp")}
        argv = ["uv", "run", "--no-project", "--offline", "--with", TOML, "python", "-B", "-E", "-s",
                "-X", f"pycache_prefix={pycache}", str(ENGINE), str(perm_root / "engine-config.json"), OWNER_FD]
        try:
            run = run_owned(argv, perm_root / "engine.log", cwd=args.permuter.resolve(), env=env,
                            timeout=args.permuter_seconds, grace=GRACE_SECONDS)
        except KeyboardInterrupt as interrupt:
            ledger.abort(number, interrupt, progress=engine_progress(events, args.permuter_candidates))
            raise
        summary_path = perm_root / "engine/summary.json"
        progress = engine_progress(events, args.permuter_candidates)
        if run["exit_code"] != 0 or not summary_path.is_file() or not progress["complete"]:
            tail = (perm_root / "engine.log").read_text(errors="replace")[-1500:]
            ledger.complete(number, {"stage": "permuter-engine", "classification": "unavailable", "run": run,
                                     "progress": progress, "log_tail": tail[-400:]})
            return {"status": "unavailable", "reason": "engine failed or timed out", "run": run, "progress": progress,
                    "log_tail": tail}
        summary = json.loads(summary_path.read_text())
        candidates = perm_root / "engine/candidates.jsonl"
        ledger.complete(number, {"stage": "permuter-engine", "classification": "score-only-not-a-result", "run": run,
                                 "candidates_jsonl": str(candidates), "candidates_sha256": sha(candidates.read_bytes()),
                                 "base_score": summary["base_score"], "best_score": summary["best_score"],
                                 "outcomes": summary["outcomes"], "passes": summary["weights_nonzero"], "seed": args.seed,
                                 "progress": progress})
    seed_text = seed_path.read_text()
    start, end = function_span(seed_text, ctx["function"])
    rechecks = []
    for name in summary["emitted"]:
        index = int(name.split("-")[1][:-2])
        meta = next(m for m in summary["improved"] if m["index"] == index)
        spliced = perm_root / "recheck" / f"{name[:-2]}_spliced.c"
        out = perm_root / "recheck" / name[:-2]
        with attempt_scope(ledger, "permuter", candidate_index=index, rng_seed=meta["rng_seed"],
                           function_sha256=meta["function_sha256"], permuter_score=meta["score"], passes=meta["passes"],
                           source=str(spliced), out=str(out)) as number:
            permuted = (perm_root / "engine" / name).read_text()
            p_start, p_end = function_span(permuted, ctx["function"])
            spliced.parent.mkdir(parents=True, exist_ok=True)
            spliced.write_text(seed_text[:start] + permuted[p_start:p_end] + seed_text[end:])
            rechecks.append(evaluate(ctx, number, spliced, out, stage="permuter", candidate_index=index,
                                     permuter_score=meta["score"], passes=meta["passes"], rng_seed=meta["rng_seed"],
                                     semantics="UNVERIFIED: permuter edits are not dependency-checked", order=10_000 + index))
    return {"status": "ran", "run": run, "summary": summary, "rechecks": sorted(rechecks, key=rank)}


def prepare(perm: Path, base: Path, targets, profile_id: str, rom: bytes) -> None:
    """PermuterInfra mapping: base.c, target.o, compile.sh, settings.toml (unchanged semantics)."""
    first, last = targets[0], targets[-1]
    perm.mkdir(parents=True, exist_ok=True)
    (perm / "tmp").mkdir(exist_ok=True)
    profile = tool_profile(PROJECT)
    gcc, assembler = profile_toolchain(profile, profile_id)
    assigned = profile["profiles"][profile_id]
    ld, objdump = profile["tools"]["ld"]["path"], profile["tools"]["objdump"]["path"]
    emulation = profile["ld_emulation"]
    preprocessed = (base / "candidate.i").read_text()
    (perm / "base.c").write_text("".join(line for line in preprocessed.splitlines(keepends=True) if not line.startswith("#")))
    shutil.copyfile(base / "link.ld", perm / "link.ld")
    original = rom[first.rom_start:last.slot_end]
    words = "\n".join(f"    .word 0x{original[i:i + 4].hex()}" for i in range(0, len(original), 4))
    (perm / "target.s").write_text(f".set noreorder\n.set noat\n.text\n.globl {first.symbol}\n.type {first.symbol}, @function\n"
                                   f"{first.symbol}:\n{words}\n")
    assignments = [line for line in (base / "link.ld").read_text().splitlines() if re.fullmatch(r"[\w.$]+ = 0x[0-9A-F]+;", line)]
    (perm / "target.ld").write_text("\n".join([f"SECTIONS {{\n  .text 0x{first.vram_start:X} : {{ target_raw.o(.text) }}\n"
                                               "  /DISCARD/ : { *(*) }\n}", *assignments]) + "\n")
    environment = compiler_environment(perm / "tmp", profile)
    subprocess.run([assembler, *assigned["assembler_flags"], "-o", "target_raw.o", "target.s"], cwd=perm, env=environment, check=True)
    subprocess.run([ld, "-m", emulation, "-T", "target.ld", "-e", "0", "-o", "target.o", "target_raw.o"], cwd=perm, env=environment, check=True)
    env = " ".join(shlex.quote(f"{k}={v}") for k, v in environment.items() if k != "TMPDIR")
    q = lambda argv: " ".join(shlex.quote(str(a)) for a in argv)  # noqa: E731
    (perm / "compile.sh").write_text(f"""#!/bin/bash
# Generated by tools/permute.py: same commands as tools/match.py check() for {profile_id}.
set -e
IN="$(cd "$(dirname "$1")" && pwd)/$(basename "$1")"
OUT="$(cd "$(dirname "$3")" && pwd)/$(basename "$3")"
WORK="$(mktemp -d {shlex.quote(str(perm / 'tmp'))}/c.XXXXXX)"
trap 'rm -rf "$WORK"' EXIT
cd "$WORK"
cp "$IN" candidate.c
E=(env -i {env} "TMPDIR=$WORK")
"${{E[@]}}" {q(gcc)} {q(assigned['compiler_flags'])} -S candidate.c -o candidate.s
"${{E[@]}}" {q([assembler])} {q(assigned['assembler_flags'])} -o candidate.o candidate.s
"${{E[@]}}" {q([ld])} -m {emulation} -T {shlex.quote(str(perm / 'link.ld'))} -e 0 -o "$OUT" candidate.o
""")
    (perm / "compile.sh").chmod(0o755)
    (perm / "settings.toml").write_text(f'func_name = "{first.symbol}"\ncompiler_type = "gcc"\n'
                                        f'objdump_command = "{objdump} -drz -m mips:4300"\n')


def trouble(ctx: dict, baseline: dict, best: dict, best_stated: dict, permuter: dict | None, hypotheses: list[dict],
            explain: dict | None) -> str:
    rows = [r for r in ctx["ledger"] if "status" in r]
    lines = [f"# TROUBLE: {ctx['function']} ({ctx['profile']})", "",
             f"No ordinary exact match. Baseline {baseline['status']} {baseline['words']} words "
             f"({baseline['text_bytes']}/{baseline['target_bytes']} B). Best {best['status']} {best['words']} words "
             f"({best['text_bytes']}/{best['target_bytes']} B, {best.get('stage')}, {best['source_sha256'][:12]})"
             + (" -- a permuter edit with UNVERIFIED semantics (statement moves are not dependency-checked)."
                if best.get("stage") == "permuter" else "."),
             f"Best with a stated (not proven) semantic argument that passed the source policy (baseline/beam): "
             f"{best_stated['status']} {best_stated['words']} words "
             f"({best_stated['text_bytes']}/{best_stated['target_bytes']} B, {'+'.join(best_stated.get('chain', [])) or 'baseline'}).",
             "", "## Attempt ledger", "", "| stage | id / passes | class | status | words | bytes | semantics | forbidden |",
             "|---|---|---|---|---|---|---|---|"]
    for r in sorted(rows, key=rank):
        label = "+".join(r.get("chain", [])) or ",".join(r.get("passes", [])) or "baseline"
        semantics = ("rejected" if r["classification"].startswith("rejected")
                     else "unverified" if r.get("stage") == "permuter" else "stated")
        lines.append(f"| {r.get('stage')} | {label} | {r['classification']} | {r['status']} | {r['words']} | "
                     f"{r['text_bytes']}/{r['target_bytes']} | {semantics} | {','.join(r.get('forbidden', [])) or '-'} |")
    other = [r for r in ctx["ledger"] if "status" not in r]
    if other:
        lines += ["", f"Other ledger records (inapplicable, duplicate, permuter-engine summaries): {len(other)}; see ledger.jsonl."]
    lines += ["", "## Causes tried", ""] + [f"- `{h['id']}` ({h['class']}): {h['mechanism']}" for h in hypotheses]
    if permuter:
        if permuter["status"] == "ran":
            s = permuter["summary"]
            lines += ["", "## Permuter rung", "",
                      f"Base score {s['base_score']}, best {s['best_score']} over {s['candidates']} candidates; outcomes {s['outcomes']}.",
                      f"Passes enabled: {', '.join(s['weights_nonzero'])}.",
                      "Score-only improvements are not results; only the re-checks above count."]
        else:
            progress = permuter.get("progress") or {}
            lines += ["", "## Permuter rung", "", f"Unavailable: {permuter['reason']}",
                      f"Durable progress: generation started for {progress.get('generation_started', 0)} of "
                      f"{progress.get('candidates')} ({len(progress.get('incomplete_generation') or [])} never committed: "
                      f"incomplete), {progress.get('generation_not_started')} never started; outcomes "
                      f"{progress.get('outcomes')}; evaluation incomplete after starting "
                      f"{progress.get('incomplete_evaluation_started')}, not started "
                      f"{len(progress.get('incomplete_evaluation_not_started') or [])}; events {progress.get('events_jsonl')}."]
    if explain:
        lines += ["", "## Baseline explain", "", f"{explain}"]
    lines += ["", "## Handoff", "",
              "The search was bounded and deterministic; nothing here is adoptable. Next: a new measured",
              "hypothesis grounded in the explain evidence (pseudo dispositions, priorities, sched2 order),",
              "not a wider lottery. Every candidate and result is in this directory."]
    return "\n".join(lines) + "\n"


def record_interruption(out: Path, error: BaseException, candidates: int) -> Path:
    """interrupted.json, reconstructed from the committed ledger and engine events only."""
    events = out / "permuter/engine/events.jsonl"
    summary: dict = {"verdict": "INTERRUPTED", "reason": f"{type(error).__name__}: {error}",
                     "note": "Reconstructed from the committed ledger.jsonl and engine events.jsonl only. Incomplete "
                             "attempts are neither completed nor unattempted; result files they produced are kept "
                             "and never ranked."}
    for key, read in (("ledger_progress", lambda: ledger_progress(out / "ledger.jsonl")),
                      ("engine_progress", lambda: engine_progress(events, candidates) if events.is_file() else None)):
        try:
            summary[key] = read()
        except ValueError as problem:
            summary[key] = {"unreadable": str(problem)}
    path = out / "interrupted.json"
    previous = signal.pthread_sigmask(signal.SIG_BLOCK, STOP_SIGNALS)
    try:
        fd = create_exclusive(path)
        try:
            view = memoryview((json.dumps(summary, indent=2, default=str) + "\n").encode())
            while view:
                view = view[os.write(fd, view):]
            os.fsync(fd)
        finally:
            os.close(fd)
    finally:
        signal.pthread_sigmask(signal.SIG_SETMASK, previous)
    return path


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("function")
    parser.add_argument("source", type=Path)
    parser.add_argument("--image", choices=sorted(DEFAULT_PROFILES))
    parser.add_argument("--profile")
    parser.add_argument("-I", dest="includes", type=Path, action="append", default=[])
    parser.add_argument("--source-root", type=Path, default=PROJECT)
    parser.add_argument("--hypotheses", type=Path, help="explicit measured hypotheses (JSON); omit to skip the beam")
    parser.add_argument("--seed", type=int, required=True, help="deterministic permuter seed")
    parser.add_argument("--beam-width", type=int, default=2)
    parser.add_argument("--beam-depth", type=int, default=2)
    parser.add_argument("--beam-candidates", type=int, default=24, help="max beam candidates evaluated")
    parser.add_argument("--permuter-candidates", type=int, default=0, help="permuter candidate limit (0: skip rung)")
    parser.add_argument("--permuter-passes", default="lifetime,order", help="hypothesis classes enabling permuter passes")
    parser.add_argument("--mutations", type=int, default=1, help="randomization passes per permuter candidate (1-3)")
    parser.add_argument("--permuter-seconds", type=float, default=1800, help="hard wall-time bound for the engine child")
    parser.add_argument("--jobs", type=int, default=2, choices=(1, 2))
    parser.add_argument("--recheck-top", type=int, default=5)
    parser.add_argument("--no-explain", action="store_true")
    parser.add_argument("--permuter", type=Path, default=DEFAULT_PERMUTER)
    parser.add_argument("--out", type=Path)
    args = parser.parse_args()

    def stop(signum, _frame):
        raise KeyboardInterrupt(f"interrupted by {signal.Signals(signum).name}")

    for signum in STOP_SIGNALS:  # one interrupt path for every rung (run_owned restores these after the engine)
        signal.signal(signum, stop)
    run_dir = None  # set once this run owns its output directory (exclusive ledger created)
    try:
        tmp = os.environ.get("TMPDIR", "")
        if not tmp or not Path(tmp).is_absolute():
            raise ValueError("an absolute private TMPDIR is required")
        if not 1 <= args.mutations <= 3 or args.beam_width < 1 or args.beam_depth < 0 or args.permuter_candidates < 0:
            raise ValueError("bounds: mutations 1..3, beam width >= 1, depth >= 0, candidates >= 0")
        args.permuter_passes = sorted(set(filter(None, args.permuter_passes.split(","))))
        if set(args.permuter_passes) - HYPOTHESIS_CLASSES:
            raise ValueError(f"--permuter-passes must be a subset of {sorted(HYPOTHESIS_CLASSES)}")
        if args.source.suffix != ".c" and (args.permuter_candidates or (args.hypotheses and args.beam_depth)):
            raise ValueError("the beam and permuter rungs need the pinned C parser for their source policy; "
                             "C++ runs are baseline + explain only (--beam-depth 0 --permuter-candidates 0)")
        targets = resolve_targets([args.function], args.image)
        if len(targets) != 1:
            raise ValueError("single-function targets only")
        profile_id = args.profile or DEFAULT_PROFILES[targets[0].image_id]
        hypotheses = load_hypotheses(args.hypotheses, args.function) if args.hypotheses else []
        out = (args.out or PROJECT / "scratch/omp/search" / args.function / time.strftime("%Y%m%dT%H%M%S")).resolve()
        if out.exists() and any(out.iterdir()):
            raise ValueError(f"{out} exists and is not empty; runs are never overwritten")
        out.mkdir(parents=True, exist_ok=True)
        attempts = Ledger(out / "ledger.jsonl")
        run_dir = out
        rom, build = CANONICAL_ROM.read_bytes(), accepted_build()
        seed_copy = out / f"seed{args.source.suffix}"
        shutil.copyfile(args.source, seed_copy)
        is_c = args.source.suffix == ".c"
        permuter_ident, blobs = permuter_identity(args.permuter.resolve()) if is_c else (None, None)
        ctx = {"targets": targets, "profile": profile_id, "includes": args.includes, "source_root": args.source_root,
               "rom": rom, "build": build, "out": out, "attempts": attempts, "ledger": attempts.rows,
               "function": args.function, "parser": Parser(args.permuter.resolve(), blobs) if is_c else None}
        if is_c:
            permuter_ident["parser_served_modules"] = ctx["parser"].served
        (out / "identity.json").write_text(json.dumps(identity(args, targets, profile_id, rom, build, args.hypotheses,
                                                               permuter_ident), indent=2, default=str) + "\n")
        with attempt_scope(attempts, "baseline", source=str(seed_copy), source_sha256=sha(seed_copy.read_bytes()),
                           out=str(out / "baseline")) as number:
            baseline = evaluate(ctx, number, seed_copy, out / "baseline", stage="baseline", order=0)
        explain = None
        if not args.no_explain:
            with attempt_scope(attempts, "explain", source=str(seed_copy), out=str(out / "baseline-explain")) as number:
                explain = explain_summary(ctx, seed_copy, out / "baseline-explain")
                attempts.complete(number, {"stage": "explain", "classification": "diagnostic", "explain": explain})
        print(f"baseline: {baseline['status']} {baseline['words']} words; explain {explain}")
        args.source = seed_copy
        beam_rows = beam(ctx, args, hypotheses, baseline) if hypotheses and args.beam_depth else []
        for r in beam_rows:
            print(f"beam {'+'.join(r['chain'])}: {r['classification']} {r['status']} {r['words']} words {r['text_bytes']}/{r['target_bytes']}")
        legal = [r for r in [baseline, *beam_rows] if not r["classification"].startswith("rejected")]
        seed_row = min(legal, key=rank)
        permuter = permuter_rung(ctx, args, seed_row) if args.permuter_candidates else None
        if permuter and permuter["status"] == "ran":
            s_ = permuter["summary"]
            print(f"permuter: base score {s_['base_score']}, best {s_['best_score']}, outcomes {s_['outcomes']}")
            for r in permuter["rechecks"]:
                print(f"recheck #{r['candidate_index']} score {r['permuter_score']}: {r['classification']} {r['status']} {r['words']} words")
        elif permuter:
            print(f"permuter: unavailable ({permuter['reason']})")
        evaluated = [r for r in ctx["ledger"] if "status" in r and not r["classification"].startswith("rejected")]
        best = min(evaluated, key=rank)
        # A stated argument is not a proof; rows must also have passed the syntactic policy check.
        stated = [r for r in evaluated if r.get("stage") in ("baseline", "beam")
                  and (r["policy"]["ast_status"] == "checked" or r.get("stage") == "baseline")]
        best_stated = min(stated, key=rank)
        exact = [r for r in evaluated if r["classification"] == "exact-normal-match-needs-review" and r is not baseline]
        verdict = ("EXACT_NEEDS_REVIEW" if exact else "BASELINE_ALREADY_MATCHES" if baseline["classification"] == "exact-normal-match-needs-review"
                   else "NEEDS_DATA" if best["classification"] == "text-match-needs-data" else "TROUBLE")
        report = {"verdict": verdict, "function": args.function, "profile": profile_id, "seed_artifacts": ctx["seed_artifacts"],
                  "baseline": baseline, "best": best, "best_with_stated_semantics": best_stated,
                  "best_semantics_note": ("best comes from a permuter edit whose semantics are UNVERIFIED; "
                                          "see best_with_stated_semantics") if best.get("stage") == "permuter" else None,
                  "exact_candidates": exact, "explain": explain, "beam_evaluated": len(beam_rows),
                  "permuter": ({k: v for k, v in permuter.items() if k != "rechecks"} if permuter else None),
                  "ledger": "ledger.jsonl", "ledger_progress": ledger_progress(out / "ledger.jsonl"),
                  "identity": "identity.json",
                  "acceptance": "none: exact candidates still need policy, semantic and independent review, gates and a full build"}
        (out / "report.json").write_text(json.dumps(report, indent=2, default=str) + "\n")
        if verdict != "EXACT_NEEDS_REVIEW":
            (out / "TROUBLE.md").write_text(trouble(ctx, baseline, best, best_stated, permuter, hypotheses, explain))
        print(f"{verdict}: best {best['status']} {best['words']} words ({best.get('stage')}); best with stated semantics "
              f"{best_stated['status']} {best_stated['words']} words -> {out}")
        sys.exit(0 if verdict == "EXACT_NEEDS_REVIEW" else 1)
    except (ValueError, OSError, KeyError, subprocess.CalledProcessError) as error:
        parser.exit(2, f"error: {error}\n")
    except KeyboardInterrupt as error:
        signal.pthread_sigmask(signal.SIG_BLOCK, STOP_SIGNALS)  # one interruption record; the process exits next
        where = record_interruption(run_dir, error, args.permuter_candidates) if run_dir is not None else None
        parser.exit(130, f"interrupted ({error}); durable progress: {where}\n")


if __name__ == "__main__":
    main()
