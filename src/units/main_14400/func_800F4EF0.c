#include "common.h"

typedef unsigned char u8;
typedef struct VTable VTable;
typedef struct { u8 pad_00[0xA]; u8 field_0A; u8 pad_0B[0x14]; u8 field_1F; u8 pad_20[4]; VTable *field_24; u8 pad_28[4]; } Object;
extern VTable D_801493A0;
extern void *func_800A38FC(s32 size);
extern void *func_800F4760(Object *object);

Object *func_800F4EF0(void) {
    Object *object = func_800A38FC(0x2C);
    func_800F4760(object);
    object->field_24 = &D_801493A0;
    object->field_0A = 8;
    object->field_1F = 8;
    return object;
}
