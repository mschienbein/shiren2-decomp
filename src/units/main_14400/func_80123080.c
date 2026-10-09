#include "common.h"
extern void *func_800AC5B4(s32 size, s32 alternate);
extern void *func_80123030(void *object);
void *func_80123080(void) { return func_80123030(func_800AC5B4(0x2C, 0)); }
