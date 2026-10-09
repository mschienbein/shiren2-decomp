#include "common.h"
typedef struct { unsigned char pad_0[0xC]; unsigned char field_C; } Object;
void func_80128B04(Object *object) { object->field_C &= ~8; }
