#include "common.h"

typedef unsigned char u8;
typedef struct { u8 pad00[0x28]; u8 field28; } Object;

s32 func_80114CEC(Object *self, s32 kind) {
    s32 result;
    if (kind == 0x1D) return self->field28 != 0;
    result = 0;
    if (kind == 0x11 || kind == 0xE || kind == 0xB || kind == 0x16) result = 1;
    return result;
}
