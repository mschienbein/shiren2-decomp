#include "common.h"

typedef unsigned short u16;
s32 func_80090144(u16 value) {
    s32 result = 5;
    if (value >= 1 && value <= 4) result = 1;
    if (value >= 5 && value <= 8) result = 3;
    if (value >= 9 && value <= 15) result = 3;
    if (value == 16) result = 4;
    if (value >= 17 && value <= 32) result = 5;
    if (value >= 33 && value <= 64) result = 6;
    if (value >= 65 && value <= 128) result = 7;
    if (value >= 129 && value <= 256) result = 8;
    return result;
}
