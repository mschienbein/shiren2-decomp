#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct Unit {
    u8 pad0[0x58];
    struct Unit *target58;
    u8 pad5C[0x3E];
    u16 flags9A;
} Unit;

s32 func_800A692C(Unit *unit, s32 kind);
s32 func_800F3310(void *obj);
u8 func_800A6420(Unit *obj, Unit *target);
s32 func_800A67DC(Unit *u, Unit *o, s32 force, s32 apply);
s32 func_800F1024(Unit *obj);
void func_800F06E4(Unit *obj);
s32 func_800E1CD4(Unit *obj, s32 kind);
s32 func_800E7104(Unit *unit);
s32 func_800E8350(Unit *unit);

/* Slot-20 override of vtable D_8015B2A8. */
s32 func_801007E0(Unit *unit) {
    Unit *target;
    s32 apply;

    if (func_800A692C(unit, 0x12)) {
        return func_800F3310(unit);
    }
    target = unit->target58;
    switch (func_800A6420(unit, target)) {
    case 1:
    case 2:
        apply = unit->flags9A & 0x40;
        apply = apply != 0;
        if (func_800A67DC(unit, target, 1, apply) && func_800F1024(unit)) {
            func_800F06E4(unit);
            return 0;
        }
        break;
    }
    if (func_800E1CD4(unit, 0x10)) {
        return func_800E8350(unit);
    }
    return func_800E7104(unit);
}
