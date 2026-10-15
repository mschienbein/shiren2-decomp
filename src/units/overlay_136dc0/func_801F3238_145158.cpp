#include "func_801F3238_145158/types.h"

extern "C" void func_801F3238_145158(void) {
    s32 i;

    for (i = 30;;) {
        OvlBUnitHead *unit;

        i--;
        if (i == -1) {
            break;
        }
        unit = func_800A8CB0((u8)i);
        if (func_800A8974(unit) == 0) {
            unit->unk52 = 1;
        }
    }
}
