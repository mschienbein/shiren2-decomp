#include "common.h"

typedef struct {
    s32 field0;
    void *source4;
    s32 field8;
} Obj_800CEAF0;

void func_800CEB54(Obj_800CEAF0 *obj);

/* Constructor-style initializer: returns the object it initialized. */
Obj_800CEAF0 *func_800CEAF0(Obj_800CEAF0 *obj, void *source, s32 value) {
    obj->source4 = source;
    obj->field8 = value;
    func_800CEB54(obj);
    return obj;
}
