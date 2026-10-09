#include "common.h"

typedef unsigned char u8;

typedef struct Vec2i {
    s32 x;
    s32 y;
} Vec2i;

typedef struct Rect800F70B4 {
    Vec2i start;
    Vec2i end;
} Rect800F70B4;

/* Rectangle iterator advanced by func_800A3610 (same layout as func_800B23B0's). */
typedef struct RectIter800F70B4 {
    Vec2i cur;
    Vec2i start;
    Vec2i end;
} RectIter800F70B4;

/* Returns its rectangle by value through the hidden result pointer. */
Rect800F70B4 func_800B3024(Vec2i *pos);
Vec2i *func_800A3610(Vec2i *out, RectIter800F70B4 *it);
void *func_800B4D80(Vec2i *pos);
s32 func_800F6F94(void *self, void *item);
s32 func_800A650C(void *object, Vec2i *pos);

static inline void rect_iter_set_start(RectIter800F70B4 *it, s32 x, s32 y)
{
    Vec2i v;

    v.x = x;
    v.y = y;
    it->start = v;
    it->cur = it->start;
}

static inline void rect_iter_set_end(RectIter800F70B4 *it, s32 x, s32 y)
{
    Vec2i v;

    v.x = x;
    v.y = y;
    it->end = v;
}

static inline s32 rect_iter_valid(RectIter800F70B4 *it)
{
    return it->cur.x <= it->end.x;
}

/* Find the nearest acceptable item in the room around `pos`; on success store its
 * cell in `pos` and return the item. */
void *func_800F70B4(void *unit, Vec2i *pos)
{
    Vec2i best;
    Rect800F70B4 rect;
    RectIter800F70B4 iter;
    void *found = 0;
    s32 best_dist = 1000;

    rect = func_800B3024(pos);
    rect_iter_set_start(&iter, rect.start.x, rect.start.y);
    rect_iter_set_end(&iter, rect.end.x, rect.end.y);
    while (rect_iter_valid(&iter)) {
        Vec2i cell;
        void *item;
        s32 dist;

        func_800A3610(&cell, &iter);
        item = func_800B4D80(&cell);
        if (item == 0) {
            continue;
        }
        if (!func_800F6F94(unit, item)) {
            continue;
        }
        dist = func_800A650C(unit, &cell);
        if (dist < best_dist) {
            found = item;
            best = cell;
            best_dist = dist;
        }
    }
    if (found != 0) {
        *pos = best;
    }
    return found;
}
