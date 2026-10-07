#include "common.h"
extern char D_801653A0[];
extern char D_801653CC[];
extern u32 D_801653F8;
extern u32 D_801653FC;
extern s32 D_80165394;
extern s32 D_80165404;
extern s32 D_80165400;
extern unsigned char D_8013B140[];
void func_8005BF7C(void *, void *, u32, u32, s32, s32);
void func_8005BBD4(void) {
    if (D_801653F8 != 0) {
        D_801653FC++;
        func_8005BF7C(D_801653A0, D_801653CC, D_801653FC, D_801653F8, D_80165404, D_8013B140[D_80165394]);
        if (D_801653FC >= D_801653F8) {
            D_801653F8 = 0;
            D_80165400 = 0;
        }
    }
}
