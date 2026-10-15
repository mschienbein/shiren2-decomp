#include "func_801F4B74_146A94/types.h"

static inline bool is_kind_x(const OvlBUnitHead *unit) { return (unit->unk1E >> 6) & 1; }
static inline bool is_active(const Rec801F2AC0 *rec) { return rec->unk10 == 1; }

extern "C" void func_801F4B74_146A94(void) {
    s32 i = 30;
    OvlBPos pos;
    OvlBPos center;

    /* Proven 0/1 s32 producer; canonical 800BF944 uses the same XOR form. */
    if ((func_800A99D0() ^ 1) != 0) {
        return;
    }
    if (D_801F5D0E_147C2E != 0) {
        return;
    }
    center = D_801476B8->pos;
    for (;;) {
        OvlBUnitHead *unit;

        i--;
        if (i == 0) {
            break;
        }
        unit = func_800A8CB0((u8)i);
        if (func_800A8974(unit)) {
            continue;
        }
        if (!is_kind_x(unit)) {
            continue;
        }
        if (!is_active((UnitX *)unit)) {
            continue;
        }
        pos = unit->pos;
        if (!(center.y - 5 < pos.y && pos.y < center.y + 5 && center.x - 5 < pos.x && pos.x < center.x + 5)) {
            if (((Rec801F2AC0 *)(UnitX *)unit)->home() == unit->facing) {
                continue;
            }
            OvlBDir home = ((Rec801F2AC0 *)(UnitX *)unit)->home();
            func_800A665C(unit, &home);
        } else {
            OvlBDir toward(func_801F49BC_1468DC(unit, D_801476B8));
            func_800A665C(unit, &toward);
        }
    }
    func_80049CB4(2);
}
