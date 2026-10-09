#include "common.h"
typedef unsigned char u8;
typedef struct VTable VTable;
typedef struct { u8 pad0[0xA]; u8 fieldA; u8 padB[0x14]; u8 field1F; u8 pad20[4]; const VTable *vtable24; } Object;
extern void *func_800A38FC(s32 size);
extern void *func_800F4760(Object *);
extern const VTable D_80149408;
Object *func_800F5590(void) {
    Object *object = func_800A38FC(0x2C);
    func_800F4760(object);
    object->vtable24 = &D_80149408;
    object->fieldA = 12;
    object->field1F = 12;
    return object;
}
