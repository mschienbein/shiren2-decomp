#include "common.h"
typedef unsigned char u8;
typedef struct { u8 value; u8 flags; } Cell;
/* 0x1008 two-byte cells, initialized by func_80061724. */
typedef union {
    Cell rows[76][54];
    u8 pairs[0x1008][2];
    signed char bytes[0x2010];
} MapCells;
extern u8 D_80169AFC[][54];
extern MapCells D_8016AB04;
extern unsigned char func_800416F8(s32, s32);
extern s32 func_80041DBC(s32 arg0);
extern s32 func_80042214(s32 a, s32 b);
void func_800621C4(s32 firstY, s32 firstX, s32 lastY, s32 lastX) {
    s32 y;
    s32 x;
    for (y = firstY; y <= lastY; y++) {
        u8 *tile;
        Cell *cell;
        x = firstX;
        tile = &D_80169AFC[y][x];
        cell = &D_8016AB04.rows[y][x];
        for (; x <= lastX; x++, tile++, cell++) {
            if ((*tile = func_800416F8(y, x)) &&
                func_80041DBC(*tile) == 16 && *tile == 0xD5) {
                cell->value = func_80042214(y, x);
            }
        }
    }
}
