#include "common.h"

extern void *func_800AC5B4(s32 size, s32 alternate);
extern void *func_80123A80(void *object);

void *func_80123AC4(void)
{
    return func_80123A80(func_800AC5B4(0x10, 0));
}
