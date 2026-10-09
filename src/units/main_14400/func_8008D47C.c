#include "common.h"
typedef struct { s32 field0, field4, field8; void *fieldC; } Object;
extern void func_80091544(void *item);
void func_8008D47C(Object *object) {
    if (object->fieldC != 0) {
        func_80091544(object->fieldC);
        object->fieldC = 0;
    }
    object->field0 = 0;
}
