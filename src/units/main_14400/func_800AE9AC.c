#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { u8 pad_00[0x18]; short delta_18, slot_1A; s32 (*test_1C)(void *, s32); } ItemTable;
typedef struct { u8 kind, id, flags_02, pad_03[2]; signed char owner_05; u8 pad_06[2]; ItemTable *vtable_08; u8 flags_0C; } Item;
extern u16 func_800AE710(Item *);
extern u8 func_800AE98C(void *);
extern s32 func_800AC584(u16);
typedef struct { u8 id, pad_01; u16 price; } PriceOverride;
extern const PriceOverride D_80142FE8[];
extern const u16 *D_80142F40[];
extern s32 func_800A99D0(void);
extern s32 func_8010EE80(void *);
extern s32 func_8010D668(void *);
extern u32 func_801118AC(void *);
extern s32 func_80114800(void *, s32);
extern s32 func_8011292C(void *);
extern s32 func_801125BC(void *);
s32 func_800AE9AC(Item *item, s32 mode, s32 discount) {
    u8 kind = item->kind;
    s32 price;
    if (mode >= 2 && kind != 9) {
        s32 owner = item->owner_05;
        if (~owner == 0) return 0;
        if ((mode == 3 && owner != 0) || (mode == 4 && owner != 1) || (mode == 5 && owner != 2)) return 0;
    }
    price = 0;
    if (func_800A99D0()) {
        const PriceOverride *entry;
        if (~item->owner_05 == 0 && mode >= 2) return 0;
        for (entry = D_80142FE8; ; entry++) {
            u8 id;
            if (!entry->price) break;
            id = item->id;
            if (id == entry->id) {
                price = entry->price;
                break;
            }
        }
    }
    if (!price) {
        switch (kind) {
        case 3: price = func_8010EE80(item); break;
        case 4: price = func_8010D668(item); break;
        case 7: price = func_801118AC(item); break;
        case 9: price = func_80114800(item, mode); break;
        case 17: price = func_8011292C(item); break;
        case 5: price = func_801125BC(item); break;
        case 14: price = item->id == 0xCD ? 999999 : 0; break;
        case 18: price = 2; break;
        default: {
            const u16 *prices = D_80142F40[kind];
            s32 index;
            s32 count;
            if (!prices) return 0;
            if (item->vtable_08->test_1C((u8 *)item + item->vtable_08->delta_18, 30)) count = (u16)func_800AE710(item);
            else count = 1;
            if (kind == 2 && ((item->flags_0C >> 1) & 1)) index = 9;
            else index = (u8)func_800AE98C(item);
            price = func_800AC584(prices[index]) * count;
            break;
        }
        }
    }
    return price - (price * discount) / 100;
}
