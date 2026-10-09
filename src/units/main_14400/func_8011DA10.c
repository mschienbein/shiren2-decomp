#include "common.h"
typedef unsigned char u8;
typedef struct VTable VTable;
typedef struct { s32 field0, field4; VTable *vtable; s32 fieldC; u8 field10, field11; } Object;
extern VTable D_8015ED90;
extern Object *func_80111530(Object *object, s32 kind);
Object *func_8011DA10(Object *object, s32 kind) {
    func_80111530(object, kind);
    object->vtable = &D_8015ED90;
    object->field10 = 0;
    object->field11 = 0;
    return object;
}
