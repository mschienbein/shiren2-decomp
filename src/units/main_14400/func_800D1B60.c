#include "common.h"
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
s32 func_800D1B60(u8 *arr, s32 row, s32 n) {
    s32 off = row * 4;
    s32 min = arr[off];
    s32 best = 0;
    s32 i;
    for (i = 0; i < n; i++) {
        if (arr[i + off] < min) {
            min = arr[i + off];
            best = i;
        }
    }
    return best;
}
