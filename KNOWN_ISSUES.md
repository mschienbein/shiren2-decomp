# Known issues: accepted checkpoint omp-r4a

omp-r4a was accepted **PASS_QUALIFIED** on 2026-10-15:
- receipt `build/omp-r4a-b`, `3edd1a64…`;
- validation record `harness-validation-omp-r4a.json`;
- 6,830 C/C++ functions / 1,125,196 bytes, 98.062449% of the provisional mapped CPU catalogue (6,660 C functions / 1,050,708 bytes; 170 C++ / 74,488 bytes). The delta from omp-b7s is +375 functions and +80,544 bytes (C +51,596, C++ +28,948).

Acceptance followed the lean superseding contract (user ruling 2026-10-14):
- exact source binding;
- an exhaustive per-TU compile/match sweep;
- a private trial build;
- `tools/interfaces.py` over the whole tree;
- a native C++ layout/ODR check with positive and negative controls;
- a guarded canonical cutover;
- two fresh byte-identical builds;
- the full suite;
- a binder;
- two independent cross-family whole-receipt reviews.

Both reviews, the primary (`scratch/omp/accept/omp-r4a/review.json`) and the independent one (`review-sol-independent.json`), returned PASS_QUALIFIED with no blocking finding.

## Qualifications carried forward

- **Source semantics.** These remain conditional, as in the carried source reviews:
  - partial foreign object views;
  - array, index and arithmetic domains;
  - adjusted receivers;
  - C++ dynamic type and lifetime.

  Byte-exact matching and the native layout check prove neither gameplay behaviour nor lawful foreign ownership.
- **Scoped invalid-C preservation (user rulings).** Each covers only the cases named:
  - the two first-probe indeterminate-pointer cases HIT-01/HIT-02 in `func_8010ECD0` and `func_8010F9FC`;
  - the two `kind = -1` discard-only lookups in the species TU `func_80074040`;
  - the 74B90 base-pointer form.

  Gameplay reachability is unknown.
- **`func_80079A80`** keeps one dead assignment, the inherited FAKEMATCH qualification R3-Q-10.
- **Stale evidence labels.** For the ten repaired r4 TUs, the `config/matches.json` evidence strings still cite older source hashes. The current matching evidence is the fresh per-TU sweep, the trial-build receipt and the prepared receipts.
- **Native C++ check scope.** It measures g++ sizes, field offsets and widths, plus Clang token/semantic identity. It does not cover aggregate `alignof`, foreign lifetime, function-local classes or whole-program ODR.
- **Skipped tests.** 29 suite tests are skipped for private prerequisites: 24 need the private copied original, 4 a private ROM/SDK witness, and 1 the private r1 ROM. Skipped tests are not passes.
- **Policy scanner.** Its line positions drift in multiline literals and form-feed files. Every one of its 257 hits was adjudicated individually.
- **History.** One historical receipt (B8t) was lost in an earlier disk cleanup. It is history only and is recorded as a process deviation.

## Not in this checkpoint

- The 23 former frontier functions (batch b9, about 11.9 KB, which would bring C/C++ coverage to about 99.1%) and the reviewed complete-game denominator layers form the next candidate.
- CPU data ownership (495 reviewed owners, 82,256 bytes) and the data-unit tool follow after that.
- The complete-game denominator remains **OPEN**: 21,966,927 research-classified bytes, with 11,587,505 research-open or located, out of 33,554,432 ROM bytes.
