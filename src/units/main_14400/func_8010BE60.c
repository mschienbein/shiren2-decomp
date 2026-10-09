#include "common.h"

typedef unsigned char u8;

/* Same byte-list layout as func_8010BD00: capacity 0x0E, count 0x0F, up to 16 items. */
typedef struct ByteList8010BE60 {
    u8 pad_00[0xE];
    u8 capacity_0E;
    u8 count_0F;
    u8 items_10[16];
} ByteList8010BE60;

/* Remove the item at index, shifting the tail down; returns 1 on success. */
s32 func_8010BE60(ByteList8010BE60 *list, u8 index)
{
    s32 i;

    if (index >= list->count_0F) {
        return 0;
    }
    for (i = index; i < list->count_0F - 1; i++) {
        list->items_10[i] = list->items_10[i + 1];
    }
    list->items_10[i] = 0;
    list->count_0F--;
    return 1;
}
