#include "common.h"

typedef struct { s32 field00; const void *vtable04; } Object;
extern const u32 D_80157FA8[];
extern void func_800D8FE8(void *object);

void func_800DAFA4(Object *object, s32 flags)
{
    object->vtable04 = D_80157FA8;
    if (flags & 1)
        func_800D8FE8(object);
}
