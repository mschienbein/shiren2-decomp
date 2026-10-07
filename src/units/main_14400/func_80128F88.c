#include "common.h"

extern void *func_800AC5B4(s32 size, s32 arg1);
extern void *func_80128F50(void *obj);

void *func_80128F88(void) {
    return func_80128F50(func_800AC5B4(0xC, 0));
}
