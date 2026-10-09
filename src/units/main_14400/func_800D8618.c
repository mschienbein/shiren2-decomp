#include "common.h"

typedef unsigned char u8;
typedef short s16;

/* Item-collection method table (vtable at +4): +0x24 count(self), +0x3C get(self, index). */
typedef struct {
    u8 pad_00[0x20];
    s16 adjust_20;
    s16 pad_22;
    s32 (*count_24)(void *self);
    u8 pad_28[0x10];
    s16 adjust_38;
    s16 pad_3A;
    void *(*get_3C)(void *self, u32 index);
} ListVTable;

typedef struct {
    u8 pad_00[4];
    ListVTable *vtable_4;
} List;

typedef struct {
    u8 pad_00[0xD];
    u8 kind_D;
    u8 arg_E;
} Item;

/* Group: its own item collection at +0xC. */
typedef struct {
    u8 pad_00[0xC];
    List items_C;
} Group;

typedef struct {
    u8 pad_00[0xA8];
    List groups_A8;
    u8 pad_B0[0xC];
    s32 count_BC;
    Group *group_C0;
    Item *item_C4;
    s32 entry_C8;
    u8 pad_CC[4];
    s32 state_D0;
} Obj;

extern s32 func_800D7D84(u8 kind, u8 arg);
extern s32 func_800D8040(s32 entry);

s32 func_800D8618(Obj *obj) {
    List *groups = &obj->groups_A8;
    List *items;

    if (obj->count_BC >= groups->vtable_4->count_24((u8 *)groups + groups->vtable_4->adjust_20)) {
        return 2;
    }
    obj->group_C0 = groups->vtable_4->get_3C((u8 *)groups + groups->vtable_4->adjust_38, obj->count_BC);
    items = &obj->group_C0->items_C;
    obj->item_C4 = items->vtable_4->get_3C((u8 *)items + items->vtable_4->adjust_38, 0);
    obj->entry_C8 = func_800D7D84(obj->item_C4->kind_D, obj->item_C4->arg_E);
    obj->count_BC++;
    if (func_800D8040(obj->entry_C8) != 0) {
        obj->state_D0 = 2;
        if (obj->item_C4->kind_D == 0x29) {
            return -8;
        }
        return -2;
    }
    obj->state_D0 = 1;
    return 1;
}
