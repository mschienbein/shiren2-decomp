#include "common.h"
typedef unsigned char u8;
/* ROM 0x8BAA4/0x8BAC8 accesses +0x13 + x*32 + y*4.
 * These are members of the complete grid, not shifted object headers. */
typedef struct { u8 kind, mask, x, y; } Cell;
typedef struct {
    u8 pad0[0x13];
    Cell cells[11][8];
    u8 pad173;
    s32 available[11][8];
} Grid;
extern u8 D_80153C14[];
extern u8 D_80153C18[];
extern u8 D_80153C1C[];
void func_800B90E8(void *, u8, u8);
s32 func_800B89D8(Grid *map, u8 x, u8 y, u8 dir) {
    s32 i;
    u8 nx;
    u8 ny;

    for (i = 0; dir != D_80153C1C[i]; i++) {
    }
    nx = x + D_80153C14[i];
    ny = y + D_80153C18[i];
    if (map->cells[x][y].kind == 0x40) {
        return 0;
    }
    if (map->cells[nx][ny].kind == 0x40) {
        return 0;
    }
    if (map->cells[x][y].kind == 0x80) {
        map->cells[x][y].kind = 0x10;
        func_800B90E8(map, x, y);
    }
    if (map->cells[nx][ny].kind == 0x80) {
        map->cells[nx][ny].kind = 0x10;
        func_800B90E8(map, nx, ny);
    }
    map->cells[x][y].mask |= D_80153C1C[i];
    map->cells[nx][ny].mask |= D_80153C1C[(i + 2) & 3];
    return 1;
}
