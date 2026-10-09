#include "common.h"

typedef struct { s32 field_00; unsigned char pad_04[12]; s32 field_10; } Message;
typedef struct { unsigned char pad_00[0x58]; short delta_58; short index_5A; s32 (*method_5C)(void *, Message *); } Methods;
typedef struct Object { unsigned char pad_00[0x24]; Methods *field_24; } Object;
void func_800A7BA4(Object *obj, s32 value)
{
    Message message;
    Message *p = &message;
    message.field_00 = 0x14;
    p->field_10 = (unsigned char)value;
    obj->field_24->method_5C((unsigned char *)obj + obj->field_24->delta_58, p);
}
