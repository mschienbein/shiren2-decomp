#include "common.h"

typedef unsigned char u8;
typedef struct { u8 pad00[0xC]; s32 field0C; } Object;

s32 func_80127858(Object *self, s32 kind) {
    s32 result = 0;
    if (kind == 0xB || (self->field0C >= 2 && kind == 0x1A)) result = 1;
    return result;
}
