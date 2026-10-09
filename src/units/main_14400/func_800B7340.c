#include "common.h"
typedef struct { unsigned char kind, variant, field_02, flags, row, field_05, field_06, field_07, mode; signed char coordinates[2], status; } SelectionRecord;
extern SelectionRecord D_80142F18;
typedef struct { unsigned char index, count, previous, field_03, masks[2], field_06, previous_count, field_08; signed char result; unsigned char field_0A; } SelectionSave;
extern SelectionSave D_80142F24;

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

/* Whole 0x26-byte floor record at D_80142EF0 (0x80142EF0..0x80142F15): func_800ABBA0
 * fills it with one func_8006AC30 copy (stride 0x26, count 1); bytes are read with lbu
 * at +0x00..+0x25 and the halfword at +0xE with lhu (func_800AB044). */
typedef struct {
    unsigned char field_00, field_01, field_02, field_03, field_04, field_05, field_06, field_07;
    unsigned char field_08, field_09, field_0A, field_0B, field_0C, field_0D;
    unsigned short field_0E;
    unsigned char field_10, field_11, field_12, field_13, field_14, field_15, field_16, field_17;
    unsigned char field_18, field_19, field_1A, field_1B, field_1C, field_1D, field_1E, field_1F;
    unsigned char field_20, field_21, field_22, field_23, field_24, field_25;
} FloorRecord;
extern FloorRecord D_80142EF0;


extern u8 D_801429C0[16];
s32 func_80046240(void);
Item *func_800AAFC8(void);
s32 func_800AE2A4(void *obj, s32 allowItem, s32 allowWater, void *area);

static __inline__ s32 isBlocked(void) {
    s32 blocked = 0;
    if (!func_80046240() && (((D_80142F18.flags >> 2) & 1) || ((D_80142F18.flags >> 4) & 1))) {
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
    count = D_80142EF0.field_11;
    while (1) {
        if (count == 0) {
            break;
        }
        item = func_800AAFC8();
        if (item != 0) {
            if (item->unk1 == 0xE6) {
                isLeader = (D_80142F18.mode & 0xE0) == 0x20;
                if (!isLeader || claimed) {
                    item->vtable[1].func((u8 *)item + item->vtable[1].delta, 3);
                    continue;
                }
                claimed = 1;
            }
            if (D_80142F24.index != 9) {
                item->unk2 |= 0x10;
            } else {
                item->unk2 &= ~0x10;
            }
            func_800AE2A4(item, 1, 0, D_801429C0);
        }
        count--;
    }
}
