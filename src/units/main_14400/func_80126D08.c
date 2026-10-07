#include "common.h"

/* The +0x1C kind-query slot supplies a receiver pointer, unused by this override. */
s32 func_80126D08(void *arg0, s32 kind) {
    s32 result = 0;

    if (kind == 0x18) {
        result = 1;
    } else if (kind == 0xB) {
        result = 1;
    }
    return result;
}
