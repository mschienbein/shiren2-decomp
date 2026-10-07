#include "common.h"

extern char D_80153AA0[];
void func_800AC68C(void *);
typedef struct { s32 x0; s32 x4; void *vtbl; } S;
void func_80125468(S *self, s32 flags) { self->vtbl = D_80153AA0; if (flags & 1) func_800AC68C(self); }
