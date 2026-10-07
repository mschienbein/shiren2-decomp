#include "common.h"

extern void *func_800AC5B4(s32, s32);
extern void *func_80124BF0(void *);
void *func_80124C28(void) { return func_80124BF0(func_800AC5B4(16, 0)); }
