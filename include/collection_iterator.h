#ifndef SHIREN2_COLLECTION_ITERATOR_H
#define SHIREN2_COLLECTION_ITERATOR_H

#include "common.h"

/* Collection cursor: index/owner/reverse/current at +0/+4/+8/+12 on the
 * target ABI (16 bytes, alignment 4; measurement remains a separate gate).
 * CEAF0/CEB20 set owner/reverse and call CEB54 to reset index. Only CEBA0
 * writes current; construction/reset leave it untouched. Call CEC68 only
 * after CEBA0 succeeds. Foreign owner/item storage and lifetimes are separate
 * from this record's actual declared subobjects. */
typedef struct CollectionIterator {
    s32 index;
    void *owner;
    s32 reverse;
    void *current;
} CollectionIterator;

#ifdef __cplusplus
extern "C" {
#endif
CollectionIterator *func_800CEAF0(CollectionIterator *iterator, void *owner, s32 reverse);
CollectionIterator *func_800CEB20(CollectionIterator *iterator, void *owner);
void func_800CEB54(CollectionIterator *iterator);
s32 func_800CEBA0(CollectionIterator *iterator);
void *func_800CEC68(CollectionIterator *iterator);
#ifdef __cplusplus
}
#endif

#endif
