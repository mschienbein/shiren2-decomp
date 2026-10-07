#include "common.h"

typedef struct { u32 w0; u32 w1; } Gfx;
typedef struct { s32 m[16]; } Matrix;
typedef struct { unsigned char pad[0x100]; unsigned short f100; } Obj;
extern Matrix *D_80165724;
extern Obj *D_80165720;
Gfx *func_8005BB94(Gfx *gdl) {
    Gfx *g;
    g = gdl++;
    g->w0 = 0xDA380007;
    g->w1 = (u32)D_80165724;
    gdl->w0 = 0xDB0E0000;
    gdl->w1 = D_80165720->f100;
    return gdl + 1;
}
