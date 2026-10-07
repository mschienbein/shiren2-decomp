#include "common.h"
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
typedef struct { u8 pad[0x958]; u16 flags; } S;
extern u8 D_80147620[];
extern u8 D_80156ABD;
extern s32 D_80143444;
extern u8 D_80143448;
s32 func_800C587C(void *, u8);
s32 func_800BC78C(S *, u8);
s32 func_800BC964(S *s) {
    u8 count = 0;
    s32 n;
    if (!(s->flags & 2)) return 0;
    n = func_800C587C(D_80147620, D_80156ABD) ? 2 : 1;
    for (n--; n != -1; n--) {
        if (func_800BC78C(s, count)) count++;
    }
    if (count) D_80143444 = 1;
    D_80143448 = count;
    return 1;
}
