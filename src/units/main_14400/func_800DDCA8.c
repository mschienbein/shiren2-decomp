#include "common.h"

typedef unsigned char u8;
typedef short s16;

/* Eight-byte collection/item link. */
typedef struct { void *collection; void *item; } Link;
/* Link list (vtable at +4, +0x20/+0x24 count) with its record array at +8. */
typedef struct { u8 pad0[0x20]; s16 delta_20; s16 pad_22; s32 (*count_24)(void *self); } ListVTable;
typedef struct { u8 pad0[4]; ListVTable *vtable_4; Link *links_8; } List;
typedef struct { u8 pad0[0xB0]; List list_B0; } Obj;

extern s32 func_800D0248(Link *link);

static inline s32 countLinks(Obj *o) {
    List *l = &o->list_B0;
    return l->vtable_4->count_24((u8 *)l + l->vtable_4->delta_20);
}

static inline s32 is_less(s32 a, s32 b) {
    return a < b;
}

s32 func_800DDCA8(Obj *obj) {
    s32 i = 0;
    s32 n = countLinks(obj);
    List *list = &obj->list_B0;
    s32 failed;

    for (; is_less(i, n); i++) {
        failed = func_800D0248(&list->links_8[i]) ^ 1;
        if (failed) {
            return 0;
        }
    }
    return 1;
}
