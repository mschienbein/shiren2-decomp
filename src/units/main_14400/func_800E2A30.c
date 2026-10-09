#include "common.h"
typedef unsigned char u8;
typedef short s16;
typedef struct {
    u8 pad_0[0x90];
    s16 offset_90;
    s16 pad_92;
    s32 (*method_94)(void *, s32, s32, u8, s32);
} VTable;
typedef struct { u8 pad_0[0x24]; VTable *field_24; } Object;
s32 func_800E2A30(Object *object)
{
    VTable *table = object->field_24;
    return table->method_94((u8 *)object + table->offset_90, 0, 14, 254, 0);
}
