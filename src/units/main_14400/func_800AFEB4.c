#include "common.h"
#include "item_records.h"

typedef unsigned char u8;

/* Kind-0x1C message: the handler-message base, the target word and the later words. */
typedef struct {
    ItemMessage base;
    void *target;
    u8 pad_08[0x10];
    s32 field_18;
} Msg800AFEB4;

void func_800AE648(Item *item);
void func_8011541C(Item *item);
void func_800AD254(Item *item);

/* Replace record `index` of `pool` with a copy of `source` taken from `owner` (or just
 * release the record when source is 0). */
void func_800AFEB4(ItemPool *owner, Item *source, ItemPool *pool, u8 index) {
    Item *slot = func_800AFD78(pool, index);
    Msg800AFEB4 msg;

    if (source == 0) {
        func_800AFC6C(pool, slot);
        return;
    }
    msg.base.kind = 0x1C;
    source->field08->handle3C((u8 *)source + source->field08->delta38, &msg.base);
    *slot = *source;
    func_800AFC6C(owner, source);
    func_800AFCD8(pool, index);
    func_800AE648(slot);
    if (slot->field00 == 9) {
        func_8011541C(slot);
    }
    if (pool == &D_80143094) {
        func_800AD254(slot);
    }
}
