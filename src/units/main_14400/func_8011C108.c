#include "common.h"

typedef struct VTable VTable;
typedef struct Object {
    unsigned char pad_00[8];
    VTable *vtable_08;
} Object;
extern VTable D_80153AA0;
extern void func_800AC68C(void *object);

void func_8011C108(Object *object, s32 flags)
{
    object->vtable_08 = &D_80153AA0;
    if (flags & 1)
        func_800AC68C(object);
}
