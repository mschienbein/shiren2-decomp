#include "common.h"
typedef struct { unsigned char kind, variant, field_02, flags, row, field_05, field_06, field_07, mode; signed char coordinates[2], status; } SelectionRecord;
extern SelectionRecord D_80142F18;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef struct { s32 x; s32 y; } Pos;
typedef struct { u8 value; } Dir;
typedef struct { u8 pad_0[0x1E]; u8 flags; } Unit;
typedef struct { u8 pad_0[0x20]; Unit *owner; } Context;
typedef struct { s32 kind; Unit *owner; void *target; Dir direction; Pos position; s32 count; void *data; } Message;
typedef struct { u8 pad_0[0x38]; s16 offset; u16 pad_3A; s32 (*dispatch)(void *, Message *); } Table;
typedef struct { u8 kind; u8 subtype; u8 flags; u8 pad_3[5]; Table *table; u8 flags_C; } Item;
extern void *func_800B4D80(Pos *pos);
extern s32 func_800EC630(Unit *owner, Item *item);
extern void func_8011205C(Context *context, Pos *pos, Dir direction);
extern void func_800AD868(Pos *pos);
extern s32 func_80049CB4(s32 id, ...);
extern void func_800A2758(Pos *pos, Dir direction);
extern s32 func_800B1AB8(Pos *pos);
extern u32 func_800B1C6C(Pos *pos);
extern s32 func_800B4888(Pos *pos);
extern s32 func_800B4F74(Pos *pos);
extern s32 func_800B56F0(void *pos);
extern s32 func_800AD714(Item *item, Pos *pos);
extern s32 func_800A251C(Pos *first, Pos *second);
extern void func_800AD7E0(Item *item, void *pos, s32 notify);
extern s32 func_80112084(Context *context, s32 kind, Pos *pos, Dir direction);

static __inline__ Dir copy_direction(const Dir *direction)
{
    return *direction;
}
static __inline__ Pos *copy_position(Pos *out, Pos *position)
{
    out->x = position->x;
    out->y = position->y;
    return out;
}
static __inline__ s32 advance_position(Item *item, Pos *position, Dir *direction)
{
    s32 available;
    func_800A2758(position, copy_direction(direction));
    available = 0;
    if (func_800B1AB8(position) != 0 && !(func_800B1C6C(position) & 0xE100) &&
        func_800B4888(position) == 0 && func_800B4F74(position) == 0 &&
        func_800B56F0(position) == 0) {
        available = func_800AD714(item, position) != 0;
    }
    return available;
}
s32 func_8011E2B0(Context *context, s32 kind, Pos *position, Dir direction)
{
    Pos next;
    Pos previous;
    Message message;
    if (kind == 5) {
        Item *item = func_800B4D80(position);
        u8 item_kind;
        s32 condition;
        s32 inert;
        if (item == 0) {
            return 0;
        }
        item_kind = item->kind;
        condition = 0;
        if (((context->owner->flags >> 2) & 1) && func_800EC630(context->owner, item) != 0) {
            u32 code = item_kind & 255;
            if (code == 19 && item->subtype != 242) {
                condition = 1;
            } else if (code == 15 && ((D_80142F18.mode & 224) ^ 32) != 0) {
                condition = 1;
            }
        } else {
            condition = 1;
        }
        if (condition != 0) {
            return 1;
        }
        func_8011205C(context, position, copy_direction(&direction));
        inert = 0;
        if ((item->flags & 32) || (item_kind == 16 && ((item->flags_C >> 1) & 1))) {
            inert = 1;
        }
        if (inert != 0) {
            return 0;
        }
        func_800AD868(position);
        func_80049CB4(225, position);
        if (item_kind == 15) {
            Pos *point = copy_position(&next, position);
            do {
                previous = next;
                condition = advance_position(item, point, &direction);
            } while (condition != 0);
            next = previous;
            if ((func_800A251C(&next, position) ^ 1) != 0) {
                func_80049CB4(4281, item, position, &next);
            }
            func_800AD7E0(item, &next, 1);
            return 0;
        }
        {
            Unit *owner = context->owner;
            message.kind = 17;
            message.owner = owner;
            message.position = *position;
            message.direction = copy_direction(&direction);
            {
                Message *event = &message;
                event->count = -1;
                message.data = 0;
                item->table->dispatch((u8 *)item + item->table->offset, event);
            }
        }
        return 0;
    } else {
        Pos *point = &next;
        point->x = position->x;
        point->y = position->y;
        return func_80112084(context, kind, point, copy_direction(&direction));
    }
}
