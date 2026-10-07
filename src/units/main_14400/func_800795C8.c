#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct {
    u8 pad0[2];
    s16 x2;
    u8 pad4[0x3A];
    u8 x3E;
    u8 x3F;
    u8 x40;
    u8 pad41[9];
    s16 x4A;
    u8 pad4C[0xB0 - 0x4C];
} Unit;
extern Unit D_801DEAB4[];
extern Unit D_801DD378[];
s32 func_80059830(s32 id);
void func_800795C8(void) {
    s32 k;
    for (k = 0; k < 2; k++) {
        Unit *list;
        Unit *u;
        s32 count;
        s32 i;
        if (k == 0) {
            list = D_801DEAB4;
            count = 30;
        } else {
            list = D_801DD378;
            count = 32;
        }
        for (i = 0; i < count; i++) {
            u = &list[i];
            if (u->x2 == -1) {
                continue;
            }
            if (u->x40) {
                u->x3F = u->x3E + func_80059830(u->x4A) * u->x40;
            } else {
                u->x3F = u->x3E;
            }
        }
    }
}
