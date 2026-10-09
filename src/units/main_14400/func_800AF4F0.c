#include "common.h"

extern unsigned char D_8015374C[];

s32 func_800AF4F0(unsigned char index, unsigned char value) {
    return value - D_8015374C[index];
}
