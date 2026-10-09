#include "common.h"
typedef struct { unsigned char pad_0[5]; unsigned char field_5; } Object;
void func_80094B80(void *arg, s32 value) { Object *object = arg; object->field_5 = (object->field_5 & ~1) | (value & 1); }
