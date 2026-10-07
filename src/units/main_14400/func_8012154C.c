#include "common.h"

typedef unsigned char u8;
typedef struct { u8 pad0[0x28]; u8 field_28; } Obj;
s32 func_8012154C(Obj *o, s32 kind) {
    if (kind == 0x1D) {
        return o->field_28 != 0;
    }
    return kind == 0xB || kind == 0x16;
}
