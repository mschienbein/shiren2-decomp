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
    u8 padE[0x1E];
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
extern u8 D_6000000[];

void func_80080558(Gfx **gfxp, s32 flipX, s32 flipY, s32 slot, s32 unused, s32 *ids, s32 count, s32 colorIndex) {
    s32 i;
    s32 id;
    s32 x;
    s32 y;
    s32 cell;
    s32 tmem;
    s32 hasPalette;
    s32 line = slot % 4 * 8;
    s32 palette = slot / 4;
    Entry80080558 *entry;

    GFX_PUT(gfxp, 0xE3001001, 0xC000);
    for (i = 0; i < count; i++) {
        id = ids[i];
        hasPalette = D_801A906C[id] != 0;
        entry = &D_801A9080[id];
        GFX_PUT(gfxp, 0xFD700000, (u32)D_6000000);
        GFX_PUT(gfxp, 0xF5700000, 0x07000000);
        GFX_PUT(gfxp, 0xE6000000, 0);
        GFX_PUT(gfxp, 0xF3000000, 0x0713F400);
        GFX_PUT(gfxp, 0xE7000000, 0);
        GFX_PUT(gfxp, 0xF5600400, hasPalette << 20);
        GFX_PUT(gfxp, 0xF2000000, 0x0007C09C);
        GFX_PUT(gfxp, 0xFA000000, (D_8013E8AC[colorIndex].r << 24) | (D_8013E8AC[colorIndex].g << 16) | (D_8013E8AC[colorIndex].b << 8) | 0xFF);
        if (flipX == 0) {
            x = entry->x6 - 1;
        } else {
            x = entry->x6 + entry->wA;
        }
        if (flipY == 0) {
            y = entry->y8 - 1;
        } else {
            y = entry->y8 + entry->hC;
        }
        cell = D_801A8BA0[y][x];
        if ((cell & 0xF) != id) {
            continue;
        }
        tmem = line;
        if (cell & 0xF0) {
            tmem += 0x10;
        }
        GFX_PUT(gfxp, 0xE4000000 | ((((x + 1) * 32) & 0xFFF) << 12) | (((y + 1) * 32) & 0xFFF), (((x * 32) & 0xFFF) << 12) | ((y * 32) & 0xFFF));
        GFX_PUT(gfxp, 0xE1000000, (tmem << 21) | ((palette << 8) & 0xFFFF));
        GFX_PUT(gfxp, 0xF1000000, 0x04000400);
    }
}
