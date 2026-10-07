#include "common.h"
extern unsigned char D_801585F8[];
extern void *func_800DA8A0(void *obj, s32 kind, void *src);
void **func_800DC3F0(void **arg, void *src) { func_800DA8A0(arg, 0x18, src); arg[1] = D_801585F8; return arg; }
