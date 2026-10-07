#include "common.h"
typedef struct { unsigned char pad_00[0x64]; s32 field_64; s32 field_68; } Object;
void func_800E2518(Object *object) {
    object->field_64 = 0;
    object->field_68 = 0;
}
