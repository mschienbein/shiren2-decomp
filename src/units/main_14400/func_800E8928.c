#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef struct { u8 pad_0[0x98]; s16 offset_98; s16 pad_9A; void *(*method_9C)(void *); } VTable;
typedef struct { u8 pad_0[0x24]; VTable *field_24; } Obj800E9FDC;
extern s32 func_800CF1C8(void *list, u8 kind);
extern u16 D_80156A0A;
s32 func_800E8928(Obj800E9FDC *object)
{
    VTable *table = object->field_24;
    void *list = table->method_9C((u8 *)object + table->offset_98);
    s32 result;
    if (list == 0) {
        result = 0;
    } else {
        result = D_80156A0A * func_800CF1C8(list, 123);
    }
    return result;
}
