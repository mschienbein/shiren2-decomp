#include "common.h"

typedef unsigned char u8;

s32 func_8010CB2C(u8 *obj, s32 mode) {
    if (mode == 30) {
        return 1;
    }
    if (mode == 0 && !(obj[2] & 4)) {
        return 1;
    }
    if (mode == 1 && (obj[2] & 4)) {
        return 1;
    }
    return 0;
}
