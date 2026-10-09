#include "common.h"

#define PI 3.141592654

/* Wraps a pitch angle into [0, 2*PI] and returns it as a rounded byte (32 = level). */
s32 func_80059D8C(float angle) {
    s32 digit;

    for (;;) {
        if (angle > 2 * PI) angle = (double)angle - 2 * PI;
        else if (angle < 0.0f) angle = (double)angle + 2 * PI;
        else break;
    }
    angle = ((double)angle / PI) * 32.0 + 32.0;
    digit = (s32)(angle * 10.0f) % 10;
    if (digit >= 5) angle = (double)angle + 0.1;
    if ((double)angle >= 64.0) angle = (double)angle - 64.0;
    return (u32)angle & 0xFF;
}
