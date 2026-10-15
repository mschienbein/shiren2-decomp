#include "func_801F490C_14682C/types.h"

/* Host 8004208C forwards a full word; only the unit lookup narrows it. */
extern "C" s32 func_801F490C_14682C(s32 cell, s32 steps, s32 previous) {
    OvlBUnitHead *unit = func_800A8CB0((u8)cell);
    OvlBDir dir;
    s32 now;

    if (func_800A8974(unit)) {
        return -1;
    }
    dir = unit->facing;
    dir = dir.turned(steps);
    unit->facing = dir;
    now = dir.v;
    /* turned() already masks v to 0..7, so this modulo is redundant, but the
     * ROM performs it: lbu 0x801F4968 reloads the Dir byte, then the signed %8
     * sequence 0x801F496C..0x801F497C; removing it changes 25 words.
     * previous is a full signed input, so its modulo below is not redundant. */
    now %= 8;
    previous %= 8;
    if (now == previous) {
        return -1;
    }
    return now;
}
