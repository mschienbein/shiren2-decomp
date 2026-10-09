#include "common.h"

typedef unsigned char u8;
typedef short s16;

/* Eight-byte collection/item link. */
typedef struct { void *collection; void *item; } Link;
/* Link list (vtable at +4, +0x20/+0x24 count) with its record array at +8. */
typedef struct { u8 pad0[0x20]; s16 delta_20; s16 pad_22; s32 (*count_24)(void *self); } ListVTable;
typedef struct { u8 pad0[4]; ListVTable *vtable_4; Link *links_8; } List;
typedef struct { u8 pad0[0xB0]; List list_B0; } Obj;

extern void *D_801476B8;
extern s32 func_800AEC2C(void *item);
extern void func_800D0348(Link *link);
extern void func_800EB744(void *actor, s32 amount);
extern s32 func_80049CB4(s32 id, ...);

static inline s32 countLinks(Obj *o) {
    List *l = &o->list_B0;
    return l->vtable_4->count_24((u8 *)l + l->vtable_4->delta_20);
}

s32 func_800D9980(Obj *obj) {
    s32 total = 0;
    s32 i = countLinks(obj);
    List *list = &obj->list_B0;

    for (;;) {
        Link *link;
        if (--i < 0) {
            break;
        }
        link = &list->links_8[i];
        total += func_800AEC2C(link->item);
        func_800D0348(link);
    }
    func_800EB744(D_801476B8, total);
    func_80049CB4(0x89, D_801476B8);
    func_80049CB4(2);
    return 0;
}
