#include "map_rect.h"

/* MapRect uses map_types.h's shared member-wise MapPos copy constructor;
 * its two-corner aggregate is returned through the hidden result pointer. */
/* 0x14-byte D_801431F0 record returned by func_800B1F90; it starts with its rectangle. */
struct Record_800B3024 { MapRect area; unsigned char pad10[4]; };

extern "C" {
/* Scoped ABI ruling: scratch/omp/rulings/ABI-IDENTICAL-DECLARATIONS.md.
 * This by-value call supplies its hidden result pointer in a0 and receives it in v0,
 * exactly as B2EDC's explicit-out definition does. A by-value B2EDC definition copies
 * the result pointer at entry and differs in 7 words. Keep this view local to this TU. */
MapRect func_800B2EDC(void *object);
extern MapRect D_801429C0;
extern unsigned char D_80143392;
u32 func_800B1C6C(void *pos);
Record_800B3024 *func_800B1F90(void *pos);
}

/* The object's rectangle clipped to the map bounds. */
extern "C" MapRect func_800B3024(void *object) {
    MapRect area = func_800B2EDC(object);
    func_800A3448(&area, &D_801429C0);
    return area;
}

/* The rectangle for a position: the func_800B1F90 record when D_80143392 is clear and the
   position has flag 0x1000, otherwise func_800B3024's clipped rectangle.
   The copy out of the func_800B1F90 record is member-wise (interleaved lw/sw), and the other path
   forwards the hidden result pointer to func_800B3024 (a0 = result, a1 = pos). */
extern "C" MapRect func_800B3080(void *pos) {
    s32 ok = 0;
    if (D_80143392 == 0) { s32 f = func_800B1C6C(pos) & 0x1000; ok = f != 0; }
    if (ok) return func_800B1F90(pos)->area;
    return func_800B3024(pos);
}
