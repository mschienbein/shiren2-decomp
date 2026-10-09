#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 state;
    u8 walls;
    u8 x;
    u8 y;
} Cell;

/* Same whole-generator view as func_800B90E8: eleven rows of eight
 * four-byte cells at +0x13, the availability array at +0x174 and the
 * axis-edge pointers at +0x2D4/+0x2D8. */
typedef struct {
    u8 pad0[0x10];
    u8 rows;
    u8 columns;
    u8 wanted;
    Cell cells[11][8];
    u8 pad173;
    s32 available[11][8];
    u8 *rowEdges;
    u8 *colEdges;
} Grid;

extern u8 D_80147620[];
extern u8 D_80153C14[];
extern u8 D_80153C18[];
extern u8 D_80153C1C[];
extern u8 func_800C57CC(void *rng, s32 limit);
extern void func_800B90E8(Grid *grid, u8 row, u8 col);

/* Carve a random walk from (x, y): each step tries up to four directions
 * from a random start and opens a passage into the first unvisited (0x80)
 * neighbour, which becomes the new position. */
void func_800B8F94(Grid *grid, u8 x, u8 y) {
    u8 dir;
    u8 tries;
    u8 nx, ny;
    do {
        dir = func_800C57CC(D_80147620, 3);
        tries = 0;
        while (1) {
            if (++tries >= 5) break;
            nx = x + D_80153C14[dir];
            ny = y + D_80153C18[dir];
            if (grid->cells[nx][ny].state == 0x80) {
                grid->cells[x][y].walls |= D_80153C1C[dir];
                grid->cells[nx][ny].walls |= D_80153C1C[(dir + 2) & 3];
                grid->cells[nx][ny].state = 0x10;
                func_800B90E8(grid, nx, ny);
                x = nx;
                y = ny;
                break;
            }
            dir = (dir + 1) & 3;
        }
    } while (tries < 5);
}
