#include "common.h"
typedef unsigned char u8;
typedef struct { u8 pad_0[0xD]; u8 field_D; } Object;
s32 func_80113FAC(Object *object)
{
    return object->field_D & 1;
}
