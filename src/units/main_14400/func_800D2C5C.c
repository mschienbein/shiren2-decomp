#include "common.h"
#include "map_position_iterator.h"

typedef struct { unsigned char unknown00; unsigned char field01; } Tile;
typedef struct { s32 unknown00; MapRect *field04; s32 unknown08[2]; s32 field10; } Object;
/* Whole 16-byte map rectangle (first corner +0, last corner +8). */
extern MapRect D_801429C0;
extern Tile *func_800B4D80(MapPos *);
extern s32 func_800AEC2C(Tile *);
static __inline__ void copy_bounds(MapRect *dst, MapRect *src) {
    dst->min = src->min;
    dst->max = src->max;
}
static __inline__ s32 has_next(MapPositionIterator *iterator) {
    return iterator->cur.x <= iterator->end.x;
}
s32 func_800D2C5C(Object *object) {
    MapRect bounds;
    MapPositionIterator iterator;
    MapPos position;
    MapRect *source = object->field04;
    s32 total = object->field10;
    if (!source) {
        copy_bounds(&bounds, &D_801429C0);
    } else {
        copy_bounds(&bounds, source);
    }
    position.x = bounds.min.x;
    position.y = bounds.min.y;
    iterator.start = position;
    iterator.cur = iterator.start;
    position.x = bounds.max.x;
    position.y = bounds.max.y;
    iterator.end = position;
    for (;;) {
        Tile *tile;
        if (!has_next(&iterator)) break;
        func_800A3610(&position, &iterator);
        tile = func_800B4D80(&position);
        if (tile && tile->field01 != 0xf2) total += func_800AEC2C(tile);
    }
    return total;
}
