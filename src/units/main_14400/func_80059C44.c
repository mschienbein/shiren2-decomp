#include "common.h"

#define PI 3.141592654

/* Wraps a heading into [0, 2*PI] and returns it as a rounded byte angle
 * counted down from 256. */
s32 func_80059C44(float angle) {
    s32 digit;

    for (;;) {
        if (angle > 2 * PI) angle = (double)angle - 2 * PI;
        else if (angle < 0.0f) angle = (double)angle + 2 * PI;
        else break;
    }
    angle = 256.0 - ((double)angle / PI) * 128.0;
    if ((double)angle >= 256.0) {
        angle = 0.0f;
    } else {
        digit = (s32)(angle * 10.0f) % 10;
        if (digit >= 5) angle = (double)angle + 0.1;
    }
    return (u32)angle & 0xFF;
}
