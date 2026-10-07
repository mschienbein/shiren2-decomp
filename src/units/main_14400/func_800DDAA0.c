#include "common.h"

typedef unsigned char u8;

typedef struct { s32 unk0; void *vtable; } S;
extern u8 D_80157FA8[];
void func_800D8FE8(void *);
void func_800DDAA0(S *s, s32 flags) { s->vtable = D_80157FA8; if (flags & 1) func_800D8FE8(s); }
