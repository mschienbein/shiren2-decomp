#include "common.h"
#include "collection_iterator.h"


/* Constructor-style initializer: returns the object it initialized. */
CollectionIterator *func_800CEAF0(CollectionIterator *obj, void *source, s32 value) {
    obj->owner = source;
    obj->reverse = value;
    func_800CEB54(obj);
    return obj;
}
