#include "common.h"

typedef unsigned char u8;
typedef short s16;

/* 0xB0-byte unit record of the D_801DEAB4 table (same prefix layout as
 * func_8007482C.c): kind at +2 (-1 = empty), field_4A at +0x4A. */
typedef struct {
    u8 pad0[0x2];
    s16 kind;
    u8 pad4[0x46];
    s16 field_4A;
    u8 pad4C[0x64];
} Unit;

extern Unit D_801DEAB4[];

/* The only caller (func_8007C324) passes the direction un-narrowed; only the
 * halfword is stored. */
s32 func_8007C1E0(s32 index, s32 value)
{
    if (D_801DEAB4[index].kind != -1) {
        D_801DEAB4[index].field_4A = value;
        return 0;
    }
    return -1;
}
