#include "common.h"

typedef struct Record {
    s32 value_00;
    unsigned char pad_04[0x10];
} Record;
typedef struct Collection {
    s32 count;
    Record *records;
} Collection;
typedef struct Index {
    s32 index;
    Collection *collection;
} Index;

s32 func_80091B30(Index *iterator)
{
    Collection *collection = iterator->collection;
    if (collection && iterator->index >= 0 && iterator->index < collection->count)
        return collection->records[iterator->index].value_00;
    return -1;
}
