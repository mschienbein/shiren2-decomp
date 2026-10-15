#include "common.h"
#include "item_records.h"

/* Whether `item` is unusable: null, the exhaustion sentinel, outside every pool, or a
 * record whose occupancy bit is clear. */
s32 func_800AFBFC(Item *item) {
    ItemPool *pool;

    if (item != 0) {
        if (item == D_80143090) {
            return 1;
        }
        pool = func_800AFB80(item);
        if (pool != 0) {
            return func_800AF920(pool, func_800AFD08(pool, item)) ^ 1;
        }
    }
    return 1;
}
