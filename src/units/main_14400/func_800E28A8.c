#include "common.h"

typedef unsigned char u8;
typedef struct { u8 pad_00[0x90]; short delta_90; short index_92; s32 (*method_94)(void *, s32, s32, u8, s32); } Methods;
typedef struct { u8 pad_00[0x24]; Methods *field_24; } Obj;
s32 func_800E28A8(Obj *obj)
{
    return obj->field_24->method_94((u8 *)obj + obj->field_24->delta_90, 1, 0x10, 0, 0);
}
