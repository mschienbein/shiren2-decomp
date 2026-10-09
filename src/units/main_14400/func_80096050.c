#include "common.h"
typedef struct { unsigned char pad_0[0x20]; s32 field_20; } Obj;
typedef struct { s32 field_0, field_4; } Pair;
s32 func_80096050(Obj *obj, Pair *pair) { return pair->field_0 + obj->field_20 * pair->field_4; }
