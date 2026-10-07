#include "common.h"

typedef unsigned char u8;
extern u8 D_8016AB05[][0x6C];
u8 func_800649C0(s32 row, s32 col) {
    return D_8016AB05[row][col * 2];
}
