#include "common.h"
#include "item_records.h"

typedef unsigned char u8;

extern const u8 D_8015488C[8];
/* Destroy every live sub-kind 0xF4 item of the pool. */
void func_800B0348(ItemPool *pool) {
    s32 i = 0;
    for (;;) {
        Item *item;
        if (i >= pool->capacity) {
            break;
        }
        if (pool->occupancy[i >> 3] & D_8015488C[i & 7]) {
            item = func_800AFD78(pool, i);
            if (item->field01 == 0xF4 && item != 0) {
                item->field08->method0C((u8 *)item + item->field08->delta08, 3);
            }
        }
        i++;
    }
}
