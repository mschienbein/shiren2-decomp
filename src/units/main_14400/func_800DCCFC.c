#include "common.h"
typedef struct { unsigned char kind, variant, field_02, flags, row, field_05, field_06, field_07, mode; signed char coordinates[2], status; } SelectionRecord;
extern SelectionRecord D_80142F18;
typedef unsigned char u8;
typedef struct { s32 kind_00; void *actor_04; void *target_08; s32 field_0C; s32 x_10, y_14, value_18, flags_1C; } ItemEvent;
typedef struct { u8 pad_00[0x38]; short adjustment_38; short reserved_3A; s32 (*event_3C)(void *, ItemEvent *); } ItemVTable;
typedef struct { u8 pad_00[8]; ItemVTable *vtable_08; } Item;
typedef struct { u8 pad_00[0x60]; short adjustment_60; short reserved_62; s32 (*query_64)(void *, void *, s32); } ListVTable;
typedef struct { s32 field_00; ListVTable *vtable_04; } List;
typedef struct { List *list; Item *item; } Selection;
typedef struct { u8 pad_00[8]; Selection selection_08; } Object800DCCFC;
typedef struct { u8 pad_00[8]; u8 direction_08; } Actor;

extern Actor *D_801476B8;
s32 func_800E1CC4(void *actor, s32 mode);
void func_800A665C(void *actor, u8 *direction);
void func_800DAD20(void *object, void *item);
void func_800DAD80(void *object);
static inline ItemEvent *actor_event(ItemEvent *event, Actor *actor)
{
    event->kind_00 = 5;
    event->actor_04 = actor;
    return event;
}
s32 func_800DCCFC(Object800DCCFC *object)
{
    ItemEvent event;
    u8 direction;
    Selection *selection;
    Item *item;
    s32 available;
    if ((D_80142F18.flags >> 2) & 1) {
        return 1;
    }
    selection = &object->selection_08;
    available = selection->list->vtable_04->query_64((u8 *)selection->list + selection->list->vtable_04->adjustment_60, selection->item, 1) == 1;
    if (!available) {
        return 0;
    }
    if (func_800E1CC4(D_801476B8, 4)) {
        direction = (D_801476B8->direction_08 + 4) & 7;
        func_800A665C(D_801476B8, &direction);
    }
    item = selection->item;
    func_800DAD20(object, item);
    item->vtable_08->event_3C((u8 *)item + item->vtable_08->adjustment_38, actor_event(&event, D_801476B8));
    func_800DAD80(object);
    return 0;
}
