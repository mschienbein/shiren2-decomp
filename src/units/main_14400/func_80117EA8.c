#include "common.h"

extern void *func_800AC5B4(s32 size, s32 flags);
extern void *func_80117E70(void *obj);

void *func_80117EA8(void) {
    return func_80117E70(func_800AC5B4(0xC, 0));
}
