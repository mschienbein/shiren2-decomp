#include "common.h"

typedef struct {
    s32 m[4][4];
} Mtx;

void func_8002CE20(float mf[4][4]);
void func_8002CD40(float mf[4][4], Mtx *m);

void func_8002D0A0(float mf[4][4], float l, float r, float b, float t, float n, float f, float scale)
{
    int i, j;

    func_8002CE20(mf);
    mf[0][0] = 2 / (r - l);
    mf[1][1] = 2 / (t - b);
    mf[2][2] = -2 / (f - n);
    mf[3][0] = -(r + l) / (r - l);
    mf[3][1] = -(t + b) / (t - b);
    mf[3][2] = -(f + n) / (f - n);
    mf[3][3] = 1;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            mf[i][j] *= scale;
        }
    }
}

void func_8002D1D0(Mtx *m, float l, float r, float b, float t, float n, float f, float scale)
{
    float mf[4][4];

    func_8002D0A0(mf, l, r, b, t, n, f, scale);
    func_8002CD40(mf, m);
}
