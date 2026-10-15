#include "common.h"
#include "collection_iterator.h"
typedef unsigned char u8;
typedef struct { u8 field0, field1, field2; } Item;
Item *func_800CF058(void *obj, u8 key) {
    CollectionIterator iter;
    func_800CEB20(&iter, obj);
    for (;;) {
        Item *item;
        s32 found;
        if (!func_800CEBA0(&iter)) return 0;
        item = func_800CEC68(&iter);
        found = (item->field2 & 4) && item->field0 == key;
        if (found) return item;
    }
}
