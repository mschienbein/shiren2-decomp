#include "common.h"
extern s32 D_80138BF0;
extern void func_8004633C(void);
extern void func_80060C54(u32 mode);
/* The original method receives an object but updates global camera state. */
void func_80047A90(void *unused) { D_80138BF0 = 1; func_8004633C(); func_80060C54(6); }
