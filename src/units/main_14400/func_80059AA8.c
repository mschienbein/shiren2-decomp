#include "common.h"
extern float D_80165438[2];
extern float D_80165440[2];
void func_80059AA8(float *v) {
    if (v[0] < D_80165438[0]) v[0] = D_80165438[0];
    else if (D_80165438[1] < v[0]) v[0] = D_80165438[1];
    if (v[2] < D_80165440[0]) v[2] = D_80165440[0];
    else if (D_80165440[1] < v[2]) v[2] = D_80165440[1];
}
