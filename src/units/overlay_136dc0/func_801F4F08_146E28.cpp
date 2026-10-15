#include "func_801F4F08_146E28/types.h"

extern "C" s32 func_801F4F08_146E28(OvlBUnitHead *unit) {
    if (func_800A99D0()) {
        if ((unit->unk1E >> 2) & 1) {
            return 1;
        }
        if (unit->unk0A == 0x5F) {
            return 1;
        }
        if ((unit->unk1E >> 6) & 1) {
            Rec801F2AC0 *rec = (UnitX *)unit;
            if (rec->unk08 != 0) {
                return 0;
            }
        }
        if ((unit->unk1E >> 3) & 1) {
            Rec801F2AC0 *rec = (UnitY *)unit;
            if (rec->unk08 != 0) {
                return 1;
            }
        }
    } else if ((unit->unk1E >> 6) & 1) {
        Rec801F2AC0 *rec = (UnitX *)unit;
        return rec->unk08 == 0;
    }
    return 0;
}
