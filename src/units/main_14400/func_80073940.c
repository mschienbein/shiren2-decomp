#include "common.h"

typedef float f32;

extern u32 D_801A7208;

f32 func_80073940(f32 value, f32 offset, f32 limit, u32 count, u32 *out) {
    f32 scale;
    f32 rate;

    if (count == 0 || D_801A7208 == (u32)value) {
        rate = 0.0f;
        scale = 216000.0f;
    } else {
        rate = limit;
        if (value == 0.0f) {
            scale = 0.0f;
        } else {
            scale = value / (((f32)D_801A7208 - value) / (f32)count);
            rate = (336.0f - offset) / scale;
        }
    }
    if (limit <= rate) {
        rate = limit;
    } else if (rate >= 1.0f) {
        rate = (s32)rate;
    } else if (rate >= 0.5f) {
        rate = 0.5f;
    } else {
        rate = 0.25f;
    }
    *out = scale;
    return rate;
}
