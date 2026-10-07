#include "common.h"

typedef unsigned char u8;
typedef signed char s8;

extern s32 D_80138BD4;
s32 D_80138BD8 = -1;
extern s8 D_80142F23;
extern u8 D_80142F1B;
extern s32 D_80138BAC;
s32 func_80052848(void);
s32 func_80046124(void);
void func_80045904(s32 arg0);
void func_800458BC(s32 arg0);

void func_8004569C(void) {
    if (D_80138BD4 != 0 || func_80052848() == 0) {
        if (D_80138BD8 == -1) {
            D_80138BD8 = D_80142F23;
        } else if (D_80138BD8 == 0x44) {
            D_80138BD8 = func_80046124();
            D_80142F23 = D_80138BD8;
        }
        func_80045904(D_80138BD8);
        func_800458BC(D_80138BD8);
    }
    if ((D_80142F1B >> 2) & 1) {
        D_80138BAC = 1;
    }
}
