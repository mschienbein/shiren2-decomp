#include "common.h"
typedef unsigned short u16;
typedef struct { unsigned char pad_00[0xC]; const void *vtable_0C; u16 field_10; } Object;
extern void *func_800C4EC0(void *, s32, s32, s32);
extern const unsigned char D_80154038[];
Object *func_800C4F34(Object *object, u16 value) {
    func_800C4EC0(object, 0x812, 0x110, 0x110);
    object->vtable_0C = D_80154038;
    object->field_10 = value;
    return object;
}
