#include "common.h"
#include "map_position_iterator.h"
#include "map_line_iterator.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct { MapRect rect; u8 pad10[4]; } Room800BCB18;
typedef struct { u8 pad0[0x2]; u8 flags2; u8 pad3[0x9]; u8 flagsC; } Obj800BCB18;
typedef struct {
    u8 pad0[0x3DC];
    s32 roomCount;
    u8 pad3E0[0x22];
    u16 unk402;
    u8 pad404[0x554];
    u16 flags958;
    u8 pad95A[0x2];
    s32 roomUsable[16];
    u8 pad99C[0x10];
    MapRect rect;
} Floor800BCB18;
extern Room800BCB18 D_801431F0[];
extern u8 D_80147620[];
extern u8 D_80156AD7;
u8 func_800C57CC(void *rng, s32 limit);
s32 func_800A3138(MapRect *room);
s32 func_800A315C(MapRect *room);
s32 func_800BB22C(void *floor, MapRect *room);
s32 func_800BAFE4(void *floor, MapRect *room);
s32 func_800C5844(void *rng, u8 base, u8 top);
u8 func_800C57A0(void *rng);
void func_800B1B58(MapPos *pos, u16 arg);
MapPos *func_800A33DC(MapPos *out, MapRect *rect);
void *func_800AC5B4(s32 size, s32 arg1);
Obj800BCB18 *func_80124820(void *mem);
s32 func_800AC670(Obj800BCB18 *obj);
s32 func_800AE18C(void *obj, MapPos *pos);
s32 func_800B4F74(MapPos *pos);
s32 func_800C587C(void *rng, u8 chance);
Obj800BCB18 *func_800AAC20(s32 arg);
void *func_800AC244(u8 id);
s32 func_800BCB18(Floor800BCB18 *floor) {
    MapRect outer;
    MapRect inner;
    MapLineIterator edge;
    MapPositionIterator it;
    s32 width = 0;
    s32 tries;
    Room800BCB18 *room;
    s32 height = 0;
    Obj800BCB18 *obj;
    MapPos *p;
    MapPos *q;
    MapPos *cell;
    s32 n;
    s32 d;
    s32 hi;
    s32 lo;
    s32 side;
    s32 usable;
    MapRect *rect;
    if (!(floor->flags958 & 8)) {
        return 0;
    }
    room = 0;
    for (tries = 100; ; ) {
        u8 r;
        if (--tries == -1) {
            break;
        }
        r = func_800C57CC(D_80147620, (u8)(floor->roomCount - 1));
        if (floor->roomUsable[r] == 0) {
            continue;
        }
        room = &D_801431F0[r];
        width = func_800A3138(&room->rect);
        height = func_800A315C(&room->rect);
        if (width == 5 && height == width) {
            if (func_800BB22C(floor, &room->rect)) {
                width = 6;
            }
            if (func_800BAFE4(floor, &room->rect)) {
                height = 6;
            }
        }
        if (width < 6 || height < 6) {
            continue;
        }
        floor->roomUsable[r] = 0;
        break;
    }
    if (tries < 0) {
        return 0;
    }
    n = 5;
    if (width - 4 < n) {
        n = width - 4;
    }
    d = width - (u8)func_800C5844(D_80147620, 2, n);
    lo = d / 2;
    hi = lo;
    if (d &= 1) {
        if (func_800C57A0(D_80147620) & 1) {
            hi = lo + 1;
        } else {
            lo++;
        }
    }
    outer.min.y = room->rect.min.y + hi - 1;
    outer.max.y = room->rect.max.y - lo + 1;
    n = 5;
    if (height - 4 < n) {
        n = height - 4;
    }
    d = height - (u8)func_800C5844(D_80147620, 2, n);
    lo = d / 2;
    hi = lo;
    if (d &= 1) {
        if (func_800C57A0(D_80147620) & 1) {
            hi = lo + 1;
        } else {
            lo++;
        }
    }
    outer.min.x = room->rect.min.x + hi - 1;
    outer.max.x = room->rect.max.x - lo + 1;
    inner.min.x = outer.min.x + 1;
    inner.min.y = outer.min.y + 1;
    inner.max.x = outer.max.x - 1;
    inner.max.y = outer.max.y - 1;
    floor->rect.min = inner.min;
    floor->rect.max = inner.max;
    edge.index = 0;
    edge.length = 0;
    side = 0;
    p = &inner.min;
    for (; ; side++) {
        s32 more = side < 4;
        if (!more) {
            break;
        }
        func_800C25F4(&edge, &floor->rect, side, 1);
        for (;;) {
            s32 pending = edge.index < edge.length;
            if (!pending) {
                break;
            }
            func_800C2758(p, &edge);
            func_800B1B58(p, floor->unk402);
        }
    }
    rect = &floor->rect;
    func_800A33DC(&inner.max, rect);
    inner.min = inner.max;
    obj = func_80124820(func_800AC5B4(0x10, 1));
    q = &inner.min;
    if ((usable = func_800AC670(obj) ^ 1)) {
        obj->flagsC |= 2;
        obj->flags2 &= ~0x10;
        func_800AE18C(obj, q);
    }
    inner.max.x = rect->min.x;
    inner.max.y = rect->min.y;
    it.start = inner.max;
    it.cur = it.start;
    inner.max.x = floor->rect.max.x;
    inner.max.y = floor->rect.max.y;
    it.end = inner.max;
    cell = q;
    for (;;) {
        Obj800BCB18 *item;
        s32 more = it.cur.x <= it.end.x;
        if (!more) {
            break;
        }
        func_800A3610(cell, &it);
        if (!func_800B4F74(cell)) {
            if (func_800C587C(D_80147620, D_80156AD7)) {
                item = func_800AAC20(0);
            } else {
                item = func_800AC244(0xCC);
            }
            if (item != 0) {
                func_800AE18C(item, &inner.min);
            }
        }
    }
    return 1;
}
