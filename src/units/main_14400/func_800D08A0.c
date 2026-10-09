#include "common.h"

typedef unsigned char u8;

typedef struct Item Item;

typedef struct {
    u8 pad00[8];
    u32 count;
} List;

typedef struct {
    List *list;
    void **vtable;
    s32 field08;
} Container;

extern u32 func_800D07D8(Container *owner, s32 index);
extern s32 func_800AF920(List *s, unsigned char n);
extern Item *func_800AFD78(List *table, u8 index);

/* D_80154668 +0x3C shares the unsigned-index collection getter contract. */
Item *func_800D08A0(Container *self, u32 index)
{
    u32 slot = func_800D07D8(self, (s32)index);

    if (slot < self->list->count) {
        if (func_800AF920(self->list, slot)) {
            return func_800AFD78(self->list, slot);
        }
        return 0;
    }
    return 0;
}
