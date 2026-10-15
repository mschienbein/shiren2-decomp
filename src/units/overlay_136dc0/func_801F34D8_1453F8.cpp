#include "func_801F34D8_1453F8/types.h"

extern "C" void func_801F34D8_1453F8(OvlBUnitHead *unit, s32 unitOnly) {
    s32 i;

    for (i = 29; i != -1; i--) {
        if (D_801F51E8_147108->pairs.units[i] == unit && unitOnly) {
            D_801F51E8_147108->pairs.units[i] = NULL;
            D_801F51E8_147108->pairs.targets[i] = NULL;
        }
        if (!unitOnly && (D_801F51E8_147108->pairs.units[i] == unit || D_801F51E8_147108->pairs.targets[i] == unit)) {
            D_801F51E8_147108->pairs.units[i] = NULL;
            D_801F51E8_147108->pairs.targets[i] = NULL;
        }
    }
}
