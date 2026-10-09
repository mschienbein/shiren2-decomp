#include "common.h"

typedef unsigned char u8;

/* Copies the record's up-to-4-character code at +0x70 into a static NUL-padded
 * 5-byte buffer and returns it. */
u8 *func_8009C17C(u8 *record)
{
    static u8 D_801C3580[5];
    s32 i;
    u8 *src = record + 0x70;

    for (i = 0; i < 4; i++, src++) {
        if (*src == 0) break;
        D_801C3580[i] = *src;
    }
    for (; i < 5; i++) D_801C3580[i] = 0;
    return D_801C3580;
}
