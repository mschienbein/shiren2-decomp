#include "common.h"
s32 func_8006FEA0(u32 value) {
    s32 bits;
    if (value != 0) value--;
    bits = 0;
    while (value != 0) {
        value >>= 1;
        bits++;
    }
    return bits;
}
