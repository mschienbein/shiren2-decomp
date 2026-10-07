#include "common.h"

typedef unsigned char u8;

typedef struct { u8 pad0[0x3E8]; s32 field_3E8; } Obj3E8;

void func_800BA68C(Obj3E8 *obj, s32 value)
{
    obj->field_3E8 = value;
}
