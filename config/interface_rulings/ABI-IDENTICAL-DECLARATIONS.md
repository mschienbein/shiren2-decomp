# User ruling: scoped ABI-identical declaration differences (2026-10-14)

The user chose **"Scoped ABI exception"** (2026-10-14, ask tool, question `abi_identical_declarations`).

## Scope

Only these named declaration/definition pairs. Every other declaration must still agree with its
definition under `docs/OMP_WORKFLOW.md` and `tools/interfaces.py`.

| Function | Definition (contract that matches) | Admitted differing declarations | Why each view is forced |
| --- | --- | --- | --- |
| `func_800A2594` | C explicit out-pointer `MapPos *func_800A2594(MapPos *out, MapPos *from, MapDir dir)` (canonical; 4 C callers match only this way: `func_800D6430.c`, `func_800B4788.c`, `func_800D5730.c`, `func_8010F020.c`) | by-value `extern "C" MapPos func_800A2594(MapPos *from, MapDir dir)` in C++ `func_800B38F8.cpp` only | GCC 2.8.1 takes a by-value call's hidden-result address before evaluating arguments (`calls.c` 929-935) but evaluates explicit arguments last to first (990-993, 1632-1677); B38F8's ROM order needs the by-value view (best explicit-out form 7 words, `opus-06/a2594-r2/`). |
| `func_800B2EDC` | explicit out-pointer `Rect *func_800B2EDC(Rect *out, Pos *pos)` returning `out` (a by-value definition copies the result pointer at entry: 7 words) | existing by-value declarations in `func_800B3024.cpp` and `func_80048B04.c` | Same a0/v0 registers; each caller's ROM matches its own by-value view. |
| `func_80032D94` (memcpy) | `void *func_80032D94(void *dst, const void *src, u32 size)` (`src/units/resident/func_80032D30.c`) | `void func_80032D94(...)` in overlay `func_801E5BE4_137B04.c` only | With the `void *` declaration the call explicitly sets `$v0`, which excludes it from reload's re-placement of the HI half (`reload1.c` 582/830/3894); the ROM's `mfhi $v0` needs the void view (compliant form 13 words, `opus-05/overlay-r2/`). Both call results are ignored. |

(Evidence paths are under `scratch/omp/wave39/opus-hardcore-matching/`.)

Correction (Main, 2026-10-14, narrowing only): the option text shown to the user also named "the
existing B5A18" for `func_800A2594`. Canonical `func_800B5A18.cpp` declares no `func_800A2594` (it is
itself a by-value `MapPos` function calling `func_800A2758`/`func_800B1C6C`), so there is nothing to
admit there; the scope above is narrowed accordingly. Found by SolInterfaceAllowlist.

## Conditions

1. At each admitted differing declaration, a comment states the ABI equivalence (hidden struct-return
   pointer in a0 and returned in v0 versus an explicit out-pointer; or an ignored v0 return), the measured
   mechanism and word counts, and cites this ruling.
2. The interface gate admits exactly these pairs through an explicit, reviewed allowlist entry naming each
   file and symbol. No blanket category relaxation.
3. Tracked as debt. Each function still needs independent cross-family review, an exact final-context
   match and the ordinary build, suite and acceptance gates.
