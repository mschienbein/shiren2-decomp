/* libultra gu/rotate.c: guRotateF + guRotate (built -O3; guRotate inlines guRotateF). */
#include "common.h"

typedef float Matrix[4][4];
typedef struct {
    s32 m[4][4];
} Mtx;

extern float func_80027C60(float angle); /* cosf */
extern float func_80032360(float angle); /* sinf */
extern void func_8002CD40(Matrix mf, Mtx *m); /* guMtxF2L */
extern void func_8002CE20(Matrix mf); /* guMtxIdentF */
extern void func_8002D040(float *x, float *y, float *z); /* guNormalize */

static float dtor = 3.1415926 / 180.0; /* D_800372E0 */

void func_80030E70(Matrix mf, float a, float x, float y, float z)
{
    float sine;
    float cosine;
    float ab, bc, ca, t;
    float xs, ys, zs;

    func_8002D040(&x, &y, &z);
    a *= dtor;
    sine = func_80032360(a);
    cosine = func_80027C60(a);
    t = (1 - cosine);
    ab = x * y * t;
    bc = y * z * t;
    ca = z * x * t;

    func_8002CE20(mf);

    xs = x * sine;
    ys = y * sine;
    zs = z * sine;

    t = x * x;
    mf[0][0] = t + cosine * (1 - t);
    mf[2][1] = bc - xs;
    mf[1][2] = bc + xs;

    t = y * y;
    mf[1][1] = t + cosine * (1 - t);
    mf[2][0] = ca + ys;
    mf[0][2] = ca - ys;

    t = z * z;
    mf[2][2] = t + cosine * (1 - t);
    mf[1][0] = ab - zs;
    mf[0][1] = ab + zs;
}

void func_80030FD0(Mtx *m, float a, float x, float y, float z)
{
    Matrix mf;

    func_80030E70(mf, a, x, y, z);
    func_8002CD40(mf, m);
}
