#include "common.h"

typedef struct {
    s32 x;
    s32 y;
} Pair;
typedef struct {
    Pair min;
    Pair max;
} Rect;
/* 0x20-byte unit iterator (as built by func_800A9204): cursor at +0, excluded unit at +4,
 * centre position at +8 and search rectangle at +0x10. */
typedef struct {
    s32 cursor;
    Pair *excluded;
    Pair center;
    Rect area;
} UnitIter;

/* Returns its rectangle by value through the hidden result pointer. */
extern Rect func_800B3080(void *pos);

/* Iterator over the units in the area around a position, with no excluded unit. */
UnitIter *func_800A919C(UnitIter *it, Pair *pos) {
    it->excluded = 0;
    it->center = *pos;
    it->area = func_800B3080(&it->center);
    it->cursor = 0;
    return it;
}
