#include "common.h"

extern float *D_8013B804;
extern s32 D_801D9358[][54];
void func_80062430(s32 x0, s32 y0, s32 x1, s32 y1) {
    s32 x, y;
    float *dst;
    if (D_8013B804 == 0) return;
    for (y = y0; y <= y1; y++) {
        x = x0;
        dst = &D_8013B804[y * 56 + x];
        for (; x <= x1; x++, dst++) {
            if (D_801D9358[x + 10][y + 10] & 0x2000) {
                *dst = -32.0f;
            } else {
                *dst = 0.0f;
            }
        }
    }
}
