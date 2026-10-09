#include "common.h"

/* Adjacent setter/getter of the initialized limiter word. */
s32 D_8013E91C = 8;

void func_80083FEC(s32 value) {
    D_8013E91C = value;
}

s32 func_80083FF8(void) {
    return D_8013E91C;
}
