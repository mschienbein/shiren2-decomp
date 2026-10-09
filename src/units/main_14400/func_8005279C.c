#include "common.h"

typedef unsigned char u8;

/* Sole definition of the one-byte mode flag at 0x801397D8; both functions of
 * this original file address it in delay slots (lbu in func_8005279C's jr slot,
 * sb/lbu in func_800527A8's j/jal slots), which gas fills only for a symbol
 * defined in the same file.  Of the following seven bytes, 0x801397D9..DA are
 * func_80051E9C's defaults, 0x801397DB is func_80052BB0's toggle, and
 * 0x801397DC..DF are unowned asm data. */
u8 D_801397D8 = 1;

void func_8012EA50(u8 mode);

s32 func_8005279C(void) {
    return D_801397D8;
}

void func_800527A8(s32 mode) {
    if (mode == 1) {
        D_801397D8 = mode;
    } else {
        D_801397D8 = 0;
    }
    func_8012EA50(D_801397D8);
}
