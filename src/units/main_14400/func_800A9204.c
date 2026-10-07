#include "common.h"

typedef struct { s32 a, b; } Pair;
typedef struct { s32 a, b, c, d; } Quad;
typedef struct {
    s32 field_0;
    Pair *field_4;
    Pair field_8;
    Quad field_10;
} Obj;

Obj *func_800A9204(Obj *o, Quad *q, Pair *p) {
    o->field_4 = p;
    o->field_8 = *p;
    o->field_10 = *q;
    o->field_0 = 0;
    return o;
}
