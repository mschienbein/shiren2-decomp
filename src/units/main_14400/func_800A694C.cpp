#include "map_unit_iterator.h"

typedef unsigned char u8;

/* Shared MapPos preserves member-wise copy construction and implicit block assignment. */
struct Cell_800A694C { MapPos position; u8 pad08[0x16]; u8 flags1E; };

extern "C" {
extern u8 D_80147620[];
u32 func_800B1C6C(void *pos);
s32 func_800A6E90(void *cell);
s32 func_800E1CC4(Cell_800A694C *cell, s32 kind);
s32 func_800A44F4(void *self, void *target);
u8 func_800C57CC(void *rng, s32 limit);
}

static inline s32 cell_usable(Cell_800A694C *c, void *center, s32 check) {
    return !(func_800B1C6C(c) & 0x4000) && !func_800A6E90(c)
        && (!(c->flags1E & 0x7C) || !func_800E1CC4(c, 1))
        && (!check || func_800A44F4(center, c) == 2);
}

/* Picks a random usable cell of the area (the last usable one when there are fewer than two). */
extern "C" MapPos func_800A694C(void *center, MapRect *bounds, u8 filter, const s32 check) {
    MapPos result(0, 0);
    MapUnitIterator it;
    s32 count = 0;
    func_800A9204(&it, bounds, center);
    for (;;) {
        Cell_800A694C *c;
        if (!func_800A9284(&it, filter)) break;
        c = static_cast<Cell_800A694C *>(func_800A942C(&it));
        if (cell_usable(c, center, check)) {
            result = c->position;
            count++;
        }
    }
    if (count >= 2) {
        count = func_800C57CC(D_80147620, (u8)(count - 1));
        it.cursor = 0;
        for (;;) {
            Cell_800A694C *c;
            if (!func_800A9284(&it, filter)) break;
            c = static_cast<Cell_800A694C *>(func_800A942C(&it));
            if (cell_usable(c, center, check)) {
                count--;
                if (count == -1) {
                    return c->position;
                }
            }
        }
    }
    return result;
}
