#include "common.h"
extern unsigned char D_8015B1E8[];
extern void func_800EFD28(void *, s32);
extern void func_800A3918(void *);
void func_801006E8(void **arg, s32 flags) { arg[0x24 / 4] = D_8015B1E8; func_800EFD28(arg, 0); if (flags & 1) func_800A3918(arg); }
