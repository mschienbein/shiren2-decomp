#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 pad0[0x9];
    u8 field_9;
} Obj_800A804C;

s32 func_800A804C(Obj_800A804C *obj) {
    return obj->field_9 & 0xF;
}
