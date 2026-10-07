#include "common.h"

extern void *func_800AC5B4(s32 size, s32 arg1);
extern void *func_80119980(void *obj);

void *func_801199B8(void) {
    return func_80119980(func_800AC5B4(0x10, 0));
}
