#include "common.h"
#include "item_records.h"

void func_800D4A28(Handle *handle, s32 index, Item *item) {
    if (handle->pool != 0) {
        func_800AFEB4(&D_80143094, item, handle->pool, index);
    }
}
