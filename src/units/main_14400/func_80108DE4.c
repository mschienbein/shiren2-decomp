#include "common.h"

typedef unsigned char u8;
typedef struct {
    u8 pad0[0x58];
    void *target_58;
    u8 pad5C[0x8A - 0x5C];
    u8 chance_8A;
} Unit;

extern u8 D_80147620[];
s32 func_800F1024(Unit *unit);
s32 func_800A67DC(Unit *unit, void *target, s32 force, s32 apply);
s32 func_800C587C(void *rng, u8 limit);
void func_800F06E4(Unit *unit);
s32 func_800E7104(Unit *unit);

s32 func_80108DE4(Unit *unit)
{
    if (unit->target_58 != 0) {
        s32 change = 0;
        if (func_800F1024(unit) && func_800A67DC(unit, unit->target_58, 0, 0)) {
            change = 1;
        } else if (func_800C587C(D_80147620, unit->chance_8A)) {
            change = 1;
        }
        if (change) {
            func_800F06E4(unit);
            return 0;
        }
    }
    return func_800E7104(unit);
}
