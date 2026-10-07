#include "common.h"
typedef struct { s32 unk0; s32 unk4; void *vtbl8; } Obj;
extern char D_80153AA0[];
void func_800AC68C(Obj *);
void func_8011FD2C(Obj *o, s32 flags){ o->vtbl8 = D_80153AA0; if (flags & 1) func_800AC68C(o); }
