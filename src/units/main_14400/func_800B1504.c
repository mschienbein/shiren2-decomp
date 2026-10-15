#include "common.h"
#include "map_position_iterator.h"
/* 16-byte map bounds rectangle at 0x801429D0: first then last corner. */
extern MapRect D_801429D0;
extern void func_800B4A1C(MapPos *point);
extern void func_800B4E7C(MapPos *point);
static inline void first_point(MapPos *point, MapRect *bounds) {
    point->x = bounds->min.x;
    point->y = bounds->min.y;
}
static inline void last_point(MapPos *point, MapRect *bounds) {
    point->x = bounds->max.x;
    point->y = bounds->max.y;
}
static inline s32 iterator_active(MapPositionIterator *iterator) {
    return iterator->cur.x <= iterator->end.x;
}
void func_800B1504(void) {
    MapPositionIterator iterator;
    MapPos point;
    first_point(&point, &D_801429D0);
    iterator.start = point;
    iterator.cur = iterator.start;
    last_point(&point, &D_801429D0);
    iterator.end = point;
    for (;;) {
        if (!iterator_active(&iterator)) break;
        func_800A3610(&point, &iterator);
        func_800B4A1C(&point);
        func_800B4E7C(&point);
    }
}
