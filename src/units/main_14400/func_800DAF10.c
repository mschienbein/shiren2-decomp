#include "common.h"
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
typedef struct { s32 pad; void *vtbl; } S;
extern u8 D_801583E8[];
void *func_800DA8A0(void *obj, s32 kind, void *src);
S *func_800DAF10(S *s, void *src) { func_800DA8A0(s, 13, src); s->vtbl = D_801583E8; return s; }
