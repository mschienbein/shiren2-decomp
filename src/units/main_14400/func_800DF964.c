#include "common.h"
typedef struct { s32 unk0; void *vtbl4; } Obj;
extern char D_80157FA8[];
void func_800D8FE8(Obj *);
void func_800DF964(Obj *o, s32 flags){ o->vtbl4 = D_80157FA8; if (flags & 1) func_800D8FE8(o); }
