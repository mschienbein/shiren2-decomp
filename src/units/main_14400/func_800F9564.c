#include "common.h"

typedef struct { short delta, index; s32 (*call)(void *); } CountEntry;
typedef struct { short delta, index; void *(*call)(void *, u32); } GetEntry;
typedef struct { short delta, index; void (*call)(void *, s32); } DestroyEntry;
typedef struct { char pad0[0x20]; CountEntry count_20; char pad28[0x10]; GetEntry get_38; } CollectionVtable;
typedef struct { void *owner; CollectionVtable *vtbl_4; } Collection;
typedef struct { char pad0[8]; DestroyEntry destroy_8; } ItemVtable;
typedef struct { char pad0[8]; ItemVtable *vtbl_8; u32 amount_C; } Item;
typedef struct { char pad0[0x8C]; Collection *collection_8C; } Object;
typedef struct { s32 x, y; } Pos;
extern u32 D_80159F54[];
extern s32 func_800CD5C0(void *collection, void *item);
extern void func_800AD868(Pos *pos);
extern s32 func_800E0F40(void *obj);
extern s32 func_80049CB4(s32 id, ...);
extern void func_800497F0(s32 id, ...);
extern s32 func_8010E05C(void *obj, s32 add);

s32 func_800F9564(Object *obj, Item *item, Pos *pos) {
    Collection *collection = obj->collection_8C;
    CountEntry *count = &collection->vtbl_4->count_20;
    Item *current;
    GetEntry *get;
    u32 limit;
    if (count->call((char *)collection + count->delta) == 0) {
        func_800CD5C0(obj->collection_8C, item);
        if (pos->y | pos->x) func_800AD868(pos);
        return 1;
    }
    collection = obj->collection_8C;
    get = &collection->vtbl_4->get_38;
    current = get->call((char *)collection + get->delta, 0);
    limit = D_80159F54[(unsigned char)func_800E0F40(obj) - 1];
    if (current->amount_C >= limit) {
        s32 text = func_80049CB4(0xDA, pos);
        func_800497F0(0x123, text);
        return 0;
    }
    func_8010E05C(current, item->amount_C);
    if (limit < current->amount_C) current->amount_C = limit;
    if (pos->y | pos->x) func_800AD868(pos);
    if (item) {
        DestroyEntry *destroy = &item->vtbl_8->destroy_8;
        destroy->call((char *)item + destroy->delta, 3);
    }
    return 1;
}
