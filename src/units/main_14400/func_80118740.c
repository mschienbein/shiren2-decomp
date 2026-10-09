#include "common.h"

extern s32 func_800E1CC4(void *obj, s32 kind);
extern short func_800E0D28(void *o, short amt, short heal);
extern s32 func_80049CB4(s32 id, ...);

/* Item vtable slot +0x44: `self` is the adjusted receiver the dispatcher supplies; not used here. */
void func_80118740(void *self, void *target) {
    short amount = func_800E1CC4(target, 3) ? 2 : 1;

    if (func_800E0D28(target, amount, amount) > 0) {
        func_80049CB4(0x128, 0x76);
    }
}
