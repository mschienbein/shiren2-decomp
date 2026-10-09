#include "common.h"
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
typedef struct { s32 pad; void *vtbl; } S;
extern u8 D_801589E8[];
S *func_800DDAD0(S *, s32);
void func_800DDC0C(S *, u8 *, s32);
S *func_800DEC68(S *s, u8 *str) { func_800DDAD0(s, 0x2C); s->vtbl = D_801589E8; { s32 len = *str++; func_800DDC0C(s, str, len - 1); } return s; }
