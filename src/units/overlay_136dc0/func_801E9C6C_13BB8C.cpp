#include "func_801E9C6C_13BB8C/types.h"

extern "C" s32 func_801E9C6C_13BB8C(void) {
    s32 i;
    u8 pick;
    i = 100;
    while (i--) {
        pick = func_800C57CC(D_80147620, 4);
        if (func_801E99C4_13B8E4(pick, 0)) {
            return pick;
        }
    }
    for (i = 0; i < 5; i++) {
        if (func_801E99C4_13B8E4(i, 0)) {
            return i;
        }
    }
    return -1;
}
