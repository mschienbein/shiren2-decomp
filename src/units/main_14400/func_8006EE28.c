#include "common.h"

extern s32 D_8013D454;
extern s32 D_8013D450;
extern void func_80071C48(void);

void func_8006EE28(void)
{
    if (D_8013D454 != 0) {
        func_80071C48();
        D_8013D450 ^= 1;
    }
}
