#include "common.h"
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
typedef struct { s32 pad; void *vtbl; } S;
extern u8 D_80157FA8[];
void func_800D8FE8(S *);
void func_800DFE08(S *s, s32 flags) { s->vtbl = D_80157FA8; if (flags & 1) func_800D8FE8(s); }
