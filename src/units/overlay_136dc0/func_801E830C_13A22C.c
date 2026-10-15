#include "func_801E830C_13A22C/types.h"

extern Obj801F71F0 D_801F71F0_149110;
extern s32 D_801DE9AC;
extern s32 D_801E4E78;
extern s32 D_801DE9B0;
extern s32 D_801DEAAC;

void func_801E830C_13A22C(u16 *src, u8 flag) {
    Obj801F71F0 *o = &D_801F71F0_149110;
    s32 i;

    for (i = 3; i != -1; i--) {
        o->x0[i] = src[i];
    }
    o->x8 = (u8)D_801DE9AC;
    o->x9 = (u8)D_801E4E78;
    o->xA = (u8)D_801DE9B0;
    o->xB = (u8)D_801DEAAC;
    if (flag == 0) {
        o->xD = 1;
        o->xC = 1;
    } else {
        o->xC = 0;
        o->xD = flag;
    }
}
