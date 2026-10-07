#include "common.h"

typedef struct { s32 w[2]; } Pair;
typedef struct { s32 w[4]; } Quad;
typedef struct {
    s32 field_0;
    s32 field_4;
    Pair pair_8;
    Quad quad_10;
} Obj;

Obj *func_800A9244(Obj *obj, Quad *quad, Pair *pair) {
    obj->field_4 = 0;
    obj->pair_8 = *pair;
    obj->quad_10 = *quad;
    obj->field_0 = 0;
    return obj;
}
