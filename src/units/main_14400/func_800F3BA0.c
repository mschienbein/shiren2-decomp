#include "common.h"
typedef struct { unsigned char field_00[0x9A]; unsigned short field_9A; } Object;
s32 func_800F3BA0(Object *obj)
{
    return obj->field_9A & 1;
}
