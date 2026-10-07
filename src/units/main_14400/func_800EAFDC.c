#include "common.h"

typedef struct { char pad0[0x78]; s32 field_78; } Obj;

s32 func_800EAFDC(Obj *obj) {
    return obj->field_78;
}
