#include "common.h"

typedef struct {
    s32 m[4][4];
} Mtx;

void func_8002CE80(float mf[4][4], Mtx *m);
void func_8002CF00(float mf[4][4], float x, float y, float z, float *ox, float *oy, float *oz);

void func_8002CFB0(Mtx *m, float x, float y, float z, float *ox, float *oy, float *oz)
{
    float mf[4][4];

    func_8002CE80(mf, m);
    func_8002CF00(mf, x, y, z, ox, oy, oz);
}
