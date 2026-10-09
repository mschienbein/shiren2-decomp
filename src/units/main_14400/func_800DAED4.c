#include "common.h"
typedef struct VTable VTable;
typedef struct { s32 field0; VTable *vtable; } Object;
extern VTable D_80157FA8;
extern void func_800D8FE8(void *object);
void func_800DAED4(Object *object, s32 flags) {
    object->vtable = &D_80157FA8;
    if (flags & 1) func_800D8FE8(object);
}
