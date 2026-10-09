#include "common.h"
typedef unsigned char u8;
typedef struct Actor Actor;
extern u8 D_80159BA0[];
/* Two-word request: method table at +0, actor pointer at +4 (read back by func_800F8360). */
typedef struct { void *vt; Actor *val; } Obj;
Obj *func_800F8348(Obj *o, Actor *v) { o->vt = D_80159BA0; o->val = v; return o; }
