#include "common.h"
typedef unsigned char u8;
extern u8 D_80153AA0[];
typedef struct { u8 pad[8]; void *vt; } Obj;
void func_800AC68C(Obj *o);
void func_8011FF80(Obj *o, s32 flags) { o->vt = D_80153AA0; if (flags & 1) func_800AC68C(o); }
