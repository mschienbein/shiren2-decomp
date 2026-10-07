#include "common.h"

extern s32 D_801D9358[][54];

void func_80062684(s32 row, s32 col) {
    s32 *flags = &D_801D9358[row][col];

    *flags = (*flags & ~0x4000) | 0x1000;
}
