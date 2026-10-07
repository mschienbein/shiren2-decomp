#include "common.h"

typedef struct { char pad0[0x10]; s32 field_10; } Obj;
void func_800D041C(Obj *o, s32 v) { o->field_10 = v; }
