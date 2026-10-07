#include "common.h"
extern s32 D_801CA71C, D_801CA718, D_801CA714;
extern void *D_801CA720;
extern void *func_8012D84C(s32);
void func_8012AAC0(s32 count) { if (count < 0x40) count = 0x40; else if (count > 0x400) count = 0x400; D_801CA720 = func_8012D84C(count * 8); D_801CA71C = count; D_801CA718 = 0; D_801CA714 = 0; }
