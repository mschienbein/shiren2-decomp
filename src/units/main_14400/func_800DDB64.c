#include "common.h"

typedef unsigned char u8;
typedef short s16;

/* Eight-byte collection/item link. */
typedef struct { void *collection; void *item; } Link;
/* Link list (vtable at +4, +0x38/+0x3C get) with its record array at +8. */
typedef struct { u8 pad0[0x38]; s16 delta_38; s16 pad_3A; void *(*get_3C)(void *self, u32 index); } ListVTable;
typedef struct { u8 pad0[4]; ListVTable *vtable_4; Link *links_8; } List;
typedef struct { u8 pad0[0xB0]; List list_B0; } Obj;

void *func_800AFB80(void *obj);
s32 func_800AF7E0(void *value);
s32 func_800AFD08(void *collection, void *record);

static inline s32 is_less(s32 a, s32 b) {
    return a < b;
}

static inline void *getItem(List *l, u32 index) {
    return l->vtable_4->get_3C((u8 *)l + l->vtable_4->delta_38, index);
}

static inline void *firstItem(Obj *o) {
    List *l = &o->list_B0;
    return l->links_8->item;
}

void func_800DDB64(Obj *obj, u8 *out, s32 count) {
    s32 i = 0;
    void *pool = func_800AFB80(firstItem(obj));
    List *list = &obj->list_B0;

    *out++ = func_800AF7E0(pool);
    while (is_less(i, count)) {
        *out = func_800AFD08(pool, getItem(list, i++));
        out++;
    }
}
