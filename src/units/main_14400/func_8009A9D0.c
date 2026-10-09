#include "common.h"
typedef unsigned char u8;
/* Item-set vtable (vptr at +4): +0x24 count, +0x3C getter (decided item-set contracts). */
typedef struct {
    u8 pad_00[0x20];
    short adjust_20;
    short pad_22;
    s32 (*count_24)(void *self);
    u8 pad_28[0x10];
    short adjust_38;
    short pad_3A;
    void *(*get_3C)(void *self, u32 index);
} ListVTable;
typedef struct { void *pool_00; ListVTable *vtable_04; } List;
typedef struct { u8 pad_00[0x98]; short adjust_98; short pad_9A; List *(*list_9C)(void *self); } ActorVTable;
typedef struct { u8 pad_00[0x24]; ActorVTable *vtable_24; } Actor;
/* Item record: kind at +0, flag byte at +2 (bit 2 = selectable). */
typedef struct { u8 kind; u8 pad_1; u8 flags; } Item;
/* Menu object: at most two selected item indices, then their count. */
typedef struct { u8 pad_00[0x50]; u32 indices_50[2]; s32 count_58; } Object;
/* 16-byte layout descriptor passed to func_8009543C (columns = 2). */
typedef struct { s32 columns; s32 field_4; s32 field_8; s32 field_C; } Layout;
extern Actor *D_801476B8;
extern Layout D_80139074;
extern void func_8009543C(Object *obj, Layout *desc);

static inline s32 is_selectable(Item *item) {
    s32 flag = item->flags & 4;
    return flag != 0;
}

void func_8009A9D0(Object *self) {
    Actor *actor = D_801476B8;
    u32 i = 0;
    List *list = actor->vtable_24->list_9C((u8 *)actor + actor->vtable_24->adjust_98);
    Item *item;
    s32 ok;
    self->count_58 = 0;
    for (i = 0; i < list->vtable_04->count_24((u8 *)list + list->vtable_04->adjust_20); i++) {
        item = list->vtable_04->get_3C((u8 *)list + list->vtable_04->adjust_38, i);
        ok = item != 0 && item->kind == 6 && is_selectable(item);
        if (!ok) continue;
        self->indices_50[self->count_58] = i;
        self->count_58++;
        if (self->count_58 >= 2) break;
    }
    func_8009543C(self, &D_80139074);
}
