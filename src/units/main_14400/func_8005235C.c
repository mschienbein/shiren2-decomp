#include "common.h"

typedef unsigned char u8;

extern u8 D_8016166E;
extern s32 D_80161648;
void func_80052B50(s32 a);
void func_800533B8(void);
void func_80052CF8(void);
void func_8005235C(void) {
    if (D_8016166E == 0) {
        func_80052B50(D_80161648);
        func_800533B8();
        func_80052CF8();
    }
}
