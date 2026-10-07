#include "common.h"

typedef unsigned char u8;

typedef struct { u8 pad0[0x73]; u8 field_73; } Obj73;

u8 func_800E24CC(Obj73 *obj)
{
    return obj->field_73;
}
