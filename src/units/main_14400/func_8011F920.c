#include "common.h"

typedef unsigned char u8;

typedef struct { s32 unk0; s32 unk4; void *vtable; } S;
extern u8 D_80153AA0[];
void func_800AC68C(void *);
void func_8011F920(S *s, s32 flags) { s->vtable = D_80153AA0; if (flags & 1) func_800AC68C(s); }
