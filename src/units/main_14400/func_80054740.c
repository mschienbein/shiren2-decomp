#include "common.h"

typedef unsigned char u8;
extern u8 D_80161B44;
void func_80053D70(s32, s32, s32, s32, s32, s32, char *, void *);
void func_80054740(s32 mode, char *fmt, void *args){ D_80161B44 = 1; func_80053D70(0, mode, 5, 0x17, 30, 4, fmt, args); }
