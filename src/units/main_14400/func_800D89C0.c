#include "common.h"
typedef unsigned char u8;
typedef signed short s16;
/* Item method table at item+8: +0x30 receiver adjustment, +0x34 name formatter
 * char *(void *self, char *dest) (decided item +0x34 contract, e.g. func_8010DDB0). */
typedef struct { u8 pad0[0x30]; s16 delta_30; s16 index_32; char *(*name_34)(void *self, char *dest); } ItemVTable;
typedef struct { u8 pad0[8]; ItemVTable *vtable_8; } Item;
typedef struct { u8 pad0[0xC4]; Item *item_C4; } Obj800D89C0;

char *func_800D89C0(Obj800D89C0 *obj, char *dest) {
    Item *item = obj->item_C4;
    return item->vtable_8->name_34((u8 *)item + item->vtable_8->delta_30, dest);
}
