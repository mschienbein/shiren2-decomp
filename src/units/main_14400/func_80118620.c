#include "common.h"

typedef struct Item Item;

typedef struct {
    unsigned char pad0[0x1E];
    unsigned char flags_1E;
} Unit;

extern short D_801569E2;

s32 func_800A99D0(void);
void func_800498E4(s32 id, ...);
s32 func_800E1CC4(Unit *obj, s32 kind);
void func_800EB488(Unit *obj, short amount);

/* Item vtable slot +0x44: `self` is the adjusted receiver the dispatcher supplies; not used here. */
void func_80118620(Item *self, Unit *unit)
{
    s32 scale;

    if (func_800A99D0()) {
        func_800498E4(0x222);
        return;
    }
    if ((unit->flags_1E >> 2) & 1) {
        if (func_800E1CC4(unit, 3)) {
            scale = 2;
        } else {
            scale = 1;
        }
        func_800EB488(unit, D_801569E2 * scale);
    }
}
