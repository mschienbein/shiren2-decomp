#include "common.h"

typedef struct { u32 w0, w1; } Gfx;
extern unsigned char D_8013CA00, D_8013CA01, D_8013CA02, D_8013CA03;
extern s32 D_8016FD50, D_8016FD54;

Gfx *func_80069F14(Gfx *cursor) {
    s32 near, difference;
    {
        Gfx *g = cursor++;
        g->w0 = 0xE7000000;
        g->w1 = 0;
    }
    {
        Gfx *g = cursor++;
        g->w0 = 0xD9FFFFFF;
        g->w1 = 0x10000;
    }
    {
        Gfx *g = cursor++;
        g->w0 = 0xF8000000;
        g->w1 = ((u32)D_8013CA00 << 24) | (D_8013CA01 << 16) | (D_8013CA02 << 8) | D_8013CA03;
    }
    near = D_8016FD50;
    difference = D_8016FD54 - near;
    if (difference >= 4) {
        s32 scale = 128000 / difference;
        s32 offset = ((500 - near) * 256) / difference;
        Gfx *g = cursor++;
        g->w0 = 0xDB080000;
        g->w1 = ((u32)scale << 16) | (offset & 0xFFFF);
    } else if (near < 997) {
        Gfx *g = cursor++;
        g->w0 = 0xDB080000;
        g->w1 = 0x7D000000 | (((500 - near) * 64) & 0xFFFF);
    } else {
        Gfx *g = cursor++;
        g->w0 = 0xDB080000;
        g->w1 = 0x7D000000 | (((504 - near) * 64) & 0xFFFF);
    }
    return cursor;
}
