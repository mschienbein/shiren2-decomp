extern "C" {
#include "func_801F17C0_1436E0/types.h"
}

static inline bool cell_occupied(Cell801F17C0 *cell) {
    return (cell->flags_1E >> 4) & 1;
}

/* Caller 0x801F1630 passes two words and consumes the actor pointer in v0.
 * Narrow only at the ROM's byte uses (0x801F18B8/0x801F18E4/0x801F18EC). */
extern "C" void *func_801F17C0_1436E0(s32 arg0, s32 flags) {
    Vec801F17C0 origin(D_801476B8->pos);
    s32 cell = 30;
    s32 best_cell = 0;
    s32 best = 0;

    while (--cell != 0) {
        Cell801F17C0 *record = (Cell801F17C0 *)func_800A8CB0((u8)cell);
        Vec801F17C0 pos;
        s32 distance;

        if (!cell_occupied(record)) {
            continue;
        }
        if (record->flags_9A & 0x40) {
            continue;
        }
        if (record->flags_9A & 0x100) {
            continue;
        }
        if (record->flags_9A & 0x200) {
            continue;
        }
        pos = record->pos;
        distance = func_80083D40(pos.y - origin.y);
        distance += func_80083D40(pos.x - origin.x);
        if (best < distance) {
            best = distance;
            best_cell = cell;
        }
    }
    if (best_cell != 0) {
        Cell801F17C0 *record = (Cell801F17C0 *)func_800A8CB0((u8)best_cell);

        if (record != 0) {
            CellVtEntry *vt = record->vptr;

            vt[1].pfn((u8 *)record + vt[1].delta, 3);
        }
    }
    return func_800A83D0((u8)arg0, (u8)flags);
}
