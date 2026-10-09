#include "common.h"
typedef unsigned char u8;
typedef struct { void *pool_0; void *field_4; } Object;
extern u8 D_801545E0[];
Object *func_800D0630(Object *object)
{
    object->field_4 = D_801545E0;
    return object;
}
