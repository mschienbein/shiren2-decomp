#include "common.h"
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;

typedef struct { s32 x, y; } Vec2;
typedef struct { u8 value; } Dir;
extern u8 D_80147620[];
extern u32 func_800B1C6C(void *pos);
extern void func_800B1B58(Vec2 *, u16);
extern s32 func_800C5844(void *rng, u8 base, u8 top);
extern void func_800A2758(void *pos, Dir dir);
extern s32 func_801368B4(Vec2 *, s32);
static inline void try_dig(Vec2 *p){
    s32 hit = 0;
    if (func_801368B4(p, 0x4000)) hit = !func_801368B4(p, 0x8020);
    if (hit) func_800B1B58(p, 0x20);
}
static inline Dir *dir_set(Dir *out, s32 value){ out->value = value & 7; return out; }
void func_800BEFB0(void *map, Vec2 *from, Vec2 *to, s32 started){
    Vec2 pos;
    Vec2 *pp;
    Dir dy;
    Dir dx;
    s32 yLess;
    s32 xLess;
    u8 dirY;
    u8 dirX;
    u8 ySteps;
    u8 xSteps;
    s32 doneY;
    s32 doneX;
    yLess = from->y < to->y;
    dirY = (!yLess) << 2;
    xLess = from->x < to->x;
    dirX = 2;
    if (xLess) dirX = 6;
    pp = &pos;
    pp->x = from->x;
    pp->y = from->y;
    if (func_800B1C6C(pp) & 0x4000) func_800B1B58(pp, 0x20);
    ySteps = (yLess ? to->y - from->y : from->y - to->y) / 4;
    if (ySteps < 2) ySteps = 2;
    xSteps = (xLess ? to->x - from->x : from->x - to->x) / 4;
    if (xSteps < 2) xSteps = 2;
    doneY = 0;
    do {
        u8 rem;
        s32 small;
        u8 n;
        if (!started) {
            started = 1;
        } else {
            rem = yLess ? to->y - pos.y : pos.y - to->y;
            doneY = rem == 0;
            if (!doneY) {
                small = rem < 2;
                n = (u8)func_800C5844(D_80147620, small ? 1 : 2, (rem < ySteps) ? rem : ySteps);
                for (;;) {
                    Dir *d;
                    if (--n == 0xFF) break;
                    d = dir_set(&dy, dirY);
                    func_800A2758(&pos, *d);
                    try_dig(&pos);
                }
            }
        }
        rem = xLess ? to->x - pos.x : pos.x - to->x;
        doneX = rem == 0;
        if (!doneX) {
            small = rem < 2;
            n = (u8)func_800C5844(D_80147620, small ? 1 : 2, (rem < xSteps) ? rem : xSteps);
            for (;;) {
                Dir *d;
                if (--n == 0xFF) break;
                d = dir_set(&dx, dirX);
                func_800A2758(&pos, *d);
                try_dig(&pos);
            }
        }
    } while (!doneY || !doneX);
}
