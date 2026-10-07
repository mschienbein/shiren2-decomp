#include "common.h"
typedef struct { unsigned char pad_00[2]; unsigned char field_02; } Object;
void func_800AF73C(Object *object) {
    object->field_02 |= 1;
}
