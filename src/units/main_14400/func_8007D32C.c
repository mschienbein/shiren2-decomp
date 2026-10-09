#include "common.h"

typedef unsigned char u8;

/* Selected palette/colour index used by func_8007C590 (D_8013DEA4[D_8013DEA0]). */
s32 D_8013DEA0 = 2;

s32 func_8004246C(void);
s32 func_80041E50(void);

void func_8007D32C(void) {
    D_8013DEA0 = func_8004246C();
    if ((u8)func_80041E50() == 1) {
        D_8013DEA0 = 0;
    }
    if (D_8013DEA0 > 20) {
        D_8013DEA0 = 2;
    }
}
