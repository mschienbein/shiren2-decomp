#include "common.h"
#include "item_records.h"
ItemPool *func_800AF890(ItemPool *pool, Item *storage, unsigned char *occupancy, s32 capacity) {
    pool->items = storage;
    pool->occupancy = occupancy;
    pool->capacity = capacity;
    func_800AF910(pool, 0);
    func_800AF8D0(pool);
    return pool;
}
