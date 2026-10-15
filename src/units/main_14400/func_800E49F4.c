#include "map_unit_iterator.h"

typedef unsigned char u8;

typedef struct {
    MapPos pos_0;
    u8 pad8[0x4];
    MapRect bounds_C;
} Unit800E49F4;

s32 func_800A44F4(Unit800E49F4 *unit, void *other);
s32 func_800E1CC4(void *other, s32 flag);
s32 func_800A650C(void *other, MapPos *pos);

static inline void copyPos(MapPos *out, MapPos *in) {
    out->x = in->x;
    out->y = in->y;
}

static inline void setRect(MapRect *r, MapPos *lo, MapPos *hi) {
    r->min = *lo;
    r->max = *hi;
}

static inline s32 isHostile(Unit800E49F4 *unit, void *other) {
    s32 ok = 0;
    if (func_800A44F4(unit, other) == 2) {
        ok = func_800E1CC4(other, 1) == 0;
    }
    return ok;
}

s32 func_800E49F4(Unit800E49F4 *unit, s32 skipClip) {
    MapPos center;
    MapRect area;
    MapPos lo;
    MapPos hi;
    MapUnitIterator it;
    s32 best = 0;
    s32 found = 0;
    s32 dist;
    void *other;
    void *target;

    copyPos(&center, &unit->pos_0);
    lo.x = center.x - 5;
    lo.y = center.y - 5;
    hi.x = center.x + 4;
    hi.y = center.y + 5;
    setRect(&area, &lo, &hi);
    if (skipClip == 0) {
        func_800A3448(&area, &unit->bounds_C);
    }
    func_800A9204(&it, &area, unit);
    while (func_800A9284(&it, 0x7C) != 0) {
        other = func_800A942C(&it);
        target = other;
        if (!isHostile(unit, other)) {
            continue;
        }
        found = 1;
        dist = func_800A650C(target, &center);
        if (best < dist) {
            best = dist;
        }
    }
    dist = best - 1;
    if (dist >= 0 && dist < 3) {
        return dist;
    }
    if (!found) {
        return 4;
    }
    return 3;
}
