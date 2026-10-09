#include "common.h"
typedef unsigned char u8;
typedef signed char s8;
/* Collection iterator; func_800CEBA0 stores the current element at +0xC. */
typedef struct { s32 cursor_00; void *collection_04; s32 advance_08; void *current_0C; } Iter;
typedef struct { u8 kind_00; u8 pad_01[4]; s8 owner_05; } Item;
Iter *func_800CEB20(Iter *iterator, void *collection);
s32 func_800CEBA0(Iter *iterator);
Item *func_800CEC68(Iter *iterator);
void *func_8011422C(u8 *item);
void func_800CDCC8(void *collection, s8 owner)
{
    Iter iterator;
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
