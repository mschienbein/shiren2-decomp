#include "func_801EA124_13C044/types.h"

extern "C" s32 func_801EA124_13C044(s32 id, s32 amount) {
    OvlBInventoryUnit *unit = func_800A8CB0((u8)func_801E8D0C_13AC2C((u16)id, 0));
    if (func_800A8974(unit)) {
        return 0;
    }
    if ((unit->flags_1E >> 2) & 1) {
        func_800EB744(unit, amount);
        func_800485DC(D_801476B8);
    }
    return 0;
}
