#include "common.h"

typedef unsigned char u8;
typedef short s16;

typedef struct {
    u8 pad00;
    u8 kind01;
    u8 pad02[0xC - 0x2];
    s32 id0C;
} Item;

typedef struct Container Container;
typedef struct {
    s16 delta;
    s16 index;
    s32 (*fn)(void *obj);
} CountEntry;
typedef struct {
    s16 delta;
    s16 index;
    Item *(*fn)(void *obj, u32 index); /* concrete target func_800CE7A0 (D_80154390 +0x3C) */
} GetEntry;
typedef struct {
    u8 pad00[0x20];
    CountEntry count;
    u8 pad28[0x38 - 0x28];
    GetEntry get;
} ContainerVtbl;
struct Container {
    void *list00;
    ContainerVtbl *vtable;
};

typedef struct {
    u8 pad00[0x28];
    Container *slots[3];
} Owner800D1758;

extern void func_800CD364(Container *a, Item *b);

/* Finds the first item of the given kind/id across the three containers and hands it over. */
void func_800D1758(Owner800D1758 *self, u8 kind, s32 id)
{
    s32 i;
    s32 j;
    Container *c;

    for (i = 0;; i++) {
        if (i >= 3) {
            return;
        }
        c = self->slots[i];
        if (c == 0) {
            continue;
        }
        j = c->vtable->count.fn((u8 *)c + c->vtable->count.delta) - 1;
        for (;;) {
            Item *item;

            if (j < 0) {
                break;
            }
            c = self->slots[i];
            item = c->vtable->get.fn((u8 *)c + c->vtable->get.delta, j);
            j--;
            if (item->kind01 == kind && item->id0C == id) {
                func_800CD364(self->slots[i], item);
                return;
            }
        }
    }
}
