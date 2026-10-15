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

Accepted checkpoint **omp-r4a**: **6,830 functions** and **1,125,196 bytes** of matching source,
98.06% of the provisional mapped CPU catalogue. That is 6,660 C functions (1,050,708 bytes)
and 170 C++ functions (74,488 bytes), the latter proven original C++ units.
Accepted with qualifications (non-blocking review findings and deferred debt): see [KNOWN_ISSUES.md](KNOWN_ISSUES.md).
Live progress, history and a per-function map: [decomp.dev/mschienbein/shiren2-decomp](https://decomp.dev/mschienbein/shiren2-decomp).

| Image | Matched functions | Matched code bytes | Code % |
| --- | ---: | ---: | ---: |
| Main game | 6,237 / 6,254 | 991,708 / 1,001,476 | 99.02% |
| Overlay 1339F0 | 62 / 63 | 10,168 / 10,584 | 96.07% |
| Overlay 136DC0 | 305 / 311 | 64,276 / 66,144 | 97.18% |
| Resident (boot + libultra) | 226 / 268 | 59,044 / 69,224 | 85.29% |

The denominator is a fixed catalogue of 6,896 original CPU functions in the resident,
main and both overlay images (`docs/progress-catalogues/`). Three verbatim upstream libgcc functions
count separately as toolchain library, never as matched C. RSP microcode, assets and the
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
- The N64 SDK's RSP audio object `n_aspMain.o`, placed at `config/rom-evidence/n_aspMain.o` for the
  RSP evidence check. It is **not** redistributed. Expected SHA-256:
  `93b92bdcc86d779cca5bf23ff0806bb3b2d3f886df5c612b1cfe023a829e20e1`, from the SDK archive named in
  `tools/rspaudio.py`.

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
