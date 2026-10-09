#include "common.h"
typedef struct { s32 field0; s32 field4; void *field8; } Obj;
extern char D_80153AA0[];
extern void func_800AC68C(Obj *);
void func_801255C8(Obj *p, s32 flags) { p->field8 = D_80153AA0; if (flags & 1) func_800AC68C(p); }
