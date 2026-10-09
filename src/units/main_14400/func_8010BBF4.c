#include "common.h"

typedef unsigned char u8;

/* Same byte-list layout as func_8010BD00: capacity 0x0E, count 0x0F, up to 16 items. */
typedef struct ByteList8010BBF4 {
    u8 pad_00[0xE];
    u8 capacity_0E;
    u8 count_0F;
    u8 items_10[16];
} ByteList8010BBF4;

/* Copy the item ids into list and return how many there are. */
s32 func_8010BBF4(ByteList8010BBF4 *u, unsigned char *list)
{
    s32 i;

    for (i = 0; i < u->count_0F; i++) {
        *list++ = u->items_10[i];
    }
    return u->count_0F;
}
