#include "common.h"
typedef struct { s32 x; s32 y; } Pair;
typedef struct { unsigned char pad0[0xA0]; Pair field_A0; } Obj;
void func_80102E98(Obj *obj, Pair *value) { obj->field_A0 = *value; }
