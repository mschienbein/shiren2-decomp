#include "common.h"

extern s32 func_8010CB2C(unsigned char *obj, s32 mode);

s32 func_8010E82C(unsigned char *obj, s32 mode) {
    if (mode == 0x1D) {
        return 1;
    }
    if (mode == 0xB) {
        return 1;
    }
    return func_8010CB2C(obj, mode);
}
