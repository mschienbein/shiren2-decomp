#include "common.h"

extern void *func_800AC5B4(s32 size, s32 kind);
extern void *func_801254A0(void *obj);

void *func_801254E4(void) {
    return func_801254A0(func_800AC5B4(0x10, 0));
}
