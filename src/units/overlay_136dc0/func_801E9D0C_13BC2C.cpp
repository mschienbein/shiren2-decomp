#include "func_801E9D0C_13BC2C/types.h"

extern "C" s32 func_801E9D0C_13BC2C(void) {
    u8 value = D_801F5228_147148[2];
    if (value >= 0xFD) {
        return 1;
    }
    return value >= 12 && value < 166;
}
