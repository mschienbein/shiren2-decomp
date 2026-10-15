#include "common.h"
#include "item_records.h"

/* Index of `item` in the pool's storage, or 0xFF for null, the exhaustion sentinel, a
 * record outside the storage or a misaligned address. */
s32 func_800AFD08(ItemPool *pool, Item *item) {
    u32 offset;
    u32 index;
    if (item == 0 || item == D_80143090) {
        return 0xFF;
    }
    /* local-arithmetic-qualification: `item` is not known to point into this pool's
     * storage, so pointer subtraction would be undefined. The original rejects foreign
     * or misaligned records by comparing the 32-bit target addresses (subu at 0x800AFD2C,
     * unsigned divide-by-48 and remainder check through 0x800AFD50); only this local
     * calculation uses integers, the interface and the stored fields stay pointers. */
    offset = (u32)item - (u32)pool->items;
    index = offset / 48U;
    if (offset != index * 48U) {
        return 0xFF;
    }
    offset = index;
    if ((s32)offset < 0 || (s32)offset >= pool->capacity) {
        return 0xFF;
    }
    return offset & 0xFF;
}
