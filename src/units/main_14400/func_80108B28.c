#include "common.h"

typedef unsigned char u8;

typedef struct { u8 pad0[0x89]; u8 field_89; } Obj89;

u8 func_80108B28(Obj89 *obj)
{
    return obj->field_89;
}
