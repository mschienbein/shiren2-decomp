#include "common.h"

s32 func_800D194C(s32 value) {
    s32 rank;

    if (value >= 100) {
        rank = 3;
    } else if (value >= 70) {
        rank = 2;
    } else if (value >= 51) {
        rank = 1;
    } else if (value > 0) {
        rank = 0;
    } else {
        rank = -1;
    }
    return rank;
}
