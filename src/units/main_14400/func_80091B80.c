#include "common.h"
typedef struct { s32 field_00; s32 *field_04; } Object;
void func_80091B80(Object *object, s32 value) {
    if (object->field_04 && value >= 0 && value < *object->field_04)
        object->field_00 = value;
}
