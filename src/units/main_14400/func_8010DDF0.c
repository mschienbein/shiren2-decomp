#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct { u8 pad0[0xC]; u16 field_C; } ObjC;

u16 func_8010DDF0(ObjC *obj)
{
    return obj->field_C;
}
