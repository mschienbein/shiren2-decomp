#include "common.h"

s32 func_800B58E4(s32 x1, s32 y1, s32 x2, s32 y2) {
    s32 same = 0;
    if (x1 == x2 || y1 == y2) same = 1;
    return same;
}
