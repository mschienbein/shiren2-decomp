#include "common.h"

typedef struct { char pad0[0xA0]; s32 field_A0; } Obj;

s32 func_80105310(Obj *o) {
    return o->field_A0;
}
