#include "common.h"
extern unsigned char D_8015488C[];
static inline void set_bit(unsigned char *base, s32 bit) {
    unsigned char *byte = base + (bit >> 3);
    *byte |= D_8015488C[bit & 7];
}
void func_800A16BC(unsigned char *base, unsigned char value) {
    if ((unsigned char)(value - 0x32) < 0x27) {
        set_bit(base, value - 0x32);
    } else if ((unsigned char)(value - 0x59) < 0x1D) {
        set_bit(base + 5, value - 0x59);
    }
}
