#include "common.h"
#include "item_records.h"

typedef unsigned char u8;

Item *func_800AFD78(ItemPool *pool, u8 index) {
    if (index == 0xFF) {
        return 0;
    }
    if (index >= pool->capacity) {
        return 0;
    }
    return &pool->items[index];
}
