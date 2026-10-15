#include "common.h"
#include "collection_iterator.h"


void *func_800CEC68(CollectionIterator *iterator)
{
    iterator->index += iterator->reverse ? -1 : 1;
    return iterator->current;
}
