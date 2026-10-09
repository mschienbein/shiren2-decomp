#include "common.h"
typedef struct { unsigned char field_00[0x7C]; unsigned short field_7C; } Object;
s32 func_800F3C08(Object *obj)
{
    return (obj->field_7C >> 12) & 1;
}
