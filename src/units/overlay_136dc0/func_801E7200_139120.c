#include "func_801E7200_139120/types.h"

extern Unit *func_8007946C(s32 side, s32 slot);
extern s32 func_80062554(s32 x, s32 y);

void func_801E7200_139120(s32 slot, s32 arg1, s32 arg2, s32 x, s32 z) {
    Unit *unit = func_8007946C(0, slot);

    unit->xC = (x << 7) + 0x40;
    unit->x10 = (z << 7) + 0x40;
    unit->xE = func_80062554(x, z) << 2;
}
