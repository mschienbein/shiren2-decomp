#include "common.h"

extern void func_800F479C(void *, s32);
extern void func_800A3918(void *);
/* Destructor bound at D_80159760+0xC (unit family D_8015CB48 destroy slot): void (void *self, s32 flags). */
void func_800F5540(void *object, s32 flags) {
    func_800F479C(object, 0);
    if (flags & 1) func_800A3918(object);
}
