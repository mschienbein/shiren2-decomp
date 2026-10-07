#include "common.h"
typedef unsigned char u8;
extern u8 D_80159BA0[];
typedef struct { void *vt; s32 val; } Obj;
Obj *func_800F8348(Obj *o, s32 v) { o->vt = D_80159BA0; o->val = v; return o; }
