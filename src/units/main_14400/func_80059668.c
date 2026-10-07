#include "common.h"

typedef float f32;

extern f32 D_80165340;

void func_80059668(f32 angle) {
    if (angle < 0.0f) {
        D_80165340 = 0.0f;
    } else if (angle > 180.0f) {
        D_80165340 = 180.0f;
    } else {
        D_80165340 = angle;
    }
}
