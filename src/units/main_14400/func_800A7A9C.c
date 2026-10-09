#include "common.h"

typedef unsigned char u8;
typedef short s16;

typedef struct Message Message;
typedef struct Result Result;

/* Event record passed to method slot 0x5C; only type, message and result are set here. */
typedef struct {
    s32 type;
    s32 pad04[3];
    Message *message;
    Result *result;
} Event800A7A9C;

typedef struct {
    u8 pad0[0x58];
    s16 delta_58;
    s16 pad5A;
    s32 (*handle_5C)(void *self, Event800A7A9C *event);
} VTable800A7A9C;

typedef struct {
    u8 pad0[0x24];
    VTable800A7A9C *vtable_24;
} Object;

void func_800A7A9C(Object *obj, Message *message, Result *result)
{
    Event800A7A9C event;
    Event800A7A9C *ev = &event;

    event.type = 8;
    ev->message = message;
    ev->result = result;
    obj->vtable_24->handle_5C((u8 *)obj + obj->vtable_24->delta_58, ev);
}
