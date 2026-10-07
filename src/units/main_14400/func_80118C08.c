#include "common.h"

extern void *func_800AC5B4(s32 size, s32 arg1);
extern void *func_80118BD0(void *obj);

void *func_80118C08(void) {
    return func_80118BD0(func_800AC5B4(0xC, 0));
}
