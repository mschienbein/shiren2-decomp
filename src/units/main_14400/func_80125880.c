#include "common.h"
typedef unsigned short u16;
extern u16 D_80156A6C;
extern s32 func_80049CB4(s32 id, ...);
extern u16 func_80115944(void *owner, u16 value);
extern void func_800A7B18(void *target, void *source, s32 amount, s32 kind);
/* Slot 0x44 receives seven pointers; origin, direction and item are unused. */
s32 func_80125880(void *self, void *source, void *origin, void *position, void *direction, void *target, void *item) {
    func_80049CB4(0x10F3, position);
    if (target != 0) {
        func_80049CB4(0x41, target);
        func_80049CB4(0x94, target);
        func_800A7B18(target, source, func_80115944(self, D_80156A6C), 12);
    }
    return 1;
}
