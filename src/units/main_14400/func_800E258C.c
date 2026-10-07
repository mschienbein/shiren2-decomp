#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 pad0[0x76];
    u8 field_76;
} Obj_800E258C;

void func_800E258C(Obj_800E258C *obj) {
    obj->field_76 = 0;
}
