#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct {
    u8 pad0[0x7C];
    u16 flags7C;
} Obj800F3C88;

s32 func_800F3C88(Obj800F3C88 *obj)
{
    return (obj->flags7C >> 4) & 1;
}
