#include "common.h"

/* The original declaration is unresolved; this view preserves all 32 bits. */
extern u32 D_80169A44;

s32 func_800610A8(void) {
    return D_80169A44 != 2;
}
