#include "common.h"

extern s32 func_800CBD2C(void);
extern void func_800CC16C(void);
extern void func_800CC248(void);

s32 func_800CCD80(void) {
    s32 active = func_800CBD2C() != 0;
    if (active) {
        func_800CC16C();
    }
    func_800CC248();
    return active;
}
