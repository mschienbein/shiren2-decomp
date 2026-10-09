#include "common.h"

typedef unsigned char u8;

/* A map position. Returned by value (struct return through the hidden result pointer);
   the user-declared copy constructor is member-wise, while assignment stays the implicit
   block copy. */
struct Pair {
    s32 x;
    s32 y;
    Pair(s32 px, s32 py) : x(px), y(py) {}
    Pair(const Pair &other) : x(other.x), y(other.y) {}
};
struct Rect { Pair min; Pair max; };
struct Cell { Pair position; u8 pad08[0x16]; u8 flags1E; };
/* Area iterator built by func_800A9204 (0x20 bytes; field 0 is the cursor). */
struct Iter { s32 cursor; u8 pad04[0x1C]; };

extern "C" {
extern u8 D_80147620[];
Iter *func_800A9204(Iter *it, Rect *bounds, void *center);
s32 func_800A9284(Iter *it, s32 filter);
Cell *func_800A942C(Iter *it);
u32 func_800B1C6C(void *pos);
s32 func_800A6E90(void *cell);
s32 func_800E1CC4(Cell *cell, s32 kind);
s32 func_800A44F4(void *self, void *target);
u8 func_800C57CC(void *rng, s32 limit);
}

static inline s32 cell_usable(Cell *c, void *center, s32 check) {
    return !(func_800B1C6C(c) & 0x4000) && !func_800A6E90(c)
        && (!(c->flags1E & 0x7C) || !func_800E1CC4(c, 1))
        && (!check || func_800A44F4(center, c) == 2);
}

/* Picks a random usable cell of the area (the last usable one when there are fewer than two). */
extern "C" Pair func_800A694C(void *center, Rect *bounds, u8 filter, const s32 check) {
    Pair result(0, 0);
    Iter it;
    s32 count = 0;
    func_800A9204(&it, bounds, center);
    for (;;) {
        Cell *c;
        if (!func_800A9284(&it, filter)) break;
        c = func_800A942C(&it);
        if (cell_usable(c, center, check)) {
            result = c->position;
            count++;
        }
    }
    if (count >= 2) {
        count = func_800C57CC(D_80147620, (u8)(count - 1));
        it.cursor = 0;
        for (;;) {
            Cell *c;
            if (!func_800A9284(&it, filter)) break;
            c = func_800A942C(&it);
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
