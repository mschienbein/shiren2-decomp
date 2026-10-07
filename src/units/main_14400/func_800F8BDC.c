#include "common.h"

typedef unsigned char u8;
extern s32 func_800F426C(void *, s32, s32, u8, s32);

s32 func_800F8BDC(void *a0, s32 a1, s32 a2, u8 a3, s32 a4) {
    if (a1 == 0 && a2 == 1) {
        return 0;
    }
    return func_800F426C(a0, a1, a2, a3, a4);
}
