#include "common.h"
typedef unsigned char u8;
typedef struct { u8 pad_0[0x14]; u8 field_14; u8 field_15; } Object;
void func_801128B0(Object *object, u8 value)
{
    object->field_14 = value;
    object->field_15 = 1;
}
