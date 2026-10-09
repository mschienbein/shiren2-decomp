#include "common.h"

s32 func_80083DC4(s32 n) {
    s32 digits;
    s32 limit = 10;
    n = (n >= 0) ? n : -n;
    for (digits = 1; n >= limit; digits++) {
        limit *= 10;
    }
    return digits;
}
