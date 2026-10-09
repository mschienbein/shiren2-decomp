#include "common.h"
typedef struct { s32 field0; void *field4; } Object;
extern s32 func_800D8FF0(void *obj);
s32 func_800A1890(Object *object) { return func_800D8FF0(object->field4); }
