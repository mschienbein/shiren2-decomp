#include "common.h"
typedef struct { unsigned char field_00[0x1C]; unsigned short field_1C; } Object;
void func_800A8330(Object *obj, s32 mask)
{
    obj->field_1C &= ~mask;
}
