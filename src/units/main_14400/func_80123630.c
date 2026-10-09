#include "common.h"
extern void *func_800AC5B4(s32 size, s32 alternate);
extern void *func_801235F0(void *object);
void *func_80123630(void) { return func_801235F0(func_800AC5B4(0x18, 0)); }
