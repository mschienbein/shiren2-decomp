#include "common.h"
extern unsigned char D_80169AFC[];
typedef unsigned char u8;
typedef struct { u8 value; u8 flags; } Cell;
/* 0x1008 two-byte cells, initialized by func_80061724. */
typedef union {
    Cell rows[76][54];
    u8 pairs[0x1008][2];
    signed char bytes[0x2010];
} MapCells;
extern MapCells D_8016AB04;
extern unsigned char func_800416F8(s32, s32), func_80041DDC(s32, s32);
extern s32 func_80041DBC(s32);
/* Full-int 0/1 result (func_8008AA20 tests it unnarrowed); narrowed into the state byte. */
extern s32 func_800421D4(s32, s32);
/* func_800423D0 returns its 0..3 class as a full int (move v0,a0 at 0x80042438);
 * this caller narrows it into the signed-char state byte (sb at 0x800620F4). */
extern s32 func_800423D0(s32, s32);
extern s32 func_80042260(s32 x, s32 y);
extern s32 func_800422B0(s32, s32);
extern unsigned char func_80042444(s32 x, s32 y);
void func_80061FB8(s32 x0, s32 y0, s32 x1, s32 y1) {
    s32 x = x0, y, rowState, rowTile;
    if (x <= x1) {
        rowState = x * 108;
        rowTile = x * 54;
        do {
        unsigned char *tile;
        signed char *state;
        u32 extraAddress;
        y = y0;
        /* local-arithmetic-qualification: plain pointer addition reassociates the
         * row and column sums in GCC 2.8.1. Keep the original addition order.
         * The flags cursor is an integer address so its final +2 does not form
         * a pointer past the array's one-past address; casts occur only at valid
         * cell accesses. No integer address escapes this function. */
        { unsigned char *tileBase = y + D_80169AFC; signed char *stateBase = y * 2 + D_8016AB04.bytes;
          tile = (unsigned char *)(rowTile + (s32)tileBase);
          state = (signed char *)(rowState + (s32)stateBase);
        }
        if (y <= y1) {
        extraAddress = (u32)state + 1;
        do {
            *tile = func_800416F8(x, y);
            *(signed char *)extraAddress = 0;
            if (*tile) {
                switch (func_80041DBC(*tile)) {
                case 2: *state = (signed char)func_800421D4(x, y); break;
                case 20:
                    switch (*tile) {
                    case 0xF5: *state = (signed char)func_800423D0(x, y); break;
                    case 0xF6: *state = (signed char)func_80042444(x, y); break;
                    default: *state = 0; break;
                    }
                    break;
                case 16:
                    switch (*tile) {
                    case 0xD5: break;
                    case 0xE7: *state = (signed char)func_80042260(x, y); break;
                    default: *state = 0; break;
                    }
                    *(signed char *)extraAddress = (signed char)func_800422B0(x, y); break;
                case 19:
                    if (*tile == 0xF2) { *tile = func_80041DDC(x, y); *state = 0; }
                    break;
                default: *state = 0; break;
                }
            }
            y++; tile++; extraAddress += 2; state += 2;
        } while (y <= y1);
        }
        rowState += 108; x++; rowTile += 54;
        } while (x <= x1);
    }
}
