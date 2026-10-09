#include "common.h"
typedef unsigned short u16;
typedef struct Unit800E5248 {
    unsigned char pad_00[0x40];
    u16 flags_40;
} Unit800E5248;
void func_800E1138(Unit800E5248 *self) {
    self->flags_40 &= 0xF0FF;
}
