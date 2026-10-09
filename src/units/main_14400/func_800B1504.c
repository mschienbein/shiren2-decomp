#include "common.h"
typedef struct { s32 x; s32 y; } Point;
/* 16-byte map bounds rectangle at 0x801429D0: first then last corner. */
typedef struct { Point first; Point last; } Bounds;
typedef struct { Point current; Point first; Point last; } Iterator;
extern Bounds D_801429D0;
extern Point *func_800A3610(Point *out, Iterator *iterator);
extern void func_800B4A1C(Point *point);
extern void func_800B4E7C(Point *point);
static inline void first_point(Point *point, Bounds *bounds) {
    point->x = bounds->first.x;
    point->y = bounds->first.y;
}
static inline void last_point(Point *point, Bounds *bounds) {
    point->x = bounds->last.x;
    point->y = bounds->last.y;
}
static inline s32 iterator_active(Iterator *iterator) {
    return iterator->current.x <= iterator->last.x;
}
void func_800B1504(void) {
    Iterator iterator;
    Point point;
    first_point(&point, &D_801429D0);
    iterator.first = point;
    iterator.current = iterator.first;
    last_point(&point, &D_801429D0);
    iterator.last = point;
    for (;;) {
        if (!iterator_active(&iterator)) break;
        func_800A3610(&point, &iterator);
        func_800B4A1C(&point);
        func_800B4E7C(&point);
    }
}
