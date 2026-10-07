#include "common.h"

typedef struct { char pad0[0x6C]; s32 field_6C; } Obj;

void func_800E32B4(Obj *obj) {
    obj->field_6C = 0;
}
