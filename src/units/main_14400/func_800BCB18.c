#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct { s32 x; s32 y; } Pos800BCB18;
typedef struct { Pos800BCB18 tl; Pos800BCB18 br; } Rect800BCB18;
typedef struct { Rect800BCB18 rect; u8 pad10[4]; } Room800BCB18;
typedef struct { Pos800BCB18 cur; Pos800BCB18 start; Pos800BCB18 end; } RectIter800BCB18;
typedef struct { u8 pad0[0xC]; s32 count; s32 index; } EdgeIter800BCB18;
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
    Rect800BCB18 rect;
} Floor800BCB18;
extern Room800BCB18 D_801431F0[];
extern u8 D_80147620[];
extern u8 D_80156AD7;
u8 func_800C57CC(void *rng, s32 limit);
s32 func_800A3138(Room800BCB18 *room);
s32 func_800A315C(Room800BCB18 *room);
s32 func_800BB22C(Floor800BCB18 *floor, Room800BCB18 *room);
s32 func_800BAFE4(Floor800BCB18 *floor, Room800BCB18 *room);
s32 func_800C5844(void *rng, u8 base, u8 top);
u8 func_800C57A0(void *rng);
void func_800C25F4(EdgeIter800BCB18 *it, Rect800BCB18 *rect, s32 side, s32 arg3);
Pos800BCB18 *func_800C2758(Pos800BCB18 *out, EdgeIter800BCB18 *it);
void func_800B1B58(Pos800BCB18 *pos, u16 arg);
Pos800BCB18 *func_800A33DC(Pos800BCB18 *out, Rect800BCB18 *rect);
void *func_800AC5B4(s32 size, s32 arg1);
Obj800BCB18 *func_80124820(void *mem);
s32 func_800AC670(Obj800BCB18 *obj);
s32 func_800AE18C(void *obj, void *pos);
Pos800BCB18 *func_800A3610(Pos800BCB18 *out, RectIter800BCB18 *it);
s32 func_800B4F74(Pos800BCB18 *pos);
s32 func_800C587C(void *rng, u8 chance);
Obj800BCB18 *func_800AAC20(s32 arg);
void *func_800AC244(u8 id);
s32 func_800BCB18(Floor800BCB18 *floor) {
    Rect800BCB18 outer;
    Rect800BCB18 inner;
    EdgeIter800BCB18 edge;
    RectIter800BCB18 it;
    s32 width = 0;
    s32 tries;
    Room800BCB18 *room;
    s32 height = 0;
    Obj800BCB18 *obj;
    Pos800BCB18 *p;
    Pos800BCB18 *q;
    Pos800BCB18 *cell;
    s32 n;
    s32 d;
    s32 hi;
    s32 lo;
    s32 side;
    s32 usable;
    Rect800BCB18 *rect;
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
        width = func_800A3138(room);
        height = func_800A315C(room);
        if (width == 5 && height == width) {
            if (func_800BB22C(floor, room)) {
                width = 6;
            }
            if (func_800BAFE4(floor, room)) {
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
    outer.tl.y = room->rect.tl.y + hi - 1;
    outer.br.y = room->rect.br.y - lo + 1;
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
    outer.tl.x = room->rect.tl.x + hi - 1;
    outer.br.x = room->rect.br.x - lo + 1;
    inner.tl.x = outer.tl.x + 1;
    inner.tl.y = outer.tl.y + 1;
    inner.br.x = outer.br.x - 1;
    inner.br.y = outer.br.y - 1;
    floor->rect.tl = inner.tl;
    floor->rect.br = inner.br;
    edge.index = 0;
    edge.count = 0;
    side = 0;
    p = &inner.tl;
    for (; ; side++) {
        s32 more = side < 4;
        if (!more) {
            break;
        }
        func_800C25F4(&edge, &floor->rect, side, 1);
        for (;;) {
            s32 pending = edge.index < edge.count;
            if (!pending) {
                break;
            }
            func_800C2758(p, &edge);
            func_800B1B58(p, floor->unk402);
        }
    }
    rect = &floor->rect;
    func_800A33DC(&inner.br, rect);
    inner.tl = inner.br;
    obj = func_80124820(func_800AC5B4(0x10, 1));
    q = &inner.tl;
    if ((usable = func_800AC670(obj) ^ 1)) {
        obj->flagsC |= 2;
        obj->flags2 &= ~0x10;
        func_800AE18C(obj, q);
    }
    inner.br.x = rect->tl.x;
    inner.br.y = rect->tl.y;
    it.start = inner.br;
    it.cur = it.start;
    inner.br.x = floor->rect.br.x;
    inner.br.y = floor->rect.br.y;
    it.end = inner.br;
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
                func_800AE18C(item, &inner.tl);
            }
        }
    }
    return 1;
}
