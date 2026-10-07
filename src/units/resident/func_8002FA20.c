#include "common.h"

/* libultra gu/position.c: guPositionF + guPosition */

typedef float Matrix[4][4];
typedef struct {
    s32 m[4][4];
} Mtx;

extern float func_80027C60(float angle); /* cosf */
extern float func_80032360(float angle); /* sinf */
extern void func_8002CD40(Matrix mf, Mtx *m); /* guMtxF2L */

static float dtor = 3.1415926 / 180.0; /* D_800372B0 */

void func_8002FA20(Matrix mf, float r, float p, float h, float s, float x, float y, float z)
{
    float sinr, sinp, sinh, cosr, cosp, cosh;

    r *= dtor;
    p *= dtor;
    h *= dtor;
    sinr = func_80032360(r);
    cosr = func_80027C60(r);
    sinp = func_80032360(p);
    cosp = func_80027C60(p);
    sinh = func_80032360(h);
    cosh = func_80027C60(h);

    mf[0][0] = (cosp * cosh) * s;
    mf[0][1] = (cosp * sinh) * s;
    mf[0][2] = (-sinp) * s;
    mf[0][3] = 0.0;

    mf[1][0] = (sinr * sinp * cosh - cosr * sinh) * s;
    mf[1][1] = (sinr * sinp * sinh + cosr * cosh) * s;
    mf[1][2] = (sinr * cosp) * s;
    mf[1][3] = 0.0;

    mf[2][0] = (cosr * sinp * cosh + sinr * sinh) * s;
    mf[2][1] = (cosr * sinp * sinh - sinr * cosh) * s;
    mf[2][2] = (cosr * cosp) * s;
    mf[2][3] = 0.0;

    mf[3][0] = x;
    mf[3][1] = y;
    mf[3][2] = z;
    mf[3][3] = 1.0;
}

void func_8002FBF8(Mtx *m, float r, float p, float h, float s, float x, float y, float z)
{
    Matrix mf;

    func_8002FA20(mf, r, p, h, s, x, y, z);
    func_8002CD40(mf, m);
}
