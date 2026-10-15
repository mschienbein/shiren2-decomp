#include "common.h"
#include "map_position_iterator.h"


typedef struct Obj Obj;

extern MapRect D_801429D0;
void *func_800B4D80(MapPos *p);
s32 func_800AC670(Obj *obj);
void *func_800B4E18(MapPos *pos);

static inline void bounds_start(MapPos *pos, MapRect *bounds) {
    pos->x = bounds->min.x;
    pos->y = bounds->min.y;
}

static inline void bounds_end(MapPos *pos, MapRect *bounds) {
    pos->x = bounds->max.x;
    pos->y = bounds->max.y;
}

void func_800B16C8(void) {
    MapPositionIterator range;
    MapPositionIterator *r = &range;
    MapPos pos;

    bounds_start(&pos, &D_801429D0);
    range.start = pos;
    range.cur = range.start;
    bounds_end(&pos, &D_801429D0);
    range.end = pos;
    for (;;) {
        s32 valid = range.cur.x <= r->end.x;
        Obj *obj;

        if (!valid) {
            break;
        }
        func_800A3610(&pos, r);
        obj = func_800B4D80(&pos);
        if (obj != 0 && func_800AC670(obj) != 0) {
            func_800B4E18(&pos);
        }
    }
}
