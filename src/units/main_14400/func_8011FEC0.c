#include "common.h"
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
typedef struct { s32 pad[2]; void *vtbl; } S;
extern u8 D_80153AA0[];
void func_800AC68C(S *);
void func_8011FEC0(S *s, s32 flags) { s->vtbl = D_80153AA0; if (flags & 1) func_800AC68C(s); }
