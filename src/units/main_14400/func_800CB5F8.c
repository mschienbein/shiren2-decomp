#include "common.h"

typedef unsigned char u8;

s32 func_800CB5F8(u8 first, u8 base)
{
    if (first == 0) {
        return 0;
    }
    return first + base - 1;
}
