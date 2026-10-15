#include "func_801F356C_14548C/types.h"

extern "C" s32 func_801E6840_138760(s32 slot);
extern "C" void func_801E6708_138628(s32 slot, s32 arg1, s32 arg2, s32 arg3);
extern "C" void func_801E6860_138780(void);

extern "C" void func_801F356C_14548C(void) {
    s32 i;

    for (i = 30;;) {
        OvlBUnitHead *unit;

        i--;
        if (i == -1) {
            break;
        }
        if ((u8)func_801E6840_138760(i) != 1) {
            continue;
        }
        unit = func_800A8CB0((u8)i);
        if (func_800A8974(unit) != 0) {
            continue;
        }
        func_801E6708_138628(i, 0, unit->facing.v, unit->unk0A);
    }
    func_801E6860_138780();
}
