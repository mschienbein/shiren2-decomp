#include "common.h"
#include "item_records.h"

void func_800D8FA8(void *object);
/* Destructor in D_80149DA8 (+0xC): drop the pool, revert to the base vtable, free on flag 1. */
void func_8013680C(Handle *handle, s32 flags) {
    handle->vtable = &D_80149DA8;
    handle->pool = 0;
    handle->vtable = &D_80149DB8;
    if (flags & 1) {
        func_800D8FA8(handle);
    }
}
