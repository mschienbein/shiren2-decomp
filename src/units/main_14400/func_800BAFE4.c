#include "common.h"
#include "map_position_iterator.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

extern u8 D_80147620[];
u32 func_800B1C6C(MapPos *pos);
void func_800B1AE0(MapPos *pos, u16 flags);
u8 func_800C57A0(void *table);
void func_800B17A4(void);
s32 func_800BAFE4(void *self, MapRect *rect) {
    MapPos pos;
    MapRect r;
    MapPositionIterator left;
    MapRect col;
    MapPositionIterator right;
    s32 can_left;
    s32 can_right;
    s32 more;
    u32 flags;
    MapPos *pt;
    s32 edge;
    s32 edge2;

    r.min.x = rect->min.x;
    r.min.y = rect->min.y;
    r.max.x = rect->max.x;
    r.max.y = rect->max.y;
    edge = r.min.x - 1;
    can_left = !(edge < 10);
    edge2 = r.max.x + 1;
    can_right = edge2 < 0x2C;
    col.min.x = r.min.x - 2;
    col.min.y = r.min.y;
    col.max.x = edge;
    col.max.y = r.max.y;
    left.start = col.min;
    left.cur = left.start;
    left.end = col.max;
    pt = &col.min;
scan_left:
    more = left.cur.x <= left.end.x;
    if (more) {
        func_800A3610(pt, &left);
        if (func_800B1C6C(pt) & 0xE100) {
            goto scan_left;
        }
        can_left = 0;
    }
    col.min.x = r.max.x + 1;
    col.min.y = r.min.y;
    col.max.x = r.max.x + 2;
    col.max.y = r.max.y;
    right.start = col.min;
    right.cur = right.start;
    right.end = col.max;
    pt = &col.min;
scan_right:
    more = right.cur.x <= right.end.x;
    if (more) {
        func_800A3610(pt, &right);
        if (func_800B1C6C(pt) & 0xE100) {
            goto scan_right;
        }
        can_right = 0;
    }
    if (!can_left && !can_right) {
        return 0;
    }
    flags = func_800B1C6C(&r.min);
    if (can_left && (!can_right || (func_800C57A0(D_80147620) & 1))) {
        pos.x = r.min.x - 1;
        r.min.x = pos.x;
    } else {
        pos.x = r.max.x + 1;
        r.max.x = pos.x;
    }
    for (pos.y = r.min.y; pos.y <= r.max.y; pos.y++) {
        func_800B1AE0(&pos, flags);
    }
    rect->min = r.min;
    rect->max = r.max;
    func_800B17A4();
    return 1;
}
