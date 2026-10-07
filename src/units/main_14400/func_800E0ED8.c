#include "common.h"

typedef short s16;
typedef unsigned short u16;
typedef struct { char pad0[0x30]; u16 field_30; } Obj;

void func_800E0ED8(Obj *obj, s32 delta) {
    s16 value = obj->field_30 + delta;

    if (value < 0) {
        value = 0;
    } else if (value >= 10000) {
        value = 9999;
    }
    obj->field_30 = value;
}
