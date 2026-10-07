#include "common.h"
typedef unsigned char u8;
u32 func_80068890(u8 x, u8 y);
u8 func_8006872C(u8 x, u8 y, u32 mask, u32 value) {
    u8 bits;
    bits = (func_80068890(x + 1, y) & mask) == value;
    if ((func_80068890(x + 1, y - 1) & mask) == value) bits |= 0x10;
    if ((func_80068890(x, y - 1) & mask) == value) bits |= 0x02;
    if ((func_80068890(x - 1, y - 1) & mask) == value) bits |= 0x20;
    if ((func_80068890(x - 1, y) & mask) == value) bits |= 0x04;
    if ((func_80068890(x - 1, y + 1) & mask) == value) bits |= 0x40;
    if ((func_80068890(x, y + 1) & mask) == value) bits |= 0x08;
    if ((func_80068890(x + 1, y + 1) & mask) == value) bits |= 0x80;
    return bits;
}
