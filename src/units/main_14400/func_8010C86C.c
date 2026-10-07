#include "common.h"

typedef signed char s8;
typedef struct { s8 pad0[0xD]; s8 field_D; } Obj;

s32 func_8010C86C(Obj *obj) {
    return obj->field_D;
}
