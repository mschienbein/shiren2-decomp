#include "common.h"

typedef unsigned char u8;
typedef struct { u8 pad0[8]; u8 field_8; } Obj;
s32 func_80049CB4(s32 id, ...);

void func_800A665C(Obj *obj, u8 *value) {
    u8 v = *value;

    if (v != obj->field_8) {
        obj->field_8 = v;
        func_80049CB4(0x8B, obj);
    }
}
