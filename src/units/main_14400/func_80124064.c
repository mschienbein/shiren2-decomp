#include "common.h"

void *func_800AC5B4(s32 size, s32 a1);
void *func_80124020(void *obj);

void *func_80124064(void) {
    return func_80124020(func_800AC5B4(0x10, 0));
}
