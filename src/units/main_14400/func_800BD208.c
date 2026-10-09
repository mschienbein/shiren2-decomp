#include "common.h"
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;

typedef struct { s32 x, y; } Vec2;
typedef struct { Vec2 min; Vec2 max; } Rect;
typedef struct { Rect rect; u8 pad[4]; } Room;
typedef struct { Vec2 cur; Vec2 start; Vec2 end; } Iter;
typedef struct {
    u8 pad0[0x3DC];
    s32 roomCount;
    u8 pad3E0[0x22];
    u16 x402;
    u8 pad404[0x554];
    u16 x958;
    u8 pad95A[2];
    s32 used[16];
} Floor;
extern u8 D_80147620[];
extern Room D_801431F0[];
extern u8 func_800C57CC(void *, s32);
extern s32 func_800C587C(void *, u8);
extern void *func_800A3610(void *out, void *it);
extern s32 func_800B1E80(Vec2 *);
extern void func_800B1B58(Vec2 *, u16);
static inline void iter_set_start(Iter *it, Vec2 lo){ it->start = lo; it->cur = it->start; }
static inline void iter_set_end(Iter *it, Vec2 hi){ it->end = hi; }
s32 func_800BD208(Floor *f){
    Iter it;
    Rect r;
    s32 n;
    u8 idx;
    Room *e;
    Iter *ip;
    Vec2 *pos;
    if (!(f->x958 & 0x40)) return 0;
    if (f->x402 != 0x80) return 0;
    for (n = 99; n != -1; n--) {
        idx = func_800C57CC(D_80147620, (u8)(f->roomCount - 1));
        if (f->used[idx]) {
            f->used[idx] = 0;
            break;
        }
    }
    if (n < 0) return 0;
    e = &D_801431F0[idx];
    r.min.x = e->rect.min.x;
    r.min.y = e->rect.min.y;
    r.max.x = e->rect.max.x;
    r.max.y = e->rect.max.y;
    ip = &it;
    {
        Vec2 lo;
        lo.x = r.min.x;
        lo.y = r.min.y;
        it.start = lo;
        it.cur = it.start;
    }
    {
        Vec2 hi;
        hi.x = r.max.x;
        hi.y = r.max.y;
        it.end = hi;
    }
    pos = &r.min;
    for (;;) {
        s32 hit;
        s32 more = it.cur.x <= ip->end.x;
        if (!more) break;
        func_800A3610(pos, ip);
        hit = 0;
        if (func_800C587C(D_80147620, 0x4B)) hit = !func_800B1E80(pos);
        if (hit) func_800B1B58(pos, 0x80);
    }
    return 1;
}
