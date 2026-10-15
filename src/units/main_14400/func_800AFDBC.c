#include "common.h"
#include "item_records.h"

/* Vacant records in the primary region; the occupancy tested is always pool D_80143094's
 * (the only caller, func_800F27A4, passes that pool). */
s32 func_800AFDBC(ItemPool *pool) {
    s32 i;
    s32 count = 0;
    for (i = 0; i < pool->primaryCount; i++) {
        s32 vacant = func_800AF920(&D_80143094, i) ^ 1;
        if (vacant) {
            count++;
        }
    }
    return count;
}
