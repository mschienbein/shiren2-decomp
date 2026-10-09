#include "common.h"

typedef unsigned char u8;

typedef struct { u8 value; u8 flags; } Cell;
/* 0x1008 two-byte cells, initialized by func_80061724. */
typedef union {
    Cell rows[76][54];
    u8 pairs[0x1008][2];
    signed char bytes[0x2010];
} MapCells;
extern MapCells D_8016AB04;

/* Callers (func_80078908) use the zero-extended byte without narrowing it
 * again, so the result is full width (as func_800649C0). */
s32 func_80064990(s32 row, s32 col) {
    return D_8016AB04.rows[row][col].value;
}
