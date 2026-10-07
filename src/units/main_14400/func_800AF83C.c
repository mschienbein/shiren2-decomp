#include "common.h"
extern void *const D_80153AE4[];
extern s32 D_80143094[];
void func_800AF8D0(void *);
void func_800AF910(void *, s32);
void func_800AF83C(void) {
    s32 i;
    for (i = 0; i < 7; i++) {
        func_800AF8D0(D_80153AE4[i]);
    }
    func_800AF910(D_80143094, 40);
}
