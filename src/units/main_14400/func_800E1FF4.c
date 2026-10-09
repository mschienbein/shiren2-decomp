#include "common.h"

typedef unsigned char u8;

typedef struct S {
    u8 pad_00[0x42];
    u8 field_42;
    u8 pad_43[0x72 - 0x43];
    u8 flags_72;
} S;

extern s32 func_800E1CD4(S *, s32);

s32 func_800E1FF4(S *self) {
    s32 result = 0;

    if (!(self->flags_72 & 4)) {
        if (self->field_42 == 0 || func_800E1CD4(self, 0x10) != 0) {
            result = 1;
        }
    }
    return result;
}
