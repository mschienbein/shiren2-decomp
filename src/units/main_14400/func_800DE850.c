#include "common.h"
typedef unsigned char u8;
typedef struct Item Item;
typedef struct List List;
typedef struct { s32 type; u8 payload[0x1C]; } Message;
typedef struct { u8 pad_00[0x38]; short adjust_38; short pad_3A; s32 (*event_3C)(void *receiver, void *event); } ItemVTable;
struct Item { u8 pad_00[8]; ItemVTable *vtable_08; };
typedef struct { List *owner; Item *item; } Entry;
typedef struct {
    u8 pad_00[0x20]; short adjust_20; short pad_22; s32 (*count_24)(void *self);
    u8 pad_28[0x38]; short adjust_60; short pad_62; s32 (*accept_64)(void *self, void *item, s32 verbose);
} ListVTable;
struct List { void *kind_00; ListVTable *vtable_04; Entry *entries_08; s32 capacity_0C, count_10; };
typedef struct { u8 pad_00[0x98]; short adjust_98; short pad_9A; void *(*list_9C)(void *self); } UnitVTable;
typedef struct { u8 pad_00[0xA]; u8 kind_0A; u8 pad_0B[0x19]; UnitVTable *vtable_24; } Unit;
typedef struct { u8 pad_00[0xB0]; List source_B0; List *destination_C4; s32 moved_C8; } Transfer;
extern Unit *D_801476B8;
extern u32 D_8013960C;
extern u8 D_80143094[];
extern s32 func_800CD4C4(void *list, void *item);
extern void func_800AE518(void *item, void *owner, s32 a, s32 b);
extern s32 func_800CD2BC(void *list, void *item);
extern s32 func_800CD538(void *list, void *item);
extern void *func_800A6D40(Unit *unit);
extern s32 func_80049CB4(s32 id, ...);
extern void func_800CDE78(void *list);
/* D_80159008+9C -> pointer-returning func_800EE07C;
   D_801545E0+24/+64 -> func_800D0414/func_800D04F4. */
s32 func_800DE850(Transfer *transfer) {
    Unit *unit = D_801476B8;
    List *owner;
    List *source;
    u32 index = 0;
    owner = (List *)unit->vtable_24->list_9C((u8 *)unit + unit->vtable_24->adjust_98);
    source = &transfer->source_B0;
    transfer->moved_C8 = 0;
    while (index < (u32)source->vtable_04->count_24((u8 *)source + source->vtable_04->adjust_20)) {
        Entry *entry = &source->entries_08[index];
        Item *item = entry->item;
        Message message;
        s32 skip = 0;
        if (!source->vtable_04->accept_64((u8 *)source + source->vtable_04->adjust_60, item, 0)
            || !func_800CD4C4(transfer->destination_C4, item)) {
            skip = 1;
        }
        if (skip) {
            index++;
            continue;
        }
        if (entry->owner == owner) {
            D_8013960C <<= 1;
            func_800AE518(item, unit, 0, 0);
            D_8013960C >>= 1;
        }
        func_800CD2BC(&transfer->source_B0, item);
        message.type = 0x1C;
        item->vtable_08->event_3C((u8 *)item + item->vtable_08->adjust_38, &message);
        func_800CD538(transfer->destination_C4, item);
        transfer->moved_C8++;
    }
    {
        Unit *other = func_800A6D40(D_801476B8);
        if (transfer->moved_C8 > 0 && other && other->kind_0A == 0x1C) {
            func_80049CB4(0x28, D_801476B8);
            func_80049CB4(0x95, other);
        }
    }
    if (transfer->destination_C4->kind_00 != D_80143094) func_800CDE78(transfer->destination_C4);
    func_80049CB4(2);
    return 1;
}
