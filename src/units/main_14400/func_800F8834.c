#include "common.h"

typedef unsigned char u8;

s32 func_800E8694(u8 *);
s32 func_800E8350(void *);
void *func_800A492C(void *, s32, s32, s32);
s32 func_800E66EC(void *);
s32 func_800E7104(void *);
s32 func_800F8834(u8 *self) {
    void *target;

    if (func_800E8694(self)) {
        return func_800E8350(self);
    }
    target = *(void **)(self + 0x58);
    if (target == 0) {
        target = func_800A492C(self, 2, 1, 1);
        if (target == 0) {
            return func_800E66EC(self);
        }
    }
    *(void **)(self + 0x58) = target;
    return func_800E7104(self);
}
