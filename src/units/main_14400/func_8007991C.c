#include "common.h"
extern float func_80032360(float);
extern float func_80027C60(float);
void func_8007991C(float *matrix, float x, float y, float z) {
    float sx = func_80032360(x);
    float cx = func_80027C60(x);
    float sy = func_80032360(y);
    float cy = func_80027C60(y);
    float sz = func_80032360(z);
    float cz = func_80027C60(z);
    matrix[0] = cy * cz;
    matrix[1] = cy * sz;
    matrix[2] = -sy;
    matrix[3] = 0;
    matrix[4] = sx * sy * cz - cx * sz;
    matrix[5] = sx * sy * sz + cx * cz;
    matrix[6] = sx * cy;
    matrix[7] = 0;
    matrix[8] = cx * cy * cz + sx * sz;
    matrix[9] = cx * cy * sz - sx * cz;
    matrix[10] = cx * cy;
    matrix[11] = 0;
    matrix[12] = 0;
    matrix[13] = 0;
    matrix[14] = 0;
    matrix[15] = 1.0f;
}
