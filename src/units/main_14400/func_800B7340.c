#include "common.h"

typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;

typedef struct VEntry { s16 delta; s16 index; void (*func)(void *, s32); } VEntry;
typedef struct {
    u8 unk0;
    u8 unk1;
    s8 unk2;
    u8 pad3[5];
    VEntry *vtable;
} Item;
extern u8 D_80142F1B;
extern u8 D_80142F01;
extern u8 D_80142F20;
extern u8 D_80142F24;
extern u8 D_801429C0[16];
s32 func_80046240(void);
Item *func_800AAFC8(void);
s32 func_800AE2A4(void *obj, s32 allowItem, s32 allowWater, void *area);

static __inline__ s32 isBlocked(void) {
    s32 blocked = 0;
    if (!func_80046240() && (((D_80142F1B >> 2) & 1) || ((D_80142F1B >> 4) & 1))) {
        blocked = 1;
    }
    return blocked;
}
void func_800B7340(void *obj) {
    s32 count;
    s32 claimed;
    s32 isLeader;
    Item *item;

    if (isBlocked()) {
        return;
    }
    claimed = 0;
    count = D_80142F01;
    while (1) {
        if (count == 0) {
            break;
        }
        item = func_800AAFC8();
        if (item != 0) {
            if (item->unk1 == 0xE6) {
                isLeader = (D_80142F20 & 0xE0) == 0x20;
                if (!isLeader || claimed) {
                    item->vtable[1].func((u8 *)item + item->vtable[1].delta, 3);
                    continue;
                }
                claimed = 1;
            }
            if (D_80142F24 != 9) {
                item->unk2 |= 0x10;
            } else {
                item->unk2 &= ~0x10;
            }
            func_800AE2A4(item, 1, 0, D_801429C0);
        }
        count--;
    }
}
