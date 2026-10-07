#include "common.h"

extern float func_80032360(float);
extern float func_80027C60(float);

float func_8006A4F0(float angle) {
    float numerator = func_80032360(angle);
    return numerator / func_80027C60(angle);
}
