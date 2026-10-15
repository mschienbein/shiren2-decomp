#include "common.h"
#include "map_position_iterator.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;
typedef float f32;

extern MapRect D_801429C0;
void func_800B4A1C(MapPos *pos);
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
void func_800B4ACC(void) {
    MapPositionIterator range;
    MapPositionIterator *r = &range;
    MapPos pos;
    range_set_start(&range, &pos, D_801429C0.min.x, D_801429C0.min.y);
    range_set_end(&range, &pos, D_801429C0.max.x, D_801429C0.max.y);
    for (;;) {
        s32 valid = range.cur.x <= r->end.x;
        if (!valid) break;
        func_800A3610(&pos, r);
        func_800B4A1C(&pos);
    }
}
