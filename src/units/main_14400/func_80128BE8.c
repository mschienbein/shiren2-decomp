#include "common.h"
typedef struct { unsigned char field_00[0xD]; unsigned char field_0D; } Object;
void func_80128BE8(Object *object, s32 value) { object->field_0D = value; }
