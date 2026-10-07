#include "common.h"

typedef struct { char pad0[0xC]; signed char field_C; } Obj;

s32 func_8010C87C(Obj *obj) {
    return obj->field_C;
}
