#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;
typedef float f32;

extern u32 D_801D9358[0x1008];
extern u8 D_80169AFC[0x1008];
typedef struct { u8 value; u8 flags; } Cell;
/* 0x1008 two-byte cells: 76 rows of 54, initialized below in flat order. */
typedef union {
    Cell rows[76][54];
    u8 pairs[0x1008][2];
    signed char bytes[0x2010];
} MapCells;
extern MapCells D_8016AB04;
extern void func_8007D8DC(s32 value);

void func_80061724(void) {
    u32 *color = D_801D9358;
    u8 *flag = D_80169AFC;
    u8 (*pair)[2] = D_8016AB04.pairs;
    s32 i = 0x1007;

    do {
        *color++ = 0x80C020;
        *flag++ = 0;
        (*pair)[0] = 0;
        (*pair)[1] = 0;
        pair++;
    } while (i-- != 0);
    func_8007D8DC(0);
}
