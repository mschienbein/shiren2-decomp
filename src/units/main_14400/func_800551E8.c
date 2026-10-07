#include "common.h"
typedef struct { u32 w0; u32 w1; } Gfx;
extern Gfx D_01000120[];
extern float D_8013B7A8;
extern float D_8013B7AC;
extern float D_8013B7B0;
extern float D_8013B7B4;
#define SHIFTL(v, s, w) ((((u32)(v)) & ((1 << (w)) - 1)) << (s))
Gfx *func_800551E8(Gfx *gfx) {
    {
        Gfx *g = gfx++;
        g->w0 = 0xDE000000;
        g->w1 = (u32)D_01000120;
    }
    {
        Gfx *g = gfx++;
        g->w0 = SHIFTL(0xF6, 24, 8) | SHIFTL(D_8013B7B0 - 1.0f, 14, 10) | SHIFTL(D_8013B7B4 - 1.0f, 2, 10);
        g->w1 = SHIFTL(D_8013B7A8, 14, 10) | SHIFTL(D_8013B7AC, 2, 10);
    }
    {
        Gfx *g = gfx++;
        g->w0 = 0xE7000000;
        g->w1 = 0;
    }
    {
        Gfx *g = gfx++;
        g->w0 = 0xE3000A01;
        g->w1 = 0;
    }
    return gfx;
}
