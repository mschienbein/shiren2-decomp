#include "common.h"

typedef unsigned char u8;
typedef struct { short delta, index; s32 (*call)(void *, s32, s32, u8, s32); } TestEntry;
typedef struct { short delta, index; s32 (*call)(void *, void *, s32); } RemoveEntry;
typedef struct { char pad0[0x90]; TestEntry test_90; } ActorVtable;
typedef struct { char pad0[0x60]; RemoveEntry remove_60; } CollectionVtable;
typedef struct { void *owner; CollectionVtable *vtbl_4; char pad8[0x14]; } Collection;
typedef struct { char pad0[0x24]; ActorVtable *vtbl_24; char pad28[0x60]; void *field_88; char pad8C[0x38]; Collection collection_C4; } Object;
typedef struct { u8 kind_0; } Item;
extern s32 func_800E2074(void *obj);
extern s32 func_800E1CC4(void *obj, s32 kind);
extern s32 func_800E4454(void *obj);
extern s32 func_8010C38C(void *item);
extern void *func_800CF058(void *target, u8 kind);
/* The selector fills two element pointers; the prior s32* contract carries addresses. */
extern u8 func_800E8B10(void *obj, void **out);
extern s32 func_800ADC90(void *obj, void *pos, void *origin);
extern s32 func_80049CB4(s32 id, ...);
extern s32 func_800CD2BC(void *collection, void *element);
extern s32 func_800CD538(void *collection, void *obj);
extern void func_800AE518(void *obj, void *source, s32 a, s32 b);

static inline s32 test(Object *obj) {
    TestEntry *entry = &obj->vtbl_24->test_90;
    return entry->call((char *)obj + entry->delta, 2, 9, 0, 0);
}

s32 func_8010A5B0(Object *obj, void *context /* unused supplied context */, Item *item) {
    s32 blocked = 0;
    s32 replace;
    Item *old;
    void *choices[2];
    s32 kind;
    if (!func_800E2074(obj) || func_800E1CC4(obj, 0) || !obj->field_88 || test(obj) || func_800E4454(obj)) {
        blocked = 1;
    }
    if (blocked) return 0;
    old = 0;
    replace = 1;
    kind = item->kind_0;
    switch (kind) {
    case 3:
    case 4:
        if (func_8010C38C(item)) replace = 0;
        else old = func_800CF058(&obj->collection_C4, kind);
        break;
    case 6:
        if (func_800E8B10(obj, choices) == 2) old = choices[0];
        break;
    default:
        return 0;
    }
    if (replace && old) {
        Collection *collection = &obj->collection_C4;
        RemoveEntry *entry = &collection->vtbl_4->remove_60;
        s32 failed = entry->call((char *)collection + entry->delta, old, 1) != 1;
        if (failed) replace = 0;
    }
    if (!replace) {
        func_800ADC90(item, obj, obj);
        return 1;
    }
    func_80049CB4(0x12F, 4);
    if (old) {
        func_80049CB4(6);
        func_800CD2BC(&obj->collection_C4, old);
        func_800ADC90(old, obj, obj);
        func_80049CB4(7);
    }
    func_80049CB4(0x16, obj, item);
    func_800CD538(&obj->collection_C4, item);
    func_800AE518(item, obj, 1, 0);
    return 1;
}
