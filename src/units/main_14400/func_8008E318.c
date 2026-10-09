#include "common.h"

typedef struct { s32 field00; s32 pad04[2]; void *field0C; } Object;
extern void func_80091544(void *item);

void func_8008E318(Object *object)
{
    if (object->field0C != 0) {
        func_80091544(object->field0C);
        object->field0C = 0;
    }
    object->field00 = 0;
}
