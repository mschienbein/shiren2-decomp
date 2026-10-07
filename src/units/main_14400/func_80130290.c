#include "common.h"

typedef unsigned short u16;

/* Partial views: only the fields touched here are known. */
typedef struct {
    char pad0[0x88];
    s32 offset;
} Owner;

typedef struct {
    char pad0[0x8];
    Owner *owner;
    char padC[0xE];
    u16 field_1A;
} Obj;

typedef struct Message {
    struct Message *next;
    s32 field_4;
    u16 kind;
    u16 field_A;
    /* Kind 14 carries a sequence descriptor; field_4 remains scheduler time. */
    void *value;
} Message;

typedef struct {
    char pad0[0x1C];
    s32 base;
} Context;

extern Context *D_80148D84;

Message *func_80130780(void);
s32 func_8012E5F8(Owner *owner, s32 kind, Message *message);

void func_80130290(Obj *obj, void *value)
{
    Message *message;

    if (obj->owner != 0) {
        message = func_80130780();
        if (message != 0) {
            s32 address = D_80148D84->base + obj->owner->offset;

            message->kind = 14;
            message->value = value;
            message->next = 0;
            message->field_4 = address;
            message->field_A = obj->field_1A;
            func_8012E5F8(obj->owner, 3, message);
        }
    }
}
