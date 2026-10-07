#include "common.h"

typedef unsigned char u8;

s32 func_800DA1AC(u8 *src, u8 *dst) {
    dst[0] = src[1];
    dst[1] = src[9];
    return 2;
}
