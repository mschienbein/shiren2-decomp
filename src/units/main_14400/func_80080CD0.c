#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct { u32 w0; u32 w1; } Gfx;
typedef struct { u8 pad0[6]; u16 col; u16 row; u8 padA[0x22]; } Entry80080CD0;
extern Entry80080CD0 D_801A9080[];
extern s16 D_801A9F60;
extern s16 D_801A9F62;
s32 func_8008269C(s32 arg0);
s32 func_80083718(void);
void func_80080CD0(Gfx **gdl) {
    s32 index;
    s32 x;
    s32 y;

    index = func_8008269C(1);
    if (func_80083718() == 0 || index < 0) {
        return;
    }
    x = D_801A9080[index].col * 8 + D_801A9F60;
    y = D_801A9080[index].row * 8 + D_801A9F62;
    {
        Gfx *g = (*gdl)++;
        s32 left = x - 8;
        s32 top = y + 3;
        g->w0 = 0xE4000000 | (((x * 4) & 0xFFF) << 12) | (((y + 11) * 4) & 0xFFF);
        g->w1 = (((left * 4) & 0xFFF) << 12) | (((top * 4) & 0xFFF));
    }
    {
        Gfx *g = (*gdl)++;
        g->w0 = 0xE1000000;
        g->w1 = 0x01000400;
    }
    {
        Gfx *g = (*gdl)++;
        g->w0 = 0xF1000000;
        g->w1 = 0x04000400;
    }
}
