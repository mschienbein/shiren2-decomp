#include "common.h"

typedef unsigned char u8;
typedef struct { u8 pad0[2]; u8 flags_2; } Obj;

s32 func_80113F6C(Obj *obj, s32 kind) {
    if (kind == 0 && !(obj->flags_2 & 4)) {
        return 1;
    }
    if (kind == 1 && (obj->flags_2 & 4)) {
        return 1;
    }
    return kind == 0xB;
}
