#include "func_801E7268_139188/types.h"

extern Unit *func_8007946C(s32 side, s32 slot);
extern s32 func_80062554(s32 x, s32 y);

void func_801E7268_139188(s32 slot, s32 x, s32 z) {
    Unit *unit = func_8007946C(0, slot);

    unit->xE = func_80062554(x, z) << 2;
}
