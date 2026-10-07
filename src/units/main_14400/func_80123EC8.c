#include "common.h"

void *func_800AC5B4(s32 kind, s32 arg);
void *func_80123E90(void *obj);

void *func_80123EC8(void) {
    return func_80123E90(func_800AC5B4(0x10, 0));
}
