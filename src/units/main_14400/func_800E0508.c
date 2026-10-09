#include "common.h"
typedef struct { unsigned char pad_0[0x34]; unsigned char field_34; } Object;
void func_800E0508(Object *object, s32 first, s32 second) {
    first--;
    second--;
    object->field_34 = ((first & 7) << 3) | (second & 7);
}
