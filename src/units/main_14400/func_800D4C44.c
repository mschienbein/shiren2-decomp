#include "common.h"
#include "item_records.h"
extern void func_800D4C34(void *object);
/* Floor-item grid: a pool handle (derived vtable D_80154828) whose further fields
 * func_800D4C34 resets; only the handle is accessed here. */
typedef struct { Handle base; } Object;
Object *func_800D4C44(Object *object) {
    object->base.vtable = &D_80149DB8;
    func_8013687C(&object->base.pool);
    object->base.vtable = &D_80154828;
    func_800D4C34(object);
    return object;
}
