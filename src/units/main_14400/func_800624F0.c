#include "common.h"

/* 76 x 54 cell-flag grid (row stride 216 bytes), shared with func_80062684. */
extern s32 D_801D9358[][54];
extern void func_8007D8C8(void);

void func_800624F0(void) {
    s32 row;
    s32 column;
    s32 *word;

    func_8007D8C8();
    for (row = 9; row < 68; row++) {
        word = &D_801D9358[row][9];
        for (column = 9; column < 46; column++) {
            *word |= 0x00800000;
            word++;
        }
    }
}
