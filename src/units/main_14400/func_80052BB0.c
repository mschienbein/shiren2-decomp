#include "common.h"

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;

/* Owned .data: the toggle byte D_801397DB (original value 0). The four zero bytes
 * 0x801397DC..0x801397DF that follow are not an object of this file: GCC 2.8.1's
 * MIPS DATA_ALIGNMENT word-aligns every array/struct, so no aggregate can begin at
 * the odd address 0x801397DB; they stay in the unowned data that leads up to the
 * table D_801397E0.
 * The preceding defaults D_801397D9/DA belong to func_80051E9C, whose
 * call delay slot reads them; this function never references those defaults. */
u8 D_801397DB = 0;
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
