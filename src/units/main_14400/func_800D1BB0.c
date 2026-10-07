#include "common.h"

typedef struct {
    unsigned char id[5][4];     /* 0x00 */
    unsigned char extra[5][4];  /* 0x14 */
} Grid;
void func_800D1BB0(Grid *g) {
    s32 pass, row, col;
    for (pass = 0; pass < 3; pass++) {
        for (row = 0; row < 5; row++) {
            for (col = 0; col < 3; col++) {
                if (g->id[row][col] == 0 && g->id[row][col + 1] != 0) {
                    g->id[row][col] = g->id[row][col + 1];
                    g->id[row][col + 1] = 0;
                    g->extra[row][col] = g->extra[row][col + 1];
                }
            }
        }
    }
}
