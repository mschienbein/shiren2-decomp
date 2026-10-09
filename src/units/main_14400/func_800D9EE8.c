#include "common.h"
typedef struct VTable VTable;
typedef struct { s32 field0; const VTable *vtable4; } Object;
extern const VTable D_80157FA8;
extern void func_800D8FE8(void *object);
void func_800D9EE8(Object *object, s32 flags) {
    object->vtable4 = &D_80157FA8;
    if (flags & 1) func_800D8FE8(object);
}
