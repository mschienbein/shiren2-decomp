#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef struct VTable VTable;
typedef struct { u16 field_00; u16 pad_02; VTable *field_04; u8 field_08; } Object;
extern VTable D_80157FA8;
extern VTable D_80158C40;

Object *func_800E0000(Object *object, u8 value) {
    object->field_04 = &D_80157FA8;
    object->field_00 = 0x3E;
    object->field_04 = &D_80158C40;
    object->field_08 = value;
    return object;
}
