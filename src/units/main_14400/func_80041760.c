#include "common.h"
typedef struct { s32 x; s32 y; } Pos;
void *func_800B4D80(Pos *p);
s32 func_80041760(s32 y, s32 x)
{
    Pos position;
    position.y = y;
    position.x = x;
    return func_800B4D80(&position) != 0;
}
