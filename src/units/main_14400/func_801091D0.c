#include "common.h"
extern unsigned char D_80159130[], D_8015C690[];
extern void *func_800EE3C0(void *obj, s32 kind, unsigned char mode);
extern s32 func_800A3934(void *);
extern void func_801092F4(void *, s32);
void **func_801091D0(void **arg, unsigned char value) { func_800EE3C0(arg, 0x18, value); arg[0xB4 / 4] = D_80159130; arg[0x24 / 4] = D_8015C690; if (!func_800A3934(arg)) func_801092F4(arg, value); return arg; }
