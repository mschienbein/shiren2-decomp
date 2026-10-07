#include "common.h"

typedef unsigned char u8;
typedef struct {
    s32 x;
    s32 y;
} Vec2;
typedef struct { u8 value; } Dir;
void *func_800A27A4(void *out, void *from, void *to);
s32 func_80083D40(s32);
void func_800A2F80(Dir *, s32);
void func_800A2F94(Dir *, s32);

Dir *func_800A22B8(Dir *out, Vec2 *from, Vec2 *to)
{
    Vec2 target;
    Vec2 *pt = &target;
    Dir dir;
    s32 dy;
    s32 dx;
    s32 kind;

    pt->x = to->x;
    pt->y = to->y;
    func_800A27A4(&dir, from, pt);
    dy = func_80083D40(to->y - from->y);
    dx = func_80083D40(to->x - from->x);

    switch (kind = dir.value) {
    case 1:
    case 5:
        if (dx * 2 < dy) {
            func_800A2F94(&dir, 1);
        } else if (dy * 2 < dx) {
            func_800A2F80(&dir, 1);
        }
        break;
    case 3:
    case 7:
        if (dx * 2 < dy) {
            func_800A2F80(&dir, 1);
        } else if (dy * 2 < dx) {
            func_800A2F94(&dir, 1);
        }
        break;
    }
    *out = dir;
    return out;
}
