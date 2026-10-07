#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct { u16 base; u8 bonus; } Info800D7D84;
extern u8 D_801480D0[];
Info800D7D84 *func_80044ECC(u8 kind, u8 level);
s32 func_800D7D84(u8 kind, u8 level) {
    Info800D7D84 *info;
    s32 value;
    s32 i;

    info = func_80044ECC(kind, level);
    if (info != 0) {
        value = info->base + level - 1;
        if (kind == 0x29) {
            for (i = 1; i < 9; i++) {
                if (level < D_801480D0[i]) {
                    break;
                }
            }
            value -= level - i;
        }
        if (kind > 0x29) {
            value += 9 - func_80044ECC(0x29, 1)->bonus;
        }
        if (kind > 0x50) {
            value -= func_80044ECC(0x50, 1)->bonus;
        }
        if (kind > 0x51) {
            value -= func_80044ECC(0x51, 1)->bonus;
        }
        if (kind > 0x55) {
            value -= func_80044ECC(0x55, 1)->bonus;
        }
        if (kind > 0x56) {
            value -= func_80044ECC(0x56, 1)->bonus;
        }
        return value;
    }
    return -1;
}
