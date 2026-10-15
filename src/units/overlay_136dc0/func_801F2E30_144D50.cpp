#include "func_801F2E30_144D50/types.h"

extern "C" s32 func_801F2E80_144DA0(s32 arg0);

extern "C" s32 func_801F2E30_144D50(s32 arg0) {
    if (arg0 == 0) {
        return 1;
    }
    if ((u8)func_801E8D0C_13AC2C((u16)arg0, 0) != 0) {
        return !func_801F2E80_144DA0(arg0);
    }
    return 0;
}
