#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;
typedef float f32;

/* Partial item view shared with func_8010BEC4 (count at 0x0F, codes at 0x10). */
typedef struct {
    u8 pad0[0xF];
    u8 count;
    u8 items[16];
} Item8010BD3C;

/* Removes the last occurrence of code from the item's code list. */
s32 func_8010BD3C(Item8010BD3C *item, s32 code) {
    s32 i;
    s32 j;

    i = item->count;
    while (--i != -1) {
        if (item->items[i] == (u8)code) {
            for (j = i; j < item->count - 1; j++) {
                item->items[j] = item->items[j + 1];
            }
            item->items[j] = 0;
            item->count--;
            return 1;
        }
    }
    return 0;
}
