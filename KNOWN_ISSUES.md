# Known issues: accepted checkpoint omp-b6r

omp-b6r was accepted **PASS_QUALIFIED** on 2026-10-09:
- receipt `build/omp-b6r-b`, `b3179e39…`;
- validation record `harness-validation-omp-b6r.json`;
- 6,396 functions / 978,648 bytes, 91.40% of the provisional mapped CPU catalogue (6,355 C functions / 956,860 bytes; 41 C++ / 21,788).

Both independent whole-receipt reviews, Astra (`scratch/omp/accept/omp-b6r/review.json`) and Opus (`review-opus.json`), returned PASS_QUALIFIED with no blocking finding. The qualifications below are tracked for the next checkpoint (omp-b7).

## Low: annotation gaps (fix in omp-b7)

| Item | Finding | Fix |
| --- | --- | --- |
| `func_8009CC34.c`: no-op `case 6:` / `case 7: break;` without a default | ASTRA-B6R-01, OPUS-B6R-01 | Add an inline `ODD_C`. Measured since: removing the labels leaves text and jump table unchanged. Fixed in the omp-b7 queue (`scratch/omp/kib6r`). |
| `func_800C75F4.c`: no-op `case 12: break;`, which has a prose comment but no `ODD_C` marker | ASTRA-B6R-02, OPUS-B6R-01 | Prefix the explanation with `ODD_C`. Measured since: removing the label changes the compare tree (57 words). Fixed in the omp-b7 queue. |
| `func_80045650.c`: stale byte-ownership comment | OPUS (info) | Corrected against the map in the omp-b7 queue. |
| Full-tree scan by Main (`scratch/omp/omp-b6r-gates/noop-case-scan-classified.json`): more no-op labels in switches with no default, missing `ODD_C`. `func_800B7E9C` case 0 changed in this checkpoint; `func_800422B0` case 0, `func_8012B6B4` case 3, `func_80131820` case 0 and resident `func_80031340` case 1 are unchanged since omp-b4m. | Main scan; not flagged by the reviewers | Annotate in omp-b7. Ten other `case X: break;` hits prevent a real `default` action and are meaningful code. |

These are relabel-class issues under the B8 ruling: plausible original code missing its marker comment. They are not steering devices.

## Deferred, non-gating debt (carried from omp-b6)

- **40 inert duplicate `undefined_syms_auto` PROVIDEs and `D_80138C60`** (a duplicate of a linked splat label), in `config/symbol_aliases.ld`. They do not affect bytes or credit. The three duplicate PROVIDEs named in omp-b6 were removed in omp-b6r. (OPUS-B6-05 remainder, ASTRA-B6R-03.)
- **`tests/test_adopt.py:758`**: a positive slot fixture whose text extends past its slot end. The production placement guard is unaffected. (ASTRA-B6-04 / ASTRA-B6R-04.)
- **`tools/codex_cost.py`**: a standalone usage estimator, not referenced by build, verification or acceptance. (ASTRA-B6R-05.)
- **Phase T1 nominal type debt**: 9 deferred findings where cross-TU object declarations differ only nominally (element types, tags, qualifiers) and extents and field widths agree. The object and layout audits report 0 overlaps, 0 extent disagreements and 0 layout conflicts.

## Process gap found (harness work for omp-b7)

The automated layout audit compares named object declarations, not member-access paths. Two cross-TU layout conflicts on one 0x9CC-byte object therefore passed the gates in omp-b6:
- a grid extent of 8 rows against 11 rows;
- a count width of a byte against a word.

The whole reviews caught them, and omp-b6r fixed them: all 59 views of that object now agree, and 561 ROM accesses were checked. omp-b7 adds a member-access width/extent audit to the gates.
