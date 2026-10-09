#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 pad0[0x80];
    u8 field_80;
} Obj;

void func_800F8AD4(Obj *obj, s32 value) {
    obj->field_80 = value;
}
