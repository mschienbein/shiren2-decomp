#include "common.h"

typedef unsigned short u16;
extern u16 D_801F5D0E;
extern s32 func_800610A8(void);

s32 func_80046240(void) {
    if (func_800610A8() == 0) {
        return 0;
    }
    return D_801F5D0E != 0;
}
