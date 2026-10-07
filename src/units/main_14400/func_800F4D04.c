#include "common.h"

typedef unsigned char u8;
typedef struct { u8 pad0[0x29]; u8 field_29; } Obj;

void func_800F4D04(Obj *obj) {
    obj->field_29 = 0;
}
