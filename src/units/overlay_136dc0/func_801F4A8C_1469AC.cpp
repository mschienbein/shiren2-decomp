#include "func_801F4A8C_1469AC/types.h"

extern "C" void func_801F4A8C_1469AC(void) {
    u8 facing = D_801476B8->facing.v;
    s32 i;

    for (i = 30;;) {
        OvlBUnitHead *unit;

        i--;
        if (i == 0) {
            break;
        }
        unit = func_800A8CB0((u8)i);
        if (func_800A8974(unit)) {
            continue;
        }
        if (unit->unk0A == 0x5F) {
            OvlBDir dir(facing);

            func_800A665C(unit, &dir);
        }
        if (!((unit->unk1E >> 3) & 1)) {
            continue;
        }
        Rec801F2AC0 *rec = (UnitY *)unit;
        if (rec->unk04 == 0) {
            continue;
        }
        OvlBDir dir(facing);
        func_800A665C(unit, &dir);
    }
}
