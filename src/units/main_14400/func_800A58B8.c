#include "common.h"
typedef unsigned char u8;
typedef struct Entity { u8 pad_00[9]; u8 field_09; } Entity;
extern u8 func_800A5AA0(Entity *self);
s32 func_800A58B8(Entity *self) {
    s32 value = self->field_09;
    value &= 15;
    value += func_800A5AA0(self);
    value--;
    if (value >= 3) value = 2;
    return value;
}
