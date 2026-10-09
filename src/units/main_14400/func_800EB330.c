#include "common.h"
typedef unsigned char u8;
typedef struct { u8 pad_00[0x88]; s32 field_88; } Obj800EB330;
void func_800EB330(Obj800EB330 *self, u8 value) {
    self->field_88 = value * 1000;
}
