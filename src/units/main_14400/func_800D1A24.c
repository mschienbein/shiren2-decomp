#include "common.h"
typedef unsigned char u8;
s32 func_800D1A24(u8 *cells) {
    s32 count = 0, row, column;
    row = 0;
    for (;;) {
        if (row < 5) {
            for (column = 0; column < 4; column++) {
                if (cells[column + (row << 2)] != 0) count++;
            }
            row++;
        } else return count;
    }
}
