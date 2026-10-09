#include "common.h"

typedef unsigned short u16;
typedef float f32;

extern u16 D_80169AE0, D_80169AE2, D_80169AE4, D_80169AE6, D_80169AE8, D_80169AEA;
extern u16 D_80169AEC, D_80169AEE, D_80169AF0, D_80169AF2, D_80169AF4, D_80169AF6;
/* Scroll progress: stored from an f32 here and read with lwc1 by func_80061AB8. */
extern f32 D_80169AF8;

void func_80061A20(f32 progress)
{
    D_80169AF8 = progress;
    if (progress >= 100.0) {
        D_80169AF8 = 0.0f;
        D_80169AE0 = D_80169AEC;
        D_80169AE2 = D_80169AEE;
        D_80169AE4 = D_80169AF0;
        D_80169AE6 = D_80169AF2;
        D_80169AE8 = D_80169AF4;
        D_80169AEA = D_80169AF6;
    }
}
