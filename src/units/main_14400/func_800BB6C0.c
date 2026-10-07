#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;
typedef float f32;

typedef struct { s32 x; s32 y; } Pos800BB6C0;
typedef struct ShirenDirection { s8 value; } ShirenDirection;
typedef struct {
    s32 x0;
    s32 y0;
    s32 x1;
    s32 y1;
    u8 open_bottom;
    u8 open_left;
    u8 open_top;
    u8 open_right;
} Room800BB6C0;
extern u8 D_80147620[];
s32 func_800A3138(Room800BB6C0 *room);
s32 func_800A315C(Room800BB6C0 *room);
s32 func_800BB474(void *self, void *position, ShirenDirection direction, s32 limit);
s32 func_800C5844(void *rng, u8 base, u8 top);
void func_800BB508(void *ctx, Pos800BB6C0 *a, Pos800BB6C0 *b, Pos800BB6C0 *c);

s32 func_800BB6C0(void *ctx, Room800BB6C0 *room) {
    Pos800BB6C0 a;
    Pos800BB6C0 b;
    Pos800BB6C0 c;
    Pos800BB6C0 probe;
    ShirenDirection dirs[8]; /* one stack slot per probe direction argument */
    s32 y0 = room->y0;
    s32 x0 = room->x0;
    s32 y1 = room->y1;
    s32 x1 = room->x1;
    s32 count_a = func_800A3138(room);
    s32 count_b = func_800A315C(room);
    s32 open_bottom = room->open_bottom;
    s32 open_left = room->open_left;
    s32 open_top = room->open_top;
    s32 open_right = room->open_right;
    u8 n;
    u8 r;
    s32 placed = 0;

    a.y = y0;
    a.x = x0;
    b.y = y0;
    c.x = x0;
    if (open_top) {
        probe.x = x0;
        probe.y = y0 - 1;
        dirs[0].value = 6;
        n = (u8)func_800BB474(ctx, &probe, dirs[0], count_b);
    } else {
        n = count_b / 2;
    }
    if (n != 0) {
        b.x = a.x + (u8)func_800C5844(D_80147620, n == 1 ? 1 : 2, n) - 1;
        if (open_left) {
            probe.x = x0 - 1;
            probe.y = y0;
            dirs[1].value = 0;
            n = (u8)func_800BB474(ctx, &probe, dirs[1], count_a);
        } else {
            n = count_a / 2;
        }
        if (n != 0) {
            r = (u8)func_800C5844(D_80147620, n == 1 ? 1 : 2, n);
            placed = 1;
            c.y = a.y + r - 1;
            func_800BB508(ctx, &a, &b, &c);
        }
    }
    a.y = y0;
    a.x = x1;
    b.y = y0;
    c.x = x1;
    if (open_top) {
        probe.x = x1;
        probe.y = y0 - 1;
        dirs[2].value = 2;
        n = (u8)func_800BB474(ctx, &probe, dirs[2], count_b);
    } else {
        n = count_b / 2;
    }
    if (n != 0) {
        b.x = a.x - (u8)func_800C5844(D_80147620, n == 1 ? 1 : 2, n) + 1;
        if (open_right) {
            probe.x = x1 + 1;
            probe.y = y0;
            dirs[3].value = 0;
            n = (u8)func_800BB474(ctx, &probe, dirs[3], count_a);
        } else {
            n = count_a / 2;
        }
        if (n != 0) {
            r = (u8)func_800C5844(D_80147620, n == 1 ? 1 : 2, n);
            placed = 1;
            c.y = a.y + r - 1;
            func_800BB508(ctx, &a, &b, &c);
        }
    }
    a.y = y1;
    a.x = x0;
    b.y = y1;
    c.x = x0;
    if (open_bottom) {
        probe.x = x0;
        probe.y = y1 + 1;
        dirs[4].value = 6;
        n = (u8)func_800BB474(ctx, &probe, dirs[4], count_b);
    } else {
        n = count_b / 2;
    }
    if (n != 0) {
        b.x = a.x + (u8)func_800C5844(D_80147620, n == 1 ? 1 : 2, n) - 1;
        if (open_left) {
            probe.x = x0 - 1;
            probe.y = y1;
            dirs[5].value = 4;
            n = (u8)func_800BB474(ctx, &probe, dirs[5], count_a);
        } else {
            n = count_a / 2;
        }
        if (n != 0) {
            r = (u8)func_800C5844(D_80147620, n == 1 ? 1 : 2, n);
            placed = 1;
            c.y = a.y - r + 1;
            func_800BB508(ctx, &a, &b, &c);
        }
    }
    a.y = y1;
    a.x = x1;
    b.y = y1;
    c.x = x1;
    if (open_bottom) {
        probe.x = x1;
        probe.y = y1 + 1;
        dirs[6].value = 2;
        n = (u8)func_800BB474(ctx, &probe, dirs[6], count_b);
    } else {
        n = count_b / 2;
    }
    if (n != 0) {
        b.x = a.x - (u8)func_800C5844(D_80147620, n == 1 ? 1 : 2, n) + 1;
        if (open_right) {
            probe.x = x1 + 1;
            probe.y = y1;
            dirs[7].value = 4;
            n = (u8)func_800BB474(ctx, &probe, dirs[7], count_a);
        } else {
            n = count_a / 2;
        }
        if (n != 0) {
            r = (u8)func_800C5844(D_80147620, n == 1 ? 1 : 2, n);
            placed = 1;
            c.y = a.y - r + 1;
            func_800BB508(ctx, &a, &b, &c);
        }
    }
    return placed;
}
