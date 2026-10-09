#include "common.h"

typedef unsigned char u8;
/* Slot 4 returns a promoted count: 800CDEB8 consumes all of v0 without narrowing. */
typedef struct { unsigned char pad_00[0x20]; short delta_20; short index_22; s32 (*method_24)(void *); } Methods;
typedef struct { void *pool_00; Methods *field_04; } Obj;
s32 func_800CE564(Obj *obj)
{
    return obj->field_04->method_24((u8 *)obj + obj->field_04->delta_20);
}
