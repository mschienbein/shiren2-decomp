#include "common.h"

typedef unsigned char u8;
typedef struct Obj Obj;
typedef struct ItemVTable {
    u8 pad_00[0x18];
    short this_delta_18;
    short slot_1A;
    s32 (*check_1C)(void *, s32);
} ItemVTable;
typedef struct Item {
    u8 kind;
    u8 subtype;
    u8 flags;
    u8 pad_03[2];
    signed char field_05;
    u8 pad_06[2];
    ItemVTable *vtable_08;
} Item;

/* The virtual query supplies an unused object receiver before the inspected item. */
s32 func_8010A944(Obj *object, void *entry)
{
    Item *item = entry;
    s32 result = 0;
    u8 kind = item->kind;
    if (!item->vtable_08->check_1C((u8 *)item + item->vtable_08->this_delta_18, 0x23)) {
        if (kind != 0xA && kind != 0xF && kind != 0x10 && kind != 0x13 &&
            !(item->flags & 0x20))
            result = item->field_05 == -1;
    }
    return result;
}
