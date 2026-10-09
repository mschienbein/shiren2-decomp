#include "common.h"

typedef unsigned short u16;

/* Adjacent setter/getter of one initialized halfword. */
u16 D_8013E82E = 0;

void func_80083724(s32 value) {
    D_8013E82E = value;
}

/* Both original callers (0x80054048 in func_80053D70, 0x80054308 in func_80054258) test v0 as a
 * full word with no andi: the zero-extended halfword is returned at full width. */
s32 func_80083730(void) {
    return D_8013E82E;
}
