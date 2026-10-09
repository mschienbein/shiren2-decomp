#include "common.h"
typedef struct { unsigned char pad0[0xA]; unsigned char fieldA; unsigned char padB[0x14]; unsigned char field1F; unsigned char pad20[4]; void *vtable24; } Object;
extern unsigned char D_80149678[];
extern void *func_800A38FC(s32 size);
extern void *func_800F4760(void *object);
Object *func_800F5F10(void) {
    Object *object = func_800A38FC(0x2C);
    func_800F4760(object);
    object->vtable24 = D_80149678;
    object->fieldA = 0x15;
    object->field1F = 0x15;
    return object;
}
