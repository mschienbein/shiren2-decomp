#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct Unit800E10E0 {
    u8 pad_00[0x40];
    u16 flags_40;
} Unit800E10E0;

/* Stores a 4-bit value into bits 4..7 of flags_40 (func_800E10FC clears it). */
void func_800E10E0(Unit800E10E0 *self, u8 value) {
    self->flags_40 = (self->flags_40 & 0xFF0F) | (value << 4);
}
