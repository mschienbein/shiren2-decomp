#include "common.h"

typedef struct Pos Pos;

extern u32 func_800B1C6C(Pos *pos);

s32 func_800B200C(Pos *pos)
{
    if (!(func_800B1C6C(pos) & 0x1000)) {
        return -1;
    }
    return func_800B1C6C(pos) & 0xF;
}
