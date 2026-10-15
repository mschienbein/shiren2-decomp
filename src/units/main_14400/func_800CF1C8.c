#include "common.h"
#include "collection_iterator.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { u8 field_0; u8 kind_1; u8 flags_2; } Item800CF1C8;

s32 func_800CF1C8(void *list, u8 kind) {
    CollectionIterator it;
    Item800CF1C8 *item;
    s32 count = 0;
    s32 match;

    func_800CEB20(&it, list);
    while (func_800CEBA0(&it)) {
        item = func_800CEC68(&it);
        match = 0;
        if (item->flags_2 & 4) {
            match = item->kind_1 == kind;
        }
        if (match) {
            count++;
        }
    }
    return count;
}
