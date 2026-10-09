#include "common.h"
typedef unsigned char u8;
typedef struct Flags { u32 bits; } Flags;
typedef struct ItemTable {
    u8 pad_00[0x18];
    short adjustment_18, pad_1A;
    s32 (*method_1C)(void *self, s32 kind);
} ItemTable;
typedef struct Item {
    u8 kind, field_01, flags_02, pad_03[2];
    signed char field_05;
    u8 pad_06[2];
    const ItemTable *vtable_08;
} Item;
typedef struct Unit {
    u8 pad_00[0x20];
    Flags flags_20;
    u8 pad_24[0xA8];
    u8 inventory_CC[0x34];
    Item *held_100;
} Unit;
extern s32 D_80148090;
extern void *func_800E215C(void *obj);
extern s32 func_800CD4C4(void *, void *);
static __inline__ Flags *copy_flags(Flags *out, Unit *unit) {
    *out = unit->flags_20;
    return out;
}
void func_800EBFE4(Unit *u) {
    Item *item;
    s32 disallowed, special, mode_check, inventory_check, flags_check;
    Flags flags;
    u->held_100 = 0;
    mode_check = D_80148090 != 0 && D_80148090 != 3;
    if (mode_check) return;
    item = func_800E215C(u);
    if (!item) return;
    if (item->kind != 14) {
        disallowed = 0;
        if (item->vtable_08->method_1C((u8 *)item + item->vtable_08->adjustment_18, 35) ||
            (item->flags_02 & 0x20) || ~item->field_05) disallowed = 1;
        if (disallowed) return;
        inventory_check = func_800CD4C4(u->inventory_CC, item);
        inventory_check ^= 1;
        if (inventory_check) return;
        special = item->kind == 16 || item->kind == 10;
        if (special) {
            copy_flags(&flags, u);
            flags_check = (flags.bits >> 23) & 1;
            flags_check ^= 1;
            if (flags_check) return;
        }
    }
    u->held_100 = item;
}
