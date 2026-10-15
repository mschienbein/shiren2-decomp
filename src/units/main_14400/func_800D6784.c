#include "common.h"
#include "map_position_iterator.h"

typedef unsigned char u8;


extern MapRect *D_80148080;
extern MapPos D_80148000;
extern u8 D_80148008[];
u32 func_800B1C6C(MapPos *pos);
s32 func_800D6AF8(void *map, void *filter, MapPos *pos);
s32 func_800D6D04(void *map, MapPos *pos, MapPos *best);

s32 func_800D6784(MapPos *best, u8 *flag) {
    MapRect *rect = D_80148080;
    MapPositionIterator range;
    MapPos pos;
    s32 better;

    best->x = 0;
    best->y = 0;
    pos.x = rect->min.x;
    pos.y = rect->min.y;
    range.start = pos;
    range.cur = range.start;
    pos.x = rect->max.x;
    pos.y = rect->max.y;
    range.end = pos;
    for (;;) {
        MapPositionIterator *r = &range;
        s32 more = r->cur.x <= r->end.x;

        if (!more) {
            break;
        }
        func_800A3610(&pos, r);
        if (!(func_800B1C6C(&pos) & 0x800)) {
            continue;
        }
        if (!func_800D6AF8(&D_80148000, D_80148008, &pos)) {
            continue;
        }
        better = 0;
        if ((best->y | best->x) == 0 || func_800D6D04(&D_80148000, &pos, best)) {
            better = 1;
        }
        if (!better) {
            continue;
        }
        *best = pos;
        *flag = 0;
    }
    return (best->y | best->x) != 0;
}
