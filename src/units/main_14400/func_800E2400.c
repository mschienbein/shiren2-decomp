#include "common.h"

typedef struct { char pad0[0x56]; unsigned char field_56; } Obj;

void func_800E2400(Obj *obj) {
    obj->field_56 = 0;
}
