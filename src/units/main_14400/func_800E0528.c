#include "common.h"
typedef struct { unsigned char field_00[0x34]; unsigned char field_34; } Object;
void func_800E0528(Object *object, s32 value) { object->field_34 = value | 0x80; }
