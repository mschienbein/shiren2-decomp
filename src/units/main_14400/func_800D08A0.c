#include "common.h"
#include "item_records.h"

typedef unsigned char u8;

/* Item container: pool +0, method table +4, +8 word. */
typedef struct {
    ItemPool *list;
    void **vtable;
    s32 field08;
} Container;

/* D_80154668 +0x3C shares the unsigned-index collection getter contract. */
Item *func_800D08A0(Container *self, u32 index)
{
    u32 slot = func_800D07D8(&self->list, (s32)index);

    if (slot < self->list->capacity) {
        if (func_800AF920(self->list, slot)) {
            return func_800AFD78(self->list, slot);
        }
        return 0;
    }
    return 0;
}
