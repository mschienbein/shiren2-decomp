#include "common.h"
#include "item_records.h"

typedef unsigned char u8;

extern void *func_80128404(Item *item);

static inline void Handle_init(Handle *h) {
    h->vtable = &D_80149DB8;
    func_8013687C(&h->pool);
    h->vtable = &D_80149DA8;
}

static inline void Handle_fini(Handle *h) {
    h->vtable = &D_80149DA8;
    h->pool = 0;
    h->vtable = &D_80149DB8;
}

void *func_800D5514(s32 index) {
    u8 slot = index;
    Handle handle;
    Item *item;
    void *result = 0;

    Handle_init(&handle);
    handle.pool = D_80147FD0[slot];
    item = func_800D55F0(&handle, 2);
    if (item != 0) {
        if (item->field01 == 0xF1) {
            result = func_80128404(item);
        }
        item->field08->method0C((char *)item + item->field08->delta08, 3);
    }
    Handle_fini(&handle);
    return result;
}
