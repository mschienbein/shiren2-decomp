#include "common.h"
typedef unsigned char u8;
/* Actor message variants are read through offset 0x17 by func_800F27A4. */
typedef struct { s32 kind; void *field_4; void *field_8; u8 variant_payload_C[0xC]; } Message;
typedef struct { u8 pad_0[0x58]; short offset_58; short pad_5A; s32 (*method_5C)(void *, Message *); } VTable;
typedef struct { u8 pad_0[0x24]; VTable *field_24; } Target;
typedef struct { u8 pad_0[0x10]; void *field_10; } Source;
s32 func_800C4D58(Source *obj, void *source, Target *target) {
    Message message;
    void *owner = obj->field_10;
    Message *event;
    message.kind = 15;
    message.field_4 = source;
    event = &message;
    event->field_8 = owner;
    return target->field_24->method_5C((u8 *)target + target->field_24->offset_58, event);
}
