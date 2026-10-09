#include "common.h"
extern void *func_800AC5B4(s32 size, s32 alternate);
extern void *func_80122D70(void *obj);
void *func_80122DA8(void) { return func_80122D70(func_800AC5B4(0x2C, 0)); }
