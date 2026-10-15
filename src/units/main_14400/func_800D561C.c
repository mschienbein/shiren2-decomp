#include "common.h"
#include "item_records.h"

s32 func_800AC670(void *ptr);

/* The handle's pool is one of D_80147FD0 (stored by every caller). */
Item *func_800D561C(Handle *handle, s32 index) {
    Item *item = func_800AFD78(handle->pool, index);
    if (item == 0 || func_800AC670(item) != 0) {
        return 0;
    }
    return item;
}
