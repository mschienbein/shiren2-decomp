#include "common.h"

typedef struct { char pad0[0x43]; unsigned char field_43; } Obj;

void func_800E3314(Obj *obj, s32 value) {
    obj->field_43 = value;
}
