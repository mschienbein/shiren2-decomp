#include "common.h"

typedef struct { s32 kind_00; const void *vtable_04; } Object;
extern unsigned char D_80157FA8[];
extern void func_800D8FE8(void *object);

void func_800DBB58(Object *object, s32 flags)
{
    object->vtable_04 = D_80157FA8;
    if (flags & 1) {
        func_800D8FE8(object);
    }
}
