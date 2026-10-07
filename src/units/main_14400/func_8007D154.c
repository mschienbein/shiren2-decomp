#include "common.h"

typedef struct { u32 w0; u32 w1; } Gfx;
/* gSPTextureRectangle-style packet: rectangle (x, y)-(x + w, y + h), texture origin (s, t), 1:1 scale, then load sync. */
Gfx *func_8007D154(Gfx *gdl, s32 s, s32 t, s32 w, s32 h, s32 x, s32 y) {
    {
        Gfx *g0 = gdl++;
        g0->w0 = 0xE4000000 | ((((x + w) * 4) & 0xFFF) << 12) | (((y + h) * 4) & 0xFFF);
        g0->w1 = (((x * 4) & 0xFFF) << 12) | ((y * 4) & 0xFFF);
        {
            Gfx *g1 = gdl++;
            g1->w0 = 0xE1000000;
            g1->w1 = (s << 21) | ((t << 5) & 0xFFFF);
        }
        {
            Gfx *g2 = gdl++;
            g2->w0 = 0xF1000000;
            g2->w1 = 0x04000400;
        }
    }
    {
        Gfx *g3 = gdl++;
        g3->w0 = 0xE6000000;
        g3->w1 = 0;
    }
    return gdl;
}
