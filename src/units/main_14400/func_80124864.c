#include "common.h"

void *func_800AC5B4(s32 size, s32 arg1);
void *func_80124820(void *arg0);

void *func_80124864(void) {
    return func_80124820(func_800AC5B4(0x10, 0));
}
