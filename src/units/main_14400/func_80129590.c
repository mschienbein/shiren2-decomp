#include "common.h"
typedef unsigned char u8;
typedef struct { u8 field_00[0xAC]; unsigned short field_AC; } Object;
u8 *func_80129590(Object *object, u8 *cursor) {
    s32 value = *cursor++;
    if (value >= 0x80) {
        value &= 0x7F;
        value <<= 8;
        value |= *cursor++;
    }
    object->field_AC = value;
    return cursor;
}
