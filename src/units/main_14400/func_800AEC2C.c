#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { u8 pad_00[0x18]; short delta_18, slot_1A; s32 (*test_1C)(void *, s32); } ItemTable;
typedef struct { u8 kind, id, flags_02, pad_03[2]; signed char owner_05; u8 pad_06[2]; ItemTable *vtable_08; u8 flags_0C; } Item;
extern u16 func_800AE710(Item *);
extern u8 func_800AE98C(void *);
extern s32 func_800AC584(u16);
extern const u16 *D_80142F94[];
extern s32 func_8010EEF4(void *);
extern s32 func_8010D6DC(void *);
extern s32 func_80111930(void *);
extern u32 func_80114A50(void *);
extern s32 func_801129CC(void *);
s32 func_800AEC2C(Item *item) {
    u8 kind = item->kind;
    const u16 *prices;
    s32 count;
    s32 index;
    if (~item->owner_05 && kind != 9) return 0;
    switch (kind) {
    case 3: return func_8010EEF4(item);
    case 4: return func_8010D6DC(item);
    case 7: return func_80111930(item);
    case 9: return func_80114A50(item);
    case 14:
        if (item->id == 0xCD) return 500000;
        /* fall through: every other kind-14 item is unsellable, like kind 16 */
    case 16:
        return 0;
    case 17: return func_801129CC(item);
    case 18: return 1;
    }
    prices = D_80142F94[kind];
    if (!prices) return 0;
    if (item->vtable_08->test_1C((u8 *)item + item->vtable_08->delta_18, 30)) {
        count = (u16)func_800AE710(item);
    } else count = 1;
    if (kind == 2 && ((item->flags_0C >> 1) & 1)) index = 9;
    else index = (u8)func_800AE98C(item);
    return func_800AC584(prices[index]) * count;
}
