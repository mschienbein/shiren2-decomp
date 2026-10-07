#include "common.h"

typedef unsigned char u8;

typedef struct { u32 w0; u32 w1; } Gfx;

/* libultra Lights3 layout: an 8-byte ambient light followed by three 16-byte directional lights. */
typedef struct { u8 col[3]; u8 pad1; u8 colc[3]; u8 pad2; } Ambient;
typedef struct { u32 words[4]; } Light;
typedef struct { Ambient a; Light l[3]; } Lights3;

/* Two consecutive Lights3 records at 0x8013C990 and 0x8013C9C8 (0x70 bytes). */
extern Lights3 D_8013C990[];

/* Pipe sync, enable lighting, then gSPSetLights3 on the first light record. */
Gfx *func_80069D78(Gfx *gfx) {
    {
        Gfx *g = gfx++;

        g->w0 = 0xE7000000;
        g->w1 = 0;
    }
    {
        Gfx *g = gfx++;

        g->w0 = 0xD9FFFFFF;
        g->w1 = 0x20000;
    }
    {
        Gfx *g = gfx++;

        g->w0 = 0xDB020000;
        g->w1 = 0x48;
    }
    {
        Gfx *g = gfx++;

        g->w0 = 0xDC08060A;
        g->w1 = (u32)&D_8013C990[0].l[0];
    }
    {
        Gfx *g = gfx++;

        g->w0 = 0xDC08090A;
        g->w1 = (u32)&D_8013C990[0].l[1];
    }
    {
        Gfx *g = gfx++;

        g->w0 = 0xDC080C0A;
        g->w1 = (u32)&D_8013C990[0].l[2];
    }
    {
        Gfx *g = gfx++;

        g->w0 = 0xDC080F0A;
        g->w1 = (u32)&D_8013C990[0].a;
    }
    return gfx;
}
