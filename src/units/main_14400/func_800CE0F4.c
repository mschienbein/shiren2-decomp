#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef signed short s16;

typedef struct {
    u8 kind;
    u8 id;
    u8 flags;
} Item;

/* Container vtable (D_80154390): count at 0x24 (func_800CE710, int dispatch), item at 0x3C
 * (func_800CE7A0: void *(self, u32 index)), swap at 0x5C (func_800CE2B0, like func_800D09D8:
 * void (self, s32 a, s32 b)). */
typedef struct {
    u8 pad0[0x20];
    s16 count_delta;
    s16 count_index;
    s32 (*count)(void *self);
    u8 pad28[0x38 - 0x28];
    s16 item_delta;
    s16 item_index;
    void *(*item)(void *self, u32 index);
    u8 pad40[0x58 - 0x40];
    s16 swap_delta;
    s16 swap_index;
    void (*swap)(void *self, s32 a, s32 b);
} ItemSetVTable;

typedef struct {
    void *pool_00;
    ItemSetVTable *vtable;
} ItemSet;

/* Pairs of item ids shuffled together (initialized .data owned here). */
u8 D_80147F68[8] = {
    0x26, 0x27,
    0x2C, 0x2D,
    0x32, 0x3F,
    0x82, 0x83,
};
extern unsigned char D_80147620[];

s32 func_800AD468(u32 bit);
s32 func_800C5954(void *ctx, u16 start, u16 end);

void func_800CE0F4(ItemSet *set) {
    s32 slots[10];
    s32 total;
    u32 pair;

    total = set->vtable->count((u8 *)set + set->vtable->count_delta);
    for (pair = 0; ; pair += 2) {
        u32 found;
        s32 hits;
        s32 i;

        if (pair >= 8) {
            break;
        }
        found = 0;
        hits = 0;
        for (i = 0; ; i++) {
            Item *item;
            s32 usable;
            u8 id;

            if (i >= total) {
                break;
            }
            item = set->vtable->item((u8 *)set + set->vtable->item_delta, (u32)i);
            if (item->flags & 4) {
                continue;
            }
            usable = func_800AD468(item->id) == 1;
            if (!usable) {
                continue;
            }
            id = item->id;
            if (id == D_80147F68[pair] || id == D_80147F68[pair + 1]) {
                slots[found++] = i;
                if (id == D_80147F68[pair + 1]) {
                    hits++;
                }
                if (found >= 10) {
                    break;
                }
            }
        }
        if (hits > 0) {
            s32 last;

            for (i = 0, last = found - 1; ; i++) {
                u16 j;

                if (i >= last) {
                    break;
                }
                j = func_800C5954(D_80147620, i, last);
                if (i != j) {
                    s32 tmp = slots[i];
                    slots[i] = slots[j];
                    slots[j] = tmp;
                    set->vtable->swap((u8 *)set + set->vtable->swap_delta, slots[i], slots[j]);
                }
            }
        }
    }
}
