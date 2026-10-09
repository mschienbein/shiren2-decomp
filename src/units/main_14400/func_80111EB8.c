#include "common.h"
typedef unsigned char u8;
typedef struct { u8 v; } Dir;
typedef struct { s32 x; s32 y; } Pos;
typedef struct { Pos pos; Dir dir; u8 pad9[0xF]; u8 active; u8 pad19[0x13]; Pos target; u8 flags; } Obj;
extern u8 D_80147620[];
void *func_800A2594(void *out, void *from, Dir dir);
u32 func_800B1C6C(Pos *pos);
void func_800A2758(Pos *p, Dir d);
u8 func_800C57A0(void *rng);
/* ODD_C: direction and wall helpers keep the one-byte Dir aggregate temporaries;
   the signed-char step keeps each step constant out of a full-width register. */
static inline Dir rotate(Dir *d, signed char step) { Dir r; r.v = (d->v + step) & 7; return r; }
static inline void turn(Dir *out, Dir *d, s32 step) { out->v = (d->v + step) & 7; }
static inline s32 isWall(Pos *p) { s32 flags = func_800B1C6C(p) & 0x4000; return flags != 0; }
static inline u8 direction(Dir *d) { return d->v; }

s32 func_80111EB8(Obj *o, Pos *pos, Dir dir)
{
    Pos left;
    Pos right;
    u32 walls;
    u8 mode;
    if (direction(&dir) & 1) {
        s32 leftWall;
        s32 rightWall;
        func_800A2594(&left, pos, rotate(&dir, -3));
        leftWall = isWall(&left);
        func_800A2594(&right, pos, rotate(&dir, 3));
        rightWall = isWall(&right);
        walls = leftWall;
        if (rightWall) walls |= 2;
        mode = walls;
        if (mode == 3) return 0;
        if (o->flags & 1) return 0;
        if (mode == 0) mode = (func_800C57A0(D_80147620) & 1) ? 1 : 2;
        if (mode == 1) {
            Dir nd;
            func_800A2758(pos, rotate(&dir, 3));
            o->pos = *pos;
            turn(&nd, &dir, 2);
            o->dir = nd;
        } else {
            Dir nd;
            func_800A2758(pos, rotate(&dir, -3));
            o->pos = *pos;
            turn(&nd, &dir, -2);
            o->dir = nd;
        }
        o->target = *pos;
        o->active = 1;
        o->flags |= 1;
        return 1;
    }
    return 0;
}
