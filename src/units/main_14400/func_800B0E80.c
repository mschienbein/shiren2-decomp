#include "common.h"
extern u32 D_80143110;
extern unsigned char D_80143114[];
/* The limit test is unsigned (sltu at 0x800B0E88): a negative index is rejected too. */
s32 func_800B0E80(s32 index) {
    if ((u32)index > D_80143110) return 0;
    return D_80143114[index];
}
