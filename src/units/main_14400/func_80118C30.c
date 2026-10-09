#include "common.h"

extern s32 func_800A529C(void *obj, s32 flag);

/* Item vtable slot +0x44: `self` is the adjusted receiver the dispatcher supplies; not used here. */
void func_80118C30(void *self, void *target) {
    func_800A529C(target, 0);
}
