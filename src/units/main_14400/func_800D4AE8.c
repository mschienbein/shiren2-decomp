#include "common.h"
#include "item_records.h"
typedef unsigned char u8;
void func_800D4AE8(Handle *handle, s32 key) {
    u8 b = key;
    Item *o;
    ItemMethods *m;
    if (handle->pool != 0 && (func_800AF920(handle->pool, b) ^ 1) == 0) {
        o = func_800AFD78(handle->pool, b);
        if (o != 0) {
            m = o->field08;
            m->method0C((u8 *)o + m->delta08, 3);
        }
    }
}
