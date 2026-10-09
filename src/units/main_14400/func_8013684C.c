#include "common.h"
typedef struct VTable VTable;
typedef struct { s32 field0; const VTable *vtable4; } Object;
extern const VTable D_80149DB8;
extern void func_800D8FA8(void *object);
void func_8013684C(Object *object, s32 flags) {
    object->vtable4 = &D_80149DB8;
    if (flags & 1) func_800D8FA8(object);
}
