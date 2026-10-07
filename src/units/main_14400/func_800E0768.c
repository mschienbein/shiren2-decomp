#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { u8 pad0[0x1E]; u8 field_1E; u8 pad1F[0xB]; u16 field_2A; } Obj;

void func_800E0768(Obj *obj, s32 delta) {
    s32 max = 9999;
    s32 value;

    if (obj->field_1E & 0xC) {
        max = 999;
    }
    value = obj->field_2A + delta;
    if (value > max) {
        value = max;
    } else if (value <= 0) {
        value = 1;
    }
    obj->field_2A = value;
}
