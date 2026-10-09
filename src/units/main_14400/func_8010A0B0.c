#include "common.h"

typedef unsigned char u8;

/* Item-set vtable (D_80154438 family): slot +0x24 is the s32 count getter. */
typedef struct {
    u8 pad_00[0x20];
    short adjust_20;
    short reserved_22;
    s32 (*count_24)(void *self);
} VTable;

typedef struct {
    s32 field_00;
    VTable *vtable_04;
    u8 pad_08[0x10];
} Collection;

typedef struct {
    u8 pad_00[0xC4];
    Collection collection_C4;
} Obj;

extern s32 func_800CF47C(void *collection);

/* Clears the embedded collection and reports whether it held anything. */
s32 func_8010A0B0(Obj *obj) {
    Collection *collection = &obj->collection_C4;
    VTable *vtable = collection->vtable_04;
    s32 had_items = vtable->count_24((u8 *)collection + vtable->adjust_20) != 0;

    func_800CF47C(collection);
    return had_items;
}
