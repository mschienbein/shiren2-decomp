#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef struct { u8 pad_0[0x28]; s16 offset_28; s16 pad_2A; void (*read_2C)(void *, s32, void *); } VTable;
typedef struct { u8 pad_0[0x14]; void *field_14; VTable *field_18; } Obj;
extern unsigned short func_800CA584(void *object, const unsigned char *text);
void func_800CA4E8(Obj *object, void *text)
{
    u16 checksum;
    s32 occupied = object->field_14 != 0;
    VTable *table = object->field_18;
    table->read_2C((u8 *)object + table->offset_28, 2, &checksum);
    if (!occupied && checksum != func_800CA584(object, text)) {
        object->field_14 = text;
    }
}
