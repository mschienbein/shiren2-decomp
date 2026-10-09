#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 pad0[0x32];
    u8 field_32;
} Obj;

void func_800E334C(Obj *obj, s32 value) {
    obj->field_32 = value;
}
