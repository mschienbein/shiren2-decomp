#include "common.h"
#include "item_records.h"

void func_800D4F04(void *);
void func_800D8FA8(void *object);
/* Storage object: a pool handle (derived vtable D_80154848); only the handle is accessed. */
typedef struct { Handle base; } Obj;
void func_800D4F5C(Obj *obj, s32 flags) {
    obj->base.vtable = &D_80154848;
    func_800D4F04(obj);
    obj->base.vtable = &D_80149DB8;
    if (flags & 1) {
        func_800D8FA8(obj);
    }
}
