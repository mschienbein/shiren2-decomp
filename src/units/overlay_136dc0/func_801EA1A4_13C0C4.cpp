#include "func_801EA1A4_13C0C4/types.h"

extern "C" s32 func_801EA1A4_13C0C4(u16 id) {
    OvlBInventoryUnit *unit = func_800A8CB0((u8)func_801E8D0C_13AC2C(id, 0));
    s32 value;
    if (func_800A8974(unit)) {
        return 0;
    }
    value = 0;
    if ((unit->flags_1E >> 2) & 1) {
        value = unit->unk_84;
    }
    return value;
}
