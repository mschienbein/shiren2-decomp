#include "common.h"

typedef struct VTable VTable;
typedef struct { s32 field_00; s32 field_04; VTable *field_08; } Object;
extern VTable D_80153AA0;
extern void func_800AC68C(void *a);

void func_80119950(Object *object, s32 mode) {
    object->field_08 = &D_80153AA0;
    if (mode & 1) {
        func_800AC68C(object);
    }
}
