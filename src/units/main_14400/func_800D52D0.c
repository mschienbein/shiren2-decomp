#include "common.h"
#include "item_records.h"

/* Store `item` as entry `index` (0..3) of pool D_80147FD0[0]. */
void func_800D52D0(Item *item, s32 index) {
    Handle handle;
    Handle *self;
    Handle *done;

    /* inlined constructor */
    self = &handle;
    self->vtable = &D_80149DB8;
    func_8013687C(&self->pool);
    self->vtable = &D_80149DA8;
    handle.pool = D_80147FD0[0];
    if (item != 0 && index >= 0 && index < 4) {
        func_800D4A28(self, index, item);
    }
    /* inlined destructor */
    done = &handle;
    done->vtable = &D_80149DA8;
    handle.pool = 0;
    done->vtable = &D_80149DB8;
}
