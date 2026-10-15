#include "common.h"
#include "item_records.h"
typedef unsigned char u8;
/* The supplied size ABI slot is unused when recycling this fixed-pool object. */
void *func_800AC5F4(s32 size, Item *obj) {
    u8 id = func_800AFD08(&D_80143094, obj);
    if (id != 0xFF) {
        if (obj) obj->field08->method0C((unsigned char *)obj + obj->field08->delta08, 3);
        func_800AFCD8(&D_80143094, id);
    }
    return obj;
}
