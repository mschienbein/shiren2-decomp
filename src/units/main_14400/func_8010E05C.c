#include "common.h"

typedef unsigned char u8;
typedef struct { u8 pad0[0xC]; s32 field_C; } Obj;
s32 func_8010E05C(Obj *o, s32 add) {
    s32 v = o->field_C + add;
    if (v > 999999) {
        o->field_C = 999999;
        return v - 999999;
    }
    if (v <= 0) {
        o->field_C = 1;
        return v;
    }
    o->field_C = v;
    return 0;
}
