#include "common.h"
typedef unsigned char u8;
typedef struct { u8 pad_00[0x60]; short delta_60; short index_62; s32 (*insert_64)(void *, void *, s32); } VTable;
typedef struct { void *pool; VTable *vtable; } Item;
typedef struct { Item *item; void *other; } Slot;
typedef struct { void *pool; void *vtable; Slot *slots; } List;
extern s32 func_800CD090(void *, void *);
s32 func_800D04F4(void *self, void *element, s32 notify) {
    List *list = self;
    Item *item = list->slots[func_800CD090(list, element)].item;
    VTable *vt = item->vtable;
    /* The original epilogue preserves this delegated insertion result in v0. */
    return vt->insert_64((u8 *)item + vt->delta_60, element, notify);
}
