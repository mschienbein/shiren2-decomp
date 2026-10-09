#include "common.h"

typedef struct { unsigned char pad_00[0xA0]; s32 field_A0; } Obj;
s32 func_80107300(Obj *obj, s32 value)
{
    return obj->field_A0 == value;
}
