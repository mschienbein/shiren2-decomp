#include "common.h"

typedef unsigned char u8;
typedef short s16;

typedef struct {
    s32 x;
    s32 y;
} Point;

/* g++ 2.x vtable slot 2: s32 (self). */
typedef struct {
    s16 delta;
    s16 index;
    s32 (*fn)(void *self);
} VtblEntry;

typedef struct Unit {
    u8 pad0[0x1E];
    u8 flags1E;
    u8 pad1F[0x5];
    VtblEntry *vtbl24;
} Unit;

/* Area scan cursor (partial view). */
typedef struct {
    u8 pad0[0x9];
    u8 count9;
    u8 indexA;
    u8 padB[0xD];
    Unit *unit18;
} Iterator;

int func_800C28EC(Iterator *it);
Point *func_800C28FC(Point *out, Iterator *it);
u32 func_800B1C6C(Point *pos);
Unit *func_800B4928(Point *pos);
s32 func_800E1CC4(Unit *obj, s32 kind);

/* Advance to the next cell holding a unit whose flags match mask; stores it in it->unit18. */
s32 func_800C2BBC(Iterator *it, unsigned char mask) {
    Point pos;
    Point *cell;
    Unit *unit;
    s32 found;
    VtblEntry *entry;

    while (func_800C28EC(it)) {
        func_800C28FC(&pos, it);
        cell = &pos;
        if (func_800B1C6C(cell) & 0x4000) {
            continue;
        }
        unit = func_800B4928(cell);
        found = 0;
        if (unit != 0 && (unit->flags1E & mask)) {
            entry = &unit->vtbl24[2];
            if (!entry->fn((u8 *)unit + entry->delta)) {
                if (!(unit->flags1E & 0x7C) || !func_800E1CC4(unit, 1)) {
                    found = 1;
                }
            }
        }
        if (found) {
            it->unit18 = unit;
            return 1;
        }
    }
    return 0;
}
