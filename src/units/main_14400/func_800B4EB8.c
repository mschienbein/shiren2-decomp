#include "common.h"
#include "map_position_iterator.h"


extern MapRect D_801429C0;
void func_800B4E7C(MapPos *pos);

static inline void range_set_start(MapPositionIterator *r, MapPos *p, s32 x, s32 y) {
    p->x = x;
    p->y = y;
    r->start = *p;
    r->cur = r->start;
}
static inline void range_set_end(MapPositionIterator *r, MapPos *p, s32 x, s32 y) {
    p->x = x;
    p->y = y;
    r->end = *p;
}
void func_800B4EB8(void) {
    MapPositionIterator range;
    MapPos pos;

    range_set_start(&range, &pos, D_801429C0.min.x, D_801429C0.min.y);
    range_set_end(&range, &pos, D_801429C0.max.x, D_801429C0.max.y);
    for (;;) {
        MapPositionIterator *r = &range;
        s32 more = r->cur.x <= r->end.x;

        if (!more) {
            break;
        }
        func_800A3610(&pos, r);
        func_800B4E7C(&pos);
    }
}
