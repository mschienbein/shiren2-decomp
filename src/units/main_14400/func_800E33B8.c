#include "common.h"
typedef struct { unsigned char pad_00[0x54]; unsigned char field_54; } Object;
void func_800E33B8(Object *object, s32 bits) { object->field_54 |= bits; }
