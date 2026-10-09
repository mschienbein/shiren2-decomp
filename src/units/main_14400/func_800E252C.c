#include "common.h"

typedef unsigned char u8;
typedef struct { u8 pad_00[0x5C]; u32 field_5C; u32 field_60; } Object;

s32 func_800E252C(Object *object) {
    return (object->field_60 | object->field_5C) != 0;
}
