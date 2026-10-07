#include "common.h"
typedef unsigned short u16;
typedef struct { void (*handler)(void *); u16 unk4; char pad6[0x16]; s32 unk1C; char pad20[0x54]; } Entry74;
extern Entry74 D_801BA380[];
s32 func_80084CD4(void (*handler)(void *));
void func_800850F8(void (*handler)(void *), u16 value){ Entry74 *e = &D_801BA380[func_80084CD4(handler)]; e->unk1C = value; if (e->unk4 == 0) e->unk4 = 1; }
