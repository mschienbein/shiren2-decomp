#include "common.h"

typedef struct { s32 a; s32 b; s32 c; } Triple;
typedef struct { Triple base; s32 field_C; } Obj;

extern Triple D_80138B40;

Obj *func_80044730(Obj *obj) {
    obj->field_C = 1;
    obj->base = D_80138B40;
    obj->base.a = 0x54;
    return obj;
}
