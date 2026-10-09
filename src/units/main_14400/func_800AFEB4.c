#include "common.h"

typedef unsigned char u8;
typedef short s16;

typedef struct Msg800AFEB4 {
    s32 kind;
    void *target;
    u8 pad_08[0x10];
    s32 field_18;
} Msg800AFEB4;

typedef struct ItemVTable {
    u8 pad_00[0x38];
    s16 adjust_38;
    s16 pad_3A;
    s32 (*handle_3C)(void *self, Msg800AFEB4 *msg);
} ItemVTable;

/* 0x30-byte item object; the vtable pointer sits at +8 (base vtable D_80153AA0). */
typedef struct Item800AFEB4 {
    u8 kind_00;
    u8 pad_01[0x7];
    ItemVTable *vtable_08;
    u8 pad_0C[0x30 - 0xC];
} Item800AFEB4;

extern u8 D_80143094[];
Item800AFEB4 *func_800AFD78(void *table, u8 index);
extern void func_800AFC6C(void *, void *);
void func_800AFCD8(void *table, u8 id);
void func_800AE648(Item800AFEB4 *obj);
void func_8011541C(Item800AFEB4 *obj);
void func_800AD254(Item800AFEB4 *);

/* Replace slot `index` of `table` with a copy of `source` (or just release it when source is 0). */
void func_800AFEB4(void *owner, Item800AFEB4 *source, void *table, u8 index) {
    Item800AFEB4 *slot = func_800AFD78(table, index);
    Msg800AFEB4 msg;

    if (source == 0) {
        func_800AFC6C(table, slot);
        return;
    }
    msg.kind = 0x1C;
    source->vtable_08->handle_3C((u8 *)source + source->vtable_08->adjust_38, &msg);
    *slot = *source;
    func_800AFC6C(owner, source);
    func_800AFCD8(table, index);
    func_800AE648(slot);
    if (slot->kind_00 == 9) {
        func_8011541C(slot);
    }
    if (table == D_80143094) {
        func_800AD254(slot);
    }
}
