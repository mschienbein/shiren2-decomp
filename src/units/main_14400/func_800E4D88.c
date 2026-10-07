#include "common.h"

typedef unsigned char u8;
typedef struct { u8 pad0[0x55]; u8 field_55; } Obj;

void func_800E4D88(Obj *obj, s32 value) {
    obj->field_55 = value;
}
