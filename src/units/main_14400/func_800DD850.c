#include "common.h"

typedef unsigned char u8;

extern u8 D_80157FA8[];
void func_800D8FE8(void *);
void func_800DD850(u8 *self, s32 flags) {
    *(void **)(self + 4) = D_80157FA8;
    if (flags & 1) {
        func_800D8FE8(self);
    }
}
