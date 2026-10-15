#include "common.h"
#include "map_position_iterator.h"

/* 16-byte map bounds rectangle at 0x801429D0: lower then upper corner. */
typedef struct {
    unsigned char field_00;
    unsigned char field_01;
} Entry;
extern MapRect D_801429D0;
extern s32 func_800B4F74(MapPos *);
extern Entry *func_800B4D80(MapPos *);
extern void func_800B4E7C(MapPos *);

static inline s32 has_next(MapPositionIterator *iterator) {
    return iterator->cur.x <= iterator->end.x;
}

static inline s32 entry_kind(Entry *entry) {
    return entry->field_01;
}

static inline void start_point(MapPos *point, MapRect *bounds) {
    point->x = bounds->min.x;
    point->y = bounds->min.y;
}

static inline void end_point(MapPos *point, MapRect *bounds) {
    point->x = bounds->max.x;
    point->y = bounds->max.y;
}

void func_800B15C8(s32 kind) {
    MapPositionIterator iterator;
    MapPos point;
    start_point(&point, &D_801429D0);
    iterator.start = point;
    iterator.cur = iterator.start;
    end_point(&point, &D_801429D0);
    iterator.end = point;
    while (has_next(&iterator)) {
        s32 matches;
        func_800A3610(&point, &iterator);
        matches = 0;
        if (func_800B4F74(&point)) {
            matches = entry_kind(func_800B4D80(&point)) == kind;
        }
        if (matches) {
            func_800B4E7C(&point);
        }
    }
}
