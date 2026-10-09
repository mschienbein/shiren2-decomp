#include "common.h"

typedef unsigned char u8;
typedef short s16;
typedef struct { s32 kind; void *field_04; u8 pad_08[8]; void *field_10; } Event;
typedef struct { u8 pad_00[0x58]; s16 delta; s16 pad_5A; s32 (*call)(void *, Event *); } VTable;
typedef struct { u8 pad_00[0x10]; void *field_10; } Action;
typedef struct { u8 pad_00[0x24]; VTable *field_24; } Object;

static __inline__ Event *init_event(Event *event, void *value) {
    event->kind = 0x10;
    event->field_04 = value;
    return event;
}

s32 func_800C4E80(Action *action, void *value, Object *object) {
    Event event;
    void *data = action->field_10;
    Event *message = init_event(&event, value);
    message->field_10 = data;
    return object->field_24->call((u8 *)object + object->field_24->delta, message);
}
