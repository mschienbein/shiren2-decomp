#include "common.h"

extern void *func_800AC5B4(s32 size, s32 arg1);
extern void *func_80126C10(void *obj);

void *func_80126C48(void) {
    return func_80126C10(func_800AC5B4(0x10, 0));
}
