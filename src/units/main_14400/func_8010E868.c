#include "common.h"
typedef struct { s32 x, y; } Position;
typedef struct { s32 kind_00; void *actor_04; void *target_08; s32 field_0C; Position position_10; s32 field_18, field_1C; } Event;
typedef struct {
    unsigned char pad_00[8]; short destroy_adjustment_08; short reserved_0A;
    void (*destroy_0C)(void *, s32);
    unsigned char pad_10[0x30]; short action_adjustment_40; short reserved_42;
    void (*action_44)(void *, void *);
} ItemVTable;
typedef struct { unsigned char pad_00[8]; ItemVTable *vtable_08; } Item;
void func_8010E988(Item *item, Event *event);
void func_800D3650(void *item);
s32 func_80049CB4(s32 id, ...);
s32 func_8010C96C(Item *item, Event *event);
static inline Position *event_position(Position *out, Event *event)
{
    out->x = event->position_10.x;
    out->y = event->position_10.y;
    return out;
}
static inline s32 event_kind(Event *event)
{
    return event->kind_00;
}

s32 func_8010E868(Item *item, Event *event)
{
    Position position;
    s32 result;
    switch (event_kind(event)) {
    case 9:
        item->vtable_08->action_44((unsigned char *)item + item->vtable_08->action_adjustment_40, event->actor_04);
        result = 1;
        break;
    case 18:
        func_8010E988(item, event);
        func_800D3650(item);
        if (item) {
            item->vtable_08->destroy_0C((unsigned char *)item + item->vtable_08->destroy_adjustment_08, 3);
        }
        result = 1;
        break;
    case 19:
        func_8010E988(item, event);
        result = 1;
        break;
    case 14:
        func_80049CB4(0x114, event_position(&position, event));
        func_800D3650(item);
        if (item) {
            item->vtable_08->destroy_0C((unsigned char *)item + item->vtable_08->destroy_adjustment_08, 3);
        }
        result = 1;
        break;
    default:
        result = func_8010C96C(item, event);
        break;
    }
    return result;
}
