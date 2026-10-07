#include "common.h"

typedef struct { char pad0[0x3F8]; s32 field_3F8; } Obj;

void func_800BA684(Obj *obj, s32 value) { obj->field_3F8 = value; }
