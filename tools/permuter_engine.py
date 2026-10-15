#!/usr/bin/env python3
"""Deterministic, bounded candidate engine over the pinned decomp-permuter (child of tools/permute.py).

    uv run --no-project --offline --with toml==0.10.2 python -B -E -s -X pycache_prefix=FRESH \
        tools/permuter_engine.py CONFIG.json OWNER_FD

Startup order; no checkout or toml code runs before step 3:
1. `owned_process.guard_owner(OWNER_FD)`, before any thread or descendant exists, so the group dies
   with its owner.
2. `pinned_tree.verify_checkout` hashes every tracked checkout file against the pinned commit (no
   changed, missing or extra file). `pinned_tree.locate_toml` finds the toml package without
   executing it, and `verify_toml` hashes its files against the pinned per-file sha256.
3. `pinned_tree.install_verified` serves `perm_pycparser`, `src` and `toml` from exactly those
   verified bytes, with no bytecode cache. Only then are they imported. The executed modules are
   audited and recorded in the `start` event. The checkout's random-walk main loop is never called.

For candidate i (0-based, i < candidates), the engine builds the base function from `perm/base.c`,
applies `mutations` randomization passes with `Random(rng_seed_i)` where
rng_seed_i = seed * 1_000_003 + i + 1, and gives weight only to the explicitly allowed passes (all
others weigh 0). Each candidate starts from the base.

Durable ordered evidence: `events.jsonl` is created exclusively. Each event carries the next
consecutive `seq`, is written with one append and is fsynced before the next step:
- `start`
- per candidate, in index order: `generation-started` (index, rng_seed) before any randomization,
  then `generated` with the pass list, function hash and any terminal outcome already known:
  duplicate, rejected-forbidden, rejected-unverifiable or randomization-error
- `evaluation-started` and `evaluated` (compile-error or scored)
- `complete`

A crash or kill leaves every committed step on disk. The parent (`permute.engine_progress`)
reports a candidate whose generation started but never committed as incomplete, with its index and
rng_seed; a generated candidate whose evaluation never finished as incomplete; and only indices
whose generation never started as not started. On completion the engine also writes
`candidates.jsonl` (one record per candidate, sorted by index) and `summary.json`.
"""
from __future__ import annotations

import hashlib
import json
import os
import sys
import threading
import time
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path


def sha(text: str) -> str:
    return hashlib.sha256(text.encode()).hexdigest()


class EventLog:
    """Exclusive append-only JSONL. Each event gets the next `seq`, is written with one append and
    is fsynced before the caller continues."""

    def __init__(self, path: Path):
        self.fd = os.open(path, os.O_WRONLY | os.O_CREAT | os.O_EXCL | os.O_APPEND, 0o644)
        directory = os.open(path.parent, os.O_RDONLY)
        try:
            os.fsync(directory)
        finally:
            os.close(directory)
        self.lock = threading.Lock()
        self.seq = 0

    def emit(self, event: str, **fields) -> None:
        with self.lock:
            view = memoryview((json.dumps({"seq": self.seq, "event": event, **fields}, sort_keys=True) + "\n").encode())
            while view:
                view = view[os.write(self.fd, view):]
            os.fsync(self.fd)
            self.seq += 1


def main() -> None:
    config_path, owner_fd = Path(sys.argv[1]), int(sys.argv[2])
    import owned_process  # stdlib-only sibling module (script directory)
    owned_process.guard_owner(owner_fd)

    import pinned_tree
    import source_policy as policy
    cfg = json.loads(config_path.read_text())
    perm_root = Path(cfg["permuter"]).resolve()
    pin, checkout = pinned_tree.verify_checkout(perm_root, cfg["commit"])
    toml_dir = pinned_tree.locate_toml()
    pin["toml"], toml_blobs = pinned_tree.verify_toml(toml_dir)
    importer = pinned_tree.install_verified({"perm_pycparser": (perm_root, checkout), "src": (perm_root, checkout),
                                             "toml": (toml_dir.parent, toml_blobs)})

    from perm_pycparser import c_ast as ca
    from src import ast_util
    from src.candidate import Candidate
    from src.compiler import Compiler
    from src.perm.eval import perm_evaluate_one
    from src.perm.parse import perm_parse
    from src.perm.perm import EvalState
    from src.randomizer import RANDOMIZATION_PASSES
    from src.scorer import Scorer
    pin["served_modules"] = importer.audit()

    perm = Path(cfg["perm_dir"])
    out = Path(cfg["out_dir"])
    out.mkdir(parents=True, exist_ok=True)
    log = EventLog(out / "events.jsonl")
    allowed = set(cfg["allowed_passes"])
    names = [p.__name__ for p in RANDOMIZATION_PASSES]
    unknown = allowed - set(names)
    if unknown:
        raise SystemExit(f"unknown permuter passes: {sorted(unknown)}")
    weights = {name: (1.0 if name in allowed else 0.0) for name in names}
    fn_name = cfg["function"]
    base_text = (perm / "base.c").read_text()
    if "PERM_" in base_text:
        raise SystemExit("base.c contains PERM_ macros; the engine only accepts plain seeds")
    base_source, _ = perm_evaluate_one(perm_parse(base_text))
    compiler = Compiler(str(perm / "compile.sh"), show_errors=False, debug_mode=False)
    scorer = Scorer(str(perm / "target.o"), stack_differences=True, algorithm="difflib", debug_mode=False,
                    ign_branch_targets=True, objdump_command=cfg["objdump_command"])

    def features(ast) -> dict:
        fn = ast_util.extract_fn(ast, fn_name)[0]
        counts = policy.ast_counts(ca, ast_util.to_c, ast, fn_name, count_casts=True)
        lexical = policy.lexical_counts(ast_util.to_c(fn))
        for key in ("asm", "token_paste"):
            counts[key] = counts.get(key, 0) + lexical[key]
        return counts

    base_cand = Candidate.from_source(base_source, EvalState(), fn_name, weights, rng_seed=0)
    try:
        base_features = features(base_cand.ast)
    except Exception as error:  # noqa: BLE001 - the base must be checkable, or nothing is
        raise SystemExit(f"the base cannot be checked by the source policy: {type(error).__name__}: {error}")
    o_file = base_cand.compile(compiler, show_errors=True)
    if not o_file:
        raise SystemExit("base does not compile under compile.sh")
    base_score, base_hash = base_cand.score(scorer, o_file).score, base_cand.score_hash
    base_fn = ast_util.to_c(ast_util.extract_fn(base_cand.ast, fn_name)[0])
    log.emit("start", candidates=cfg["candidates"], seed=cfg["seed"], mutations=cfg["mutations"],
             allowed_passes=sorted(allowed), base_score=base_score, base_function_sha256=sha(base_fn), pin=pin)

    # Generate every candidate first (single thread; the randomizer is not thread-safe), then
    # compile/score with at most `jobs` concurrent compiler pipelines.
    generated = []
    seen: dict[str, int] = {sha(base_fn): -1}
    for index in range(cfg["candidates"]):
        rng_seed = cfg["seed"] * 1_000_003 + index + 1
        log.emit("generation-started", index=index, rng_seed=rng_seed)  # write-ahead: attempted from here on
        applied: list[str] = []
        record = {"index": index, "rng_seed": rng_seed, "passes": applied}
        try:
            cand = Candidate.from_source(base_source, EvalState(), fn_name, weights, rng_seed=rng_seed)
            recorded = []
            for method, weight in cand.randomizer.methods:
                def wrapped(*a, _m=method, **k):
                    _m(*a, **k)
                    applied.append(_m.__name__)
                wrapped.__name__ = method.__name__
                recorded.append((wrapped, weight))
            cand.randomizer.methods = recorded
            for _ in range(cfg["mutations"]):
                cand.randomize_ast()
        except Exception as error:  # noqa: BLE001 - recorded, never hidden
            record.update(outcome="randomization-error", error=f"{type(error).__name__}: {error}"[:300])
            log.emit("generated", **record)
            generated.append((record, None))
            continue
        fn_text = ast_util.to_c(ast_util.extract_fn(cand.ast, fn_name)[0])
        digest = sha(fn_text)
        record["function_sha256"] = digest
        try:
            record["forbidden"] = policy.forbidden_delta(base_features, features(cand.ast))
        except Exception as error:  # noqa: BLE001 - a candidate the policy cannot read is never clean
            record["policy_error"] = f"{type(error).__name__}: {error}"[:300]
        if digest in seen:
            record.update(outcome="duplicate", duplicate_of=seen[digest])
        elif "policy_error" in record:
            record["outcome"] = "rejected-unverifiable"  # never compiled, never reported as an improvement
        elif record["forbidden"]:
            record["outcome"] = "rejected-forbidden"  # never compiled, never reported as an improvement
        seen.setdefault(digest, index)
        log.emit("generated", **record)
        if "outcome" in record:
            generated.append((record, None))
            continue
        record["function_text"] = fn_text
        generated.append((record, cand))

    def evaluate(item):
        record, cand = item
        if cand is None:
            return record
        log.emit("evaluation-started", index=record["index"])
        started = time.monotonic()
        o_file = cand.compile(compiler)
        if not o_file:
            record.update(outcome="compile-error", seconds=round(time.monotonic() - started, 3))
        else:
            result = cand.score(scorer, o_file)
            record.update(outcome="scored", score=result.score, score_hash=result.hash,
                          seconds=round(time.monotonic() - started, 3))
        log.emit("evaluated", **{k: v for k, v in record.items() if k not in ("function_text", "passes", "forbidden")})
        return record

    with ThreadPoolExecutor(max_workers=cfg["jobs"]) as pool:
        records = list(pool.map(evaluate, generated))
    with (out / "candidates.jsonl").open("w") as handle:
        for record in records:
            handle.write(json.dumps(record, sort_keys=True) + "\n")
    scored = [r for r in records if r.get("outcome") == "scored"]
    improved = sorted((r for r in scored if r["score"] < base_score), key=lambda r: (r["score"], r["index"]))
    for r in improved[: cfg["emit_top"]]:
        (out / f"improved-{r['index']:05d}.c").write_text(r["function_text"])
    summary = {"base_score": base_score, "base_hash": base_hash, "base_function_sha256": sha(base_fn),
               "base_features": base_features, "weights_nonzero": sorted(allowed), "candidates": cfg["candidates"],
               "outcomes": {k: sum(r.get("outcome") == k for r in records) for k in
                            ("scored", "duplicate", "rejected-forbidden", "rejected-unverifiable", "compile-error",
                             "randomization-error")},
               "best_score": min([base_score, *(r["score"] for r in scored)]),
               "improved": [{"index": r["index"], "score": r["score"], "rng_seed": r["rng_seed"],
                             "passes": r["passes"], "function_sha256": r["function_sha256"]} for r in improved],
               "emitted": [f"improved-{r['index']:05d}.c" for r in improved[: cfg["emit_top"]]], "pin": pin}
    (out / "summary.json").write_text(json.dumps(summary, indent=2) + "\n")
    log.emit("complete", outcomes=summary["outcomes"], best_score=summary["best_score"])
    print(json.dumps({k: summary[k] for k in ("base_score", "best_score", "outcomes")}))


if __name__ == "__main__":
    main()
