#include "common.h"
#include "item_records.h"

typedef unsigned char u8;

/* index is an int: both original callers pass an unmasked int (a0 is the caller's
 * own argument at the 0x800D5418 call; daddu a0,s1 at 0x80122698) and the body
 * narrows it to a byte (andi s0,s0,0xFF at 0x800D53B0). */
s32 func_800D5374(s32 index) {
    Handle handle;
    Handle *self = &handle;
    Item *result;
    self->vtable = &D_80149DB8;
    func_8013687C(&self->pool);
    self->vtable = &D_80149DA8;
    handle.pool = D_80147FD0[(u8)index];
    result = func_800D561C(self, 0);
    self->vtable = &D_80149DA8;
    handle.pool = 0;
    self->vtable = &D_80149DB8;
    return result != 0;
}
