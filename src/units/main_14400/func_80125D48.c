#include "common.h"

void *func_800AC5B4(s32 size, s32 arg1);
void *func_80125D10(void *obj);

void *func_80125D48(void) {
    return func_80125D10(func_800AC5B4(0x10, 0));
}
