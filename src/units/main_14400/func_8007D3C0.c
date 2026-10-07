#include "common.h"

typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;

typedef struct {
    u32 w0;
    u32 w1;
} Gfx;

/* gDPSetTextureLUT + gDPLoadTLUT_pal16 + gDPLoadSync, one block per macro */
Gfx *func_8007D3C0(Gfx *gfx, void *palette, s32 pal) {
    { /* gDPSetTextureLUT(G_TT_IA16) */
        Gfx *g = gfx++;
        g->w0 = 0xE3001001;
        g->w1 = 0xC000;
    }
    { /* gDPSetTextureImage(RGBA, 16b, 1, palette) */
        Gfx *g = gfx++;
        g->w0 = 0xFD100000;
        g->w1 = (u32)palette;
    }
    { /* gDPTileSync */
        Gfx *g = gfx++;
        g->w0 = 0xE8000000;
        g->w1 = 0;
    }
    { /* gDPSetTile(tmem 256 + pal * 16, G_TX_LOADTILE) */
        Gfx *g = gfx++;
        g->w0 = 0xF5000100 | ((pal & 0xF) << 4);
        g->w1 = 0x07000000;
    }
    { /* gDPLoadSync */
        Gfx *g = gfx++;
        g->w0 = 0xE6000000;
        g->w1 = 0;
    }
    { /* gDPLoadTLUTCmd(G_TX_LOADTILE, 15) */
        Gfx *g = gfx++;
        g->w0 = 0xF0000000;
        g->w1 = 0x0703C000;
    }
    { /* gDPPipeSync */
        Gfx *g = gfx++;
        g->w0 = 0xE7000000;
        g->w1 = 0;
    }
    { /* gDPLoadSync */
        Gfx *g = gfx++;
        g->w0 = 0xE6000000;
        g->w1 = 0;
    }
    return gfx;
}
