#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 pad0[0x28];
    u8 field_28;
} Obj_80115554;

void func_80115554(Obj_80115554 *obj) {
    obj->field_28 = 0;
}
