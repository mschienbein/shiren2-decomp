#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

u16 func_800CA560(u16 value) {
    u16 mix = (value >> 6) ^ (value >> 13);

    return (value << 7) | (mix & 0x7F);
}
