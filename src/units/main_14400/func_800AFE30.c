#include "common.h"
#include "item_records.h"

extern void func_800AE648(Item *item);

/* Take a record from the pool, copy source into it and register it; 0 when the pool is full. */
Item *func_800AFE30(ItemPool *pool, Item *source) {
    Item *record = func_800AFA7C(pool);

    if (record == D_80143090) {
        return 0;
    }
    *record = *source;
    func_800AE648(record);
    return record;
}
