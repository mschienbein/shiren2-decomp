#include "common.h"

typedef struct Ent Ent;
typedef struct Iter {
    s32 index;
    void *container;
    s32 reverse;
    Ent *entry;
} Iter;

Ent *func_800CEC68(Iter *iterator)
{
    iterator->index += iterator->reverse ? -1 : 1;
    return iterator->entry;
}
