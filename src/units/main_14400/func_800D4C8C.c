#include "common.h"
#include "item_records.h"

/* Floor-item grid: a pool handle (derived vtable D_80154828); only the handle is accessed. */
typedef struct {
    Handle base;
} Obj_800D4C8C;

extern void func_800D4C34(Obj_800D4C8C *obj);
extern void func_800D8FA8(void *object);

void func_800D4C8C(Obj_800D4C8C *obj, s32 flags) {
    obj->base.vtable = &D_80154828;
    func_800D4C34(obj);
    obj->base.vtable = &D_80149DB8;
    if (flags & 1) {
        func_800D8FA8(obj);
    }
}
