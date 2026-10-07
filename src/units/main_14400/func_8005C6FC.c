#include "common.h"

typedef unsigned char u8;

typedef struct {
    u32 w0;
    u32 w1;
} Gfx;

extern u32 D_80165940;
extern u32 D_80165944;
extern u32 D_80165948;
extern u32 D_8016594C;
extern char D_010000D0[];

Gfx *func_8005C6FC(Gfx *gfx) {
    u32 alpha = D_8016594C;

    if (alpha == 0) {
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
        /* The low byte is byte 3 within each big-endian color word. */
        _g->w1 = ((u32)((u8 *)&D_80165940)[3] << 24)
               | ((u32)((u8 *)&D_80165944)[3] << 16)
               | ((u32)((u8 *)&D_80165948)[3] << 8)
               | (alpha & 0xFF);
    }
    {
        Gfx *_g = gfx++;

        _g->w0 = 0xF64FC3BC;
        _g->w1 = 0;
    }
    return gfx;
}
