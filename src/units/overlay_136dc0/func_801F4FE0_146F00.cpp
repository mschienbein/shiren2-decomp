#include "func_801F4FE0_146F00/types.h"

extern "C" void func_800E9F14(OvlBUnitHead *unit, s32 arg1);

extern "C" void func_801F4FE0_146F00(s32 arg0) {
    u8 cell;

    func_800E9F14(D_801476B8, arg0);
    cell = (u8)func_801E8D0C_13AC2C(0x1A, 0);
    if (cell != 0) {
        OvlBUnitHead *unit = func_800A8CB0(cell);

        if (!func_800A8974(unit)) {
            func_800E9F14(unit, arg0);
        }
    }
}
