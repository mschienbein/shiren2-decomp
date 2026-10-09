#include "common.h"
typedef struct VTable VTable;
typedef struct { s32 field0, field4; VTable *vtable; } Object;
extern VTable D_80153AA0;
extern void func_800AC68C(void *object);
void func_8011BDE4(Object *object, s32 flags) {
    object->vtable = &D_80153AA0;
    if (flags & 1) func_800AC68C(object);
}
