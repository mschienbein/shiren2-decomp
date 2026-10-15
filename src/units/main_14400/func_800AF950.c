#include "common.h"
#include "item_records.h"
extern const unsigned char D_8015488C[8];
/* Whether the primary region has a vacant record (always while func_800B0164 restores). */
s32 func_800AF950(ItemPool *pool) {
    s32 index;
    if (D_80143104) return 1;
    index = 0;
    while (1) {
        if (index >= pool->primaryCount) break;
        if (!(pool->occupancy[index >> 3] & D_8015488C[index & 7])) return 1;
        index++;
    }
    return 0;
}
