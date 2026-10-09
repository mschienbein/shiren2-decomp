#include "common.h"

extern float func_8006A524(float value);
extern const double D_8014C628;

float func_8006A67C(float y, float x) {
    float ratio;
    float result;
    double angle;
    if (x > 0.0f) return func_8006A524(y / x);
    if (x < 0.0f) {
        ratio = y / x;
        if (ratio < 0.0f) ratio = -ratio;
        angle = D_8014C628 - (double)func_8006A524(ratio);
        if (y < 0.0f) angle = -angle;
        return (float)angle;
    }
    if (y == 0.0f) return 0.0f;
    result = 1.5707964f;
    if (y < 0.0f) result = -1.5707964f;
    return result;
}
