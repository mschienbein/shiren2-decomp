#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { u8 pad_00[0xD]; signed char bonus_0D; u8 capacity_0E, count_0F; u8 items_10[16]; } Item;
extern u8 D_8015374C[];
extern u8 func_800AC1AC(u8);
extern s32 func_800AC584(u16);
/* ODD_C: separate the byte argument from the full-width table difference;
 * narrowing the returned index in the caller also shapes entry scheduling. */
static inline s32 kindIndex(u8 kind) {
    return kind - D_8015374C[func_800AC1AC(kind)];
}
s32 func_8010C174(Item *item, u8 kind, u16 *base, u8 *scales, u16 *values34, u16 *values1, u16 *values2, u16 *values5, u16 *values8, u16 *values6) {
    u8 index = kindIndex(kind);
    s32 total = func_800AC584(base[index]);
    s32 count = item->count_0F;
    total += total * (item->bonus_0D * scales[index]) / 100;
    for (;;) {
        u8 value;
        s32 offset;
        if (--count == -1) break;
        value = item->items_10[count];
        offset = value - D_8015374C[func_800AC1AC(value)];
        switch (func_800AC1AC(value)) {
        case 3: case 4: total += func_800AC584(values34[(u8)offset]); break;
        case 1: total += func_800AC584(values1[(u8)offset]); break;
        case 2: total += func_800AC584(values2[(u8)offset]); break;
        case 5: total += func_800AC584(values5[(u8)offset]); break;
        case 8: total += func_800AC584(values8[(u8)offset]); break;
        case 6: total += func_800AC584(values6[(u8)offset]); break;
        }
    }
    return total < 0 ? 0 : total;
}
