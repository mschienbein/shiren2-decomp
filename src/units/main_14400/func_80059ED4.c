#include "common.h"

typedef unsigned char u8;
typedef float f32;
extern f32 D_8016530C[3];
extern f32 D_80165300[3];
extern u8 D_8013B140[];
extern s32 D_80165394;
void func_80059ED4(f32 x, f32 y, f32 z) {
    f32 *vec = D_8016530C;
    if (D_8013B140[D_80165394] == 0) {
        vec = D_80165300;
    }
    vec[0] += x;
    vec[1] += y;
    vec[2] += z;
}
