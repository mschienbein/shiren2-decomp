#include "common.h"

typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;

typedef struct {
    s32 x;
    s32 y;
} Pos;

typedef struct {
    u8 value;
} Dir;

typedef struct {
    u8 pad_0[0x1E];
    u8 flags;
} Unit;

typedef struct {
    u8 pad_0[0x20];
    Unit *owner;
} Context;

/* Item vtable slot 3: kind query (e.g. func_801172A0 / func_8011702C). */
typedef struct {
    u8 pad_0[0x18];
    s16 offset;
    u16 pad_1A;
    s32 (*is_kind)(void *self, s32 kind);
} ItemTable;

typedef struct {
    u8 kind;
    u8 subtype;
    u8 flags;
    u8 pad_3[5];
    ItemTable *table;
    u8 flags_C;
} Item;

/* Line iterator over positions (func_800C25D0 / func_800C2758). */
typedef struct {
    Pos cur;
    Dir dir;
    u8 pad_9[3];
    s32 count;
    s32 index;
    s32 field_14;
} Iter;

extern void *func_800B4D80(Pos *pos);
extern s32 func_800EC630(Unit *owner, Item *item);
extern void func_8011205C(Context *context, Pos *pos, Dir direction);
extern void func_800C25D0(Iter *iter, Pos *pos, Dir *dir, s32 range);
extern void *func_800C2758(void *out, void *iterator);
extern u32 func_800B1C6C(Pos *pos);
extern void *func_800B4928(Pos *pos);
extern void func_800A2758(Pos *pos, Dir direction);
extern s32 func_800A251C(Pos *first, Pos *second);
extern void func_800AD868(Pos *pos);
extern s32 func_80049CB4(s32 id, ...);
extern s32 func_800B56F0(void *object);
extern s32 func_800B5690(void *object);
extern s32 func_800ADC90(void *obj, void *pos, void *origin);
extern s32 func_80112084(Context *context, s32 kind, Pos *pos, Dir direction);
typedef struct { u8 kind, variant, field_02, flags, row, field_05, field_06, field_07, mode; signed char coordinates[2], status; } SelectionRecord;
extern SelectionRecord D_80142F18;

static __inline__ void copy_position(Pos *out, Pos *position) {
    out->x = position->x;
    out->y = position->y;
}

/* The direction pointing the other way (eight compass directions). */
static __inline__ Dir opposite(const Dir *direction) {
    Dir result;

    result.value = (direction->value + 4) & 7;
    return result;
}

/* Throw/push action: carry the item at position along the reversed direction. */
s32 func_8011EA60(Context *context, s32 kind, Pos *position, Dir direction) {
    Pos cur;
    Iter iter;
    Pos step;
    Dir back;

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
        if (!(((context->owner->flags >> 2) & 1) && func_800EC630(context->owner, item) != 0)
            || (item_kind == 19 && item->subtype != 242)
            || (item_kind == 15 && ((D_80142F18.mode & 224) ^ 32) != 0)) {
            condition = 1;
        }
        if (condition != 0) {
            return 1;
        }
        func_8011205C(context, position, direction);
        inert = 0;
        if ((item->flags & 32) || (item_kind == 16 && ((item->flags_C >> 1) & 1))) {
            inert = 1;
        }
        if (inert != 0) {
            return 0;
        }
        back = opposite(&direction);
        func_800C25D0(&iter, position, &back, 0xFF);
        for (;;) {
            s32 found;

            if (iter.index >= iter.count) {
                break;
            }
            func_800C2758(&step, &iter);
            found = 0;
            copy_position(&step, &iter.cur);
            cur = step;
            if ((func_800B1C6C(&cur) & 0x4000) || (item_kind == 15 && (func_800B1C6C(&cur) & 0x2180))
                || func_800B4928(&cur) == context->owner) {
                found = 1;
            }
            if (found) {
                func_800A2758(&cur, direction);
                break;
            }
        }
        {
            Pos *target = &cur;
            s32 occupied;

            if ((func_800A251C(position, target) ^ 1) == 0) {
                return 0;
            }
            func_800AD868(position);
            func_80049CB4(0xB9, item, position, target);
            func_80049CB4(2);
            occupied = 0;
            if (item->table->is_kind((u8 *)item + item->table->offset, 0x24)) {
                occupied = func_800B56F0(target) != 0;
            }
            if (occupied) {
                func_800B5690(target);
            }
            func_800ADC90(item, target, target);
        }
        return 0;
    } else {
        Pos *point = &cur;

        point->x = position->x;
        point->y = position->y;
        return func_80112084(context, kind, point, direction);
    }
}
