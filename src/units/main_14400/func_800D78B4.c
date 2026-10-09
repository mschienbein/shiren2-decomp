#include "common.h"
typedef struct { unsigned char field_00[0xA]; unsigned char field_0A; unsigned char field_0B[0x92]; unsigned char field_9D; } Object;
extern s32 func_800E0F40(Object *obj);
extern s32 func_800D78F4(unsigned char kind, unsigned char value, unsigned char level);
s32 func_800D78B4(Object *obj)
{
    s32 type = obj->field_0A;
    unsigned char value = func_800E0F40(obj);
    return func_800D78F4(type, value, obj->field_9D);
}
