#include "common.h"
extern const double D_8014C288;
extern float func_80032360(float);
float func_8005C454(float start, float amplitude, float phase) {
    return amplitude * func_80032360((float)((double)phase * D_8014C288)) + start;
}
