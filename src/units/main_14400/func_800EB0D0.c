#include "common.h"

typedef struct { char pad0[0x78]; s32 field_78; } Obj;

void func_800EB0D0(Obj *obj, s32 value) { obj->field_78 = value; }
