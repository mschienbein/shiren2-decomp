#include "common.h"
typedef unsigned char u8;
typedef struct { u8 pad_0[0x52]; u8 field_52; } Object;
s32 func_800E2394(Object *object)
{
    return object->field_52 == 3;
}
