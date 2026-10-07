#include "common.h"
typedef struct { unsigned char field_00; unsigned char field_01[0x53]; float field_54; } Object;
void func_80090D98(Object *object, float value) { if (!object->field_00) object->field_54 = value; }
