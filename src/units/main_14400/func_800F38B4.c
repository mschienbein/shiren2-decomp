#include "common.h"

typedef unsigned char u8;

typedef struct { u8 pad0[0x9D]; u8 field_9D; } Obj9D;

u8 func_800F38B4(Obj9D *obj)
{
    return obj->field_9D;
}
