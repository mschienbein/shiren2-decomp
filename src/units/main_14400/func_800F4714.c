#include "common.h"

extern void func_800E016C(void *, s32);
extern void func_800A3918(void *);

void func_800F4714(void *object, s32 flags) {
    func_800E016C(object, 0);
    if (flags & 1) {
        func_800A3918(object);
    }
}
