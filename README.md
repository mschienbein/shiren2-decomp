# Shiren the Wanderer 2 (N64) — decompilation

[![Code progress](https://decomp.dev/mschienbein/shiren2-decomp.svg?mode=shield&measure=code&label=Code)](https://decomp.dev/mschienbein/shiren2-decomp)
[![Functions](https://decomp.dev/mschienbein/shiren2-decomp.svg?mode=shield&measure=functions&label=Functions)](https://decomp.dev/mschienbein/shiren2-decomp)
[![progress](https://github.com/mschienbein/shiren2-decomp/actions/workflows/progress.yml/badge.svg)](https://github.com/mschienbein/shiren2-decomp/actions/workflows/progress.yml)

A work-in-progress **matching decompilation** of *Fushigi no Dungeon: Fūrai no Shiren 2 — Oni Shūrai!
Shiren-jō!* (不思議のダンジョン 風来のシレン2 鬼襲来！シレン城！), Nintendo 64, Japan
(`NUS-NSIJ-JPN`, revision 0).

The goal is C source that compiles, with the game's original compilers, to a ROM byte-identical to
the original.

## Status

Accepted checkpoint **omp-b6r**: **6,396 functions** and **978,648 bytes** of matching source,
91.40% of the provisional mapped CPU catalogue. That is 6,355 C functions (956,860 bytes)
and 41 C++ functions (21,788 bytes), the latter proven original C++ units.
Accepted with qualifications (non-blocking review findings and deferred debt): see [KNOWN_ISSUES.md](KNOWN_ISSUES.md).
Live progress, history and a per-function map: [decomp.dev/mschienbein/shiren2-decomp](https://decomp.dev/mschienbein/shiren2-decomp).

| Image | Matched functions | Matched code bytes | Code % |
| --- | ---: | ---: | ---: |
| Main game | 6,170 / 6,249 | 919,604 / 1,001,476 | 91.82% |
| Resident (boot + libultra) | 226 / 268 | 59,044 / 69,224 | 85.29% |

The denominator is a fixed catalogue of 6,517 original CPU functions in the resident
and main images (`docs/progress-catalogues/`). The two overlays, RSP microcode, assets and the
complete-game total are not counted yet.

Matching source in this repository receives **accepted checkpoints only**.
A function counts as matching only after all of the following:
- two fresh full-ROM builds reproduce the original byte for byte;
- the full test suite passes;
- independent reviewers have checked the C source rules: no inline asm, no padding blobs standing in
  for code or data, real types for every interface.

Progress publication is continuous: commit and push coherent accepted-source,
tooling and documentation checkpoints as work advances, and before a handoff,
rather than waiting for the full decompilation. The private `n64-experiments`
working/evidence repository is checkpointed alongside this public repository.
Status documentation may describe unaccepted work, but it must not change the
published source checkpoint or matching counters without ordinary acceptance.
Never publish ROMs, extracted assets, credentials, build outputs or incomplete
live worker writes; never force-push or rewrite history to hide failed work.
Publication does not replace source review, verification or execution permission.

As of 2026-10-13, the next private successor remains in verification integration.
Runtime and build repairs passed static review with qualifications. The newest
native aggregate-array repair also received a qualified complete-source static
pass, but the whole native/final package remains blocked on genuine fixtures,
baselines, full caller/census integration and provenance/permission evidence.
Metadata-graph and seven-view composition reviews found remaining defects.
The scoped ordered-assertion/parent-cutoff and graph-first retained-reader repairs
subsequently passed independent static review with qualifications; full containing
package, census, baseline and provenance closure remains blocked.

A thirty-way private Muse wave, supervised by GPT-6.1-Sol max callers, returned
byte-sealed static case/handler/source maps. Complete independent review rejected
all thirty as complete mappings: exact-source and nested callable/caller/schema
joins remain unfinished, one map omitted original scalar clauses, and disclosed
directory-scope violations remain. Valid partial literals/source facts are retained
separately; successful model transport and byte seals are not acceptance.
Model roles changed on 2026-10-14: **Claude Opus 5.5 (xhigh)** integrates and
takes the hardest work — hard-tail and overlay matching, original-ROM evidence and
deep debugging — while **GPT-6.1-Sol (max)** implements harness code, performs
independent reviews and scouting. Muse is not used. All ten native Sol authors returned
sealed private static maps on 2026-10-14; exact literal/seal checks passed.
Six independent Sol critiques completed with qualified static-packet verdicts,
retaining one citation-boundary exception and all genuine integration/identity/
fixture/source/execution witnesses. These results are not accepted-source progress.
The user subsequently authorized coherent fresh Sol-reviewer gate migration and
resumption of the gated local workflow. An independently reviewed external tool
baseline now reads the accepted B7s checkpoint correctly; private source remains
unaccepted. A new original-backed hypothesis was compiled and rejected without
altering its target. Router/bootstrap and installed-census repairs are in progress.
In the Opus hardcore wave (2026-10-14), all 23 remaining frontier functions now have
exact private forms, including `func_800897AC` (1,056 bytes), `func_80071088`
(880 bytes) and the 142-function Creature joint. The last one, `func_800A2A5C`
(820 bytes), matches under a narrow user ruling scoped to that function and has passed
independent review. On 2026-10-14 the user approved a lean acceptance contract: two
fresh builds, the full suite, independent reviews and a native C++ layout/ODR check,
replacing the remaining private verification harness. That contract has passed
independent review. The successor needed ten source fixes, which are now made and
reviewed. An exhaustive sweep then compiled all 6,494 of its translation units, and
every one matches exactly. Its native layout/ODR check found no new problem, and both
controls behaved as expected. A full private trial build reproduced the original ROM
byte for byte. Cutover and acceptance come next, followed by a batch carrying all 23
frontier functions. None of these private checkpoints updates this repository's
**omp-b6r** source or matching counters.

## Requirements

- macOS (the pinned toolchains in `toolchain.lock.json` are macOS builds), Python 3.12 and
  [uv](https://docs.astral.sh/uv/).
- Your own copy of the ROM. It is **not** provided. Expected SHA-256:
  `4073a9f6516ef5d15cdd8b259a952a7d07633a2815f2a738fb5d98415d30a458`. `tools/rom.py` accepts big-endian,
  byte-swapped and little-endian dumps. The tools look for the ROM two directories above the repository
  root, so a checkout at `<workspace>/projects/shiren2-decomp` reads
  `<workspace>/inputs/shiren2/baserom.jp.z64`.

## Building

```sh
uv sync --frozen
uv run --frozen python tools/setup.py                        # pinned compilers and binutils
uv run --frozen python tools/build.py --name local --no-promote
```

The build splits the ROM with splat, compiles every C unit with its recorded compiler profile, links
at the original addresses and checks the result against the original ROM.

## Matching a function

```sh
uv run --frozen python tools/match.py func_80041AF4 --asm        # original assembly
uv run --frozen python tools/match.py func_80041AF4 candidate.c   # compile, link in place, compare
```

Main-game code uses GCC 2.8.1 (Paper Mario build) with GNU as 2.9.1. The resident image uses GCC 2.7.2
with the KMC assembler. Per-unit profiles live in `config/compiler_profiles.json`.

## Layout

| Path | Contents |
| --- | --- |
| `src/units/<image>/` | one C file per original translation unit (or joint unit) |
| `include/` | shared headers |
| `config/` | splat layout (`shiren2.jp.yaml`), matched functions (`matches.json`), compiler profiles, linker aliases |
| `tools/` | build, match, adopt, verify and progress tooling |
| `docs/progress-catalogues/` | the fixed progress denominator |
| `ci/` | progress report for [decomp.dev](https://decomp.dev) |

## Progress tracking

On every push to `main`, CI writes an [objdiff](https://github.com/encounter/objdiff)-format progress
report from `config/matches.json` and the catalogue. It uploads the report as the `jp_report`
artifact, which [decomp.dev](https://decomp.dev/projects) reads. CI does not need the ROM.

## Legal

This repository contains no ROM, no extracted assets and no other copyrighted game data. You need your
own legally obtained copy of the game. *Shiren the Wanderer* and related names are trademarks of their
respective owners.
