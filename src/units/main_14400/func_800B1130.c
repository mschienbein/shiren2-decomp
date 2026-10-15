#include "common.h"
#include "map_position_iterator.h"

typedef unsigned char u8;


extern MapRect D_801429D0;
extern u8 D_80146468[][76];
extern u8 D_80145460[][76];

static inline void lower_point(MapPos *point, MapRect *bounds) {
    point->x = bounds->min.x;
    point->y = bounds->min.y;
}

static inline void upper_point(MapPos *point, MapRect *bounds) {
    point->x = bounds->max.x;
    point->y = bounds->max.y;
}

static inline s32 iter_has_next(MapPositionIterator *it) {
    return it->cur.x <= it->end.x;
}

/* Mark every cell of the map bounds as unset (0xFF) in both grids. */
void func_800B1130(void) {
    MapPositionIterator it;
    MapPos point;

    lower_point(&point, &D_801429D0);
    it.start = point;
    it.cur = it.start;
    upper_point(&point, &D_801429D0);
    it.end = point;
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
