#include "common.h"
extern s32 D_80157FA8[];
void func_800D8FE8(void *);
void func_800E00EC(void *self, s32 flags) {
    ((void **)self)[1] = D_80157FA8;
    if (flags & 1) {
        func_800D8FE8(self);
    }
}
