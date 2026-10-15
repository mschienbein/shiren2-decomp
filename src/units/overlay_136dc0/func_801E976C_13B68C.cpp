#include "common.h"
typedef unsigned char u8;

extern "C" u8 func_801E9F70_13BE90(s32 bit);

extern "C" u8 func_801E976C_13B68C(u32 kind) {
    u8 value = 0;
    u8 low, high;
    s32 high_bit;
    switch (kind) {
    case 0:
        value = func_801E9F70_13BE90(0xC1);
        break;
    case 1:
        low = func_801E9F70_13BE90(0xBF);
        high_bit = 0xC0;
        goto two_bits;
    case 2:
        value = func_801E9F70_13BE90(0xC2);
        break;
    case 3:
        low = func_801E9F70_13BE90(0xC3);
        high_bit = 0xC4;
    two_bits:
        value = low;
        high = func_801E9F70_13BE90(high_bit);
        value += high * 2;
        break;
    case 4:
        value = func_801E9F70_13BE90(0xC5);
        break;
    case 5:
        value = func_801E9F70_13BE90(0xC6);
        break;
    case 6:
        value = func_801E9F70_13BE90(0xC7);
        break;
    case 7:
        value = func_801E9F70_13BE90(0xA4);
        break;
    }
    return value;
}
