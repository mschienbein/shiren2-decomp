#include "common.h"
extern const double D_8014C280;
extern float func_80032360(float);
float func_8005C3FC(float start, float end, float phase) {
    float distance = end - start;
    return distance * func_80032360((float)((double)phase * D_8014C280)) + start;
}
