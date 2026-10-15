# Known issues: accepted checkpoint omp-r6b

omp-r6b was accepted **PASS_WITH_QUALIFICATIONS** on 2026-10-15:
- receipt `build/omp-r6b-b`, `2732f16b…` (companion `build/omp-r6b-a`, `89a71eaf…`);
- validation record `harness-validation-omp-r6b.json`;
- 6,853 C/C++ functions / 1,137,076 bytes, 99.097808% of the mapped CPU catalogue (6,538 C functions / 1,044,812 bytes; 315 C++ / 92,264 bytes, including 2 generated C++ functions / 88 bytes).

The delta from omp-r4a is +23 functions and +11,880 bytes. C goes from 6,660 to 6,538 functions and C++ from 170 to 315, because the joint unit `func_800A3D9C.cpp` absorbs 138 former C units (14,420 bytes) and 2 units that were already C++ (2,116 bytes).
With the toolchain library (3 functions / 4,060 bytes, no C/C++ credit) and classified handwritten code (6,292 bytes), **all 1,147,428 bytes of CPU code are accounted for**.

Acceptance followed the lean superseding contract (user ruling 2026-10-14):
- exact source binding;
- an exhaustive per-TU compile/match sweep;
- a private trial build;
- `tools/interfaces.py` over the whole tree;
- a native C++ layout/ODR check with positive and negative controls;
- guarded canonical cutovers: G5 for r6, G5T for one test file, G5S for one source annotation;
- two fresh byte-identical builds;
- the full suite;
- a binder;
- two independent whole-receipt reviews.

Both reviews, the primary (`scratch/omp/accept/omp-r6b/review.json`) and the independent one (`review-sol-independent.json`), returned PASS_WITH_QUALIFICATIONS with no blocking finding.

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
- **Original out-of-bounds read, preserved and annotated.** `func_801E5BE4_137B04.c` copies whole 22-byte records out of a 400-byte staging buffer. For records 122 and 157 this reads 2–4 bytes past the buffer, as the original program does; the bytes are identical. It carries comment-only `ORIGINAL BUG:` annotations under the ORIGINAL-BUG-OVERREAD ruling, and the compiled object is unchanged. The guarded safe forms do not match. They are safe only on the non-aliasing 12/22-byte routes the game uses, not for arbitrary input. This resolves omp-r6a's review finding G8-I-01.
- **`func_80079A80`** keeps one dead assignment, the inherited FAKEMATCH qualification R3-Q-10.
- **Native C++ check scope.** It measures g++ sizes, field offsets and widths, plus Clang token/semantic identity. It does not cover aggregate `alignof`, foreign lifetime, function-local classes or whole-program ODR.
- **Skipped tests.** 145 suite tests are skipped for private prerequisites: the contract's 26 known skips plus 119 that need the private copied original. With that copy present, those witness tests pass (private diagnostic). Skipped tests are not passes.
- **Policy scanner.** Its reported line positions drift after escaped multiline literals and form feeds; the primary review records corrected locations and a repair. Every one of its 257 hits was adjudicated individually, with zero unruled findings.
- **Binder reports.** Its P1/P2 checks report historical drafting-index discrepancies, which are informational. One historical pin, the pre-annotation `func_801E5BE4_137B04.c`, is reported as superseded by the reviewed G5S cutover, never as equal. The control runner's output-symlink guard issue is inherited and nonblocking.
- **History.** omp-r6a (the same source without G5S) was prepared but failed independent review on G8-I-01; it is history only and is never finalized. One historical receipt (B8t) was lost in an earlier disk cleanup and is recorded as a process deviation.

## Not in this checkpoint

- **CPU data ownership (r7).** 702 reviewed data and function units from WP4, WP5 and WP6 (including WP6 round 2) are adopted privately; a byte-identical labelled build and its own acceptance follow.
- **The asset/never-read denominator batch.** Every one of the 233,300 bytes of previously located asset data now has an independently reviewed route, still to be composed and accepted:
  - format witnesses for 4 songs, zero-heavy files, the sound-effect bank, sky files, the WINDOW image and 33 tiny sky files;
  - 11 "no reader found" ranges under the user's rule. Global absence is not proven for any of them.
- **The complete-game denominator remains OPEN**: 32,040,312 research-classified bytes, with 1,514,120 research-open or located, out of 33,554,432 ROM bytes.
- **Readability.** Names, subsystem folders, types and documentation are part of the goal. A reviewed names registry and the rename tooling are in progress; nothing is renamed yet.
