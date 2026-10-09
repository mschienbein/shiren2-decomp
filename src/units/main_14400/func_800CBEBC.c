#include "common.h"
extern s32 func_800CBDB0(s32, s32);
extern void func_800CA0A8(void *, s32);
extern void *D_80147F44;
extern unsigned char D_801476D0;
void func_800CBEBC(unsigned char first, unsigned char second) {
    s32 value = func_800CBDB0(first, second);
    func_800CA0A8(D_80147F44, value);
    D_801476D0 = second;
}
