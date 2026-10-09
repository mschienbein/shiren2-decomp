#include "common.h"

#define PI 3.141592654

/* Adds the per-axis deltas to three Euler angles and wraps each into [-PI, PI]. */
void func_80059F60(float *angles, float x, float y, float z) {
    s32 i;

    for (i = 2; i != -1; --i) {
        float *angle;

        switch (i) {
        case 0:
        default:
            angle = &angles[0];
            *angle += x;
            break;
        case 1:
            angle = &angles[1];
            *angle += y;
            break;
        case 2:
            angle = &angles[2];
            *angle += z;
            break;
        }
        while (*angle > PI) *angle = (double)*angle - 2 * PI;
        while (*angle < -PI) *angle = (double)*angle + 2 * PI;
    }
}
