#include "common.h"
typedef unsigned char u8;
typedef struct { u8 field_00[0xD2]; u8 field_D2; u8 field_D3; } Object;
u8 *func_80129838(Object *object, u8 *cursor) {
    u8 value = *cursor;
    object->field_D2 = 0;
    object->field_D3 = value;
    return cursor + 1;
}
