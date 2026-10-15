#include "common.h"
#include "item_records.h"
typedef struct { s32 x, y; } Position;
typedef struct { s32 kind_00; void *actor_04; void *target_08; s32 field_0C; Position position_10; s32 field_18, field_1C; } Event;
/* Kind 11 constructor 8010E5C0 installs D_8015D398. */
typedef struct {
    ItemMethods base;
    short delta40, index42;
    void (*action44)(void *, void *);
} ItemActionMethods;
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
        ((ItemActionMethods *)item->field08)->action44((unsigned char *)item + ((ItemActionMethods *)item->field08)->delta40, event->actor_04);
        result = 1;
        break;
    case 18:
        func_8010E988(item, event);
        func_800D3650(item);
        if (item) {
            item->field08->method0C((unsigned char *)item + item->field08->delta08, 3);
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
            item->field08->method0C((unsigned char *)item + item->field08->delta08, 3);
        }
        result = 1;
        break;
    default:
        result = func_8010C96C(item, event);
        break;
    }
    return result;
}
