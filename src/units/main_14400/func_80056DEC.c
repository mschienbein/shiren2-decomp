#include "common.h"

extern s32 func_8006D44C(s32 arg0, s32 arg1);

void func_80056DEC(void) {
    while (func_8006D44C(0, -1) != 2) {
    }
}
