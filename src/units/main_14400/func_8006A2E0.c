#include "common.h"

typedef struct { u32 w0; u32 w1; } Gfx;
extern unsigned char D_8013CA00; /* fill color r */
extern unsigned char D_8013CA01; /* fill color g */
extern unsigned char D_8013CA02; /* fill color b */
extern float D_8013B7A8; /* rect ulx */
extern float D_8013B7AC; /* rect uly */
extern float D_8013B7B0; /* rect width */
extern float D_8013B7B4; /* rect height */

#define SHIFTL(v, s, w) ((u32)(((u32)(v) & ((1 << (w)) - 1)) << (s)))
#define RGBA5551(r, g, b, a) ((((r) << 8) & 0xF800) | (((g) << 3) & 0x7C0) | (((b) >> 2) & 0x3E) | ((a) & 1))

Gfx *func_8006A2E0(Gfx *gdl) {
    {
        Gfx *g = gdl++;
        g->w0 = 0xE7000000; /* G_RDPPIPESYNC */
        g->w1 = 0;
    }
    {
        Gfx *g = gdl++;
        g->w0 = 0xE3000A01; /* G_SETOTHERMODE_H: cycle type */
        g->w1 = 0x300000;   /* G_CYC_FILL */
    }
    {
        Gfx *g = gdl++;
        u32 color = RGBA5551(D_8013CA00, D_8013CA01, D_8013CA02, 1);
        g->w0 = 0xF7000000; /* G_SETFILLCOLOR */
        g->w1 = (color << 16) | color;
    }
    {
        Gfx *g = gdl++;
        g->w0 = SHIFTL(0xF6, 24, 8) | SHIFTL(D_8013B7B0 - 1.0f, 14, 10) | SHIFTL(D_8013B7B4 - 1.0f, 2, 10);
        g->w1 = SHIFTL(D_8013B7A8, 14, 10) | SHIFTL(D_8013B7AC, 2, 10);
    }
    return gdl;
}
