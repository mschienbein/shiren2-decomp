#include "common.h"

extern void func_800F479C(void *, s32);
extern void func_800A3918(void *);

void func_800F5150(void *object, s32 flags) {
    func_800F479C(object, 0);
    if (flags & 1) {
        func_800A3918(object);
    }
}
