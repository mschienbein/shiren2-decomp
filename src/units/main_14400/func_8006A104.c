#include "common.h"

typedef unsigned char u8;
u8 D_8013CA00 = 0;
u8 D_8013CA01 = 0;
u8 D_8013CA02 = 0;
u8 D_8013CA03 = 0;
void func_8006A104(s32 r, s32 g, s32 b, s32 a) {
    s32 t;
    s32 v1;
    s32 v2;
    s32 v3;

    t = D_8013CA00 + r;
    r = (t < 0) ? 0 : ((t > 0xFF) ? 0xFF : t);
    D_8013CA00 = r;
    v1 = D_8013CA01 + g;
    t = (v1 < 0) ? 0 : ((v1 > 0xFF) ? 0xFF : v1);
    D_8013CA01 = t;
    v2 = D_8013CA02 + b;
    t = (v2 < 0) ? 0 : ((v2 > 0xFF) ? 0xFF : v2);
    D_8013CA02 = t;
    v3 = D_8013CA03 + a;
    t = (v3 < 0) ? 0 : ((v3 > 0xFF) ? 0xFF : v3);
    D_8013CA03 = t;
}
