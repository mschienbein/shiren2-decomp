#include "map_position_iterator.h"

typedef unsigned char u8;

extern u8 D_801569FF;
s32 func_80049CB4(s32 id, ...);
s32 func_800B1AB8(MapPos *pos);
void *func_800B31E8(MapPos *pos, s32 team);
s32 func_80083D40(s32 value);
s32 func_800B5300(MapPos *pos, MapPos *target, u8 arg);

/* Item-effect slot +0x44 supplies self, actor and item; self and item are unused here. */
void func_8011CC30(void *unused, MapPos *target, void *item) {
    MapPos center;
    MapRect rect;
    MapPositionIterator range;
    MapPos pos;
    MapPos delta;
    s32 radius;
    s32 found;
    s32 skip;

    center.x = target->x;
    center.y = target->y;
    func_80049CB4(0x129, 0x1F);
    radius = 1;
    for (;;) {
        MapPos *origin;

        if (radius >= 4) {
            break;
        }
        func_800A2FD0(&rect, &center, radius);
        found = 0;
        pos.x = rect.min.x;
        pos.y = rect.min.y;
        range.start = pos;
        range.cur = range.start;
        pos.x = rect.max.x;
        pos.y = rect.max.y;
        range.end = pos;
        func_80049CB4(6);
        for (;;) {
            s32 more = range.cur.x <= range.end.x;

            if (!more) {
                break;
            }
            func_800A3610(&pos, &range);
            skip = 0;
            if (!func_800B1AB8(&pos) || func_800B31E8(&pos, 10)) {
                skip = 1;
            }
            if (skip) {
                continue;
            }
            origin = &center;
            delta.x = pos.x;
            delta.y = pos.y;
            delta.x -= center.x;
            delta.y -= origin->y;
            if (func_80083D40(delta.y) + func_80083D40(delta.x) != radius) {
                continue;
            }
            if (func_800B5300(&pos, target, D_801569FF)) {
                found = 1;
            }
        }
        func_80049CB4(7);
        if (found) {
            func_80049CB4(0x12E);
        }
        func_80049CB4(0x129, 8);
        radius++;
    }
}
