#include "common.h"
typedef struct { s32 field_00; void *vtable_04; } Object800DB968;
extern unsigned char D_80157FA8[];
void func_800D8FE8(void *object);
void func_800DB968(Object800DB968 *object, s32 flags)
{
    object->vtable_04 = D_80157FA8;
    if (flags & 1) {
        func_800D8FE8(object);
    }
}
