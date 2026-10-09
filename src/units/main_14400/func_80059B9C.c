#include "common.h"

#define PI 3.141592654

/* Converts a byte angle (0..255, counted down from 256) to radians. */
float func_80059B9C(s32 value) {
    return (256.0 - (float)(value & 0xFF)) / 128.0 * PI;
}
