#include "common.h"
typedef struct { void *field_00; void *vtable_04; unsigned char *field_08; } Object;
extern s32 func_800AFD08(void *table, void *obj);
void func_800CE7D8(Object *object, s32 index, void *value) {
    object->field_08[index] = func_800AFD08(object->field_00, value);
}
