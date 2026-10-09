#include "common.h"

typedef struct { s32 field00, field04; } Position;
typedef struct { Position first, last; } Bounds;
typedef struct { Position current, first, last; } Iterator;
typedef struct { unsigned char unknown00; unsigned char field01; } Tile;
typedef struct { s32 unknown00; Bounds *field04; s32 unknown08[2]; s32 field10; } Object;
/* Whole 16-byte map rectangle (first corner +0, last corner +8). */
extern Bounds D_801429C0;
extern Position *func_800A3610(Position *, Iterator *);
extern Tile *func_800B4D80(Position *);
extern s32 func_800AEC2C(Tile *);
static __inline__ void copy_bounds(Bounds *dst, Bounds *src) {
    dst->first = src->first;
    dst->last = src->last;
}
static __inline__ s32 has_next(Iterator *iterator) {
    return iterator->current.field00 <= iterator->last.field00;
}
s32 func_800D2C5C(Object *object) {
    Bounds bounds;
    Iterator iterator;
    Position position;
    Bounds *source = object->field04;
    s32 total = object->field10;
    if (!source) {
        copy_bounds(&bounds, &D_801429C0);
    } else {
        copy_bounds(&bounds, source);
    }
    position.field00 = bounds.first.field00;
    position.field04 = bounds.first.field04;
    iterator.first = position;
    iterator.current = iterator.first;
    position.field00 = bounds.last.field00;
    position.field04 = bounds.last.field04;
    iterator.last = position;
    for (;;) {
        Tile *tile;
        if (!has_next(&iterator)) break;
        func_800A3610(&position, &iterator);
        tile = func_800B4D80(&position);
        if (tile && tile->field01 != 0xf2) total += func_800AEC2C(tile);
    }
    return total;
}
