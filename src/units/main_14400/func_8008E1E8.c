#include "common.h"

typedef struct { unsigned char field00; } Obj;

s32 func_8008E1E8(Obj *obj)
{
    return obj->field00 == 2;
}
