#include "common.h"
typedef unsigned char u8;
s32 func_800DF398(const u8 *object, u8 *packed) {
    packed[0] = object[1];
    packed[1] = (object[9] << 4) | (object[8] & 7);
    return 2;
}
