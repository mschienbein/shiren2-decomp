#include "common.h"

typedef unsigned char u8;

s32 func_800CBCF8(u8 arg0) {
    if (arg0 < 10) {
        return arg0 * 144;
    }
    return (arg0 - 10) * 20 + 1440;
}
