#include "common.h"

typedef struct VTable VTable;
typedef struct Object {
    unsigned char pad_00[4];
    VTable *vtable_04;
} Object;
extern VTable D_80157FA8;
extern void func_800D8FE8(void *object);

void func_800DB0F8(Object *object, s32 flags)
{
    object->vtable_04 = &D_80157FA8;
    if (flags & 1)
        func_800D8FE8(object);
}
