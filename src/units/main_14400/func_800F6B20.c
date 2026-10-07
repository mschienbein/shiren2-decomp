#include "common.h"

typedef struct { char pad0[0x94]; s32 field_94; } Obj;

s32 func_800F6B20(Obj *obj) {
    return obj->field_94;
}
