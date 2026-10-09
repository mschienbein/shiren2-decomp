#include "common.h"
typedef unsigned char u8;
typedef short s16;
typedef struct Damage Damage;
typedef struct { s32 type; u8 fields_4[16]; } Message;
typedef struct { s32 type; u8 fields_4[12]; Damage *field_10; } DamageMessage;
typedef struct {
    u8 pad_0[0x58];
    s16 offset_58;
    s16 pad_5A;
    s32 (*method_5C)(void *, Message *);
} VTable;
typedef struct { u8 pad_0[0x24]; VTable *field_24; } Entity;
void func_800A7B68(Entity *object, Damage *damage)
{
    union { Message base; DamageMessage damage; } message;
    DamageMessage *payload = &message.damage;
    VTable *table;
    message.base.type = 10;
    payload->field_10 = damage;
    table = object->field_24;
    table->method_5C((u8 *)object + table->offset_58, &message.base);
}
