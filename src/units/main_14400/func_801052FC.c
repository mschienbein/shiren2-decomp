#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct {
    u8 pad0[0xA0];
    s32 field_A0;
} Obj;

void func_801052FC(Obj *obj) {
    obj->field_A0 = 0;
}
