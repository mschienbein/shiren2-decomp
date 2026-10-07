#include "common.h"
typedef unsigned char u8;
extern u8 D_80157FA8[];
typedef struct { u8 pad[4]; void *vt; } Obj;
void func_800D8FE8(Obj *o);
void func_800DFFC4(Obj *o, s32 flags) { o->vt = D_80157FA8; if (flags & 1) func_800D8FE8(o); }
