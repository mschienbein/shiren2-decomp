#include "common.h"
#include "collection_iterator.h"

CollectionIterator *func_800CEB20(CollectionIterator *s, void *a) {
    s->owner = a;
    s->reverse = 1;
    func_800CEB54(s);
    return s;
}
