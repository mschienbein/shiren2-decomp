#include "func_801F4E2C_146D4C/types.h"

extern "C" s32 func_801F4E2C_146D4C(OvlBUnitHead *unit) {
    if ((D_80142F18.flags >> 2) & 1) {
        if ((unit->unk1E >> 2) & 1) {
            return 1;
        }
        if (unit->unk0A == 0x5F) {
            return 1;
        }
        if ((unit->unk1E >> 6) & 1) {
            Rec801F2AC0 *rec = (UnitX *)unit;
            return rec->unk08 != 0;
        }
        if ((unit->unk1E >> 3) & 1) {
            Rec801F2AC0 *rec = (UnitY *)unit;
            return rec->unk08 != 0;
        }
        s32 ranged = 0;
        if (unit->unk0A >= 29) {
            ranged = unit->unk0A < 87;
        }
        if (ranged) {
            return 1;
        }
        if (D_801F5D0E_147C2E != 0) {
            return 0;
        }
        return selection_mode(&D_80142F18) == 0x4F;
    }
    if ((unit->unk1E >> 6) & 1) {
        Rec801F2AC0 *rec = (UnitX *)unit;
        return rec->unk08 != 0;
    }
    return 1;
}
