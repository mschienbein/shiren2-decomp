#include "common.h"

/* One RDP display-list command (two 32-bit words). */
typedef struct {
    u32 w0;
    u32 w1;
} Gfx;

/* Segment-1 display list referenced by address only. */
extern unsigned char D_010000D0[];

/* Fills the 320x240 screen with the given primitive colour unless the
 * alpha is zero. */
Gfx *func_8005C790(Gfx *gfx, unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    if (a == 0) {
        return gfx;
    }
    {
        Gfx *_g = gfx++;

        _g->w0 = 0xDE000000;
        _g->w1 = (u32)D_010000D0;
    }
    {
        Gfx *_g = gfx++;

        _g->w0 = 0xFA000000;
        _g->w1 = (r << 24) | ((g & 0xFF) << 16) | ((b & 0xFF) << 8) | (a & 0xFF);
    }
    {
        Gfx *_g = gfx++;

        _g->w0 = 0xF64FC3BC;
        _g->w1 = 0;
    }
    return gfx;
}
