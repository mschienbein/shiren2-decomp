#include "common.h"

typedef struct { char pad0[0x78]; s32 field_78; } Obj;

s32 func_800EB0C8(Obj *o) {
    return o->field_78;
}
