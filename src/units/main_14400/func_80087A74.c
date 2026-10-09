#include "common.h"

extern unsigned char D_801C3395[];

void func_80087A74(void) {
    s32 i;

    for (i = 0; i < 30; i++) {
        D_801C3395[i] = 0;
    }
}
