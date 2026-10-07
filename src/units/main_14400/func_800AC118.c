#include "common.h"

extern unsigned char *D_8015380C[];
extern unsigned char D_80153734[];
void func_800AD3A0(void);
void func_800AD48C(void);
void func_800B0B10(s32);
void func_800AC118(void) {
    s32 i;
    func_800AD3A0();
    func_800AD48C();
    for (i = 20;; i--) {
        unsigned char *list;
        s32 j;
        if (i <= 0) break;
        list = D_8015380C[i];
        if (list == 0) continue;
        for (j = D_80153734[i] - 1; j >= 0; j--) {
            func_800B0B10(list[j]);
            list[j] = 0;
        }
    }
}
