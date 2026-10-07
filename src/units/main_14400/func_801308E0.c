#include "common.h"

typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;

typedef struct { u32 w0; u32 w1; } Gfx;
typedef struct { u8 pad[0x48]; s32 *unk48; } S;
extern S *D_80148D84;
Gfx *func_80130980(s32 pos, Gfx *cursor);
Gfx *func_801308E0(s32 pos, Gfx *cursor) {
    Gfx *gfx = func_80130980(pos, cursor);
    {
        Gfx *g = gfx++;
        g->w0 = 0x0D000000;
    }
    {
        Gfx *g = gfx++;
        g->w0 = 0x062E0000;
        g->w1 = (u32)D_80148D84->unk48;
    }
    return gfx;
}
