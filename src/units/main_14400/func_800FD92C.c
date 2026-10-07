#include "common.h"
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
typedef struct { u8 pad[0x24]; void *vtbl; } S;
extern u8 D_8015A7C0[];
void func_800EFD28(S *, s32);
void func_800A3918(S *);
void func_800FD92C(S *s, s32 flags) { s->vtbl = D_8015A7C0; func_800EFD28(s, 0); if (flags & 1) func_800A3918(s); }
