#include "common.h"
#include "item_records.h"
extern void func_800D8FA8(void *object);
/* Base destructor in D_80149DB8 (+0xC). */
void func_8013684C(Handle *handle, s32 flags) {
    handle->vtable = &D_80149DB8;
    if (flags & 1) func_800D8FA8(handle);
}
