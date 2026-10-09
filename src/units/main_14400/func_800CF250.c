#include "common.h"

typedef unsigned char u8;
typedef signed short s16;

typedef struct { u8 kind; u8 pad1; u8 flags; } Item;
/* Item-set method table: +0x24 count s32(self), +0x3C get void *(self, u32). */
typedef struct {
    u8 pad0[0x20];
    s16 countAdj; s16 pad22; s32 (*count)(void *self);
    u8 pad28[0x10];
    s16 getAdj; s16 pad3A; void *(*get)(void *self, u32 index);
} ListVTable;
typedef struct { s32 x0; ListVTable *vtable; } List;

/* Collects up to `capacity` kind-6 items whose flag bit 2 is set, clears the
 * rest of `out` and returns the full-width count. */
s32 func_800CF250(List *list, Item **out, s32 capacity)
{
    u8 max = capacity;
    s32 found = 0;
    u32 index = 0;
    s32 i;
    while (1) {
        Item *item;
        s32 ok;
        if (index >= list->vtable->count((u8 *)list + list->vtable->countAdj)) {
            break;
        }
        item = list->vtable->get((u8 *)list + list->vtable->getAdj, index);
        ok = 0;
        if (item != 0 && item->kind == 6) {
            ok = (item->flags & 4) >> 2;
        }
        if (ok) {
            out[found] = item;
            found++;
            if (found >= max) {
                break;
            }
        }
        index++;
    }
    for (i = found; i < max; i++) {
        out[i] = 0;
    }
    return found;
}
