#include "common.h"
#include "item_records.h"
typedef unsigned char u8;
/* index is an int: both original overlay callers pass an unmasked loop counter
 * (daddu a0,s0 at 0x801EB1C4 and 0x801F1070) and the body narrows it to a byte
 * (andi s0,s0,0xFF at 0x800D54C0), as in func_800D5374. */
s32 func_800D5484(s32 index)
{
    Handle handle;
    Handle *self = &handle;
    Handle *done;
    Item *result;
    self->vtable = &D_80149DB8;
    func_8013687C(&self->pool);
    self->vtable = &D_80149DA8;
    handle.pool = D_80147FD0[(u8)index];
    result = func_800D561C(self, 2);
    done = &handle;
    done->vtable = &D_80149DA8;
    handle.pool = 0;
    done->vtable = &D_80149DB8;
    return result != 0;
}
