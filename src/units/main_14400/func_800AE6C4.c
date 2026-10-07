#include "common.h"
typedef unsigned short u16;
typedef struct { unsigned char field_00[0x18]; short field_18; s32 (*field_1C)(void *, s32); } VTable;
typedef struct { unsigned char field_00[8]; VTable *field_08; u16 field_0C; } Object;
void func_800AE6C4(Object *object, u16 value) { if (object->field_08->field_1C((char *)object + object->field_08->field_18, 0x1E)) object->field_0C = value; }
