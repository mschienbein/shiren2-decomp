#include "common.h"

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;

extern u8 D_80142F28[2];

s32 func_800A9BC0(u8 id, u8 *valid) {
    if ((u8)(id - 0x18) < 5) {
        s32 n = id - 0x18;
        s32 bit0 = 1;
        *valid = bit0;
        return !(D_80142F28[0] & (bit0 << n)) && (D_80142F28[1] & (bit0 << n));
    }
    return 0;
}