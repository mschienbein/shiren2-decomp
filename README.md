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

This repository only receives **accepted checkpoints**. A function counts as matching only after all
of the following:
- two fresh full-ROM builds reproduce the original byte for byte;
- the full test suite passes;
- independent reviewers have checked the C source rules: no inline asm, no padding blobs standing in
  for code or data, real types for every interface.

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
