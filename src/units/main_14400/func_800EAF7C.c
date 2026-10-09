#include "common.h"
typedef unsigned char u8;
typedef short s16;
typedef struct {
    u8 pad_0[0x60];
    s16 offset_60;
    s16 pad_62;
    void (*method_64)(void *);
} VTable;
typedef struct { u8 pad_0[0x24]; VTable *field_24; } Object;
extern s32 func_800E5E4C(Object *object, s32 notify);
s32 func_800EAF7C(Object *object, s32 notify)
{
    VTable *table;
    func_800E5E4C(object, notify);
    table = object->field_24;
    table->method_64((u8 *)object + table->offset_60);
    return 1;
}
