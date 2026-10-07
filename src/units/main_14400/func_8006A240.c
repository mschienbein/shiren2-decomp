#include "common.h"

extern s32 D_8016FD50;
extern s32 D_8016FD54;

void func_8006A240(s32 *outA, s32 *outB) {
    if (outA != 0) {
        *outA = D_8016FD50;
    }
    if (outB != 0) {
        *outB = D_8016FD54;
    }
}
