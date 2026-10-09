#include "common.h"
typedef struct { s32 field0; s32 field4; void *field8; } Object;
extern unsigned char D_801604D0[];
extern Object *func_80115690(Object *, s32);
Object *func_80126C10(Object *obj) { func_80115690(obj, 0xE8); obj->field8 = D_801604D0; return obj; }
