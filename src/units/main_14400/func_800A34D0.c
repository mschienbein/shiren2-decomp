#include "common.h"

s32 func_800A34D0(s32 *bounds) {
    s32 valid = bounds[1] <= bounds[3];
    return valid && bounds[0] <= bounds[2];
}
