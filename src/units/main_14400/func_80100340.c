#include "common.h"
typedef unsigned short u16;
/* +0x04 is the message sender pointer (stored by other senders); this item event leaves it unset. */
typedef struct { s32 kind_00; void *sender_04; void *item_08; s32 field_0C; u32 quantity_10; } Event;
typedef struct { unsigned char pad_00[0x58]; short adjustment_58; short reserved_5A; s32 (*method_5C)(void *object, Event *event); } EventVTable;
typedef struct { unsigned char pad_00[0x24]; EventVTable *vtable_24; } Object80100340;
s32 func_800AC670(void *item);
static inline Event *item_event(Event *event, void *item, u16 quantity)
{
    event->kind_00 = 14;
    event->item_08 = item;
    event->quantity_10 = quantity;
    return event;
}
s32 func_80100340(Object80100340 *object, void *item, u16 quantity)
{
    Event event;
    Event *payload;
    s32 result;
    if ((func_800AC670(item) ^ 1) == 0) {
        result = 0;
    } else {
        payload = item_event(&event, item, quantity);
        result = object->vtable_24->method_5C((unsigned char *)object + object->vtable_24->adjustment_58, payload);
    }
    return result;
}
