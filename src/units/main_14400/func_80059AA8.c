#include "common.h"
/* One 16-byte bounds object {min x, max x, min z, max z}: func_800598B4 stores all
 * four floats from the single D_80165438 base (+0x0/+0x4/+0x8/+0xC). */
extern float D_80165438[4];
void func_80059AA8(float *v) {
    if (v[0] < D_80165438[0]) v[0] = D_80165438[0];
    else if (D_80165438[1] < v[0]) v[0] = D_80165438[1];
    if (v[2] < D_80165438[2]) v[2] = D_80165438[2];
    else if (D_80165438[3] < v[2]) v[2] = D_80165438[3];
}
