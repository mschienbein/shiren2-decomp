#include "common.h"

extern s32 D_8013FEE0;
extern s32 D_801D2BFC;

s32 func_8008C684(void)
{
    if (D_8013FEE0 == 0) {
        return 0;
    }
    return D_801D2BFC;
}
