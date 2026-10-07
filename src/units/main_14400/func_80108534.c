#include "common.h"

typedef struct { char pad0[0xC0]; s32 field_C0; } Obj;

void func_80108534(Obj *obj) {
    obj->field_C0 = 0;
}
