#include "common.h"

typedef unsigned char u8;

s32 func_800B0EB0(u8 *a, u8 *b) {
    s32 i;

    for (i = 0; i < 4; i++) {
        if (*a != *b++) {
            return 0;
        }
        if (*a++ == 0) {
            return 1;
        }
    }
    return 1;
}
