#include "common.h"

typedef float f32;

/* Scales the first three rows of a 4x4 float matrix by x, y and z, skipping
 * rows whose factor is exactly 1.0 (compared in double precision). */
void func_80079800(f32 m[4][4], f32 x, f32 y, f32 z) {
    if (x != 1.0) {
        m[0][0] *= x;
        m[0][1] *= x;
        m[0][2] *= x;
        m[0][3] *= x;
    }
    if (y != 1.0) {
        m[1][0] *= y;
        m[1][1] *= y;
        m[1][2] *= y;
        m[1][3] *= y;
    }
    if (z != 1.0) {
        m[2][0] *= z;
        m[2][1] *= z;
        m[2][2] *= z;
        m[2][3] *= z;
    }
}
