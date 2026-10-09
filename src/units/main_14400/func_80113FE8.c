#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 pad00[0xC];
    u8 flag0C;
} Obj80113FE8;

s32 func_80113FE8(Obj80113FE8 *obj)
{
    return obj->flag0C != 0;
}
