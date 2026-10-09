#include "common.h"

/* Direction lookup by the signs of a step: row = sign(x) (-,0,+ -> 0,3,6),
 * column = sign(y) (-,0,+ -> 0,1,2). */
s32 D_80142980[9] = { 3, 2, 1, 4, 0, 0, 5, 6, 7 };

s32 func_800A21B0(s32 y, s32 x)
{
    s32 column;
    s32 row;

    column = y == 0 ? 1 : (y > 0) * 2;
    row = x == 0 ? 3 : (x > 0 ? 6 : 0);
    return D_80142980[row + column];
}
