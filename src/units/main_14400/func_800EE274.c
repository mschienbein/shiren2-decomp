#include "common.h"
typedef unsigned char u8;
typedef struct { u8 pad_0[0x104]; void *field_104; } Object;
s32 func_800EE274(Object *object, void *other)
{
    void *stored = object->field_104;
    s32 equal = 0;
    if (stored != 0) {
        equal = other == stored;
    }
    return equal;
}
