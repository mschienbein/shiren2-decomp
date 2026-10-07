#include "common.h"
extern s32 D_80151350[];
void func_800D8FA8(void *);
void func_80091BC8(void *self, s32 flags) {
    ((void **)self)[3] = D_80151350;
    if (flags & 1) {
        func_800D8FA8(self);
    }
}
