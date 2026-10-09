#include "common.h"

/* List vtable slots (D_80154390): +0x24 func_800CE710 element count, +0x3C func_800CE7A0
 * element getter void *(self, u32 index). */
typedef struct {
    short delta;
    short index;
    s32 (*fn)(void *self);
} CountEntry;

typedef struct {
    short delta;
    short index;
    void *(*fn)(void *self, u32 index);
} GetEntry;

typedef struct {
    unsigned char pad0[0x20];
    CountEntry count;
    unsigned char pad28[0x38 - 0x28];
    GetEntry get;
} VTable;

typedef struct {
    void *pool;
    VTable *vt;
} Container;

/* Index of `element` in the container (searching from the end), or -1. */
s32 func_800CD090(void *container, void *element)
{
    Container *c = container;
    s32 i;

    for (i = c->vt->count.fn((char *)c + c->vt->count.delta) - 1; i >= 0; i--) {
        if (c->vt->get.fn((char *)c + c->vt->get.delta, (u32)i) == element) {
            return i;
        }
    }
    return -1;
}
