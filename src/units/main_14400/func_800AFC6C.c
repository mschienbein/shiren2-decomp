#include "common.h"
#include "item_records.h"

typedef unsigned char u8;

void func_800AFC6C(ItemPool *pool, Item *item) {
    u8 id = func_800AFD08(pool, item);

    if (id != 0xFF) {
        func_800AFCA8(pool, id);
    }
}
