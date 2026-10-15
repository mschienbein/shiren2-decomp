#include "common.h"
#include "item_records.h"

typedef unsigned char u8;

/* Remove seal `index` of a kind-3/4 item, shifting the tail down; returns 1 on success. */
s32 func_8010BE60(Item *item, u8 index)
{
    s32 i;

    if (index >= item->payload.seals.count0F) {
        return 0;
    }
    for (i = index; i < item->payload.seals.count0F - 1; i++) {
        item->payload.seals.contents10[i] = item->payload.seals.contents10[i + 1];
    }
    item->payload.seals.contents10[i] = 0;
    item->payload.seals.count0F--;
    return 1;
}
