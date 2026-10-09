#include "common.h"

typedef unsigned char u8;

extern u8 D_801F5CCE;
s32 func_800610A8(void);

u8 func_80042060(void) {
    if (func_800610A8() != 0) {
        return D_801F5CCE;
    }
    return 0;
}
