#include "common.h"
#include "item_records.h"

typedef unsigned char u8;

/* Moves item `key` of the handle's pool into pool D_80143094 and returns the moved record;
 * 0 without a pool or item. */
Item *func_800D4A60(Handle *handle, s32 wide_key) {
    u8 key = wide_key;
    Item *entry;

    if (handle->pool == 0) {
        return 0;
    }
    if ((func_800AF920(handle->pool, key) ^ 1) != 0) {
        return 0;
    }
    entry = func_800AFD78(handle->pool, key);
    if (entry == 0) {
        return 0;
    }
    return func_800AFD78(&D_80143094, (u8)func_800AFFD0(handle->pool, entry, &D_80143094));
}
