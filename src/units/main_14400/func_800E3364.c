#include "common.h"

typedef unsigned char u8;
typedef short s16;

typedef struct {
    u8 pad0[0x2C];
    s16 field_2C;
} Obj_800E3364;

void func_800E3364(Obj_800E3364 *obj, s32 value) {
    obj->field_2C = value;
}
