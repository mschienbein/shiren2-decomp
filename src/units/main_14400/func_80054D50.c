#include "common.h"

typedef struct { u32 w0, w1; } Gfx;
extern void *func_8006A8D8(char *name, u32 size);
extern char D_8014C0C8[];
extern s32 D_801630B0;
extern s32 D_801630B4;
extern Gfx *(*D_801630B8)(Gfx *);
extern void *D_801630BC;

s32 func_80054D50(void)
{
    s32 ret = 0;

    D_801630B0 = 0;
    D_801630B4 = 0;
    D_801630B8 = 0;
    D_801630BC = func_8006A8D8(D_8014C0C8, 0x25800);
    if (D_801630BC == 0) {
        ret = -1;
    }
    return ret;
}
