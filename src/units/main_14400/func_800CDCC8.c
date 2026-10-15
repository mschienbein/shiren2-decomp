#include "common.h"
#include "collection_iterator.h"
typedef unsigned char u8;
typedef signed char s8;
/* Collection iterator; func_800CEBA0 stores the current element at +0xC. */
typedef struct { u8 kind_00; u8 pad_01[4]; s8 owner_05; } Item;
void *func_8011422C(u8 *item);
void func_800CDCC8(void *collection, s8 owner)
{
    CollectionIterator iterator;
    func_800CEB20(&iterator, collection);
    while (func_800CEBA0(&iterator)) {
        Item *item = func_800CEC68(&iterator);
        if (item->owner_05 == owner) {
            item->owner_05 = -1;
        }
        if (item->kind_00 == 9) {
            func_800CDCC8(func_8011422C((u8 *)item), owner);
        }
    }
}
