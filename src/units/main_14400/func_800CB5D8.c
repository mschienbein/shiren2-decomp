#include "common.h"
typedef unsigned char u8;
s32 func_800CB5D8(u8 value, s32 table) {
    if (value == 0) return 0;
    else return value - (u8)table + 1;
}
