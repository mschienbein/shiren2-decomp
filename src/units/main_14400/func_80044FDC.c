#include "common.h"

extern char D_80160B30[];
extern char D_00194FC0[];
extern char D_2023BE0[];
unsigned short *func_80044ECC(unsigned char, unsigned char);
void func_8006AC30(void *, void *, void *, s32, s32, s32);
char *func_80044FDC(unsigned char a, unsigned char b) {
    unsigned short *p = func_80044ECC(a, b);
    if (p == 0) return 0;
    {
        s32 n = *p - 1;
        func_8006AC30(D_80160B30, D_00194FC0, D_2023BE0, 0x18, n + b, 1);
    }
    return D_80160B30;
}
