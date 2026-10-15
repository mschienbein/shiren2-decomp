# User ruling: R0 closes as an unexplained residual (2026-10-14)

The user chose **"Close as unexplained residual"** on 2026-10-14 (ask tool, question `r0`).

## Scope

Exactly one range: ROM 0x8C79A0-0x8CA860 (11,968 B), the only `asset_raw_unattributed` / `asset_raw:open` range
in the combined-r2 plus assetstack-r2 research map (`opus-09/assetstack-r2/exercise/tree/config/rom_map.json`).

## Ruling

1. The denominator may close these exact bytes as unattributed data retained verbatim. The classification must say
   it is unexplained and cite the negative investigations:
   - `wave37/raw-graphics-main-intake.json`
   - `wave37/raw-descriptors-main-intake.json`
   - the hypothetical CI4 preview
   - the 22x(512+32) structural lead
2. It makes no format, consumer, ownership or padding claim and earns no C/C++ credit.
3. Any later original-backed witness supersedes this residual classification.
4. The ruling covers no other range. Any other open byte still needs its own witness or ruling.
