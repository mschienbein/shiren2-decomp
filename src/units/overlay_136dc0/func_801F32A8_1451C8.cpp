#include "func_801F32A8_1451C8/types.h"

extern "C" void func_801F1F64_143E84(OvlBUnitHead *unit);

extern "C" void func_801F32A8_1451C8(void) {
    s32 i;

    for (i = 30;;) {
        OvlBUnitHead *unit;

        i--;
        if (i == -1) {
            break;
        }
        unit = func_800A8CB0((u8)i);
        if (func_800A8974(unit) != 0) {
            continue;
        }
        unit->unk52 = 0;
        func_801F1F64_143E84(unit);
    }
}
