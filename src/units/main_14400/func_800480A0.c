#include "common.h"

extern s32 D_80138BF0;
extern void func_8004633C(void),func_80060C54(u32 mode);
extern s32 func_80054DF0(u32 mode);
extern s32 func_800925EC(void);
void func_800480A0(void *unused_receiver) { D_80138BF0=1; func_8004633C(); func_80060C54(1); func_80054DF0(func_800925EC() ? 8 : 9); }
