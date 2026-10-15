#include "common.h"
#include "item_records.h"

typedef unsigned char u8;

/* Move `item` from `source` into a fresh record of `destination`; 0xFF when no record is
 * available, else the new record's index. */
u8 func_800AFFD0(ItemPool *source, Item *item, ItemPool *destination) {
    Item *record = func_800AFA7C(destination);
    u8 slot;

    if (func_800AFBFC(record)) {
        return 0xFF;
    }
    slot = func_800AFD08(destination, record);
    func_800AFEB4(source, item, destination, slot);
    return slot;
}
