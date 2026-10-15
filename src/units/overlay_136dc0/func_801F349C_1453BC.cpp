#include "func_801F349C_1453BC/types.h"

extern "C" void func_801F349C_1453BC(OvlBUnitHead *unit, OvlBUnitHead *target) {
    s32 i;

    for (i = 29; i != -1; i--) {
        if (D_801F51E8_147108->pairs.units[i] == NULL) {
            D_801F51E8_147108->pairs.units[i] = unit;
            D_801F51E8_147108->pairs.targets[i] = target;
            return;
        }
    }
}
