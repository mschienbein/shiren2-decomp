#include "common.h"

typedef unsigned char u8;

extern u8 D_801595F0[];
void func_800A38A0(void *, s32);
void func_800A3918(void *);
void func_800F479C(u8 *self, s32 flags) {
    *(void **)(self + 0x24) = D_801595F0;
    func_800A38A0(self, 0);
    if (flags & 1) {
        func_800A3918(self);
    }
}
