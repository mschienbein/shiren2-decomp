#include "common.h"

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;

/* Owned .data block 0x801397D8..0x801397E0 (original bytes 01 50 06 00 00 00 00 00). */
u8 D_801397D8 = 0x01;
u8 D_801397D9 = 0x50;
u8 D_801397DA = 0x06;
u8 D_801397DB = 0;
u8 D_801397DC = 0;
u8 D_801397DD = 0;
u8 D_801397DE = 0;
u8 D_801397DF = 0;
extern u8 D_80161670;

void func_80052C34(void);
void func_80052C90(void);
void func_800534D4(void);
s32 func_8006C570(void);
void func_800532F0(u8 scale);

void func_80052BB0(void) {
    func_80052C34();
    if (D_801397DB) {
        func_80052C90();
        func_800534D4();
        D_801397DB = 0;
    } else {
        D_801397DB = 1;
    }
    if (func_8006C570() && !D_80161670) {
        func_800532F0(3);
        D_80161670 = 1;
    }
}
