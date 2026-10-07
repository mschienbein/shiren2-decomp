#include "common.h"
extern s32 D_80154668[];
extern void func_800D0E38(void *object);
/* The descriptor at +0 is dereferenced by func_800D0E38. */
typedef struct { void *field_00; s32 *field_04; s32 field_08; } Object;
Object *func_800D0650(Object *object, void *descriptor) {
    object->field_04 = D_80154668;
    object->field_00 = descriptor;
    object->field_08 = 1;
    func_800D0E38(object);
    return object;
}
