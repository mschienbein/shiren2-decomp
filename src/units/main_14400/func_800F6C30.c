#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct VTable VTable;
typedef struct { u8 pad0[0x1C]; u16 field1C; u8 pad1E[6]; VTable *vtable; } Object;
extern VTable D_80149778;
extern Object *func_800F3CF0(Object *object, s32 kind, u8 value);
void *func_800F6C30(void *obj, u8 id) {
    Object *object = obj;
    func_800F3CF0(object, 0x59, id);
    object->vtable = &D_80149778;
    object->field1C |= 0x80;
    return object;
}
