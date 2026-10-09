#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct Unit800E5248 {
    u8 pad_00[0x40];
    u16 flags_40;
} Unit800E5248;

/* Clears the 4-bit field in bits 4..7 of flags_40 (see func_800E10E0). */
void func_800E10FC(Unit800E5248 *self) {
    self->flags_40 &= 0xFF0F;
}
