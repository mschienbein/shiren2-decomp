#include "common.h"
typedef struct { char pad[0x8]; void *unk8; } S;
extern char D_80153AA0[];
void func_800AC68C(S *);
void func_80124948(S *s, s32 f) { s->unk8 = D_80153AA0; if (f & 1) func_800AC68C(s); }
