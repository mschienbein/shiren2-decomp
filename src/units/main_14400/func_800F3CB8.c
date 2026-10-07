#include "common.h"

typedef unsigned char u8;

typedef struct { u8 pad0[0x86]; u8 field_86; } Obj86;

u8 func_800F3CB8(Obj86 *obj)
{
    return obj->field_86;
}
