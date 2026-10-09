#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { s32 x, y; } Pos;
typedef struct { s32 a, b; } Tmp;
typedef struct Unit { Pos pos; char pad8[0x81]; u8 x89; char pad8A[0x16]; struct Unit *targetA0; } Unit;
extern u32 D_8013960C;
s32 func_800E20CC(void *u);
s32 func_800A4520(void *u, void *arg);
u16 func_800E08B0(Unit *u);
u16 func_800E08F0(Unit *u);
Unit *func_800FD4B4(Unit *u);
s32 func_800A23E8(Pos *a, Pos *b);
s32 func_800A5D2C(void *u, void *p, s32 flag);
s32 func_80049CB4(s32 id, ...);
void func_800A58FC(Unit *u, Pos *p);
s32 func_800A251C(Pos *a, Pos *b);
void *func_800A65E4(void *out, void *u, void *t);
void func_800A665C(Unit *u, Tmp *in);
s32 func_800E0534(void *t, s32 v);
char *func_800A3B20(void *u);
void func_800497F0(s32 id, ...);
s32 func_800FD6C4(Unit *u, void *arg) {
    Unit *t;
    Pos self;
    Pos *selfp;
    Pos dst;
    Pos tmp;
    s32 handle;
    if (!func_800E20CC(u)) {
        t = u->targetA0;
        if (t == 0) {
            if (func_800A4520(u, arg)) return 0;
        } else if (func_800E08B0(t)) {
            goto haveTarget;
        }
    }
    t = func_800FD4B4(u);
haveTarget:
    if (t == 0) t = u;
    selfp = &self;
    self.x = u->pos.x;
    selfp->y = u->pos.y;
    dst.x = t->pos.x;
    dst.y = t->pos.y;
    tmp.x = dst.x;
    tmp.y = dst.y;
    if (func_800A23E8(selfp, &tmp) >= 2) {
        if (func_800A5D2C(u, &dst, 1)) {
            func_80049CB4(0x108C, u, selfp, &dst);
            func_800A58FC(u, &dst);
        } else if (func_800E08B0(u) < func_800E08F0(u)) {
            t = u;
            dst = self;
        } else if (func_800A4520(u, arg)) {
            return 0;
        }
    }
    {
        s32 apart = func_800A251C(&self, &dst) != 1;
        if (apart) {
            Tmp x;
            func_800A65E4(&x, u, t);
            func_800A665C(u, &x);
        }
    }
    handle = func_80049CB4(0x59, u);
    if (func_800E08B0(t) < func_800E08F0(t)) {
        char *a;
        func_80049CB4(0x80, t);
        D_8013960C <<= 1;
        func_800E0534(t, u->x89);
        D_8013960C >>= 1;
        a = func_800A3B20(u);
        func_800497F0(0x130, handle, a, func_800A3B20(t));
    }
    return 1;
}
