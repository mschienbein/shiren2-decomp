#include "common.h"
#include "item_records.h"

typedef unsigned char u8;

static inline void construct(Handle *self) {
    self->vtable = &D_80149DB8;
    func_8013687C(&self->pool);
    self->vtable = &D_80149DA8;
}

static inline void destruct(Handle *self) {
    self->vtable = &D_80149DA8;
    self->pool = 0;
    self->vtable = &D_80149DB8;
}

/* Destroy the items of the last pool of D_80147FD0, shift each pool's items one pool up
 * and empty the first. */
void func_800D5160(void) {
    Handle src;
    Handle dst;
    s32 i;
    s32 row;

    construct(&src);
    construct(&dst);
    dst.pool = D_80147FD0[2];
    for (i = 0; ; i++) {
        Item *obj;

        if (i >= 3) {
            break;
        }
        obj = func_800D4A60(&dst, i);
        if (obj != 0) {
            obj->field08->method0C((u8 *)obj + obj->field08->delta08, 3);
        }
    }
    for (row = 2; ; row--) {
        ItemPool *prev;
        ItemPool *next;

        if (row <= 0) {
            break;
        }
        prev = D_80147FD0[row - 1];
        src.pool = prev;
        next = D_80147FD0[row];
        dst.pool = next;
        for (i = 0; ; i++) {
            Item *entry;

            if (i >= 3) {
                break;
            }
            entry = func_800D4A60(&src, i);
            func_800D4A28(&dst, i, entry);
        }
    }
    i = 0;
    do {
        func_800D4A28(&src, i, 0);
    } while (++i < 3);
    destruct(&dst);
    destruct(&src);
}
