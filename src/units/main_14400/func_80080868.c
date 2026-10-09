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
} Color80080558;

/* 0x2C-byte room/sprite entry. */
typedef struct {
    u8 pad0[6];
    u16 x6;
    u16 y8;
    u16 wA;
    u16 hC;
    u8 padE[4];
    u16 trim12;
    u8 pad14[0x18];
} Entry80080558;

/* Same shape as the libultra gbi packet macros: claim one command, then fill it. */
#define GFX_PUT(gfxp, hi, lo)              \
    {                                      \
        Gfx *_g = (Gfx *)((*(gfxp))++);    \
        _g->w0 = (hi);                     \
        _g->w1 = (lo);                     \
    }

extern u16 D_801A906C[];
extern Entry80080558 D_801A9080[];
extern Color80080558 D_8013E8AC[];
extern u8 D_801A8BA0[][40];
extern const u8 D_6000000[];

/* Draws the runs of map cells owned by each listed entry along one edge of its
 * rectangle. `unused` mirrors func_80080558's caller-supplied fifth argument;
 * it is never read. */
void func_80080868(Gfx **gfxp, s32 flipX, s32 flipY, s32 slot, s32 unused, s32 *ids, s32 count, s32 colorIndex)
{
    s32 i;
    s32 id;
    s32 x;
    s32 y;
    s32 x2 = 0;
    s32 left;
    s32 top;
    s32 right;
    s32 bottom;
    s32 hasPalette;
    s32 line = slot % 4 * 8;
    s32 palette = slot / 4;
    s32 tile = palette * 8;
    s32 masks = line < 2 ? 3 : 0;
    s32 y2 = 0;
    Entry80080558 *entry;

    GFX_PUT(gfxp, 0xE3001001, 0xC000);
    for (i = 0; i < count; i++) {
        id = ids[i];
        hasPalette = D_801A906C[id] != 0;
        entry = &D_801A9080[id];
        GFX_PUT(gfxp, 0xFD700000, (u32)(D_6000000 + tile / 4 * 64));
        GFX_PUT(gfxp, 0xF5700000, 0x0700C000 | (masks << 4));
        GFX_PUT(gfxp, 0xE6000000, 0);
        GFX_PUT(gfxp, 0xF3000000, 0x070FF400);
        GFX_PUT(gfxp, 0xE7000000, 0);
        GFX_PUT(gfxp, 0xF5600400, (hasPalette << 20) | 0xC000 | (masks << 4));
        GFX_PUT(gfxp, 0xF2000000, 0x0007C07C);
        GFX_PUT(gfxp, 0xFA000000, ((u32)D_8013E8AC[colorIndex].r << 24) | (D_8013E8AC[colorIndex].g << 16) | (D_8013E8AC[colorIndex].b << 8) | 0xFF);
        left = entry->x6;
        right = left + entry->wA;
        top = entry->y8;
        bottom = top + entry->hC;
        if (flipX == 0 && flipY == 0 && entry->trim12 != 0) {
            right -= 2;
        }
        if (flipX != 0) {
            y = top;
            if (flipY != 0) {
                x = right;
            } else {
                x = left - 1;
            }
            x2 = x + 1;
        } else {
            x = left;
            if (flipY != 0) {
                y = bottom;
            } else {
                y = top - 1;
            }
            y2 = y + 1;
        }
        while (flipX != 0 ? y < bottom : x < right) {
            if ((D_801A8BA0[y][x] & 0xF) == id) {
                if (flipX != 0) {
                    y2 = y + 1;
                    while (y2 < bottom && (D_801A8BA0[y2][x] & 0xF) == id) {
                        y2++;
                    }
                } else {
                    for (x2 = x + 1; x2 < right; x2++) {
                        if ((D_801A8BA0[y][x2] & 0xF) != id) {
                            break;
                        }
                    }
                }
                GFX_PUT(gfxp, 0xE4000000 | (((x2 * 32) & 0xFFF) << 12) | ((y2 * 32) & 0xFFF), (((x * 32) & 0xFFF) << 12) | ((y * 32) & 0xFFF));
                GFX_PUT(gfxp, 0xE1000000, ((u32)line << 21) | (((u32)palette << 8) & 0xFFFF));
                GFX_PUT(gfxp, 0xF1000000, 0x04000400);
                if (flipX != 0) {
                    y = y2;
                } else {
                    x = x2;
                }
            } else if (flipX != 0) {
                y++;
            } else {
                x++;
            }
        }
    }
}
