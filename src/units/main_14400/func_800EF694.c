#include "common.h"

typedef struct { char pad0[0xB8]; short field_B8; } Obj;

void func_800EF694(Obj *obj, s32 value) {
    obj->field_B8 = value;
}
