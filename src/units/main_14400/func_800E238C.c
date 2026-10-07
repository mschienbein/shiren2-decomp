#include "common.h"

typedef unsigned char u8;
typedef struct { u8 pad0[0x53]; u8 field_53; } Obj;

s32 func_800E238C(Obj *obj) {
    return obj->field_53;
}
