#include "common.h"
typedef unsigned char u8;
extern u8 D_80158418[];
typedef struct { u8 pad[4]; void *vt; } Obj;
void *func_800DA8A0(void *obj, s32 kind, void *src);
Obj *func_800DAFE0(Obj *o, void *src) { func_800DA8A0(o, 0xE, src); o->vt = D_80158418; return o; }
