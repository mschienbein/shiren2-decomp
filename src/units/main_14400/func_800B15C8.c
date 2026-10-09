#include "common.h"

typedef struct {
    s32 x;
    s32 y;
} Pair;
/* 16-byte map bounds rectangle at 0x801429D0: lower then upper corner. */
typedef struct {
    Pair lower;
    Pair upper;
} Bounds;
typedef struct {
    Pair current;
    Pair start;
    Pair end;
} Iterator;
typedef struct {
    unsigned char field_00;
    unsigned char field_01;
} Entry;
extern Bounds D_801429D0;
extern Pair *func_800A3610(Pair *, Iterator *);
extern s32 func_800B4F74(Pair *);
extern Entry *func_800B4D80(Pair *);
extern void func_800B4E7C(Pair *);

static inline s32 has_next(Iterator *iterator) {
    return iterator->current.x <= iterator->end.x;
}

static inline s32 entry_kind(Entry *entry) {
    return entry->field_01;
}

static inline void start_point(Pair *point, Bounds *bounds) {
    point->x = bounds->lower.x;
    point->y = bounds->lower.y;
}

static inline void end_point(Pair *point, Bounds *bounds) {
    point->x = bounds->upper.x;
    point->y = bounds->upper.y;
}

void func_800B15C8(s32 kind) {
    Iterator iterator;
    Pair point;
    start_point(&point, &D_801429D0);
    iterator.start = point;
    iterator.current = iterator.start;
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
