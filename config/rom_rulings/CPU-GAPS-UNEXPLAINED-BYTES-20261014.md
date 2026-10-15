# User ruling: three CPU-image gaps close as unexplained bytes (2026-10-14)

The user chose **"Close as unexplained bytes"** on 2026-10-14 (ask tool, question `cpu_gaps`).

## Scope

Exactly three `cpu_text_unowned` ranges (136 B):

| ROM range | Bytes | Note |
| --- | ---: | --- |
| 0x1040-0x1060 | 32 | no witness |
| 0x871C-0x8780 | 100 | G4b; the link-order / ordered-member tier stays rejected as a padding witness (`Main-decision-cpu-padding-tiers-001.json`) |
| 0xABF4C-0xABF50 | 4 | no witness |

Out of scope: the two 12 B pending-peer ranges (0x93A54-0x93A60 and 0xFABB4-0xFABC0). They close through their
witness-assembler joint records once `func_800BFEA0` and `func_80127888` are adopted and those records are
re-pinned and re-reviewed.

## Ruling

1. The denominator may close these exact bytes as unexplained CPU-image bytes retained verbatim. The
   classification must name each range's precise missing witness.
2. They are not padding and not code, and earn no C/C++ credit. No padding tier is widened.
3. Any later original-backed witness supersedes this classification. No other range is covered.
