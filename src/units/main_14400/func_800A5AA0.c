#include "common.h"
typedef unsigned char u8;
typedef struct Object { u8 pad_00[9]; u8 high_09:4; u8 low_09:4; u8 pad_0A[0x14]; u8 flags_1E; } Object;
extern s32 func_800E1CD4(Object *, s32);
u8 func_800A5AA0(Object *self) {
    return ((self->flags_1E & 0x7C) && func_800E1CD4(self, 15)) ? 1 : self->high_09;
}
