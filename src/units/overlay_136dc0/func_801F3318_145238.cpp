#include "func_801F3318_145238/types.h"

extern "C" void func_801E6A8C_1389AC(u8 slot);

/* Preserve the real UnitY-to-record base conversion at the call boundary;
 * the state remains a word, not a bool-valued external contract. */
static inline s32 engagement_state(const Rec801F2AC0 *rec) {
    return rec->unk04;
}

extern "C" void func_801F3318_145238(s32 flag)
{
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
        s32 found = 0;
        /* ROM 801F3378 branches to found=1 for kind 0x5F. Other kinds
         * require the +0x84 record selected by flag bit 3 and state 1. */
        if (unit->unk0A == 0x5F ||
            (((unit->unk1E >> 3) & 1) && engagement_state((UnitY *)unit) == 1)) {
            found = 1;
        }
        if (!found) {
            continue;
        }
        if (flag) {
            unit->unk52 = 1;
        } else {
            unit->unk52 = 0;
            func_801E6A8C_1389AC(i);
        }
    }
}
