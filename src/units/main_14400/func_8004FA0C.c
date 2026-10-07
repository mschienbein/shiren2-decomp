#include "common.h"

typedef unsigned char u8;

extern s32 D_8013968C;
void func_80050F34(s32 id, void *pos, s32 arg2, s32 arg3);

void func_8004FA0C(void *pos, u8 *a1) {
    if (D_8013968C == 0xD6) {
        func_80050F34(0xA2, pos, *a1, 0);
    }
}
