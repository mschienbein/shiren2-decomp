#include "func_801F5054_146F74/types.h"

/* EBCA8 passes three words; the id narrows only at lookup (801F5068). */
extern "C" s32 func_801F5054_146F74(s32 id, s32 rows, s32 columns) {
    OvlBUnitHead *unit = func_800A8CB0((u8)func_801E8D0C_13AC2C((u16)id, 0));

    if (func_800A8974(unit)) {
        return 0;
    }
    OvlBPos pos = unit->pos;
    s32 top = (u8)rows;
    s32 bottom = (u8)(rows >> 8);
    s32 left = (u8)columns;
    s32 right = (u8)(columns >> 8);

    if (pos.y < top || bottom < pos.y || pos.x < left || right < pos.x) {
        return 0;
    }
    return 1;
}
