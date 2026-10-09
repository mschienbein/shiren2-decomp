#include "common.h"

typedef unsigned char u8;

typedef struct {
    s32 x;
    s32 y;
} Pair;

typedef struct {
    Pair lower;
    Pair upper;
} Bounds;

/* Rectangle iterator: current position within [lower, upper]. */
typedef struct {
    Pair current;
    Pair lower;
    Pair upper;
} Iter;

extern Bounds D_801429D0;
extern u8 D_80146468[][76];
extern u8 D_80145460[][76];
extern Pair *func_800A3610(Pair *out, Iter *it);

static inline void lower_point(Pair *point, Bounds *bounds) {
    point->x = bounds->lower.x;
    point->y = bounds->lower.y;
}

static inline void upper_point(Pair *point, Bounds *bounds) {
    point->x = bounds->upper.x;
    point->y = bounds->upper.y;
}

static inline s32 iter_has_next(Iter *it) {
    return it->current.x <= it->upper.x;
}

/* Mark every cell of the map bounds as unset (0xFF) in both grids. */
void func_800B1130(void) {
    Iter it;
    Pair point;

    lower_point(&point, &D_801429D0);
    it.lower = point;
    it.current = it.lower;
    upper_point(&point, &D_801429D0);
    it.upper = point;
    for (;;) {
        s32 valid = iter_has_next(&it);

        if (!valid) {
            break;
        }
        func_800A3610(&point, &it);
        D_80146468[point.x][point.y] = 0xFF;
        D_80145460[point.x][point.y] = 0xFF;
    }
}
