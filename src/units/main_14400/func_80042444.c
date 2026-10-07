#include "common.h"
typedef unsigned char u8;
unsigned char func_800B52D0(s32 *pair);
u8 func_80042444(s32 a, s32 b) {
    s32 pair[2];
    pair[1] = a;
    pair[0] = b;
    return (u8)func_800B52D0(pair);
}
