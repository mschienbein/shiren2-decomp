#include "func_801F48A4_1467C4/types.h"

extern "C" s32 func_801F48A4_1467C4(void) {
    OvlBUnitHead *unit = unit_of(D_801F51E8_147108->args[0].word);

    if (func_800A8974(unit)) {
        return 1;
    }
    if (unit->unk52 == 2) {
        return 0;
    }
    return unit->unk52 != 3;
}
