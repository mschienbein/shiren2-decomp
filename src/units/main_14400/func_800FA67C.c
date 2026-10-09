#include "common.h"
typedef unsigned char u8;
typedef struct { u8 pad0[0x20]; short delta20, pad22; s32 (*count24)(void *); } VTable;
typedef struct { void *pool; VTable *vtable; } Container;
typedef struct { u8 pad0[9]; u8 field9; u8 padA[0x82]; Container *field8C; } Object;
typedef struct { u8 type, field1, field2, field3; } Item;
extern s32 func_800F1848(Object *object, Item *item);
s32 func_800FA67C(Object *object, Item *item) {
    return item != 0 && object->field8C->vtable->count24((char *)object->field8C + object->field8C->vtable->delta20) == 0
        && item->type != 0xE && item->field3 == (object->field9 & 0xF) && func_800F1848(object, item);
}
