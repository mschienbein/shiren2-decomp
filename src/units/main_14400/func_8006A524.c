#include "common.h"

extern const double D_8014C5E0, D_8014C5E8, D_8014C5F0;
extern const double D_8014C5F8, D_8014C600, D_8014C608, D_8014C610;
extern const double D_8014C618, D_8014C620;

float func_8006A524(float value) {
    s32 quadrant;
    double square;
    float angle;
    if (value == 0.0f) return 0.0f;
    if ((double)value > 1.0) {
        quadrant = 1;
        value = 1.0f / value;
    } else if ((double)value < -1.0) {
        quadrant = 2;
        value = 1.0f / value;
    } else {
        quadrant = 0;
    }
    square = (double)(value * value);
    angle = (float)((double)value *
        (square / (square / (square / (square / (square /
        (square / D_8014C5E0 + D_8014C5E8) + D_8014C5F0) + D_8014C5F8)
        + D_8014C600) + D_8014C608) + D_8014C610));
    switch (quadrant) {
    case 0: /* ODD_C: quadrant 0 is a real value (|value| <= 1, set above) whose
             * result is the plain arctangent; listing it gives GCC the dense
             * 0..2 case set, so it emits the ==1 / <2 / ==2 compare tree of
             * the original instead of two equality tests (if/switch spellings
             * without it compile 8-20 bytes short). */
        break;
    case 1: return D_8014C618 - angle;
    case 2: return D_8014C620 - angle;
    }
    return angle;
}
