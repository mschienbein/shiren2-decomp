#include "common.h"
typedef unsigned char u8;
/* Map cells are 4 bytes wide, 8 per row; the cell at (x, y) starts at
 * byte (x * 32 + y * 4) and its kind/mask bytes sit at +0x13/+0x14. */
typedef struct { char pad[0x13]; u8 kind; u8 mask; } CellView800B89D8;
extern u8 D_80153C14[];
extern u8 D_80153C18[];
extern u8 D_80153C1C[];
void func_800B90E8(void *, u8, u8);
s32 func_800B89D8(void *map, u8 x, u8 y, u8 dir) {
    s32 i;
    u8 nx;
    u8 ny;
    CellView800B89D8 *from;
    CellView800B89D8 *to;

    for (i = 0; dir != D_80153C1C[i]; i++) {
    }
    nx = x + D_80153C14[i];
    ny = y + D_80153C18[i];
    from = (CellView800B89D8 *)((u8 *)map + (y * 4 + x * 32));
    if (from->kind == 0x40) {
        return 0;
    }
    to = (CellView800B89D8 *)((u8 *)map + (ny * 4 + nx * 32));
    if (to->kind == 0x40) {
        return 0;
    }
    if (from->kind == 0x80) {
        from->kind = 0x10;
        func_800B90E8(map, x, y);
    }
    if (to->kind == 0x80) {
        to->kind = 0x10;
        func_800B90E8(map, nx, ny);
    }
    from->mask |= D_80153C1C[i];
    to->mask |= D_80153C1C[(i + 2) & 3];
    return 1;
}
