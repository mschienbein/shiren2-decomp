#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct { u8 pad0[0x2C]; u16 field_2C; } Obj2C;

u16 func_800E333C(Obj2C *obj)
{
    return obj->field_2C;
}
