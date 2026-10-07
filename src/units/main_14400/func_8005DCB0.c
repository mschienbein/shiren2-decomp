#include "common.h"

s32 func_8005DCB0(s32 arg0) {
    s32 a;
    s32 b;

    if (arg0 >= 0xF0) {
        a = (arg0 & 0xF00) >> 8;
        b = ((arg0 - 0x25) & 0xF00) >> 8;
        return (arg0 & 0xFFF) - a - b - 2;
    }
    return arg0 - 0x80;
}
