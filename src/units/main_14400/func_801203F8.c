#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

typedef struct { u8 pad[8]; void *vt; } S;
extern u8 D_80153AA0[];
void func_800AC68C(S *s);
void func_801203F8(S *s, s32 flags) { s->vt = D_80153AA0; if (flags & 1) func_800AC68C(s); }
