#include "common.h"
extern s32 D_8013CA20;
extern void *func_8006A8D8(char *name, u32 size);
extern void func_8006AAF0(void *dst, u32 devAddr, s32 size);
void *func_8006ABC4(char *name, u32 devAddr, s32 size) { void *object; if (!D_8013CA20) return 0; object = func_8006A8D8(name, size); if (object) func_8006AAF0(object, devAddr, size); return object; }
