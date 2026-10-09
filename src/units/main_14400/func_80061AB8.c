#include "common.h"
extern short D_80169AE0, D_80169AE2, D_80169AE4, D_80169AE6, D_80169AE8, D_80169AEA;
extern short D_80169AEC, D_80169AEE, D_80169AF0, D_80169AF2, D_80169AF4, D_80169AF6;
extern float D_80169AF8;
void func_80061AB8(float *a, float *b, float *c, float *d, float *e, float *f) {
    float va = D_80169AE0 * 32.0 + 16.0;
    float vb = D_80169AE2 * 32.0 + 16.0;
    float vc = D_80169AE4 * 32.0 + 16.0;
    float vd = D_80169AE6 * 32.0 + 16.0;
    float ve = D_80169AE8 * 32.0 + 16.0;
    float vf = D_80169AEA * 32.0 + 16.0;
    if (D_80169AF8 != 0.0) {
        float factor = (D_80169AF8 / 100.0f) * 32.0;
        va += (D_80169AEC - D_80169AE0) * factor;
        vb += (D_80169AEE - D_80169AE2) * factor;
        vc += (D_80169AF0 - D_80169AE4) * factor;
        vd += (D_80169AF2 - D_80169AE6) * factor;
        ve += (D_80169AF4 - D_80169AE8) * factor;
        vf += (D_80169AF6 - D_80169AEA) * factor;
    }
    if (a) *a = va;
    if (b) *b = vb;
    if (c) *c = vc;
    if (d) *d = vd;
    if (e) *e = ve;
    if (f) *f = vf;
}
