#include "common.h"

typedef struct VTable VTable;
typedef struct { s32 field_00; VTable *field_04; } Object;
extern VTable D_80157FA8;
extern void func_800D8FE8(void *object);

void func_800D94F8(Object *object, s32 mode) {
    object->field_04 = &D_80157FA8;
    if (mode & 1) {
        func_800D8FE8(object);
    }
}
