#include "common.h"

#define PI 3.141592654

/* Converts a byte pitch angle (32 = level) to radians. */
float func_80059BF0(s32 value) {
    return ((float)(value & 0xFF) - 32.0) / 32.0 * PI;
}
