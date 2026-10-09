#include "common.h"
extern void *func_800AC5B4(s32 size, s32 alternate);
extern void *func_80123B50(void *self);
void *func_80123B88(void) {
    return func_80123B50(func_800AC5B4(0x10, 0));
}
