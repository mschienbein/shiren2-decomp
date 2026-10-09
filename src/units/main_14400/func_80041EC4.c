#include "common.h"
s32 func_800610A8(void);
extern s32 D_801F5CD8;
/* The original returns the complete word without narrowing to the prior u8 declaration. */
s32 func_80041EC4(void) {
    if (func_800610A8()) return D_801F5CD8;
    return 0;
}
