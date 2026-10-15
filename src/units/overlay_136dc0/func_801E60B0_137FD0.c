#include "func_801E60B0_137FD0/types.h"

void func_801E60B0_137FD0(BytePair *position, u8 x, u8 y)
{
    if (x != 0) position->x = x;
    else position->x = D_8014BF98.x;
    if (y != 0x80) position->y = y;
    else position->y = D_8014BF98.y;
}
