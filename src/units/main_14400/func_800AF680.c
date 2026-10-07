#include "common.h"
typedef struct { unsigned char field_00[2]; unsigned char field_02; } Object;
void func_800AF680(Object *object) { object->field_02 |= 0x80; }
