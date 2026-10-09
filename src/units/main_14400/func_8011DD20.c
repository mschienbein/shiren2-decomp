#include "common.h"
typedef unsigned char u8;
typedef struct { u8 pad_00[8]; short adjustment_08; short reserved_0A; void (*destroy_0C)(void *item, s32 flags); } ItemVTable;
typedef struct { u8 field_00; u8 kind_01; u8 pad_02[6]; ItemVTable *vtable_08; } Item;
typedef struct { u8 pad_00[0x10]; u8 kind_10; u8 flag_11; } Object8011DD20;
s32 func_800ACEB4(Item *item);
void func_800D3650(void *item);
/* The virtual slot supplies context even though this implementation ignores it. */
s32 func_8011DD20(Object8011DD20 *object, void *context, Item *item)
{
    if (object->kind_10 == 0) {
        object->kind_10 = item->kind_01;
        object->flag_11 = func_800ACEB4(item) != 0;
        func_800D3650(item);
        if (item != 0) {
            item->vtable_08->destroy_0C((u8 *)item + item->vtable_08->adjustment_08, 3);
        }
        return 1;
    }
    return 0;
}
