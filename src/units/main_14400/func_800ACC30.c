#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
extern u8 D_8015374C[];
extern u16 *D_801538B4[];
u8 func_800AC1AC(u8);
char *func_80048480(u16);
char *func_800ACC30(u8 arg0) {
    s32 id = arg0 & 0xFF;
    u8 group = func_800AC1AC(id);
    return func_80048480(D_801538B4[group][(u8)(id - D_8015374C[group])]);
}
