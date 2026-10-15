#include "common.h"

typedef struct { unsigned char kind, variant, field_02, flags, row, field_05; unsigned short field_06; unsigned char mode; signed char coordinates[2], status; } SelectionRecord;
extern SelectionRecord D_80142F18;

/* Owned by func_80045754.c: four initialized bytes at 0x80138BD4.
 * This file owns D_80138BD8, the next four bytes, instead. */
extern s32 D_80138BD4;
s32 D_80138BD8 = -1;
extern s32 D_80138BDC;
extern s32 D_80138BAC;
s32 func_80052848(void);
s32 func_80046124(void);
void func_80045904(s32 arg0);
void func_800458BC(s32 arg0);
void func_80051CE8(void);

void func_80045650(void)
{
    D_80138BD4 = 1;
    D_80138BD8 = D_80138BDC = -1;
    func_80045904(-1);
    func_800458BC(-1);
    func_80051CE8();
}

void func_8004569C(void)
{
    if (D_80138BD4 != 0 || func_80052848() == 0) {
        if (D_80138BD8 == -1) {
            D_80138BD8 = D_80142F18.status;
        } else if (D_80138BD8 == 0x44) {
            D_80138BD8 = func_80046124();
            D_80142F18.status = D_80138BD8;
        }
        func_80045904(D_80138BD8);
        func_800458BC(D_80138BD8);
    }
    if ((D_80142F18.flags >> 2) & 1) {
        D_80138BAC = 1;
    }
}
