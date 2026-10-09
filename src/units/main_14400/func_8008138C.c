#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct {
    u32 w0;
    u32 w1;
} Gfx;

typedef struct {
    u8 r;
    u8 g;
    u8 b;
} Color8008138C;

/* 0x2C-byte room/sprite entry (see func_80081040, func_80080558). */
typedef struct {
    u16 field00;
    u16 id02;
    u16 field04;
    u16 x06;
    u16 y08;
    u16 dx0A;
    u8 pad0C[6];
    u16 layer12;
    u8 pad14[0x18];
} Entry8008138C;

/* libultra gbi field packer. */
#define SHIFTL(v, s, w) ((u32)(((u32)(v) & ((0x01 << (w)) - 1)) << (s)))

/* Same shape as the libultra gbi packet macros: claim one command, then fill it. */
#define GFX_PUT(gfxp, hi, lo)              \
    {                                      \
        Gfx *_g = (Gfx *)((*(gfxp))++);    \
        _g->w0 = (hi);                     \
        _g->w1 = (lo);                     \
    }

extern u16 D_801A906C[];
extern Entry8008138C D_801A9080[];
extern Color8008138C D_8013E8AC[];
extern u8 D_801A8BA0[][40];
extern const u8 D_6000000[];

static inline s32 texture_bank(s32 alternate)
{
    return alternate != 0;
}

/* Draws the 16x8 border tiles of every entry owned by `id`, layer by layer
 * (1..3), each with its own texture-rectangle s/t origin.
 * `unused`: the only caller (func_8007F350) passes 0; it is never read. */
void func_8008138C(Gfx **gfxp, s32 id, s32 unused, s32 colorIndex) {
    s32 selected[10];
    s32 count = 0;
    s32 i, j;
    s32 s, t;
    s32 width, height;

    for (i = 0; i < 10; i++) {
        if (D_801A9080[i].id02 == id) {
            selected[count++] = i;
        }
    }
    if (count == 0) {
        return;
    }
    for (i = 1; i < 4; i++) {
        switch (i) {
        case 1:
            s = 0;
            t = 0x28;
            break;
        case 2:
            s = 0x10;
            t = 0x28;
            break;
        case 3:
        default:
            s = 0x10;
            t = 0x20;
            break;
        }
        width = 16;
        height = 8;
        for (j = 0; j < count; j++) {
            s32 index = selected[j];
            if (D_801A9080[index].layer12 == i) {
                s32 bordered = D_801A906C[index];
                s32 col, row, x, y, x0, y0, x1, y1;
                GFX_PUT(gfxp, 0xFD500000, (u32)D_6000000);
                GFX_PUT(gfxp, 0xF5500000, 0x07000000);
                GFX_PUT(gfxp, 0xE6000000, 0);
                GFX_PUT(gfxp, 0xF3000000, 0x0717F400);
                GFX_PUT(gfxp, 0xE7000000, 0);
                GFX_PUT(gfxp, 0xF5400400, texture_bank(bordered) << 20);
                GFX_PUT(gfxp, 0xF2000000, 0x0007C0BC);
                GFX_PUT(gfxp, 0xFA000000, (D_8013E8AC[colorIndex].r << 24) | (D_8013E8AC[colorIndex].g << 16) | (D_8013E8AC[colorIndex].b << 8) | 0xFF);
                col = D_801A9080[index].x06 + D_801A9080[index].dx0A - 2;
                row = D_801A9080[index].y08 - 1;
                x = col * 8;
                y = row * 8;
                x0 = x / 8;
                y0 = y / 8;
                x1 = (x + width - 1) / 8;
                y1 = (y + height - 1) / 8;
                if ((D_801A8BA0[y0][x0] & 0xF) == index &&
                    (D_801A8BA0[y0][x1] & 0xF) == index &&
                    (D_801A8BA0[y1][x0] & 0xF) == index &&
                    (D_801A8BA0[y1][x1] & 0xF) == index) {
                    GFX_PUT(gfxp, 0xE4000000 | ((((x + width) * 4) & 0xFFF) << 12) | (((y + height) * 4) & 0xFFF),
                            (((x * 4) & 0xFFF) << 12) | ((y * 4) & 0xFFF));
                    GFX_PUT(gfxp, 0xE1000000, SHIFTL(s << 5, 16, 16) | SHIFTL(t << 5, 0, 16));
                    GFX_PUT(gfxp, 0xF1000000, 0x04000400);
                }
            }
        }
    }
}
