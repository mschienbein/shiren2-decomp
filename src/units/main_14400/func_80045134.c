#include "common.h"
typedef unsigned char u8;
extern char D_80160B50[], D_00194FC0[], D_2025EF8[];
extern s32 func_800A3B00(u8);
extern void func_8006AC30(void *, const void *, const void *, s32, s32, s32);
void *func_80045134(u8 index) {
    if (func_800A3B00(index)) {
        func_8006AC30(D_80160B50, D_00194FC0, D_2025EF8, 4, index - 0x57, 1);
        return D_80160B50;
    }
    return 0;
}
