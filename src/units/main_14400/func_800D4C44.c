#include "common.h"
extern s32 D_80149DB8[];
extern s32 D_80154828[];
extern void func_8013687C(void **pool);
extern void func_800D4C34(void *object);
/* Pool container: pool-record pointer at +0 (cleared by func_8013687C), vtable at +4. */
typedef struct { void *pool; s32 *field_04; } Object;
Object *func_800D4C44(Object *object) {
    object->field_04 = D_80149DB8;
    func_8013687C(&object->pool);
    object->field_04 = D_80154828;
    func_800D4C34(object);
    return object;
}
