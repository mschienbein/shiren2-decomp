#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { s32 x, y; } Pos;
typedef struct { u8 v; } Dir;
typedef struct { Pos pos; Dir dir; } Mover;
void func_800A2758(Pos *, Dir);
u32 func_800B1C6C(Pos *);
void *func_800B4928(Pos *);
s32 func_800A674C(Mover *, void *);
static inline s32 Pos_isOut(Pos *p) {
    return !(p->y < 0x4C && p->x < 0x36 && p->y >= 0 && p->x >= 0);
}
static inline s32 isBlocked(Pos *p, s32 flag) {
    s32 stop = 0;
    if (!(func_800B1C6C(p) & 0xC000) || flag) {
        if (Pos_isOut(p)) {
            stop = 1;
        }
    } else {
        stop = 1;
    }
    return stop;
}
void *func_800A6BA4(Mover *self, s32 count, s32 flag) {
    Pos pos;
    Dir dir;
    dir = self->dir;
    pos.x = self->pos.x;
    pos.y = self->pos.y;
    while (1) {
        void *e;
        if (count-- <= 0) break;
        func_800A2758(&pos, dir);
        if (isBlocked(&pos, flag)) return 0;
        e = func_800B4928(&pos);
        if (e == 0) continue;
        if (func_800A674C(self, e)) return e;
    }
    return 0;
}
