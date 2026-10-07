#include "common.h"

typedef unsigned short u16;
typedef struct {
    s32 m[4][4];
} Mtx;

void func_8002CE20(float mf[4][4]);
void func_8002CD40(float mf[4][4], Mtx *m);
float func_80027C60(float x);
float func_80032360(float x);

void func_8002D380(float mf[4][4], u16 *perspNorm, float fovy, float aspect, float near, float far, float scale)
{
    float cot;
    int i, j;

    func_8002CE20(mf);
    fovy *= 3.1415926 / 180.0;
    cot = func_80027C60(fovy / 2) / func_80032360(fovy / 2);
    mf[0][0] = cot / aspect;
    mf[1][1] = cot;
    mf[2][2] = (near + far) / (near - far);
    mf[2][3] = -1;
    mf[3][2] = (2 * near * far) / (near - far);
    mf[3][3] = 0;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            mf[i][j] *= scale;
        }
    }
    if (perspNorm != 0) {
        if (near + far <= 2.0) {
            *perspNorm = 65535;
        } else {
            *perspNorm = (u16)((double)(1 << 17) / (near + far));
            if (*perspNorm <= 0) {
                *perspNorm = 1;
            }
        }
    }
}

void func_8002D530(Mtx *m, u16 *perspNorm, float fovy, float aspect, float near, float far, float scale)
{
    float mf[4][4];

    func_8002D380(mf, perspNorm, fovy, aspect, near, far, scale);
    func_8002CD40(mf, m);
}
