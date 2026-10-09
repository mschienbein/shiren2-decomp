#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct {
    u8 pad0[0x1C];
    u16 flags1C;
    u8 pad1E[0x24];
    u8 flag42;
    u8 pad43[0x2F];
    u8 flags72;
} Obj_800E2044;

s32 func_800E2044(Obj_800E2044 *self) {
    s32 result = 0;

    if (self->flag42 == 0 && !(self->flags72 & 1)) {
        s32 bit = self->flags1C & 2;
        result = bit == 0;
    }
    return result;
}
