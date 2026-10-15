#include "func_801F4D10_146C30/types.h"

extern "C" void func_801F4D10_146C30(void) {
    s32 i = 30;

    /* The s32 predicates below return only 0/1; see types.h producer proof. */
    if ((func_800A99D0() ^ 1) != 0) {
        return;
    }
    if (D_801F5D0E_147C2E != 0) {
        return;
    }
    for (;;) {
        OvlBUnitHead *unit;
        OvlBDir home;

        i--;
        if (i == 0) {
            break;
        }
        unit = func_800A8CB0((u8)i);
        if (func_800A8974(unit)) {
            continue;
        }
        if ((unit->unk1E >> 6) & 1) {
            if ((func_801F2BE8_144B08((UnitX *)unit, 0) ^ 1) != 0) {
                continue;
            }
            home = ((Rec801F2AC0 *)(UnitX *)unit)->home();
        } else {
            if (!((unit->unk1E >> 3) & 1)) {
                continue;
            }
            if ((func_801F2BE8_144B08((UnitY *)unit, 0) ^ 1) != 0) {
                continue;
            }
            home = ((Rec801F2AC0 *)(UnitY *)unit)->home();
        }
        if (home == unit->facing) {
            continue;
        }
        func_800A665C(unit, &home);
    }
    func_80049CB4(2);
}
