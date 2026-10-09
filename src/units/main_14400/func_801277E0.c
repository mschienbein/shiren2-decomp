#include "common.h"
typedef struct { char pad0[8]; void *field8; } Obj;
extern unsigned char D_80153AA0[];
void func_800AC68C(void *a);
void func_801277E0(Obj *obj, s32 flags) { obj->field8 = D_80153AA0; if (flags & 1) func_800AC68C(obj); }
