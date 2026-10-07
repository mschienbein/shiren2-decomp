#include "common.h"

typedef unsigned char u8;

s32 func_80083D8C(void *left, void *right, u32 count) {
    u8 *a = left;
    u8 *b = right;

    while (count-- > 0) {
        if (*a != *b) {
            return *a - *b;
        }
        a++;
        b++;
    }
    return 0;
}
