#include "common.h"

void *func_800AC5B4(s32 size, s32 arg1);
void *func_801185C0(void *obj);

void *func_801185F8(void) {
    return func_801185C0(func_800AC5B4(0xC, 0));
}
