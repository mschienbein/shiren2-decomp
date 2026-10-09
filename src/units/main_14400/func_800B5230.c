#include "common.h"
typedef struct { unsigned char field_00[2]; unsigned char field_02; } Object;
extern Object *func_800B51D4(void *arg);
s32 func_800B5230(void *arg)
{
    Object *obj = func_800B51D4(arg);
    if (obj != 0) return obj->field_02;
    return 0;
}
