#include "common.h"

typedef struct {
    s32 m[4][4];
} Mtx;

void func_8002CE80(float mf[4][4], Mtx *m);
void func_8002CBC0(float mf[4][4], float nf[4][4], float res[4][4]);
void func_8002CD40(float mf[4][4], Mtx *m);

void func_8002CCC0(Mtx *m, Mtx *n, Mtx *res)
{
    float mf[4][4], nf[4][4], resf[4][4];

    func_8002CE80(mf, m);
    func_8002CE80(nf, n);
    func_8002CBC0(mf, nf, resf);
    func_8002CD40(resf, res);
}
