#include "map_position_iterator.h"

typedef unsigned char u8;

void *func_800B4D80(MapPos *pos);
s32 func_800F6F94(void *self, void *item);
s32 func_800A650C(void *object, MapPos *pos);

static inline void rect_iter_set_start(MapPositionIterator *it, s32 x, s32 y)
{
    MapPos v;

    v.x = x;
    v.y = y;
    it->start = v;
    it->cur = it->start;
}

static inline void rect_iter_set_end(MapPositionIterator *it, s32 x, s32 y)
{
    MapPos v;

    v.x = x;
    v.y = y;
    it->end = v;
}

static inline s32 rect_iter_valid(MapPositionIterator *it)
{
    return it->cur.x <= it->end.x;
}

/* Find the nearest acceptable item in the room around `pos`; on success store its
 * cell in `pos` and return the item. */
void *func_800F70B4(void *unit, MapPos *pos)
{
    MapPos best;
    MapRect rect;
    MapPositionIterator iter;
    void *found = 0;
    s32 best_dist = 1000;

    rect = func_800B3024(pos);
    rect_iter_set_start(&iter, rect.min.x, rect.min.y);
    rect_iter_set_end(&iter, rect.max.x, rect.max.y);
    while (rect_iter_valid(&iter)) {
        MapPos cell;
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
