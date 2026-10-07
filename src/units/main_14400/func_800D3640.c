#include "common.h"

typedef struct { char pad0[0x4]; s32 field_4; } Obj;

void func_800D3640(Obj *obj, s32 value) {
    obj->field_4 = value;
}
