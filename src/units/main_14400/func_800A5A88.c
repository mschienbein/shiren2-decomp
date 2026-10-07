#include "common.h"

typedef unsigned char u8;

typedef struct { u8 pad0[0x9]; u8 field_9; } Obj800A5A88;

void func_800A5A88(Obj800A5A88 *obj, s32 value) {
    obj->field_9 = (obj->field_9 & 0xF) | (value << 4);
}
