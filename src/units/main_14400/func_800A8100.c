#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 pad0[0x1E];
    u8 field_1E;
} Obj800A8100;

void func_800A8100(Obj800A8100 *obj, s32 value) {
    obj->field_1E = value;
}
